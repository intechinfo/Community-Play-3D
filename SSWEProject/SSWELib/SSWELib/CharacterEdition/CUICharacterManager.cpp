//
//  CUICharacterWindow.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 10/10/12.
//
//

#include "stdafx.h"
#include "CUICharacterManager.h"

void CUICharacterManager::setFullScreen() {
	characterWindow->setRelativePosition(rect<s32>(0, 75, devices->getVideoDriver()->getScreenSize().Width+2,
												   devices->getVideoDriver()->getScreenSize().Height));
	characterWindow->setDraggable(false);
	viewPort->setRelativePosition(rect<s32>(5, 75, 600, 550));
	tabCtrl->setRelativePosition(rect<s32>(viewPort->getRelativePosition().UpperLeftCorner.X+viewPort->getRelativePosition().getWidth()+5,
										   75, characterWindow->getRelativePosition().getWidth()-10, 400));
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
	devices->setRenderScene(false);
}

CUICharacterManager::CUICharacterManager(CDevices *_devices, IAnimatedMesh *mesh) {
    devices = _devices;
	this->mesh = mesh;
    
    devices->getEventReceiver()->AddEventReceiver(this);
    
    characterWindow = 0;

    node = 0;
    
    actions.clear();
	meshes.clear();
}

CUICharacterManager::~CUICharacterManager() {

}

void CUICharacterManager::open() {
    
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
	submenu->addItem(L"Open Saved Animation Configuration", -1);
    submenu->addSeparator();
	submenu->addItem(L"Save", CXT_CHARACTER_MANAGER_MENU_SAVE);
	submenu->addSeparator();
	submenu->addItem(L"Close", CXT_CHARACTER_MANAGER_MENU_CLOSE);
    
	submenu = menu->getSubMenu(1);
	submenu->addItem(L"Enter Bones Edition...", -1);
	submenu->addSeparator();
	submenu->addItem(L"Add Frames from other model...", -1);

    submenu = menu->getSubMenu(2);
	submenu->addItem(L"How to ?", -1);
    
    //-----------------------------------
    
    //-----------------------------------
    //TOOLBAR
    bar = devices->getGUIEnvironment()->addToolBar(characterWindow, -1);
    bar->setRelativePosition(position2di(0, 40));
    bar->setRelativePosition(rect<s32>(0, 43, 795, 70));

	ITexture* image = devices->getVideoDriver()->getTexture(devices->getWorkingDirectory() + stringc("/GUI/folder.png"));
	bar->addButton(-1, 0, L"Open Saved Animation Configuration", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture(devices->getWorkingDirectory() + stringc("/GUI/edit.png"));
	bar->addButton(-1, 0, L"Edit The Node", image, 0, false, true);
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
    //-----------------------------------
    
    //-----------------------------------
    //BUTTONS AND LISTBOX
    actionsText = devices->getGUIEnvironment()->addStaticText(L"Actions : ", rect<s32>(5, 375, 190, 395), true, true, characterWindow, -1, false);
    add = devices->getGUIEnvironment()->addButton(rect<s32>(195, 375, 295, 395), characterWindow, -1, L"Add", L"Add an animation");
    remove = devices->getGUIEnvironment()->addButton(rect<s32>(300, 375, 400, 395), characterWindow, -1, L"Delete", L"Delete this animation");
    
    animationList = devices->getGUIEnvironment()->addListBox(rect<s32>(5, 400, 400, 590), characterWindow, -1, true);
    //-----------------------------------
    
    //-----------------------------------
    //TABS
    tabCtrl = devices->getGUIEnvironment()->addTabControl(rect<int>(405, 75, 795, 595), characterWindow, true, true, 0);
    actionsTab = tabCtrl->addTab(L"Action");
	animationsTab = tabCtrl->addTab(L"Animations");
    detailsTab = tabCtrl->addTab(L"Details");
    //-----------------------------------
    
    //-----------------------------------
    //ACTIONS TAB
    previewAction = devices->getGUIEnvironment()->addButton(rect<s32>(10, 20, 110, 45), actionsTab, -1, L"Preview", L"Preview Action");
    
    devices->getGUIEnvironment()->addStaticText(L"Start Frame : ", rect<s32>(10, 60, 110, 80), false, true, actionsTab, -1, false);
    startAction = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(120, 60, 320, 80), true, actionsTab, -1);
    devices->getGUIEnvironment()->addStaticText(L"End Frame : ", rect<s32>(10, 90, 110, 110), false, true, actionsTab, -1, false);
    endAction = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(120, 90, 320, 110), true, actionsTab, -1);
    devices->getGUIEnvironment()->addStaticText(L"Name : ", rect<s32>(10, 120, 110, 140), false, true, actionsTab, -1, false);
    nameAction = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(120, 120, 320, 140), true, actionsTab, -1);
    
    devices->getGUIEnvironment()->addStaticText(L"Anim Speed : ", rect<s32>(10, 180, 110, 200), false, true, actionsTab, -1, false);
    speedAction =  devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(120, 180, 320, 200), true, actionsTab, -1);

	devices->getGUIEnvironment()->addStaticText(L"Animation : ", rect<s32>(10, 220, 110, 240), false, true, actionsTab, -1, false);
    selectedAnimationcb =  devices->getGUIEnvironment()->addComboBox(rect<s32>(120, 220, 360, 240), actionsTab, -1);
	selectedAnimationcb->addItem(L"Current");
    //-----------------------------------

	animationslb = devices->getGUIEnvironment()->addListBox(rect<s32>(15, 35, 375, 175), animationsTab, -1, true);
	addAnimationb = devices->getGUIEnvironment()->addButton(rect<s32>(15, 15, 35, 35), animationsTab, -1, L"+", L"Add Animation");
	deleteAnimationb = devices->getGUIEnvironment()->addButton(rect<s32>(35, 15, 55, 35), animationsTab, -1, L"-", L"Delete Animation");

	setFullScreen();
}

