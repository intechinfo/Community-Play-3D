//
//  CExporter.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#ifndef __C_GENERIC_MONITOR_H_INCLUDED__
#define __C_GENERIC_MONITOR_H_INCLUDED__

#include <irrlicht.h>

#include <IMonitor.h>
#include <DLLExport.h>

class CGenericMonitor : public IMonitor {
public:

	CGenericMonitor();
	~CGenericMonitor();

	//-----------------------------------
	//IMONITOR
	void init();
	void destroy() {}

	void setName(irr::core::stringc name) { monitorName = name; }
	irr::core::stringc getName() { return monitorName; }

	void setActiveCamera(irr::scene::ICameraSceneNode *camera);
	irr::scene::ICameraSceneNode* getActiveCamera() { return monitorCamera; }

	void setSceneManager(irr::scene::ISceneManager *sceneManager);
	void setToolsSceneManager(irr::scene::ISceneManager *sceneManager) { effectSmgr = sceneManager; }
	irr::scene::ISceneManager* getSceneManager() { return smgr; }

	void setGUIEnvironment(irr::gui::IGUIEnvironment *guiEnv) { gui = guiEnv; }
	irr::gui::IGUIEnvironment* getGUIEnvironment() { return gui; }

	bool isEnabled() { return enabled; }
	void setEnable(bool enable) { enabled = enable; }

	void setXEffectRendered(bool enable) { renderXEffect = enable; }
	bool isXEffectRendered() { return renderXEffect; }
	void setRenderingXEffectFullTraitement(bool enable) { renderFullPostTraitements = enable; }
	bool isRenderingXEffectFullTraitement() { return renderFullPostTraitements; }
	void setXEffect(EffectHandler *xEffect) { effect = xEffect; }
	EffectHandler* getXEffect() { return effect; }

	void drawScene();
	void renderXEffectFullPostTraitement(irr::video::ITexture *texture);
	void drawGUI();
	//-----------------------------------

private:

	//-----------------------------------
	//RENDERS
	irr::video::IVideoDriver *driver;
	irr::scene::ISceneManager *smgr, *effectSmgr;
	irr::gui::IGUIEnvironment *gui;
	EffectHandler *effect;
	//-----------------------------------

	//-----------------------------------
	//SCENE NODES
	irr::scene::ICameraSceneNode *monitorCamera;
	//-----------------------------------

	//-----------------------------------
	//VALUES
	irr::core::stringc monitorName;
	bool enabled;

	bool renderXEffect;
	bool renderFullPostTraitements;
	//-----------------------------------

};

#endif
