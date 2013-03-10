/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "UIMainWindow.h"

CUIMainWindow::CUIMainWindow(CDevices *_devices) {
    devices = _devices;
    devices->getEventReceiver()->AddEventReceiver(this);
    
    //-----------------------------------
    //WINDOW
    mainWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(10, 80, 430, 580), 
                                                                     false, L"Main Window", 0, -1);
    mainWindow->getCloseButton()->remove();
    mainWindow->getMaximizeButton()->setVisible(true);
    isWindowed = true;
    //-----------------------------------
    
    //-----------------------------------
    //TABS
    tabCtrl = devices->getGUIEnvironment()->addTabControl(rect<int>(5, 20, 415, 495), 
                                                          mainWindow, true, true, 0);
    terrainsTab = tabCtrl->addTab(L"Terrains");
    treesTab = tabCtrl->addTab(L"Trees");
    objectsTab = tabCtrl->addTab(L"Objects");
    lightsTab = tabCtrl->addTab(L"Lights");
    dynamicLightsTab = tabCtrl->addTab(L"Volume Lights");
    waterSurfacesTab = tabCtrl->addTab(L"Waters");
    //-----------------------------------
    
    //-----------------------------------
    //LISTS VIEW
    terrainsListBox = devices->getGUIEnvironment()->addListBox(rect<s32>(0, 5, 405, 395), 
                                                              terrainsTab, -1, true);
    treesListBox = devices->getGUIEnvironment()->addListBox(rect<s32>(0, 5, 405, 395), 
                                                               treesTab, -1, true);
    objectsListBox = devices->getGUIEnvironment()->addListBox(rect<s32>(0, 5, 405, 395),
                                                              objectsTab, -1, true);
    lightsListBox = devices->getGUIEnvironment()->addListBox(rect<s32>(0, 5, 405, 395),
                                                              lightsTab, -1, true);
    dynamicListBox = devices->getGUIEnvironment()->addListBox(rect<s32>(0, 5, 405, 395),
                                                             dynamicLightsTab, -1, true);
    waterSurfacesListBox = devices->getGUIEnvironment()->addListBox(rect<s32>(0, 5, 405, 395),
                                                              waterSurfacesTab, -1, true);
    //-----------------------------------
    
    //-----------------------------------
    //BUTTONS
    addTerrain = devices->getGUIEnvironment()->addButton(rect<s32>(5, 400, 185, 435), 
                                                         terrainsTab, CXT_MAIN_WINDOW_EVENTS_ADD_OCTTREE, L"Add a Terrain", 
                                                         L"Add a Terrain Mesh.");
    removeTerrain = devices->getGUIEnvironment()->addButton(rect<s32>(190, 400, 370, 435), 
                                                            terrainsTab, CXT_MAIN_WINDOW_EVENTS_DELETE_OCTTREE, 
                                                            L"Delete This Terrain", L"Delete the selected terrain mesh.");
    
    addTree = devices->getGUIEnvironment()->addButton(rect<s32>(5, 400, 185, 435), 
                                                      treesTab, CXT_MAIN_WINDOW_EVENTS_ADD_TREE, L"Add a Tree", 
                                                      L"Add a Tree Mesh.");
    removeTree = devices->getGUIEnvironment()->addButton(rect<s32>(190, 400, 370, 435), 
                                                         treesTab, CXT_MAIN_WINDOW_EVENTS_DELETE_TREE, 
                                                         L"Delete This Tree", L"Delete the selected tree mesh.");
    
    addObject = devices->getGUIEnvironment()->addButton(rect<s32>(5, 400, 185, 435), 
                                                        objectsTab, CXT_MAIN_WINDOW_EVENTS_ADD_OBJECT, L"Add An Object", 
                                                        L"Add an object mesh.");
    removeObject = devices->getGUIEnvironment()->addButton(rect<s32>(190, 400, 370, 435), 
                                                           objectsTab, CXT_MAIN_WINDOW_EVENTS_DELETE_OBJECT, 
                                                           L"Delete This Object", L"Delete the selected object mesh.");
    
    addLight = devices->getGUIEnvironment()->addButton(rect<s32>(5, 400, 185, 435), 
                                                       lightsTab, CXT_MAIN_WINDOW_EVENTS_ADD_LIGHT, L"Add a Light", 
                                                       L"Add a Light.");
    removeLight = devices->getGUIEnvironment()->addButton(rect<s32>(190, 400, 370, 435), 
                                                          lightsTab, CXT_MAIN_WINDOW_EVENTS_DELETE_LIGHT, 
                                                          L"Delete This Light", L"Delete the selected light.");
    
    addDynamicL = devices->getGUIEnvironment()->addButton(rect<s32>(5, 400, 185, 435), 
                                                         dynamicLightsTab, CXT_MAIN_WINDOW_EVENTS_ADD_DYNAMIC_L, L"Add a Volume Light", 
                                                         L"Add a Volume Light.");
    removeDynamicL = devices->getGUIEnvironment()->addButton(rect<s32>(190, 400, 370, 435), 
                                                            dynamicLightsTab, CXT_MAIN_WINDOW_EVENTS_DELETE_DYNAMIC_L, 
                                                            L"Delete Volume Light", L"Delete the selected volume light.");
    
    addWaterSurface = devices->getGUIEnvironment()->addButton(rect<s32>(5, 400, 185, 435), 
                                                          waterSurfacesTab, CXT_MAIN_WINDOW_EVENTS_ADD_WATER_SURFACE, L"Add a Water Surface", 
                                                          L"Add a water surface.");
    removeWaterSurface = devices->getGUIEnvironment()->addButton(rect<s32>(190, 400, 370, 435), 
                                                             waterSurfacesTab, CXT_MAIN_WINDOW_EVENTS_DELETE_WATER_SURFACE, 
                                                             L"Delete Water Surface", L"Delete the selected water surface.");
    //-----------------------------------
    
    //-----------------------------------
    //WINDOWS INSTANCES
    addOctTreeInstance = new CUIWindowAddOctTree(devices, terrainsListBox);
    
    addTreeInstance = new CUIWindowAddTree(devices, treesListBox);
    
    addObjectInstance = new CUIWindowAddObject(devices, objectsListBox);
    
    addLightInstance = new CUIWindowAddLight(devices, lightsListBox);
    //-----------------------------------
    
    light_icon = devices->getSceneManager()->addBillboardSceneNode(0, dimension2d<f32>(20.f, 20.f), 
                                                                   vector3df(0, 0, 0));
    light_icon->setMaterialTexture(0, devices->getVideoDriver()->getTexture("GUI/light_icon.jpg"));
    light_icon->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
    light_icon->setMaterialFlag(EMF_LIGHTING, false);
    light_icon->setVisible(false);
    light_icon->setName("editor:Light_Icon");
    
    collisionManager = devices->getSceneManager()->getSceneCollisionManager();
    previousNode = 0;
    
}

