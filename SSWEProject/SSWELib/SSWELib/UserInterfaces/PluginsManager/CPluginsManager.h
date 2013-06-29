#ifndef __C_PLUGINS_MANAGER_H_INCLUDED__
#define __C_PLUGINS_MANAGER_H_INCLUDED__

#include "../../Device/CDevices.h"

class CPluginsManager {

public:

	CPluginsManager(CDevices *_devices);
	~CPluginsManager();

	array<stringc> getAllMonitorsPluginsNames();

	void loadMonitorPlugin(stringc path);

private:

	CDevices *devices;

};

#endif
