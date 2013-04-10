//
//  CUIRenderingInfos.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#include "CUIRightSceneTreeView.h"

CUIRightSceneTreeView::CUIRightSceneTreeView(CDevices *_devices) {
	devices = _devices;
	worldCore = devices->getCoreData();
	driver = devices->getVideoDriver();

	//SET UP VALUES TO 0
	terrainsNode = 0;
	treesNode = 0;
	objectsNode = 0;
	lightsNode = 0;

	IGUIEnvironment *gui = devices->getGUIEnvironment();
	//SET UP WINDOW
	window = gui->addWindow(rect<s32>(0, 0, 0, 0), false, L"Scene View", 0, -1);
	window->getCloseButton()->remove();
	window->setDraggable(false);
	window->setVisible(false);

	//SET UP TREE VIEW
	sceneView = gui->addTreeView(rect<s32>(0, 0, 0, 0), window, -1, true, true, false);
	rootNode = sceneView->getRoot()->addChildBack(L"Scene", L"-", 1, -1);
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

}

CUIRightSceneTreeView::~CUIRightSceneTreeView() {

}

void CUIRightSceneTreeView::addChildrenBackWithArray(IGUITreeViewNode *treeNode, array<ISceneNode *> *nodes) {
	IGUITreeViewNode *nextNode = treeNode->getFirstChild();
	IGUITreeViewNode *nextNextNode;

	for (u32 i=0; i < nodes->size(); i++) {

		if (nextNode) {
			if (i > 0) {
				nextNode = nextNode->getNextSibling();
			}
			ISceneNode *dataNode = (ISceneNode *)nextNode->getData();
			if (devices->getCore()->nodeExistsInArray(*nodes, dataNode) != -1) {
				nextNode->setText(stringw(nodes->operator[](i)->getName()).c_str());
				addChildrenBackRecursively(nextNode, nodes->operator[](i));
			} else {
				nextNextNode = nextNode->getPrevSibling();
				if (!nextNextNode) {
					nextNextNode = 0;
				}
				treeNode->deleteChild(nextNode);
				nextNode = nextNextNode;
			}
		} else {
			IGUITreeViewNode *newTreeNode = treeNode->addChildBack(stringw(nodes->operator[](i)->getName()).c_str(), L"", 
																   getImageListIndexForNodeType(nodes->operator[](i)->getType()), -1);
			newTreeNode->setData(nodes->operator[](i));
			addChildrenBackRecursively(newTreeNode, nodes->operator[](i));
		}
	}
}

void CUIRightSceneTreeView::addChildrenBackRecursively(IGUITreeViewNode *treeNode, ISceneNode *node) {

	s32 imageIndex;
	IGUITreeViewNode *nodeTreeView = treeNode->getFirstChild();
	IGUITreeViewNode *nextNodeTreeView;
	u32 i=0, j=0;

	core::list<ISceneNode *>::ConstIterator it = node->getChildren().begin();
	for (; it != node->getChildren().end(); ++it) {
		imageIndex = getImageListIndexForNodeType((*it)->getType());
		
		if (i > 0 && nodeTreeView) {
			nodeTreeView = nodeTreeView->getNextSibling();
		}
		i++;

		if (!nodeTreeView) {
			nodeTreeView = treeNode->addChildBack(stringw((*it)->getName()).c_str(), L"", imageIndex);
			nodeTreeView->setData(*it);
		} else {
			CCore *core = devices->getCore();
			ISceneNode *dataNode = (ISceneNode *)nodeTreeView->getData();
			if (core->nodeExistsInArray(*core->getArrayOfAListOfNodeChildren(node), dataNode) == -1) {
				nextNodeTreeView = nodeTreeView->getPrevSibling();
				if (!nextNodeTreeView) {
					nextNodeTreeView = 0;
				}
				treeNode->deleteChild(nodeTreeView);
				nodeTreeView = nextNodeTreeView;
			}
		}
		
		/*core::list<ISceneNodeAnimator*>::ConstIterator ait = (*it)->getAnimators().begin();
		for (; ait != (*it)->getAnimators().end(); ++ait) {
			imageIndex = -1;
			
			switch ((*ait)->getType()) {
				case ESNAT_FLY_CIRCLE:
				case ESNAT_FLY_STRAIGHT:
				case ESNAT_FOLLOW_SPLINE:
				case ESNAT_ROTATION:
				case ESNAT_TEXTURE:
				case ESNAT_DELETION:
				case ESNAT_COLLISION_RESPONSE:
				case ESNAT_CAMERA_FPS:
				case ESNAT_CAMERA_MAYA:
				default:break;
			}
			nodeTreeView->addChildBack(stringw(devices->getDevice()->getSceneManager()->getAnimatorTypeName((*ait)->getType())).c_str(), 0, imageIndex);
			nodeTreeView->setData(*ait);
		}*/
		stringc nameNodeTreeView = "";
		nameNodeTreeView += (*it)->getName();
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
		default:index = -1; break;
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
		addChildrenBackWithArray(treesNode, worldCore->getTreeNodes());
		addChildrenBackWithArray(objectsNode, worldCore->getObjectNodes());
		addChildrenBackWithArray(lightsNode, worldCore->getLightsNodes());
	}

	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == EMIE_MOUSE_MOVED) {
			if (window->isVisible()) {
				if (devices->getDevice()->getCursorControl()->getPosition().X < driver->getScreenSize().Width-window->getRelativePosition().getWidth()) {
					window->setVisible(false);
				}
			} else {
				if (devices->getDevice()->getCursorControl()->getPosition().Y > 75) {
					if (devices->getDevice()->getCursorControl()->getPosition().X >= driver->getScreenSize().Width-10) {
						window->setVisible(true);
					}
				}
			}
		}
	}

	return false;
}
