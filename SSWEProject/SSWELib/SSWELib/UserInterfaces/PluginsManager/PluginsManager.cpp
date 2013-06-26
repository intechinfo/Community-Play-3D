#include "stdafx.h"
#include "PluginsManager.h"


CPluginsManager::CPluginsManager(CDevices *_devices) {
	devices = _devices;

}

CPluginsManager::~CPluginsManager() {

}

void CPluginsManager::loadMonitorPlugin(stringc path) {
	for (u32 i=0; i < devices->getMonitorRegister()->getMonitorCount(); i++) {
		if (path == devices->getMonitorRegister()->getMonitor(i)->getName()) {
			IMonitor *existedMonitor = devices->getMonitorRegister()->getMonitor(i);
			for (u32 j=0; j < devices->getCoreData()->getMonitors()->size(); j++) {
				if (devices->getCoreData()->getMonitors()->operator[](j).getMonitor() == existedMonitor) {
					devices->getMonitorRegister()->unregisterMonitor(i);
					devices->getCoreData()->getMonitors()->operator[](j).freeInstance();
					devices->getCoreData()->getMonitors()->erase(j);
					return;
				}
			}
		}
	}

	stringc ppath = devices->getWorkingDirectory().c_str();
	ppath += "plugins/monitors/";
	ppath += path;
	ppath += ".dll";

	HINSTANCE hdll = NULL;
	IMonitor* newMonitor = NULL;
	typedef void* (*pvFunctv)();
	pvFunctv createMonitor;
	hdll = LoadLibrary(stringw(ppath).c_str());
	if (hdll == NULL) {
		devices->addErrorDialog(L"Plugin Error", stringw(ppath + L"\nAn error occured\nCannot load the monitor plugin").c_str(), EMBF_OK);
	} else {
		createMonitor = reinterpret_cast < pvFunctv > (GetProcAddress(hdll, "createMonitor"));
		if (createMonitor == NULL) {
			devices->addErrorDialog(L"Plugin Error", stringw(ppath + L"\nAn error occured\nCannot find the process \"createMonitor\" into the DLL").c_str(), EMBF_OK);
		} else {
			newMonitor = static_cast < IMonitor* > (createMonitor());

			newMonitor->init();
			newMonitor->setGUIEnvironment(devices->getGUIEnvironment());
			newMonitor->setName(path);
			newMonitor->setToolsSceneManager(devices->getSecondSceneManager());
			newMonitor->setXEffect(devices->getXEffect());

			SMonitor m(newMonitor, hdll);
			devices->getCoreData()->getMonitors()->push_back(m);
			devices->getMonitorRegister()->registerMonitor(newMonitor);
		}
	}
}
