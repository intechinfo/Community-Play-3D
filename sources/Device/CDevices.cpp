/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "CDevices.h"


CDevices::CDevices() {
    //DEVICE
	Device = 0;
	wolrdCore = new CCore();
    worldCoreData = new CCoreData();
    
    //RENDERS
    hdr = 0;
    effect = 0;
    
    renderHDR = false;
    renderXEffect = false;
    renderGUI = true;
    
    editBoxEntered = false;
    
    window = 0;
}

CDevices::~CDevices() {
    
}

void CDevices::updateDevice() {
    
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
    
    objPlacement->refresh(cursorBillBoard->getPosition());
    
    for (int i=0; i < worldCoreData->getLightsNodes()->size(); i++) {
        effect->getShadowLight(i).setPosition(worldCoreData->getLightsNodes()->operator[](i)->getPosition());
        effect->getShadowLight(i).setTarget(worldCoreData->getLightsNodes()->operator[](i)->getRotation());
        
        effect->getShadowLight(i).setLightColor(SColorf
                                                (((ILightSceneNode *)worldCoreData->getLightsNodes()->operator[](i))->getLightData().DiffuseColor.r,
                                                 ((ILightSceneNode *)worldCoreData->getLightsNodes()->operator[](i))->getLightData().DiffuseColor.g,
                                                 ((ILightSceneNode *)worldCoreData->getLightsNodes()->operator[](i))->getLightData().DiffuseColor.b,
                                                 255));
        effect->getShadowLight(i).setFarValue(((ILightSceneNode *)worldCoreData->getLightsNodes()->operator[](i))->getRadius());
    }
    
    //RENDERS
    if (renderHDR) {
        hdr->render();
        //hdr->getPostProcessorNode()->setVisible(true);
    } else {
        hdr->getPostProcessorNode()->setVisible(false);
    }
    
    if (renderXEffect) {
        effect->setActiveSceneManager(smgr);
        effect->update();
    } else {
        smgr->drawAll();
    }
    
    effectSmgr->drawAll();
    
    if (renderGUI) {
        gui->drawAll();
    }
    
    stringw text = L"Soganatsu Studios World Editor V1.  FPS : ";
    text += driver->getFPS();
    Device->setWindowCaption(text.c_str());
    
    camera_maya->setAspectRatio(1.f * driver->getScreenSize().Width / driver->getScreenSize().Height);
}

