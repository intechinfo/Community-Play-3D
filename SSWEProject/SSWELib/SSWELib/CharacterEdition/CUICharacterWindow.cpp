//
//  CUICharacterWindow.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 10/10/12.
//
//

#include "stdafx.h"
#include "CUICharacterWindow.h"

CUICharacterWindow::CUICharacterWindow(CDevices *_devices) {
    devices = _devices;
    
    devices->getEventReceiver()->AddEventReceiver(this);
    
    characterWindow = 0;
    
    currentLoad = 0;
    node = 0;

	lastBoneSceneNode = 0;
    
    actions.clear();
}

CUICharacterWindow::~CUICharacterWindow() {
    delete editBones;
}

void CUICharacterWindow::open() {
    
    characterWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(devices->getVideoDriver()->getScreenSize().Width/2-400, 
                                                                        100, devices->getVideoDriver()->getScreenSize().Width/2+400, 700),
                                                              false, L"Animated Node Edition Window", 0, -1);
    characterWindow->getCloseButton()->remove();
	characterWindow->getMaximizeButton()->setVisible(true);
	characterWindow->getMinimizeButton()->setVisible(true);
    
    //-----------------------------------
    //MENU
    menu = devices->getGUIEnvironment()->addMenu(characterWindow);
    
    menu->addItem(L"File", -1, true, true);
	menu->addItem(L"View", -1, true, true);
	menu->addItem(L"Help", -1, true, true);
    
    submenu = menu->getSubMenu(0);
	submenu->addItem(L"Open Saved Animation Configuration", CXT_EDIT_WINDOW_CHARACTER_EVENTS_OPEN);
    submenu->addSeparator();
	submenu->addItem(L"Save", CXT_EDIT_WINDOW_CHARACTER_EVENTS_SAVE);
	submenu->addSeparator();
	submenu->addItem(L"Close", CXT_EDIT_WINDOW_CHARACTER_EVENTS_CLOSE);
    
	submenu = menu->getSubMenu(1);
	submenu->addItem(L"Enter Bones Edition...", CXT_EDIT_WINDOW_CHARACTER_EVENTS_ENTER_BONES_EDITION);
	submenu->addSeparator();
	submenu->addItem(L"Add Frames from other model...", CXT_EDIT_WINDOW_CHARACTER_EVENTS_ADD_FRAMES);

    submenu = menu->getSubMenu(2);
	submenu->addItem(L"How to ?", -1);
    
    //-----------------------------------
    
    //-----------------------------------
    //TOOLBAR
    bar = devices->getGUIEnvironment()->addToolBar(characterWindow, -1);
    bar->setRelativePosition(position2di(0, 40));
    bar->setRelativePosition(rect<s32>(0, 43, 795, 70));

	ITexture* image = devices->getVideoDriver()->getTexture(devices->getWorkingDirectory() + stringc("/GUI/folder.png"));
	bar->addButton(CXT_EDIT_WINDOW_CHARACTER_EVENTS_OPEN, 0, L"Open Saved Animation Configuration", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture(devices->getWorkingDirectory() + stringc("/GUI/edit.png"));
	bar->addButton(CXT_EDIT_WINDOW_CHARACTER_EVENTS_EDIT, 0, L"Edit The Node", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture(devices->getWorkingDirectory() + stringc("/GUI/save.png"));
	bar->addButton(0, 0, L"Export this scene", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture(devices->getWorkingDirectory() + stringc("/GUI/help.png"));
	bar->addButton(0, 0, L"Open Help", image, 0, false, true);
    //-----------------------------------

    //-----------------------------------
    //VIEW PORT CHARACTER VIEW
	smgr = devices->createNewSceneManager();
    viewPort = new CGUIViewport(devices->getGUIEnvironment(), characterWindow, 1, 
                                              rect<s32>(5, 75, 400, 350)); 
    if (viewPort) {
        viewPort->setSceneManager(smgr);
        smgr->setAmbientLight(SColorf(0.3f, 0.3f, 0.3f));
        viewPort->setOverrideColor(SColor(255, 0, 0, 0)); 

		camera = smgr->addCameraSceneNodeMaya();
		grid = new CGridSceneNode(smgr->getRootSceneNode(), smgr);
		smgr->setActiveCamera(camera);
        camera->setFarValue(42000.0f);
        camera->setPosition(vector3df(0.f, 1.f, 1.f));
        camera->setAspectRatio(1.f * viewPort->getRelativePosition().getWidth() / viewPort->getRelativePosition().getHeight());
        camera->setPosition(devices->getSceneManager()->getActiveCamera()->getPosition());
        camera->setTarget(devices->getSceneManager()->getActiveCamera()->getTarget());
    }
    
    rotationScrollBar = devices->getGUIEnvironment()->addScrollBar(true, rect<s32>(5, 355, 400, 370), characterWindow, 
                                                                   CXT_EDIT_WINDOW_CHARACTER_EVENTS_ROTATION_SB);
    rotationScrollBar->setMax(359);
    rotationScrollBar->setMin(1);
    rotationScrollBar->setPos(rotationScrollBar->getMin());
    //-----------------------------------
    
    //-----------------------------------
    //BUTTONS AND LISTBOX
    actionsText = devices->getGUIEnvironment()->addStaticText(L"Actions : ", rect<s32>(5, 375, 190, 395), true, true, characterWindow, -1, false);
    add = devices->getGUIEnvironment()->addButton(rect<s32>(195, 375, 295, 395), characterWindow, CXT_EDIT_WINDOW_CHARACTER_EVENTS_ADD, L"Add", L"Add an animation");
    remove = devices->getGUIEnvironment()->addButton(rect<s32>(300, 375, 400, 395), characterWindow, CXT_EDIT_WINDOW_CHARACTER_EVENTS_DELETE, L"Delete", L"Delete this animation");
    
    animationList = devices->getGUIEnvironment()->addListBox(rect<s32>(5, 400, 400, 590), characterWindow, -1, true);
    //-----------------------------------
    
    //-----------------------------------
    //TABS
    tabCtrl = devices->getGUIEnvironment()->addTabControl(rect<int>(405, 75, 795, 595), characterWindow, true, true, 0);
    actionsTab = tabCtrl->addTab(L"Action");
    framesTab = tabCtrl->addTab(L"Frames");
    detailsTab = tabCtrl->addTab(L"Details");
    //-----------------------------------
    
    //-----------------------------------
    //ACTIONS TAB
    previewAction = devices->getGUIEnvironment()->addButton(rect<s32>(10, 20, 110, 45), actionsTab, CXT_EDIT_WINDOW_CHARACTER_EVENTS_PREVIEW, L"Preview", L"Preview Action");
    
    devices->getGUIEnvironment()->addStaticText(L"Start Frame : ", rect<s32>(10, 60, 110, 80), false, true, actionsTab, -1, false);
    startAction = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(120, 60, 320, 80), true, actionsTab, -1);
    devices->getGUIEnvironment()->addStaticText(L"End Frame : ", rect<s32>(10, 90, 110, 110), false, true, actionsTab, -1, false);
    endAction = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(120, 90, 320, 110), true, actionsTab, -1);
    devices->getGUIEnvironment()->addStaticText(L"Name : ", rect<s32>(10, 120, 110, 140), false, true, actionsTab, -1, false);
    nameAction = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(120, 120, 320, 140), true, actionsTab, -1);
    
    devices->getGUIEnvironment()->addStaticText(L"Anim Speed : ", rect<s32>(10, 180, 110, 200), false, true, actionsTab, -1, false);
    speedAction =  devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(120, 180, 320, 200), true, actionsTab, -1);
    //-----------------------------------

	//-----------------------------------
    //FRAMES TAB
	framesListBox = devices->getGUIEnvironment()->addListBox(rect<s32>(5, 5, tabCtrl->getRelativePosition().getWidth()-5, 
																	   framesTab->getRelativePosition().getHeight()-5), 
																	   framesTab, -1, true);
	//-----------------------------------
}

