//
//  CUIEditParticleFlags.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/01/2014.
//
//

#ifndef __C_UI_PARTICLE_EDITOR_INTINTERPOLATORS_H_INCLUDED__
#define __C_UI_PARTICLE_EDITOR_INTINTERPOLATORS_H_INCLUDED__

#include "../../Device/CDevices.h"

class CUIParticleEditorInterpolators : public IEventReceiver {
public:
    
    CUIParticleEditorInterpolators(CDevices *_devices, SPK::Model *_model, IGUIElement *parent);
    ~CUIParticleEditorInterpolators();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    //-----------------------------------
	//METHODS

    //-----------------------------------
    
    //-----------------------------------
	//DEVICE && DATAS
    CDevices *devices;

	SPK::Model *model;
    //-----------------------------------
    
    //-----------------------------------
	//GUI ELEMENTS
    IGUIWindow *window;

    //-----------------------------------
    
};

#endif
