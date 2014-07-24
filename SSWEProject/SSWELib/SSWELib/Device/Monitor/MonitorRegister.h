#ifndef MONITORREGISTER_H
#define MONITORREGISTER_H

#include "IMonitorRegisterer.h"

class MonitorRegister : public cp3d::video::IMonitorRegisterer
{
public:
	MonitorRegister();
	~MonitorRegister();

	void registerMonitor(cp3d::video::IMonitor *monitor);
	void unregisterMonitor(int index);
	void unregisterMonitorByName(irr::core::stringc name);
	cp3d::video::IMonitor* getMonitor(int index);
	cp3d::video::IMonitor* getMonitorByName(irr::core::stringc name);
	irr::core::array<cp3d::video::IMonitor*> getRegisteredMonitors();
	int getMonitorCount();

private:
	irr::core::array<cp3d::video::IMonitor*> *m_register;
};
#endif
