//
//  CCoreGUI.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 25/09/12.
//
//

#include "stdafx.h"
#include "CCoreUserInterface.h"

#include "../../UserInterfaces/CodeEditor/CUICodeEditor.h"

CCoreUserInterface::CCoreUserInterface() {
    
    //-----------------------------------
    //DEVICE

    #ifdef _IRR_OSX_PLATFORM_
    params.DriverType=EDT_OPENGL;
        //params.WindowSize = dimension2d<u32>(1920, 800); // For see The XCode Debug Window
        params.WindowSize = dimension2d<u32>(1920, 1070);
        //params.WindowSize = dimension2d<u32>(1280, 690);
    #else
	params.DriverType=EDT_DIRECT3D9;
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
	
	if (params.Fullscreen) {
		IrrlichtDevice *tempDevice = createDevice(EDT_NULL);
		params.WindowSize = tempDevice->getVideoModeList()->getDesktopResolution();
		tempDevice->closeDevice();
		tempDevice->drop();
	}

	devices = new CDevices();
	devices->createDevice(params);
    
    driver = devices->getVideoDriver();

    smgr = devices->getSceneManager();
    gui = devices->getGUIEnvironment();
    
    windowSize = devices->getVideoDriver()->getScreenSize();

	#ifdef SSWE_RELEASE
		devices->getDevice()->getLogger()->setLogLevel(ELL_NONE);
	#else
		devices->getDevice()->getLogger()->setLogLevel(ELL_NONE);
	#endif

    //-----------------------------------

    //-----------------------------------
    //USER INTERFACE OBJECTS

    contextMenuInstance = new CUIContextMenu(devices);
	rightSceneTreeViewInstance = new CUIRightSceneTreeView(devices, contextMenuInstance);
	windowsManagerInstance = new CUIWindowsManager(devices);

    //-----------------------------------
    
    //-----------------------------------
    //GUI SKIN

    IGUISkin *skin_window_classic = gui->createSkin(EGST_WINDOWS_CLASSIC);
	IGUIFont *FontM = gui->getFont("GUI/fontlucida.png");
    skin_window_classic->setFont(FontM);
    gui->setSkin(skin_window_classic);
    
	programmersImage = gui->addImage(driver->getTexture("GUI/ss_logo.png"), position2d<s32>(0, driver->getScreenSize().Height-16));
    
    skin_window_classic->setColor(EGDC_BUTTON_TEXT, SColor(255,0xAA,0xAA,0xAA));
	skin_window_classic->setColor(EGDC_3D_HIGH_LIGHT, SColor(255,0x22,0x22,0x22));
	skin_window_classic->setColor(EGDC_3D_FACE, SColor(255,0x44,0x44,0x44));
	skin_window_classic->setColor(EGDC_WINDOW, SColor(255,0x66,0x66,0x66));
	skin_window_classic->setColor(EGDC_EDITABLE, SColor(255,0x66,0x66,0x66));
	skin_window_classic->setColor(EGDC_INACTIVE_CAPTION, SColor(255,0,0,0));

	skin_window_classic->getSpriteBank()->addTexture(driver->getTexture("GUI/error.png"));
	skin_window_classic->getSpriteBank()->addTexture(driver->getTexture("GUI/warning.png"));
	skin_window_classic->getSpriteBank()->addTexture(driver->getTexture("GUI/info.png"));

    //-----------------------------------
    
    //-----------------------------------
    //LOG CONSOLE

    logWindow =  gui->addWindow(rect<s32>(0, 0, 320, 520), false, L"Log Window", 0, -1);
    logWindow->getMaximizeButton()->setVisible(true);
    logWindow->getCloseButton()->remove();
    logListBox = gui->addListBox(rect<s32>(0, 20, 320, 470), logWindow, -1, true);
    logListBox->setAutoScrollEnabled(true);

    logLevel = gui->addComboBox(rect<s32>(10, 480, 200, 510), logWindow, -1);
    logLevel->addItem(L"ELL_INFORMATION");
    logLevel->addItem(L"ELL_WARNING");
    logLevel->addItem(L"ELL_ERROR");
    logLevel->addItem(L"ELL_NONE");
	logLevel->setSelected(3);

    clear = gui->addButton(rect<s32>(207, 480, 307, 510), logWindow, -1, L"Clear", L"Clear The Console");

    logVisible = false;
    logWindow->setVisible(logVisible);

    //-----------------------------------

    devices->getEventReceiver()->AddEventReceiver(this);
    devices->getEventReceiver()->AddEventReceiver(contextMenuInstance);
	devices->getEventReceiver()->AddEventReceiver(rightSceneTreeViewInstance);
	devices->getEventReceiver()->AddEventReceiver(windowsManagerInstance);

	//stringw test = "test de texte \n";
	//CUICodeEditor *c = new CUICodeEditor(devices, &test, true);
	//c->setAutoSave(true);
}

