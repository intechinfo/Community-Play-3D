//
//  CUIWindowAddOctTree.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 27/09/12.
//
//
#include "stdafx.h"
#include "CUIWindowAddTree.h"

CUIWindowAddTree::CUIWindowAddTree(CDevices *_devices, IGUIListBox *_treesListBox) {
    devices = _devices;
    devices->getEventReceiver()->AddEventReceiver(this);
    
    treesListBox = _treesListBox;
    
    path_file = L"";
    isOpenFileDialogOpened = false;
}

CUIWindowAddTree::~CUIWindowAddTree() {
    
}

void CUIWindowAddTree::open() {
    
    addTreeWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(450, 100, 750, 240), 
                                                               true, L"Add a terrain mesh", 0, -1);
    
    devices->getGUIEnvironment()->addStaticText(L"Name : (#tree: will be added automatically)", 
                                                rect<s32>(5, 25, 250, 85), false, false, addTreeWindow, -1, false);
    
    addTreeEditBox = devices->getGUIEnvironment()->addEditBox(L"myName", rect<s32>(5, 50, 210, 80), true, 
                                                                 addTreeWindow, -1);
    
    devices->getGUIEnvironment()->addButton(rect<s32>(220, 50, 290, 80), addTreeWindow, 
                                            CXT_WINDOW_ADD_TREE_EVENTS_SELECT, L"Select", L"Select the mesh");
    
    devices->getGUIEnvironment()->addButton(rect<s32>(5, 110, 70, 135), addTreeWindow, 
                                            CXT_WINDOW_ADD_TREE_EVENTS_ACCEPT, L"Accept", 
                                            L"Accept and add the selected mesh");
    
    devices->getGUIEnvironment()->addButton(rect<s32>(80, 110, 145, 135), addTreeWindow, 
                                            CXT_WINDOW_ADD_TREE_EVENTS_CLOSE, L"Cancel", L"Close this window");
    
}

bool CUIWindowAddTree::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_GUI_EVENT) {
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            s32 id = event.GUIEvent.Caller->getID();
            switch (id) {
                case CXT_WINDOW_ADD_TREE_EVENTS_SELECT:
                    devices->createFileOpenDialog(L"Select the mesh", 0);
                    isOpenFileDialogOpened = true;
                    break;
                    
                case CXT_WINDOW_ADD_TREE_EVENTS_ACCEPT: {
                    IMesh *octTreeMesh = devices->getSceneManager()->getMesh(path_file.c_str());
                    IMeshSceneNode *treeNode = devices->getSceneManager()->addOctreeSceneNode(octTreeMesh, 0, -1, 256);
                    if (treeNode) {
						treeNode->setMesh(octTreeMesh);
                        treeNode->setMaterialFlag(EMF_LIGHTING, false);
                        treeNode->setMaterialFlag(EMF_NORMALIZE_NORMALS, false);
                        
                        stringw treeNodeName = L"#tree:";
                        treeNodeName += addTreeEditBox->getText();
                        treeNode->setName(treeNodeName.c_str());
                        
                        devices->getXEffect()->addShadowToNode(treeNode, devices->getXEffectFilterType(), ESM_BOTH);
                        devices->getCollisionManager()->setCollisionToAnOctTreeNode(treeNode);
                        STreesData tdata(octTreeMesh, treeNode, path_file.c_str());

                        devices->getCoreData()->getTreesData()->push_back(tdata);
                        
                        treesListBox->addItem(treeNodeName.c_str());
                        
                        addTreeWindow->remove();
                    } else {
                        devices->addWarningDialog(L"Warning", 
                                                  L"Error when loading the selected mesh \n"
                                                  L"Please verify you 3D model : \n", EMBF_OK);
                    }
                }
                    break;
                    
                case CXT_WINDOW_ADD_TREE_EVENTS_CLOSE:
                    addTreeWindow->remove();
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
