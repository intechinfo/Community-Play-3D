//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_LIGHT_MAPS_CREATOR_H_INCLUDED__
#define __C_UI_LIGHT_MAPS_CREATOR_H_INCLUDED__

#include "../../Device/CDevices.h"

class CRadiosityGenerator;

class CUILightsMapCreator : public IEventReceiver {

public:

	CUILightsMapCreator(CDevices *_devices, SData *data);
	~CUILightsMapCreator();

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
    //DEVICE AND DATAS
	CDevices *devices;
	SData *data;

	CRadiosityGenerator *myNode;
	//-----------------------------------

	//-----------------------------------
    //METHODS
	void log(stringw text);
	void buildLightMaps();
	//-----------------------------------

	//-----------------------------------
    //GUI ELEMENTS
	IGUIWindow *window;

	IGUIEditBox *samplingFactoreb;
	IGUIEditBox *occlusionAccuracyeb;
	IGUIEditBox *diffuseSmootheb;
	IGUIEditBox *occlusionAngleeb;

	IGUIEditBox *bounceseb;
	IGUIEditBox *accuracyeb;

	IGUIButton *closeb, *calculateb;

	CGUIEditBoxIRB *logeb;
	//-----------------------------------

};

#endif