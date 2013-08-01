//
//  CCollisionManager.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 01/10/12.
//
//

#include "stdafx.h"
#include "CCollisionManager.h"

CCollisionManager::CCollisionManager(ISceneManager *_smgr) {
    smgr = _smgr;

    meta = smgr->createMetaTriangleSelector();

	fpsCameraSettings = new SFPSCameraSettings();
}

CCollisionManager::~CCollisionManager() {
	meta->removeAllTriangleSelectors();
	meta->drop();
}

void CCollisionManager::setCollisionToAnUnknownNode(IMesh *mesh, ISceneNode *node) {
	ITriangleSelector *selector = 0;
	node->setTriangleSelector(selector);
    meta->addTriangleSelector(selector);
	selector->drop();
}

ITriangleSelector *CCollisionManager::setCollisionToAnOctTreeNode(ISceneNode *node) {
	meta->removeTriangleSelector(node->getTriangleSelector());
	ITriangleSelector *selector = 0;
	if (node->getType() == ESNT_OCTREE || node->getType() == ESNT_MESH) {
		selector = smgr->createOctreeTriangleSelector(((IMeshSceneNode*)node)->getMesh(), node);
	} else if (node->getType() == ESNT_TERRAIN) {
		selector = smgr->createTerrainTriangleSelector((ITerrainSceneNode*)node);
	}
	node->setTriangleSelector(selector);
    meta->addTriangleSelector(selector);
	selector->drop();

	return selector;
}

void CCollisionManager::setCollisionToAnAnimatedNode(ISceneNode *node) {
	meta->removeTriangleSelector(node->getTriangleSelector());
	ITriangleSelector *selector = 0;
	selector = smgr->createTriangleSelector((IAnimatedMeshSceneNode*)node);
	node->setTriangleSelector(selector);
    meta->addTriangleSelector(selector);
	selector->drop();
}

void CCollisionManager::setCollisionFromBoundingBox(ISceneNode *node) {
	meta->removeTriangleSelector(node->getTriangleSelector());
    ITriangleSelector *selector = 0;
	selector = smgr->createTriangleSelectorFromBoundingBox(node);
	node->setTriangleSelector(selector);
    meta->addTriangleSelector(selector);
	selector->drop();
}

void CCollisionManager::createAnimatorCollisionCamera(ISceneNode *camera) {
	if (fpsCameraSettings->getAnimator()) {
		fpsCameraSettings->unsetSettings(camera);
	}

	fpsCameraSettings->setSettings(smgr, meta, camera);
}
