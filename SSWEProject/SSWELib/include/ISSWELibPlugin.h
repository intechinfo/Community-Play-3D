#ifndef __I_SSWE_LIB_PLUGIN_H_INCLUDED__
#define __I_SSWE_LIB_PLUGIN_H_INCLUDED__

#include <irrlicht.h>
#include <IDevices.h>

class ISSWEPluginSystem {
public:
	ISSWEPluginSystem(IDevices *_devices) {
		devices = _devices;
	}
	virtual ~ISSWEPluginSystem() {

	}

	void remove() {

	}

private:
	IDevices *devices;
};

class ISSWELibPlugin {
public: 
	virtual void setDevices(IDevices *_devices) = 0;
	virtual IDevices *getDevices() = 0;

	virtual void open() = 0;
	virtual void close() = 0;
};

#endif
