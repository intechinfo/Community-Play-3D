//
//  CExporter.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//
#include "stdafx.h"
#include "CSSWELibTestPlugin.h"

void* createSSWELibPlugin() {
	return static_cast< void* > (new CSSWELibTestPlugin);
}

CSSWELibTestPlugin::CSSWELibTestPlugin() {

}

CSSWELibTestPlugin::~CSSWELibTestPlugin() {

}

void CSSWELibTestPlugin::setDevices(IDevices *_devices) {
	devices = _devices;
}

void CSSWELibTestPlugin::close() {
	window->remove();

	devices->getEventReceiver()->RemoveEventReceiver(this);
	delete this;
}

void CSSWELibTestPlugin::open() {
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	//GUI ELEMENTS
	window = gui->addWindow(rect<s32>(0, 0, 500, 500), false, L"Plugin test !", 0, -1);
	window->getMaximizeButton()->setVisible(true);
	window->getMinimizeButton()->setVisible(true);

	closeWindow = gui->addButton(rect<s32>(30, 70, 100, 100), window, -1, L"Close", L"Close this window");

	//DATAS
	devices->getEventReceiver()->AddEventReceiver(this, window);

}

bool CSSWELibTestPlugin::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == window->getMaximizeButton()) {
				devices->getCore()->maximizeWindow(window, rect<s32>(0, 0, 500, 500));
			}

			if (event.GUIEvent.Caller == closeWindow) {
				this->close();
			}
		}
	}

	return false;
}