void CUICharacterManager::setModel(IAnimatedMeshSceneNode *pnode) {
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
        
        node->setName(pnode->getName());
        node->setDebugDataVisible(EDS_SKELETON);
        
        updateDetails();

    }

	if (node) {
		SObjectsData *sdatat = (SObjectsData*)devices->getCoreData()->getISDataOfSceneNode(pnode);
		if (sdatat) {
			for (u32 i=1; i < sdatat->getAnimationMeshes()->size(); i++) {
				meshes.push_back(sdatat->getAnimationMeshes()->operator[](i));
				animationslb->addItem(stringw(devices->getSceneManager()->getMeshCache()->getMeshName(sdatat->getAnimationMeshes()->operator[](i))).c_str());
				selectedAnimationcb->addItem(stringw(devices->getSceneManager()->getMeshCache()->getMeshName(sdatat->getAnimationMeshes()->operator[](i))).c_str());
			}
			for (u32 i=0; i < sdatat->getActions()->size(); i++) {
				actions.push_back(sdatat->getActions()->operator[](i));
				animationList->addItem(sdatat->getActions()->operator[](i)->getName().c_str());
			}
		}
	}
    
	if (animationList->getItemCount() == 0) {
		startAction->setEnabled(false);
		endAction->setEnabled(false);
		nameAction->setEnabled(false);
		speedAction->setEnabled(false);
		previewAction->setEnabled(false);
		selectedAnimationcb->setEnabled(false);
	}
}