CUIMainWindow::~CUIMainWindow() {
    
}

void CUIMainWindow::refresh() {
    //TERRAINS
    int selected = terrainsListBox->getSelected();
    terrainsListBox->clear();
    for (int i=0; i < devices->getCoreData()->getTerrainNodes()->size(); i++) {
        stringw name = devices->getCoreData()->getTerrainNodes()->operator[](i)->getName();
        terrainsListBox->addItem(name.c_str());
    }
    terrainsListBox->setSelected(selected);
    
    //TREES
    selected = treesListBox->getSelected();
    treesListBox->clear();
    for (int i=0; i < devices->getCoreData()->getTreeNodes()->size(); i++) {
        stringw name = devices->getCoreData()->getTreeNodes()->operator[](i)->getName();
        treesListBox->addItem(name.c_str());
    }
    treesListBox->setSelected(selected);
    
    //OBJECTS
    selected = objectsListBox->getSelected();
    objectsListBox->clear();
    for (int i=0; i < devices->getCoreData()->getObjectNodes()->size(); i++) {
        stringw name = devices->getCoreData()->getObjectNodes()->operator[](i)->getName();
        objectsListBox->addItem(name.c_str());
    }
    objectsListBox->setSelected(selected);
    
    //LIGHTS
    selected = lightsListBox->getSelected();
    lightsListBox->clear();
    for (int i=0; i < devices->getCoreData()->getLightsNodes()->size(); i++) {
        stringw name = devices->getCoreData()->getLightsNodes()->operator[](i)->getName();
        lightsListBox->addItem(name.c_str());
    }
    lightsListBox->setSelected(selected);

}

