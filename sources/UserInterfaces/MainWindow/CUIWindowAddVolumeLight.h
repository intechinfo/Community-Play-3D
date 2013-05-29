/* defined(__MacOSX__CUIWindowAddVolumeLight__) *///
//  CUIWindowAddVolumeLight.h
//  MacOSX
//
//  Created by Arthur Caron on 28/05/2013
//
//

#ifndef __C_UI_WINDOW_ADD_VOLUME_LIGHT_H_INCLUDED__
#define __C_UI_WINDOW_ADD_VOLUME_LIGHT_H_INCLUDED__

#include "../../Device/CDevices.h"

class CUIWindowAddVolumeLight : public IEventReceiver {
    
public:
    
    CUIWindowAddVolumeLight(CDevices *_devices, IGUIListBox *_lightsListBox);
    ~CUIWindowAddVolumeLight();
    
    void open();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    CDevices *devices;
    
    IGUIListBox *volumeLightsListBox;
    
    IGUIWindow *addVolumeLightWindow;
    IGUIEditBox *addVolumeLightEditBox;
    
	IGUIButton *acceptButton;
	IGUIButton *closeButton;
};

#endif
