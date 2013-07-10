//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_SSWE_OPTIONS_H_INCLUDED__
#define __C_UI_SSWE_OPTIONS_H_INCLUDED__

#include "../../Device/CDevices.h"

#include "../../Scenes/CExporter.h"

struct SFPSKeyMap {
	IGUIButton *button;
	bool active;
};

class CUISSWEOptions : public IEventReceiver {

public:

	CUISSWEOptions(CDevices *_devices);
	~CUISSWEOptions();

	bool OnEvent(const SEvent &event);

private:

	CDevices *devices;

	//-----------------------------------
    //GUI ELEMENTS
	IGUIWindow *window;

	IGUITabControl *tabctrl;

	IGUITab *fpsTab;
	SFPSKeyMap fpsButtons[5];
	IGUIStaticText *fpsInfoText;
	//-----------------------------------

};

#endif