IGUIListBox *CUIMainWindow::getActiveListBox() {
    IGUIListBox *listBox;
    
    if (tabCtrl->getActiveTab() == terrainsTab->getNumber()) {
        listBox = terrainsListBox;
    } else if (tabCtrl->getActiveTab() == treesTab->getNumber()) {
        listBox = treesListBox;
    } else if (tabCtrl->getActiveTab() == objectsTab->getNumber()) {
        listBox = objectsListBox;
    } else if (tabCtrl->getActiveTab() == lightsTab->getNumber()) {
        listBox = lightsListBox;
    }
    
    return listBox;
}

ISceneNode *CUIMainWindow::getSelectedNode() {
    ISceneNode *node = 0;
    
    if (tabCtrl->getActiveTab() == terrainsTab->getNumber()) {
        if (terrainsListBox->getSelected() != -1) {
            node = devices->getCoreData()->getTerrainNodes()->operator[](terrainsListBox->getSelected());
        }
    } else if (tabCtrl->getActiveTab() == treesTab->getNumber()) {
        if (treesListBox->getSelected() != -1) {
            node = devices->getCoreData()->getTreeNodes()->operator[](treesListBox->getSelected());
        }
    } else if (tabCtrl->getActiveTab() == objectsTab->getNumber()) {
        if (objectsListBox->getSelected() != -1) {
            node = devices->getCoreData()->getObjectNodes()->operator[](objectsListBox->getSelected());
        }
    } else if (tabCtrl->getActiveTab() == lightsTab->getNumber()) {
        if (lightsListBox->getSelected() != -1) {
            node = devices->getCoreData()->getLightsNodes()->operator[](lightsListBox->getSelected());
        }
    }
    
    return node;
}

void CUIMainWindow::selectSelectedNode(ISceneNode *node) {
    bool founded=false;
    u32 i=0;
    while (!founded && i < devices->getCoreData()->getTreeNodes()->size()) {
        if (devices->getCoreData()->getTreeNodes()->operator[](i) == node) {
            tabCtrl->setActiveTab(treesTab);
            treesListBox->setSelected(i);
            founded = true;
        }
        i++;
    }
    i=0;
    while (!founded && i < devices->getCoreData()->getObjectNodes()->size()) {
        if (devices->getCoreData()->getObjectNodes()->operator[](i) == node) {
            tabCtrl->setActiveTab(objectsTab);
            objectsListBox->setSelected(i);
            founded = true;
        }
        i++;
    }
}

stringc CUIMainWindow::getSelectedNodePrefix(ISceneNode *node) {
    stringc prefix = "";
    
    if (node) {
        stringc nodeName = node->getName();
        int i=0;
        while (i < nodeName.size() && nodeName[i] != ':') {
            prefix.append(nodeName[i]);
            i++;
        }
    }
    
    return prefix;
}

