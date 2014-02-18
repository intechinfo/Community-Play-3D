//
//  CUIRenderingInfos.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//
#include "stdafx.h"
#include "CUIRightSceneTreeView.h"

CUIRightSceneTreeView::CUIRightSceneTreeView(CDevices *_devices, CUIContextMenu *_cxtMenu) {
	devices = _devices;
	cxtMenu = _cxtMenu;

	worldCore = devices->getCoreData();
	core = devices->getCore();

	driver = devices->getVideoDriver();

	//SET UP VALUES TO 0
	terrainsNode = 0;
	treesNode = 0;
	objectsNode = 0;
	lightsNode = 0;
	whoIstreeNodeSelected = 0;

	rightClickCxtMenu = 0;

	alwaysVisible = false;

	IGUIEnvironment *gui = devices->getGUIEnvironment();
	//SET UP WINDOW
	window = gui->addWindow(rect<s32>(0, 0, 0, 0), false, L"Scene View", 0, -1);
	window->getCloseButton()->remove();
	window->getMaximizeButton()->setVisible(true);
	window->setDraggable(false);
	window->setVisible(false);

	//SET UP TREE VIEW
	sceneView = gui->addTreeView(rect<s32>(0, 0, 0, 0), window, -1, true, true, false);
	rootNode = sceneView->getRoot()->addChildBack(L"SCENE", L"SCENE", 1, -1);
	rootNode->setExpanded(true);
	IGUIImageList* imageList = devices->getGUIEnvironment()->createImageList(driver->getTexture("GUI/iconlist_small.png"), dimension2d<s32>(16, 16), true);
	if (imageList) {
		sceneView->setImageList(imageList);
		imageList->drop();
	}

	//SET UP TREE VIEW NODES
	terrainsNode = rootNode->addChildBack(L"TERRAINS", L"", 2, -1);
	treesNode = rootNode->addChildBack(L"TREES", L"", 2, -1);
	objectsNode = rootNode->addChildBack(L"OBJECTS", L"", 2, -1);
	lightsNode = rootNode->addChildBack(L"LIGHTS", L"", 2, -1);
	volumeLightsNode = rootNode->addChildBack(L"VOLUME LIGHTS", L"", 2, -1);
	waterSurfacesNode = rootNode->addChildBack(L"WATER SURFACES", L"", 2, -1);
    particlesNode = rootNode->addChildBack(L"PARTICLE SYSTEMS", L"", 2, -1);
	rootNode->addChildBack(L"OTHERS (TO DO)", L"", 2, -1);

	addChildrenBackWithSDataArray(terrainsNode, worldCore->getTerrainsSData());
	addChildrenBackWithSDataArray(treesNode, worldCore->getTreesSData());
	addChildrenBackWithSDataArray(objectsNode, worldCore->getObjectsSData());
	addChildrenBackWithSDataArray(lightsNode, worldCore->getLightsSData());
	addChildrenBackWithSDataArray(volumeLightsNode, worldCore->getVolumeLightsSData());
	addChildrenBackWithSDataArray(waterSurfacesNode, worldCore->getWaterSurfacesSData());
    //addChildrenBackWithSDataArray(particlesNode, worldCore->getParticleSystemsSData());


}

CUIRightSceneTreeView::~CUIRightSceneTreeView() {

}

void CUIRightSceneTreeView::addChildrenBackWithArray(IGUITreeViewNode *treeNode, array<ISceneNode *> *nodes) {
	IGUITreeViewNode *nextNode = treeNode->getFirstChild();
	IGUITreeViewNode *nextNextNode;
	if (sceneView->getSelected()) {
		whoIstreeNodeSelected = (ISceneNode *)sceneView->getSelected()->getData();
	}
	treeNode->clearChildren();
	for (u32 i=0; i < nodes->size(); i++) {
		IGUITreeViewNode *newTreeNode = treeNode->addChildBack(stringw(nodes->operator[](i)->getName()).c_str(), L"", 
															   getImageListIndexForNodeType(nodes->operator[](i)->getType()), -1);
		newTreeNode->setData(nodes->operator[](i));
		newTreeNode->setExpanded(true);
		if (whoIstreeNodeSelected == nodes->operator[](i)) {
			newTreeNode->setSelected(true);
		}
		addChildrenBackRecursively(newTreeNode, nodes->operator[](i));
	}
}

