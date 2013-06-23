//
//  CRenderCallback.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 28/11/12.
//
//
#include "stdafx.h"
#include "CUIWindowEditMaterialsCallback.h"

CUIWindowEditMaterialsCallback::CUIWindowEditMaterialsCallback(CDevices *_devices) {
	devices = _devices;
	editWaterAddon = NULL;
	waterSurfaceData = NULL;

	//-----------------------------------
	//GET DATAS
	driver = devices->getVideoDriver();
	gui = devices->getGUIEnvironment();
	//-----------------------------------

	planeMesh = 0;
	previewNode = 0;

	console = 0;
}

CUIWindowEditMaterialsCallback::~CUIWindowEditMaterialsCallback() {
	if (planeMesh) {
		planeMesh->grab();
		planeMesh->drop();
	}

}

void CUIWindowEditMaterialsCallback::open(CShaderCallback *_callback) {
	
	callback = _callback;

	if (callback == 0) {
		devices->addErrorDialog(L"Error", L"There is no callback to edit.", EMBF_OK);
		delete this;
		return;
	}

	smgr = devices->getSceneManager()->createNewSceneManager();
    planeMesh = smgr->addHillPlaneMesh("plane", dimension2df(6, 6), dimension2du(6, 6));
    smgr->getMeshManipulator()->makePlanarTextureMapping(planeMesh->getMesh(0), 0.05f);

	//-----------------------------------
	//SET UP devices->getGUIEnvironment()

	editMaterialWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(180, 140, 1090, 660), false, L"Edit Material Renderers", 0, -1);
	editMaterialWindow->getMinimizeButton()->setVisible(true);
	editMaterialWindow->getMaximizeButton()->setVisible(true);

    devices->getGUIEnvironment()->addStaticText(L"Vertex Shader", rect<s32>(10, 30, 150, 50), false, true, editMaterialWindow, -1, false);
    vLoadFromFile = devices->getGUIEnvironment()->addButton(rect<s32>(150, 30, 260, 50), editMaterialWindow, -1, L"Load From File", L"Load From File");
    vEdit = devices->getGUIEnvironment()->addButton(rect<s32>(260, 30, 370, 50), editMaterialWindow, -1, L"Edit", L"Edit shader");
    devices->getGUIEnvironment()->addStaticText(L"Pixel Shader", rect<s32>(10, 50, 150, 70), false, true, editMaterialWindow, -1, false);
    pLoadFromFile = devices->getGUIEnvironment()->addButton(rect<s32>(150, 50, 260, 70), editMaterialWindow, -1, L"Load From File", L"Load From File");
    pEdit = devices->getGUIEnvironment()->addButton(rect<s32>(260, 50, 370, 70), editMaterialWindow, -1, L"Edit", L"Edit shader");
	devices->getGUIEnvironment()->addStaticText(L"Constants", rect<s32>(10, 70, 150, 90), false, true, editMaterialWindow, -1, false);
    cLoadFromFile = devices->getGUIEnvironment()->addButton(rect<s32>(150, 70, 260, 90), editMaterialWindow, -1, L"Load From File", L"Load From File");
    cEdit = devices->getGUIEnvironment()->addButton(rect<s32>(260, 70, 370, 90), editMaterialWindow, -1, L"Edit", L"Edit constants");
                
    devices->getGUIEnvironment()->addStaticText(L"Vertex Shader Type", rect<s32>(10, 90, 150, 110), false, true, editMaterialWindow, -1, false);
    vShaderType = devices->getGUIEnvironment()->addComboBox(rect<s32>(150, 90, 440, 110), editMaterialWindow, -1);
    devices->getGUIEnvironment()->addStaticText(L"Pixel Shader Type", rect<s32>(10, 110, 150, 130), false, true, editMaterialWindow, -1, false);
    pShaderType = devices->getGUIEnvironment()->addComboBox(rect<s32>(150, 110, 440, 130), editMaterialWindow, -1);
    devices->getGUIEnvironment()->addStaticText(L"Base Material", rect<s32>(10, 130, 150, 150), false, true, editMaterialWindow, -1, false);
    bShaderType = devices->getGUIEnvironment()->addComboBox(rect<s32>(150, 130, 440, 150), editMaterialWindow, -1);
                
	devices->getGUIEnvironment()->addStaticText(L"Geometry", rect<s32>(10, 160, 150, 180), false, true, editMaterialWindow, -1, false);
    gLoadFromFile = devices->getGUIEnvironment()->addButton(rect<s32>(150, 160, 260, 180), editMaterialWindow, -1, L"Load From File", L"Load From File");
    gEdit = devices->getGUIEnvironment()->addButton(rect<s32>(260, 160, 370, 180), editMaterialWindow, -1, L"Edit", L"Edit constants");

    devices->getGUIEnvironment()->addStaticText(L"On Set Constants", rect<s32>(10, 200, 440, 220), false, true, editMaterialWindow, -1, false);
    constantsCodeBox = new CGUIEditBoxIRB(L"", true, true, devices->getGUIEnvironment(), editMaterialWindow, -1, rect<s32>(10, 220, 440, 430), devices->getDevice());
	constantsCodeBox->setMultiLine(true);
    stringw text = callback->getConstants();
    constantsCodeBox->setText(text.c_str());
	constantsCodeBox->clearKeywords();
    constantsCodeBox->setOverrideColor(SColor(180, 32, 32, 32));
    constantsCodeBox->setBackgroundColor(SColor(255, 200, 200, 200));
    constantsCodeBox->setLineCountColors(SColor(255, 32, 32, 32), SColor(200, 64, 120, 180), SColor(255, 200, 200, 224));
    constantsCodeBox->setSelectionColors(SColor(180, 0, 96, 64), SColor(255, 255, 255, 255), SColor(180, 0, 128, 96));
    constantsCodeBox->addConstantsShaderKeyWords();
                
    editText = devices->getGUIEnvironment()->addStaticText(L"Edit :", rect<s32>(10, 440, 50, 460), false, true, editMaterialWindow, -1, false);
    editorChoice = devices->getGUIEnvironment()->addComboBox(rect<s32>(50, 440, 440, 460), editMaterialWindow, -1);
    editorChoice->addItem(L"Constants");
    editorChoice->addItem(L"Vertex Shader");
    editorChoice->addItem(L"Pixel Shader");
                
    separator = devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(445, 40, 455, 500), true, true, editMaterialWindow, -1, false);
                
    previewText = devices->getGUIEnvironment()->addStaticText(L"Preview :", rect<s32>(460, 40, 560, 62), false, true, editMaterialWindow, -1, false);
    viewPort = new CGUIViewport(devices->getGUIEnvironment(), editMaterialWindow, 1, rect<s32>(460, 60, 890, 330));
	smgr = devices->getSceneManager()->createNewSceneManager();
    if (viewPort) {
        viewPort->setSceneManager(smgr);
        smgr->setAmbientLight(SColorf(0.9f, 0.9f, 0.9f));
        viewPort->setOverrideColor(SColor(255, 0, 200, 0));
                    
        previewNode = smgr->addCubeSceneNode();
        previewNode->setMaterialFlag(EMF_LIGHTING, false);
        previewNode->setScale(vector3df(2, 2, 2));
        previewNode->setMaterialTexture(0, devices->getVideoDriver()->getTexture((stringc(devices->getWorkingDirectory().c_str()) + 
                                                                                    stringc("shaders/Textures/predefined/diffuse.tga")).c_str()));
        previewNode->setMaterialTexture(1, devices->getVideoDriver()->getTexture((stringc(devices->getWorkingDirectory().c_str()) + 
                                                                                    stringc("shaders/Textures/predefined/normal.tga")).c_str()));
		previewNode->setMaterialTexture(2, devices->getVideoDriver()->getTexture((stringc(devices->getWorkingDirectory().c_str()) + 
                                                                                    stringc("shaders/Textures/predefined/specular.tga")).c_str()));
        previewNode->setMaterialType((E_MATERIAL_TYPE)callback->getMaterial());
                    
        ICameraSceneNode *camera = smgr->addCameraSceneNode();
        camera->setFarValue(50.f);
        camera->setPosition(vector3df(-20.f, 15.f, -17.f));
        camera->setAspectRatio(1.f * viewPort->getRelativePosition().getWidth() / viewPort->getRelativePosition().getHeight());
        camera->setTarget(previewNode->getPosition());
        ISceneNodeAnimator *anim = smgr->createFlyCircleAnimator(previewNode->getPosition(), 25.f, 0.0001f, vector3df(0.f, 15.f, 0.f), 0.f, 0.f);
        //camera->addAnimator(anim);
        anim->drop();
    }
                
    buildMaterial = devices->getGUIEnvironment()->addButton(rect<s32>(460, 340, 590, 360), editMaterialWindow, -1, L"Build Material", 
                                                            L"Build Material with selected settings");
    editPreviewNode = devices->getGUIEnvironment()->addButton(rect<s32>(460, 360, 590, 380), editMaterialWindow, -1, L"Edit Preview Node", 
                                                                L"Edit the selected preview node");
    previewObjectText = devices->getGUIEnvironment()->addStaticText(L"Preview Object :", rect<s32>(610, 340, 720, 360), false, true, editMaterialWindow, -1, false);
    previewNodeChoice = devices->getGUIEnvironment()->addComboBox(rect<s32>(720, 340, 890, 360), editMaterialWindow, -1);
    previewNodeChoice->addItem(L"Cube");
    previewNodeChoice->addItem(L"Plane");
    previewNodeChoice->addItem(L"Sphere");
	previewNodeChoice->addItem(L"Current Scene");
                
    closeEditMaterialWindow = devices->getGUIEnvironment()->addButton(rect<s32>(800, 480, 900, 510), editMaterialWindow, -1, L"Close", L"Close Window");

	if(waterSurfaceData != NULL && editWaterAddon == NULL)
	{
		editWaterAddon = new CUIWindowEditWater(waterSurfaceData, devices, this->getSize());
		//editWaterAddon = new CUIWindowEditWater(waterSurfaceData, devices, rect<s32>(editMaterialWindow->getAbsolutePosition().UpperLeftCorner.X, editMaterialWindow->getAbsolutePosition().UpperLeftCorner.Y + editMaterialWindow->getAbsolutePosition().getHeight(), editMaterialWindow->getAbsolutePosition().UpperLeftCorner.X + 200, editMaterialWindow->getAbsolutePosition().UpperLeftCorner.Y + editMaterialWindow->getAbsolutePosition().getHeight() + 90));
	}

	oldSize = this->getSize();

    //FILL VERTEX AND PIXEL SHADER TYPES COMBO BOXES
    for (u32 i=0; i < 7; i++) {
        stringw name = VERTEX_SHADER_TYPE_NAMES[i];
        name.make_upper();
        vShaderType->addItem(name.c_str());
    }
    for (u32 i=0; i < 11; i++) {
        stringw name = PIXEL_SHADER_TYPE_NAMES[i];
        name.make_upper();
        pShaderType->addItem(name.c_str());
    }
	vShaderType->setSelected((E_VERTEX_SHADER_TYPE)(callback->getVertexShaderType()));
	pShaderType->setSelected((E_PIXEL_SHADER_TYPE)callback->getPixelShaderType());
                
    //FILL BASE MATERIAL COMBO BOX
    CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
    editNode->createMaterialTypesComboBox(bShaderType);
	bShaderType->setSelected((E_MATERIAL_TYPE)callback->getBaseMaterial());
	//-----------------------------------

	devices->getEventReceiver()->AddEventReceiver(this);
}

