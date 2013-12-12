#ifndef __C_UI_PLUGINS_MANAGER_H_INCLUDED__
#define __C_UI_PLUGINS_MANAGER_H_INCLUDED__

#include "../../Device/CDevices.h"
#include "CPluginsManager.h"

class CUIPluginsManager : public IEventReceiver {

public:

	CUIPluginsManager(CDevices *_devices, CPluginsManager *_pluginsManager);
	~CUIPluginsManager();

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
    //DEVICE AND DATAS
	CDevices *devices;
	CPluginsManager *pluginsManager;
	//-----------------------------------

	//-----------------------------------
    //GUI ELEMENTS
	IGUIWindow *window;
	IGUITabControl *tabcrtl;

	IGUITab *monitorsTab;
	IGUIListBox *monitors;
	IGUICheckBox *enableMonitor;

	IGUITab *sswetab;
	IGUIListBox *ssweplugins;
	IGUIButton *runssweplugin;

	IGUIButton *close;
	//-----------------------------------
};

#endif
