/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "UIContextMenu.h"

CUIContextMenu::CUIContextMenu(CDevices *_devices) {
    devices = _devices;
    
    mainWindowInstance = new CUIMainWindow(devices);
    
    sceneViewInstance = new CUISceneView(devices);
    
    editGridInstance = new CUIWindowEditGrid(devices);
    
    characterWindowInstance = new CUICharacterWindow(devices);
    
    openSceneInstance = new CUIWindowOpenScene(devices);
    
    exportSceneInstance = new CUIWindowExportScene(devices);
    
    //-----------------------------------
    //MENU
    menu = devices->getGUIEnvironment()->addMenu();
    
    menu->addItem(L"World Editor", CXT_MENU_EVENTS_WORLD_EDITOR, false, false, false, false);
    menu->addItem(L"File", -1, true, true);
	menu->addItem(L"Edit", -1, true, true);
	menu->addItem(L"View", -1, true, true);
	menu->addItem(L"Animators", -1, true, true);
	menu->addItem(L"Renders", -1, true, true);
    menu->addItem(L"Shaders", -1, true, true);
    menu->addItem(L"Animated Objects", -1, true, true);
    menu->addItem(L"Window", -1, true, true);
	menu->addItem(L"Help", -1, true, true);
    
    int i=0;
    
    submenu = menu->getSubMenu(i++);
    
    submenu = menu->getSubMenu(i++);
	submenu->addItem(L"Open a script (CTRL+O)", CXT_MENU_EVENTS_OPEN_SCRIPT);
	submenu->addItem(L"Export this scene", CXT_MENU_EVENTS_FILE_EXPORT_SCENE);
	submenu->addItem(L"Clean the scene", CXT_MENU_EVENTS_FILE_CLEAN_SCENE);
    submenu->addSeparator();
	submenu->addItem(L"Render", CXT_MENU_EVENTS_FILE_RENDER);
	submenu->addSeparator();
	submenu->addItem(L"Quit", CXT_MENU_EVENTS_FILE_QUIT);
    
    submenu = menu->getSubMenu(i++);
	submenu->addItem(L"Edit Selected Node (CTRL+E)", CXT_MENU_EVENTS_EDIT_EDIT_SELECTED_NODE);
    submenu->addSeparator();
	submenu->addItem(L"Set all the nodes lighting", CXT_MENU_EVENTS_EDIT_SET_ALL_NODES_LIGHTING);
	submenu->addItem(L"Set all the nodes not lighting", CXT_MENU_EVENTS_EDIT_SET_ALL_NODES_NOT_LIGHTING);
    submenu->addSeparator();
    submenu->addItem(L"Edit Grid", CXT_MENU_EVENTS_EDIT_GRID_SCENE_NODE);
    submenu->addSeparator();
    submenu->addItem(L"Clone", CXT_MENU_EVENTS_EDIT_CLONE);
    
    submenu = menu->getSubMenu(i++);
	submenu->addItem(L"Maya Camera", CXT_MENU_EVENTS_VIEW_MAYA_CAMERA);
	submenu->addItem(L"FPS Camera", CXT_MENU_EVENTS_VIEW_FPS_CAMERA);
    submenu->addSeparator();
	submenu->addItem(L"View Tree Node Window", CXT_MENU_EVENTS_VIEW_VIEW_TREE_NODES_WINDOW);
    submenu->addSeparator();
	submenu->addItem(L"Wire Frame", CXT_MENU_EVENTS_VIEW_WIREFRAME, true, false, false, false);
    submenu->addItem(L"Point Cloud", CXT_MENU_EVENTS_VIEW_POINTCLOUD, true, false, false, false);
    
    i++;//TO CHANGE
    
    submenu = menu->getSubMenu(i);
    submenu->addItem(L"HDR", -1, true, true);
	submenu->addItem(L"Effects", -1, true, true);
    submenu = submenu->getSubMenu(0);
    submenu->addItem(L"Draw HDR", CXT_MENU_EVENTS_RENDERS_HDR_DRAW);
    submenu->addSeparator();
    submenu->addItem(L"Edit HDR", CXT_MENU_EVENTS_RENDERS_HDR_EDIT);
    submenu = menu->getSubMenu(i);
	submenu = submenu->getSubMenu(1);
    submenu->addItem(L"Draw Effects (CTRL+X)", CXT_MENU_EVENTS_RENDERS_XEFFECT_DRAW);
    submenu->addSeparator();
    submenu->addItem(L"Edit (CTRL+SHIFT+X)", CXT_MENU_EVENTS_RENDERS_XEFFECT_EDIT);
    i++;
    
    i++;//TO CHANGE
    
    submenu = menu->getSubMenu(i++);
	submenu->addItem(L"Animated Models Window Edition (CTRL+A)", CXT_MENU_EVENTS_ANIMATED_MODELS_WINDOW_EDITION);
	submenu->addItem(L"Simple Edition", CXT_MENU_EVENTS_SIMPLE_EDITION);
    
    submenu = menu->getSubMenu(i++);
	submenu->addItem(L"Current Rendering Infos", CXT_MENU_EVENTS_RENDERING_INFOS);
	submenu->addItem(L"Simple Edition", -1);
    //-----------------------------------
    
    //-----------------------------------
    //TOOLBAR
    bar = devices->getGUIEnvironment()->addToolBar(0, -1);
    bar->setRelativePosition(position2d<int>(0, menu->getRelativePosition().getHeight()+5));
	ITexture* image = devices->getVideoDriver()->getTexture("GUI/open.png");
	bar->addButton(CXT_MENU_EVENTS_OPEN_SCRIPT, 0, L"Open a scene", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture("GUI/tools.png");
	bar->addButton(CXT_MENU_EVENTS_EDIT_NODE, 0, L"Edit Selected Node", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture("GUI/zip.png");
	bar->addButton(CXT_MENU_EVENTS_EXPORT_SCENE, 0, L"Export this scene", image, 0, false, true);
    image = devices->getVideoDriver()->getTexture("GUI/edit_ao.png");
    bar->addButton(CXT_MENU_EVENTS_EDIT_AO, 0, L"Edit Animated Object", image, 0, false, true);
    
    image = devices->getVideoDriver()->getTexture("GUI/help.png");
	bar->addButton(CXT_MENU_EVENTS_HELP, 0, L"Open Help", image, 0, false, true);
    //-----------------------------------
    
    //-----------------------------------
    //INFORMATIONS BAR
    infosBar = devices->getGUIEnvironment()->addToolBar(0, -1);
    infosBar->setRelativePosition(position2d<int>(0, bar->getRelativePosition().getHeight() +
                                                  menu->getRelativePosition().getHeight()));
    image = devices->getVideoDriver()->getTexture("GUI/Render.png");
    infosBar->addButton(CXT_BAR_EVENTS_RENDER, 0, L"Render Current View", image, 0, false, true);
    //-----------------------------------
    
    timer = devices->getDevice()->getTimer();
    timer->start();
    timer->setTime(0);
    
    CImporter *impoterInstance = new CImporter(devices);
    impoterInstance->importScene("project.world");
    delete impoterInstance;
    //CUIWindowEditNode *edit = new CUIWindowEditNode(devices);
    //edit->open(devices->getCoreData()->getTerrainNodes()->operator[](0), L"#terrain:");
    
    //CUIWindowEditEffects *editEffects = new CUIWindowEditEffects(devices);
    //editEffects->open();
    
    CUIWindowEditLight *editLight = new CUIWindowEditLight(devices, 0);
    editLight->open(devices->getCoreData()->getLightsNodes()->operator[](0), "#light:");
}

CUIContextMenu::~CUIContextMenu() {
    
}

void CUIContextMenu::update() {
    if (timer->getTime() > 3000 ) {
        timer->setTime(0);
        mainWindowInstance->refresh();
    }
}

bool CUIContextMenu::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_GUI_EVENT) {
        s32 id = event.GUIEvent.Caller->getID();
        //-----------------------------------
        //CONTEXT MENU EVENT
        if (event.GUIEvent.EventType == EGET_MENU_ITEM_SELECTED) {
            
            IGUIContextMenu *tempMenu = (IGUIContextMenu *)event.GUIEvent.Caller;
                        
            switch (tempMenu->getItemCommandId(tempMenu->getSelectedItem())) {
                    
                //-----------------------------------
                //CONTEXT MENU FILE EVENT
                case CXT_MENU_EVENTS_FILE_QUIT:
                    devices->getDevice()->closeDevice();
                    break;
                    
                case CXT_MENU_EVENTS_FILE_CLEAN_SCENE:
                    devices->getGUIEnvironment()->addMessageBox(L"Are you sure ?",
                                                                L"Are you sure you want to clean all the scene ?\n\n"
                                                                L"Your work will be deleted...", true, 0x3, 0,
                                                                CXT_MENU_EVENTS_FILE_CLEAN_SCENE_OK,
                                                                devices->getVideoDriver()->getTexture(devices->getWorkingDirectory() + stringc("/GUI/info.png")));
                    break;
                    
                case CXT_MENU_EVENTS_OPEN_SCRIPT:
                    openSceneInstance->open();
                    break;
                    
                case CXT_MENU_EVENTS_FILE_RENDER: {
                    CUIWindowRender *render = new CUIWindowRender(devices);
                    render->open();
                }
                    break;
                    
                case CXT_MENU_EVENTS_FILE_EXPORT_SCENE: {
                    exportSceneInstance->setPathFile(openSceneInstance->getPathFile());
                    exportSceneInstance->open();
                }
                    break;
                //-----------------------------------
                
                //-----------------------------------
                //CONTEXT MENU VIEW EVENT
                case CXT_MENU_EVENTS_EDIT_EDIT_SELECTED_NODE: {
                    stringc prefix = mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode());
                    if (prefix == "#map" || prefix == "#tree" || prefix == "#object") {
                        CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
                        editNode->open(mainWindowInstance->getSelectedNode(), 
                                       mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode()));
                    } else if (prefix == "#light") {
                        CUIWindowEditLight *editLight = new CUIWindowEditLight(devices, mainWindowInstance->getActiveListBox()->getSelected());
                        editLight->open(mainWindowInstance->getSelectedNode(),
                                        mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode()));
                    }
                }
                    break;
                    
                case CXT_MENU_EVENTS_EDIT_SET_ALL_NODES_LIGHTING: {
                    array<ISceneNode *> nodes = devices->getCoreData()->getAllSceneNodes();
                    for (int i=0; i < nodes.size(); i++) {
                        nodes[i]->setMaterialFlag(EMF_LIGHTING, true);
                    }
                }
                    break;
                    
                case CXT_MENU_EVENTS_EDIT_SET_ALL_NODES_NOT_LIGHTING: {
                    array<ISceneNode *> nodes = devices->getCoreData()->getAllSceneNodes();
                    for (int i=0; i < nodes.size(); i++) {
                        nodes[i]->setMaterialFlag(EMF_LIGHTING, false);
                    }
                }
                    break;
                    
                case CXT_MENU_EVENTS_EDIT_GRID_SCENE_NODE:
                    editGridInstance->open();
                    break;
                    
                case CXT_MENU_EVENTS_EDIT_CLONE:
                    mainWindowInstance->cloneNode();
                    break;
                //-----------------------------------
                
                //-----------------------------------
                //CONTEXT MENU VIEW EVENT
                case CXT_MENU_EVENTS_VIEW_MAYA_CAMERA:
                    devices->getSceneManager()->setActiveCamera(devices->getMayaCamera());
                    break;
                    
                case CXT_MENU_EVENTS_VIEW_FPS_CAMERA:
                    devices->getFPSCamera()->setPosition(devices->getMayaCamera()->getPosition());
                    devices->getFPSCamera()->setRotation(vector3df(0, -1, 0));
                    devices->getCollisionManager()->createAnimatorCollisionCamera(devices->getFPSCamera());
                    devices->getSceneManager()->setActiveCamera(devices->getFPSCamera());
                    break;
                    
                case CXT_MENU_EVENTS_VIEW_VIEW_TREE_NODES_WINDOW:
                    sceneViewInstance->open();
                    break;
                    
                case CXT_MENU_EVENTS_VIEW_WIREFRAME: {
                    ISceneNode *nodeWireFrame = mainWindowInstance->getSelectedNode();
                    if (nodeWireFrame) {
                        if (nodeWireFrame->getType() == ESNT_TERRAIN || nodeWireFrame->getType() == ESNT_OCTREE) {
                            nodeWireFrame->setMaterialFlag(EMF_WIREFRAME, !nodeWireFrame->getMaterial(0).Wireframe);
                            submenu = menu->getSubMenu(3);
                            if (nodeWireFrame->getMaterial(0).Wireframe) {
                                submenu->setItemChecked(5, true);
                            } else {
                                submenu->setItemChecked(5, false);
                            }
                        }
                    } else {
                        devices->addWarningDialog(L"Warning", L"Please Select A Node Before...", EMBF_OK);
                    }
                }
                    break;
                    
                case CXT_MENU_EVENTS_VIEW_POINTCLOUD: {
                    ISceneNode *nodePointCloud = mainWindowInstance->getSelectedNode();
                    if (nodePointCloud) {
                        if (nodePointCloud->getType() == ESNT_TERRAIN || nodePointCloud->getType() == ESNT_OCTREE) {
                            nodePointCloud->setMaterialFlag(EMF_POINTCLOUD, !nodePointCloud->getMaterial(0).PointCloud);
                            submenu = menu->getSubMenu(3);
                            if (nodePointCloud->getMaterial(0).PointCloud) {
                                submenu->setItemChecked(6, true);
                            } else {
                                submenu->setItemChecked(6, false);
                            }
                        }
                    } else {
                        devices->addWarningDialog(L"Warning", L"Please Select A Node Before...", EMBF_OK);
                    }
                }
                    break;
                //-----------------------------------
                
                //-----------------------------------
                //CONTEXT MENU RENDERS EVENT
                case CXT_MENU_EVENTS_RENDERS_HDR_DRAW:
                    devices->setHDRDrawable(!devices->isHDRDrawable());
                    break;
                    
                case CXT_MENU_EVENTS_RENDERS_XEFFECT_DRAW:
                    devices->setXEffectDrawable(!devices->isXEffectDrawable());
                    break;
                    
                case CXT_MENU_EVENTS_RENDERS_XEFFECT_EDIT: {
                    CUIWindowEditEffects *editEffects = new CUIWindowEditEffects(devices);
                    editEffects->open();
                }
                    break;
                //-----------------------------------
                    
                //-----------------------------------
                //CONTEXT MENU ANIMATED OBJECTS EVENT
                case CXT_MENU_EVENTS_ANIMATED_MODELS_WINDOW_EDITION: {
                    if (mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode()) == "#object") {
                        if (!characterWindowInstance->isWindowActive()) {
                            characterWindowInstance->open();
                            characterWindowInstance->setModel((IAnimatedMeshSceneNode *)mainWindowInstance->getSelectedNode(), mainWindowInstance->getActiveListBox()->getSelected());
                        }
                    }
                }
                    break;
                //-----------------------------------
                
                //-----------------------------------
                //CONTEXT MENU VIEW EVENT
                case CXT_MENU_EVENTS_RENDERING_INFOS: {
                    CUIRenderingInfos *renderInfos = new CUIRenderingInfos(devices);
                }
                    break; 
                //-----------------------------------
                    
                default:
                    break;
            }
        }
        //-----------------------------------
        
        if (event.GUIEvent.EventType == EGET_MESSAGEBOX_OK) {
            switch (id) {
                case CXT_MENU_EVENTS_FILE_CLEAN_SCENE_OK:
                    //Clear Nodes
                    devices->getCoreData()->clear();
                    //Clear all the shadows
                    for (u32 i=0; i < devices->getCoreData()->getAllSceneNodes().size(); i++) {
                        devices->getXEffect()->removeShadowFromNode(devices->getCoreData()->getAllSceneNodes()[i]);
                    }
                    //Clear all the shadow lights
                    for (u32 i=0; i < devices->getXEffect()->getShadowLightCount(); i++) {
                        devices->getXEffect()->removeShadowLight(i);
                    }
                    //Clear all the PostProcessing effects (shaders)
                    for (u32 i=0; i < devices->getCoreData()->getEffectRenders()->size(); i++) {
                        devices->getXEffect()->removePostProcessingEffect(devices->getCoreData()->getEffectRenders()->operator[](i));
                    }
                    //Clear CoreData
                    devices->getCoreData()->clearAllTheArrays();
                    //Resetting grid
                    devices->getObjectPlacement()->getGridSceneNode()->SetAccentlineOffset(8);
                    devices->getObjectPlacement()->getGridSceneNode()->SetSize(1024);
                    devices->getObjectPlacement()->getGridSceneNode()->SetSpacing(8);
                    break;
                    
                default:
                    break;
            }
        }
        
        //-----------------------------------
        //LISTBOX GUI EVENT
        
        if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
            submenu = menu->getSubMenu(2);
            ISceneNode *node = mainWindowInstance->getSelectedNode();
            if (node) {
                if (node->getMaterial(0).Wireframe) {
                    submenu->setItemChecked(5, true);
                } else {
                    submenu->setItemChecked(5, false);
                }
                
                if (node->getMaterial(0).PointCloud) {
                    submenu->setItemChecked(6, true);
                } else {
                    submenu->setItemChecked(6, false);
                }
            }
        }
        
        //-----------------------------------
        
        //-----------------------------------
        //BUTTON GUI EVENT
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            
            switch (id) {
                //-----------------------------------
                //TOOLBAR
                case CXT_MENU_EVENTS_OPEN_SCRIPT:
                    openSceneInstance->open();
                    break;
                    
                case CXT_MENU_EVENTS_EDIT_NODE: {
                    stringc prefix = mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode());
                    if (prefix == "#map" || prefix == "#tree" || prefix == "#object") {
                        CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
                        editNode->open(mainWindowInstance->getSelectedNode(), 
                                       mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode()));
                    } else if (prefix == "#light") {
                        CUIWindowEditLight *editLight = new CUIWindowEditLight(devices, mainWindowInstance->getActiveListBox()->getSelected());
                        editLight->open(mainWindowInstance->getSelectedNode(),
                                        mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode()));
                    }
                }
                    break;
                    
                case CXT_MENU_EVENTS_EXPORT_SCENE:
                    exportSceneInstance->setPathFile(openSceneInstance->getPathFile());
                    exportSceneInstance->open();
                    break;
                    
                case CXT_MENU_EVENTS_EDIT_AO: {
                    if (mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode()) == "#object") {
                        if (!characterWindowInstance->isWindowActive()) {
                            characterWindowInstance->open();
                            characterWindowInstance->setModel((IAnimatedMeshSceneNode *)mainWindowInstance->getSelectedNode(), mainWindowInstance->getActiveListBox()->getSelected());
                        }
                    }
                }
                    break;
                    
                case CXT_MENU_EVENTS_HELP:
                    devices->getGUIEnvironment()->addMessageBox(L"How to ?",
                                                                L"KEY CODES : \n"
                                                                L"A     : Edit the selected animated object \n"
                                                                L"P     : Set the selected node to the Arrow Position \n"
                                                                L"Space : Set the arrow node position on/off \n"
                                                                L"X     : Draw Shadows \n"
                                                                L"L     : Set the selected node lighting \n"
                                                                L"C     : Clone the selected node\n"
                                                                L"E     : Edit the selected node\n", 
                                                                false, EMBF_OK, 0, -1,
                                                                devices->getVideoDriver()->getTexture(devices->getWorkingDirectory() + 
                                                                                                      stringc("/GUI/help.png")));
                    
                    break;
                //-----------------------------------
                //-----------------------------------
                //SECOND TOOLBAR 
                case CXT_BAR_EVENTS_RENDER: {
                    CUIWindowRender *render = new CUIWindowRender(devices);
                    render->open();
                }
                    break;
                    
                //-----------------------------------
                    
                default:
                    break;
            }
        }
        //-----------------------------------
    }
    
    if (event.EventType == EET_KEY_INPUT_EVENT) {
		if (!event.KeyInput.PressedDown) {
			switch (event.KeyInput.Key) {
                    
                case KEY_KEY_X:
                    if (!devices->isEditBoxEntered() && devices->isCtrlPushed() && !devices->isShiftPushed()) {
                        devices->setXEffectDrawable(!devices->isXEffectDrawable());
                    }
                    if (!devices->isEditBoxEntered() && devices->isCtrlPushed() && devices->isShiftPushed()) {
                        CUIWindowEditEffects *editEffects = new CUIWindowEditEffects(devices);
                        editEffects->open();
                    }
                    break;
                    
                case KEY_KEY_W: {
                    if (!devices->isEditBoxEntered() && devices->isCtrlPushed()) {
                        ISceneNode *nodeWireFrame = mainWindowInstance->getSelectedNode();
                        if (nodeWireFrame) {
                            if (nodeWireFrame->getType() == ESNT_TERRAIN || nodeWireFrame->getType() == ESNT_OCTREE) {
                                nodeWireFrame->setMaterialFlag(EMF_WIREFRAME, !nodeWireFrame->getMaterial(0).Wireframe);
                                submenu = menu->getSubMenu(3);
                                if (nodeWireFrame->getMaterial(0).Wireframe) {
                                    submenu->setItemChecked(5, true);
                                } else {
                                    submenu->setItemChecked(5, false);
                                }
                            }
                        } else {
                            devices->addWarningDialog(L"Warning", L"Please Select A Node Before...", EMBF_OK);
                        }
                    }
                }
                    break;
                    
                case KEY_KEY_H:
                    if (!devices->isEditBoxEntered() && devices->isCtrlPushed()) {
                        devices->setHDRDrawable(!devices->isHDRDrawable());
                    }
                    break;
                    
                case KEY_KEY_A:
                    if (!devices->isEditBoxEntered() && devices->isCtrlPushed()) {
                        if (mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode()) == "#object") {
                            if (!characterWindowInstance->isWindowActive()) {
                                characterWindowInstance->open();
                                characterWindowInstance->setModel((IAnimatedMeshSceneNode *)mainWindowInstance->getSelectedNode(), mainWindowInstance->getActiveListBox()->getSelected());
                            }
                        }
                    }
                    break;
                    
                case KEY_KEY_O:
                    if (!devices->isEditBoxEntered() && devices->isCtrlPushed()) {
                        openSceneInstance->open();
                    }
                    break;
                    
                case KEY_KEY_E: {
                    if (!devices->isEditBoxEntered() && devices->isCtrlPushed()) {
                        stringc prefix = mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode());
                        if (prefix == "#map" || prefix == "#tree" || prefix == "#object") {
                            CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
                            editNode->open(mainWindowInstance->getSelectedNode(), 
                                           mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode()));
                        } else if (prefix == "#light") {
                            CUIWindowEditLight *editLight = new CUIWindowEditLight(devices, mainWindowInstance->getActiveListBox()->getSelected());
                            editLight->open(mainWindowInstance->getSelectedNode(),
                                            mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode()));
                        }
                    }
                }
                    break;
                    
                default:
                    break;
                    
            }
        }
    }
    
    return false;
}
