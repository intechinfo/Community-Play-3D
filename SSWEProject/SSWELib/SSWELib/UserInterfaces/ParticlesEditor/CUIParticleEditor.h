/*
 *  UIContextMenu.h
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __C_UI_PARTICLE_EDITOR_H_INCLUDED__
#define __C_UI_PARTICLE_EDITOR_H_INCLUDED__

#include "../../Device/CDevices.h"

#include "../../GUIExtension/NodesEditor/CGUINodesEditor.h"

class CUIParticleEditor : public IEventReceiver {

public:

	CUIParticleEditor(CDevices *_devices, SParticleSystem *_ps);
	~CUIParticleEditor();

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
	//DEVICE
	CDevices *devices;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
    IGUIWindow *window;
    
    CGUINodesEditor *nodesEditor;
	//-----------------------------------

	//-----------------------------------
	//DATAS
    SParticleSystem *ps;
    
    enum E_PS_DATA_TYPE {
        EPSDT_SYSTEM = 0,
        EPSDT_GROUP,
        EPSDT_EMITTER,
        EPSDT_RENDERER,
        EPSDT_MODEL
    };
	//-----------------------------------

};

#endif
