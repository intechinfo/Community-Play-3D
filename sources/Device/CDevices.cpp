/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include "CDevices.h"

CDevices::CDevices() {
    //DEVICE
	Device = 0;
	wolrdCore = new CCore();
    worldCoreData = new CCoreData();
	processesLogger = 0;
	scripting = 0;

    //RENDERS
    effect = 0;
	renderCallbacks = 0;

	renderScene = true;

    renderXEffect = false;
    renderGUI = true;

    editBoxEntered = false;

    window = 0;

    projectName = L"";
	contextName = L"";

    ctrlWasPushed = false;
    shiftWasPushed = false;

	skydome = 0;
	skybox = 0;

	sceneManagerToDrawIndice = 0;

}

CDevices::~CDevices() {
	/*gui->drop();

    smgr->drop();
	effectSmgr->drop();

	driver->drop();

	delete effect;
	delete worldCoreData;
	delete wolrdCore;*/
}

void CDevices::removeSceneManager(ISceneManager *smgrToDelete) {
	for (u32 i=0; i < smgrs.size(); i++) {
		if (smgrs[i] == smgrToDelete) {
			smgrs[i]->drop();
			delete smgrs[i];
			smgrs.erase(i);
			break;
		}
	}
}

void CDevices::setSceneManagerToDraw(ISceneManager *smgrToDraw) {
	for (u32 i=0; i < smgrs.size(); i++) {
		if (smgrs[i] == smgrToDraw) {
			sceneManagerToDrawIndice = i;
			break;
		}
	}
}

