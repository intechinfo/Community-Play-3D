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

enum E_PARTICLE_FLAG_TYPE {
    EPFT_ENABLED = 0,
    EPFT_MUTABLE,
    EPFT_RANDOM,
    EPFT_INTERPOLATED
};

class CUIParticleEditorFlags : public IEventReceiver {
public:
    
    CUIParticleEditorFlags(CDevices *_devices, SPK::Model *_model, E_PARTICLE_FLAG_TYPE _type, IGUIElement *parent, array<stringw> &modelParams);
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
    
    array<IGUIElement *> paramsElements;
    //-----------------------------------
    
    //-----------------------------------
	//GUI ELEMENTS
    
    IGUIWindow *window;
    
    //-----------------------------------
    
};

#endif
