/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include "CUISSWEOptions.h"

CUISSWEOptions::CUISSWEOptions(CDevices *_devices) {
	devices = _devices;
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	//-----------------------------------
    //GUI ELEMENTS
	window = gui->addWindow(rect<s32>(340, 140, 810, 420), false, L"SSWE/Plugins Options", 0, -1);
	window->getMinimizeButton()->setVisible(true);
	window->getMaximizeButton()->setVisible(true);

	tabctrl = gui->addTabControl(rect<s32>(3, 21, 470, 280), window, true, true, -1);
	fpsTab = tabctrl->addTab(L"FPS Camera", -1);

	//FPS TAB
	gui->addStaticText(L"Move Forward : ", rect<s32>(10, 10, 110, 30), true, true, fpsTab, -1, true);
	gui->addStaticText(L"Mve Backward : ", rect<s32>(10, 30, 110, 50), true, true, fpsTab, -1, true);
	gui->addStaticText(L"Strafe Left : ", rect<s32>(10, 50, 110, 70), true, true, fpsTab, -1, true);
	gui->addStaticText(L"Strafe Right : ", rect<s32>(10, 70, 110, 90), true, true, fpsTab, -1, true);
	gui->addStaticText(L"Jump : ", rect<s32>(10, 90, 110, 110), true, true, fpsTab, -1, true);
	for (u32 i=0; i < 5; i++) {
		fpsButtons[i].button = gui->addButton(rect<s32>(110, 10, 210, 30), fpsTab, -1, stringw(devices->getKeyMap(i).KeyCode).c_str() , L"");
		fpsButtons[i].button->setRelativePosition(position2di(110, 10+20*i));
		fpsButtons[i].active = false;
	}

	fpsInfoText = gui->addStaticText(L"Click on button and press the key you want.", rect<s32>(10, 140, 390, 160), true, true, fpsTab, -1, true);

	//-----------------------------------

	devices->getEventReceiver()->AddEventReceiver(this, window);
}
CUISSWEOptions::~CUISSWEOptions() {

}

bool CUISSWEOptions::OnEvent(const SEvent &event) {

	if (event.EventType == EET_KEY_INPUT_EVENT) {
		if (tabctrl->getActiveTab() == fpsTab->getNumber()) {
			for (u32 i=0; i < 5; i++) {
				if (fpsButtons[i].active) {
					fpsButtons[i].button->setText(&event.KeyInput.Char);
					fpsButtons[i].active = false;
					SKeyMap keyMap;
					keyMap.Action = devices->getKeyMap(i).Action;
					keyMap.KeyCode = event.KeyInput.Key;
					devices->setKeyMap(keyMap, i);
					devices->applyKeyMapOnFPSCamera();
					CExporter *exporter = new CExporter(devices);
					exporter->exportCamerasConfig();
					fpsInfoText->setText(L"Click on button and press the key you want.");
					break;
				}
			}
		}
	}

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (tabctrl->getActiveTab() == fpsTab->getNumber()) {
				for (u32 i=0; i < 5; i++) {
					fpsButtons[i].active = false;
					if (event.GUIEvent.Caller == fpsButtons[i].button) {
						fpsButtons[i].active = true;
						fpsInfoText->setText(L"Push the key you want");
						break;
					}
				}
			}
		}

		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			devices->getEventReceiver()->RemoveEventReceiver(this);
			window->remove();
			delete this;
		}
	}

	return false;
}
