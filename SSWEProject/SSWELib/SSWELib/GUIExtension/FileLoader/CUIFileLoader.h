//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_FILE_LOADER_H_INCLUDED__
#define __C_UI_FILE_LOADER_H_INCLUDED__

#include "../../Device/CDevices.h"

class CUIFileLoader : public IEventReceiver {
    
public:
    
    CUIFileLoader(CDevices *_devices);
    ~CUIFileLoader();
    
    IGUIFileOpenDialog *create(stringw name, CGUIFileSelector::E_FILESELECTOR_TYPE type, IGUIElement *parent, bool modal);
    
    bool OnEvent(const SEvent &event);
    
    //GUI ELEMENTS
    IGUIWindow *getWindow() { return window; }
    
private:
    
    //-----------------------------------
	//DEVICE
    CDevices *devices;
    //-----------------------------------
    
    //-----------------------------------
	//GUI ELEMENTS
    IGUIWindow *window;
    
    CGUIFileSelector *selector;
    s32 normalWidth;
    
    IGUIImage *preview;
    IGUIStaticText *infostxt;
    
    IGUIButton *accept, *close;
    //-----------------------------------
    
    //-----------------------------------
	//DATAS
    bool removeOldTexture;
    ITexture *oldTexture;
    //-----------------------------------
    
};

#endif