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

class CUpdate;

class CExample4 : public irr::IEventReceiver, public IUpdate {

public:

	//-----------------------------------
	//CTOR & DTOR
    CExample4(ISSWECoreUserInterface *core);
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
	ISSWECoreUserInterface *core;
	IDevices *devices;

	CUpdate *updater;
	//-----------------------------------

};


#endif
