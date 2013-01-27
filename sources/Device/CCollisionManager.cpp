//
//  CCollisionManager.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 01/10/12.
//
//

#include "CCollisionManager.h"

CCollisionManager::CCollisionManager(ISceneManager *_smgr) {
    smgr = _smgr;
    
    meta = smgr->createMetaTriangleSelector();
    animCollisions = 0;
}

CCollisionManager::~CCollisionManager() {
    
}

void CCollisionManager::setCollisionToAnOctTreeNode(ISceneNode *node) {
	ITriangleSelector *selector = 0;
	if (node->getType() == ESNT_OCTREE) {
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
	selector = smgr->createTriangleSelectorFromBoundingBox(node);
	node->setTriangleSelector(selector);
    meta->addTriangleSelector(selector);
	selector->drop();
}

void CCollisionManager::createAnimatorCollisionCamera(ISceneNode *camera) {
    if (animCollisions) {
        camera->removeAnimator(animCollisions);
    }
    animCollisions = smgr->createCollisionResponseAnimator(meta, camera, vector3df(2.f, 8.5f, 2.f),
                                                           vector3df(0.f, -0.981f, 0.f), vector3df(0.f, 0.f, 0.f));
    camera->addAnimator(animCollisions);
}
