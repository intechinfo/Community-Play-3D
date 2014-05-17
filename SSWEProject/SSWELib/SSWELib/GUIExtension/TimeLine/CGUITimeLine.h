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

	//! Returns the end time of the timeline (total duration) in milliseconds
	u32 getTimeEnd() { return timeEnd; }
	//! Sets the end time of the time line (total duration) in milliseconds
	void setTimeEnd(u32 time);

	//! Sets the current zoom on the timeline (0-20) where 0 shows the entire timeline
	void setZoom(s32 interval);

private:

	//-----------------------------------
	//METHODS
	void updateView();
    //-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
    IGUIScrollBar *scrollBar;

	IGUIButton *zoomPlus, *zoomMinus;
    //-----------------------------------

	//-----------------------------------
	//DATAS
    irr::IrrlichtDevice *device;
    bool clip;

	/// View Datas
	s32 zoom; //current zoom in range [0-20] where 0 is the complete timeline
	u32 start, end; //start and end view in milliseconds

	u32 timeEnd; //Total time of the limeline
    //-----------------------------------

};
        
        
}
}

#endif