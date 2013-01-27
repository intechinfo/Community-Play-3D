//
//  CCoreGUI.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 25/09/12.
//
//

#ifndef __C_CORE_GUI_H_INCLUDED__
#define __C_CORE_GUI_H_INCLUDED__

#include "../CDevices.h"

#include "../../User Interfaces/UIContextMenu.h"

class CCoreUserInterface : public IEventReceiver {
    
public:
    
    CCoreUserInterface();
    ~CCoreUserInterface();
    
    //-----------------------------------
    //DEVICE
    CDevices *getDevices() { return devices; }
    IVideoDriver *getVideoDriver() { return driver; }
    ISceneManager *getSceneManager() { return smgr; }
    IGUIEnvironment *getGUIEnvironment() { return gui; }
    //-----------------------------------
    
    //-----------------------------------
    //METHODS
    bool OnEvent(const SEvent &event);
    
    void update();
    //-----------------------------------
    
private:
    
    //-----------------------------------
    //DEVICE
    CDevices *devices;
    IVideoDriver *driver;
    ISceneManager *smgr;
    
    IGUIEnvironment *gui;
    IGUIImage *programmersImage;
    //-----------------------------------
    
    //-----------------------------------
    //PROPERTIES
    CUIContextMenu *contextMenuInstance;
    //-----------------------------------
    
    //-----------------------------------
    //METHODS
    
    //-----------------------------------
    
};

#endif