void CUICharacterWindow::setModel(IAnimatedMeshSceneNode *pnode, s32 index) {
	if (pnode->getType() != ESNT_ANIMATED_MESH) {
		add->setEnabled(false);
		remove->setEnabled(false);
		previewAction->setEnabled(false);
		return;
	}

    if (pnode) {
        if (node) {
            node->remove();
            node = 0;
        }
		IAnimatedMesh *mesh = smgr->getMesh(devices->getCoreData()->getObjectsData()->operator[](index).getPath());
		node = smgr->addAnimatedMeshSceneNode(mesh);
        node->setPosition(pnode->getPosition());
        node->setRotation(pnode->getRotation());
        node->setScale(pnode->getScale());
        node->setMaterialFlag(EMF_LIGHTING, false);

		grid->setPosition(vector3df(grid->getPosition().X, node->getPosition().Y, grid->getPosition().Z));
        
        for (int i=0; i < pnode->getMaterialCount(); i++) {
            for (int a=0; a < 4; a++) {
                node->getMaterial(i).TextureLayer[a] = pnode->getMaterial(i).TextureLayer[a];
            }
        }

		for (u32 i=0; i < node->getEndFrame() && node->getEndFrame() >= 0; i++) {
			framesListBox->addItem(stringw(stringc("Frame ") + stringc(i+1)).c_str());
		}
        
        node->setName(pnode->getName());
        //node->setDebugDataVisible(EDS_SKELETON);
        
        devices->getGUIEnvironment()->addStaticText(L"Animation Speed (frames/s) : ", rect<s32>(5, 15, 200, 35), false, true, detailsTab, -1, true);
        animationSpeed = devices->getGUIEnvironment()->addEditBox(L"0", rect<s32>(210, 15, 280, 35), true, detailsTab, -10);
        animationSpeed->setText(devices->getCore()->getStrNumber(node->getAnimationSpeed()).c_str());
        applyAnimationSpeed = devices->getGUIEnvironment()->addButton(rect<s32>(290, 15, 380, 35), detailsTab, CXT_EDIT_WINDOW_CHARACTER_EVENTS_APPLY_SPEED, L"Apply");
        
        stringw frameCount = L"Frames Count : ";
        frameCount += node->getEndFrame();
        devices->getGUIEnvironment()->addStaticText(frameCount.c_str(), rect<s32>(5, 40, 200, 60), false, true, detailsTab, -1, true);
        stringw startFrame = L"Start Frame : ";
        startFrame += node->getStartFrame();
        devices->getGUIEnvironment()->addStaticText(startFrame.c_str(), rect<s32>(5, 65, 200, 85), false, true, detailsTab, -1, true);
        stringw endFrame = L"End Frame : ";
        endFrame += node->getEndFrame();
        devices->getGUIEnvironment()->addStaticText(endFrame.c_str(), rect<s32>(5, 90, 200, 110), false, true, detailsTab, -1, true);
        stringw jointCount = L"Joint Count : ";
        jointCount += node->getJointCount();
        devices->getGUIEnvironment()->addStaticText(jointCount.c_str(), rect<s32>(5, 115, 200, 135), false, true, detailsTab, -1, true);
        stringw nodeName = L"Object name : ";
        nodeName += node->getName();
        devices->getGUIEnvironment()->addStaticText(nodeName.c_str(), rect<s32>(5, 140, 200, 160), false, true, detailsTab, -1, true);
    }
    
    startAction->setEnabled(false);
    endAction->setEnabled(false);
    nameAction->setEnabled(false);
    speedAction->setEnabled(false);
    previewAction->setEnabled(false);

	editBones = new CUIEditBones(devices, node);
	devices->getEventReceiver()->AddEventReceiver(editBones);
}