void CUIWindowEditMaterialsCallback::resetCodeBox() {
	constantsCodeBox->remove();
	constantsCodeBox = new CGUIEditBoxIRB(L"", true, true, devices->getGUIEnvironment(), editMaterialWindow, -1, rect<s32>(10, 220, 440, 430), devices->getDevice());
	constantsCodeBox->setMultiLine(true);
	constantsCodeBox->clearKeywords();
    constantsCodeBox->setOverrideColor(SColor(180, 32, 32, 32));
    constantsCodeBox->setBackgroundColor(SColor(255, 200, 200, 200));
    constantsCodeBox->setLineCountColors(SColor(255, 32, 32, 32), SColor(200, 64, 120, 180), SColor(255, 200, 200, 224));
    constantsCodeBox->setSelectionColors(SColor(180, 0, 96, 64), SColor(255, 255, 255, 255), SColor(180, 0, 128, 96));

	stringw text ;
	switch (editorChoice->getSelected()) {
		case 0:
			text = callback->getConstants();
			constantsCodeBox->setText(text.c_str());
			break;
		case 1:
			text = callback->getVertexShader();
			constantsCodeBox->setText(text.c_str());
			break;
		case 2:
			text = callback->getPixelShader();
			constantsCodeBox->setText(text.c_str());
			break;

		default:
			break;
	}

	if (editMaterialWindow->getRelativePosition().getWidth() == devices->getVideoDriver()->getScreenSize().Width) {
		constantsCodeBox->setRelativePosition(rect<s32>(10, 220, separator->getRelativePosition().UpperLeftCorner.X-10,
														editMaterialWindow->getRelativePosition().getHeight()-50));
	}
}

