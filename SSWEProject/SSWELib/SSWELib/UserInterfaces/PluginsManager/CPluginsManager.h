#ifndef __C_PLUGINS_MANAGER_H_INCLUDED__
#define __C_PLUGINS_MANAGER_H_INCLUDED__

#include "../../Device/CDevices.h"

class CPluginsManager {

public:

	CPluginsManager(CDevices *_devices);
	~CPluginsManager();

	array<stringc> getAllMonitorsPluginsNames();
	array<stringc> getAllSSWEPluginsNames();

	void loadMonitorPlugin(stringc path);
	void loadSSWEPlugin(stringc path);

private:

	CDevices *devices;

};

#endif
