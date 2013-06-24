#ifndef MONITORREGISTER_H
#define MONITORREGISTER_H

#include "IMonitorRegisterer.h"
#include <vector>

class MonitorRegister : public IMonitorRegisterer
{
public:
	MonitorRegister();
	~MonitorRegister();

	void registerMonitor(IMonitor *monitor);
	void unregisterMonitor(int index);
	void unregisterMonitorByName(irr::core::stringc name);
	IMonitor* getMonitor(int index);
	IMonitor* getMonitorByName(irr::core::stringc name);
	irr::core::array<IMonitor*> getRegisteredMonitors();
	int getMonitorCount();

private:
	irr::core::array<IMonitor*> *m_register;
};
#endif
