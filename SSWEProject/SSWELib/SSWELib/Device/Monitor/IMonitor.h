#ifndef IMONITOR_H
#define IMONITOR_H

#include <iostream>
#include <irrlicht.h>
#include "Renders/XEffect/XEffects.h"

class IMonitor
{
public:

	virtual void init() = 0;

	virtual void setName(irr::core::stringc name) = 0;
	virtual irr::core::stringc getName() = 0;

	virtual bool isEnabled() = 0;
	virtual void setEnable(bool enable) = 0;
	virtual void setXEffectRendered(bool enable) = 0;
	virtual bool isXEffectRendered() = 0;
	virtual void setRenderingXEffectFullTraitement(bool enable) = 0;
	virtual bool isRenderingXEffectFullTraitement() = 0;

	virtual void setActiveCamera(irr::scene::ICameraSceneNode *camera) =0;
	virtual irr::scene::ICameraSceneNode* getActiveCamera() = 0;
	virtual void setSceneManager(irr::scene::ISceneManager *sceneManager) =0;
	virtual irr::scene::ISceneManager* getSceneManager() = 0;
	virtual void setToolsSceneManager(irr::scene::ISceneManager *sceneManager) = 0;
	virtual void setGUIEnvironment(irr::gui::IGUIEnvironment *guiEnv) =0;
	virtual irr::gui::IGUIEnvironment* getGUIEnvironment() = 0;
	virtual void setXEffect(EffectHandler *xEffect) = 0;
	virtual EffectHandler* getXEffect() = 0; 


	virtual void drawScene() = 0;
	virtual void renderXEffectFullPostTraitement(irr::video::ITexture *texture) = 0;
	virtual void drawGUI() = 0;
};

//! ARRAY HOLDING THE EXISTING METHODS INTO INonitor INTERFACE
const char* const sIMonitorMethodsNames[] = {
	"init",

	"setName",
	"getName",

	"isEnabled",
	"setEnable",

	"setXEffectRendered",
	"isXEffectRendered",

	"setRenderingXEffectFullTraitement",
	"isRenderingXEffectFullTraitement",

	"setActiveCamera",
	"getActiveCamera",

	"setSceneManager",
	"getSceneManager",
	"setToolsSceneManager",

	"setGUIEnvironment",
	"getGUIEnvironment",

	"setXEffect",
	"getXEffect",

	"drawScene",
	"renderXEffectFullPostTraitement",

	0
};

#endif
