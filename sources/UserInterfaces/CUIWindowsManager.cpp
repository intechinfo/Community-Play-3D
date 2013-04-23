/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "CUIWindowsManager.h"

CUIWindowsManager::CUIWindowsManager(CDevices *_devices) {
	devices = _devices;

	IGUIEnvironment *gui = devices->getGUIEnvironment();
	IVideoDriver *driver = devices->getVideoDriver();

	window = gui->addWindow(rect<s32>(0, 0, 0, 0),
		false, L"Windows Manager", 0, -1);
	window->getCloseButton()->remove();
	window->setVisible(false);
}

CUIWindowsManager::~CUIWindowsManager() {

}

bool CUIWindowsManager::OnEvent(const SEvent &event) {

	if (window->isVisible()) {
		IVideoDriver *driver = devices->getVideoDriver();
		window->setRelativePosition(rect<s32>(0, driver->getScreenSize().Height-100, driver->getScreenSize().Width, driver->getScreenSize().Height));
	}

	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == EMIE_MOUSE_MOVED && !event.MouseInput.isLeftPressed()) {
			if (event.MouseInput.Y >= devices->getVideoDriver()->getScreenSize().Height-100) {
				//window->setVisible(true);
			} else {
				window->setVisible(false);
			}
		}
	}

	return false;
}
