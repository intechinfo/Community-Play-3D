//
//  CUIWindowAddOctTree.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 27/09/12.
//
//

#ifndef __C_UI_WINDOW_RENDER_H_INCLUDED__
#define __C_UI_WINDOW_RENDER_H_INCLUDED__

#include "../Device/CDevices.h"
#include "../GUIExtension/ViewPort/CGUIViewPort.h"

enum GUI_WINDOW_EVENTS_RENDER {
	GUI_WINDOW_EVENTS_RENDER_CLOSE = 0x16000,
	GUI_WINDOW_EVENTS_RENDER_SAVE
};

class CUIWindowRender : public IEventReceiver {

public:

	CUIWindowRender(CDevices *_devices);
	~CUIWindowRender();

	void open();

	bool OnEvent(const SEvent &event);

private:

	CDevices *devices;

	IGUIWindow *renderWindow;
	IGUIEditBox *saveAs;
	IGUICheckBox *shadowsE, *hdrE, *shadersE;

	IGUIViewport *viewPort;
};

#endif
