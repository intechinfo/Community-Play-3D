//
//  CUIRenderingInfos.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

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
	//TERRAINS
	terrainsNode = rootNode->addChildBack(L"TERRAINS", L"", 2, -1);
	treesNode = rootNode->addChildBack(L"TREES", L"", 2, -1);
	objectsNode = rootNode->addChildBack(L"OBJECTS", L"", 2, -1);
	lightsNode = rootNode->addChildBack(L"LIGHTS", L"", 2, -1);
	volumeLightsNode = rootNode->addChildBack(L"VOLUME LIGHTS", L"", 2, -1);
	waterSurfacesNode = rootNode->addChildBack(L"WATER SURFACES", L"", 2, -1);

}

CUIRightSceneTreeView::~CUIRightSceneTreeView() {

}

void CUIRightSceneTreeView::addChildrenBackWithArray(IGUITreeViewNode *treeNode, array<ISceneNode *> *nodes) {
	IGUITreeViewNode *nextNode = treeNode->getFirstChild();
	IGUITreeViewNode *nextNextNode;
	if (sceneView->getSelected()) {
		whoIstreeNodeSelected = (ISceneNode *)sceneView->getSelected()->getData();
	}
	treeNode->clearChilds();
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
	treeNode->clearChilds();

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
		window->setRelativePosition(rect<s32>(driver->getScreenSize().Width - 500, 75, 
											  driver->getScreenSize().Width, 
											  driver->getScreenSize().Height));
		sceneView->setRelativePosition(rect<s32>(10, 25, 
												 window->getRelativePosition().getWidth()-10, 
												 window->getRelativePosition().getHeight()-10));
		addChildrenBackWithArray(terrainsNode, worldCore->getTerrainNodes());
		addChildrenBackWithArray(treesNode, &worldCore->getArrayOfTreeNodes());
		addChildrenBackWithArray(objectsNode, &worldCore->getArrayOfObjectNodes());
		addChildrenBackWithArray(lightsNode, &worldCore->getArrayOfLightNodes());
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
							CUIWindowEditLight *editLight = new CUIWindowEditLight(devices, core->nodeExistsInArray(&worldCore->getArrayOfLightNodes(), node));
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
				} else {
					devices->addInformationDialog(L"Informations", L"You cannot use this node", EMBF_OK, 0);
				}
				rightClickCxtMenu = 0;
			}
		}
	}

	return false;
}
