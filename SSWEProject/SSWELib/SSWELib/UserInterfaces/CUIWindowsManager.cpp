/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "stdafx.h"
#include "CUIWindowsManager.h"

CUIWindowsManager::CUIWindowsManager(CDevices *_devices) {
	devices = _devices;

	IGUIEnvironment *gui = devices->getGUIEnvironment();
	IVideoDriver *driver = devices->getVideoDriver();

	window = gui->addWindow(rect<s32>(0, 0, 0, 0),
		false, L"Windows Manager", 0, -1);
	window->getCloseButton()->remove();
	window->setVisible(true);
	window->setDrawTitlebar(false);

	textureImage = driver->getTexture("GUI/ss_logo.png");

	focusOn = false;
}

CUIWindowsManager::~CUIWindowsManager() {

}

void CUIWindowsManager::update() {
	devices->getGUIEnvironment()->getRootGUIElement()->bringToFront(window);
	u32 numOfWindows = 0;
	for (u32 i=0; i < devices->getEventReceiver()->GetMinimizedWindowsCount(); i++) {
		if (devices->getEventReceiver()->getWindowName(i)) {
			devices->getEventReceiver()->getWindowName(i)->setRelativePosition(rect<s32>(10+(150*numOfWindows*1), 2, 150+(150*numOfWindows+1), 23));
			((IGUIStaticText *)devices->getEventReceiver()->getWindowName(i))->setDrawBackground(true);
			if (devices->getGUIEnvironment()->getFocus() != devices->getEventReceiver()->getWindowName(i))
				((IGUIStaticText *)devices->getEventReceiver()->getWindowName(i))->setDrawBorder(true);
			else
				((IGUIStaticText *)devices->getEventReceiver()->getWindowName(i))->setDrawBorder(false);
			window->addChild(devices->getEventReceiver()->getWindowName(i));
			numOfWindows++;
		}
	}
}

bool CUIWindowsManager::OnEvent(const SEvent &event) {

	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP) {
			if (devices->getCore()->elementIsInArrayOfElements(devices->getGUIEnvironment()->getFocus(),
															   devices->getCore()->getArrayOfAListOfGUIElementChildren(window))) 
			{
				if (devices->getGUIEnvironment()->getFocus()) {
					IGUIEnvironment *gui = devices->getGUIEnvironment();
					if (gui->getFocus() != window && devices->getDevice()->getCursorControl()->getPosition().Y >= 20) {
						devices->getEventReceiver()->ReactiveWindow(devices->getGUIEnvironment()->getFocus()->getID());
						((IGUIStaticText *)devices->getGUIEnvironment()->getFocus())->setDrawBorder(false);
					}
				}
			}
		}

		if (event.MouseInput.Event == EMIE_MOUSE_MOVED && !event.MouseInput.isLeftPressed()) {
			if (event.MouseInput.Y >= devices->getVideoDriver()->getScreenSize().Height-10) {
				focusOn = true;
			}
			if (window->isVisible() && event.MouseInput.Y < devices->getVideoDriver()->getScreenSize().Height-100) {
				focusOn = false;
			}
			if (event.MouseInput.Y >= devices->getVideoDriver()->getScreenSize().Height-30) {
				window->setVisible(true);
			}
			if (window->isVisible() && event.MouseInput.Y < devices->getVideoDriver()->getScreenSize().Height-25 && !focusOn) {
				//window->setVisible(false);
			}
		}
	}

	if (window->isVisible()) {
		IVideoDriver *driver = devices->getVideoDriver();
		if (focusOn) {
			//window->setRelativePosition(rect<s32>(0, driver->getScreenSize().Height-100, driver->getScreenSize().Width, driver->getScreenSize().Height));
		} else {
			window->setRelativePosition(rect<s32>(0, driver->getScreenSize().Height-25, driver->getScreenSize().Width, driver->getScreenSize().Height));
		}
	}

	return false;
}