void CUIMainWindow::cloneNode() {
    ISceneNode *node = 0;
    stringw name = L"";
    s32 index;
    if (getSelectedNode()) {
        node = getSelectedNode()->clone();
        if (node) {
            //NODE WAS CLONED BY IRRLICHT
            node->setPosition(devices->getCursorPosition());
            
            if (getActiveListBox() == objectsListBox) {
                index = objectsListBox->getSelected();
                
                devices->getCoreData()->getObjectNodes()->push_back(node);
                devices->getCoreData()->getObjectPaths()->push_back(devices->getCoreData()->getObjectPaths()->operator[](index));
                
                objectsListBox->addItem(name.c_str());
            }
            
            if (getActiveListBox() == lightsListBox) {
                index = lightsListBox->getSelected();
                
                devices->getCoreData()->getLightsNodes()->push_back(node);
                devices->getXEffect()->addShadowLight(devices->getXEffect()->getShadowLight(index));
                
                lightsListBox->addItem(name.c_str());
            }
        } else {
            
            //CLONING MANUALLY THE NODE
            ISceneNode *clonedNode;
            stringw path;
            
            if (getActiveListBox() == terrainsListBox) {
                path = devices->getCoreData()->getTerrainPaths()->operator[](terrainsListBox->getSelected()).c_str();
            }
            if (getActiveListBox() == treesListBox) {
                path = devices->getCoreData()->getTreePaths()->operator[](treesListBox->getSelected()).c_str();
            }
            
            clonedNode = devices->getCore()->clone(getSelectedNode(), path.c_str(), devices->getSceneManager());
            
            if (clonedNode) {
                clonedNode->setPosition(devices->getCursorPosition());
                
                if (getActiveListBox() == terrainsListBox) {
                    devices->getCoreData()->getTerrainNodes()->push_back(clonedNode);
                    devices->getCoreData()->getTerrainPaths()->push_back(path.c_str());
                }
                if (getActiveListBox() == treesListBox) {
                    devices->getCoreData()->getTreeNodes()->push_back(clonedNode);
                    devices->getCoreData()->getTreePaths()->push_back(path.c_str());
                }
                
                devices->addInformationDialog(L"Information", L"The node was cloned\n"
                                              L"But it wasn't cloned thanks to the irrlicht engine clone function\n"
                                              L"Possibile error(s)...", EMBF_OK);
                
            } else {
                devices->addErrorDialog(L"Error", L"The node wasn't cloned", EMBF_OK);
            }
        }
        if (node) {
            //SETTING COLLISIONS
            if (node->getType() == ESNT_OCTREE) {
                devices->getCollisionManager()->setCollisionToAnOctTreeNode(node);
            }
            if (node->getType() == ESNT_ANIMATED_MESH) {
                devices->getCollisionManager()->setCollisionToAnAnimatedNode(node);
            }
            //SETTING SHADOWS
            if (devices->getXEffect()->isNodeShadowed(getSelectedNode(), devices->getXEffectFilterType(), ESM_BOTH)) {
                devices->getXEffect()->addShadowToNode(node, devices->getXEffectFilterType(), ESM_BOTH);
            }
            if (devices->getXEffect()->isNodeShadowed(getSelectedNode(), devices->getXEffectFilterType(), ESM_CAST)) {
                devices->getXEffect()->addShadowToNode(node, devices->getXEffectFilterType(), ESM_CAST);
            }
            if (devices->getXEffect()->isNodeShadowed(getSelectedNode(), devices->getXEffectFilterType(), ESM_RECEIVE)) {
                devices->getXEffect()->addShadowToNode(node, devices->getXEffectFilterType(), ESM_RECEIVE);
            }
        }
    } else {
        devices->addInformationDialog(L"Information", L"Please select an item before", EMBF_OK);
    }
}

