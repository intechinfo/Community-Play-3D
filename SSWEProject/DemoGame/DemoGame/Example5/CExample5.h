//
//  CUIOpenFileDialog.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//

#ifndef __C_EXAMPLE_5_H_INCLUDED__
#define __C_EXAMPLE_5_H_INCLUDED__

#include <ISSWECoreUserInterface.h>

class CExample5 {

public:

	//-----------------------------------
	//CTOR & DTOR
    CExample5(cp3d::ISSWECoreUserInterface *core);
	~CExample5();
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
	//DESATURATION FILTER CALLBACK
	irr::f32 desaturation;
	//-----------------------------------
};


#endif
