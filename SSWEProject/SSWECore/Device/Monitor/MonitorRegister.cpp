#include "stdafx.h"
#include "MonitorRegister.h"


MonitorRegister::MonitorRegister()
{
	m_register = new std::vector<IMonitor*>();
}


MonitorRegister::~MonitorRegister()
{
	delete m_register;
}

void MonitorRegister::registerMonitor(IMonitor *monitor)
{
	m_register->push_back(monitor);
}
