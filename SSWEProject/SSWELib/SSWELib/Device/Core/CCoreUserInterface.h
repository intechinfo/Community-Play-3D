//
//  CCoreGUI.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 25/09/12.
//
//

#ifndef __C_CORE_GUI_H_INCLUDED__
#define __C_CORE_GUI_H_INCLUDED__

#include "../CDevices.h"

#include "../../UserInterfaces/UIContextMenu.h"
#include "../../UserInterfaces/CUIRightSceneTreeView.h"
#include "../../UserInterfaces/CUIWindowsManager.h"

class CCoreUserInterface : public IEventReceiver {

public:

	CCoreUserInterface();
	~CCoreUserInterface();

	//-----------------------------------
	//DEVICE
	CDevices *getDevices() { return devices; }
	IVideoDriver *getVideoDriver() { return driver; }
	ISceneManager *getSceneManager() { return smgr; }
	IGUIEnvironment *getGUIEnvironment() { return gui; }
	//-----------------------------------

	//-----------------------------------
	//METHODS
	bool OnEvent(const SEvent &event);

	void update();

	void setLogEventWindowVisible(bool visible) { logVisible = visible; }
	bool isLogEventWindowVisible() { return logVisible; }
	//-----------------------------------

private:

	//-----------------------------------
	//DEVICE
	SIrrlichtCreationParameters params;

	CDevices *devices;
	IVideoDriver *driver;
	ISceneManager *smgr;

	IGUIEnvironment *gui;
	IGUIImage *programmersImage;

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
	//-----------------------------------

	//-----------------------------------
	//METHODS

	//-----------------------------------

};

#endif
