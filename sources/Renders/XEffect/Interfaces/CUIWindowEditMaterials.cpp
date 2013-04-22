//
//  CUIWindowEditMaterials.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 23/02/13.
//
//

#include "CUIWindowEditMaterials.h"

CUIWindowEditMaterials::CUIWindowEditMaterials(CDevices *_devices) {
    devices = _devices;
    
    devices->getEventReceiver()->AddEventReceiver(this);
    
    openingVertex = false;
    openingPixel = false;
    
    editingConstants = true;
    editingVertexShader = false;
    editingPixelShader = false;
    
    editMaterialWindow = 0;
	console = 0;
    
    previewNode = 0;
    
    smgr = devices->getSceneManager()->createNewSceneManager();
    planeMesh = smgr->addHillPlaneMesh("plane", dimension2df(6, 6), dimension2du(6, 6));
    smgr->getMeshManipulator()->makePlanarTextureMapping(planeMesh->getMesh(0), 0.05f);
}

CUIWindowEditMaterials::~CUIWindowEditMaterials() {
	planeMesh->grab();
    planeMesh->drop();
}

void CUIWindowEditMaterials::open() {
    materialsWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(130, 110, 700, 540), false, L"Material Renderers", 0, -1);
	materialsWindow->getCloseButton()->remove();
    
    menu = devices->getGUIEnvironment()->addMenu(materialsWindow);
    
    menu->addItem(L"File", -1, true, true);
	menu->addItem(L"Help", -1, true, true);
    
    submenu = menu->getSubMenu(0);
	submenu->addItem(L"Open a shader", -1);
    submenu->addSeparator();
	submenu->addItem(L"Close", -1);
    
    submenu = menu->getSubMenu(1);
	submenu->addItem(L"How to ?", -1);
    
    devices->getGUIEnvironment()->addStaticText(L"Material Shaders :", rect<s32>(10, 60, 520, 80), false, true, materialsWindow, -1, false);
    editOGLMaterialShader = devices->getGUIEnvironment()->addButton(rect<s32>(460, 60, 520, 80), materialsWindow, -1, L"Edit", L"Edit shader");
    addOGLMaterialShader = devices->getGUIEnvironment()->addButton(rect<s32>(540, 60, 560, 80), materialsWindow, -1, L"+", L"Add a shader");
    removeOGLMaterialShader = devices->getGUIEnvironment()->addButton(rect<s32>(520, 60, 540, 80), materialsWindow, -1, L"-", L"Remove a shader");
    openGLMaterialsList = devices->getGUIEnvironment()->addListBox(rect<s32>(10, 80, 560, 380), materialsWindow, -1, true);
    
    devices->getGUIEnvironment()->addStaticText(L"Name :", rect<s32>(10, 390, 70, 410), false, true, materialsWindow, -1, false);
    materialName = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(70, 390, 280, 410), true, materialsWindow, -1);
    
    close = devices->getGUIEnvironment()->addButton(rect<s32>(460, 390, 560, 420), materialsWindow, -1, L"Close", L"Close this window");
    
    for (u32 i=0; i < devices->getCoreData()->getShaderCallbacks()->size(); i++) {
        stringw name = devices->getCoreData()->getShaderCallbacks()->operator[](i)->getName().c_str();
        openGLMaterialsList->addItem(name.c_str());
    }
    
    if (devices->getCoreData()->getShaderCallbacks()->size() == 0) {
        materialName->setEnabled(false);
        editOGLMaterialShader->setEnabled(false);
    }
}

