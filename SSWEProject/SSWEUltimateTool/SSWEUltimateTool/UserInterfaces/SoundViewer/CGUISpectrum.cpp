#include "stdafx.h"

#include "CGUISpectrum.h"

#define SPECTRUM_SIZE 512

namespace irr {
namespace gui {

CGUISpectrum::CGUISpectrum(cp3d::audio::IAudioElement *element, IrrlichtDevice *device, IGUIElement* parent, s32 id,
						   core::rect<s32> rectangle, bool noclip)
	: IGUIElement(EGUIET_ELEMENT, device->getGUIEnvironment(), parent, id, rectangle)
{
	#ifdef _DEBUG
	setDebugName("CGUINodesEditor");
	#endif
	setNotClipped(noclip);
    
    Clip = noclip;
    Device = device;

	// This element cannot be tabbed.
	setTabStop(false);
	setTabOrder(-1);

	Spectrum = new f32[SPECTRUM_SIZE];
	AudioElement = 0;

	LastTime = device->getTimer()->getTime();

}

CGUISpectrum::~CGUISpectrum() {
	delete[] Spectrum;

	IGUIElement::~IGUIElement();
}

bool CGUISpectrum::OnEvent(const SEvent& event) {
	return Parent ? Parent->OnEvent(event) : false;
}

void CGUISpectrum::draw() {
	if( !IsVisible )
	{
		return;
	}

	IGUISkin* skin = Environment->getSkin();
	irr::video::IVideoDriver* driver = Environment->getVideoDriver();
    
	core::rect<s32>* clipRect = 0;
	if (Clip)
	{
		clipRect = &AbsoluteClippingRect;
	}
    
	// draw background
	core::rect<s32> frameRect( AbsoluteRect );
    
    driver->draw2DRectangle( skin->getColor( EGDC_3D_HIGH_LIGHT ), frameRect,
                            clipRect );
    
	// draw items
	core::rect<s32> clientClip( AbsoluteRect );
	clientClip.UpperLeftCorner.Y += 1;
	clientClip.UpperLeftCorner.X += 1;
	clientClip.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X;
	clientClip.LowerRightCorner.Y -= 1;

	if( clipRect )
	{
		clientClip.clipAgainst( *clipRect );
	}

	if (AudioElement /*&& Device->getTimer()->getTime() - LastTime >= 25*/) {

		AudioElement->getSpectrum(Spectrum, SPECTRUM_SIZE);

		s32 height = AbsoluteRect.LowerRightCorner.Y - AbsoluteRect.UpperLeftCorner.Y;

		for (u32 i=0; i < SPECTRUM_SIZE-1; i++) {
			Environment->getVideoDriver()->draw2DLine(core::vector2di(AbsoluteRect.UpperLeftCorner.X+i, AbsoluteRect.LowerRightCorner.Y),
				core::vector2di(AbsoluteRect.UpperLeftCorner.X+i, AbsoluteRect.LowerRightCorner.Y - s32(height*Spectrum[i])),
				video::SColor(255, 255, 255*(s32)Spectrum[i], 255*(s32)Spectrum[i]));
		}

	}

	LastTime = Device->getTimer()->getTime();

	/// End
	IGUIElement::draw();
}

void CGUISpectrum::setAudioElement(cp3d::audio::IAudioElement *element) {
	AudioElement = element;
}

} /// End namespace gui
} /// End namespace irr