void CDevices::updateEntities() {
	//UPDATE CURSOR POSITION
    line3d<f32> ray;
	ray.start = smgr->getActiveCamera()->getPosition();
	ray.end = ray.start + (smgr->getActiveCamera()->getTarget() - ray.start).normalize() * 1000.0f;
	vector3df intersection;
	triangle3df hitTriangle;
	ISceneCollisionManager *collMan = smgr->getSceneCollisionManager();
	ISceneNode *selectedSceneNode = collMan->getSceneNodeAndCollisionPointFromRay(ray, intersection, hitTriangle, 1, 0);
	if(selectedSceneNode) {
		cursorBillBoard->setPosition(intersection);
        cursorBillBoard->setSize(dimension2d<f32>((ray.getLength()*1.0f)/300.f, (ray.getLength()*1.0f)/300.f));
	}
    
    objPlacement->refresh(cursorBillBoard);
    //UPDATE EFFECT LIGHTS
    for (u32 i=0; i < worldCoreData->getLightsData()->size(); i++) {
		effect->getShadowLight(i).setPosition(worldCoreData->getLightsData()->operator[](i).getNode()->getPosition());
        effect->getShadowLight(i).setTarget(worldCoreData->getLightsData()->operator[](i).getNode()->getRotation());
        
        effect->getShadowLight(i).setLightColor(SColorf
												 (((ILightSceneNode *)worldCoreData->getLightsData()->operator[](i).getNode())->getLightData().DiffuseColor.r,
                                                 ((ILightSceneNode *)worldCoreData->getLightsData()->operator[](i).getNode())->getLightData().DiffuseColor.g,
                                                 ((ILightSceneNode *)worldCoreData->getLightsData()->operator[](i).getNode())->getLightData().DiffuseColor.b,
                                                 255));
		if (worldCoreData->getLightsData()->operator[](i).isLightShaftsEnable()) {
			SLightsData ldata = worldCoreData->getLightsData()->operator[](i);

			ldata.getLightShaftsCallback()->lightCameraProjectionMatrix = effect->getShadowLight(i).getProjectionMatrix();
			ldata.getLightShaftsCallback()->lightCameraViewMatrix = effect->getShadowLight(i).getViewMatrix();
			ldata.getLightShaftsCallback()->uLightPosition = effect->getShadowLight(i).getPosition();

			ldata.getLightShaftsCallback()->lightCamera->setPosition(ldata.getNode()->getPosition());
			ldata.getNode()->addChild(ldata.getLightShaftsCallback()->lightCamera);
		}
    }
    
    //UPDATE LENS FLARE STRENGTHS
    for (u32 i=0; i < worldCoreData->getLightsData()->size(); i++) {
		if (worldCoreData->getLightsData()->operator[](i).getLensFlareSceneNode() != 0) {
			if (worldCoreData->getLightsData()->operator[](i).getLensFlareSceneNode()->getFalseOcclusion()) {
				driver->runAllOcclusionQueries(false);
				driver->updateAllOcclusionQueries(false);
				u32 occlusionQueryResult = driver->getOcclusionQueryResult(worldCoreData->getLightsData()->operator[](i).getLensFlareMeshSceneNode());
				if(occlusionQueryResult != 0xffffffff) {
					worldCoreData->getLightsData()->operator[](i).getLensFlareSceneNode()->setStrength(f32(occlusionQueryResult)/8000.f);
				}
				worldCoreData->getLightsData()->operator[](i).getLensFlareSceneNode()->updateAbsolutePosition();
            }
			ISceneNode *light = worldCoreData->getLightsData()->operator[](i).getNode();
			CLensFlareSceneNode *lfNode = worldCoreData->getLightsData()->operator[](i).getLensFlareSceneNode();
			line3d<f32> length(smgrs[sceneManagerToDrawIndice]->getActiveCamera()->getPosition(), light->getPosition());
			lfNode->setStrength(lfNode->getStrength()/(length.getLength()/worldCoreData->getLightsData()->operator[](i).getLensFlareStrengthFactor()));
        }
    }

	//UPDATING WATER
	for (u32 i=0; i < worldCoreData->getWaterSurfaces()->size(); i++) {
		if (renderXEffect) {
			if (effect->isUsingMotionBlur()) {
				worldCoreData->getWaterSurfaces()->operator[](i).getWaterSurface()->setOriginRTT(effect->getPostProcessMotionBlur()->getMaterial(0).TextureLayer[0].Texture);
			} else {
				worldCoreData->getWaterSurfaces()->operator[](i).getWaterSurface()->setOriginRTT(effect->getScreenQuad().rt[1]);
			}
		} else {
			worldCoreData->getWaterSurfaces()->operator[](i).getWaterSurface()->setOriginRTT(0);
		}
	}

}

void CDevices::updateDevice() {

	//#ifndef _IRR_OSX_PLATFORM_
	//	EnterCriticalSection(&CriticalSection);
	//#endif

	#ifndef _IRR_OSX_PLATFORM_
		if (renderScene) {
			this->drawScene();
		}
		this->drawGUI();
	#else
		if (renderScene) {
			if (renderXEffect) {
				effect->setActiveSceneManager(smgrs[sceneManagerToDrawIndice]);
				effect->update();
			} else {
				smgrs[sceneManagerToDrawIndice]->drawAll();
			}
		}
		effectSmgr->drawAll();

		if (renderGUI) {
			gui->drawAll();
		}
	#endif

	//camera_maya->setAspectRatio(1.f * driver->getScreenSize().Width / driver->getScreenSize().Height);
	//camera_fps->setAspectRatio(1.f * driver->getScreenSize().Width / driver->getScreenSize().Height);
	smgrs[sceneManagerToDrawIndice]->getActiveCamera()->setAspectRatio(1.f * driver->getScreenSize().Width / driver->getScreenSize().Height);

	//#ifndef _IRR_OSX_PLATFORM_
	//	LeaveCriticalSection(&CriticalSection);
	//#endif
}

void CDevices::reupdate(EffectHandler *_effect) {
	if (Device->run()) {

		EffectHandler *tempEffect = effect;
		if (_effect) {
			effect = _effect;
		}

		driver->beginScene(true, true, SColor(0x0));
		updateDevice();
		driver->endScene();

		if (_effect) {
			effect = tempEffect;
		}
	}
}

