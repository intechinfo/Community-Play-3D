//
//  CAction.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 07/11/12.
//
//

#ifndef __C_FRAME_H_INCLUDED__
#define __C_FRAME_H_INCLUDED__

#include <irrlicht.h>

#include "CAnimation.h"

class CFrame {

public:

	CFrame(irr::scene::IAnimatedMeshSceneNode *_node);
    ~CFrame();

	irr::core::array<CAnimation *> *getAnimations() { return &animations; }

	irr::core::stringw getName() { return name; }
	void setName(irr::core::stringw _name) { name = _name; }

	CAnimation *createNewAnimation();
	void removeAnimation(CAnimation *anim);

private:

	irr::scene::IAnimatedMeshSceneNode *node;

	irr::core::array<CAnimation *> animations;

	irr::core::stringw name;

};

#endif
