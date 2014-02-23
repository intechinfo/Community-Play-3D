/*
 *  UIContextMenu.h
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __C_UI_PARTICLES_EDIT_ZONE_H_INCLUDED__
#define __C_UI_PARTICLES_EDIT_ZONE_H_INCLUDED__

#include "../../Device/CDevices.h"

class CUIParticlesEditZone : public IEventReceiver {

public:

	CUIParticlesEditZone(CDevices *_devices, SPK::Emitter *_emitter, IGUIElement *parent);
	~CUIParticlesEditZone();

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
	//DEVICE & DATAS
	CDevices *devices;

	SPK::Emitter *emitter;

	enum E_ZONE_TYPE {
		EZT_AABOX = 0,
		EZT_CYLINDER,
		EZT_LINE,
		EZT_PLANE,
		EZT_POINT,
		EZT_RING,
		EZT_SPHERE,
		EZT_UNKNOWN
	} zoneType;
	array<stringw> zoneTypeNames;

	struct SVector3DParams {
		IGUIEditBox *x;
		IGUIEditBox *y;
		IGUIEditBox *z;
	};
	struct S2Params {
		IGUIEditBox *param1;
		IGUIEditBox *param2;
	};
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
    IGUIWindow *window;

	IGUIComboBox *zonecb;
	IGUIEditBox *positionx, *positiony, *positionz;
	IGUIButton *accept, *close;

	array<IGUIElement *> elements;
	array<IGUIElement *> otherElements;
	//-----------------------------------

	//-----------------------------------
	//METHODS
    stringc getZonePrefix();

	E_ZONE_TYPE getZoneType();
	void createExternalGUIElements();

	SVector3DParams create3DParams(stringw name, s32 relativePositionElementY);
	S2Params create2Params(stringw name1, stringw name2, s32 relativePositionElementY);
	IGUIElement *create1Param(stringw name, s32 relativePositionElementY);
	//-----------------------------------

};

#endif
