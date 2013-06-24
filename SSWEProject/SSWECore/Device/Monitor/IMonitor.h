#pragma once

#include <iostream>
#include "Device\CDevices.h"

class IMonitor
{
public:

	virtual void init() = 0;

	virtual void setName(std::string name) = 0;
	virtual std::string getName() = 0;

	virtual void setActiveCamera(ICameraSceneNode *camera) =0;
	virtual void setSceneManager(ISceneManager *sceneManager) =0;


	virtual void drawScene() = 0;
	virtual void drawGUI() = 0;
};

