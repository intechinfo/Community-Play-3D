//
//  CAction.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 07/11/12.
//
//

#include "stdafx.h"
#include "CFrame.h"

CFrame::CFrame(irr::scene::IAnimatedMeshSceneNode *_node) {
	node = _node;
	animations.clear();
}

CFrame::~CFrame() {
	for (irr::u32 i=0; i < animations.size(); i++) {
		delete animations[i];
	}
	animations.clear();
}

CAnimation *CFrame::createNewAnimation() {
	CAnimation *anim = new CAnimation(node);
	getAnimations()->push_back(anim);

	return anim;
}

void CFrame::removeAnimation(CAnimation *anim) {
	for (irr::u32 i=0; i < animations.size(); i++) {
		if (animations[i] == anim) {
			delete animations[i];
			animations.erase(i);
			break;
		}
	}
}
