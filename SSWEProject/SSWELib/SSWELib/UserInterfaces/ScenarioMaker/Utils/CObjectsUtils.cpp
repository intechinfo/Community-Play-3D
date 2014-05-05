//
//  CUIRenderingInfos.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//
#include "stdafx.h"
#include "CObjectsUtils.h"

CObjectsUtils::CObjectsUtils(CDevices *_devices, IGUITreeView *_treeView) {
	devices = _devices;
	treeView = _treeView;

	gui = devices->getGUIEnvironment();

	createNodes();
}

CObjectsUtils::~CObjectsUtils() {

}

void CObjectsUtils::createNodes() {
	IGUIImageList* imageList = gui->createImageList(devices->getVideoDriver()->getTexture(devices->getWorkingDirectory() + "GUI/iconlist_small.png"),
													dimension2d<s32>(16, 16), true);
	if (imageList) {
		treeView->setImageList(imageList);
		imageList->drop();
	}

	rootNode = treeView->getRoot();
	terrainsNode = rootNode->addChildBack(L"TERRAINS", L"", 2, -1);
	objectsNode = rootNode->addChildBack(L"OBJECTS", L"", 2, -1);
	lightsNode = rootNode->addChildBack(L"LIGHTS", L"", 2, -1);
	volumeLightsNode = rootNode->addChildBack(L"VOLUME LIGHTS", L"", 2, -1);
	waterSurfacesNode = rootNode->addChildBack(L"WATER SURFACES", L"", 2, -1);

	fill();
}

void CObjectsUtils::fill() {
	fillTerrains();
	fillObjects();
	filleLights();
	fillVolumeLights();
	fillWaterSurfaces();
}

void CObjectsUtils::fillTerrains() {
	terrainsNode->clearChildren();
	addChildrenBackWithSDataArray(terrainsNode, devices->getCoreData()->getTerrainsSData());
}

void CObjectsUtils::fillObjects() {
	objectsNode->clearChildren();
	addChildrenBackWithSDataArray(objectsNode, devices->getCoreData()->getObjectsSData());
}
void CObjectsUtils::filleLights() {
	lightsNode->clearChildren();
	addChildrenBackWithSDataArray(lightsNode, devices->getCoreData()->getLightsSData());
}
void CObjectsUtils::fillVolumeLights() {
	volumeLightsNode->clearChildren();
	addChildrenBackWithSDataArray(volumeLightsNode, devices->getCoreData()->getVolumeLightsSData());
}
void CObjectsUtils::fillWaterSurfaces() {
	waterSurfacesNode->clearChildren();
	addChildrenBackWithSDataArray(waterSurfacesNode, devices->getCoreData()->getWaterSurfacesSData());
}

void CObjectsUtils::addChildrenBackWithSDataArray(IGUITreeViewNode *treeNode, array<SData> *nodes) {
	IGUITreeViewNode *nextNode = treeNode->getFirstChild();
	IGUITreeViewNode *nextNextNode;
	if (treeView->getSelected())
		whoIstreeNodeSelected = (ISceneNode *)treeView->getSelected()->getData();

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
void CObjectsUtils::addChildrenBackSDataRecursively(IGUITreeViewNode *treeNode, SData node) {
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
void CObjectsUtils::addChildrenBackRecursively(IGUITreeViewNode *treeNode, ISceneNode *node) {

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
s32 CObjectsUtils::getImageListIndexForNodeType(ESCENE_NODE_TYPE type) {
	s32 index = 10;
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