void CDevices::drawScene() {
	if (renderXEffect) {
		matrix4 viewProj;
		effect->setActiveSceneManager(smgrs[sceneManagerToDrawIndice]);
		effect->update(true);
    } else {
        smgrs[sceneManagerToDrawIndice]->drawAll();
    }

    effectSmgr->drawAll();
}

void CDevices::drawGUI() {
	if (renderGUI) {
        gui->drawAll();
    }
}

void CDevices::createDevice(SIrrlichtCreationParameters parameters) {
    //DEVICE

	Device = createDeviceEx(parameters);
    Device->setWindowCaption(L"Soganatsu Studios World Editor V1");
	Device->setResizable(true);
    
    driver = Device->getVideoDriver();
    
	smgr = Device->getSceneManager();
	smgrs.push_back(smgr);

    effectSmgr = smgr->createNewSceneManager();
    gui = Device->getGUIEnvironment();
    
    Device->setEventReceiver(&receiver);
    receiver.AddEventReceiver(this);
	receiver.setDriver(driver);
	receiver.setGUI(gui);

	wolrdCore->setDevice(Device);
    workingDirectory = Device->getFileSystem()->getWorkingDirectory().c_str();

	#ifdef _IRR_OSX_PLATFORM_
		workingDirectory += "/";
	#else
		workingDirectory += "/";
	#endif

	//DRAW SPLASH SCREEN
	ITexture *splashScreen = driver->getTexture("GUI/scs/sc1.png");
	driver->beginScene(true, true, SColor(0x0));
	driver->draw2DImage(splashScreen, rect<s32>(0, 0, 800, 600), rect<s32>(0, 0, 800, 600));
	driver->endScene();
	driver->removeTexture(splashScreen);
    
    //-----------------------------------
    //CAMERAS
    keyMap[0].Action = EKA_MOVE_FORWARD;
	keyMap[0].KeyCode = KEY_KEY_Z;
	keyMap[1].Action = EKA_MOVE_BACKWARD;
	keyMap[1].KeyCode = KEY_KEY_S;
	keyMap[2].Action = EKA_STRAFE_LEFT;
	keyMap[2].KeyCode = KEY_KEY_Q;
	keyMap[3].Action = EKA_STRAFE_RIGHT;
	keyMap[3].KeyCode = KEY_KEY_D;
	keyMap[4].Action = EKA_JUMP_UP;
	keyMap[4].KeyCode = KEY_SPACE;
    
	camera_fps = smgr->addCameraSceneNodeFPS(0, 200.0f, 0.09f, -1, keyMap, 5, true, 0.3f, false, true);
	camera_fps->setTarget(vector3df(0.f, 5.f, 0.f));
	camera_fps->setFarValue(42000.0f);
	camera_fps->setName("editor:FPScamera");
	camera_fps->setID(-1);
	
	camera_maya = smgr->addCameraSceneNodeMaya();
	camera_maya->setTarget(vector3df(0.0f,0.0f, 0.0f));
	camera_maya->setPosition(vector3df(50.0f, 50.0f, 50.0f));
	camera_maya->bindTargetAndRotation(true);
	camera_maya->setFarValue(42000.0f);
	camera_maya->setName("editor:MayaCamera");
	camera_maya->setID(-1);
	camera_maya->setAspectRatio(1.f * driver->getScreenSize().Width / driver->getScreenSize().Height);
    
	smgr->setActiveCamera(camera_maya);
	effectSmgr->setActiveCamera(camera_maya);
    
    cursorBillBoard = smgr->addBillboardSceneNode();
    cursorBillBoard->setName("editor:cursor");
	cursorBillBoard->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	cursorBillBoard->setMaterialTexture(0, driver->getTexture("GUI/particle.bmp"));
	cursorBillBoard->setMaterialFlag(EMF_LIGHTING, false);
	cursorBillBoard->setMaterialFlag(EMF_ZBUFFER, false);
	cursorBillBoard->setSize(dimension2d<f32>(1.0f, 1.0f));
	cursorBillBoard->setID(-1);

    //-----------------------------------

    //3D INTERACTION
    collisionManager = new CCollisionManager(smgr);
	objPlacement = new CCoreObjectPlacement(effectSmgr, Device->getCursorControl(), new CCollisionManager(effectSmgr), smgr);

	//INIT EFFECTS
    //effect = new EffectHandler(Device, dimension2du(1920, 1138), true, true, true);
	effect = new EffectHandler(Device, Device->getVideoModeList()->getDesktopResolution(), false, true, true);
    effect->setActiveSceneManager(smgr);
	filterType = EFT_4PCF;
	effect->setClearColour(SColor(0x0));
	effect->setAmbientColor(SColor(255, 64, 64, 64));
	effect->setUseMotionBlur(false);
	effect->setUseVSMShadows(false);
    shaderExt = (driver->getDriverType() == EDT_DIRECT3D9) ? ".hlsl" : ".glsl";
	setXEffectDrawable(true);
	effect->enableDepthPass(true);
	renderCallbacks = new CRenderCallbacks(effect, shaderExt, workingDirectory);
	effect->addShadowToNode(objPlacement->getGridSceneNode(), filterType, ESM_EXCLUDE);

	//ADD EVENTS
    receiver.AddEventReceiver(objPlacement);

	//ADVANCED GUI ASSETS
	processesLogger = new CUIProcessesLogger(gui);

	//SCRIPTING
	//RUN SCRIPTING
	scripting = new CScripting(this);
	scripting->initializeSceneScripting();
	scripting->initializeFileSystemScripting();
}

