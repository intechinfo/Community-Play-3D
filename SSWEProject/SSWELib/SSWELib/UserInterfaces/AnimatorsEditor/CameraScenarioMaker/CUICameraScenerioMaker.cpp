//
//  CUIWindowEditNodeAnimators.cpp
//  Windows
//
//  Created by Arthur Caron on 12/06/13.
//
//

#include "stdafx.h"
#include "CUICameraScenerioMaker.h"

CUICameraScenarioMaker::CUICameraScenarioMaker(CDevices *_devices) {
	devices = _devices;

	//-----------------------------------
    //GUI ELEMENTS

	//-----------------------------------

	devices->getEventReceiver()->AddEventReceiver(this, window);
}

CUICameraScenarioMaker::~CUICameraScenarioMaker() {
	
}

bool CUICameraScenarioMaker::OnEvent(const SEvent &event) {

	return false;
}
