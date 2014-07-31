//
//  CUIOpenFileDialog.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//

#ifndef __C_EXAMPLE_6_H_INCLUDED__
#define __C_EXAMPLE_6_H_INCLUDED__

#include <ISSWECoreUserInterface.h>

class CExample6 : public irr::IEventReceiver, public cp3d::core::IUpdate {

public:

	//-----------------------------------
	//CTOR & DTOR
    CExample6(cp3d::ISSWECoreUserInterface *core);
	~CExample6();
	//-----------------------------------

	//-----------------------------------
	//INHERITANCE
	/// Handle our events
	bool OnEvent(const SEvent &event);

	/// Update the game
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
	//-----------------------------------

	//-----------------------------------
	//CHARACTER
	irr::f32 direction;
	irr::f32 zdirection;
	irr::scene::IAnimatedMeshSceneNode *player;
	irr::scene::IBoneSceneNode *boneNode;
	irr::scene::ICameraSceneNode *camera;
	//-----------------------------------
};


#endif