void CDevices::rebuildXEffect() {
    effect->setScreenRenderTargetResolution(driver->getScreenSize());
}

IGUIFileOpenDialog *CDevices::createFileOpenDialog(stringw title, IGUIElement *parent) {
    window = gui->addWindow(rect<s32>(100, 100, 960, 590), true, 
                            L"Open File Dialog", parent, -1);
    
	dialog = this->createFileOpenDialog(title, CGUIFileSelector::EFST_OPEN_DIALOG, window);
    dialog->setRelativePosition(rect<s32>(0, 20, 560, 440));
    
    gui->addButton(rect<s32>(320, 450, 430, 480), window, DEVICES_FILE_OPEN_DIALOG_EVENTS_OK, L"Ok", L"Accept");
    gui->addButton(rect<s32>(440, 450, 550, 480), window, DEVICES_FILE_OPEN_DIALOG_EVENTS_CLOSE, L"Close", L"Close this window");
    dialogPreview = gui->addImage(rect<s32>(570, 20, 1260-410, 440), window, -1, L"Image Preview (Double Click To View)");

	return dialog;
}

CGUIFileSelector *CDevices::createFileOpenDialog(stringw title, CGUIFileSelector::E_FILESELECTOR_TYPE type) {
	return this->createFileOpenDialog(title, type, gui->getRootGUIElement());
}

CGUIFileSelector *CDevices::createFileOpenDialog(stringw title, CGUIFileSelector::E_FILESELECTOR_TYPE type, IGUIElement *parent) {
    CGUIFileSelector* selector = new CGUIFileSelector(title.c_str(), gui, parent, 1, type);
    selector->setRelativePosition(rect<s32>(0, 20, 560, 440));
    
    selector->setCustomFileIcon(driver->getTexture(workingDirectory + "GUI/file.png"));
    selector->setCustomDirectoryIcon(driver->getTexture(workingDirectory + "GUI/folder.png"));
    
    return selector;
}

IGUIWindow *CDevices::addInformationDialog(stringw title, stringw text, s32 flag, bool modal, IGUIElement *parent) {
    IGUIWindow *window = gui->addMessageBox(title.c_str(), text.c_str(), modal, flag, parent, -1, 
											gui->getVideoDriver()->getTexture(workingDirectory + stringc("/GUI/info.png")));
	return window;
}

