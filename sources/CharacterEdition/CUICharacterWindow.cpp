//
//  CUICharacterWindow.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 10/10/12.
//
//

#include "CUICharacterWindow.h"

CUICharacterWindow::CUICharacterWindow(CDevices *_devices) {
    devices = _devices;
    
    devices->getEventReceiver()->AddEventReceiver(this);
    
    characterWindow = 0;
    
    currentLoad = 0;
    node = 0;
    
    actions.clear();
}

CUICharacterWindow::~CUICharacterWindow() {
    
}

IGUIWindow *CUICharacterWindow::isWindowActive() {
    return characterWindow;
}

void CUICharacterWindow::open() {
    
    characterWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(devices->getVideoDriver()->getScreenSize().Width/2-400, 
                                                                        100, devices->getVideoDriver()->getScreenSize().Width/2+400, 700),
                                                              false, L"Animated Node Edition Window", 0, -1);
    characterWindow->getCloseButton()->remove();
    
    
    //-----------------------------------
    //MENU
    menu = devices->getGUIEnvironment()->addMenu(characterWindow);
    
    menu->addItem(L"File", -1, true, true);
	menu->addItem(L"Help", -1, true, true);
    
    submenu = menu->getSubMenu(0);
	submenu->addItem(L"Open Saved Animation Configuration", CXT_EDIT_WINDOW_CHARACTER_EVENTS_OPEN);
    submenu->addSeparator();
	submenu->addItem(L"Save", CXT_EDIT_WINDOW_CHARACTER_EVENTS_SAVE);
	submenu->addSeparator();
	submenu->addItem(L"Close", CXT_EDIT_WINDOW_CHARACTER_EVENTS_CLOSE);
    
    submenu = menu->getSubMenu(1);
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
    smgr = devices->getSceneManager()->createNewSceneManager();
    viewPort = new CGUIViewport(devices->getGUIEnvironment(), characterWindow, 1, 
                                              rect<s32>(5, 75, 400, 350), false); 
    if (viewPort) {
        viewPort->setSceneManager(smgr);
        smgr->drop();
        smgr->setAmbientLight(SColorf(0.3f, 0.3f, 0.3f));
        viewPort->setOverrideColor(SColor(255, 0, 0, 0)); 
        
        ICameraSceneNode *camera = smgr->addCameraSceneNode();
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
    devices->getGUIEnvironment()->addStaticText(L"Actions : ", rect<s32>(5, 375, 190, 395), true, true, characterWindow, -1, false);
    add = devices->getGUIEnvironment()->addButton(rect<s32>(195, 375, 295, 395), characterWindow, CXT_EDIT_WINDOW_CHARACTER_EVENTS_ADD, L"Add", L"Add an animation");
    remove = devices->getGUIEnvironment()->addButton(rect<s32>(300, 375, 400, 395), characterWindow, CXT_EDIT_WINDOW_CHARACTER_EVENTS_DELETE, L"Delete", L"Delete this animation");
    
    animationList = devices->getGUIEnvironment()->addListBox(rect<s32>(5, 400, 400, 590), characterWindow, -1, true);
    //-----------------------------------
    
    //-----------------------------------
    //TABS
    tabCtrl = devices->getGUIEnvironment()->addTabControl(rect<int>(405, 75, 795, 595), characterWindow, true, true, 0);
    actionsTab = tabCtrl->addTab(L"Action");
    jointNodesTab = tabCtrl->addTab(L"Joint Nodes");
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
}

void CUICharacterWindow::setModel(IAnimatedMeshSceneNode *pnode, s32 index) {
    if (pnode) {
        if (node) {
            node->remove();
            node = 0;
        }
        IAnimatedMesh *mesh = smgr->getMesh(devices->getCoreData()->getObjectPaths()->operator[](index));
		node = smgr->addAnimatedMeshSceneNode(mesh);
        node->setPosition(pnode->getPosition());
        node->setRotation(pnode->getRotation());
        node->setScale(pnode->getScale());
        node->setMaterialFlag(EMF_LIGHTING, false);
        
        for (int i=0; i < pnode->getMaterialCount(); i++) {
            for (int a=0; a < 4; a++) {
                node->getMaterial(i).TextureLayer[a] = pnode->getMaterial(i).TextureLayer[a];
            }
        }
        
        node->setName(pnode->getName());
        node->setDebugDataVisible(EDS_SKELETON);
        node->setMaterialFlag(EMF_WIREFRAME, true);
        
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
}

void CUICharacterWindow::exportAnimatedModel() {
    
}

bool CUICharacterWindow::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_GUI_EVENT) {
        
        if (event.GUIEvent.EventType == EGET_MENU_ITEM_SELECTED) {
            
            IGUIContextMenu *tempMenu = (IGUIContextMenu *)event.GUIEvent.Caller;
            
            switch (tempMenu->getItemCommandId(tempMenu->getSelectedItem())) {
             
                //FILE
                case CXT_EDIT_WINDOW_CHARACTER_EVENTS_CLOSE:
                    characterWindow->remove();
                    viewPort->remove();
                    if (node) {
                        node->removeAll();
                        node->remove();
                        node = 0;
                    }
                    characterWindow = 0;
                    actions.clear();
                    break;

				case CXT_EDIT_WINDOW_CHARACTER_EVENTS_SAVE:
					saveDialog = devices->createFileOpenDialog(L"Select the directory", CGUIFileSelector::EFST_SAVE_DIALOG, characterWindow);
					break;
                    
                default:
                    break;
            }
        }
        
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            s32 id = event.GUIEvent.Caller->getID();
            
            switch (id) {
                //TOOLBAR
                case CXT_EDIT_WINDOW_CHARACTER_EVENTS_OPEN:
                    currentLoad = 1;
                    devices->getGUIEnvironment()->addFileOpenDialog(L"Open Character", true, characterWindow, -1);
                    break;
                    
                case CXT_EDIT_WINDOW_CHARACTER_EVENTS_EDIT: {
                    CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
                    editNode->open(node, "#object:");
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
                    node->setAnimationSpeed(actions[animationList->getSelected()]->getAnimSpeed());
                    node->setFrameLoop(actions[animationList->getSelected()]->getStart(),
                                       actions[animationList->getSelected()]->getEnd());
                    break;
                    
                default:
                    break;
            }
        }
        
        if (event.GUIEvent.EventType == EGET_FILE_CHOOSE_DIALOG_CANCELLED) {
            currentLoad = 0;
        }
        
        if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
            IGUIListBox *listBox = (IGUIListBox *)event.GUIEvent.Caller;
            
            if (listBox == animationList) {
                startAction->setText(devices->getCore()->getStrNumber(actions[animationList->getSelected()]->getStart()).c_str());
                endAction->setText(devices->getCore()->getStrNumber(actions[animationList->getSelected()]->getEnd()).c_str());
                nameAction->setText(actions[animationList->getSelected()]->getName().c_str());
                speedAction->setText(devices->getCore()->getStrNumber(actions[animationList->getSelected()]->getAnimSpeed()).c_str());
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
