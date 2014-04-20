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

#include "../../Device/Core/ParticleSystems/CParticleSystemsTypes.h"

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
	SPK::Zone *existedZone, *currentZone;

	array<stringw> zoneTypeNames;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
    IGUIWindow *window;
	IGUIComboBox *zonecb;
	IGUIButton *accept, *close;
	IGUIStaticText *editZone;

	IGUIEditBox *v1_1, *v1_2, *v1_3;
	IGUIEditBox *v2_1, *v2_2, *v2_3;
	IGUIEditBox *eb1, *eb2, *eb3;
	//-----------------------------------

	//-----------------------------------
	//METHODS
	void addVector3D1(stringw name);
	void addVector3D2(stringw name);
	void addTwoParameters(stringw name1, stringw name2, bool addedVector1, bool addedVector2);

	void modifyVector1(SPK::Vector3D v);
	void modifyVector2(SPK::Vector3D v);
	void modifyTwoParameters(float f1, float f2);
	//-----------------------------------

};

#endif
