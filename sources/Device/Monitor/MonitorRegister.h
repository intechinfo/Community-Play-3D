#ifndef MONITORREGISTER_H
#define MONITORREGISTER

#include "IMonitorRegisterer.h"
#include <vector>

class MonitorRegister : public IMonitorRegisterer
{
public:
	MonitorRegister();
	~MonitorRegister();

	void registerMonitor(IMonitor *monitor);
	IMonitor* getMonitor(int index);
	IMonitor* getMonitorByName(std::string name);
	std::vector<IMonitor*> getRegisteredMonitors();
	int getMonitorCount();

private:
	std::vector<IMonitor*> *m_register;
};
#endif
