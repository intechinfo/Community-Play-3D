//
//  CUIOpenFileDialog.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//

#ifndef __I_ANIMATION_CONTROLLER_H_INCLUDED__
#define __I_ANIMATION_CONTROLLER_H_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace controllers {

class IAnimationController {
    
public:

	/// Apply a saved animation to the scene node "node".
	/// animationNumber between 0-n, previously configured by the user in CP3D World Editor.
	/// If animationNumber is false then the animation will no be ran
	virtual void applyAnimationToModel(irr::scene::ISceneNode *node, irr::u32 animationNumber) = 0;

	/// Same as applyAnimationToModel but finds the animation by name
	/// name : the name of the animation
	virtual void applyAnimationFromNameToModel(irr::scene::ISceneNode *node, irr::core::stringc name) = 0;

	/// Returns the current animation number played by the node
	/// Value from 0 to n, if unused, then returns -1
	virtual irr::s32 getCurrentAnimationIndiceOf(irr::scene::ISceneNode *node) = 0;

};

} /// End namespace controllers
} /// End namespace cp3d

#endif
