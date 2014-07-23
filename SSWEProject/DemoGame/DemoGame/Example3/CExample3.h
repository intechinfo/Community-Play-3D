//
//  CUIOpenFileDialog.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//

#ifndef __C_EXAMPLE_3_H_INCLUDED__
#define __C_EXAMPLE_3_H_INCLUDED__

#include <ISSWECoreUserInterface.h>

class CExample3 : public irr::IEventReceiver, public cp3d::core::IUpdate {

public:

	//-----------------------------------
	//CTOR & DTOR
    CExample3(cp3d::ISSWECoreUserInterface *core);
	~CExample3();

	/// From IEventReceiver interface
	bool OnEvent(const SEvent &event);
	/// From IUpdate interface
	void update();
	//-----------------------------------

	//-----------------------------------
	//METHODS

	/// Load the scene
	void load();

	/// Run the game
	void run();

	//-----------------------------------
    
private:

	//-----------------------------------
	//DATAS
	cp3d::ISSWECoreUserInterface *core;
	cp3d::core::IDevices *devices;

	irr::scene::IAnimatedMeshSceneNode *mp5Node;
	//-----------------------------------


	//-----------------------------------
	//RELOAD CALLBACK
	class CReloadCallback : public irr::scene::IAnimationEndCallBack {
	public:
		CReloadCallback(CExample3 *instance) : example(instance), reloading(false) { }

		/// If finished then return to IDLE animation (looped)
		void OnAnimationEnd(irr::scene::IAnimatedMeshSceneNode* node) {
			example->devices->getAnimationController()->applyAnimationFromNameToModel(example->mp5Node, "IDLE");
			example->mp5Node->setLoopMode(true);
			reloading = false;
		}

		bool isReloading() { return reloading; }
		void setIsReloading(bool reloading) { this->reloading = reloading; }

	private:
		CExample3 *example;
		bool reloading;

	} *reloadCallback;
	//-----------------------------------

	//-----------------------------------
	//RELOAD CALLBACK
	class CFireCallback : public irr::scene::IAnimationEndCallBack {
	public:
		CFireCallback(CExample3 *instance) : example(instance), fireing(false) { }

		/// If finished then return to IDLE animation (looped)
		void OnAnimationEnd(irr::scene::IAnimatedMeshSceneNode* node) {
			example->devices->getAnimationController()->applyAnimationFromNameToModel(example->mp5Node, "IDLE");
			example->mp5Node->setLoopMode(true);
			fireing = false;
		}

		bool isFireing() { return fireing; }
		void setIsFireing(bool fireing) { this->fireing = fireing; }

	private:
		CExample3 *example;
		bool fireing;

	} *fireCallback;
	//-----------------------------------
};


#endif
