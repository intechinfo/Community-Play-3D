//
//  CUIWindowAddOctTree.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 27/09/12.
//
//
#include "stdafx.h"
#include "CUIWindowAddObject.h"

CUIWindowAddObject::CUIWindowAddObject(CDevices *_devices, IGUIListBox *_objectsListBox) {
    devices = _devices;
    devices->getEventReceiver()->AddEventReceiver(this);
    
    objectsListBox = _objectsListBox;
    
    path_file = L"";
    isOpenFileDialogOpened = false;
}

CUIWindowAddObject::~CUIWindowAddObject() {
    
}

void CUIWindowAddObject::open() {
    
    addObjectWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(450, 100, 750, 240), 
                                                            true, L"Add an object mesh (animated)", 0, -1);
    
    devices->getGUIEnvironment()->addStaticText(L"Name : (#object: will be added automatically)", 
                                                rect<s32>(5, 25, 250, 85), false, false, addObjectWindow, -1, false);
    
    addObjectEditBox = devices->getGUIEnvironment()->addEditBox(L"myName", rect<s32>(5, 50, 210, 80), true, 
                                                              addObjectWindow, -1);
    
    devices->getGUIEnvironment()->addButton(rect<s32>(220, 50, 290, 80), addObjectWindow, 
                                            CXT_WINDOW_ADD_OBJECT_EVENTS_SELECT, L"Select", L"Select the mesh");
    
    devices->getGUIEnvironment()->addButton(rect<s32>(5, 110, 70, 135), addObjectWindow, 
                                            CXT_WINDOW_ADD_OBJECT_EVENTS_ACCEPT, L"Accept", 
                                            L"Accept and add the selected mesh");
    
    devices->getGUIEnvironment()->addButton(rect<s32>(80, 110, 145, 135), addObjectWindow, 
                                            CXT_WINDOW_ADD_OBJECT_EVENTS_CLOSE, L"Cancel", L"Close this window");
    
}

bool CUIWindowAddObject::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_GUI_EVENT) {
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            s32 id = event.GUIEvent.Caller->getID();
            switch (id) {
                case CXT_WINDOW_ADD_OBJECT_EVENTS_SELECT:
					devices->createFileOpenDialog(L"Select the mesh", CGUIFileSelector::EFST_OPEN_DIALOG, devices->getGUIEnvironment()->getRootGUIElement(), true);
                    isOpenFileDialogOpened = true;
                    break;
                    
                case CXT_WINDOW_ADD_OBJECT_EVENTS_ACCEPT: {
                    IAnimatedMesh *objectMesh = devices->getSceneManager()->getMesh(path_file.c_str());
                    IAnimatedMeshSceneNode *objectNode = devices->getSceneManager()->addAnimatedMeshSceneNode(objectMesh);
                    
                    if (objectNode) {
						objectNode->setMesh(objectMesh);

						objectNode->setAnimationSpeed(0);
						objectNode->setFrameLoop(0, 0);

                        objectNode->setMaterialFlag(EMF_LIGHTING, false);
                        objectNode->setMaterialFlag(EMF_NORMALIZE_NORMALS, false);
                        
                        stringw objectNodeName = L"#object:";
                        objectNodeName += addObjectEditBox->getText();
                        objectNode->setName(objectNodeName.c_str());
                        
                        devices->getXEffect()->addShadowToNode(objectNode, devices->getXEffectFilterType(), ESM_BOTH);
                        devices->getCollisionManager()->setCollisionToAnAnimatedNode(objectNode);
                        
						SObjectsData odata(objectMesh, objectNode, path_file.c_str());
						devices->getCoreData()->getObjectsData()->push_back(odata);
                        
                        objectsListBox->addItem(objectNodeName.c_str());
                        
                        addObjectWindow->remove();

						devices->getEventReceiver()->sendUserEvent(ECUE_NODE_ADDED, objectNode);
                    } else {
                        devices->addWarningDialog(L"Warning", 
                                                  L"Error when loading the selected mesh \n"
                                                  L"Please verify you 3D model : \n", EMBF_OK);
                    }
                }
                    break;
                    
                case CXT_WINDOW_ADD_OBJECT_EVENTS_CLOSE:
                    addObjectWindow->remove();
                    break;
                    
                default:
                    break;
            }
        }
        
        if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
            if (isOpenFileDialogOpened == true) {
                IGUIFileOpenDialog *dialog = (IGUIFileOpenDialog *)event.GUIEvent.Caller;
                path_file = L"";
                path_file.append(dialog->getFileName());
                isOpenFileDialogOpened = false;
            }
        }
    }
    
    return false;
}
