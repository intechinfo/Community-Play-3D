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
	menu->addItem(L"Nodes Factory", -1, true, true);
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
    submenu->addItem(L"Edit Materials (CTRL+SHIFT+E)", CXT_MENU_EVENTS_EDIT_EDIT_MATERIALS_SELECTED_NODE);
    submenu->addSeparator();
	submenu->addItem(L"Set all the nodes lighting", CXT_MENU_EVENTS_EDIT_SET_ALL_NODES_LIGHTING);
	submenu->addItem(L"Set all the nodes not lighting", CXT_MENU_EVENTS_EDIT_SET_ALL_NODES_NOT_LIGHTING);
    submenu->addSeparator();
    submenu->addItem(L"Edit Grid", CXT_MENU_EVENTS_EDIT_GRID_SCENE_NODE);
    submenu->addSeparator();
    submenu->addItem(L"Clone (CTRL+SHIFT+C)", CXT_MENU_EVENTS_EDIT_CLONE);
    
    submenu = menu->getSubMenu(i++);
	submenu->addItem(L"Maya Camera (CTRL+SHIFT+M)", CXT_MENU_EVENTS_VIEW_MAYA_CAMERA);
	submenu->addItem(L"FPS Camera (CTRL+SHIFT+F)", CXT_MENU_EVENTS_VIEW_FPS_CAMERA);
    submenu->addSeparator();
	submenu->addItem(L"View Tree Node Window", CXT_MENU_EVENTS_VIEW_VIEW_TREE_NODES_WINDOW);
    submenu->addSeparator();
	submenu->addItem(L"Wire Frame (CTRL+W)", CXT_MENU_EVENTS_VIEW_WIREFRAME, true, false, false, false);
    submenu->addItem(L"Point Cloud", CXT_MENU_EVENTS_VIEW_POINTCLOUD, true, false, false, false);
    
    i++;//TO CHANGE
    
    submenu = menu->getSubMenu(i);
    submenu->addItem(L"Post Processes", -1, true, true);
	submenu->addItem(L"Effects", -1, true, true);
    submenu = submenu->getSubMenu(0);
    submenu->addItem(L"Draw Motion Blur", CXT_MENU_EVENTS_RENDERS_MOTION_BLUR_DRAW, true, false,
					devices->getXEffect()->isUsingMotionBlur(), true);
    submenu->addSeparator();
    submenu->addItem(L"...", CXT_MENU_EVENTS_RENDERS_HDR_EDIT);
    submenu = menu->getSubMenu(i);
	submenu = submenu->getSubMenu(1);
    submenu->addItem(L"Draw Effects (CTRL+X)", CXT_MENU_EVENTS_RENDERS_XEFFECT_DRAW);
    submenu->addSeparator();
    submenu->addItem(L"Edit (CTRL+SHIFT+X)", CXT_MENU_EVENTS_RENDERS_XEFFECT_EDIT);
    i++;
    
    submenu = menu->getSubMenu(i);
    submenu->addItem(L"Edit Material Shaders", CXT_MENU_EVENTS_EDIT_MATERIALS_SHADER);
    submenu->addItem(L"Edit Selected Water Shader", -1);
    i++;
    
    submenu = menu->getSubMenu(i++);
	submenu->addItem(L"Animated Models Window Edition (CTRL+A)", CXT_MENU_EVENTS_ANIMATED_MODELS_WINDOW_EDITION);
	submenu->addItem(L"Simple Edition", CXT_MENU_EVENTS_SIMPLE_EDITION);

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
	skydomeMenu->addItem(L"Edit...", CXT_MENU_EVENTS_NODE_FACTORY_EDIT_SKYDOME);
	skydomeMenu->addItem(L"Edit Materials...", CXT_MENU_EVENTS_NODE_FACTORY_EDIT_MATERIALS_SKYDOME);
	submenu = menu->getSubMenu(i-1);
	submenu->addItem(L"Mesh Manipulator", -1, true, true);
	submenu = submenu->getSubMenu(6);
	submenu->addItem(L"Make Planar Texture Mapping...", CXT_MENU_EVENTS_NODE_FACTORY_PLANAR_TEXTURE_MAPPING);
	submenu->addItem(L"Create Mesh WIth Tangents...", CXT_MENU_EVENTS_NODE_FACTORY_CREATE_MESH_WITH_TANGENTS);
	submenu->addItem(L"Scale Mesh...", -1);
	submenu = menu->getSubMenu(i-1);
    
    submenu = menu->getSubMenu(i++);
	submenu->addItem(L"Current Rendering Infos", CXT_MENU_EVENTS_RENDERING_INFOS);
	submenu->addItem(L"Draw/Hide Main Window", -1);
    //-----------------------------------
    
    //-----------------------------------
    //TOOLBAR
    bar = devices->getGUIEnvironment()->addToolBar(0, -1);
    bar->setRelativePosition(position2d<int>(0, menu->getRelativePosition().getHeight()+5));
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
    
    //-----------------------------------
    
    timer = devices->getDevice()->getTimer();
    timer->start();
    timer->setTime(0);
    
	stringw scene_to_import = L"zombie.world";
    CImporter *impoterInstance = new CImporter(devices);
	impoterInstance->importScene(scene_to_import.c_str());
	scene_to_import.remove(L".world");
	exportSceneInstance->setPathFile(scene_to_import.c_str());
    delete impoterInstance;
    //CUIWindowEditNode *edit = new CUIWindowEditNode(devices);
    //edit->open(devices->getCoreData()->getTerrainNodes()->operator[](0), L"#terrain:");
    
    //CUIWindowEditEffects *editEffects = new CUIWindowEditEffects(devices);
    //editEffects->open();
    //devices->setXEffectDrawable(true);
    
    //CUIWindowEditLight *editLight = new CUIWindowEditLight(devices, 0);
    //editLight->open(devices->getCoreData()->getLightsNodes()->operator[](0), "#light:");
    
    //CUIWindowEditMaterials *editMaterials = new CUIWindowEditMaterials(devices);
	//editMaterials->open();
    
    //CUIMaterialEditor *matEditor = new CUIMaterialEditor(devices);
    //matEditor->open(devices->getCoreData()->getTerrainNodes()->operator[](0));

	CUICharacterWindow *editChar = new CUICharacterWindow(devices);
	editChar->open();
	editChar->setModel((IAnimatedMeshSceneNode *)devices->getCoreData()->getObjectsData()->operator[](0).getNode(), 0);

	movementType = CCoreObjectPlacement::Undefined;
	devices->getObjectPlacement()->setArrowType(movementType);
}