void CUIRightSceneTreeView::addChildrenBackRecursively(IGUITreeViewNode *treeNode, ISceneNode *node) {

	s32 imageIndex;
	IGUITreeViewNode *nodeTreeView;
	treeNode->clearChildren();

	core::list<ISceneNode *>::ConstIterator it = node->getChildren().begin();
	for (; it != node->getChildren().end(); ++it) {
		imageIndex = getImageListIndexForNodeType((*it)->getType());
		
		nodeTreeView = treeNode->addChildBack(stringw((*it)->getName()).c_str(), 0, imageIndex);
		nodeTreeView->setData(*it);

		if (whoIstreeNodeSelected) {
			if (*it == whoIstreeNodeSelected) {
				nodeTreeView->setSelected(true);
			}
		}

		nodeTreeView->setExpanded(true);
		
		core::list<ISceneNodeAnimator*>::ConstIterator ait = (*it)->getAnimators().begin();
		for (; ait != (*it)->getAnimators().end(); ++ait) {
			imageIndex = 10;

			nodeTreeView->addChildBack(stringw(devices->getDevice()->getSceneManager()->getAnimatorTypeName((*ait)->getType())).c_str(), 0, imageIndex);
			nodeTreeView->setData(*ait);
		}

        addChildrenBackRecursively(nodeTreeView, *(it));
	}
}

void CUIRightSceneTreeView::addChildrenBackWithSDataArray(IGUITreeViewNode *treeNode, array<SData> *nodes) {
	IGUITreeViewNode *nextNode = treeNode->getFirstChild();
	IGUITreeViewNode *nextNextNode;
	if (sceneView->getSelected())
		whoIstreeNodeSelected = (ISceneNode *)sceneView->getSelected()->getData();

	treeNode->clearChildren();

	for (u32 i=0; i < nodes->size(); i++) {
		ISceneNode *nnode = nodes->operator[](i).getNode();
		if (nnode) {
			IGUITreeViewNode *newTreeNode = treeNode->addChildBack(stringw(nodes->operator[](i).getNode()->getName()).c_str(), L"", 
																   getImageListIndexForNodeType(nodes->operator[](i).getType()), -1);
			newTreeNode->setData(nodes->operator[](i).getNode());
			newTreeNode->setExpanded(true);
			if (whoIstreeNodeSelected == nodes->operator[](i).getNode()) {
				newTreeNode->setSelected(true);
			}
			addChildrenBackSDataRecursively(newTreeNode, nodes->operator[](i));
		}
	}
}
void CUIRightSceneTreeView::addChildrenBackSDataRecursively(IGUITreeViewNode *treeNode, SData node) {
	s32 imageIndex;
	IGUITreeViewNode *nodeTreeView;
	treeNode->clearChildren();

	if (node.getClonedNodeCount() > 0) {
		IGUITreeViewNode *clonedNodeTreeNode = treeNode->addChildBack(L"Clones", L"", getImageListIndexForNodeType(ESNT_UNKNOWN), -1, node.getNode());
		clonedNodeTreeNode->setExpanded(true);
		ISceneNodeList::Iterator it = node.getClonedNodeList()->begin();
		for (; it != node.getClonedNodeList()->end(); ++it) {
			IGUITreeViewNode *newNode = clonedNodeTreeNode->addChildBack(stringw((*it)->getName()).c_str(), L"",
																		 getImageListIndexForNodeType((*it)->getType()), -1, (ISceneNode*)(*it));
		}
	}

	core::list<ISceneNode *>::ConstIterator it = node.getNode()->getChildren().begin();
	for (; it != node.getNode()->getChildren().end(); ++it) {
		if ((*it)->getReferenceCount() == 0)
			continue;

		imageIndex = getImageListIndexForNodeType((*it)->getType());
		
		nodeTreeView = treeNode->addChildBack(stringw((*it)->getName()).c_str(), 0, imageIndex);
		nodeTreeView->setData(*it);

		if (whoIstreeNodeSelected) {
			if (*it == whoIstreeNodeSelected) {
				nodeTreeView->setSelected(true);
			}
		}

		nodeTreeView->setExpanded(true);
		
		core::list<ISceneNodeAnimator*>::ConstIterator ait = (*it)->getAnimators().begin();
		for (; ait != (*it)->getAnimators().end(); ++ait) {
			imageIndex = 10;

			nodeTreeView->addChildBack(stringw(devices->getDevice()->getSceneManager()->getAnimatorTypeName((*ait)->getType())).c_str(), 0, imageIndex);
			nodeTreeView->setData(*ait);
		}

        addChildrenBackRecursively(nodeTreeView, *(it));
	}
}

s32 CUIRightSceneTreeView::getImageListIndexForNodeType(ESCENE_NODE_TYPE type) {
	s32 index;
	switch (type) {
		case ESNT_Q3SHADER_SCENE_NODE: index = 0; break;
		case ESNT_CAMERA: index = 1; break;
		case ESNT_EMPTY: index = 2; break;
		case ESNT_MESH: index = 3; break;
		case ESNT_OCTREE: index = 3; break;
		case ESNT_ANIMATED_MESH: index = 4; break;
		case ESNT_SPHERE: index = 5; break;
		case ESNT_SKY_BOX: index = 5; break;
		case ESNT_BILLBOARD: index = 6; break;
		case ESNT_PARTICLE_SYSTEM: index = 7; break;
		case ESNT_TEXT: index = 8; break;
		case ESNT_LIGHT: index = 9; break;
		default:index = 10; break;
	}

	return index;
}