bool CUIWindowEditMaterials::OnEvent(const SEvent &event) {
    
	if (event.EventType == EET_LOG_TEXT_EVENT) {
		if (console) {
			console->addItem(stringw(event.LogEvent.Text).c_str());
			console->setSelected(console->getItemCount()-1);
			console->setToolTipText(stringw(console->getListItem(console->getSelected())).c_str());
		}
	}

    if (event.EventType == EET_GUI_EVENT) {
        
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            IGUIElement *element = event.GUIEvent.Caller;
            
            //WINDOW PART
            if (element == addOGLMaterialShader) {
                CShaderCallback *callback = new CShaderCallback();
                callback->setName("New Material Type");
                callback->setDevice(devices->getDevice());
                devices->getCoreData()->getShaderCallbacks()->push_back(callback);
                
                stringw name = callback->getName().c_str();

                openGLMaterialsList->addItem(name.c_str());
                
                materialName->setEnabled(true);
                editOGLMaterialShader->setEnabled(true);
            }
            
            if (element == removeOGLMaterialShader) {
                s32 selected;
                selected = openGLMaterialsList->getSelected();
                openGLMaterialsList->removeItem(selected);
                if (selected != -1) {
                    delete devices->getCoreData()->getShaderCallbacks()->operator[](selected);
                    devices->getCoreData()->getShaderCallbacks()->erase(selected);
                } else {
                    devices->addInformationDialog(L"Information", L"Please select a material before trying to remove one...", EMBF_OK);
                }
                
                bool empty;
                empty = (openGLMaterialsList->getItemCount() == 0);
                if (empty) {
                    materialName->setEnabled(false);
                    editOGLMaterialShader->setEnabled(false);
                    
                }
            }
            
            //EDIT MATERIAL
            if (element == editOGLMaterialShader) {
                s32 selected;
                selected = openGLMaterialsList->getSelected();
                
                editingMat = (E_MATERIAL_TYPE)0;
                editingMat = (E_MATERIAL_TYPE)devices->getCoreData()->getShaderCallbacks()->operator[](selected)->getMaterial();
                
                editMaterialWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(180, 140, 1090, 660), true, L"Edit Material Renderers", 0, -1);
				editMaterialWindow->getCloseButton()->remove();
				editMaterialWindow->getMaximizeButton()->setVisible(true);
                
                devices->getGUIEnvironment()->addStaticText(L"Vertex Shader", rect<s32>(10, 30, 150, 50), false, true, editMaterialWindow, -1, false);
                vLoadFromFile = devices->getGUIEnvironment()->addButton(rect<s32>(150, 30, 260, 50), editMaterialWindow, -1, L"Load From File", L"Load From File");
                vEdit = devices->getGUIEnvironment()->addButton(rect<s32>(260, 30, 370, 50), editMaterialWindow, -1, L"Edit", L"Edit shader");
                devices->getGUIEnvironment()->addStaticText(L"Pixel Shader", rect<s32>(10, 50, 150, 70), false, true, editMaterialWindow, -1, false);
                pLoadFromFile = devices->getGUIEnvironment()->addButton(rect<s32>(150, 50, 260, 70), editMaterialWindow, -1, L"Load From File", L"Load From File");
                pEdit = devices->getGUIEnvironment()->addButton(rect<s32>(260, 50, 370, 70), editMaterialWindow, -1, L"Edit", L"Edit shader");
                
                devices->getGUIEnvironment()->addStaticText(L"Vertex Shader Type", rect<s32>(10, 90, 150, 110), false, true, editMaterialWindow, -1, false);
                vShaderType = devices->getGUIEnvironment()->addComboBox(rect<s32>(150, 90, 470, 110), editMaterialWindow, -1);
                vShaderType->setSelected((E_VERTEX_SHADER_TYPE)devices->getCoreData()->getShaderCallbacks()->operator[](selected)->getVertexShaderType());
                devices->getGUIEnvironment()->addStaticText(L"Pixel Shader Type", rect<s32>(10, 110, 150, 130), false, true, editMaterialWindow, -1, false);
                pShaderType = devices->getGUIEnvironment()->addComboBox(rect<s32>(150, 110, 470, 130), editMaterialWindow, -1);
                pShaderType->setSelected((E_PIXEL_SHADER_TYPE)devices->getCoreData()->getShaderCallbacks()->operator[](selected)->getPixelShaderType());
                devices->getGUIEnvironment()->addStaticText(L"Base Material", rect<s32>(10, 130, 150, 150), false, true, editMaterialWindow, -1, false);
                bShaderType = devices->getGUIEnvironment()->addComboBox(rect<s32>(150, 130, 470, 150), editMaterialWindow, -1);
				bShaderType->setSelected((E_MATERIAL_TYPE)devices->getCoreData()->getShaderCallbacks()->operator[](selected)->getBaseMaterial());
                
                devices->getGUIEnvironment()->addStaticText(L"On Set Constants", rect<s32>(10, 150, 440, 170), false, true, editMaterialWindow, -1, false);
                constantsCodeBox = new CGUIEditBoxIRB(L"", true, true, devices->getGUIEnvironment(), editMaterialWindow, -1, rect<s32>(10, 170, 440, 380), devices->getDevice());
                //constantsCodeBox->setWordWrap(false);
                stringw text = devices->getCoreData()->getShaderCallbacks()->operator[](selected)->getConstants();
                constantsCodeBox->setText(text.c_str());
				constantsCodeBox->clearKeywords();
                constantsCodeBox->setOverrideColor(SColor(180, 32, 32, 32));
                constantsCodeBox->setBackgroundColor(SColor(255, 200, 200, 200));
                constantsCodeBox->setLineCountColors(SColor(255, 32, 32, 32), SColor(200, 64, 120, 180), SColor(255, 200, 200, 224));
                constantsCodeBox->setSelectionColors(SColor(180, 0, 96, 64), SColor(255, 255, 255, 255), SColor(180, 0, 128, 96));
                constantsCodeBox->addKeyword("vint", SColor(180, 180, 180, 64), true);
                constantsCodeBox->addKeyword("pint", SColor(180, 180, 180, 64), true);
                constantsCodeBox->addKeyword("vfloat", SColor(180, 180, 180, 64), true);
                constantsCodeBox->addKeyword("pfloat", SColor(180, 180, 180, 64), true);
                constantsCodeBox->addKeyword("vvector3df", SColor(180, 180, 180, 64), true);
                constantsCodeBox->addKeyword("pvector3df", SColor(180, 180, 180, 64), true);
                constantsCodeBox->addKeyword("vmatrix4", SColor(180, 180, 180, 64), true);
                constantsCodeBox->addKeyword("pmatrix4", SColor(180, 180, 180, 64), true);
                constantsCodeBox->addKeyword("vSColor", SColor(180, 180, 180, 64), true);
                constantsCodeBox->addKeyword("pSColor", SColor(180, 180, 180, 64), true);
                
                editText = devices->getGUIEnvironment()->addStaticText(L"Edit :", rect<s32>(10, 390, 50, 410), false, true, editMaterialWindow, -1, false);
                editorChoice = devices->getGUIEnvironment()->addComboBox(rect<s32>(50, 390, 440, 410), editMaterialWindow, -1);
                editorChoice->addItem(L"Constants");
                editorChoice->addItem(L"Vertex Shader");
                editorChoice->addItem(L"Pixel Shader");
                
                separator = devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(445, 40, 455, 500), true, true, editMaterialWindow, -1, false);
                
                previewText = devices->getGUIEnvironment()->addStaticText(L"Preview :", rect<s32>(460, 40, 560, 62), false, true, editMaterialWindow, -1, false);
                viewPort = new CGUIViewport(devices->getGUIEnvironment(), editMaterialWindow, 1, rect<s32>(460, 60, 890, 330), false); 
                if (viewPort) {
                    viewPort->setSceneManager(smgr);
                    smgr->setAmbientLight(SColorf(0.9f, 0.9f, 0.9f));
                    viewPort->setOverrideColor(SColor(255, 0, 200, 0));
                    
                    previewNode = smgr->addCubeSceneNode();
                    previewNode->setMaterialFlag(EMF_LIGHTING, false);
                    previewNode->setScale(vector3df(2, 2, 2));
                    previewNode->setMaterialTexture(0, devices->getVideoDriver()->getTexture((stringc(devices->getWorkingDirectory().c_str()) + 
                                                                                              stringc("cloud.tga")).c_str()));
                    previewNode->setMaterialTexture(1, devices->getVideoDriver()->getTexture((stringc(devices->getWorkingDirectory().c_str()) + 
                                                                                              stringc("lavatile.jpg")).c_str()));
                    previewNode->setMaterialType((E_MATERIAL_TYPE)devices->getCoreData()->getShaderCallbacks()->operator[](selected)->getMaterial());
                    
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
                
                closeEditMaterialWindow = devices->getGUIEnvironment()->addButton(rect<s32>(800, 480, 900, 510), editMaterialWindow, -1, L"Close", 
                                                                                  L"Close Window");
                
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
                
                //FILL BASE MATERIAL COMBO BOX
                CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
                editNode->createMaterialTypesComboBox(bShaderType);
            }
            
            //EDIT MATERIAL WINDOW
            
            if (element == editPreviewNode) {
                CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
                editNode->open(previewNode, "#object:", editMaterialWindow);
            }
                
            if (element == vLoadFromFile) {
                openingVertex = true;
                devices->createFileOpenDialog(L"Choose your file", CGUIFileSelector::EFST_OPEN_DIALOG, editMaterialWindow)->addFileFilter(L"Vertex Shader File", L"vbs", 0);
            }
            if (element == pLoadFromFile) {
                openingPixel = true;
                devices->createFileOpenDialog(L"Choose your file", CGUIFileSelector::EFST_OPEN_DIALOG, editMaterialWindow)->addFileFilter(L"Pixel Shader File", L"fbs", 0);
            }
            
            if (element == buildMaterial) {
                s32 selected;
                selected = openGLMaterialsList->getSelected();
                devices->getCoreData()->getShaderCallbacks()->operator[](selected)->setVertexShaderType((E_VERTEX_SHADER_TYPE)vShaderType->getSelected());
                devices->getCoreData()->getShaderCallbacks()->operator[](selected)->setPixelShaderType((E_PIXEL_SHADER_TYPE)pShaderType->getSelected());
                devices->getCoreData()->getShaderCallbacks()->operator[](selected)->setBaseMaterial((E_MATERIAL_TYPE)bShaderType->getSelected());
                
                devices->getCoreData()->getShaderCallbacks()->operator[](selected)->buildMaterial(devices->getVideoDriver());
                
                previewNode->setMaterialType((E_MATERIAL_TYPE)devices->getCoreData()->getShaderCallbacks()->operator[](selected)->getMaterial());
            }
                
            if (element == closeEditMaterialWindow) {
                smgr->clear();
                editMaterialWindow->remove();
                editMaterialWindow = 0;
				console = 0;
				devices->setRenderScene(true);
            }

			if (editMaterialWindow) {
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
						constantsCodeBox->setRelativePosition(rect<s32>(10, 175, separator->getRelativePosition().UpperLeftCorner.X-10,
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
						devices->getDevice()->getLogger()->setLogLevel(ELL_INFORMATION);
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
						constantsCodeBox->setRelativePosition(rect<s32>(10, 170, 440, 380));
						editText->setRelativePosition(rect<s32>(10, 390, 50, 410));
						editorChoice->setRelativePosition(rect<s32>(50, 390, 440, 410));
						if (console) {
							console->remove();
							console = 0;
						}
						devices->setRenderScene(true);
					}
				}
			}

			if (element == close) {
                materialsWindow->remove();
				editMaterialWindow = 0;
				materialsWindow = 0;
                devices->getEventReceiver()->RemoveEventReceiver(this);
                delete this;
            }
        }
            
        if (event.GUIEvent.EventType == EGET_FILE_CHOOSE_DIALOG_CANCELLED) {
            openingPixel = false;
            openingVertex = false;
        }
        if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
            s32 selected;
            selected = openGLMaterialsList->getSelected();
            
            IGUIFileOpenDialog *dialog = (IGUIFileOpenDialog *)event.GUIEvent.Caller;
            stringc path = dialog->getFileName();
            stringc lines;
            std::ifstream file(path.c_str());
            if (file.is_open()) {
                while (file.good()) {
                    std::string line;
                    std::getline(file, line);
                    lines += line.c_str();
                    lines += "\n";
                }
                file.close();
            }
            
            if (openingVertex) {
                devices->getCoreData()->getShaderCallbacks()->operator[](selected)->setVertexShader(lines.c_str());
            }
            if (openingPixel) {
                devices->getCoreData()->getShaderCallbacks()->operator[](selected)->setPixelShader(lines.c_str());
            }
            openingPixel = false;
            openingVertex = false;
        }
        
        if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
            if (event.GUIEvent.Caller == openGLMaterialsList) {
                IGUIListBox *listbox = (IGUIListBox *)event.GUIEvent.Caller;
                if (listbox->getItemCount() > 0) {
                    stringw name = devices->getCoreData()->getShaderCallbacks()->operator[](listbox->getSelected())->getName().c_str();
                    materialName->setText(name.c_str());
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
                
                if (previewNode) {
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
                    previewNode->getMaterial(0).MaterialType = editingMat;
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
                    previewNode->getMaterial(0).MaterialType = editingMat;
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
                    previewNode->getMaterial(0).MaterialType = editingMat;
                    previewNode->getMaterial(0).setTexture(0, tex1);
                    previewNode->getMaterial(0).setTexture(1, tex2);
                    previewNode->getMaterial(0).setTexture(2, tex3);
                    previewNode->getMaterial(0).setTexture(3, tex4);
                    previewNode->setPosition(position);
                    previewNode->setScale(scale);
                    previewNode->setMaterialFlag(EMF_LIGHTING, false);
                }
            }
            
            if (event.GUIEvent.Caller == editorChoice) {
                s32 selected;
                selected = openGLMaterialsList->getSelected();
                editingConstants = false;
                editingVertexShader = false;
                editingPixelShader = false;
				constantsCodeBox->restartScrollBar();
				constantsCodeBox->recalculateTextRect();
                switch (editorChoice->getSelected()) {
                    case 0:
                        editingConstants = true;
                        constantsCodeBox->clearKeywords();
                        constantsCodeBox->addKeyword("vint", SColor(180, 180, 180, 64));
                        constantsCodeBox->addKeyword("pint", SColor(180, 180, 180, 64));
                        constantsCodeBox->addKeyword("vfloat", SColor(180, 180, 180, 64));
                        constantsCodeBox->addKeyword("pfloat", SColor(180, 180, 180, 64));
                        constantsCodeBox->addKeyword("vvector3df", SColor(180, 180, 180, 64));
                        constantsCodeBox->addKeyword("pvector3df", SColor(180, 180, 180, 64));
                        constantsCodeBox->addKeyword("vmatrix4", SColor(180, 180, 180, 64));
                        constantsCodeBox->addKeyword("pmatrix4", SColor(180, 180, 180, 64));
                        constantsCodeBox->addKeyword("vSColor", SColor(180, 180, 180, 64));
                        constantsCodeBox->addKeyword("pSColor", SColor(180, 180, 180, 64));
                        constantsCodeBox->setText(stringw(devices->getCoreData()->getShaderCallbacks()->operator[](selected)->getConstants().c_str()).c_str());
                        break;
                    case 1:
                        editingVertexShader = true;
                        constantsCodeBox->clearKeywords();
                        constantsCodeBox->addShaderKeywords();
                        constantsCodeBox->setText(stringw(devices->getCoreData()->getShaderCallbacks()->operator[](selected)->getVertexShader().c_str()).c_str());
                        break;
                    case 2:
                        editingPixelShader = true;
                        constantsCodeBox->clearKeywords();
                        constantsCodeBox->addShaderKeywords();
                        constantsCodeBox->setText(stringw(devices->getCoreData()->getShaderCallbacks()->operator[](selected)->getPixelShader().c_str()).c_str());
                        break;
                        
                    default:
                        break;
                }
                
                editorChoice->setTabStop(true);
            }
        }
        
        if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
            if (event.GUIEvent.Caller == materialName) {
                stringw name = materialName->getText();
                s32 selected;
                selected = openGLMaterialsList->getSelected();
                openGLMaterialsList->setItem(openGLMaterialsList->getSelected(), name.c_str(), 0);
                devices->getCoreData()->getShaderCallbacks()->operator[](selected)->setName(name.c_str());
            }
            
            if (event.GUIEvent.Caller == constantsCodeBox) {
                s32 selected;
                selected = openGLMaterialsList->getSelected();
                if (editingConstants) {
                    devices->getCoreData()->getShaderCallbacks()->operator[](selected)->setConstants(constantsCodeBox->getText());
                } else if (editingVertexShader) {
                    devices->getCoreData()->getShaderCallbacks()->operator[](selected)->setVertexShader(constantsCodeBox->getText());
                } else if (editingPixelShader) {
                    devices->getCoreData()->getShaderCallbacks()->operator[](selected)->setPixelShader(constantsCodeBox->getText());
                }
            }
        }
        
    }
    
    return false;
}
