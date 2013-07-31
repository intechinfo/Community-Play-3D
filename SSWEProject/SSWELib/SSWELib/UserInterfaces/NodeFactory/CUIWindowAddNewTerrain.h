//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_WINDOW_ADD_NEW_TERRAIN_H_INCLUDED__
#define __C_UI_WINDOW_ADD_NEW_TERRAIN_H_INCLUDED__

#include "../../Device/CDevices.h"

class CUIWindowAddNewTerrain : public IEventReceiver {

public:

	CUIWindowAddNewTerrain(CDevices *_devices);
	~CUIWindowAddNewTerrain();

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
	//DEVICES ELEMENTS AND DATAS
	CDevices *devices;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	IGUIWindow *window;

	IGUIEditBox *sizeeb;
	IGUICheckBox *randomHillscb;

	IGUIButton *accept, *close;
	//-----------------------------------

};

#endif