//
//  CUIOpenFileDialog.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//

#ifndef __C_ANIMATION_CONTROLLER_H_INCLUDED__
#define __C_ANIMATION_CONTROLLER_H_INCLUDED__

#include <SSWELib.h>
#include <irrlicht.h>
#include <IAnimationController.h>

class CDevices;

namespace cp3d {
namespace controllers {

class CAnimationController : public IAnimationController {
    
public:
    
    CAnimationController(CDevices *_devices);
    ~CAnimationController();

	virtual void applyAnimationToModel(irr::scene::ISceneNode *node, irr::u32 animationNumber);

	virtual void applyAnimationFromNameToModel(irr::scene::ISceneNode *node, irr::core::stringc name);

	virtual irr::s32 getCurrentAnimationIndiceOf(irr::scene::ISceneNode *node);
    
private:

	CDevices *devices;

};

} /// End namespace controllers
} /// End namespace cp3d

#endif
