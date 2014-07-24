#include "stdafx.h"
#include "MonitorRegister.h"


MonitorRegister::MonitorRegister()
{
	m_register = new irr::core::array<cp3d::video::IMonitor*>();
}


MonitorRegister::~MonitorRegister()
{
	delete m_register;
}

void MonitorRegister::registerMonitor(cp3d::video::IMonitor *monitor)
{
	m_register->push_back(monitor);
}

void MonitorRegister::unregisterMonitor(int index)
{
	//delete m_register->operator[](index);
	m_register->operator[](index)->destroy();
	m_register->operator[](index) = NULL;
	m_register->erase(index);
}
	
void MonitorRegister::unregisterMonitorByName(irr::core::stringc name)
{
	for(int i = 0; i < m_register->size(); i++)
	{
		if(m_register->operator[](i)->getName() == name)
		{
			delete m_register->operator[](i);
			m_register->erase(i);
			break;
		}
	}
}

cp3d::video::IMonitor* MonitorRegister::getMonitor(int index)
{
	return m_register->operator[](index);
}

cp3d::video::IMonitor* MonitorRegister::getMonitorByName(irr::core::stringc name)
{
	for(int i = 0; i < m_register->size(); i++)
	{
		if(m_register->operator[](i)->getName() == name)
		{
			return m_register->operator[](i);
			break;
		}
		else if(i == m_register->size()-1)
		{
			irr::core::stringc errorString = irr::core::stringc("ERROR: No monitor named: \"") + name + irr::core::stringc("\" found !");
			fprintf(stderr, errorString.c_str());
			return NULL;
		}
	}
}

irr::core::array<cp3d::video::IMonitor*> MonitorRegister::getRegisteredMonitors()
{
	return *m_register;
}

int MonitorRegister::getMonitorCount()
{
	return m_register->size();
}