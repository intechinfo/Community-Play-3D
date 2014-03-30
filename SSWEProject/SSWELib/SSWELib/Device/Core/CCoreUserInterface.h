//
//  CCoreGUI.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 25/09/12.
//
//

#ifndef __C_CORE_GUI_H_INCLUDED__
#define __C_CORE_GUI_H_INCLUDED__

#include <ISSWECoreUserInterface.h>

#include "../CDevices.h"

#include "../../UserInterfaces/UIContextMenu.h"
#include "../../UserInterfaces/CUIRightSceneTreeView.h"
#include "../../UserInterfaces/CUIWindowsManager.h"

class CCoreUserInterface : public IEventReceiver, public ISSWECoreUserInterface {

public:

	CCoreUserInterface(bool playOnly = false, irr::core::stringc argPath = "");
	~CCoreUserInterface();

	/// ISSWE Core User Interface
	ISSWEImporter *createImporter();
	IMonitor *addMonitor(irr::core::stringc path);

	//-----------------------------------
	//DEVICE
	CDevices *getDevices() { return devices; }
	IDevices *getIDevices() { return (IDevices*)devices; }
	IVideoDriver *getVideoDriver() { return driver; }
	ISceneManager *getSceneManager() { return smgr; }
	IGUIEnvironment *getGUIEnvironment() { return gui; }
	//-----------------------------------

	//-----------------------------------
	//METHODS
	bool OnEvent(const SEvent &event);

	void update();

	void setLogEventWindowVisible(bool visible);
	bool isLogEventWindowVisible() { return logVisible; }

	CUIMainWindow *getMainWindow() { return contextMenuInstance->getMainWindow(); }
	//-----------------------------------

private:

	//-----------------------------------
	//DEVICE
	SIrrlichtCreationParameters params;

	CDevices *devices;
	IrrlichtDevice *device;
	IVideoDriver *driver;
	ISceneManager *smgr;

	IGUIEnvironment *gui;

	IGUIWindow *logWindow;
	IGUIListBox *logListBox;
	IGUIComboBox *logLevel;
	IGUIButton *clear;
	bool logVisible;

	dimension2d<u32> windowSize;
	//-----------------------------------

	//-----------------------------------
	//PROPERTIES
	CUIContextMenu *contextMenuInstance;
	CUIRightSceneTreeView *rightSceneTreeViewInstance;
	CUIWindowsManager *windowsManagerInstance;

	CPluginsManager *pluginsManager;
	//-----------------------------------

	//-----------------------------------
	//METHODS

	//-----------------------------------

};

#endif
