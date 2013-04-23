/*
 *  UIContextMenu.h
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __C_UI_WINDOWS_MANAGER_H_INCLUDED__
#define __C_UI_WINDOWS_MANAGER_H_INCLUDED__

#include "../Device/CDevices.h"

class CUIWindowsManager : public IEventReceiver {

public:

	CUIWindowsManager(CDevices *_devices);
	~CUIWindowsManager();

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
	//DEVICE
	CDevices *devices;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	IGUIWindow *window;
	//-----------------------------------

};

#endif
