//
//  CCoreGUI.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 25/09/12.
//
//

#include "CCoreUserInterface.h"

CCoreUserInterface::CCoreUserInterface() {
    
    //-----------------------------------
    //DEVICE
    SIrrlichtCreationParameters params;
	params.DriverType=EDT_OPENGL;
    #ifdef _IRR_OSX_PLATFORM_
        //params.WindowSize = dimension2d<u32>(1920, 800); // For see The XCode Debug Window
        params.WindowSize = dimension2d<u32>(1920, 1070);
        //params.WindowSize = dimension2d<u32>(1280, 690);
    #else
        params.WindowSize = dimension2d<u32>(800, 600);
    #endif
        params.Bits=32;
    #ifdef _IRR_OSX_PLATFORM_
        params.Fullscreen = false;
    #else
        params.Fullscreen = false;
    #endif
	params.Stencilbuffer=true;
	params.Vsync=false;
	params.AntiAlias=true; 
    params.ZBufferBits = 32;
	params.EventReceiver=0;
	
	devices = new CDevices();
	devices->createDevice(params);
    devices->getEventReceiver()->AddEventReceiver(this);
    
    driver = devices->getVideoDriver();
    smgr = devices->getSceneManager();
    gui = devices->getGUIEnvironment();
    //-----------------------------------
    
    //-----------------------------------
    //USER INTERFACE OBJECTS
    contextMenuInstance = new CUIContextMenu(devices);
    devices->getEventReceiver()->AddEventReceiver(contextMenuInstance);
    //-----------------------------------
    
    //-----------------------------------
    //GUI SKIN
    IGUISkin *skin_window_classic = gui->createSkin(EGST_WINDOWS_CLASSIC);
	IGUIFont *FontM = gui->getFont("GUI/fontlucida.png");
    skin_window_classic->setFont(FontM);
    gui->setSkin(skin_window_classic);
    
	programmersImage = gui->addImage(driver->getTexture("GUI/programmer.png"), position2d<s32>(0, driver->getScreenSize().Height-16));
    
    skin_window_classic->setColor(EGDC_BUTTON_TEXT, SColor(255,0xAA,0xAA,0xAA));
	skin_window_classic->setColor(EGDC_3D_HIGH_LIGHT, SColor(255,0x22,0x22,0x22));
	skin_window_classic->setColor(EGDC_3D_FACE, SColor(255,0x44,0x44,0x44));
	skin_window_classic->setColor(EGDC_WINDOW, SColor(255,0x66,0x66,0x66));
    //-----------------------------------
    
}

CCoreUserInterface::~CCoreUserInterface() {
    
}

void CCoreUserInterface::update() {
    devices->updateDevice();
    
    contextMenuInstance->update();
    
    programmersImage->setRelativePosition(position2di(0, driver->getCurrentRenderTargetSize().Height-16));
}

bool CCoreUserInterface::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_KEY_INPUT_EVENT) {
        if (event.KeyInput.Key == KEY_ESCAPE) {
            devices->getDevice()->closeDevice();
        }
        
        if (event.KeyInput.Key == KEY_KEY_M) {
            if (!devices->isEditBoxEntered()) {
                devices->getSceneManager()->setActiveCamera(devices->getMayaCamera());
            }
        }
    }
    
    if (event.EventType == EET_GUI_EVENT) {
        if (event.GUIEvent.EventType == EGET_EDITBOX_MARKING_CHANGED) {
            devices->setEditBoxEntered(true);
            devices->getObjectPlacement()->setAllowMoving(false);
        }
    }
    
    if (event.EventType == EET_MOUSE_INPUT_EVENT) {
        if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN) {
            devices->setEditBoxEntered(false);
            devices->getObjectPlacement()->setAllowMoving(true);
        }
    }
    
    return false;
}

