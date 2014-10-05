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

	CXT_MENU_EVENTS_OPEN_SCRIPT_EDITOR,
	CXT_MENU_EVENTS_MAKE_PLANAR_MAPPING,
	CXT_MENU_EVENTS_CREATE_MESH_WITH_TANGENTS,

	CXT_MENU_EVENTS_PLAY_GAME,

	CXT_MENU_EVENTS_CREATE_CUBE,
	CXT_MENU_EVENTS_CREATE_SPHERE,
	CXT_MENU_EVENTS_CREATE_PLANE,
	CXT_MENU_EVENTS_CREATE_BILLBOARD,
	CXT_MENU_EVENTS_CREATE_LIGHT,

	CXT_BAR_EVENTS_RENDER,
	CXT_BAR_EVENTS_LOG_WINDOW,

	CXT_MENU_EVENTS_FILE_OPEN_SCRIPT,
	CXT_MENU_EVENTS_FILE_CONCAT_SCENE_SCRIPT,
	CXT_MENU_EVENTS_FILE_ADD_NODES_GROUP,
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
	CXT_MENU_EVENTS_ANIMATED_MODELS_WINDOW_EDITION,
	CXT_MENU_EVENTS_SIMPLE_EDITION,
	CXT_MENU_EVENTS_EDIT_OPTIONS,

	CXT_MENU_EVENTS_VIEW_MAYA_CAMERA,
	CXT_MENU_EVENTS_VIEW_FPS_CAMERA,
	CXT_MENU_EVENTS_VIEW_VIEW_TREE_NODES_WINDOW,
	CXT_MENU_EVENTS_VIEW_TEXTURES_MANAGER,
	CXT_MENU_EVENTS_VIEW_WIREFRAME,
	CXT_MENU_EVENTS_VIEW_POINTCLOUD,

	CXT_MENU_EVENTS_RENDERS_DOF_DRAW,
	CXT_MENU_EVENTS_RENDERS_MOTION_BLUR_DRAW,
	CXT_MENU_EVENTS_RENDERS_SSAO,
	CXT_MENU_EVENTS_RENDERS_DRAW_FULL_PT,
	CXT_MENU_EVENTS_ALLOW_REFLECTION_PASS,
	CXT_MENU_EVENTS_ALLOW_LIGHT_SCATTERING_PASS,
	CXT_MENU_EVENTS_ALLOW_NORMAL_PASS,

		CXT_MENU_EVENTS_RENDERS_XEFFECT_DRAW,
		CXT_MENU_EVENTS_RENDERS_XEFFECT_EDIT,
		CXT_MENU_EVENTS_RENDERS_XEFFECT_RECALCULATE_LIGHTS,

		CXT_MENU_EVENTS_RENDERS_EDIT_DOF,

	CXT_MENU_EVENTS_EDIT_HDR_MANAGER,
	CXT_MENU_EVENTS_EDIT_MATERIALS_SHADER,
    
	CXT_MENU_EVENTS_EDIT_CLOUDS,
    CXT_MENU_EVENTS_SCENE_MANAGE_PARTICLE_SYSTEMS,
    CXT_MENU_EVENTS_SCENE_ADD_TERRAIN,
    CXT_MENU_EVENTS_SCENE_ADD_OBJECT,
    CXT_MENU_EVENTS_SCENE_ADD_LIGHT,
    CXT_MENU_EVENTS_SCENE_ADD_VOLUME_LIGHT,
    CXT_MENU_EVENTS_SCENE_ADD_WATER_SURFACE,
	CXT_MENU_EVENTS_SCENE_ADD_3D_SOUND,

	CXT_MENU_EVENTS_ADD_CUBE_SCENE_NODE,
	CXT_MENU_EVENTS_ADD_SPHERE_SCENE_NODE,
	CXT_MENU_EVENTS_ADD_HILL_PLANE_MESH,
	CXT_MENU_EVENTS_ADD_BILL_BOARD,
	CXT_MENU_EVENTS_NODE_FACTORY_PLANAR_TEXTURE_MAPPING,
	CXT_MENU_EVENTS_NODE_FACTORY_CREATE_MESH_WITH_TANGENTS,
	CXT_MENU_EVENTS_NODE_FACTORY_ENABLE_SKYBOX,
    CXT_MENU_EVENTS_NODE_FACTORY_EDIT_SKYBOX,
    CXT_MENU_EVENTS_NODE_FACTORY_EDIT_MATERIALS_SKYBOX,
	CXT_MENU_EVENTS_NODE_FACTORY_ENABLE_SKYDOME,
	CXT_MENU_EVENTS_NODE_FACTORY_EDIT_SKYDOME,
	CXT_MENU_EVENTS_NODE_FACTORY_EDIT_MATERIALS_SKYDOME,
	CXT_MENU_EVENTS_NODE_FACTORY_NEW_TERRAIN,
	CXT_MENU_EVENTS_MESH_FACTORY_EDIT,
	CXT_MENU_EVENTS_ANIMATED_MESH_TESTER,
	CXT_MENU_EVENTS_MESH_SIMPLIFICATOR,

	CXT_MENU_EVENTS_ANIMATOR_CREATE_FLY_STRAGHT,
	CXT_MENU_EVENTS_ANIMATOR_CREATE_FLY_CIRCLE,
	CXT_MENU_EVENTS_ANIMATOR_CREATE_FOLLOW_SPLINE,
	CXT_MENU_EVENTS_ANIMATOR_SCENARIO_MAKER,

	CXT_MENU_EVENTS_SCRIPTS_OPEN_EDITOR,

	CXT_MENU_EVENTS_RENDERING_INFOS,
	CXT_MENU_EVENTS_HIDE_DRAW_MAIN_WINDOW,
	CXT_MENU_EVENTS_HELP_ABOUT,

	CXT_MENU_EVENTS_PLUGINS_EDIT,
    CXT_MENU_EVENTS_PLUGINS_LOAD_DEVELOPMENT_INSTANCE,
    CXT_MENU_EVENTS_PLUGINS_RUN_DEVELOPMENT_INSTANCE
};

