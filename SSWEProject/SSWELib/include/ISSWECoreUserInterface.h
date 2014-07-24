#ifndef __I_CORE_USER_INTERFACE_H_INCLUDED__
#define __I_CORE_USER_INTERFACE_H_INCLUDED__

#include <irrlicht.h>

#include <ISSWEImporter.h>
#include <IDevices.h>
#include <IMonitor.h>

namespace cp3d {

class ISSWECoreUserInterface {
public: 

	virtual ISSWEImporter *createImporter() = 0;
	
	virtual core::IDevices *getIDevices() = 0;

	virtual video::IMonitor *addMonitor(irr::core::stringc path) = 0;

	virtual void setLogEventWindowVisible(bool visible) = 0;

};

} /// End namespace cp3d

#endif
