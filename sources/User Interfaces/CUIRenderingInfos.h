//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_RENDERING_INFOS_H_INCLUDED__
#define __C_UI_RENDERING_INFOS_H_INCLUDED__

#include "../Device/CDevices.h"

class CUIRenderingInfos : public IEventReceiver {
    
public:
    
    CUIRenderingInfos(CDevices *_devices);
    ~CUIRenderingInfos();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    CDevices *devices;
    
    //-----------------------------------
    //GUI ELEMENTS
    IGUIWindow *window;
    IGUIStaticText *fps, *driver, *resolution;
    IGUIButton *close;
    //-----------------------------------
    
};

#endif
