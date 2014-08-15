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

class CGUISoundViewer : public IGUIElement {

public:

	//! constructor
    CGUISoundViewer(cp3d::audio::IAudioElement *element, irr::IrrlichtDevice *device, IGUIElement* parent,
					core::rect<s32> rectangle, bool noclip=false);

	//! destructor
    ~CGUISoundViewer();

    //! called if an event happened.
    bool OnEvent(const SEvent& event);

	//! draws the element and its children
    void draw();

	void setAudioElement(cp3d::audio::IAudioElement *element);

private:

	//-----------------------------------
	//DATAS
    IrrlichtDevice *Device;
    bool Clip;

	f32 *Spectrum; /// Spectrum array to draw

	cp3d::audio::IAudioElement *AudioElement;

	IGUIScrollBar *timeline;
    //-----------------------------------

};
        
        
}
}

#endif