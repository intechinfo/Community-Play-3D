/* defined(__MacOSX__CUIWindowAddObject__) *///
//  CUIWindowAddObject.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 27/09/12.
//
//

#ifndef __C_UI_WINDOW_ADD_LIGHT_H_INCLUDED__
#define __C_UI_WINDOW_ADD_LIGHT_H_INCLUDED__

#include "../../Device/CDevices.h"

enum GUI_WINDOW_EVENTS_ADD_LIGHT {
    CXT_WINDOW_ADD_LIGHT_EVENTS_CLOSE = 0x70000,
    CXT_WINDOW_ADD_LIGHT_EVENTS_ACCEPT
};

class CUIWindowAddLight : public IEventReceiver {
    
public:
    
    CUIWindowAddLight(CDevices *_devices, IGUIListBox *_lightsListBox);
    ~CUIWindowAddLight();
    
    void open();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    CDevices *devices;
    
    IGUIListBox *lightsListBox;
    
    IGUIWindow *addLightWindow;
    IGUIEditBox *addLightEditBox;
    
};

#endif