bool CUIMainWindow::OnEvent(const SEvent &event) {
    
    //-----------------------------------
    //MOUSE INPUT EVENTS
    if (event.EventType == EET_MOUSE_INPUT_EVENT) {
        if (event.MouseInput.Event == EMIE_MOUSE_MOVED) {
            if (previousNode) {
                previousNode->setMaterialFlag(EMF_WIREFRAME, false);
                //previousNode->setDebugDataVisible(EDS_OFF);
            }
            core::line3d<f32> ray = collisionManager->getRayFromScreenCoordinates(devices->getDevice()->getCursorControl()->getPosition());
            vector3df intersection;
            triangle3df hitTriangle;
            ISceneNode *selectedMouseNode = collisionManager->getSceneNodeAndCollisionPointFromRay(ray, intersection,
                                                                                                   hitTriangle, 0, 0);
            if (selectedMouseNode) {
                devices->getVideoDriver()->setTransform(ETS_WORLD, matrix4());
                if (devices->getCoreData()->getTerrainNodes()->linear_search(selectedMouseNode)) {
                    if (selectedMouseNode->getType() != ESNT_OCTREE && selectedMouseNode->getType() != ESNT_MESH) {
                        selectedMouseNode->setMaterialFlag(EMF_WIREFRAME, true);
                        //selectedMouseNode->setDebugDataVisible(EDS_BBOX);
                    }
                }
                devices->getVideoDriver()->draw3DTriangle(hitTriangle, SColor(255, 255, 0, 255));
                previousNode = selectedMouseNode;
            } else {
                previousNode = 0;
            }
        }
        
        if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP && devices->isShiftPushed()) {
            if (previousNode) {
                selectSelectedNode(previousNode);
            }
        }
    }
    //-----------------------------------
    
    //-----------------------------------
    //KEY INPUT EVENTS
    if (event.EventType == EET_KEY_INPUT_EVENT) {
        if (!event.KeyInput.PressedDown) {
            
            if (event.KeyInput.Key == KEY_KEY_P && devices->isCtrlPushed()) {
                if (!devices->isEditBoxEntered()) {
                    if (devices->getObjectPlacement()->isPlacing()) {
                        if (devices->getObjectPlacement()->getNodeToPlace()) {
                            devices->getObjectPlacement()->setNodeToPlace(0);
                        } else {
                            devices->getObjectPlacement()->setNodeToPlace(getSelectedNode());
                            if (!devices->getObjectPlacement()->getNodeToPlace()) {
                                devices->addWarningDialog(L"Warning", L"Please Select A Node Before...", EMBF_OK);
                            }
                        }
                    }
                }
            }
            
            if (event.KeyInput.Key == KEY_KEY_L && devices->isCtrlPushed()) {
                ISceneNode *node = getSelectedNode();
                if (node) {
                    node->setMaterialFlag(EMF_LIGHTING, !node->getMaterial(0).Lighting);
                } else {
                    devices->addWarningDialog(L"Warning", L"Please Select A Node Before...", EMBF_OK);
                }
            }
            
            if (event.KeyInput.Key == KEY_KEY_C && devices->isCtrlPushed()) {
                cloneNode();
            }
            
            if (event.KeyInput.Key == KEY_RETURN) {
                if (!devices->isEditBoxEntered()) {
                    array<ISceneNode *> nodes = devices->getCoreData()->getAllSceneNodes();
                    for (int i=0; i < nodes.size(); i++) {
                        nodes[i]->setDebugDataVisible(EDS_OFF);
                    }
                    light_icon->setParent(devices->getSceneManager()->getRootSceneNode());
                    light_icon->setVisible(false);
                    devices->getObjectPlacement()->setLightNode(0);
                }
            }
        }
    }
    //-----------------------------------
    
    //-----------------------------------
    //GUI EVENTS
    if (event.EventType == EET_GUI_EVENT && event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
        array<ISceneNode *> nodes = devices->getCoreData()->getAllSceneNodes();
        for (int i=0; i < nodes.size(); i++) {
            nodes[i]->setDebugDataVisible(EDS_OFF);
        }
        if (getSelectedNode()) {
            getSelectedNode()->setDebugDataVisible(EDS_BBOX);
            if (getSelectedNode()->getType() == ESNT_LIGHT) {
                refresh();
                light_icon->setParent(getSelectedNode());
                light_icon->setPosition(vector3df(0, 0, 0));
                light_icon->setVisible(true);
                devices->getObjectPlacement()->setLightNode(getSelectedNode());
            } else {
                light_icon->setParent(devices->getSceneManager()->getRootSceneNode());
                light_icon->setVisible(false);
                devices->getObjectPlacement()->setLightNode(0);
            }
        } else {
            light_icon->setParent(devices->getSceneManager()->getRootSceneNode());
            light_icon->setVisible(false);
        }
    }
    
    if (event.EventType == EET_GUI_EVENT && event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
        
        if (event.GUIEvent.Caller == mainWindow->getMaximizeButton()) {
            if (isWindowed) {
                mainWindow->setRelativePosition(rect<s32>(0, 74, 420, devices->getVideoDriver()->getScreenSize().Height - 16));
                
                tabCtrl->setRelativePosition(rect<int>(5, 20, 415, mainWindow->getRelativePosition().getHeight()-5));
                terrainsListBox->setRelativePosition(rect<s32>(0, 5, 405, mainWindow->getRelativePosition().getHeight() - 105));
                treesListBox->setRelativePosition(rect<s32>(0, 5, 405, mainWindow->getRelativePosition().getHeight() - 105));
                objectsListBox->setRelativePosition(rect<s32>(0, 5, 405, mainWindow->getRelativePosition().getHeight() - 105));
                lightsListBox->setRelativePosition(rect<s32>(0, 5, 405, mainWindow->getRelativePosition().getHeight() - 105));
                dynamicListBox->setRelativePosition(rect<s32>(0, 5, 405, mainWindow->getRelativePosition().getHeight() - 105));
                
                addTerrain->setRelativePosition(rect<s32>(5, tabCtrl->getRelativePosition().getHeight() - 75, 185, tabCtrl->getRelativePosition().getHeight() - 40));
                removeTerrain->setRelativePosition(rect<s32>(190, tabCtrl->getRelativePosition().getHeight() - 75, 370, tabCtrl->getRelativePosition().getHeight() - 40));
                
                addTree->setRelativePosition(rect<s32>(5, tabCtrl->getRelativePosition().getHeight() - 75, 185, tabCtrl->getRelativePosition().getHeight() - 40));
                removeTree->setRelativePosition(rect<s32>(190, tabCtrl->getRelativePosition().getHeight() - 75, 370, tabCtrl->getRelativePosition().getHeight() - 40));
                
                addObject->setRelativePosition(rect<s32>(5, tabCtrl->getRelativePosition().getHeight() - 75, 185, tabCtrl->getRelativePosition().getHeight() - 40));
                removeObject->setRelativePosition(rect<s32>(190, tabCtrl->getRelativePosition().getHeight() - 75, 370, tabCtrl->getRelativePosition().getHeight() - 40));
                
                addLight->setRelativePosition(rect<s32>(5, tabCtrl->getRelativePosition().getHeight() - 75, 185, tabCtrl->getRelativePosition().getHeight() - 40));
                removeLight->setRelativePosition(rect<s32>(190, tabCtrl->getRelativePosition().getHeight() - 75, 370, tabCtrl->getRelativePosition().getHeight() - 40));
                
                addDynamicL->setRelativePosition(rect<s32>(5, tabCtrl->getRelativePosition().getHeight() - 75, 185, tabCtrl->getRelativePosition().getHeight() - 40));
                removeDynamicL->setRelativePosition(rect<s32>(190, tabCtrl->getRelativePosition().getHeight() - 75, 370, tabCtrl->getRelativePosition().getHeight() - 40));
                
                mainWindow->setDraggable(false);
                mainWindow->setDrawTitlebar(false);
            } else {
                mainWindow->setRelativePosition(rect<s32>(10, 80, 430, 580));
                
                tabCtrl->setRelativePosition(rect<int>(5, 20, 415, 495));
                terrainsListBox->setRelativePosition(rect<s32>(0, 5, 405, 395));
                treesListBox->setRelativePosition(rect<s32>(0, 5, 405, 395));
                objectsListBox->setRelativePosition(rect<s32>(0, 5, 405, 395));
                lightsListBox->setRelativePosition(rect<s32>(0, 5, 405, 395));
                dynamicListBox->setRelativePosition(rect<s32>(0, 5, 405, 395));
                
                addTerrain->setRelativePosition(rect<s32>(5, 400, 185, 435));
                removeTerrain->setRelativePosition(rect<s32>(190, 400, 370, 435));
                
                addTree->setRelativePosition(rect<s32>(5, 400, 185, 435));
                removeTree->setRelativePosition(rect<s32>(190, 400, 370, 435));
                
                addObject->setRelativePosition(rect<s32>(5, 400, 185, 435));
                removeObject->setRelativePosition(rect<s32>(190, 400, 370, 435));
                
                addLight->setRelativePosition(rect<s32>(5, 400, 185, 435));
                removeLight->setRelativePosition(rect<s32>(190, 400, 370, 435));
                
                addDynamicL->setRelativePosition(rect<s32>(5, 400, 185, 435));
                removeDynamicL->setRelativePosition(rect<s32>(190, 400, 370, 435));
                
                mainWindow->setDraggable(true);
                mainWindow->setDrawTitlebar(true);
            }
            isWindowed = !isWindowed;
        }
        
        s32 id = event.GUIEvent.Caller->getID();
        
        switch (id) {
                
            //-----------------------------------
            //OCT_TREE
            case CXT_MAIN_WINDOW_EVENTS_ADD_OCTTREE:
                addOctTreeInstance->open();
                break;
                
            case CXT_MAIN_WINDOW_EVENTS_DELETE_OCTTREE:
                if (terrainsListBox->getSelected() != -1) {
                    devices->getXEffect()->removeShadowFromNode(devices->getCoreData()->getTerrainNodes()->operator[](terrainsListBox->getSelected()));
                    //devices->getCoreData()->getTerrainNodes()->operator[](terrainsListBox->getSelected())->removeAll();
                    devices->getCoreData()->getTerrainNodes()->operator[](terrainsListBox->getSelected())->remove();
                    devices->getCoreData()->getTerrainNodes()->erase(terrainsListBox->getSelected());
                    devices->getCoreData()->getTerrainPaths()->erase(terrainsListBox->getSelected());
                    devices->getObjectPlacement()->setNodeToPlace(0);
                    terrainsListBox->removeItem(terrainsListBox->getSelected());
                } else {
                    devices->addInformationDialog(L"Error", L"Please select an item before", EMBF_OK);
                }
                break;
            //-----------------------------------
            //-----------------------------------
            //TREE
            case CXT_MAIN_WINDOW_EVENTS_ADD_TREE:
                addTreeInstance->open();
                break;
                
            case CXT_MAIN_WINDOW_EVENTS_DELETE_TREE:
                if (treesListBox->getSelected() != -1) {
                    //devices->getCoreData()->getTreeNodes()->operator[](treesListBox->getSelected())->removeAll();
                    devices->getXEffect()->removeShadowFromNode(devices->getCoreData()->getTreeNodes()->operator[](treesListBox->getSelected()));
                    devices->getCoreData()->getTreeNodes()->operator[](treesListBox->getSelected())->remove();
                    devices->getCoreData()->getTreeNodes()->erase(treesListBox->getSelected());
                    devices->getCoreData()->getTreePaths()->erase(treesListBox->getSelected());
                    devices->getObjectPlacement()->setNodeToPlace(0);
                    treesListBox->removeItem(treesListBox->getSelected());
                } else {
                    devices->addInformationDialog(L"Error", L"Please select an item before", EMBF_OK);
                }
                break;
            //-----------------------------------
            //-----------------------------------
            //OBJECT
            case CXT_MAIN_WINDOW_EVENTS_ADD_OBJECT:
                addObjectInstance->open();
                break;
                
            case CXT_MAIN_WINDOW_EVENTS_DELETE_OBJECT:
                if (objectsListBox->getSelected() != -1) {
                    devices->getXEffect()->removeShadowFromNode(devices->getCoreData()->getObjectNodes()->operator[](objectsListBox->getSelected()));
                    //devices->getCoreData()->getObjectNodes()->operator[](objectsListBox->getSelected())->removeAll();
                    devices->getCoreData()->getObjectNodes()->operator[](objectsListBox->getSelected())->remove();
                    devices->getCoreData()->getObjectNodes()->erase(objectsListBox->getSelected());
                    devices->getCoreData()->getObjectPaths()->erase(objectsListBox->getSelected());
                    devices->getObjectPlacement()->setNodeToPlace(0);
                    objectsListBox->removeItem(objectsListBox->getSelected());
                } else {
                    devices->addInformationDialog(L"Error", L"Please select an item before", EMBF_OK);
                }
                break;
            //-----------------------------------
            //-----------------------------------
            //LIGHT
            case CXT_MAIN_WINDOW_EVENTS_ADD_LIGHT:
                addLightInstance->open();
                break;
                
            case CXT_MAIN_WINDOW_EVENTS_DELETE_LIGHT:
                if (lightsListBox->getSelected() != -1) {
                    devices->getXEffect()->removeShadowLight(lightsListBox->getSelected());
                    //devices->getCoreData()->getLightsNodes()->operator[](lightsListBox->getSelected())->removeAll();
                    light_icon->setParent(devices->getSceneManager()->getRootSceneNode());
                    devices->getCoreData()->getLightsNodes()->operator[](lightsListBox->getSelected())->remove();
                    devices->getCoreData()->getLightsNodes()->erase(lightsListBox->getSelected());
                    devices->getObjectPlacement()->setNodeToPlace(0);
                    devices->getObjectPlacement()->setLightNode(0);
                    light_icon->setParent(devices->getSceneManager()->getRootSceneNode());
                    light_icon->setVisible(false);
                    
                    lightsListBox->removeItem(lightsListBox->getSelected());
                    lightsListBox->setSelected(-1);
                } else {
                    devices->addInformationDialog(L"Error", L"Please select an item before", EMBF_OK);
                }
                break;
            //-----------------------------------
            //-----------------------------------
            //VOLUME LIGHT
            case CXT_MAIN_WINDOW_EVENTS_ADD_DYNAMIC_L: {
                devices->addInformationDialog(L"Information",
                                              L"Not implanted yes...", EMBF_OK);
            }
                break;
                
            case CXT_MAIN_WINDOW_EVENTS_DELETE_DYNAMIC_L:
                if (dynamicListBox->getSelected() != -1) {
                    
                } else {
                    devices->addInformationDialog(L"Information",
                                                  L"Please select a node before...", EMBF_OK);
                }
                break;
            //-----------------------------------
            default:
                break;
        }
    }
    //-----------------------------------
    
    return false;
}
