#ifndef __I_CORE_USER_INTERFACE_H_INCLUDED__
#define __I_CORE_USER_INTERFACE_H_INCLUDED__

#include <irrlicht.h>

#include <ISSWEImporter.h>
#include <IDevices.h>
#include <IMonitor.h>

class ISSWECoreUserInterface {
public: 

	virtual ISSWEImporter *createImporter() = 0;
	
	virtual IDevices *getIDevices() = 0;

	virtual IMonitor *addMonitor(irr::core::stringc path) = 0;

	virtual void setLogEventWindowVisible(bool visible) = 0;

};

#endif
