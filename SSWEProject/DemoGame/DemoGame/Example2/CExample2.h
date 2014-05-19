//
//  CUIOpenFileDialog.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//

#ifndef __C_EXAMPLE_2_H_INCLUDED__
#define __C_EXAMPLE_2_H_INCLUDED__

#include <ISSWECoreUserInterface.h>

class CExample2 : public irr::IEventReceiver {

public:

	//-----------------------------------
	//CTOR & DTOR
    CExample2(ISSWECoreUserInterface *core);
	~CExample2();
	//-----------------------------------

	//-----------------------------------
	//EVENTS HANDLER (virtual OnEvent(const SEvent &event));
	bool OnEvent(const SEvent &event);
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
	ISSWECoreUserInterface *core;
	IDevices *devices;

	/// Chief Node is our animated mesh scene node
	irr::scene::IAnimatedMeshSceneNode *chiefNode;
	//-----------------------------------

	//-----------------------------------
	//ANIMATION CALLBACK TEST
	class CExample2AnimationCallback : public irr::scene::IAnimationEndCallBack {
	public:
		CExample2AnimationCallback(CExample2 *instance) : example(instance) { }

		virtual void OnAnimationEnd(irr::scene::IAnimatedMeshSceneNode* node) {
			/// In fact, "node" is the chiefNode store in CExample2
			/// This function is called when the previous animation is ended
			/// So, set up a new animation,
			example->devices->getAnimationController()->applyAnimationFromNameToModel(node, "OTHER");
			/// remove the animation callback of the node
			node->setAnimationEndCallback(0);
			/// Free memory, only C++11 !!! =D
			delete this;
		}

	private:

		CExample2 *example;

	};
	//-----------------------------------

};


#endif
