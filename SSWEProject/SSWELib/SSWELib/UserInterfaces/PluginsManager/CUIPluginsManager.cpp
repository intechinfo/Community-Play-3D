#include "stdafx.h"
#include "CUIPluginsManager.h"


CUIPluginsManager::CUIPluginsManager(CDevices *_devices, CPluginsManager *_pluginsManager) {
	devices = _devices;
	pluginsManager = _pluginsManager;

	IGUIEnvironment *gui = devices->getGUIEnvironment();

	//-----------------------------------
    //GUI ELEMENTS
	window  = gui->addWindow(rect<s32>(80, 230, 600, 700), false, L"Plugins Manager", 0, -1);
	window->getMinimizeButton()->setVisible(true);
	tabcrtl = gui->addTabControl(rect<s32>(10, 60, 510, 410), window, true, true, -1);

	//MONITORS
	monitorsTab = tabcrtl->addTab(L"Monitors", -1);
	monitors = gui->addListBox(rect<s32>(10, 0, 490, 290), monitorsTab, -1, true);
	(enableMonitor = gui->addCheckBox(false, rect<s32>(10, 290, 490, 310), monitorsTab, -1, L"Enable"))->setEnabled(false);
	for (u32 i=0; i < pluginsManager->getAllMonitorsPluginsNames().size(); i++) {
		monitors->addItem(stringw(pluginsManager->getAllMonitorsPluginsNames()[i]).c_str());
	}

	tabcrtl->addTab(L"Other", -1);

	close = gui->addButton(rect<s32>(10, 430, 110, 457), window, -1, L"Close", L"Close the window");
	//-----------------------------------

	devices->getEventReceiver()->AddEventReceiver(this, window);
}

CUIPluginsManager::~CUIPluginsManager() {

}

bool CUIPluginsManager::OnEvent(const SEvent &event) {

	if (event.EventType == EET_USER_EVENT) {
		if (event.UserEvent.UserData1 == ECUE_REACTIVE_MINIMIZED_WINDOW) {
			if (event.UserEvent.UserData2 == window->getReferenceCount()) {
				//devices->getEventReceiver()->RemoveMinimizedWindow(this);
			}
		}
	}

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window) {
				devices->getEventReceiver()->RemoveEventReceiver(this);
				window->remove();
				delete this;
			}
		}

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == window->getMinimizeButton()) {
				//devices->getEventReceiver()->AddMinimizedWindow(this, window);
			}

			if (event.GUIEvent.Caller == close) {
				devices->getEventReceiver()->RemoveEventReceiver(this);
				window->remove();
				delete this;
			}
		}

		if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED || event.GUIEvent.EventType == EGET_LISTBOX_SELECTED_AGAIN) {
			if (event.GUIEvent.Caller == monitors) {
				if (monitors->getSelected() != -1) {
					enableMonitor->setEnabled(true);
					for (u32 i=0; i < devices->getMonitorRegister()->getMonitorCount(); i++) {
						stringc monitorName =  monitors->getListItem(monitors->getSelected());
						monitorName.make_upper();
						if (devices->getMonitorRegister()->getMonitor(i)->getName() == monitorName) {
							enableMonitor->setChecked(true);
							break;
						}
						enableMonitor->setChecked(false);
					}
				} else {
					enableMonitor->setChecked(false);
					enableMonitor->setEnabled(false);
				}
			}
		}

		if (event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED) {
			if (event.GUIEvent.Caller == enableMonitor) {
				stringc monitorName =  monitors->getListItem(monitors->getSelected());
				monitorName.make_upper();
				pluginsManager->loadMonitorPlugin(monitorName);
			}
		}
	}

	return false;
}
