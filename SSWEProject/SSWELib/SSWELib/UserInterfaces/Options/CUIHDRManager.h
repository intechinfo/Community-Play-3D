//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_HDR_MANAGER_H_INCLUDED__
#define __C_UI_HDR_MANAGER_H_INCLUDED__

#include "../../Device/CDevices.h"

struct SHDRDataStruct;

class CUIHDRManager : public IEventReceiver {

public:

	CUIHDRManager(CDevices *_devices);
	~CUIHDRManager();

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
    //METHODS
	void fillField(u32 index, f32 value);
	//-----------------------------------

	//-----------------------------------
    //DATAS
	CDevices *devices;

	array<SHDRDataStruct> hdrStructs;
	//-----------------------------------
    //GUI ELEMENTS

	//-----------------------------------
    //GUI ELEMENTS
	IGUIWindow *window;
	//-----------------------------------

};

#endif