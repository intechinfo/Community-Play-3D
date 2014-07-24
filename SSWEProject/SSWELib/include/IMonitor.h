#ifndef IMONITOR_H
#define IMONITOR_H

#include <irrlicht.h>

#include <ISSWERender.h>

namespace cp3d {
namespace video {

class IMonitor
{
public:

	IMonitor() : renderFull(false), renderRenderer(false), enabled(true), monitorName("New Monitor")
	{
		
	}

	virtual void init() = 0;
	virtual void destroy() = 0;

	void setName(irr::core::stringc name) {
		monitorName = name;
	}
	irr::core::stringc getName() {
		return monitorName;
	}

	bool isEnabled() {
		return enabled;
	}
	void setEnable(bool enable) {
		enabled = enable;
	}

	void setRenderingFullTraitement(bool enable) {
		renderFull = enable;
	}
	bool isRenderingFullTraitement() {
		return renderFull;
	}

	void setRenderRenderer(bool enable) {
		renderRenderer = enable;
	}
	bool isRenderingRenderer() {
		return renderRenderer;
	}

	//CAMERA
	virtual void setActiveCamera(irr::scene::ICameraSceneNode *camera) =0;
	virtual irr::scene::ICameraSceneNode* getActiveCamera() = 0;

	//SCENE MANAGER
	virtual void setSceneManager(irr::scene::ISceneManager *sceneManager) =0;
	virtual irr::scene::ISceneManager* getSceneManager() = 0;

	//GUI
	virtual void setGUIEnvironment(irr::gui::IGUIEnvironment *guiEnv) =0;
	virtual irr::gui::IGUIEnvironment* getGUIEnvironment() = 0;

	void setRender(ISSWERender *_renderer) {
		renderer = _renderer;
	}
	ISSWERender* getRender() {
		return renderer;
	}


	virtual void drawScene() = 0;
	virtual void drawGUI() = 0;

protected:
	ISSWERender *renderer;

	bool renderFull;
	bool renderRenderer;
	bool enabled;

	irr::core::stringc monitorName;
};

} /// End namespace video
} /// End namespace cp3d

//! ARRAY HOLDING THE EXISTING METHODS INTO INonitor INTERFACE
/*const char* const sIMonitorMethodsNames[] = {
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
	"drawGUI",

	0
};*/

#endif