void CDevices::createDevice(SIrrlichtCreationParameters parameters) {
    //DEVICE
	Device = createDeviceEx(parameters);
    Device->setWindowCaption(L"Soganatsu Studios World Editor V1");
	Device->setResizable(true);
    
    driver = Device->getVideoDriver();
    smgr = Device->getSceneManager();
    effectSmgr = smgr->createNewSceneManager();
    gui = Device->getGUIEnvironment();
    
    Device->setEventReceiver(&receiver);
    receiver.AddEventReceiver(this);
    
    workingDirectory = Device->getFileSystem()->getWorkingDirectory().c_str();
    
    //-----------------------------------
    //CAMERAS
    keyMap[0].Action = EKA_MOVE_FORWARD;
	keyMap[0].KeyCode = KEY_KEY_W;
	keyMap[1].Action = EKA_MOVE_BACKWARD;
	keyMap[1].KeyCode = KEY_KEY_S;
	keyMap[2].Action = EKA_STRAFE_LEFT;
	keyMap[2].KeyCode = KEY_KEY_A;
	keyMap[3].Action = EKA_STRAFE_RIGHT;
	keyMap[3].KeyCode = KEY_KEY_D;
	keyMap[4].Action = EKA_JUMP_UP;
	keyMap[4].KeyCode = KEY_SPACE;
    
    camera_fps = smgr->addCameraSceneNodeFPS(0, 200.0f, 0.04f, -1, keyMap, 5, false, 0.3f, false, true);
	camera_fps->setTarget(vector3df(0.f, 5.f, 0.f));
	camera_fps->setFarValue(42000.0f);
	camera_fps->setPosition(vector3df(0.f, 10.f, 13.f));
	camera_fps->setName("editor:FPScamera");
    camera_fps->setID(-1);
	
	camera_maya = smgr->addCameraSceneNodeMaya();
	camera_maya->setPosition(vector3df(0.f, 30.f, 130.f));
    camera_maya->setFarValue(42000.0f);
	camera_maya->setName("editor:MayaCamera");
    camera_maya->setID(-1);
    camera_maya->setAspectRatio(1.f * driver->getScreenSize().Width / driver->getScreenSize().Height);
    
    smgr->setActiveCamera(camera_maya);
    
    cursorBillBoard = smgr->addBillboardSceneNode();
    cursorBillBoard->setName("editor:cursor");
	cursorBillBoard->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	cursorBillBoard->setMaterialTexture(0, driver->getTexture("GUI/particle.bmp"));
	cursorBillBoard->setMaterialFlag(EMF_LIGHTING, false);
	cursorBillBoard->setMaterialFlag(EMF_ZBUFFER, false);
	cursorBillBoard->setSize(dimension2d<f32>(1.0f, 1.0f));
	cursorBillBoard->setID(-1);
    //-----------------------------------
    
    //RENDERS
    hdr = new ShaderGroup(Device, smgr);
	hdr->sampleDist0 = 0.0118f;
    hdr->sampleDist1 = 0.125f;
	hdr->distanceScale = 0.006f;
	hdr->range = 2.0f;
    hdr->focus = 0.26f;
    
    effect = new EffectHandler(Device, driver->getScreenSize(), false, true, true);
    //effect = new EffectHandler(Device, dimension2du(800, 600), false, true, true);
    effect->setActiveSceneManager(smgr);
	filterType = EFT_4PCF;
	effect->setClearColour(SColor(0x0));
	effect->setAmbientColor(SColor(255, 64, 64, 64));
    shaderExt = (driver->getDriverType() == EDT_DIRECT3D9) ? ".hlsl" : ".glsl";
    
    //3D INTERACTION
    collisionManager = new CCollisionManager(smgr);
    objPlacement = new CCoreObjectPlacement(smgr, Device->getCursorControl(), collisionManager);
    
    receiver.AddEventReceiver(objPlacement);
}

void CDevices::createFileOpenDialog(stringw title) {
    window = gui->addWindow(rect<s32>(100, 100, 960, 590), true, 
                            L"Open File Dialog", 0, -1);
    
    dialog = gui->addFileOpenDialog(title.c_str(), false, window, -1);
    dialog->setRelativePosition(rect<s32>(0, 20, 560, 440));
    
    gui->addButton(rect<s32>(320, 450, 430, 480), window, DEVICES_FILE_OPEN_DIALOG_EVENTS_OK, L"Ok", L"Accept");
    gui->addButton(rect<s32>(440, 450, 550, 480), window, DEVICES_FILE_OPEN_DIALOG_EVENTS_CLOSE, L"Close", L"Close this window");
    dialogPreview = gui->addImage(rect<s32>(570, 20, 1260-410, 440), window, -1, L"Image Preview (Double Click To View)");
}

void CDevices::addInformationDialog(stringw title, stringw text, EMESSAGE_BOX_FLAG flag) {
    gui->addMessageBox(title.c_str(), text.c_str(), true, flag, 0, -1, 
                       gui->getVideoDriver()->getTexture(workingDirectory + stringc("/GUI/info.png")));
}

void CDevices::addErrorDialog(stringw title, stringw text, EMESSAGE_BOX_FLAG flag) {
    gui->addMessageBox(title.c_str(), text.c_str(), true, flag, 0, -1, 
                       gui->getVideoDriver()->getTexture(workingDirectory + stringc("/GUI/error.png")));
}

void CDevices::addWarningDialog(stringw title, stringw text, EMESSAGE_BOX_FLAG flag) {
    gui->addMessageBox(title.c_str(), text.c_str(), true, flag, 0, -1, 
                       gui->getVideoDriver()->getTexture(workingDirectory + stringc("/GUI/warning.png")));
}

bool CDevices::OnEvent(const SEvent &event) {
    
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