void CUICharacterManager::updateDetails() {
	/*core::list<IGUIElement *>::ConstIterator it = detailsTab->getChildren().begin();
	for (; it != detailsTab->getChildren().end() != 0 && detailsTab->getChildren().size() != 0; ++it) {
		if ((*it) != 0)
			(*it)->remove();
	}*/

	devices->getGUIEnvironment()->addStaticText(L"Animation Speed (frames/s) : ", rect<s32>(5, 15, 200, 35), false, true, detailsTab, -1, true);
    animationSpeed = devices->getGUIEnvironment()->addEditBox(L"0", rect<s32>(210, 15, 280, 35), true, detailsTab, -10);
    animationSpeed->setText(devices->getCore()->getStrNumber(node->getAnimationSpeed()).c_str());
    applyAnimationSpeed = devices->getGUIEnvironment()->addButton(rect<s32>(290, 15, 380, 35), detailsTab, -1, L"Apply");

	stringw frameCount = L"Frames Count : ";
	frameCount += ((ISkinnedMesh*)mesh)->getFrameCount();
    devices->getGUIEnvironment()->addStaticText(frameCount.c_str(), rect<s32>(5, 40, 200, 60), false, true, detailsTab, -1, true);
    stringw startFrame = L"Start Frame : ";
    startFrame += node->getStartFrame();
    devices->getGUIEnvironment()->addStaticText(startFrame.c_str(), rect<s32>(5, 65, 200, 85), false, true, detailsTab, -1, true);
    stringw endFrame = L"End Frame : ";
    endFrame += mesh->getFrameCount();
    devices->getGUIEnvironment()->addStaticText(endFrame.c_str(), rect<s32>(5, 90, 200, 110), false, true, detailsTab, -1, true);
    stringw jointCount = L"Joint Count : ";
    jointCount += node->getJointCount();
    devices->getGUIEnvironment()->addStaticText(jointCount.c_str(), rect<s32>(5, 115, 200, 135), false, true, detailsTab, -1, true);
    stringw nodeName = L"Object name : ";
    nodeName += node->getName();
    devices->getGUIEnvironment()->addStaticText(nodeName.c_str(), rect<s32>(5, 140, 200, 160), false, true, detailsTab, -1, true);
}

