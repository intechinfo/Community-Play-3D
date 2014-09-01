#include "stdafx.h"

#include "CGUISoundViewer.h"

#define SPECTRUM_SIZE 512

namespace irr {
namespace gui {

CGUISoundViewer::CGUISoundViewer(cp3d::audio::IAudioElement *element, IrrlichtDevice *device, IGUIElement* parent,
								 core::rect<s32> rectangle, bool noclip)
	: IGUIElement(EGUIET_ELEMENT, device->getGUIEnvironment(), parent, -1, rectangle)
{
	#ifdef _DEBUG
	setDebugName("CGUINodesEditor");
	#endif
	setNotClipped(noclip);

	/// Configure class
    Clip = noclip;
    Device = device;

	/// Create gui components
	timeline = Environment->addScrollBar(true, core::rect<s32>(0, rectangle.getHeight()-15, rectangle.getWidth(), rectangle.getHeight()), this, -1);

	/// This element cannot be tabbed.
	setTabStop(false);
	setTabOrder(-1);

	/// Create other elements
	Spectrum = new f32[SPECTRUM_SIZE];
	AudioElement = 0;

}

CGUISoundViewer::~CGUISoundViewer() {
	delete[] Spectrum;

	//IGUIElement::~IGUIElement();
}

bool CGUISoundViewer::OnEvent(const SEvent& event) {
	if (event.EventType == EET_GUI_EVENT && event.GUIEvent.EventType == EGET_SCROLL_BAR_CHANGED) {
		if (event.GUIEvent.Caller == timeline) {
			if (AudioElement)
				AudioElement->setCurrentTime(timeline->getPos());
		}
	}

	return Parent ? Parent->OnEvent(event) : false;
}

void CGUISoundViewer::draw() {
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

		timeline->setPos(AudioElement->getCurrentTime());

		AudioElement->getSpectrum(Spectrum, SPECTRUM_SIZE);

		//------------------------------------------------------------------------------------
		/// Get Time
		u32 currentTimeMs = AudioElement->getCurrentTime(); /// ms
		u32 currentTimeS = currentTimeMs / 1000; /// sec
		u32 currentTimeMin = currentTimeS / 60;
		u32 currentTimeH = currentTimeMin / 60;

		if (currentTimeS >= 60)
			currentTimeS -= 60 * currentTimeMin;

		core::stringw time = core::stringw(currentTimeH) + "h " + core::stringw(currentTimeMin) + "min "
			+ core::stringw(currentTimeS) + "sec ";

		Environment->getSkin()->getFont()->draw(time, core::rect<s32>(AbsoluteRect.UpperLeftCorner.X+5, AbsoluteRect.UpperLeftCorner.Y,
																	  AbsoluteRect.LowerRightCorner.X, AbsoluteRect.UpperLeftCorner.Y+50),
									 video::SColor(255, 255, 255, 255));
		//------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------
		/// Draw Spectrum
		video::IVideoDriver *driver = Environment->getVideoDriver();

		s32 height = AbsoluteRect.LowerRightCorner.Y - AbsoluteRect.UpperLeftCorner.Y;

		for (u32 i=0; i < 127; i++) {
			core::vector2di begin(AbsoluteRect.UpperLeftCorner.X+i*4, AbsoluteRect.LowerRightCorner.Y-height/2 - s32((height/2)*Spectrum[i]));
			core::vector2di end(AbsoluteRect.UpperLeftCorner.X+(i+1)*4, AbsoluteRect.LowerRightCorner.Y-height/2 - s32((height/2)*Spectrum[i+1]));

			driver->draw2DLine(begin, end, video::SColor(255, 255, 255*(s32)Spectrum[i], 255*(s32)Spectrum[i]));
		}
		//------------------------------------------------------------------------------------

	}

	/// End
	IGUIElement::draw();
}

void CGUISoundViewer::setAudioElement(cp3d::audio::IAudioElement *element) {
	AudioElement = element;

	if (AudioElement)
		timeline->setMax(AudioElement->getDuration());
}

} /// End namespace gui
} /// End namespace irr
