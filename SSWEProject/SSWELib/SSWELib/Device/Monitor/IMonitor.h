#ifndef IMONITOR_H
#define IMONITOR_H

#include <iostream>
#include <irrlicht.h>

class IMonitor
{
public:

	virtual void init() = 0;

	virtual void setName(std::string name) = 0;
	virtual std::string getName() = 0;

	virtual void setActiveCamera(irr::scene::ICameraSceneNode *camera) =0;
	virtual void setSceneManager(irr::scene::ISceneManager *sceneManager) =0;

	virtual void drawScene() = 0;
	virtual void drawGUI() = 0;
};
#endif
