//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_GUI_TIME_LINE_H_INCLUDED__
#define __C_GUI_TIME_LINE_H_INCLUDED__

#include "../../Device/CDevices.h"

namespace irr {
namespace gui {

class CGUITimeLine : public IGUIElement {

public:

	//---------------------------------------------------------------------------------------------
    //-----------------------------------------GUI ELEMENT-----------------------------------------
    //---------------------------------------------------------------------------------------------

	//! constructor
    CGUITimeLine(irr::IrrlichtDevice *_devices, IGUIElement* parent,
                 s32 id, core::rect<s32> rectangle, bool noclip=false);

	//! destructor
    virtual ~CGUITimeLine();

    //! called if an event happened.
    virtual bool OnEvent(const SEvent& event);

	//! draws the element and its children
    virtual void draw();
    
    void setRelativePosition(const core::rect<s32>& r);

	//---------------------------------------------------------------------------------------------
    //-----------------------------------------METHODS---------------------------------------------
    //---------------------------------------------------------------------------------------------

	//! Sets time to draw in the timeline (in sec)
	void setLength(s32 seconds);

	//! Sets the current zoom on the timeline (0-20) where 10 is equivalent of normal zoom
	void setZoom(s32 interval);

private:

	//-----------------------------------
	//GUI ELEMENTS
    IGUIScrollBar *scrollBar;
    //-----------------------------------

	//-----------------------------------
	//DATAS
    irr::IrrlichtDevice *device;
    bool clip;

	s32 length, zoom;
    //-----------------------------------

};
        
        
}
}

#endif