IGUIWindow *CDevices::addErrorDialog(stringw title, stringw text, s32 flag) {
    IGUIWindow *window = gui->addMessageBox(title.c_str(), text.c_str(), true, flag, 0, -1, 
                       gui->getVideoDriver()->getTexture(workingDirectory + stringc("/GUI/error.png")));
	return window;
}

IGUIWindow *CDevices::addWarningDialog(stringw title, stringw text, s32 flag) {
    IGUIWindow *window = gui->addMessageBox(title.c_str(), text.c_str(), true, flag, 0, -1, 
                       gui->getVideoDriver()->getTexture(workingDirectory + stringc("/GUI/warning.png")));
	return window;
}

bool CDevices::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_KEY_INPUT_EVENT) {
        if (!event.KeyInput.PressedDown) {
            #ifdef _IRR_OSX_PLATFORM_
            if (event.KeyInput.Key == KEY_SHIFT) {
            #else
            if (event.KeyInput.Key == KEY_LCONTROL) {
            #endif
                ctrlWasPushed = false;
            }
            #ifdef _IRR_OSX_PLATFORM_
            if (event.KeyInput.Key == KEY_SHIFT) {
            #else
            if (event.KeyInput.Key == KEY_LSHIFT) {
            #endif
                shiftWasPushed = false;
            }
        }
        if (event.KeyInput.PressedDown) {
            #ifdef _IRR_OSX_PLATFORM_
            if (event.KeyInput.Key == KEY_SHIFT) {
            #else
            if (event.KeyInput.Key == KEY_LCONTROL) {
            #endif
                ctrlWasPushed = true;
            }
            #ifdef _IRR_OSX_PLATFORM_
            if (event.KeyInput.Key == KEY_SHIFT) {
            #else
            if (event.KeyInput.Key == KEY_LSHIFT) {
            #endif
                shiftWasPushed = true;
            }
        }
    }
    
    if (event.EventType == EET_GUI_EVENT) {
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            s32 id = event.GUIEvent.Caller->getID();
            
            switch (id) {
                case DEVICES_FILE_OPEN_DIALOG_EVENTS_CLOSE:
                    window->remove();
                    window = 0;
                    break;
                    
                case DEVICES_FILE_OPEN_DIALOG_EVENTS_OK:
                    //window->remove();
                    addInformationDialog(L"Information",
                                         L"Not implanted yet, please click \"Ok\" in the file open dialog child \n\n"
                                         L"Work in progress...\n", EMBF_OK);
                    break;
                    
                default:
                    break;
            }
        }
        
        if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
            if (window) {
                window->remove();
                window = 0;
            }
        }
        
        if (event.GUIEvent.EventType == EGET_FILE_CHOOSE_DIALOG_CANCELLED) {
            if (window) {
                window->remove();
                window = 0;
            }
        }
        
        if (event.EventType == EET_GUI_EVENT) {
            if (event.GUIEvent.EventType == EGET_EDITBOX_MARKING_CHANGED) {
                setEditBoxEntered(true);
                getObjectPlacement()->setAllowMoving(false);
            }
			if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
				setEditBoxEntered(true);
				getObjectPlacement()->setAllowMoving(false);
			}
        }
    }
    
    if (event.EventType == EET_MOUSE_INPUT_EVENT) {
        if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN) {
            setEditBoxEntered(false);
            getObjectPlacement()->setAllowMoving(true);
        }
    }
    
    if (event.EventType == EET_MOUSE_INPUT_EVENT) {
        if (event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN) {
            if (window) {
                if (window->isVisible()) {
                    dialogPreview->setImage(driver->getTexture(dialog->getFileName()));
                    dialogPreview->setScaleImage(true);
                }
            }
        }
    }
    
    return false;
}
