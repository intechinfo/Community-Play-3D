//
//  CUIWindowEditGrid.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 08/10/12.
//
//

#ifndef __C_UI_WINDOW_EDIT_GRID_H_INCLUDED__
#define __C_UI_WINDOW_EDIT_GRID_H_INCLUDED__

#include "../Device/CDevices.h"

enum CXT_EDIT_WINDOW_GRID_EVENTS {
    CXT_EDIT_WINDOW_GRID_EVENTS_ACCENT_OFFSET_EDITBOX = 0x11000,
    CXT_EDIT_WINDOW_GRID_EVENTS_SIZE_EDITBOX,
    CXT_EDIT_WINDOW_GRID_EVENTS_SPACING_EDITBOX,
    
    CXT_EDIT_WINDOW_GRID_EVENTS_CLOSE
};

class CUIWindowEditGrid : public IEventReceiver {
    
public:
    
    CUIWindowEditGrid(CDevices *_devices);
    ~CUIWindowEditGrid();
    
    void open();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    CDevices *devices;
    
    IGUIWindow *editGridWindow;
    IGUIEditBox *accentLineOffset, *size, *spacing;
    IGUICheckBox *axisLineActive;
    
};

#endif
