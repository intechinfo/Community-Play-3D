#include "Stdafx.h"

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

}

CGUISpectrum::~CGUISpectrum() {
	delete[] Spectrum;

	//IGUIElement::~IGUIElement();
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

	if (AudioElement) {

		AudioElement->getSpectrum(Spectrum, SPECTRUM_SIZE);

		s32 height = AbsoluteRect.LowerRightCorner.Y - AbsoluteRect.UpperLeftCorner.Y;

		Environment->getSkin()->getFont()->draw(Name, core::rect<s32>(AbsoluteRect.UpperLeftCorner.X+5, AbsoluteRect.UpperLeftCorner.Y,
																	  AbsoluteRect.LowerRightCorner.X, AbsoluteRect.UpperLeftCorner.Y+50),
									 video::SColor(255, 255, 255, 255));

		video::IVideoDriver *driver = Environment->getVideoDriver();

		for (u32 i=0; i < 127; i++) {
			core::vector2di begin(AbsoluteRect.UpperLeftCorner.X+i*4, AbsoluteRect.LowerRightCorner.Y-height/2 - s32((height/2)*Spectrum[i]));
			core::vector2di end(AbsoluteRect.UpperLeftCorner.X+(i+1)*4, AbsoluteRect.LowerRightCorner.Y-height/2 - s32((height/2)*Spectrum[i+1]));

			driver->draw2DLine(begin, end, video::SColor(255, 255, 255*(s32)Spectrum[i], 255*(s32)Spectrum[i]));
		}

	}

	/// End
	IGUIElement::draw();
}

void CGUISpectrum::setAudioElement(cp3d::audio::IAudioElement *element) {
	AudioElement = element;
}

} /// End namespace gui
} /// End namespace irr