bool CUIRightSceneTreeView::OnEvent(const SEvent &event) {

	if (window->isVisible()) {
		window->setRelativePosition(rect<s32>(driver->getScreenSize().Width - 400, 75, 
											  driver->getScreenSize().Width, 
											  driver->getScreenSize().Height-20));
		sceneView->setRelativePosition(rect<s32>(10, 25, 
												 window->getRelativePosition().getWidth()-10, 
												 window->getRelativePosition().getHeight()-10));
	}

	if (event.EventType == EET_USER_EVENT) {
		if (event.UserEvent.UserData1 == ECUE_NODE_REMOVED || event.UserEvent.UserData1 == ECUE_NODE_ADDED) {
			addChildrenBackWithSDataArray(terrainsNode, worldCore->getTerrainsSData());
			addChildrenBackWithSDataArray(treesNode, worldCore->getTreesSData());
			addChildrenBackWithSDataArray(objectsNode, worldCore->getObjectsSData());
			addChildrenBackWithSDataArray(lightsNode, worldCore->getLightsSData());
			addChildrenBackWithSDataArray(volumeLightsNode, worldCore->getVolumeLightsSData());
			addChildrenBackWithSDataArray(waterSurfacesNode, worldCore->getWaterSurfacesSData());
		}
	}

	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP && devices->getGUIEnvironment()->getFocus() == 0) {
			if (window->isVisible() && window->isPointInside(devices->getDevice()->getCursorControl()->getPosition())) {
				IGUIEnvironment *gui = devices->getGUIEnvironment();
				ICursorControl *cursor = devices->getDevice()->getCursorControl();

				rect<s32> rectMenu = rect<s32>(cursor->getPosition().X, cursor->getPosition().Y, 
											   cursor->getPosition().X+300, cursor->getPosition().Y+200);
				rightClickCxtMenu = gui->addContextMenu(rectMenu, 0, -1);
				rightClickCxtMenu->addItem(L"Edit...", 0, true, false, false, false);
				rightClickCxtMenu->addItem(L"Edit Materials...", 1, true, false, false, false);
				rightClickCxtMenu->addSeparator();
				rightClickCxtMenu->addItem(L"Clone", 2, true, false, false, false);
				rightClickCxtMenu->addSeparator();
				rightClickCxtMenu->addItem(L"Make Planar Texture Mapping...", 3, true, false, false, false);
				rightClickCxtMenu->addItem(L"Create Mesh With Tangents...", 4, true, false, false, false);
				rightClickCxtMenu->addItem(L"Scale Mesh", 5, true, false, false, false);
				rightClickCxtMenu->addSeparator();
				rightClickCxtMenu->addItem(L"Apply Physic", 6, true, false, false, true);
				rightClickCxtMenu->addItem(L"Generate LightMaps...", 7, true, false, false, true);
				rightClickCxtMenu->addSeparator();
				rightClickCxtMenu->addItem(L"Remove", 8, true, false, false, true);
			}
		}

		if (event.MouseInput.Event == EMIE_MOUSE_MOVED) {
			if (window->isVisible()) {
				if (devices->getDevice()->getCursorControl()->getPosition().X < driver->getScreenSize().Width-window->getRelativePosition().getWidth()) {
					if (!alwaysVisible) {
						window->setVisible(false);
					}
				}
			} else {
				if (devices->getDevice()->getCursorControl()->getPosition().Y > 75) {
					if (devices->getDevice()->getCursorControl()->getPosition().X >= driver->getScreenSize().Width-1) {
						window->setVisible(true);
						window->getParent()->bringToFront(window);
					}
				}
			}
		}
	}

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_TREEVIEW_NODE_SELECT) {
			if (event.GUIEvent.Caller == sceneView) {
				ISceneNode *node = (ISceneNode *)sceneView->getSelected()->getData();
				if (node) {
					cxtMenu->getMainWindow()->selectSelectedNode(node);
					devices->getObjectPlacement()->setNodeToPlace(node);
				}
			}
		}

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == window->getMaximizeButton()) {
				alwaysVisible = true;
			}
			if (event.GUIEvent.Caller == window->getMinimizeButton()) {
				alwaysVisible = false;
			}
			window->getMaximizeButton()->setVisible(!alwaysVisible);
			window->getMinimizeButton()->setVisible(alwaysVisible);
		}

		if (event.GUIEvent.EventType == EGET_MENU_ITEM_SELECTED) {
			if (event.GUIEvent.Caller == rightClickCxtMenu) {
				ISceneNode *node = (ISceneNode *)sceneView->getSelected()->getData();
				if (node) {
					if (rightClickCxtMenu->getItemCommandId(rightClickCxtMenu->getSelectedItem()) == 0) {
						stringc prefix = cxtMenu->getMainWindow()->getSelectedNodePrefix(node);
						if (prefix == "#light" && node->getType() == ESNT_LIGHT) {
                            array<ISceneNode *> lights = worldCore->getArrayOfLightNodes();
                            #ifndef _IRR_OSX_PLATFORM_
							CUIWindowEditLight *editLight = new CUIWindowEditLight(devices, core->nodeExistsInArray(&lights, node));
                            #else
                            CUIWindowEditLight *editLight = new CUIWindowEditLight(devices, core->nodeExistsInArray(lights, node));
                            #endif
							editLight->open(node, prefix.c_str());
						} else {
							CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
							editNode->open(node, prefix.c_str(), false);
						}
					}

					if (rightClickCxtMenu->getItemCommandId(rightClickCxtMenu->getSelectedItem()) == 1) {
						if (node->getType() != ESNT_LIGHT) {
							CUIMaterialEditor *matEditor = new CUIMaterialEditor(devices);
							matEditor->open(node);
						} else {
							devices->addInformationDialog(L"Informations", L"You cannot edit materials of a light", EMBF_OK, 0);
						}
					}

					if (rightClickCxtMenu->getItemCommandId(rightClickCxtMenu->getSelectedItem()) == 2) {
						if (cxtMenu->getMainWindow()->getSelectedNode().getNode() == node) {
							cxtMenu->getMainWindow()->cloneNode();
						} else {
							devices->addInformationDialog(L"Informations", L"Clone is forbidden...", EMBF_OK, 0);
						}
					}

					if (rightClickCxtMenu->getItemCommandId(rightClickCxtMenu->getSelectedItem()) == 3) {
						if (node->getType() != ESNT_LIGHT) {
							SSelectedNode ssn = cxtMenu->getMainWindow()->getSelectedNode();
							if (ssn.getMesh() && ssn.getNode()) {
								CUINodeFactoryPlanarMapping *cfpm = new CUINodeFactoryPlanarMapping(devices);
								cfpm->open(ssn.getNode(), ssn.getMesh());
							} else {
								devices->addInformationDialog(L"Informations", L"Node not found", EMBF_OK, 0);
							}
						} else {
							devices->addInformationDialog(L"Informations", L"You cannot use a light", EMBF_OK, 0);
						}
					}

					if (rightClickCxtMenu->getItemCommandId(rightClickCxtMenu->getSelectedItem()) == 4) {
						if (node->getType() != ESNT_LIGHT) {
							SSelectedNode ssn = cxtMenu->getMainWindow()->getSelectedNode();
							if (ssn.getMesh() && ssn.getNode()) {
								CUINodeFactoryCreateMeshWithTangents *cmwt = new CUINodeFactoryCreateMeshWithTangents(devices);
								cmwt->open(ssn.getNode(), ssn.getMesh());
							} else {
								devices->addInformationDialog(L"Informations", L"Node not found", EMBF_OK, 0);
							}
						} else {
							devices->addInformationDialog(L"Informations", L"You cannot use a light", EMBF_OK, 0);
						}
					}

					if (rightClickCxtMenu->getItemCommandId(rightClickCxtMenu->getSelectedItem()) == 5) {

					}

					if (rightClickCxtMenu->getItemCommandId(rightClickCxtMenu->getSelectedItem()) == 7) {
						if (node->getType() == ESNT_MESH || node->getType() == ESNT_OCTREE) {
							SSelectedNode ssn = cxtMenu->getMainWindow()->getSelectedNode();
							if (ssn.getMesh() && ssn.getNode()) {
								u32 nindice = devices->getCoreData()->getTerrainNodeIndice(ssn.getNode());
								CUILightsMapCreator *gene = new CUILightsMapCreator(devices, &devices->getCoreData()->getTerrainsData()->operator[](nindice));
							}
						}
					}
                    
                    if (rightClickCxtMenu->getItemCommandId(rightClickCxtMenu->getSelectedItem()) == 8) {
                        if (sceneView->getSelected()->getData()) {
                            devices->getCoreData()->removeSceneNode((ISceneNode *)sceneView->getSelected()->getData(), devices->getXEffect());
                            devices->getEventReceiver()->sendUserEvent(ECUE_NODE_REMOVED);
                        } else {
                            devices->addInformationDialog(L"Informations", L"Node not found", EMBF_OK, 0);
                        }
                    }
				} else {
					devices->addInformationDialog(L"Informations", L"You cannot use this node", EMBF_OK, 0);
				}
				rightClickCxtMenu = 0;
			}
		}
	}

	return false;
}
