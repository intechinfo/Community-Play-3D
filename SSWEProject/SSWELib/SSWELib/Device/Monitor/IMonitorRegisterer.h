#ifndef IMONITORREGISTERER_H
#define IMONITORREGISTERER_H

#include "IMonitor.h"

class IMonitorRegisterer
{
	virtual void registerMonitor(IMonitor *monitor) = 0;
	virtual IMonitor* getMonitor() =0;
	virtual IMonitor* getMonitorByName() = 0;
	virtual std::vector<IMonitor*> getRegisteredMonitors() = 0;
	virtual int getMonitorCount() = 0;
};
#endif