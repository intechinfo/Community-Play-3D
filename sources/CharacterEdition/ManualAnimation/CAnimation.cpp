//
//  CAction.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 07/11/12.
//
//

#include "CAnimation.h"

CAnimation::CAnimation(irr::scene::IAnimatedMeshSceneNode *_node) {
	node = _node;
	if (node) {
		for (irr::u32 i=0; i < node->getJointCount(); i++) {
			irr::scene::IBoneSceneNode *bn = node->getJointNode(i);
			SBoneInformations sbi(bn, bn->getPosition(), bn->getRotation(), bn->getScale());
			bonesInformations.push_back(sbi);
		}
	}
	name = "myName";
}

CAnimation::~CAnimation() {
	bonesInformations.clear();
}

CManualAction *CAnimation::addManualAction() {
	CManualAction *ma = new CManualAction();

	return ma;
}