CCoreUserInterface::~CCoreUserInterface() {

}

void CCoreUserInterface::update() {
    devices->updateDevice();

    contextMenuInstance->update();
	windowsManagerInstance->update();
	devices->getProcessesLogger()->update();

    programmersImage->setRelativePosition(position2di(0, driver->getCurrentRenderTargetSize().Height-97));

    if (logWindow) {
        if (logWindow->getRelativePosition().getHeight() == 520) {
            logWindow->setRelativePosition(position2di(driver->getScreenSize().Width-320, driver->getScreenSize().Height-520));
        } else {
            logWindow->setRelativePosition(position2di(driver->getScreenSize().Width-1000, driver->getScreenSize().Height-600));
        }
    }
}

bool CCoreUserInterface::OnEvent(const SEvent &event) {
    
    if (devices->getVideoDriver()->getScreenSize() != windowSize) {
        windowSize = devices->getVideoDriver()->getScreenSize();
    }
    
    if (event.EventType == EET_GUI_EVENT) {
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            if (event.GUIEvent.Caller == contextMenuInstance->getConsoleButton()) {
                logVisible = !logVisible;
                logWindow->setVisible(logVisible);
            }
            
            if (event.GUIEvent.Caller == clear) {
                logListBox->clear();
            }
            
            if (event.GUIEvent.Caller == logWindow->getMaximizeButton()) {
                if (logWindow->getRelativePosition().getHeight() == 520) {
                    logWindow->setRelativePosition(rect<s32>(0, 0, 1000, 600));
                    logListBox->setRelativePosition(rect<s32>(0, 20, 980, 560));
                    logLevel->setRelativePosition(rect<s32>(10, 570, 200, 590));
                    clear->setRelativePosition(rect<s32>(880, 570, 980, 590));
                } else {
                    logWindow->setRelativePosition(rect<s32>(0, 0, 320, 520));
                    logListBox->setRelativePosition(rect<s32>(0, 20, 320, 470));
                    logLevel->setRelativePosition(rect<s32>(10, 480, 200, 510));
                    clear->setRelativePosition(rect<s32>(207, 480, 307, 510));
                }
            }
        }

        if (event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED) {
            if (event.GUIEvent.Caller == logLevel) {
                devices->getDevice()->getLogger()->setLogLevel((ELOG_LEVEL)logLevel->getSelected());
            }
        }
    }
    
    if (event.EventType == EET_LOG_TEXT_EVENT && logListBox) {
		if (event.LogEvent.Text) {
			stringw text = event.LogEvent.Text;
			logListBox->addItem(text.c_str());
			logListBox->setSelected(logListBox->getItemCount());
		}
    }

    if (event.EventType == EET_KEY_INPUT_EVENT) {
		if (!event.KeyInput.PressedDown) {
			if (event.KeyInput.Key == KEY_ESCAPE) {
				devices->getDevice()->closeDevice();
			}

			if (event.KeyInput.Key == KEY_KEY_F && devices->isCtrlPushed() && devices->isShiftPushed()) {
				if (!devices->isEditBoxEntered()) {
					CUIEditFPSCamera *editFPS = new CUIEditFPSCamera(devices);
				}
			}
        
			if (event.KeyInput.Key == KEY_KEY_M && devices->isCtrlPushed() && devices->isShiftPushed()) {
				if (!devices->isEditBoxEntered()) {
					devices->getSceneManager()->setActiveCamera(devices->getMayaCamera());
					devices->getDevice()->getCursorControl()->setVisible(true);
					devices->getCursor()->setVisible(true);
				}
			}

			if (event.KeyInput.Key == KEY_KEY_G && devices->isCtrlPushed() && devices->isShiftPushed()) {
				devices->setRenderGUI(!devices->isRenderingGUI());
			}
		}
    }
    
    return false;
}

