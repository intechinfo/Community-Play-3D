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

	void applyAnimationToModel(irr::scene::ISceneNode *node, irr::u32 animationNumber);

	void applyAnimationFromNameToModel(irr::scene::ISceneNode *node, irr::core::stringc name);

	irr::s32 getCurrentAnimationIndiceOf(irr::scene::ISceneNode *node);

	void setCustomCallback(irr::scene::ISceneNode *node, std::function<void(irr::scene::IAnimatedMeshSceneNode *node)> callback);
    
private:

	CDevices *devices;

	class CCustomAnimationCallback : public irr::scene::IAnimationEndCallBack {
	public:
		CCustomAnimationCallback(std::function<void(irr::scene::IAnimatedMeshSceneNode *node)> callback) {
			Callback = callback;
		}

		~CCustomAnimationCallback() {
			//delete &Callback;
		}

		void OnAnimationEnd(irr::scene::IAnimatedMeshSceneNode *node) {
			node->setAnimationEndCallback(0);
			Callback(node);
			delete this;
		}
	private:
		std::function<void(irr::scene::IAnimatedMeshSceneNode *node)> Callback;
	};

};

} /// End namespace controllers
} /// End namespace cp3d

#endif
