//
//  CUIEditEffects.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 20/11/12.
//
//

#ifndef __C_UI_WINDOW_EDIT_FILTERS_H_INCLUDED__
#define __C_UI_WINDOW_EDIT_FILTERS_H_INCLUDED__

#include "../../../Device/CDevices.h"

#include "../../../GUIExtension/ViewPort/CGUIViewPort.h"
#include "../../../UserInterfaces/CodeEditor/CUICodeEditor.h"

class CCoreFilterCallback;

class CUIWindowEditFilters : public IEventReceiver {

public:

	CUIWindowEditFilters(CDevices *_devices);
	~CUIWindowEditFilters();

	void open();

	bool OnEvent(const SEvent &event);

private:

	CDevices *devices;
	CCoreFilterCallback *fcallback;

	//-----------------------------------
	//METHODS
	void reloadFilters();

	void createLogWindow();
	//-----------------------------------

	//-----------------------------------
	//MAIN WINDOW
	IGUIWindow *window;

	CGUIViewport *viewPort;
	IGUICheckBox *useVSM;
	IGUICheckBox *enableDepthPass;

	IGUIListBox *filters;
	IGUIButton *editFilter, *editCallback;
	IGUIButton *addFilter, *removeFilter;
	IGUIEditBox *filterName;
	IGUIButton *openLogger;

	IGUIButton *close;

	//OTHERS
	IGUIContextMenu *menu;
	IGUIFileOpenDialog *openShader;

	//LOGGER
	IGUIWindow *loggerWindow;
	IGUIEditBox *logEditBox;
	IGUIButton *closeLogWindow;
	//-----------------------------------

};

#endif
