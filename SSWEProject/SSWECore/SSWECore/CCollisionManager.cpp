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

	fpsCameraSettings = SFPSCameraSettings();
}

CCollisionManager::~CCollisionManager() {
	meta->removeAllTriangleSelectors();
	meta->drop();
}

void CCollisionManager::setCollisionToAnOctTreeNode(ISceneNode *node) {
	ITriangleSelector *selector = 0;
	if (node->getType() == ESNT_OCTREE || node->getType() == ESNT_MESH) {
		selector = smgr->createOctreeTriangleSelector(((IMeshSceneNode*)node)->getMesh(), node);
	} else {
		selector = smgr->createTerrainTriangleSelector((ITerrainSceneNode*)node);
	}
	node->setTriangleSelector(selector);
    meta->addTriangleSelector(selector);
	selector->drop();
}

void CCollisionManager::setCollisionToAnAnimatedNode(ISceneNode *node) {
	ITriangleSelector *selector = 0;
    selector = smgr->createTriangleSelector((IAnimatedMeshSceneNode *)node);
	node->setTriangleSelector(selector);
    meta->addTriangleSelector(selector);
	selector->drop();
}

void CCollisionManager::setCollisionFromBoundingBox(ISceneNode *node) {
    ITriangleSelector *selector = 0;
	selector = smgr->createTriangleSelectorFromBoundingBox(node);
	node->setTriangleSelector(selector);
    meta->addTriangleSelector(selector);
	selector->drop();
}

void CCollisionManager::createAnimatorCollisionCamera(ISceneNode *camera) {
	if (fpsCameraSettings.getAnimator()) {
		fpsCameraSettings.unsetSettings(camera);
	}

	fpsCameraSettings.setEllipsoidRadius(vector3df(1.f, 1.f, 1.f));
	fpsCameraSettings.setGravityPerSecond(vector3df(0.f, -0.981f, 0.f));
	fpsCameraSettings.setEllipsoidTranslation(vector3df(0.f, 14.4f, 0.f));

	fpsCameraSettings.setSettings(smgr, meta, camera);
}
