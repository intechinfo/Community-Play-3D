//
//  CUIWindowEditNodeAnimators.h
//  Windows
//
//  Created by Arthur Caron on 12/06/13.
//
//

#ifndef __C_UI_CAMERA_SCENARIO_MAKER_H_INCLUDED__
#define __C_UI_CAMERA_SCENARIO_MAKER_H_INCLUDED__

#include "../../../Device/CDevices.h"

class CUICameraScenarioMaker : public IEventReceiver {

public:

	CUICameraScenarioMaker(CDevices *_devices);
	~CUICameraScenarioMaker();

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
    //DEVICE AND DATAS
	CDevices *devices;
	//-----------------------------------

	//-----------------------------------
    //GUI ELEMENTS
	IGUIWindow *window;
	//-----------------------------------

};

#endif