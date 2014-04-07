//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_WINDOW_EDIT_CLOUDS_H_INCLUDED__
#define __C_UI_WINDOW_EDIT_CLOUDS_H_INCLUDED__

#include "../../Device/CDevices.h"

class CUIWindowEditClouds : public IEventReceiver {

public:

	CUIWindowEditClouds(CDevices *_devices);
	~CUIWindowEditClouds();

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
	//DEVICES ELEMENTS AND DATAS
	CDevices *devices;
	//-----------------------------------

	//-----------------------------------
	//METHODS
	void fillContents(CCloudSceneNode *node);
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	IGUIWindow *window;

	IGUITabControl *tabctrl;
	IGUITab *generalTab, *colorsTab;

	IGUIButton *addCloud, *removeCloud;
	IGUIListBox *clouds;

	IGUIEditBox *translationX, *translationY;
	IGUIEditBox *textureScale;
	IGUIEditBox *innerRadius, *outerRadius;
	IGUIEditBox *centerHeight, *innerHeight, *outerHeight;

	IGUIEditBox *centerColorRed, *centerColorGreen, *centerColorBlue, *centerColorAlpha;
	IGUIEditBox *innerColorRed, *innerColorGreen, *innerColorBlue, *innerColorAlpha;
	IGUIEditBox *outerColorRed, *outerColorGreen, *outerColorBlue, *outerColorAlpha;
	//-----------------------------------

};

#endif