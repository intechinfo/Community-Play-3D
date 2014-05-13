#include "stdafx.h"

#include "CGUITimeLine.h"

namespace irr {
namespace gui {

CGUITimeLine::CGUITimeLine(IrrlichtDevice *_devices, IGUIElement* parent, s32 id, core::rect<s32> rectangle, bool noclip)
	: IGUIElement(EGUIET_ELEMENT, _devices->getGUIEnvironment(), parent, id, rectangle)
{
	#ifdef _DEBUG
	setDebugName("CGUINodesEditor");
	#endif
	setNotClipped(noclip);
    
    clip = noclip;
    device = _devices;

	// This element can be tabbed.
	setTabStop(false);
	setTabOrder(-1);

	IGUISkin* skin = Environment->getSkin();
	s32 s = skin->getSize( EGDS_SCROLLBAR_SIZE );

	scrollBar = Environment->addScrollBar(true, core::rect<s32>(0, RelativeRect.getHeight() - s, RelativeRect.getWidth(), RelativeRect.getHeight()), this, 1);
    scrollBar->setSubElement(true);
    scrollBar->setPos( 0 );

	zoomMinus = Environment->addButton(rect<s32>(RelativeRect.getWidth()-20, RelativeRect.getHeight() - s - 20,
												 RelativeRect.getWidth(), RelativeRect.getHeight() - s),
									   this, -1, L"", L"Zoom less on timeline");
	zoomMinus->setImage(Environment->getVideoDriver()->getTexture("GUI/zoom_remove.png"));
	zoomMinus->setScaleImage(true);
	zoomMinus->setUseAlphaChannel(true);

	zoomPlus = Environment->addButton(rect<s32>(RelativeRect.getWidth()-40, RelativeRect.getHeight() - s - 20,
												RelativeRect.getWidth()-20, RelativeRect.getHeight() - s),
									  this, -1, L"", L"Zoom less on timeline");
	zoomPlus->setImage(Environment->getVideoDriver()->getTexture("GUI/zoom_add.png"));
	zoomPlus->setScaleImage(true);
	zoomPlus->setUseAlphaChannel(true);
}

CGUITimeLine::~CGUITimeLine() {

}

bool CGUITimeLine::OnEvent(const SEvent& event) {

	if (event.EventType == EET_GUI_EVENT) {

		/// Button clicked
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == zoomPlus) {
				setZoom(zoom + 1);
				return true;
			} else
			if (event.GUIEvent.Caller == zoomMinus) {
				setZoom(zoom - 1);
				return true;
			}
		}

	}

	return Parent ? Parent->OnEvent(event) : false;
}

