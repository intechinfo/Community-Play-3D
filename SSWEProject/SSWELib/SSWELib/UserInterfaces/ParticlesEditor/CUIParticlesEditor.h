/*
 *  UIContextMenu.h
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __C_UI_PARTICLES_EDITOR_H_INCLUDED__
#define __C_UI_PARTICLES_EDITOR_H_INCLUDED__

#include "../../Device/CDevices.h"

#include "CUIParticleEditor.h"

class CUIParticlesEditor : public IEventReceiver {

public:

	CUIParticlesEditor(CDevices *_devices);
	~CUIParticlesEditor();

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
	//DEVICE
	CDevices *devices;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
    IGUIWindow *window;
    IGUIButton *edit, *remove, *add;
    IGUIListBox *particleSystems;
    IGUICheckBox *update;
    IGUIEditBox *name;
	//-----------------------------------

	//-----------------------------------
	//DATAS
    
	//-----------------------------------

};

#endif
