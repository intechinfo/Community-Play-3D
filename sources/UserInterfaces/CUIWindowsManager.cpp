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

	textureImage = driver->getTexture("GUI/ss_logo.png");
}

CUIWindowsManager::~CUIWindowsManager() {

}

void CUIWindowsManager::update() {
	devices->getGUIEnvironment()->getRootGUIElement()->bringToFront(window);
	EventReceiver *ercv = devices->getEventReceiver();
	array<IGUIElement *> windowElements = devices->getCore()->getArrayOfAListOfGUIElementChildren(window);
	for (u32 i=0; i < ercv->GetMinimizedWindowsCount(); i++) {
		if (!devices->getCore()->elementIsInArrayOfElements(ercv->getWindow(i), windowElements)) {
			//WORK ON ANIMATION HERE
			window->addChild(ercv->getWindow(i));
		} else {
			ercv->getWindow(i)->setRelativePosition(rect<s32>(10+(90*i*1), 30, 90+(90*i+1), 80));
		}
	}
}

bool CUIWindowsManager::OnEvent(const SEvent &event) {

	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP) {
			if (devices->getCore()->elementIsInArrayOfElements(devices->getGUIEnvironment()->getFocus(),
															   devices->getCore()->getArrayOfAListOfGUIElementChildren(window))) 
			{

				devices->getEventReceiver()->ReactiveWindow(devices->getGUIEnvironment()->getFocus()->getReferenceCount());
			}
		}

		if (event.MouseInput.Event == EMIE_MOUSE_MOVED && !event.MouseInput.isLeftPressed()) {
			if (event.MouseInput.Y >= devices->getVideoDriver()->getScreenSize().Height-10) {
				window->setVisible(true);
			}
			if (window->isVisible() && event.MouseInput.Y < devices->getVideoDriver()->getScreenSize().Height-100) {
				window->setVisible(false);
			}
		}
	}

	IVideoDriver *driver = devices->getVideoDriver();
	if (window->isVisible()) {
		window->setRelativePosition(rect<s32>(0, driver->getScreenSize().Height-100, driver->getScreenSize().Width, driver->getScreenSize().Height));
	}

	return false;
}
