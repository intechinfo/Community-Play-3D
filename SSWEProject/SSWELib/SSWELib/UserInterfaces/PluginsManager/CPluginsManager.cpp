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
            #ifndef _IRR_OSX_PLATFORM_
			pluginname.remove(".dll");
            #else
            pluginname.remove(".dylib");
            #endif
			names.push_back(pluginname);
		}
	}

	devices->getDevice()->getFileSystem()->changeWorkingDirectoryTo(oldPath.c_str());

	return names;
}

array<stringc> CPluginsManager::getAllSSWEPluginsNames() {
	array<stringc> names;

	stringc oldPath = devices->getDevice()->getFileSystem()->getWorkingDirectory();
	devices->getDevice()->getFileSystem()->changeWorkingDirectoryTo(stringc(devices->getWorkingDirectory() + "plugins/SSWE").c_str());

	IFileList *fl = devices->getDevice()->getFileSystem()->createFileList();
	for (u32 j=0; j < fl->getFileCount(); j++) {
		if (fl->getFileName(j) != "." && fl->getFileName(j) != "..") {
			stringw pluginname = fl->getFileName(j);
            #ifndef _IRR_OSX_PLATFORM_
			pluginname.remove(".dll");
            #else
            pluginname.remove(".dylib");
            #endif
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
                    devices->getCoreData()->destroyMonitor(existedMonitor);
					return;
				}
			}
		}
	}

	//IF MONITOR DOESN'T EXISTS, TRY AND LOAD IT
	stringc ppath = devices->getWorkingDirectory().c_str();
	ppath += "plugins/monitors/";
	ppath += path;
    #ifndef _IRR_OSX_PLATFORM_
	ppath += ".dll";
    #else
    ppath += ".dylib";
    #endif

    #ifndef _IRR_OSX_PLATFORM_
    
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
    
    #else
    
    void* hdll = NULL;
	IMonitor* newMonitor = NULL;
	typedef void* (*pvFunctv)();
	pvFunctv createMonitor;
    hdll = dlopen(stringc(ppath).c_str(), RTLD_LAZY);
    if (hdll == NULL) {
        devices->addErrorDialog(L"Plugin Error", stringw(ppath + L"\nAn error occured\nCannot load the monitor plugin").c_str(), EMBF_OK);
    } else {
        createMonitor = reinterpret_cast < pvFunctv > (dlsym(hdll, "createMonitor"));
        if (createMonitor == NULL) {
			devices->addErrorDialog(L"Plugin Error", stringw(ppath + L"\nAn error occured\nCannot find the process \"createMonitor\" into the dynamic library").c_str(), EMBF_OK);
		} else {
			newMonitor = static_cast < IMonitor* > (createMonitor());
            
			newMonitor->init();
			newMonitor->setActiveCamera(devices->getRenderingSceneManager()->getActiveCamera());
			newMonitor->setSceneManager(devices->getRenderingSceneManager());
			newMonitor->setGUIEnvironment(devices->getGUIEnvironment());
			newMonitor->setXEffect(devices->getXEffect());
			newMonitor->setName(path);
            
			SMonitor m(newMonitor, hdll);
			devices->getCoreData()->getMonitors()->push_back(m);
			devices->getMonitorRegister()->registerMonitor(newMonitor);
		}
    }
    
    #endif
}

void CPluginsManager::loadSSWEPlugin(stringc path) {
	//IF MONITOR DOESN'T EXISTS, TRY AND LOAD IT
	stringc ppath = devices->getWorkingDirectory().c_str();
	ppath += "plugins/sswe/";
	ppath += path;
    #ifndef _IRR_OSX_PLATFORM_
	ppath += ".dll";
    #else
    ppath += ".dylib";
    #endif

    #ifndef _IRR_OSX_PLATFORM_
	HINSTANCE hdll = NULL;
    #else
    void *hdll = NULL;
    #endif
	ISSWELibPlugin* newPlugin = NULL;
	typedef void* (*pvFunctv)();
	pvFunctv createSSWELibPlugin;
    #ifndef _IRR_OSX_PLATFORM_
	hdll = LoadLibrary(stringw(ppath).c_str());
    #else
    hdll = dlopen(stringc(ppath).c_str(), RTLD_LAZY);
    #endif

	if (hdll == NULL) {
		devices->addErrorDialog(L"Plugin Error", stringw(ppath + L"\nAn error occured\nCannot load the SSWE plugin").c_str(), EMBF_OK);
	} else {
        #ifndef _IRR_OSX_PLATFORM_
		createSSWELibPlugin = reinterpret_cast < pvFunctv > (GetProcAddress(hdll, "createSSWELibPlugin"));
        #else
        createSSWELibPlugin = reinterpret_cast < pvFunctv > (dlsym(hdll, "createSSWELibPlugin"));
        #endif
		if (createSSWELibPlugin == NULL) {
			devices->addErrorDialog(L"Plugin Error", stringw(ppath + L"\nAn error occured\nCannot find the process \"createSSWELibPlugin\" into the DLL").c_str(), EMBF_OK);
		} else {
			newPlugin = static_cast < ISSWELibPlugin* > (createSSWELibPlugin());

            SSSWEPlugin sswePlugin(newPlugin, hdll);
            devices->getCoreData()->getSSWEPlugins()->push_back(sswePlugin);
            
			newPlugin->setDevices(devices);
			newPlugin->open();
		}
	}
}
