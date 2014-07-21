//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_GUI_SPECTRUM_H_INCLUDED__
#define __C_GUI_SPECTRUM_H_INCLUDED__

#include <irrlicht.h>
#include <IAudioElement.h>

namespace irr {
namespace gui {

class CGUISpectrum : public IGUIElement {

public:

	//! constructor
    CGUISpectrum(cp3d::audio::IAudioElement *element, irr::IrrlichtDevice *device, IGUIElement* parent,
                 s32 id, core::rect<s32> rectangle, bool noclip=false);

	//! destructor
    ~CGUISpectrum();

    //! called if an event happened.
    bool OnEvent(const SEvent& event);

	//! draws the element and its children
    void draw();

	void setAudioElement(cp3d::audio::IAudioElement *element);

private:

	//-----------------------------------
	//DATAS
    irr::IrrlichtDevice *Device;
    bool Clip;

	u32 LastTime;

	f32 *Spectrum; /// Spectrum array to draw

	cp3d::audio::IAudioElement *AudioElement;
    //-----------------------------------

};
        
        
}
}

#endif