CUIContextMenu::~CUIContextMenu() {
    
}

void CUIContextMenu::update() {
    //if (timer->getTime() / 3000 == 0 ) {
        //timer->setTime(0);
        mainWindowInstance->refresh();
    //}
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
					stringc prefix = mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode());
                    if (prefix == "#map" || prefix == "#tree" || prefix == "#object") {
                        CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
						editNode->open(mainWindowInstance->getSelectedNode().getNode(), 
									   mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode()));
                    } else if (prefix == "#light") {
                        CUIWindowEditLight *editLight = new CUIWindowEditLight(devices, mainWindowInstance->getActiveListBox()->getSelected());
						editLight->open(mainWindowInstance->getSelectedNode().getNode(),
										mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode()));
                    }
                }
                    break;
                    
                case CXT_MENU_EVENTS_EDIT_EDIT_MATERIALS_SELECTED_NODE: {
					stringc prefix = mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode());
                    if (prefix == "#map" || prefix == "#tree" || prefix == "#object") {
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
                //-----------------------------------
                
                //-----------------------------------
                //CONTEXT MENU VIEW EVENT
                case CXT_MENU_EVENTS_VIEW_MAYA_CAMERA:
                    devices->getSceneManager()->setActiveCamera(devices->getMayaCamera());
                    break;
                    
                case CXT_MENU_EVENTS_VIEW_FPS_CAMERA:
                    devices->getCollisionManager()->createAnimatorCollisionCamera(devices->getFPSCamera());
                    devices->getSceneManager()->setActiveCamera(devices->getFPSCamera());
					devices->getDevice()->getCursorControl()->setVisible(false);
                    break;
                    
                case CXT_MENU_EVENTS_VIEW_VIEW_TREE_NODES_WINDOW:
                    sceneViewInstance->open();
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
                case CXT_MENU_EVENTS_RENDERS_MOTION_BLUR_DRAW:
					devices->getXEffect()->setUseMotionBlur(!devices->getXEffect()->isUsingMotionBlur());
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
					if (mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode()) == "#object") {
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
                //CONTEXT MENU MESH FACTORY EVENT
				case CXT_MENU_EVENTS_ADD_CUBE_SCENE_NODE: {
					IMeshSceneNode *cube = devices->getSceneManager()->addCubeSceneNode();
					cube->setName("#object:new_cube");
					cube->setMaterialFlag(EMF_LIGHTING, false);
					devices->getXEffect()->addShadowToNode(cube);
					SObjectsData odata(cube->getMesh(), cube, "cube");
					devices->getCoreData()->getObjectsData()->push_back(odata);
					devices->getCollisionManager()->setCollisionFromBoundingBox(cube);
				}
					break;

				case CXT_MENU_EVENTS_ADD_SPHERE_SCENE_NODE: {
					IMeshSceneNode *sphere = devices->getSceneManager()->addSphereSceneNode();
					sphere->setName("#object:new_sphere");
					sphere->setMaterialFlag(EMF_LIGHTING, false);
					devices->getXEffect()->addShadowToNode(sphere);
					SObjectsData odata(sphere->getMesh(), sphere, "sphere");
					devices->getCoreData()->getObjectsData()->push_back(odata);
					devices->getCollisionManager()->setCollisionFromBoundingBox(sphere);
				}
					break;

				case CXT_MENU_EVENTS_ADD_HILL_PLANE_MESH: {
					IAnimatedMesh *planeMesh = devices->getSceneManager()->addHillPlaneMesh("#object:new_hille_plane_mesh", 
																					dimension2df(25, 25), 
																					dimension2du(25, 25));
					ISceneNode *planeNode = devices->getSceneManager()->addMeshSceneNode(planeMesh->getMesh(0), 0, -1, vector3df(0, 0, 0));
					planeNode->setName("#object:new_hille_plane_mesh");
					planeNode->setMaterialFlag(EMF_LIGHTING, false);
					devices->getXEffect()->addShadowToNode(planeNode, devices->getXEffectFilterType(), ESM_EXCLUDE);
					SObjectsData odata(planeMesh, planeNode, "hillPlaneMesh");
					devices->getCoreData()->getObjectsData()->push_back(odata);
					devices->getCollisionManager()->setCollisionFromBoundingBox(planeNode);
				}
					break;

				case CXT_MENU_EVENTS_ADD_BILL_BOARD: {
					IBillboardSceneNode *billboard = devices->getSceneManager()->addBillboardSceneNode();
					billboard->setName("#object:new_bill_board");
					billboard->setMaterialFlag(EMF_LIGHTING, false);
					devices->getXEffect()->addShadowToNode(billboard, devices->getXEffectFilterType(), ESM_EXCLUDE);
					SObjectsData odata(0, billboard, "billboard");
					devices->getCoreData()->getObjectsData()->push_back(odata);
				}
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

				case CXT_MENU_EVENTS_NODE_FACTORY_EDIT_SKYDOME: {
					if (devices->getSkydome()) {
						CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
						editNode->open(devices->getSkydome(), "#object", false);
					}
				}

					break;

				case CXT_MENU_EVENTS_NODE_FACTORY_EDIT_MATERIALS_SKYDOME: {
					CUIMaterialEditor *matEditor = new CUIMaterialEditor(devices);
					matEditor->open(devices->getSkydome());
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
			IGUIElement *parent = devices->getGUIEnvironment()->getFocus();
			bool isAListBoxOfMainWindow = (parent == mainWindowInstance->getActiveListBox() && mainWindowInstance->getActiveListBox()->getItemCount() != 0);
			if (isAListBoxOfMainWindow && devices->getObjectPlacement()->getArrowType() != CCoreObjectPlacement::Undefined) {
				CCoreObjectPlacement *cobj = devices->getObjectPlacement();
				/*if (cobj->isPlacing() && cobj->getArrowType() == movementType) {
					cobj->setCollisionToNormal();
					cobj->setNodeToPlace(0);
					cobj->setArrowType(CCoreObjectPlacement::Undefined);
					cobj->setArrowVisible(false);
				} else {*/
					cobj->setNodeToPlace(mainWindowInstance->getSelectedNode().getNode());
					if (!cobj->getNodeToPlace()) {
						devices->addWarningDialog(L"Warning", L"Please Select A Node Before...", EMBF_OK);
					} else {
						cobj->setArrowType(movementType);
						cobj->setArrowVisible(true);
					}
				//}
			}
		}
        
        //-----------------------------------
        
        //-----------------------------------
        //BUTTON GUI EVENT
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            
			//POSITION
			if (event.GUIEvent.Caller == ibposition) {
				if (ibposition->isPressed()) {
					movementType = CCoreObjectPlacement::Position;
					devices->getObjectPlacement()->setArrowType(movementType);
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
					devices->getObjectPlacement()->setArrowVisible(true);
				}
				ibposition->setPressed(false);
				ibrotation->setPressed(false);
			}

			//IF NO BUTTON PRESSED
			if (!ibposition->isPressed() && !ibrotation->isPressed() && !ibscale->isPressed()) {
				devices->getObjectPlacement()->setArrowType(CCoreObjectPlacement::Undefined);
				devices->getObjectPlacement()->setArrowVisible(false);
			}

            switch (id) {
                //-----------------------------------
                //TOOLBAR
                case CXT_MENU_EVENTS_OPEN_SCRIPT:
                    openSceneInstance->open();
                    break;
                    
                case CXT_MENU_EVENTS_EDIT_NODE: {
					stringc prefix = mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode());
                    if (prefix == "#map" || prefix == "#tree" || prefix == "#object" || prefix == "#water") {
                        CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
						editNode->open(mainWindowInstance->getSelectedNode().getNode(), 
									  mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode()));
                    } else if (prefix == "#light") {
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
                    CUIWindowEditEffects *editEffects = new CUIWindowEditEffects(devices);
					editEffects->open();
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
                    if (!devices->isEditBoxEntered() && devices->isCtrlPushed() && devices->isShiftPushed()) {
                        devices->setXEffectDrawable(!devices->isXEffectDrawable());
                    }
                    break;
                    
                case KEY_KEY_W: {
                    if (!devices->isEditBoxEntered() && devices->isCtrlPushed()) {
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
                }
                    break;
                    
                case KEY_KEY_A:
                    if (!devices->isEditBoxEntered() && devices->isCtrlPushed()) {
						if (mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode()) == "#object") {
							if (mainWindowInstance->getSelectedNode().getNode()) {
								CUICharacterWindow *characterWindowInstance = new CUICharacterWindow(devices);
                                characterWindowInstance->open();
								characterWindowInstance->setModel((IAnimatedMeshSceneNode *)mainWindowInstance->getSelectedNode().getNode(), mainWindowInstance->getActiveListBox()->getSelected());
                            }
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
                        if (prefix == "#map" || prefix == "#tree" || prefix == "#object") {
                            CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
							editNode->open(mainWindowInstance->getSelectedNode().getNode(), 
										   mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode()));
                        } else if (prefix == "#light") {
                            CUIWindowEditLight *editLight = new CUIWindowEditLight(devices, mainWindowInstance->getActiveListBox()->getSelected());
							editLight->open(mainWindowInstance->getSelectedNode().getNode(),
											mainWindowInstance->getSelectedNodePrefix(mainWindowInstance->getSelectedNode().getNode()));
                        }
                    }
					if (!devices->isEditBoxEntered() && devices->isCtrlPushed() && devices->isShiftPushed()) {
						ISceneNode *node = mainWindowInstance->getSelectedNode().getNode();
						bool canEdit = (node->getType() == ESNT_MESH || node->getType() == ESNT_OCTREE || node->getType() == ESNT_ANIMATED_MESH
							|| node->getType() == ESNT_BILLBOARD || node->getType() == ESNT_SKY_BOX || node->getType() == ESNT_SKY_DOME
							|| node->getType() == ESNT_TERRAIN || node->getType() == ESNT_CUBE || node->getType() == ESNT_SPHERE);
						if (canEdit) {
							CUIMaterialEditor *editMat = new CUIMaterialEditor(devices);
							editMat->open(node);
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
