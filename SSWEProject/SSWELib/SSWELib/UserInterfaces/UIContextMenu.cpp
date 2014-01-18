/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "stdafx.h"
#include "UIContextMenu.h"

CUIContextMenu::CUIContextMenu(CDevices *_devices) {
    devices = _devices;

    mainWindowInstance = new CUIMainWindow(devices);
    sceneViewInstance = new CUISceneView(devices);
    editGridInstance = new CUIWindowEditGrid(devices);
    openSceneInstance = new CUIWindowOpenScene(devices);
    exportSceneInstance = new CUIWindowExportScene(devices);

	nodeFactory = new CNodeFactory(devices);

	pluginsManager = new CPluginsManager(devices);

	//SETS GENERIC MONITOR AS DEFAULT
	#ifndef _IRR_OSX_PLATFORM_
		#ifndef SSWE_RELEASE
			pluginsManager->loadMonitorPlugin("SSWEGENERICMONITOR_D");
		#else
			pluginsManager->loadMonitorPlugin("SSWEGENERICMONITOR");
		#endif
	#else
		#ifdef SSWE_RELEASE
			pluginsManager->loadMonitorPlugin("LIBSSWEGENERICMONITOR");
			//pluginsManager->loadMonitorPlugin("libSSWEGenericMonitor");
		#endif
	#endif

    //-----------------------------------
    //MENU
    menu = devices->getGUIEnvironment()->addMenu();
    
    menu->addItem(L"World Editor", CXT_MENU_EVENTS_WORLD_EDITOR, false, false, false, false);
    menu->addItem(L"File", -1, true, true);
	menu->addItem(L"Edit", -1, true, true);
	menu->addItem(L"View", -1, true, true);
	menu->addItem(L"Animators", -1, true, true);
	menu->addItem(L"Renders", -1, true, true);
    menu->addItem(L"Scene", -1, true, true);
	menu->addItem(L"Factory", -1, true, true);
	menu->addItem(L"Scripting", -1, true, true);
	menu->addItem(L"Sounds", -1, true, true);
    menu->addItem(L"Window", -1, true, true);
	menu->addItem(L"Plugins", -1, true, true);

    int i=0;

	//WORLD EDITOR
    submenu = menu->getSubMenu(i++);

	//FILE
    submenu = menu->getSubMenu(i++);
	submenu->addItem(L"Open a script (CTRL+O)", CXT_MENU_EVENTS_OPEN_SCRIPT);
	submenu->addItem(L"Concat this scene with other...", CXT_MENU_EVENTS_FILE_CONCAT_SCENE_SCRIPT);
	submenu->addItem(L"Add nodes group from directory...", CXT_MENU_EVENTS_FILE_ADD_NODES_GROUP);
	submenu->addSeparator();
	submenu->addItem(L"Export this scene", CXT_MENU_EVENTS_FILE_EXPORT_SCENE);
	submenu->addItem(L"Clean the scene", CXT_MENU_EVENTS_FILE_CLEAN_SCENE);
    submenu->addSeparator();
	submenu->addItem(L"Render", CXT_MENU_EVENTS_FILE_RENDER);
	submenu->addSeparator();
	submenu->addItem(L"Quit", CXT_MENU_EVENTS_FILE_QUIT);

	//EDIT
    submenu = menu->getSubMenu(i++);
	submenu->addItem(L"Edit Selected Node (CTRL+E)", CXT_MENU_EVENTS_EDIT_EDIT_SELECTED_NODE);
    submenu->addItem(L"Edit Materials (CTRL+SHIFT+E)", CXT_MENU_EVENTS_EDIT_EDIT_MATERIALS_SELECTED_NODE);
    submenu->addSeparator();
	submenu->addItem(L"Set all the nodes lighting", CXT_MENU_EVENTS_EDIT_SET_ALL_NODES_LIGHTING);
	submenu->addItem(L"Set all the nodes not lighting", CXT_MENU_EVENTS_EDIT_SET_ALL_NODES_NOT_LIGHTING);
    submenu->addSeparator();
    submenu->addItem(L"Edit Grid", CXT_MENU_EVENTS_EDIT_GRID_SCENE_NODE);
    submenu->addSeparator();
    submenu->addItem(L"Clone (CTRL+SHIFT+C)", CXT_MENU_EVENTS_EDIT_CLONE);
	submenu->addSeparator();
	submenu->addItem(L"Automatic Animated Models Window Edition (CTRL+SHIFT+A)", CXT_MENU_EVENTS_ANIMATED_MODELS_WINDOW_EDITION);
	submenu->addItem(L"Manual Animation", CXT_MENU_EVENTS_SIMPLE_EDITION);
	submenu->addSeparator();
	submenu->addItem(L"Preferences...", CXT_MENU_EVENTS_EDIT_OPTIONS);

	//VIEW
    submenu = menu->getSubMenu(i++);
	submenu->addItem(L"Maya Camera (CTRL+SHIFT+M)", CXT_MENU_EVENTS_VIEW_MAYA_CAMERA);
	submenu->addItem(L"FPS Camera (CTRL+SHIFT+F)", CXT_MENU_EVENTS_VIEW_FPS_CAMERA);
    submenu->addSeparator();
	submenu->addItem(L"View Tree Node Window", CXT_MENU_EVENTS_VIEW_VIEW_TREE_NODES_WINDOW);
	submenu->addItem(L"Textures Manager", CXT_MENU_EVENTS_VIEW_TEXTURES_MANAGER);
    submenu->addSeparator();
	submenu->addItem(L"Wire Frame (CTRL+W)", CXT_MENU_EVENTS_VIEW_WIREFRAME, true, false, false, false);
    submenu->addItem(L"Point Cloud", CXT_MENU_EVENTS_VIEW_POINTCLOUD, true, false, false, false);

	//ANIMATORS
    submenu = menu->getSubMenu(i++);

	//RENDERS
    submenu = menu->getSubMenu(i);
    submenu->addItem(L"Post Processes", -1, true, true);
	submenu->addItem(L"Effects", -1, true, true);
    submenu = submenu->getSubMenu(0);
	submenu->addItem(L"Draw Full Post-Traitement", CXT_MENU_EVENTS_RENDERS_DRAW_FULL_PT, true, false, devices->isRenderingFullPostTraitements(), true);
	submenu->addItem(L"Draw Depth Of Field", CXT_MENU_EVENTS_RENDERS_DOF_DRAW, true, false, devices->getXEffect()->isUsingDepthOfField(), true);
    submenu->addItem(L"Draw Motion Blur", CXT_MENU_EVENTS_RENDERS_MOTION_BLUR_DRAW, true, false,
					devices->getXEffect()->isUsingMotionBlur(), true);
	submenu->addItem(L"Draw SSAO", CXT_MENU_EVENTS_RENDERS_SSAO, true, false, devices->getRenderCallbacks()->getSSAORenderCallback(), true);
    submenu = menu->getSubMenu(i);
	submenu = submenu->getSubMenu(1);
	submenu->addItem(L"Draw Effects (CTRL+X)", CXT_MENU_EVENTS_RENDERS_XEFFECT_DRAW, true, false, devices->isXEffectDrawable(), true);
    submenu->addSeparator();
    submenu->addItem(L"Edit (CTRL+SHIFT+X)", CXT_MENU_EVENTS_RENDERS_XEFFECT_EDIT);
	submenu->addItem(L"Recalculate All Shadow Lights (CTRL+SHIFT+L)", CXT_MENU_EVENTS_RENDERS_XEFFECT_RECALCULATE_LIGHTS);
	submenu->addSeparator();
	submenu->addItem(L"Edit Depth Of Field...", CXT_MENU_EVENTS_RENDERS_EDIT_DOF);
	submenu = menu->getSubMenu(i);
	submenu->addItem(L"Edit Material Shaders", CXT_MENU_EVENTS_EDIT_MATERIALS_SHADER);
	i++;

	//SHADERS
    submenu = menu->getSubMenu(i++);
	submenu->addItem(L"Manage Clouds... (TO DO)", -1);
	submenu->addItem(L"Manage Suns... (TO DO)", -1);
    submenu->addItem(L"Manage Vegetation... (TO DO)", -1);
    submenu->addItem(L"Manage Particle Systems... (WIP)", CXT_MENU_EVENTS_SCENE_MANAGE_PARTICLE_SYSTEMS);
    submenu->addSeparator();
    submenu->addItem(L"Add Terrain...", CXT_MENU_EVENTS_SCENE_ADD_TERRAIN);
    submenu->addItem(L"Add Object...", CXT_MENU_EVENTS_SCENE_ADD_OBJECT);
    submenu->addItem(L"Add Light...", CXT_MENU_EVENTS_SCENE_ADD_LIGHT);
    submenu->addItem(L"Add Volume Light...", CXT_MENU_EVENTS_SCENE_ADD_VOLUME_LIGHT);
    submenu->addItem(L"Add Water Surface...", CXT_MENU_EVENTS_SCENE_ADD_WATER_SURFACE);

	//FACTORY
	submenu = menu->getSubMenu(i++);
	submenu->addItem(L"Add Cube Scene Node", CXT_MENU_EVENTS_ADD_CUBE_SCENE_NODE);
	submenu->addItem(L"Add Sphere Scene Node", CXT_MENU_EVENTS_ADD_SPHERE_SCENE_NODE);
	submenu->addItem(L"Add Hill Plane Mesh", CXT_MENU_EVENTS_ADD_HILL_PLANE_MESH);
	submenu->addItem(L"Add a billboard", CXT_MENU_EVENTS_ADD_BILL_BOARD);
	submenu->addItem(L"Edit Skybox", -1, true, true);
	skyboxMenu = submenu->getSubMenu(4);
	skyboxMenu->addItem(L"Enable", CXT_MENU_EVENTS_NODE_FACTORY_ENABLE_SKYBOX, true, false, false, false);
	skyboxMenu->addItem(L"Edit...", -1);
	skyboxMenu->addItem(L"Edit Materials...", -1);
	submenu = menu->getSubMenu(i-1);
	submenu->addItem(L"Edit Skydome", -1, true, true);
	skydomeMenu = submenu->getSubMenu(5);
	skydomeMenu->addItem(L"Enable", CXT_MENU_EVENTS_NODE_FACTORY_ENABLE_SKYDOME, true, false, true, false);
	skydomeMenu->addItem(L"Edit... (TO DO)", CXT_MENU_EVENTS_NODE_FACTORY_EDIT_SKYDOME);
	skydomeMenu->addItem(L"Edit Materials... (TO DO)", CXT_MENU_EVENTS_NODE_FACTORY_EDIT_MATERIALS_SKYDOME);
	submenu = menu->getSubMenu(i-1);
	submenu->addSeparator();
	submenu->addItem(L"Mesh Manipulator", -1, true, true);
	submenu = submenu->getSubMenu(7);
	submenu->addItem(L"Make Planar Texture Mapping...", CXT_MENU_EVENTS_NODE_FACTORY_PLANAR_TEXTURE_MAPPING);
	submenu->addItem(L"Create Mesh WIth Tangents...", CXT_MENU_EVENTS_NODE_FACTORY_CREATE_MESH_WITH_TANGENTS);
	submenu->addItem(L"Scale Mesh...", -1);
	submenu = menu->getSubMenu(i-1);
	submenu->addSeparator();
	submenu->addItem(L"New Terrain... (WIP)", CXT_MENU_EVENTS_NODE_FACTORY_NEW_TERRAIN);

	//SCRIPTING
	submenu = menu->getSubMenu(i++);
	submenu->addItem(L"Open Script Editor (WIP)", CXT_MENU_EVENTS_SCRIPTS_OPEN_EDITOR);

	//SOUNDS
	submenu = menu->getSubMenu(i++);
	submenu->addItem(L"Edit Sounds System (TO DO)", -1);

	//WINDOW
    submenu = menu->getSubMenu(i++);
	submenu->addItem(L"Current Rendering Infos", CXT_MENU_EVENTS_RENDERING_INFOS);
	submenu->addItem(L"Draw/Hide Main Window", CXT_MENU_EVENTS_HIDE_DRAW_MAIN_WINDOW);
	submenu->addSeparator();
	submenu->addItem(L"About...", CXT_MENU_EVENTS_HELP_ABOUT);

	//PLUGINS
	submenu = menu->getSubMenu(i++);
	submenu->addItem(L"Edit Plugins...", CXT_MENU_EVENTS_PLUGINS_EDIT);
	devices->getDevice()->getFileSystem()->changeWorkingDirectoryTo("plugins/monitors");
	submenu->addItem(L"Monitors", -1, true, true);
	monitorsMenu = submenu->getSubMenu(1);
	submenu = monitorsMenu;
	IFileList *fl = devices->getDevice()->getFileSystem()->createFileList();
	for (u32 j=0; j < fl->getFileCount(); j++) {
		if (fl->getFileName(j) != "." && fl->getFileName(j) != "..") {
			stringw pluginname = fl->getFileName(j);
			pluginname.remove(".dll");
			submenu->addItem(pluginname.c_str(), -1, true, false, false, true);
		}
	}
	devices->getDevice()->getFileSystem()->changeWorkingDirectoryTo("../../");
    //-----------------------------------

    //-----------------------------------
    //TOOLBAR
    bar = devices->getGUIEnvironment()->addToolBar(0, -1);
    bar->setRelativePosition(position2d<int>(0, menu->getRelativePosition().getHeight()+7));
	ITexture* image = devices->getVideoDriver()->getTexture("GUI/folder.png");
	bar->addButton(CXT_MENU_EVENTS_OPEN_SCRIPT, 0, L"Open a scene", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture("GUI/edit.png");
	bar->addButton(CXT_MENU_EVENTS_EDIT_NODE, 0, L"Edit Selected Node", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture("GUI/save.png");
	bar->addButton(CXT_MENU_EVENTS_EXPORT_SCENE, 0, L"Export this scene", image, 0, false, true);

    bar->addButton(-1, 0, L"", image, false, true)->setVisible(false);

    image = devices->getVideoDriver()->getTexture("GUI/edit_ao.png");
    bar->addButton(CXT_MENU_EVENTS_EDIT_AO, 0, L"Edit Animated Object", image, 0, false, true);
    image = devices->getVideoDriver()->getTexture("GUI/animators.png");
    bar->addButton(-1, 0, L"Edit animators of selected node", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture("GUI/shaders.png");
    bar->addButton(CXT_MENU_EVENTS_EDIT_MATERIAL_SHADERS, 0, L"Edit Material Shaders", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture("GUI/effects.png");
    bar->addButton(CXT_MENU_EVENTS_EDIT_EFFECTS, 0, L"Edit Effects", image, 0, false, true);

	bar->addButton(-1, 0, L"", image, false, true)->setVisible(false);

	image = devices->getVideoDriver()->getTexture("GUI/scripts.png");
	bar->addButton(CXT_MENU_EVENTS_OPEN_SCRIPT_EDITOR, 0, L"open script editor", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture("GUI/planarmapping.png");
	bar->addButton(CXT_MENU_EVENTS_MAKE_PLANAR_MAPPING, 0, L"Make planar mapping to node...", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture("GUI/tangents.png");
	bar->addButton(CXT_MENU_EVENTS_CREATE_MESH_WITH_TANGENTS, 0, L"Create mesh with tangents", image, 0, false, true);

	IGUIEnvironment *gui = devices->getGUIEnvironment();
	IVideoDriver *driver = devices->getVideoDriver();
	contextNameText = gui->addStaticText(L"", rect<s32>(bar->getRelativePosition().getWidth()-400, 5,
														bar->getRelativePosition().getWidth(), 25), 
										true, true, bar, -1, true);
	contextNameText->setBackgroundColor(SColor(255, 0, 0, 0));
	contextNameText->setOverrideColor(SColor(255, 255, 255, 255));
	contextNameText->setTextAlignment(EGUIA_LOWERRIGHT, EGUIA_CENTER);

    //-----------------------------------

    //-----------------------------------
    //INFORMATIONS BAR
    infosBar = devices->getGUIEnvironment()->addToolBar(0, -1);
    infosBar->setRelativePosition(position2d<int>(0, bar->getRelativePosition().getHeight() +
                                                  menu->getRelativePosition().getHeight()));
    image = devices->getVideoDriver()->getTexture("GUI/Render.png");
    infosBar->addButton(CXT_BAR_EVENTS_RENDER, 0, L"Render Current View", image, 0, false, true);
    image = devices->getVideoDriver()->getTexture("GUI/console.png");
    consoleButton = infosBar->addButton(CXT_BAR_EVENTS_LOG_WINDOW, 0, L"Show/Hide Console", image, 0, false, true);
    image = devices->getVideoDriver()->getTexture("GUI/help.png");
	infosBar->addButton(CXT_MENU_EVENTS_HELP, 0, L"Open Help", image, 0, false, true);

    infosBar->addButton(-1, 0, L"", image, false, true)->setVisible(false);

    //PLACE POSITION, ROTATION, SCALE HERE
    image = devices->getVideoDriver()->getTexture("GUI/position.png");
	(ibposition = infosBar->addButton(-1, 0, L"Change Object Position", image, 0, false, true))->setIsPushButton();
    image = devices->getVideoDriver()->getTexture("GUI/rotation.png");
	(ibrotation = infosBar->addButton(-1, 0, L"Change Object Rotation", image, 0, false, true))->setIsPushButton();
    image = devices->getVideoDriver()->getTexture("GUI/scale.png");
	(ibscale = infosBar->addButton(-1, 0, L"Change Object Scale", image, 0, false, true))->setIsPushButton();

	infosBar->addButton(-1, 0, L"", image, false, true)->setVisible(false);
	infosBar->addButton(-1, 0, L"", image, false, true)->setVisible(false);

	//CUBE SHPERE PLANE BILLBOARD
	image = devices->getVideoDriver()->getTexture("GUI/cube.png");
	infosBar->addButton(CXT_MENU_EVENTS_CREATE_CUBE, 0, L"Add a cube scene node", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture("GUI/sphere.png");
	infosBar->addButton(CXT_MENU_EVENTS_CREATE_SPHERE, 0, L"Add a sphere scene node", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture("GUI/hill_plane.png");
	infosBar->addButton(CXT_MENU_EVENTS_CREATE_PLANE, 0, L"Add a hill plane scene node", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture("GUI/billboard.png");
	infosBar->addButton(CXT_MENU_EVENTS_CREATE_BILLBOARD, 0, L"Add a billboard scene node", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture("GUI/light.png");
	infosBar->addButton(CXT_MENU_EVENTS_CREATE_LIGHT, 0, L"Add a light scene node", image, 0, false, true);

	comboModecb = devices->getGUIEnvironment()->addComboBox(rect<s32>(infosBar->getRelativePosition().getWidth()-400, 5,
																	  infosBar->getRelativePosition().getWidth(), 25),
															0, -1);
	comboModecb->addItem(L"Render Mode");
	comboModecb->addItem(L"Full Mode");
	comboModecb->addItem(L"Edit Mode");
	comboModecb->setSubElement(false);

    //-----------------------------------

    timer = devices->getDevice()->getTimer();
    timer->start();
    timer->setTime(0);

	#ifndef SSWE_RELEASE
		for (u32 i=0; i < 1; i++) {
			//devices->getCoreData()->clear();
			//devices->getCoreData()->clearAllTheArrays();
			//devices->getXEffect()->clearAll();

			stringw scene_to_import = L"LTest.world";
			CImporter *impoterInstance = new CImporter(devices);
			impoterInstance->importScene(scene_to_import.c_str());
			//impoterInstance->setPathOfFile_t(scene_to_import.c_str());
			//std::thread importer_t(&CImporter::import_t, *impoterInstance);
			//importer_t.detach();

			scene_to_import.remove(L".world");
			exportSceneInstance->setPathFile(scene_to_import.c_str());
			delete impoterInstance;

			printf("Importing scene number %u \n", i);
		}

		/*if (devices->getCoreData()->getTerrainsCount() > 0) {
			if (devices->getCoreData()->getTerrainsData()->operator[](0).getNode()->getType() == ESNT_TERRAIN) {
				CUITerrainPainter *terrainPainter = new CUITerrainPainter(devices, devices->getCoreData()->getTerrainsData()->operator[](0), mainWindowInstance);
			}
		}*/
	#endif

	CImporter *impoterInstance = new CImporter(devices);
	impoterInstance->importCamerasConfig();

    //CUIWindowEditNode *edit = new CUIWindowEditNode(devices);
    //edit->open(devices->getCoreData()->getTerrainNodes()->operator[](0), L"#terrain:");

    //CUIWindowEditEffects *editEffects = new CUIWindowEditEffects(devices);
    //editEffects->open();

    //CUIWindowEditLight *editLight = new CUIWindowEditLight(devices, 0);
    //editLight->open(devices->getCoreData()->getLightsNodes()->operator[](0), "#light:");

    //CUIWindowEditMaterials *editMaterials = new CUIWindowEditMaterials(devices);
	//editMaterials->open();

    //CUIMaterialEditor *matEditor = new CUIMaterialEditor(devices);
    //matEditor->open(devices->getCoreData()->getTerrainNodes()->operator[](0));

	//CUICharacterWindow *editChar = new CUICharacterWindow(devices);
	//editChar->open();
	//editChar->setModel((IAnimatedMeshSceneNode *)devices->getCoreData()->getObjectsData()->operator[](3).getNode(), 3);

	movementType = CCoreObjectPlacement::Undefined;
	devices->getObjectPlacement()->setArrowType(movementType);

	devices->setContextName("General");

	//CUITexturesManager *texmgr = new CUITexturesManager(devices);
	//CUIPluginsManager *pm = new CUIPluginsManager(devices, pluginsManager);

	//pluginsManager->loadSSWEPlugin("SSWEWEBPLUGIN_D");

	//CUISSWEOptions *preferences = new CUISSWEOptions(devices);
    
    CUIParticlesEditor *editor = new CUIParticlesEditor(devices);

	//LIGHT SCATTERING
	/*devices->getRenderCallbacks()->buildLightScattering();
	for (u32 i=0; i < devices->getCoreData()->getObjectsData()->size(); i++) {
		devices->getXEffect()->addNodeToDepthPass(devices->getCoreData()->getObjectsData()->operator[](i).getNode());
	}
	devices->getXEffect()->addNodeToDepthPass(devices->getCoreData()->getLightsData()->operator[](0).getLensFlareBillBoardSceneNode());
	devices->getCoreData()->getLightsData()->operator[](0).getLensFlareSceneNode()->setVisible(false);*/

	//CUIWindowEditFilters *f = new CUIWindowEditFilters(devices);
    
    //devices->createFileOpenDialog(L"Test...", CGUIFileSelector::EFST_OPEN_DIALOG, 0, false);
}

CUIContextMenu::~CUIContextMenu() {
    
}

void CUIContextMenu::update() {
	if (devices->getContextName() == "General") {
		if (mainWindowInstance->getMainWindow()->isVisible())
			mainWindowInstance->getMainWindow()->setVisible(true);
		devices->getCore()->deactiveChildrenOfGUIElement(bar, true);
		mainWindowInstance->refresh();
	} else {
		mainWindowInstance->getMainWindow()->setVisible(false);
		devices->getCore()->deactiveChildrenOfGUIElement(bar, false);
	}

	if (devices->getDevice()->getCursorControl()->getPosition().Y < 75) {
		contextNameText->setVisible(true);
		contextNameText->setRelativePosition(rect<s32>(bar->getRelativePosition().getWidth()-200, 5,
													   bar->getRelativePosition().getWidth(), 25));
		contextNameText->setText(stringw(stringw("Context : ") + devices->getContextName().c_str()).c_str());
		contextNameText->setToolTipText(stringw(stringw("Context : ") + devices->getContextName().c_str()).c_str());
	} else {
		contextNameText->setVisible(false);
	}

	comboModecb->setRelativePosition(rect<s32>(infosBar->getRelativePosition().getWidth()-200, infosBar->getRelativePosition().UpperLeftCorner.Y+5,
											   infosBar->getRelativePosition().getWidth(), infosBar->getRelativePosition().UpperLeftCorner.Y+25));
}

bool CUIContextMenu::OnEvent(const SEvent &event) {

	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP && devices->getGUIEnvironment()->getFocus() == menu) {
			stringc oldPath = devices->getDevice()->getFileSystem()->getWorkingDirectory().c_str();
			devices->getDevice()->getFileSystem()->changeWorkingDirectoryTo(stringc(devices->getWorkingDirectory() + "plugins/monitors").c_str());
			IFileList *fl = devices->getDevice()->getFileSystem()->createFileList();
			monitorsMenu->removeAllItems();
			for (u32 j=0; j < fl->getFileCount(); j++) {
				if (fl->getFileName(j) != "." && fl->getFileName(j) != "..") {
					stringw pluginname = fl->getFileName(j);
                    #ifndef _IRR_OSX_PLATFORM_
					pluginname.remove(".dll");
                    #else
                    pluginname.remove(".dylib");
                    #endif
					pluginname.make_upper();
					u32 itemID = monitorsMenu->addItem(pluginname.c_str(), -1, true, false, false, true);
					for (u32 i=0; i < devices->getMonitorRegister()->getMonitorCount(); i++) {
						if (devices->getMonitorRegister()->getMonitor(i)->getName() == pluginname) {
							monitorsMenu->setItemChecked(itemID, true);
							break;
						}
					}
				}
			}
			devices->getDevice()->getFileSystem()->changeWorkingDirectoryTo(oldPath.c_str());
		}
	}

    if (event.EventType == EET_GUI_EVENT) {
        s32 id = event.GUIEvent.Caller->getID();
        //-----------------------------------
        //CONTEXT MENU EVENT
		if (event.GUIEvent.EventType == EGET_MENU_ITEM_SELECTED) {
            
            IGUIContextMenu *tempMenu = (IGUIContextMenu *)event.GUIEvent.Caller;

			if (tempMenu == monitorsMenu) {
				pluginsManager->loadMonitorPlugin(monitorsMenu->getItemText(tempMenu->getSelectedItem()));
			}

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

				case CXT_MENU_EVENTS_FILE_CONCAT_SCENE_SCRIPT:
					openSceneInstance->open(true);
					break;

				case CXT_MENU_EVENTS_FILE_ADD_NODES_GROUP: {
					CUILoadNodesGroup *loadGroup = new CUILoadNodesGroup(devices);
				}
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
					ISceneNode *prefix = mainWindowInstance->getSelectedNode().getNode();
					if (prefix->getType() != ESNT_LIGHT) {
                        CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
						editNode->open(mainWindowInstance->getSelectedNode().getNode(), 
									   mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode()));
                    } else {
                        CUIWindowEditLight *editLight = new CUIWindowEditLight(devices, mainWindowInstance->getActiveListBox()->getSelected());
						editLight->open(mainWindowInstance->getSelectedNode().getNode(),
										mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode()));
                    }
                }
                    break;
                    
                case CXT_MENU_EVENTS_EDIT_EDIT_MATERIALS_SELECTED_NODE: {
					ISceneNode *prefix = mainWindowInstance->getSelectedNode().getNode();
					if (prefix->getType() != ESNT_LIGHT) {
                        CUIMaterialEditor *matEditor = new CUIMaterialEditor(devices);
						matEditor->open(mainWindowInstance->getSelectedNode().getNode());
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

				case CXT_MENU_EVENTS_EDIT_OPTIONS: {
					CUISSWEOptions *preferences = new CUISSWEOptions(devices);
				}
					break;
                //-----------------------------------
                
                //-----------------------------------
                //CONTEXT MENU VIEW EVENT
                case CXT_MENU_EVENTS_VIEW_MAYA_CAMERA:
                    devices->getSceneManager()->setActiveCamera(devices->getMayaCamera());
                    break;
                    
                case CXT_MENU_EVENTS_VIEW_FPS_CAMERA: {
                    CUIEditFPSCamera *editFPS = new CUIEditFPSCamera(devices);
				}
                    break;
                    
                case CXT_MENU_EVENTS_VIEW_VIEW_TREE_NODES_WINDOW:
                    sceneViewInstance->open();
                    break;

				case CXT_MENU_EVENTS_VIEW_TEXTURES_MANAGER: {
					CUITexturesManager *texmgr = new CUITexturesManager(devices);
				}
					break;
                    
                case CXT_MENU_EVENTS_VIEW_WIREFRAME: {
					ISceneNode *nodeWireFrame = mainWindowInstance->getSelectedNode().getNode();
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
					ISceneNode *nodePointCloud = mainWindowInstance->getSelectedNode().getNode();
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
				case CXT_MENU_EVENTS_RENDERS_DRAW_FULL_PT: {
					devices->setRenderFullPostTraitements(!devices->isRenderingFullPostTraitements());
					for (u32 i=0; i < devices->getMonitorRegister()->getMonitorCount(); i++) {
						devices->getMonitorRegister()->getMonitor(i)->setRenderingFullTraitement(devices->isRenderingFullPostTraitements());
					}
				}
					break;
				case CXT_MENU_EVENTS_RENDERS_DOF_DRAW:
					devices->getXEffect()->setUseDepthOfField(!devices->getXEffect()->isUsingDepthOfField());
					break;
                case CXT_MENU_EVENTS_RENDERS_MOTION_BLUR_DRAW:
					devices->getXEffect()->setUseMotionBlur(!devices->getXEffect()->isUsingMotionBlur());
                    break;
				case CXT_MENU_EVENTS_RENDERS_SSAO:
					if (!devices->getRenderCallbacks()->getSSAORenderCallback()) {
						devices->getRenderCallbacks()->buildSSAO();
					} else {
						devices->getRenderCallbacks()->removeSSAO();
					}
					break;
                    
                case CXT_MENU_EVENTS_RENDERS_XEFFECT_DRAW:
                    devices->setXEffectDrawable(!devices->isXEffectDrawable());
					for (u32 i=0; i < devices->getMonitorRegister()->getMonitorCount(); i++) {
						devices->getMonitorRegister()->getMonitor(i)->setRenderRenderer(devices->isXEffectDrawable());
					}
                    break;
                    
                case CXT_MENU_EVENTS_RENDERS_XEFFECT_EDIT: {
                    //CUIWindowEditEffects *editEffects = new CUIWindowEditEffects(devices);
                    //editEffects->open();
					CUIWindowEditFilters *editFilters = new CUIWindowEditFilters(devices);
                }
					break;
				case CXT_MENU_EVENTS_RENDERS_XEFFECT_RECALCULATE_LIGHTS: {
					for (u32 i=0; i < devices->getXEffect()->getShadowLightCount(); i++) {
						devices->getXEffect()->getShadowLight(i).setRecalculate(true);
					}
				}
                    break;
				case CXT_MENU_EVENTS_RENDERS_EDIT_DOF: {
					CUIWindowEditDOF *editDOF = new CUIWindowEditDOF(devices);
				}
					break;
                //-----------------------------------

                //-----------------------------------
                //CONTEXT MENU SHADERS EVENT
                case CXT_MENU_EVENTS_EDIT_MATERIALS_SHADER: {
                    CUIWindowEditMaterials *editMaterials = new CUIWindowEditMaterials(devices);
                    editMaterials->open();
                }
                    break;
                //-----------------------------------

                //-----------------------------------
                //CONTEXT MENU ANIMATED OBJECTS EVENT
                case CXT_MENU_EVENTS_ANIMATED_MODELS_WINDOW_EDITION: {
					if (mainWindowInstance->getSelectedNode().getNode()->getType() == ESNT_ANIMATED_MESH) {
						if (mainWindowInstance->getSelectedNode().getNode()) {
							CUICharacterWindow *characterWindowInstance = new CUICharacterWindow(devices);
                            characterWindowInstance->open();
							characterWindowInstance->setModel((IAnimatedMeshSceneNode *)mainWindowInstance->getSelectedNode().getNode(), mainWindowInstance->getActiveListBox()->getSelected());
                        }
                    }
                }
                    break;
                //-----------------------------------
                    
                //-----------------------------------
                //CONTEXT MENU SCENE EVENT
                    
                case CXT_MENU_EVENTS_SCENE_MANAGE_PARTICLE_SYSTEMS: {
                    CUIParticlesEditor *pse = new CUIParticlesEditor(devices);
                }
                    break;
                    
                case CXT_MENU_EVENTS_SCENE_ADD_TERRAIN:
                    mainWindowInstance->openAddTerrain();
                    break;
                case CXT_MENU_EVENTS_SCENE_ADD_OBJECT:
                    mainWindowInstance->openAddObject();
                    break;
                case CXT_MENU_EVENTS_SCENE_ADD_LIGHT:
                    mainWindowInstance->openAddLight();
                    break;
                case CXT_MENU_EVENTS_SCENE_ADD_VOLUME_LIGHT:
                    mainWindowInstance->openAddVolumeLight();
                    break;
                case CXT_MENU_EVENTS_SCENE_ADD_WATER_SURFACE:
                    mainWindowInstance->openAddWaterSurface();
                    break;
                    
                //-----------------------------------
                
				//-----------------------------------
                //CONTEXT MENU MESH FACTORY EVENT
				case CXT_MENU_EVENTS_ADD_CUBE_SCENE_NODE:
					nodeFactory->createCubeSceneNode();
					break;

				case CXT_MENU_EVENTS_ADD_SPHERE_SCENE_NODE:
					nodeFactory->createSphereSceneNode();
					break;

				case CXT_MENU_EVENTS_ADD_HILL_PLANE_MESH:
					nodeFactory->createPlaneMeshSceneNode();
					break;

				case CXT_MENU_EVENTS_ADD_BILL_BOARD:
					nodeFactory->createBillBoardSceneNode();
					break;

				case CXT_MENU_EVENTS_NODE_FACTORY_ENABLE_SKYDOME: {
					if (skydomeMenu->isItemChecked(0)) {
						if (devices->getSkydome()) {
							devices->getSkydome()->setVisible(false);
						}
						skydomeMenu->setItemChecked(0, false);
					} else {
						if (devices->getSkydome()) {
							devices->getSkydome()->setVisible(true);
							if (devices->getSkyBox()) {
								devices->getSkyBox()->setVisible(false);
							}
						}
						skyboxMenu->setItemChecked(0, false);
						skydomeMenu->setItemChecked(0, true);
					}
				}
					break;
                    
                case CXT_MENU_EVENTS_NODE_FACTORY_EDIT_SKYBOX: {
                    if (devices->getSkyBox()) {
						CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
						editNode->open(devices->getSkyBox(), "#object", false);
					}
                }
                    break;
                    
                case CXT_MENU_EVENTS_NODE_FACTORY_EDIT_MATERIALS_SKYBOX: {
                    if (devices->getSkyBox()) {
						CUIMaterialEditor *matEditor = new CUIMaterialEditor(devices);
						matEditor->open(devices->getSkyBox());
					}
                }
                    break;

				case CXT_MENU_EVENTS_NODE_FACTORY_EDIT_SKYDOME: {
					if (devices->getSkydome()) {
						CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
						editNode->open(devices->getSkydome(), "#object", false);
					}
				}

					break;

				case CXT_MENU_EVENTS_NODE_FACTORY_EDIT_MATERIALS_SKYDOME: {
					if (devices->getSkydome()) {
						CUIMaterialEditor *matEditor = new CUIMaterialEditor(devices);
						matEditor->open(devices->getSkydome());
					}
				}
					break;

				case CXT_MENU_EVENTS_NODE_FACTORY_ENABLE_SKYBOX: {
					if (skyboxMenu->isItemChecked(0)) {
						if (devices->getSkyBox()) {
							devices->getSkyBox()->setVisible(false);
						}
						skyboxMenu->setItemChecked(0, false);
					} else {
						if (devices->getSkyBox()) {
							devices->getSkyBox()->setVisible(true);
							if (devices->getSkydome()) {
								devices->getSkydome()->setVisible(false);
							}
						}
						skydomeMenu->setItemChecked(0, false);
						skyboxMenu->setItemChecked(0, true);
					}
				}
					break;

				case CXT_MENU_EVENTS_NODE_FACTORY_PLANAR_TEXTURE_MAPPING: {
					SSelectedNode ssn = mainWindowInstance->getSelectedNode();
					if (ssn.getNode() && ssn.getMesh()) {
						CUINodeFactoryPlanarMapping *nfpm = new CUINodeFactoryPlanarMapping(devices);
						nfpm->open(ssn.getNode(), ssn.getMesh());
					} else if (ssn.getNode() && !ssn.getMesh()) {
						devices->addInformationDialog(L"Information", L"You cannot modify this kind of node...", EMBF_OK);
					} else if (!ssn.getNode() && ssn.getMesh()) {
						devices->addInformationDialog(L"Information", L"I don't know what the fuck is going on...\n"
														L"You have the mesh but not the node...", EMBF_OK);
					} else {
						devices->addWarningDialog(L"Warning", L"Please select a node before...", EMBF_OK);
					}
				}
					break;

				case CXT_MENU_EVENTS_NODE_FACTORY_CREATE_MESH_WITH_TANGENTS: {
					SSelectedNode ssn = mainWindowInstance->getSelectedNode();
					if (ssn.getNode() && ssn.getMesh()) {
						CUINodeFactoryCreateMeshWithTangents *createTangents = new CUINodeFactoryCreateMeshWithTangents(devices);
						createTangents->open(ssn.getNode(), ssn.getMesh());
						createTangents->setType(ssn.getNode()->getType());
						createTangents->setMinPolysPerNode(ssn.getMinPolysPerNode());
						createTangents->setMeshPath(ssn.getPath());
					} else if (ssn.getNode() && !ssn.getMesh()) {
						devices->addInformationDialog(L"Information", L"You cannot modify this kind of node...", EMBF_OK);
					} else if (!ssn.getNode() && ssn.getMesh()) {
						devices->addInformationDialog(L"Information", L"I don't know what the fuck is going on...\n"
														L"You have the mesh but not the node...", EMBF_OK);
					} else {
						devices->addWarningDialog(L"Warning", L"Please select a node before...", EMBF_OK);
					}
				}
					break;

				case CXT_MENU_EVENTS_NODE_FACTORY_NEW_TERRAIN: {
					CUIWindowAddNewTerrain *newTerrain = new CUIWindowAddNewTerrain(devices);
				}
					break;

				//-----------------------------------

				case CXT_MENU_EVENTS_SCRIPTS_OPEN_EDITOR: {
					CUIScriptEditor *spte = new CUIScriptEditor(devices);
				}
					break;

                //-----------------------------------
                //CONTEXT MENU WINDOW EVENT
                case CXT_MENU_EVENTS_RENDERING_INFOS: {
                    CUIRenderingInfos *renderInfos = new CUIRenderingInfos(devices);
                }
                    break; 

				case CXT_MENU_EVENTS_HIDE_DRAW_MAIN_WINDOW:
					mainWindowInstance->getMainWindow()->setVisible(!mainWindowInstance->getMainWindow()->isVisible());
					break;
                //-----------------------------------
                    
				//-----------------------------------
                //HELP MENU EVENT
				case CXT_MENU_EVENTS_HELP_ABOUT:
					//devices->addInformationDialog(L"About...", L"Created by Julien Moreau-Mathis\n"
					//										   L"All rights reserved", 
					//							  EMBF_OK);
                    devices->getGUIEnvironment()->addMessageBox(L"About...", L"Created by Julien Moreau-Mathis\n"
                                                                             L"All rights reserved",
                                                                true, EMBF_OK, 0, -1,
                                                                devices->getVideoDriver()->getTexture(devices->getWorkingDirectory() + "GUI/ss_logo.png"));
					break;
				//-----------------------------------

				case CXT_MENU_EVENTS_PLUGINS_EDIT: {
					CUIPluginsManager *uiPluginsManager = new CUIPluginsManager(devices, pluginsManager);
				}
					break;
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
                    //Clear CoreData
                    devices->getCoreData()->clearAllTheArrays();
					//Resetting XEffect Framework
					devices->getXEffect()->clearAll();
                    //Resetting grid
                    devices->getObjectPlacement()->getGridSceneNode()->SetAccentlineOffset(8);
                    devices->getObjectPlacement()->getGridSceneNode()->SetSize(1024);
                    devices->getObjectPlacement()->getGridSceneNode()->SetSpacing(8);

					devices->getCollisionManager()->getMetaTriangleSelectors()->removeAllTriangleSelectors();
                    break;
                    
                default:
                    break;
            }
        }
        
        //-----------------------------------
        //COMBO BOX GUI EVENT
        
        if (event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED) {
            if (event.GUIEvent.Caller == comboModecb) {
                if (comboModecb->getSelected() == 0) {
                    devices->setXEffectDrawable(true);
                    devices->getXEffect()->setAllShadowLightsRecalculate();
                }
                if (comboModecb->getSelected() == 1) {
                    devices->setXEffectDrawable(true);
                    devices->setRenderFullPostTraitements(true);
                    devices->getXEffect()->setAllShadowLightsRecalculate();
                }
                if (comboModecb->getSelected() == 2) {
                    devices->setRenderFullPostTraitements(false);
                    devices->setXEffectDrawable(false);
                }
            }
        }
        
        //-----------------------------------
        //LISTBOX GUI EVENT
        
        if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
            submenu = menu->getSubMenu(2);
			ISceneNode *node = mainWindowInstance->getSelectedNode().getNode();
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

		//NODE SELECTION IN MAIN WINDOW CHANGED, DOUBLE CLICK TO CONFIRM ARROWS, OR SINGLE CLICK ??????????
		if (event.GUIEvent.EventType == EGET_LISTBOX_SELECTED_AGAIN || event.GUIEvent.EventType == EGET_LISTBOX_SELECTED_AGAIN) {
			if (event.GUIEvent.Caller == mainWindowInstance->getActiveListBox()) {
				if (movementType != CCoreObjectPlacement::Undefined) {
					CCoreObjectPlacement *cobj = devices->getObjectPlacement();
					cobj->setArrowType(movementType);
					cobj->setNodeToPlace(mainWindowInstance->getSelectedNode().getNode());
					cobj->setArrowVisible(true);
				}
			}
		}
        
        //-----------------------------------
        
        //-----------------------------------
        //BUTTON GUI EVENT
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {

			IGUIElement *caller = event.GUIEvent.Caller;
			if (caller == ibposition || caller == ibrotation || caller == ibscale) {
				movementType = CCoreObjectPlacement::Undefined;
				devices->getObjectPlacement()->setArrowType(movementType);
				devices->getObjectPlacement()->setNodeToPlace(0);
				devices->getObjectPlacement()->setArrowVisible(false);

				devices->getGUIEnvironment()->setFocus(devices->getGUIEnvironment()->getRootGUIElement());
			}

			//POSITION
			if (event.GUIEvent.Caller == ibposition) {
				if (ibposition->isPressed()) {
					movementType = CCoreObjectPlacement::Position;
					devices->getObjectPlacement()->setArrowType(movementType);
					devices->getObjectPlacement()->setNodeToPlace(mainWindowInstance->getSelectedNode().getNode());
					devices->getObjectPlacement()->setArrowVisible(true);
				}
				ibrotation->setPressed(false);
				ibscale->setPressed(false);
			}

			//ROTATION
			if (event.GUIEvent.Caller == ibrotation) {
				if (ibrotation->isPressed()) {
					movementType = CCoreObjectPlacement::Rotation;
					devices->getObjectPlacement()->setArrowType(movementType);
					devices->getObjectPlacement()->setNodeToPlace(mainWindowInstance->getSelectedNode().getNode());
					devices->getObjectPlacement()->setArrowVisible(true);
				}
				ibposition->setPressed(false);
				ibscale->setPressed(false);
			}

			//SCALE
			if (event.GUIEvent.Caller == ibscale) {
				if (ibscale->isPressed()) {
					movementType = CCoreObjectPlacement::Scale;
					devices->getObjectPlacement()->setArrowType(movementType);
					devices->getObjectPlacement()->setNodeToPlace(mainWindowInstance->getSelectedNode().getNode());
					devices->getObjectPlacement()->setArrowVisible(true);
				}
				ibposition->setPressed(false);
				ibrotation->setPressed(false);
			}

            switch (id) {
                //-----------------------------------
                //TOOLBAR
                case CXT_MENU_EVENTS_OPEN_SCRIPT:
                    openSceneInstance->open();
                    break;
                    
                case CXT_MENU_EVENTS_EDIT_NODE: {
					stringc prefix = mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode());
                    if (prefix != "#light") {
                        CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
						editNode->open(mainWindowInstance->getSelectedNode().getNode(), 
									  mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode()));
                    } else {
                        CUIWindowEditLight *editLight = new CUIWindowEditLight(devices, mainWindowInstance->getActiveListBox()->getSelected());
						editLight->open(mainWindowInstance->getSelectedNode().getNode(),
										mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode()));
                    }
                }
                    break;
                    
                case CXT_MENU_EVENTS_EXPORT_SCENE:
                    exportSceneInstance->setPathFile(openSceneInstance->getPathFile());
                    exportSceneInstance->open();
                    break;
                    
                case CXT_MENU_EVENTS_EDIT_AO: {
					if (mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode()) == "#object") {
						if (mainWindowInstance->getSelectedNode().getNode()) {
							CUICharacterWindow *characterWindowInstance = new CUICharacterWindow(devices);
                            characterWindowInstance->open();
							characterWindowInstance->setModel((IAnimatedMeshSceneNode *)mainWindowInstance->getSelectedNode().getNode(), mainWindowInstance->getActiveListBox()->getSelected());
                        }
                    }
                }
                    break;
                    
                case CXT_MENU_EVENTS_EDIT_MATERIAL_SHADERS: {
                    CUIWindowEditMaterials *editMat = new CUIWindowEditMaterials(devices);
                    editMat->open();
                }
                    break;

				case CXT_MENU_EVENTS_EDIT_EFFECTS: {
                    //CUIWindowEditEffects *editEffects = new CUIWindowEditEffects(devices);
					//editEffects->open();
					CUIWindowEditFilters *editFilters = new CUIWindowEditFilters(devices);
                }
                    break;

				case CXT_MENU_EVENTS_OPEN_SCRIPT_EDITOR: {
					CUIScriptEditor *scriptEditor = new CUIScriptEditor(devices);
				}
					break;

				case CXT_MENU_EVENTS_MAKE_PLANAR_MAPPING: {
					if (mainWindowInstance->getSelectedNode().getNode() && mainWindowInstance->getSelectedNode().getMesh()) {
						CUINodeFactoryPlanarMapping *cuifpm = new CUINodeFactoryPlanarMapping(devices);
						cuifpm->open(mainWindowInstance->getSelectedNode().getNode(), mainWindowInstance->getSelectedNode().getMesh());
					}
				}
					break;

				case CXT_MENU_EVENTS_CREATE_MESH_WITH_TANGENTS: {
					if (mainWindowInstance->getSelectedNode().getNode() && mainWindowInstance->getSelectedNode().getMesh()) {
						CUINodeFactoryCreateMeshWithTangents *cuinfcwt = new CUINodeFactoryCreateMeshWithTangents(devices);
						cuinfcwt->open(mainWindowInstance->getSelectedNode().getNode(), mainWindowInstance->getSelectedNode().getMesh());
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
                    
                case CXT_BAR_EVENTS_LOG_WINDOW:
                    
                    break;

				case CXT_MENU_EVENTS_CREATE_CUBE:
					nodeFactory->createCubeSceneNode();
					break;
				case CXT_MENU_EVENTS_CREATE_SPHERE:
					nodeFactory->createSphereSceneNode();
					break;
				case CXT_MENU_EVENTS_CREATE_PLANE:
					nodeFactory->createPlaneMeshSceneNode();
					break;
				case CXT_MENU_EVENTS_CREATE_BILLBOARD:
					nodeFactory->createBillBoardSceneNode();
					break;
				case CXT_MENU_EVENTS_CREATE_LIGHT:
					nodeFactory->createLightSceneNode();
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

			//if (devices->getGUIEnvironment()->getFocus() != devices->getGUIEnvironment()->getRootGUIElement()) {
			//	return false;
			//}
			switch (event.KeyInput.Key) {
                    
                case KEY_KEY_X:
                    if (!devices->isEditBoxEntered() && devices->isCtrlPushed() && devices->isShiftPushed()) {
                        devices->setXEffectDrawable(!devices->isXEffectDrawable());
						for (u32 i=0; i < devices->getMonitorRegister()->getMonitorCount(); i++) {
							devices->getMonitorRegister()->getMonitor(i)->setRenderRenderer(devices->isXEffectDrawable());
						}
                    }
                    break;
                    
                case KEY_KEY_W: {
                    if (!devices->isEditBoxEntered() && devices->isCtrlPushed()) {
						ISceneNode *nodeWireFrame = mainWindowInstance->getSelectedNode().getNode();
                        if (nodeWireFrame) {
                            nodeWireFrame->setMaterialFlag(EMF_WIREFRAME, !nodeWireFrame->getMaterial(0).Wireframe);
                            submenu = menu->getSubMenu(3);
                            if (nodeWireFrame->getMaterial(0).Wireframe) {
                                submenu->setItemChecked(5, true);
                            } else {
                                submenu->setItemChecked(5, false);
                            }
                        } else {
                            devices->addWarningDialog(L"Warning", L"Please Select A Node Before...", EMBF_OK);
                        }
                    }
                }
                    break;
                    
                case KEY_KEY_A:
                    if (!devices->isEditBoxEntered() && devices->isCtrlPushed() && !devices->isShiftPushed()) {
						if (mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode()) == "#object") {
							if (mainWindowInstance->getSelectedNode().getNode()) {
								CUICharacterWindow *characterWindowInstance = new CUICharacterWindow(devices);
                                characterWindowInstance->open();
								characterWindowInstance->setModel((IAnimatedMeshSceneNode *)mainWindowInstance->getSelectedNode().getNode(), mainWindowInstance->getActiveListBox()->getSelected());
                            }
                        }
                    }
					if(!devices->isEditBoxEntered() && !devices->isCtrlPushed() && devices->isShiftPushed()) {
						stringc prefix = mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode());
						if(prefix == L"#animator") {
							devices->addWarningDialog(L"Warning", L"Please, select a node which is not an animator", EMBF_OK);
						} else {
						CUIWindowEditNodeAnimators *editNodeAnimator = new CUIWindowEditNodeAnimators(devices);
						editNodeAnimator->open(mainWindowInstance->getSelectedNode().getNode(), prefix);
						}
					}
                    break;
                    
                case KEY_KEY_O:
                    if (!devices->isEditBoxEntered() && devices->isCtrlPushed()) {
                        openSceneInstance->open();
                    }
                    break;
                    
                case KEY_KEY_S:
                    if (!devices->isEditBoxEntered() && devices->isCtrlPushed()) {
                        exportSceneInstance->open();
                    }
                    break;
                    
                case KEY_KEY_E: {
					if (!devices->isEditBoxEntered() && devices->isCtrlPushed() && !devices->isShiftPushed()) {
						stringc prefix = mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode());
                        if (prefix != "#light") {
                            CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
							editNode->open(mainWindowInstance->getSelectedNode().getNode(), 
										   mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode()));
                        } else {
                            CUIWindowEditLight *editLight = new CUIWindowEditLight(devices, mainWindowInstance->getActiveListBox()->getSelected());
							editLight->open(mainWindowInstance->getSelectedNode().getNode(),
											mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode()));
                        }
                    }
					if (!devices->isEditBoxEntered() && devices->isCtrlPushed() && devices->isShiftPushed()) {
						ISceneNode *node = mainWindowInstance->getSelectedNode().getNode();
						if (mainWindowInstance->getSelectedNode().getNode()) {
							bool canEdit = (node->getType() == ESNT_MESH || node->getType() == ESNT_OCTREE || node->getType() == ESNT_ANIMATED_MESH
								|| node->getType() == ESNT_BILLBOARD || node->getType() == ESNT_SKY_BOX || node->getType() == ESNT_SKY_DOME
								|| node->getType() == ESNT_TERRAIN || node->getType() == ESNT_CUBE || node->getType() == ESNT_SPHERE);
							if (canEdit) {
								CUIMaterialEditor *editMat = new CUIMaterialEditor(devices);
								editMat->open(node);
							}
						}
					}
					if (!devices->isEditBoxEntered() && !devices->isCtrlPushed() && devices->isShiftPushed()) {
						stringc prefix = mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode());
                        if (prefix != "#volumeLight") {
                            CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
							editNode->open(mainWindowInstance->getSelectedNode().getNode(), 
										   mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode()));
                        } else {
                            CUIWindowEditVolumeLight *editVolumeLight = new CUIWindowEditVolumeLight(devices);
							editVolumeLight->open(mainWindowInstance->getSelectedNode().getNode(),
											mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode()));
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