bool CUIWindowEditMaterialsCallback::OnEvent(const SEvent &event) {

	if (event.EventType == EET_KEY_INPUT_EVENT) {

	}

	if (event.EventType == EET_USER_EVENT) {
		if (event.UserEvent.UserData1 == ECUE_REACTIVE_MINIMIZED_WINDOW) {
			if (event.UserEvent.UserData2 == editMaterialWindow->getReferenceCount()) {
				devices->getEventReceiver()->RemoveMinimizedWindow(this);
				if(editWaterAddon != NULL)
					editWaterAddon->setVisible(true);
			}
		}
	}

	if (event.EventType == EET_LOG_TEXT_EVENT) {
		if (console) {
			console->clear();
			console->addItem(stringw(event.LogEvent.Text).c_str());
			console->setSelected(console->getItemCount()-1);
			console->setToolTipText(stringw(console->getListItem(console->getSelected())).c_str());
		}
	}

	if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
		if (event.GUIEvent.Caller == editMaterialWindow) {
			SEvent ev;
			ev.EventType = EET_GUI_EVENT;
			ev.GUIEvent.EventType = EGET_BUTTON_CLICKED;
			ev.GUIEvent.Caller = closeEditMaterialWindow;
			ev.GUIEvent.Element = closeEditMaterialWindow;
			OnEvent(ev);
			return true;
		}
	}

	if (editMaterialWindow) {
		if(oldSize != this->getSize())
		{
			if(editWaterAddon != NULL)
				editWaterAddon->resize(this->getSize());
		}
	}

	if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
		IGUIElement *element = event.GUIEvent.Caller;
            
		//WINDOW PART
		if (event.GUIEvent.Caller == editMaterialWindow->getMinimizeButton()) {
			devices->getEventReceiver()->AddMinimizedWindow(this, editMaterialWindow);
			devices->setRenderScene(true);
			if(editWaterAddon != NULL)
				editWaterAddon->setVisible(false);
		}

		//EDIT MATERIAL WINDOW
		if (element == editPreviewNode) {
			CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
			editNode->open(previewNode, "#object:", editMaterialWindow);
		}

		if (element == vLoadFromFile) {
			openConstantsfod = devices->createFileOpenDialog(L"Choose your file", CGUIFileSelector::EFST_OPEN_DIALOG, editMaterialWindow);
			((CGUIFileSelector *)openConstantsfod)->addFileFilter(L"Vertex Shader File", L"vbs", 0);
		}
		if (element == vEdit) {
			CUICodeEditor *codeEditor = callback->modifyVertexShader(devices);
			codeEditor->setAutoSave(true);
			codeEditor->setAlwaysBringToFront(true);
		}

		if (element == pLoadFromFile) {
			openPixelShaderfod = devices->createFileOpenDialog(L"Choose your file", CGUIFileSelector::EFST_OPEN_DIALOG, editMaterialWindow);
			((CGUIFileSelector *)openPixelShaderfod)->addFileFilter(L"Pixel Shader File", L"fbs", 0);
		}
		if (element == pEdit) {
			CUICodeEditor *codeEditor = callback->modifyPixelShader(devices);
			codeEditor->setAutoSave(true);
			codeEditor->setAlwaysBringToFront(true);
		}

		if (element == cLoadFromFile) {
			openConstantsfod = devices->createFileOpenDialog(L"Choose your file", CGUIFileSelector::EFST_OPEN_DIALOG, editMaterialWindow);
			((CGUIFileSelector *)openConstantsfod)->addFileFilter(L"Material Shader Constants", L"cst", 0);
		}
		if (element == cEdit) {
			CUICodeEditor *codeEditor = callback->modifyConstants(devices);
			codeEditor->setAutoSave(true);
			codeEditor->setAlwaysBringToFront(true);
		}
            
		if (element == buildMaterial) {
			callback->setVertexShaderType((E_VERTEX_SHADER_TYPE)vShaderType->getSelected());
			callback->setPixelShaderType((E_PIXEL_SHADER_TYPE)pShaderType->getSelected());
			callback->setBaseMaterial((E_MATERIAL_TYPE)bShaderType->getSelected());
                
			callback->buildMaterial(devices->getVideoDriver());

			previewNode->setMaterialType((E_MATERIAL_TYPE)callback->getMaterial());

			if (console) {
				console->clear();
			}
		}
                
		if (element == closeEditMaterialWindow) {
			smgr->clear();
			editMaterialWindow->remove();
			editMaterialWindow = 0;
			console = 0;
			devices->setRenderScene(true);
			devices->getEventReceiver()->RemoveEventReceiver(this);
			if(editWaterAddon != NULL)
			{
				editWaterAddon->close();
			}
			delete this;
			return true;
		}

		if (element == editMaterialWindow->getMaximizeButton()) {
			if (editMaterialWindow->getRelativePosition().getWidth() != devices->getVideoDriver()->getScreenSize().Width) {
				editMaterialWindow->setRelativePosition(rect<s32>(0, 75, devices->getVideoDriver()->getScreenSize().Width,
																	devices->getVideoDriver()->getScreenSize().Height));
				editMaterialWindow->setDraggable(false);

				viewPort->setRelativePosition(rect<s32>(editMaterialWindow->getRelativePosition().getWidth()-500,
														40, editMaterialWindow->getRelativePosition().getWidth()-10, 450));
				previewText->setRelativePosition(position2di(viewPort->getRelativePosition().UpperLeftCorner.X, 25));
				separator->setRelativePosition(rect<s32>(viewPort->getRelativePosition().UpperLeftCorner.X - 20, 25,
															viewPort->getRelativePosition().UpperLeftCorner.X - 10, 
															editMaterialWindow->getRelativePosition().getHeight()-10));
				buildMaterial->setRelativePosition(position2di(viewPort->getRelativePosition().UpperLeftCorner.X,
																viewPort->getRelativePosition().UpperLeftCorner.Y
																+ viewPort->getRelativePosition().getHeight()+10));
				editPreviewNode->setRelativePosition(position2di(buildMaterial->getRelativePosition().UpperLeftCorner.X,
																	buildMaterial->getRelativePosition().UpperLeftCorner.Y+30));
				previewObjectText->setRelativePosition(position2di(buildMaterial->getRelativePosition().UpperLeftCorner.X
																	+ buildMaterial->getRelativePosition().getWidth()+20,
																	buildMaterial->getRelativePosition().UpperLeftCorner.Y));
				previewNodeChoice->setRelativePosition(position2di(previewObjectText->getRelativePosition().UpperLeftCorner.X
																	+ previewObjectText->getRelativePosition().getWidth(),
																	previewObjectText->getRelativePosition().UpperLeftCorner.Y));
				closeEditMaterialWindow->setRelativePosition(position2di(editMaterialWindow->getRelativePosition().getWidth()-110,
																			editMaterialWindow->getRelativePosition().getHeight()-40));
				constantsCodeBox->setRelativePosition(rect<s32>(10, 220, separator->getRelativePosition().UpperLeftCorner.X-10,
																editMaterialWindow->getRelativePosition().getHeight()-50));
				editText->setRelativePosition(position2di(editText->getRelativePosition().UpperLeftCorner.X,
															editMaterialWindow->getRelativePosition().getHeight()-40));
				editorChoice->setRelativePosition(position2di(editText->getRelativePosition().getWidth(), 
																editText->getRelativePosition().UpperLeftCorner.Y));
				console = devices->getGUIEnvironment()->addListBox(rect<s32>(viewPort->getRelativePosition().UpperLeftCorner.X,
																			editPreviewNode->getRelativePosition().UpperLeftCorner.Y+40, 
																			editMaterialWindow->getRelativePosition().getWidth()-15,
																			closeEditMaterialWindow->getRelativePosition().UpperLeftCorner.Y-10),
																			editMaterialWindow, -1, true);
				console->setAutoScrollEnabled(true);
				devices->getDevice()->getLogger()->setLogLevel(ELL_ERROR);
				devices->setRenderScene(false);
			} else {
				editMaterialWindow->setRelativePosition(rect<s32>(180, 140, 1090, 660));
				editMaterialWindow->setDraggable(true);

				viewPort->setRelativePosition(rect<s32>(460, 60, 890, 330));
				previewText->setRelativePosition(rect<s32>(460, 40, 560, 62));
				separator->setRelativePosition(rect<s32>(445, 40, 455, 500));
				buildMaterial->setRelativePosition(rect<s32>(460, 340, 590, 360));
				editPreviewNode->setRelativePosition(rect<s32>(460, 360, 590, 380));
				previewObjectText->setRelativePosition(rect<s32>(610, 340, 720, 360));
				previewNodeChoice->setRelativePosition(rect<s32>(720, 340, 890, 360));
				closeEditMaterialWindow->setRelativePosition(rect<s32>(800, 480, 900, 510));
				constantsCodeBox->setRelativePosition(rect<s32>(10, 220, 440, 430));
				editText->setRelativePosition(rect<s32>(10, 440, 50, 460));
				editorChoice->setRelativePosition(rect<s32>(50, 440, 440, 460));
				if (console) {
					console->remove();
					console = 0;
				}
				devices->setRenderScene(true);
			}
		}
	}

	if (event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED) {
        if (event.GUIEvent.Caller == previewNodeChoice) {
            ITexture *tex1;
            ITexture *tex2;
            ITexture *tex3;
            ITexture *tex4;
            vector3df position;
            vector3df scale;

			viewPort->setSceneManager(smgr);

			if (previewNode && previewNodeChoice->getSelected() != 3) {
                tex1 = previewNode->getMaterial(0).getTexture(0);
                tex2 = previewNode->getMaterial(0).getTexture(1);
                tex3 = previewNode->getMaterial(0).getTexture(2);
                tex4 = previewNode->getMaterial(0).getTexture(3);
                position = previewNode->getPosition();
                scale = previewNode->getScale();
                previewNode->remove();
                previewNode = 0;
            }
                
            if (previewNodeChoice->getSelected() == 0) {
                previewNode = smgr->addCubeSceneNode();
                previewNode->getMaterial(0).MaterialType = (E_MATERIAL_TYPE)callback->getMaterial();
                previewNode->getMaterial(0).setTexture(0, tex1);
                previewNode->getMaterial(0).setTexture(1, tex2);
                previewNode->getMaterial(0).setTexture(2, tex3);
                previewNode->getMaterial(0).setTexture(3, tex4);
                previewNode->setPosition(position);
                previewNode->setScale(scale);
                previewNode->setMaterialFlag(EMF_LIGHTING, false);
            }
                
            if (previewNodeChoice->getSelected() == 1) {
                previewNode = smgr->addMeshSceneNode(planeMesh, 0);
                previewNode->getMaterial(0).MaterialType = (E_MATERIAL_TYPE)callback->getMaterial();
                previewNode->getMaterial(0).setTexture(0, tex1);
                previewNode->getMaterial(0).setTexture(1, tex2);
                previewNode->getMaterial(0).setTexture(2, tex3);
                previewNode->getMaterial(0).setTexture(3, tex4);
                previewNode->setPosition(position);
                previewNode->setScale(scale);
                previewNode->setMaterialFlag(EMF_LIGHTING, false);
            }
                
            if (previewNodeChoice->getSelected() == 2) {
                previewNode = smgr->addSphereSceneNode();
				previewNode->getMaterial(0).MaterialType = (E_MATERIAL_TYPE)callback->getMaterial();
                previewNode->getMaterial(0).setTexture(0, tex1);
                previewNode->getMaterial(0).setTexture(1, tex2);
                previewNode->getMaterial(0).setTexture(2, tex3);
                previewNode->getMaterial(0).setTexture(3, tex4);
                previewNode->setPosition(position);
                previewNode->setScale(scale);
                previewNode->setMaterialFlag(EMF_LIGHTING, false);
            }

			if (previewNodeChoice->getSelected() == 3) {
				viewPort->setSceneManager(devices->getSceneManager());
			}
        }
            
        if (event.GUIEvent.Caller == editorChoice) {
            editingConstants = false;
            editingVertexShader = false;
            editingPixelShader = false;
            switch (editorChoice->getSelected()) {
                case 0:
                    editingConstants = true;
                    resetCodeBox();
					constantsCodeBox->addConstantsShaderKeyWords();
                    break;
                case 1:
                    editingVertexShader = true;
                    resetCodeBox();
					constantsCodeBox->addShaderKeywords();
                    break;
                case 2:
                    editingPixelShader = true;
                    resetCodeBox();
					constantsCodeBox->addShaderKeywords();
                    break;

                default:
                    break;
            }
            editorChoice->setTabStop(true);
        }
    }

	if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
        if (event.GUIEvent.Caller == constantsCodeBox) {

            if (editingConstants) {
                callback->setConstants(constantsCodeBox->getText());
            } else if (editingVertexShader) {
                callback->setVertexShader(constantsCodeBox->getText());
            } else if (editingPixelShader) {
                callback->setPixelShader(constantsCodeBox->getText());
            }
        }
    }

	if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
		IGUIFileOpenDialog *dialog = (IGUIFileOpenDialog *)event.GUIEvent.Caller;

		if (dialog == openVertexShaderfod) {
			stringc lines = devices->getCore()->getStringcFromFile(stringc(dialog->getFileName()).c_str());
			callback->setVertexShader(lines.c_str());
		}

		if (dialog == openPixelShaderfod) {
			stringc lines = devices->getCore()->getStringcFromFile(stringc(dialog->getFileName()).c_str());
			callback->setPixelShader(lines.c_str());
		}

		if (dialog == openConstantsfod) {
			stringc lines = devices->getCore()->getStringcFromFile(stringc(dialog->getFileName()).c_str());
			callback->setConstants(lines.c_str());
		}
	}

	oldSize = this->getSize();

	return false;
}

CDevices* CUIWindowEditMaterialsCallback::getDevices()
{
	return devices;
}

rect<s32> CUIWindowEditMaterialsCallback::getSize()
{
	return editMaterialWindow->getRelativePosition();
}

CShaderCallback *CUIWindowEditMaterialsCallback::getCallback()
{
	return callback;
}

SWaterSurfacesData *CUIWindowEditMaterialsCallback::getWaterSurfaceData()
{
	return waterSurfaceData;
}

void CUIWindowEditMaterialsCallback::setWaterSurfaceData(SWaterSurfacesData *_waterSurfaceData)
{
	waterSurfaceData = _waterSurfaceData;
}