void CUICharacterManager::exportAnimatedModel() {
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

bool CUICharacterManager::OnEvent(const SEvent &event) {

    if (event.EventType == EET_GUI_EVENT) {
        
        if (event.GUIEvent.EventType == EGET_MENU_ITEM_SELECTED) {
            IGUIContextMenu *tempMenu = (IGUIContextMenu *)event.GUIEvent.Caller;
            switch (tempMenu->getItemCommandId(tempMenu->getSelectedItem())) {
                //FILE
                case CXT_CHARACTER_MANAGER_MENU_CLOSE:
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
					devices->setRenderScene(true);
					delete this;
					return true;
                    break;

				case CXT_CHARACTER_MANAGER_MENU_SAVE:
					saveDialog = devices->createFileOpenDialog(L"Select the directory", CGUIFileSelector::EFST_SAVE_DIALOG, characterWindow);
					break;
                    
                default:
                    break;
            }
        }

        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {

			//ACTIONS
			if (event.GUIEvent.Caller == add) {
				animationList->addItem(L"New Action");
                actions.push_back(new CAction());
                    
                if (animationList->getItemCount() >= 1) {
                    startAction->setEnabled(true);
                    endAction->setEnabled(true);
                    nameAction->setEnabled(true);
                    speedAction->setEnabled(true);
                    previewAction->setEnabled(true);
					selectedAnimationcb->setEnabled(true);
                }
			}
			if (event.GUIEvent.Caller == remove) {
				if (animationList->getSelected() != -1) {
					actions.erase(animationList->getSelected());
					animationList->removeItem(animationList->getSelected());
                        
					if (animationList->getItemCount() == 0) {
						startAction->setEnabled(false);
						endAction->setEnabled(false);
						nameAction->setEnabled(false);
						speedAction->setEnabled(false);
						previewAction->setEnabled(false);
						selectedAnimationcb->setEnabled(false);
					}
				}
			}
			if (event.GUIEvent.Caller == previewAction) {
				node->setAnimationSpeed(actions[animationList->getSelected()]->getAnimSpeed());
				node->setFrameLoop(actions[animationList->getSelected()]->getStart(),
									actions[animationList->getSelected()]->getEnd());
				node->setLoopMode(true);
			}

			//ANIMATIONS
			if (event.GUIEvent.Caller == addAnimationb) {
				addAnimationDialog = devices->createFileOpenDialog(L"Select mesh", CGUIFileSelector::EFST_OPEN_DIALOG, 0, true);
			}
			if (event.GUIEvent.Caller == deleteAnimationb) {
				if (animationslb->getSelected() != -1) {
					selectedAnimationcb->removeItem(animationslb->getSelected());
					meshes.erase(animationslb->getSelected());
					animationslb->removeItem(animationslb->getSelected());
				}
			}

			//DETAIL
			if (event.GUIEvent.Caller == applyAnimationSpeed) {
				stringc speed = "";
                speed += animationSpeed->getText();
                node->setAnimationSpeed(devices->getCore()->getF32(speed.c_str()));
			}

        }
        
		if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
            IGUIListBox *listBox = (IGUIListBox *)event.GUIEvent.Caller;

			if (listBox == animationList && listBox->getSelected() != -1) {
                startAction->setText(devices->getCore()->getStrNumber(actions[animationList->getSelected()]->getStart()).c_str());
                endAction->setText(devices->getCore()->getStrNumber(actions[animationList->getSelected()]->getEnd()).c_str());
                nameAction->setText(actions[animationList->getSelected()]->getName().c_str());
                speedAction->setText(devices->getCore()->getStrNumber(actions[animationList->getSelected()]->getAnimSpeed()).c_str());
				if (actions[animationList->getSelected()]->getAnimationPath() == "" || actions[animationList->getSelected()]->getAnimationPath() == "Current") {
					((ISkinnedMesh*)node->getMesh())->useAnimationFrom((ISkinnedMesh*)node->getMesh());
					selectedAnimationcb->setSelected(0);
				} else {
					for (u32 i=0; i < meshes.size(); i++) {
						if (actions[animationList->getSelected()]->getAnimationPath() == devices->getSceneManager()->getMeshCache()->getMeshName(meshes[i]).getPath()) {
							selectedAnimationcb->setSelected(i);
							((ISkinnedMesh*)node->getMesh())->useAnimationFrom((ISkinnedMesh*)meshes[i]);
						}
					}
				}
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

		if (event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED) {
			if (event.GUIEvent.Caller == selectedAnimationcb) {
				if (node->getMesh()->getMeshType() != EAMT_SKINNED)
					return false;

				if (selectedAnimationcb->getSelected() == 0) {
					((ISkinnedMesh*)node->getMesh())->useAnimationFrom((ISkinnedMesh*)mesh);
				} else {
					((ISkinnedMesh*)node->getMesh())->useAnimationFrom((ISkinnedMesh*)meshes[selectedAnimationcb->getSelected()-1]);
				}

				actions[animationList->getSelected()]->setAnimationPath(selectedAnimationcb->getItem(selectedAnimationcb->getSelected()));

				updateDetails();
			}
		}
        
        if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
            IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
			//SAVE
			if (dialog == saveDialog) {
				exportAnimatedModel();
			}
			if (dialog == addAnimationDialog) {
				IAnimatedMesh *smesh = devices->getSceneManager()->getMesh(dialog->getFileName());
				if (smesh) {
					if (smesh->getMeshType() == EAMT_SKINNED) {
						meshes.push_back(smesh);
						stringw txt = dialog->getFileName();
						animationslb->addItem(txt.c_str());
						selectedAnimationcb->addItem(txt.c_str());
					}
				} else {
					devices->addErrorDialog(L"Error", L"Canno load the mesh", EMBF_OK);
				}
			}

        }
    }
    
    return false;
}
