//
//  CUIOpenFileDialog.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//

#ifndef __C_EXAMPLE_4_H_INCLUDED__
#define __C_EXAMPLE_4_H_INCLUDED__

#include <ISSWECoreUserInterface.h>

class CExample4 : public irr::IEventReceiver, public cp3d::core::IUpdate {

public:

	//-----------------------------------
	//CTOR & DTOR
    CExample4(cp3d::ISSWECoreUserInterface *core);
	~CExample4();

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
	//ANIMATIONS
	bool reloading;
	bool fireing;
	//-----------------------------------
};


#endif
