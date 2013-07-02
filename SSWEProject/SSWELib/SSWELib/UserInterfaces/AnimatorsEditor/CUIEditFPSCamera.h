//
//  CUIWindowEditNodeAnimators.h
//  Windows
//
//  Created by Arthur Caron on 12/06/13.
//
//

#ifndef __C_UI_EDIT_FPS_CAMERA_H_INCLUDED__
#define __C_UI_EDIT_FPS_CAMERA_H_INCLUDED__

#include "../../Device/CDevices.h"

class CUIEditFPSCamera : public IEventReceiver {

public:

	CUIEditFPSCamera(CDevices *_devices);
	~CUIEditFPSCamera();

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
    //DEVICE AND DATAS
	CDevices *devices;
	//-----------------------------------

	//-----------------------------------
    //GUI ELEMENTS
	IGUIWindow *window;

	IGUIEditBox *erx, *ery, *erz;
	IGUIEditBox *gpsx, *gpsy, *gpsz;
	IGUIEditBox *etx, *ety, *etz;
	IGUIEditBox *sv, *ms;

	IGUIButton *accept, *close;
	//-----------------------------------

};

#endif