void CUICharacterWindow::exportAnimatedModel() {
	FILE *export_file;
	export_file = fopen("test.anc", "w");
	fprintf(export_file, "<?xml version=\"1.0\"?>\n\n");
	fprintf(export_file, "<rootAnim>\n\n");
	for (u32 i=0; i < actions.size(); i++) {
		fprintf(export_file, "\t");
		fprintf(export_file, actions[i]->getXMLValues().c_str());
	}
	fprintf(export_file, "\n</rootAnim>\n");
	fclose(export_file);
}

bool CUICharacterWindow::OnEvent(const SEvent &event) {

	if (event.EventType == EET_USER_EVENT) {
		if (event.UserEvent.UserData1 == ECUE_REACTIVE_MINIMIZED_WINDOW) {
			if (event.UserEvent.UserData2 == characterWindow->getReferenceCount()) {
				//devices->getEventReceiver()->RemoveMinimizedWindow(this);
			}
		}
	}

    if (event.EventType == EET_GUI_EVENT) {
        
        if (event.GUIEvent.EventType == EGET_MENU_ITEM_SELECTED) {
            IGUIContextMenu *tempMenu = (IGUIContextMenu *)event.GUIEvent.Caller;
            switch (tempMenu->getItemCommandId(tempMenu->getSelectedItem())) {
                //FILE
                case CXT_EDIT_WINDOW_CHARACTER_EVENTS_CLOSE:
					editBones->close();
                    characterWindow->remove();
                    viewPort->remove();
                    if (node) {
                        node->removeAll();
                        node->remove();
                        node = 0;
						smgr->clear();
                    }
					devices->removeSceneManager(smgr);
                    characterWindow = 0;
                    actions.clear();
					devices->getEventReceiver()->RemoveEventReceiver(this);
					devices->getEventReceiver()->RemoveEventReceiver(editBones);
					devices->setRenderScene(true);
					delete this;
					return false;
                    break;

				case CXT_EDIT_WINDOW_CHARACTER_EVENTS_SAVE:
					saveDialog = devices->createFileOpenDialog(L"Select the directory", CGUIFileSelector::EFST_SAVE_DIALOG, characterWindow);
					break;

				case CXT_EDIT_WINDOW_CHARACTER_EVENTS_ENTER_BONES_EDITION:
					node->setDebugDataVisible(EDS_SKELETON);
					//devices->getEventReceiver()->AddMinimizedWindow(this, characterWindow);
					characterWindow->setVisible(false);
					devices->getSceneManagers()->push_back(smgr);
					devices->setSceneManagerToDraw(smgr);
					smgr->setActiveCamera(devices->getMayaCamera());
					devices->getSecondSceneManager()->setActiveCamera(devices->getMayaCamera());
					devices->getDevice()->setInputReceivingSceneManager(smgr);
					node->setAnimationSpeed(0);
					node->setFrameLoop(0, 0);
					node->setLoopMode(true);
					node->render();
					node->setJointMode(EJUOR_CONTROL);
					devices->setContextName("Bones Edition");
					editBones->open();
					//cameraMaya->setUpVector(camera->getPosition());
					//cameraMaya->setTarget(node->getPosition());
					devices->setRenderScene(true);
					break;
				case CXT_EDIT_WINDOW_CHARACTER_EVENTS_ADD_FRAMES: {
					devices->getDevice()->getFileSystem()->changeWorkingDirectoryTo("data/Characters/Murphy/");
					openMeshSkinned = devices->createFileOpenDialog(L"Select mesh", CGUIFileSelector::EFST_OPEN_DIALOG, 0, true);
				}
					break;
                    
                default:
                    break;
            }
        }

        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == characterWindow->getMinimizeButton()) {
				//devices->getEventReceiver()->AddMinimizedWindow(this, characterWindow);
				devices->setRenderScene(true);
			}

			if (event.GUIEvent.Caller == characterWindow->getMaximizeButton()) {
				if (characterWindow->getRelativePosition().getWidth() != devices->getVideoDriver()->getScreenSize().Width+2) {
					characterWindow->setRelativePosition(rect<s32>(0, 75, devices->getVideoDriver()->getScreenSize().Width+2,
																   devices->getVideoDriver()->getScreenSize().Height));
					characterWindow->setDraggable(false);
					viewPort->setRelativePosition(rect<s32>(5, 75, 600, 550));
					tabCtrl->setRelativePosition(rect<s32>(viewPort->getRelativePosition().UpperLeftCorner.X+viewPort->getRelativePosition().getWidth()+5,
					75, characterWindow->getRelativePosition().getWidth()-10, 350));
					bgJointNodes = devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(tabCtrl->getRelativePosition().UpperLeftCorner.X,
						tabCtrl->getRelativePosition().UpperLeftCorner.Y+tabCtrl->getRelativePosition().getHeight()+10,
						characterWindow->getRelativePosition().getWidth()-5, characterWindow->getRelativePosition().getHeight()-10), 
						true, true, characterWindow, -1, true);
					bgJointNodes->setBackgroundColor(SColor(255, 50, 50, 50));
					jointNodesListBox = devices->getGUIEnvironment()->addListBox(rect<s32>(5, 5,bgJointNodes->getRelativePosition().getWidth(), 
																						   bgJointNodes->getRelativePosition().getHeight()-5), 
																				 bgJointNodes, -1, true);
					if (node) {
						for (u32 i=0; i < node->getJointCount(); i++) {
							stringc name = "Name : ";
							name += node->getJointNode(i)->getName();
							stringc boneName = "Bone Name = ";
							boneName += node->getJointNode(i)->getBoneName();
							jointNodesListBox->addItem(stringw(name + stringc(" -/- ") + boneName).c_str());
						}
					}
					boneNode = 0;
					devices->setRenderScene(false);
				} else {
					characterWindow->setRelativePosition(rect<s32>(devices->getVideoDriver()->getScreenSize().Width/2-400, 
                                                                   100, devices->getVideoDriver()->getScreenSize().Width/2+400, 700));
					characterWindow->setDraggable(true);
					viewPort->setRelativePosition(rect<s32>(5, 75, 400, 350));
					tabCtrl->setRelativePosition(rect<s32>(viewPort->getRelativePosition().UpperLeftCorner.X+viewPort->getRelativePosition().getWidth()+5,
						75, characterWindow->getRelativePosition().getWidth()-10, characterWindow->getRelativePosition().getHeight()-5));
					bgJointNodes->remove();
					devices->setRenderScene(true);
				}
				rotationScrollBar->setRelativePosition(rect<s32>(viewPort->getRelativePosition().UpperLeftCorner.X,
					viewPort->getRelativePosition().UpperLeftCorner.Y+viewPort->getRelativePosition().getHeight()+5,
					viewPort->getRelativePosition().getWidth(),
					viewPort->getRelativePosition().UpperLeftCorner.Y+viewPort->getRelativePosition().getHeight()+20));
				actionsText->setRelativePosition(rect<s32>(viewPort->getRelativePosition().UpperLeftCorner.X,
					viewPort->getRelativePosition().UpperLeftCorner.Y+viewPort->getRelativePosition().getHeight()+25,
					viewPort->getRelativePosition().getWidth()-205,
					viewPort->getRelativePosition().UpperLeftCorner.Y+viewPort->getRelativePosition().getHeight()+45));
				add->setRelativePosition(rect<s32>(actionsText->getRelativePosition().UpperLeftCorner.X+actionsText->getRelativePosition().getWidth()+5,
					actionsText->getRelativePosition().UpperLeftCorner.Y,
					actionsText->getRelativePosition().UpperLeftCorner.X+actionsText->getRelativePosition().getWidth()+105,
					actionsText->getRelativePosition().UpperLeftCorner.Y+20));
				remove->setRelativePosition(rect<s32>(add->getRelativePosition().UpperLeftCorner.X+add->getRelativePosition().getWidth()+5,
					actionsText->getRelativePosition().UpperLeftCorner.Y,
					viewPort->getRelativePosition().getWidth(),
					actionsText->getRelativePosition().UpperLeftCorner.Y+20));
				animationList->setRelativePosition(rect<s32>(actionsText->getRelativePosition().UpperLeftCorner.X,
					actionsText->getRelativePosition().UpperLeftCorner.Y+25,
					viewPort->getRelativePosition().getWidth(),
					characterWindow->getRelativePosition().getHeight()-2));
				framesListBox->setRelativePosition(rect<s32>(5, 5, framesTab->getRelativePosition().getWidth(), 
															 framesTab->getRelativePosition().getHeight()-5));
			}

            s32 id = event.GUIEvent.Caller->getID();
            
            switch (id) {
                //TOOLBAR
                case CXT_EDIT_WINDOW_CHARACTER_EVENTS_OPEN:
                    currentLoad = 1;
                    devices->getGUIEnvironment()->addFileOpenDialog(L"Open Character", true, characterWindow, -1);
                    break;
                    
                case CXT_EDIT_WINDOW_CHARACTER_EVENTS_EDIT: {
                    CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
                    editNode->open(node, "#object:", true);
                }
                    break;
                    
                //LIST BOX BUTTONS
                case CXT_EDIT_WINDOW_CHARACTER_EVENTS_ADD:
                    animationList->addItem(L"New Action");
                    actions.push_back(new CAction());
                    
                    if (animationList->getItemCount() >= 1) {
                        startAction->setEnabled(true);
                        endAction->setEnabled(true);
                        nameAction->setEnabled(true);
                        speedAction->setEnabled(true);
                        previewAction->setEnabled(true);
                    }
                    
                    break;
                    
                case CXT_EDIT_WINDOW_CHARACTER_EVENTS_DELETE:
                    if (animationList->getSelected() != -1) {
                        actions.erase(animationList->getSelected());
                        animationList->removeItem(animationList->getSelected());
                        
                        if (animationList->getItemCount() == 0) {
                            startAction->setEnabled(false);
                            endAction->setEnabled(false);
                            nameAction->setEnabled(false);
                            speedAction->setEnabled(false);
                            previewAction->setEnabled(false);
                        }
                        
                    } else {
                        devices->addWarningDialog(L"Warning", L"Please select an item before", EMBF_OK);
                    }
                    break;
                    
                case CXT_EDIT_WINDOW_CHARACTER_EVENTS_APPLY_SPEED: {
                    stringc speed = "";
                    speed += animationSpeed->getText();
                    node->setAnimationSpeed(devices->getCore()->getF32(speed.c_str()));
                }
                    break;
                    
                case CXT_EDIT_WINDOW_CHARACTER_EVENTS_PREVIEW:
					if (animationList->getSelected() != -1) {
						node->setAnimationSpeed(actions[animationList->getSelected()]->getAnimSpeed());
						node->setFrameLoop(actions[animationList->getSelected()]->getStart(),
										   actions[animationList->getSelected()]->getEnd());
						node->setLoopMode(true);
					}
                    break;
                    
                default:
                    break;
            }

			if (event.GUIEvent.Caller == editBones->getCloseButton()) {
				devices->getObjectPlacement()->setNodeToPlace(0);
				devices->getObjectPlacement()->setArrowVisible(false);
				//devices->getEventReceiver()->RemoveMinimizedWindow(this);
				characterWindow->setVisible(true);
				devices->setSceneManagerToDraw(devices->getSceneManager());
				smgr->setActiveCamera(camera);
				devices->getSecondSceneManager()->setActiveCamera(devices->getMayaCamera());
				devices->getSceneManager()->setActiveCamera(devices->getMayaCamera());
				devices->getDevice()->setInputReceivingSceneManager(devices->getSceneManager());
				devices->setContextName("General");
				editBones->close();
			}
        }

        if (event.GUIEvent.EventType == EGET_FILE_CHOOSE_DIALOG_CANCELLED) {
            currentLoad = 0;
        }
        
		if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
            IGUIListBox *listBox = (IGUIListBox *)event.GUIEvent.Caller;

			if (listBox == animationList && listBox->getSelected() != -1) {
                startAction->setText(devices->getCore()->getStrNumber(actions[animationList->getSelected()]->getStart()).c_str());
                endAction->setText(devices->getCore()->getStrNumber(actions[animationList->getSelected()]->getEnd()).c_str());
                nameAction->setText(actions[animationList->getSelected()]->getName().c_str());
                speedAction->setText(devices->getCore()->getStrNumber(actions[animationList->getSelected()]->getAnimSpeed()).c_str());
            }

			if (listBox == framesListBox) {
				node->setAnimationSpeed(0);
				node->setFrameLoop(framesListBox->getSelected(), framesListBox->getSelected());
				node->setLoopMode(true);
			}

			if (listBox == jointNodesListBox && listBox->getSelected() != -1) {

			}
        }
        
        if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
            IGUIEditBox *editBox = (IGUIEditBox *)event.GUIEvent.Caller;
            
            if (editBox == nameAction) {
                animationList->setItem(animationList->getSelected(), nameAction->getText(), 0);
                actions[animationList->getSelected()]->setName(nameAction->getText());
            }
            
            if (editBox == startAction) {
                stringc start = startAction->getText();
                actions[animationList->getSelected()]->setStart(devices->getCore()->getS32(start.c_str()));
            }
            
            if (editBox == endAction) {
                stringc end = endAction->getText();
                actions[animationList->getSelected()]->setEnd(devices->getCore()->getS32(end.c_str()));
            }
            
            if (editBox == speedAction) {
                stringc speed = speedAction->getText();
                actions[animationList->getSelected()]->setAnimSpeed(devices->getCore()->getF32(speed.c_str()));
            }
        }
        
        if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
            IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
			//SAVE
			if (dialog == saveDialog) {
				exportAnimatedModel();
			}

			if (dialog == openMeshSkinned) {

				if (node->getMesh()->getMeshType() == EAMT_SKINNED) {
					ISkinnedMesh *smesh = (ISkinnedMesh*)node->getMesh();

					IAnimatedMesh *newMesh = devices->getSceneManager()->getMesh(dialog->getFileName());
					if (newMesh->getMeshType() == EAMT_SKINNED) {

						if (smesh->getJointCount() != ((ISkinnedMesh*)newMesh)->getJointCount()+1) {
							devices->addErrorDialog(L"Error", L"Joints number is not equal", EMBF_OK);
							return false;
						}
					}

					ISkinnedMesh *snewMesh = (ISkinnedMesh*)newMesh;

					smesh->useAnimationFrom(snewMesh);
					return false;

					u32 count = snewMesh->getAllJoints().size();

					for (u32 i=1; i < count; i++) {
						for (u32 j=0; j < snewMesh->getAllJoints()[i]->PositionKeys.size(); j++) {
							ISkinnedMesh::SPositionKey *key = smesh->addPositionKey(smesh->getAllJoints()[i]);
							key->frame = snewMesh->getAllJoints()[i]->PositionKeys[j].frame + smesh->getFrameCount();
							key->position = snewMesh->getAllJoints()[i]->PositionKeys[j].position;
						}
						for (u32 j=0; j < snewMesh->getAllJoints()[i]->RotationKeys.size(); j++) {
							ISkinnedMesh::SRotationKey *key = smesh->addRotationKey(smesh->getAllJoints()[i]);
							key->frame = snewMesh->getAllJoints()[i]->RotationKeys[j].frame + smesh->getFrameCount();
							key->rotation = snewMesh->getAllJoints()[i]->RotationKeys[j].rotation;
						}
						for (u32 j=0; j < snewMesh->getAllJoints()[i]->ScaleKeys.size(); j++) {
							ISkinnedMesh::SScaleKey *key = smesh->addScaleKey(smesh->getAllJoints()[i]);
							key->frame = snewMesh->getAllJoints()[i]->ScaleKeys[j].frame + smesh->getFrameCount();
							key->scale = snewMesh->getAllJoints()[i]->ScaleKeys[j].scale;
						}
						/*for (u32 j=0; j < snewMesh->getAllJoints()[i]->Weights.size(); j++) {
							ISkinnedMesh::SWeight *key = smesh->addWeight(smesh->getAllJoints()[i]);
							key->buffer_id = snewMesh->getAllJoints()[i]->Weights[j].buffer_id;
							key->strength = snewMesh->getAllJoints()[i]->Weights[j].strength;
							key->vertex_id = snewMesh->getAllJoints()[i]->Weights[j].vertex_id;
						}*/
					}
					
					//smesh->convertMeshToTangents();
					//smesh->skinMesh();
					smesh->finalize();

					//devices->getSceneManager()->getMeshCache()->removeMesh(newMesh);

					//smesh->setDirty();
					//smesh->skinMesh();
					//smesh->finalize();
					//devices->addInformationDialog(L"", stringw(smesh->getFrameCount()).c_str(), EMBF_OK);

				}
			}
        }
        
        if (event.GUIEvent.EventType == EGET_SCROLL_BAR_CHANGED) {
            s32 id = event.GUIEvent.Caller->getID();
            switch (id) {
                case CXT_EDIT_WINDOW_CHARACTER_EVENTS_ROTATION_SB:
                    if (node) {
                        f32 pos = rotationScrollBar->getPos();
                        node->setRotation(vector3df(node->getRotation().X, pos, node->getRotation().Z));
                    }
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    return false;
}
