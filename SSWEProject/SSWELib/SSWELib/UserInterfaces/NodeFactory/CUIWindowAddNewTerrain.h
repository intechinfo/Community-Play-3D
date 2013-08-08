//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_WINDOW_ADD_NEW_TERRAIN_H_INCLUDED__
#define __C_UI_WINDOW_ADD_NEW_TERRAIN_H_INCLUDED__

#include "../../Device/CDevices.h"

class CUIWindowAddNewTerrain : public IEventReceiver {

public:

	CUIWindowAddNewTerrain(CDevices *_devices);
	~CUIWindowAddNewTerrain();

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
	//DEVICES ELEMENTS AND DATAS
	CDevices *devices;
	//-----------------------------------

	//-----------------------------------
	//METHODS
	ITexture *noise(bool colored, s32 w, s32 h, double zoom, s32 octaves, f32 persistance, s32 r, s32 g, s32 b);
	double interpolateNoise(int seed, double x, double y);
	double smoothNoise(int seed, double x,double y);
	double findNoise(int seed, double x,double y);
	double interpolate(double a,double b,double x);
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	IGUIWindow *window;

	IGUIEditBox *sizeeb;
	IGUICheckBox *randomHillscb;

	IGUIButton *accept, *close;
	//-----------------------------------

};

#endif