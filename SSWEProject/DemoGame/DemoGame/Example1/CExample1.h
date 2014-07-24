//
//  CUIOpenFileDialog.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//

#ifndef __C_EXAMPLE_1_H_INCLUDED__
#define __C_EXAMPLE_1_H_INCLUDED__

#include <ISSWECoreUserInterface.h>

class CExample1 {

public:

	//-----------------------------------
	//CTOR & DTOR
    CExample1(cp3d::ISSWECoreUserInterface *core);
	~CExample1();
	//-----------------------------------

	//-----------------------------------
	//METHODS

	/// Load the scene
	void load(irr::core::stringc file);

	/// Run the game
	void run();

	//-----------------------------------
    
private:

	//-----------------------------------
	//DATAS
	cp3d::ISSWECoreUserInterface *core;
	cp3d::core::IDevices *devices;
	//-----------------------------------

};


#endif
