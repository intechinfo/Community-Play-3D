//
//  CUIWindowAddOctTree.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 27/09/12.
//
//

#include "CUIWindowAddTerrain.h"

CUIWindowAddOctTree::CUIWindowAddOctTree(CDevices *_devices, IGUIListBox *_terrainsListBox) {
    devices = _devices;
    devices->getEventReceiver()->AddEventReceiver(this);
    
    terrainsListBox = _terrainsListBox;
    
    path_file = L"";
    isOpenFileDialogOpened = false;
}

CUIWindowAddOctTree::~CUIWindowAddOctTree() {
    
}

void CUIWindowAddOctTree::open() {
    
    addOctTreeWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(450, 100, 750, 240), 
                                                               true, L"Add a terrain mesh", 0, -1);
    
    devices->getGUIEnvironment()->addStaticText(L"Name : (#map: will be added automatically)", 
                                                rect<s32>(5, 25, 250, 85), false, false, addOctTreeWindow, -1, false);
    
    addOctTreeEditBox = devices->getGUIEnvironment()->addEditBox(L"myName", rect<s32>(5, 50, 210, 80), true, 
                                                                 addOctTreeWindow, -1);
    
    devices->getGUIEnvironment()->addButton(rect<s32>(220, 50, 290, 80), addOctTreeWindow, 
                                            CXT_WINDOW_ADD_OCT_TREE_EVENTS_SELECT, L"Select", L"Select the mesh");
    
    devices->getGUIEnvironment()->addButton(rect<s32>(5, 110, 70, 135), addOctTreeWindow, 
                                            CXT_WINDOW_ADD_OCT_TREE_EVENTS_ACCEPT, L"Accept", 
                                            L"Accept and add the selected mesh");
    
    devices->getGUIEnvironment()->addButton(rect<s32>(80, 110, 145, 135), addOctTreeWindow, 
                                            CXT_WINDOW_ADD_OCT_TREE_EVENTS_CLOSE, L"Cancel", L"Close this window");
    
}

bool CUIWindowAddOctTree::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_GUI_EVENT) {
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            s32 id = event.GUIEvent.Caller->getID();
            switch (id) {
                case CXT_WINDOW_ADD_OCT_TREE_EVENTS_SELECT:
                    devices->createFileOpenDialog(L"Select the mesh");
                    isOpenFileDialogOpened = true;
                    break;
                    
                case CXT_WINDOW_ADD_OCT_TREE_EVENTS_ACCEPT: {
                    IMesh *octTreeMesh;
                    ISceneNode *octTreeNode;
                    
                    io::path extension;
                    core::getFileNameExtension(extension, path_file);
                    extension.make_lower();
                    
                    if (extension == ".jpg" || extension == ".png") {
                        octTreeNode = devices->getSceneManager()->addTerrainSceneNode(path_file, 0, -1, vector3df(0.f, 0.f, 0.f),
                                                                                      vector3df(0.f, 0.f, 0.f), vector3df(1.f, 0.1f, 1.f),
                                                                                      SColor (255, 255, 255, 255), 5, ETPS_17, 4);
                        ((ITerrainSceneNode *)octTreeNode)->setDynamicSelectorUpdate(true);
                    } else {
                        octTreeMesh = devices->getSceneManager()->getMesh(path_file.c_str());
                        octTreeNode = devices->getSceneManager()->addOctreeSceneNode(octTreeMesh, 0, -1, 1024);
                    }
                    if (octTreeNode) {
                        octTreeNode->setMaterialFlag(EMF_LIGHTING, false);
                        octTreeNode->setMaterialFlag(EMF_NORMALIZE_NORMALS, false);
                        
                        stringw octTreeNodeName = L"#map:";
                        octTreeNodeName += addOctTreeEditBox->getText();
                        octTreeNode->setName(octTreeNodeName.c_str());
                        
                        devices->getXEffect()->addShadowToNode(octTreeNode, devices->getXEffectFilterType());
                        devices->getCollisionManager()->setCollisionToAnOctTreeNode(octTreeNode);
                        
                        devices->getCoreData()->getTerrainNodes()->push_back(octTreeNode);
                        devices->getCoreData()->getTerrainPaths()->push_back(path_file);
                        
                        terrainsListBox->addItem(octTreeNodeName.c_str());
                        
                        addOctTreeWindow->remove();
                    } else {
                        devices->addWarningDialog(L"Warning", 
                                                  L"Error when loading the selected mesh \n\n"
                                                  L"Please verify you 3D model : \n", EMBF_OK);
                    }
                }
                    break;
                    
                case CXT_WINDOW_ADD_OCT_TREE_EVENTS_CLOSE:
                    addOctTreeWindow->remove();
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
