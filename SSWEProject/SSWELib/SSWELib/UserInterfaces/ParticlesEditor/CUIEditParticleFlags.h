//
//  CUIEditParticleFlags.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/01/2014.
//
//

#ifndef __C_UI_PARTICLE_EDITOR_INTFLAGS_H_INCLUDED__
#define __C_UI_PARTICLE_EDITOR_INTFLAGS_H_INCLUDED__

#include "../../Device/CDevices.h"

class CUIParticleEditorFlags : public IEventReceiver {
public:
    
    CUIParticleEditorFlags(CDevices *_devices, SPK::Model *_model, IGUIElement *parent, array<stringw> &modelParams);
    ~CUIParticleEditorFlags();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    //-----------------------------------
	//METHODS
    void fillWindow(u32 currenty, IGUIElement *element);
    //-----------------------------------
    
    //-----------------------------------
	//DEVICE && DATAS
    CDevices *devices;

	SPK::Model *model;
    //-----------------------------------
    
    //-----------------------------------
	//GUI ELEMENTS
    IGUIWindow *window;
    array<IGUIElement *> paramsElements;
    //-----------------------------------
    
};

#endif
