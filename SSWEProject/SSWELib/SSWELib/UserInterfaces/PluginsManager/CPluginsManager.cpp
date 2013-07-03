#include "stdafx.h"
#include "CPluginsManager.h"


CPluginsManager::CPluginsManager(CDevices *_devices) {
	devices = _devices;

}

CPluginsManager::~CPluginsManager() {

}

array<stringc> CPluginsManager::getAllMonitorsPluginsNames() {
	array<stringc> names;

	stringc oldPath = devices->getDevice()->getFileSystem()->getWorkingDirectory();
	devices->getDevice()->getFileSystem()->changeWorkingDirectoryTo(stringc(devices->getWorkingDirectory() + "plugins/monitors").c_str());

	IFileList *fl = devices->getDevice()->getFileSystem()->createFileList();
	for (u32 j=0; j < fl->getFileCount(); j++) {
		if (fl->getFileName(j) != "." && fl->getFileName(j) != "..") {
			stringw pluginname = fl->getFileName(j);
			pluginname.remove(".dll");
			names.push_back(pluginname);
		}
	}

	devices->getDevice()->getFileSystem()->changeWorkingDirectoryTo(oldPath.c_str());

	return names;
}

void CPluginsManager::loadMonitorPlugin(stringc path) {
	//SEARCH IF THE MONITORS EXISTS
	//IF EXISTS, MEANS WE REMOVE IT
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

	//IF MONITOR DOESN'T EXISTS, TRY AND LOAD IT
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
			newMonitor->setActiveCamera(devices->getRenderingSceneManager()->getActiveCamera());
			newMonitor->setSceneManager(devices->getRenderingSceneManager());
			newMonitor->setGUIEnvironment(devices->getGUIEnvironment());
			newMonitor->setToolsSceneManager(devices->getSecondSceneManager());
			newMonitor->setXEffect(devices->getXEffect());
			newMonitor->setName(path);

			SMonitor m(newMonitor, hdll);
			devices->getCoreData()->getMonitors()->push_back(m);
			devices->getMonitorRegister()->registerMonitor(newMonitor);
		}
	}
}
