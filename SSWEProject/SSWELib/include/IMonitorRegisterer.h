#ifndef IMONITORREGISTERER_H
#define IMONITORREGISTERER_H

#include "IMonitor.h"

namespace cp3d {
namespace video {

class IMonitorRegisterer {

public:

	virtual void registerMonitor(IMonitor *monitor) = 0;

	virtual void unregisterMonitor(int index) = 0;
	virtual void unregisterMonitorByName(irr::core::stringc name) =0;

	virtual IMonitor* getMonitor(int index) = 0;
	virtual IMonitor* getMonitorByName(irr::core::stringc name) = 0;

	virtual irr::core::array<IMonitor*> getRegisteredMonitors() = 0;
	virtual int getMonitorCount() = 0;

};

} /// End namespace video
} /// End namespace cp3d

#endif