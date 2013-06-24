#pragma once

#include "IMonitor.h"

class IMonitorRegisterer
{
	virtual void registerMonitor(IMonitor *monitor) = 0;
};