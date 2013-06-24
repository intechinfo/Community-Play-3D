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

IMonitor* MonitorRegister::getMonitor(int index)
{
	return m_register->at(index);
}

IMonitor* MonitorRegister::getMonitorByName(std::string name)
{
	for(int i = 0; i < m_register->size(); i++)
	{
		if(m_register->at(i)->getName() == name)
		{
			return m_register->at(i);
			break;
		}
		else if(i == m_register->size()-1)
		{
			std::string errorString = "ERROR: No monitor named: \"" + name + "\" found !";
			fprintf(stderr, errorString.c_str());
			return NULL;
		}
	}
}

std::vector<IMonitor*> MonitorRegister::getRegisteredMonitors()
{
	return *m_register;
}

int MonitorRegister::getMonitorCount()
{
	return m_register->size();
}