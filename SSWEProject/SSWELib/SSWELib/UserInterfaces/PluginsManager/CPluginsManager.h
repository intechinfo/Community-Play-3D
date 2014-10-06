#ifndef __C_PLUGINS_MANAGER_H_INCLUDED__
#define __C_PLUGINS_MANAGER_H_INCLUDED__

#include "../../Device/CDevices.h"

class CDevelopmentPluginManager;

#include <DLLExport.h>

class CPluginsManager {

public:

    /// Ctor & dtor
	CPluginsManager(CDevices *_devices);
	~CPluginsManager();

	array<stringc> getAllMonitorsPluginsNames();
	array<stringc> getAllSSWEPluginsNames();

    /// External plugins (monitors, loaders, etc.)
	void loadMonitorPlugin(stringc path);
	void loadSSWEPlugin(stringc path);
	void loadAudioPlugin(stringc path);
    
    void loadDevelopmentInstance(stringc path);
    void unloadDevelopmentInstance();
    void reloadDevelopmentInstance();
    
    /// Development instances
    cp3d::core::IDevelomentInstance *getDevClassInstance() { return devClassInstance; }
    stringc getDevelopmentInstancePath() { return developmentInstancePath; }
    CDevelopmentPluginManager *getDevelopmentInstanceManager() { return devInstanceManager; }

private:

    /// Datas
	CDevices *devices;
    
    /// Development instances
    CDevelopmentPluginManager *devInstanceManager;
    cp3d::core::IDevelomentInstance *devClassInstance;
    #ifdef _IRR_WINDOWS_API_
    HINSTANCE developmentInstance;
    #else
    void *developmentInstance;
    #endif
    stringc developmentInstancePath;

};

#endif
