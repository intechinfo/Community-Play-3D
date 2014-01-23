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

class CCGenericMonitor : public IMonitor {
public:

    CCGenericMonitor();
    ~CCGenericMonitor();

	//-----------------------------------
	//IMONITOR
    void init();
	void destroy() { }

	void setActiveCamera(irr::scene::ICameraSceneNode *camera);
	irr::scene::ICameraSceneNode* getActiveCamera() { return monitorCamera; }

	void setSceneManager(irr::scene::ISceneManager *sceneManager);
	irr::scene::ISceneManager* getSceneManager() { return smgr; }

	void setGUIEnvironment(irr::gui::IGUIEnvironment *guiEnv) { gui = guiEnv; }
	irr::gui::IGUIEnvironment* getGUIEnvironment() { return gui; }

	void drawScene();
	void drawGUI();
	//-----------------------------------

private:

	//-----------------------------------
	//RENDERS
	irr::video::IVideoDriver *driver;
	irr::scene::ISceneManager *smgr;
	irr::gui::IGUIEnvironment *gui;
	//-----------------------------------

	//-----------------------------------
	//SCENE NODES
	irr::scene::ICameraSceneNode *monitorCamera;
	//-----------------------------------

};

#endif
