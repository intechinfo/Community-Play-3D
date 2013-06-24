#pragma once

#include "IMonitorRegisterer.h"
#include <vector>

class MonitorRegister : public IMonitorRegisterer
{
public:
	MonitorRegister();
	~MonitorRegister();

	void registerMonitor(IMonitor *monitor);

private:
	std::vector<IMonitor*> *m_register;
};