void CGUITimeLine::draw() {
	if( !IsVisible )
	{
		return;
	}

	IGUISkin* skin = Environment->getSkin();
	irr::video::IVideoDriver* driver = Environment->getVideoDriver();
    
	core::rect<s32>* clipRect = 0;
	if (clip)
	{
		clipRect = &AbsoluteClippingRect;
	}
    
	// draw background
	core::rect<s32> frameRect( AbsoluteRect );
    
    driver->draw2DRectangle( skin->getColor( EGDC_3D_HIGH_LIGHT ), frameRect,
                            clipRect );
    
	// draw the border
	frameRect.LowerRightCorner.Y = frameRect.UpperLeftCorner.Y + 1;
	driver->draw2DRectangle( skin->getColor( EGDC_3D_SHADOW ), frameRect,
                            clipRect );
    
	frameRect.LowerRightCorner.Y = AbsoluteRect.LowerRightCorner.Y;
	frameRect.LowerRightCorner.X = frameRect.UpperLeftCorner.X + 1;
	driver->draw2DRectangle( skin->getColor( EGDC_3D_SHADOW ), frameRect,
                            clipRect );
    
	frameRect = AbsoluteRect;
	frameRect.UpperLeftCorner.X = frameRect.LowerRightCorner.X - 1;
	driver->draw2DRectangle( skin->getColor( EGDC_3D_HIGH_LIGHT ), frameRect,
                            clipRect );
    
	frameRect = AbsoluteRect;
	frameRect.UpperLeftCorner.Y = AbsoluteRect.LowerRightCorner.Y - 1;
	frameRect.LowerRightCorner.Y = AbsoluteRect.LowerRightCorner.Y;
	driver->draw2DRectangle( skin->getColor( EGDC_3D_HIGH_LIGHT ), frameRect,
                            clipRect );
    
    
	// draw items
	core::rect<s32> clientClip( AbsoluteRect );
	clientClip.UpperLeftCorner.Y += 1;
	clientClip.UpperLeftCorner.X += 1;
	clientClip.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X;
	clientClip.LowerRightCorner.Y -= 1;
    
	if ( scrollBar )
		clientClip.LowerRightCorner.Y -= skin->getSize( EGDS_SCROLLBAR_SIZE );

	if( clipRect )
	{
		clientClip.clipAgainst( *clipRect );
	}

	frameRect = AbsoluteRect;
	frameRect.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X - skin->getSize( EGDS_SCROLLBAR_SIZE );
    
	if ( scrollBar )
	{
		frameRect.UpperLeftCorner.X -= scrollBar->getPos();
		frameRect.LowerRightCorner.X -= scrollBar->getPos();
	}

	/// Draw timeline
	frameRect = AbsoluteClippingRect;
    frameRect.UpperLeftCorner.X += 1;
    frameRect.UpperLeftCorner.Y += 1;

	driver->draw2DLine(core::vector2d<s32>(frameRect.UpperLeftCorner.X, frameRect.UpperLeftCorner.Y + frameRect.getHeight() / 2 - skin->getSize(EGDS_SCROLLBAR_SIZE)),
                       core::vector2d<s32>(frameRect.UpperLeftCorner.X + frameRect.getWidth(), frameRect.UpperLeftCorner.Y + frameRect.getHeight() / 2 - skin->getSize(EGDS_SCROLLBAR_SIZE)),
                       video::SColor(255, 255, 255, 255));

	/// Draw time
	u32 steps = 5;
	core::rect<s32> rect;
	for (u32 i=0; i < steps + 1; i++) {
		/// Calculate frame rect
		rect = frameRect;
		rect.UpperLeftCorner.X = frameRect.UpperLeftCorner.X + (AbsoluteRect.getWidth() / (s32)steps) * i;
		rect.LowerRightCorner.X = rect.UpperLeftCorner.X + (AbsoluteRect.getWidth() / (s32)steps) + (AbsoluteRect.getWidth() / (s32)steps) * i;

		if (rect.LowerRightCorner.X > AbsoluteRect.LowerRightCorner.X)
			rect.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X;

		/// Write current secs (FIX with ms)
		stringw txt = "";
		txt += (30 / steps) * i;
		txt += " secs";
		Environment->getSkin()->getFont()->draw(txt, rect, video::SColor(255, 196, 196, 196));

		/// Draw separators
		driver->draw2DLine(core::vector2di(rect.UpperLeftCorner.X, rect.UpperLeftCorner.Y),
						   core::vector2di(rect.UpperLeftCorner.X, rect.LowerRightCorner.Y - skin->getSize(EGDS_SCROLLBAR_SIZE)),
						   video::SColor(255, 64, 64, 64));
	}

	/// End
	IGUIElement::draw();
}

void CGUITimeLine::setRelativePosition(const core::rect<s32>& r) {
    IGUIElement::setRelativePosition(r);
    
    IGUISkin* skin = Environment->getSkin();
    s32 s = skin->getSize( EGDS_SCROLLBAR_SIZE );
    
    scrollBar->setRelativePosition(core::rect<s32>(0, RelativeRect.getHeight() - s, RelativeRect.getWidth(), RelativeRect.getHeight()));
	zoomMinus->setRelativePosition(rect<s32>(RelativeRect.getWidth()-20, RelativeRect.getHeight() - s - 20, RelativeRect.getWidth(), RelativeRect.getHeight() - s));
	zoomPlus->setRelativePosition(rect<s32>(RelativeRect.getWidth()-40, RelativeRect.getHeight() - s - 20, RelativeRect.getWidth()-20, RelativeRect.getHeight() - s));
}

void CGUITimeLine::setTimeEnd(u32 time) {

}

void CGUITimeLine::setZoom(s32 interval) {
	if (interval > 20)
		interval = 20;

	zoom = interval;
}

void CGUITimeLine::updateView() {
	/// zoom
	/// current view
	/// time end


}

} /// End namespace gui
} /// End namespace irr