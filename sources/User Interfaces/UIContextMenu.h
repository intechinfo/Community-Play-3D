/*
 *  UIContextMenu.h
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __C_UI_CONTEXT_MENU_H_INCLUDED__
#define __C_UI_CONTEXT_MENU_H_INCLUDED__

enum GUI_LOAD_FILE_EVENT {
    GUI_LOAD_FILE_IMPORT
};

enum GUI_CXT_MENU_EVENTS {
    CXT_MENU_EVENTS_WORLD_EDITOR = 0x10000,
    
    CXT_MENU_EVENTS_OPEN_SCRIPT,
    CXT_MENU_EVENTS_EDIT_NODE,
    CXT_MENU_EVENTS_EXPORT_SCENE,
    CXT_MENU_EVENTS_EDIT_AO,
    CXT_MENU_EVENTS_HELP,
    
    CXT_BAR_EVENTS_RENDER,
    
    CXT_MENU_EVENTS_FILE_OPEN_SCRIPT,
    CXT_MENU_EVENTS_FILE_EXPORT_SCENE,
    CXT_MENU_EVENTS_FILE_CLEAN_SCENE, CXT_MENU_EVENTS_FILE_CLEAN_SCENE_OK,
    CXT_MENU_EVENTS_FILE_RENDER,
    CXT_MENU_EVENTS_FILE_QUIT,
    
    CXT_MENU_EVENTS_EDIT_EDIT_SELECTED_NODE,
    CXT_MENU_EVENTS_EDIT_SET_ALL_NODES_LIGHTING,
    CXT_MENU_EVENTS_EDIT_SET_ALL_NODES_NOT_LIGHTING,
    CXT_MENU_EVENTS_EDIT_GRID_SCENE_NODE,
    CXT_MENU_EVENTS_EDIT_CLONE,
    
    CXT_MENU_EVENTS_VIEW_MAYA_CAMERA,
    CXT_MENU_EVENTS_VIEW_FPS_CAMERA,
    CXT_MENU_EVENTS_VIEW_VIEW_TREE_NODES_WINDOW,
    CXT_MENU_EVENTS_VIEW_WIREFRAME,
    CXT_MENU_EVENTS_VIEW_POINTCLOUD,
    
    CXT_MENU_EVENTS_RENDERS_HDR_DRAW,
    CXT_MENU_EVENTS_RENDERS_HDR_EDIT,
    CXT_MENU_EVENTS_RENDERS_XEFFECT_DRAW,
    CXT_MENU_EVENTS_RENDERS_XEFFECT_EDIT,
    
    CXT_MENU_EVENTS_ANIMATED_MODELS_WINDOW_EDITION,
    CXT_MENU_EVENTS_SIMPLE_EDITION,
    
    CXT_MENU_EVENTS_RENDERING_INFOS
};

#include "../Device/CDevices.h"

#include "Main Window/UIMainWindow.h"

#include "../User Interfaces/CUIWindowOpenScene.h"
#include "../User Interfaces/CUIWindowExportScene.h"

#include "CUIWindowEditNode.h"
#include "CUIWindowEditLight.h"
#include "CUISceneView.h"
#include "CUIWindowEditGrid.h"

#include "CUIRenderingInfos.h"

#include "../Character Edition/CUICharacterWindow.h"
#include "UIWindowRender.h"

#include "../Renders/XEffect/Interfaces/CUIWindowEditEffects.h"

class CCoreUserInterface;

class CUIContextMenu : public IEventReceiver {
    
public:
    
    CUIContextMenu(CDevices *_devices);
    ~CUIContextMenu();
    
    void update();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    //-----------------------------------
    //DEVICE
    CDevices *devices;
    //-----------------------------------
    
    //-----------------------------------
    //USER INTERFACE ELEMENTS
    CUIMainWindow *mainWindowInstance;
    CUISceneView *sceneViewInstance;
    CUIWindowEditGrid *editGridInstance;
    CUICharacterWindow *characterWindowInstance;
    
    CUIWindowOpenScene *openSceneInstance;
    CUIWindowExportScene *exportSceneInstance;

    ITimer *timer;
    //-----------------------------------
    
    //-----------------------------------
    //GUI ELEMENTS
    IGUIContextMenu *menu, *submenu;
    IGUIToolBar *bar, *infosBar;
    //-----------------------------------
    
};

#endif
