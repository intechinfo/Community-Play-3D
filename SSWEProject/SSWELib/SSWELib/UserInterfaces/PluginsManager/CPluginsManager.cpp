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
	devices->getDevice()->getFileSystem()->changeWorkingDirectoryTo(stringc(devices->getWorkingDirectory() + "Plugins/Monitors").c_str());

	IFileList *fl = devices->getDevice()->getFileSystem()->createFileList();
	for (u32 j=0; j < fl->getFileCount(); j++) {
		if (fl->getFileName(j) != "." && fl->getFileName(j) != "..") {
			stringw pluginname = fl->getFileName(j);
            #ifdef _IRR_WINDOWS_API_
            pluginname.remove(".dll");
            #endif
            #ifdef _IRR_LINUX_PLATFORM_
            pluginname.remove(".so");
            #endif
            #ifdef _IRR_OSX_PLATFORM°
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
	devices->getDevice()->getFileSystem()->changeWorkingDirectoryTo(stringc(devices->getWorkingDirectory() + "Plugins/SSWE").c_str());

	IFileList *fl = devices->getDevice()->getFileSystem()->createFileList();
	for (u32 j=0; j < fl->getFileCount(); j++) {
		if (fl->getFileName(j) != "." && fl->getFileName(j) != "..") {
			stringw pluginname = fl->getFileName(j);
            #ifdef _IRR_WINDOWS_API_
            pluginname.remove(".dll");
            #endif
            #ifdef _IRR_LINUX_PLATFORM_
            pluginname.remove(".so");
            #endif
            #ifdef _IRR_OSX_PLATFORM°
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
		/*if (path == devices->getMonitorRegister()->getMonitor(i)->getName()) {
			IMonitor *existedMonitor = devices->getMonitorRegister()->getMonitor(i);
			for (u32 j=0; j < devices->getCoreData()->getMonitors()->size(); j++) {
				if (devices->getCoreData()->getMonitors()->operator[](j).getMonitor() == existedMonitor) {
					devices->getMonitorRegister()->unregisterMonitor(i);
                    devices->getCoreData()->destroyMonitor(existedMonitor);
					return;
				}
			}
		}*/
        for (u32 j=0; j < devices->getCoreData()->getMonitors()->size(); j++) {
			stringc temp_path = devices->getCoreData()->getMonitors()->operator[](j).getName();
			if (devices->getCoreData()->getMonitors()->operator[](j).getName() == path) {
                devices->getMonitorRegister()->unregisterMonitor(i);
                devices->getCoreData()->destroyMonitor(devices->getCoreData()->getMonitors()->operator[](j).getMonitor());
                return;
            }
        }
	}

	//IF MONITOR DOESN'T EXISTS, TRY AND LOAD IT
	stringc ppath = devices->getWorkingDirectory().c_str();
	ppath += "Plugins/Monitors/";
	ppath += path;
    #ifdef _IRR_WINDOWS_API_
	ppath += ".dll";
    #endif
    #ifdef _IRR_OSX_PLATFORM_
    ppath += ".dylib";
    #endif
    #ifdef _IRR_LINUX_PLATFORM_
    ppath += ".so";
    #endif

    #ifdef _IRR_WINDOWS_API_

	HINSTANCE hdll = NULL;
	cp3d::video::IMonitor* newMonitor = NULL;
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
			newMonitor = static_cast < cp3d::video::IMonitor* > (createMonitor());

			newMonitor->setActiveCamera(devices->getRenderingSceneManager()->getActiveCamera());
			newMonitor->setSceneManager(devices->getRenderingSceneManager());
			newMonitor->setGUIEnvironment(devices->getGUIEnvironment());
			newMonitor->setRender(devices->getXEffect());
			newMonitor->setName(path);

			newMonitor->init();

			SMonitor m(newMonitor, hdll);
			m.setName(path);
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
        printf("%s\n", stringc(ppath + L"\nAn error occured\nCannot load the monitor plugin\n" + dlerror()).c_str());
        devices->addErrorDialog(L"Plugin Error", stringw(ppath + L"\nAn error occured\nCannot load the monitor plugin").c_str(), EMBF_OK);
    } else {
        createMonitor = reinterpret_cast < pvFunctv > (dlsym(hdll, "createMonitor"));
        if (createMonitor == NULL) {
            printf("%s\n", stringc(ppath + L"\nAn error occured\nCannot find the process \"createMonitor\" into the dynamic library\n" + dlerror()).c_str());
			devices->addErrorDialog(L"Plugin Error", stringw(ppath + L"\nAn error occured\nCannot find the process \"createMonitor\" into the dynamic library").c_str(), EMBF_OK);
		} else {
			newMonitor = static_cast < IMonitor* > (createMonitor());

			newMonitor->setActiveCamera(devices->getRenderingSceneManager()->getActiveCamera());
			newMonitor->setSceneManager(devices->getRenderingSceneManager());
			newMonitor->setGUIEnvironment(devices->getGUIEnvironment());
			newMonitor->setRender(devices->getXEffect());
			newMonitor->setName(path);

            newMonitor->init();

			SMonitor m(newMonitor, hdll);
			m.setName(path);
			devices->getCoreData()->getMonitors()->push_back(m);
			devices->getMonitorRegister()->registerMonitor(newMonitor);
		}
    }

    #endif
}

void CPluginsManager::loadSSWEPlugin(stringc path) {
	//IF MONITOR DOESN'T EXISTS, TRY AND LOAD IT
	stringc ppath = devices->getWorkingDirectory().c_str();
	ppath += "Plugins/SSWE/";
	ppath += path;
    #ifdef _IRR_WINDOWS_API_
	ppath += ".dll";
    #endif
    #ifdef _IRR_OSX_PLATFORM_
    ppath += ".dylib";
    #endif
    #ifdef _IRR_LINUX_PLATFORM_
    ppath += ".so";
    #endif

    #ifdef _IRR_WINDOWS_API_
	HINSTANCE hdll = NULL;
    #else
    void *hdll = NULL;
    #endif
	cp3d::core::ISSWELibPlugin* newPlugin = NULL;
	typedef void* (*pvFunctv)();
	pvFunctv createSSWELibPlugin;
    #ifdef _IRR_WINDOWS_API_
	hdll = LoadLibrary(stringw(ppath).c_str());
    #else
    hdll = dlopen(stringc(ppath).c_str(), RTLD_LAZY);
    #endif
	
	if (hdll == NULL) {
		devices->addErrorDialog(L"Plugin Error", stringw(ppath + L"\nAn error occured\nCannot load the SSWE plugin").c_str(), EMBF_OK);
	} else {
        #ifdef _IRR_WINDOWS_API_
		createSSWELibPlugin = reinterpret_cast < pvFunctv > (GetProcAddress(hdll, "createSSWELibPlugin"));
        #else
        createSSWELibPlugin = reinterpret_cast < pvFunctv > (dlsym(hdll, "createSSWELibPlugin"));
        #endif
		if (createSSWELibPlugin == NULL) {
			devices->addErrorDialog(L"Plugin Error", stringw(ppath + L"\nAn error occured\nCannot find the process \"createSSWELibPlugin\" into the DLL").c_str(), EMBF_OK);
		} else {
			newPlugin = static_cast < cp3d::core::ISSWELibPlugin* > (createSSWELibPlugin());

            SSSWEPlugin sswePlugin(newPlugin, hdll);
            devices->getCoreData()->getSSWEPlugins()->push_back(sswePlugin);

			newPlugin->setDevices(devices);
			newPlugin->setWorkingDirectory(devices->getWorkingDirectory() + "Plugins/SSWE/");
			newPlugin->open();
		}
	}
}

void CPluginsManager::loadAudioPlugin(stringc path) {

    #ifdef _IRR_WINDOWS_API_
	HINSTANCE hdll = NULL;
    #else
    void *hdll = NULL;
    #endif
	cp3d::audio::IAudioManager* newPlugin = NULL;
	typedef void* (*pvFunctv)();
	pvFunctv createAudioManager;
    #ifdef _IRR_WINDOWS_API_
	hdll = LoadLibrary(stringw(path).c_str());
    #else
    stringc ppath = path;
    hdll = dlopen(ppath.c_str(), RTLD_LAZY);
    #endif

	if (hdll == NULL) {
        
		devices->addErrorDialog(L"Plugin Error", stringw(path + L"\nAn error occured\nCannot load the SSWE plugin").c_str(), EMBF_OK);
		#ifndef _IRR_WINDOWS_API_
		printf("\n%s\n", dlerror());
        devices->getDevice()->getLogger()->log(stringc(dlerror()).c_str());
		#endif
        
	} else {
        
        #ifdef _IRR_WINDOWS_API_
		createAudioManager = reinterpret_cast < pvFunctv > (GetProcAddress(hdll, "createAudioManager"));
        #else
        createAudioManager = reinterpret_cast < pvFunctv > (dlsym(hdll, "createAudioManager"));
        #endif
        
		if (createAudioManager == NULL) {
			#ifndef _IRR_WINDOWS_API_
            printf("\n%s\n", dlerror());
			#endif
			devices->addErrorDialog(L"Plugin Error", stringw(path + L"\nAn error occured\nCannot find the process \"createSSWELibPlugin\" into the DLL").c_str(), EMBF_OK);
		} else {
			newPlugin = static_cast < cp3d::audio::IAudioManager* > (createAudioManager());

			if (!newPlugin->initialize()) {
				devices->addErrorDialog(L"Plugin Error", stringw(path + L"\nAn error occured\nLoaded but cannot initialize the audio plugin").c_str(), EMBF_OK);
                return;
            }

			SAudioPlugin audioPlugin("testaudio", hdll);
			audioPlugin.setAudioManager(newPlugin);

			devices->getCoreData()->getAudioPlugins()->push_back(audioPlugin);
		}
        
	}
}