#include "../Device/CDevices.h"

#include "MainWindow/UIMainWindow.h"

#include "../UserInterfaces/CUIWindowOpenScene.h"
#include "../UserInterfaces/CUIWindowExportScene.h"

#include "CUILoadNodesGroup.h"
#include "CUIWindowEditNode.h"
#include "MaterialEditor/CUIMaterialEditor.h"
#include "CUIWindowEditLight.h"
#include "CUIWindowEditVolumeLight.h"
#include "CUIWindowEditGrid.h"

#include "Options/CUISSWEOptions.h"
#include "Options/CUIHDRManager.h"

#include "NodeFactory/CNodeFactory.h"
#include "NodeFactory/CUINodeFactoryPlanarMapping.h"
#include "NodeFactory/CUINodeFactoryCreateMeshWithTangents.h"
#include "NodeFactory/CUIWindowAddNewTerrain.h"

#include "AnimatorsEditor/CUIWindowEditNodeAnimators.h"
#include "AnimatorsEditor/CUIEditFPSCamera.h"

#include "CUISceneView.h"
#include "TexturesManager/CUITexturesManager.h"

#include "CUIRenderingInfos.h"

#include "../CharacterEdition/CUICharacterWindow.h"
#include "UIWindowRender.h"

#include "../Renders/XEffect/Interfaces/CUIWindowEditFilters.h"
#include "../Renders/XEffect/Interfaces/CUIWindowEditMaterials.h"
#include "../Renders/DepthOfField/CUIWindowEditDOF.h"

#include "../Device/Core/Scripting/CUIScriptEditor.h"

#include "PluginsManager/CPluginsManager.h"
#include "PluginsManager/CUIPluginsManager.h"

#include "ParticlesEditor/CUIParticlesEditor.h"

#include "NodeFactory/CUIWindowEditClouds.h"

#include "ScenarioMaker/CUIScenarioMakerMain.h"

#include "MeshFactory/CUIMeshFactory.h"
#include "MeshFactory/CUIAnimatedMeshViewer.h"
#include "MeshFactory/CUIMeshSimplificator.h"

#include "Audio/CUIAddAudioElement.h"

class CUIContextMenu : public IEventReceiver {

public:

	CUIContextMenu(CDevices *_devices, CPluginsManager *manager);
	~CUIContextMenu();

	void update();

	IGUIButton *getConsoleButton() { return consoleButton; }
	CUIMainWindow *getMainWindow() { return mainWindowInstance; }

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
	//METHODS
	void playExampleGame();
	//-----------------------------------

	//-----------------------------------
	//DEVICE
	CDevices *devices;

	IGUIButton *consoleButton;

	CNodeFactory *nodeFactory;
	//-----------------------------------

	//-----------------------------------
	//USER INTERFACE ELEMENTS
	CUIMainWindow *mainWindowInstance;
	CUISceneView *sceneViewInstance;
	CUIWindowEditGrid *editGridInstance;

	CUIWindowOpenScene *openSceneInstance;
	CUIWindowExportScene *exportSceneInstance;

	ITimer *timer;

	CPluginsManager *pluginsManager;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	IGUIContextMenu *menu, *submenu;
	IGUIContextMenu *monitorsMenu;
	IGUIContextMenu *renderingMenu, *postProcessesMenu;

	IGUIToolBar *bar, *infosBar;

	IGUIButton *ibposition, *ibrotation, *ibscale;
	CCoreObjectPlacement::ArrowType movementType;

	IGUIContextMenu *skydomeMenu, *skyboxMenu;

	IGUIStaticText *contextNameText;
	IGUIComboBox *comboModecb;
	//-----------------------------------

};

#endif
