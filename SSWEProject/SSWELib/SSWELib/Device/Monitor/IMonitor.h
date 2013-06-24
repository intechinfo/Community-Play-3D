#ifndef IMONITOR_H
#define IMONITOR_H

#include <iostream>
#include <irrlicht.h>

class IMonitor
{
public:

	virtual void init() = 0;

	virtual void setName(irr::core::stringc name) = 0;
	virtual irr::core::stringc getName() = 0;

	virtual void setActiveCamera(irr::scene::ICameraSceneNode *camera) =0;
	virtual void setSceneManager(irr::scene::ISceneManager *sceneManager) =0;

	virtual void drawScene() = 0;
	virtual void renderXEffectFullPostTraitement(irr::video::ITexture *texture) = 0;
	virtual void drawGUI() = 0;
};
#endif
