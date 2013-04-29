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
	CXT_MENU_EVENTS_EDIT_MATERIAL_SHADERS,
	CXT_MENU_EVENTS_EDIT_EFFECTS,
	CXT_MENU_EVENTS_HELP,

	CXT_BAR_EVENTS_RENDER,
	CXT_BAR_EVENTS_LOG_WINDOW,

	CXT_MENU_EVENTS_FILE_OPEN_SCRIPT,
	CXT_MENU_EVENTS_FILE_EXPORT_SCENE,
	CXT_MENU_EVENTS_FILE_CLEAN_SCENE, CXT_MENU_EVENTS_FILE_CLEAN_SCENE_OK,
	CXT_MENU_EVENTS_FILE_RENDER,
	CXT_MENU_EVENTS_FILE_QUIT,

	CXT_MENU_EVENTS_EDIT_EDIT_SELECTED_NODE,
    CXT_MENU_EVENTS_EDIT_EDIT_MATERIALS_SELECTED_NODE,
	CXT_MENU_EVENTS_EDIT_SET_ALL_NODES_LIGHTING,
	CXT_MENU_EVENTS_EDIT_SET_ALL_NODES_NOT_LIGHTING,
	CXT_MENU_EVENTS_EDIT_GRID_SCENE_NODE,
	CXT_MENU_EVENTS_EDIT_CLONE,

	CXT_MENU_EVENTS_VIEW_MAYA_CAMERA,
	CXT_MENU_EVENTS_VIEW_FPS_CAMERA,
	CXT_MENU_EVENTS_VIEW_VIEW_TREE_NODES_WINDOW,
	CXT_MENU_EVENTS_VIEW_WIREFRAME,
	CXT_MENU_EVENTS_VIEW_POINTCLOUD,

	CXT_MENU_EVENTS_RENDERS_MOTION_BLUR_DRAW,
	CXT_MENU_EVENTS_RENDERS_HDR_EDIT,
	CXT_MENU_EVENTS_RENDERS_XEFFECT_DRAW,
	CXT_MENU_EVENTS_RENDERS_XEFFECT_EDIT,

	CXT_MENU_EVENTS_EDIT_MATERIALS_SHADER,

	CXT_MENU_EVENTS_ANIMATED_MODELS_WINDOW_EDITION,
	CXT_MENU_EVENTS_SIMPLE_EDITION,

	CXT_MENU_EVENTS_ADD_CUBE_SCENE_NODE,
	CXT_MENU_EVENTS_ADD_SPHERE_SCENE_NODE,
	CXT_MENU_EVENTS_ADD_HILL_PLANE_MESH,
	CXT_MENU_EVENTS_ADD_BILL_BOARD,
	CXT_MENU_EVENTS_NODE_FACTORY_PLANAR_TEXTURE_MAPPING,
	CXT_MENU_EVENTS_NODE_FACTORY_CREATE_MESH_WITH_TANGENTS,
	CXT_MENU_EVENTS_NODE_FACTORY_ENABLE_SKYBOX,
	CXT_MENU_EVENTS_NODE_FACTORY_ENABLE_SKYDOME,
	CXT_MENU_EVENTS_NODE_FACTORY_EDIT_SKYDOME,
	CXT_MENU_EVENTS_NODE_FACTORY_EDIT_MATERIALS_SKYDOME,

	CXT_MENU_EVENTS_RENDERING_INFOS,
	CXT_MENU_EVENTS_HIDE_DRAW_MAIN_WINDOW,

	CXT_MENU_EVENTS_HELP_ABOUT
};

#include "../Device/CDevices.h"

#include "MainWindow/UIMainWindow.h"

#include "../UserInterfaces/CUIWindowOpenScene.h"
#include "../UserInterfaces/CUIWindowExportScene.h"

#include "CUIWindowEditNode.h"
#include "MaterialEditor/CUIMaterialEditor.h"
#include "CUIWindowEditLight.h"
#include "CUIWindowEditGrid.h"

#include "NodeFactory/CUINodeFactoryPlanarMapping.h"
#include "NodeFactory/CUINodeFactoryCreateMeshWithTangents.h"

#include "CUISceneView.h"

#include "CUIRenderingInfos.h"

#include "../CharacterEdition/CUICharacterWindow.h"
#include "UIWindowRender.h"

#include "../Renders/XEffect/Interfaces/CUIWindowEditEffects.h"
#include "../Renders/XEffect/Interfaces/CUIWindowEditMaterials.h"

class CUIContextMenu : public IEventReceiver {

public:

	CUIContextMenu(CDevices *_devices);
	~CUIContextMenu();

	void update();

	IGUIButton *getConsoleButton() { return consoleButton; }
	CUIMainWindow *getMainWindow() { return mainWindowInstance; }

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
	//DEVICE
	CDevices *devices;

	IGUIButton *consoleButton;
	//-----------------------------------

	//-----------------------------------
	//USER INTERFACE ELEMENTS
	CUIMainWindow *mainWindowInstance;
	CUISceneView *sceneViewInstance;
	CUIWindowEditGrid *editGridInstance;

	CUIWindowOpenScene *openSceneInstance;
	CUIWindowExportScene *exportSceneInstance;

	ITimer *timer;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	IGUIContextMenu *menu, *submenu;
	IGUIToolBar *bar, *infosBar;

	IGUIButton *ibposition, *ibrotation, *ibscale;
	CCoreObjectPlacement::ArrowType movementType;

	IGUIContextMenu *skydomeMenu, *skyboxMenu;
	//-----------------------------------

};

#endif
