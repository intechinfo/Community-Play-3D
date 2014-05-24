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

#include "ParticleSystems/CParticleSystemsImporter.h"

CCoreUserInterface::CCoreUserInterface(bool playOnly, irr::core::stringc argPath) {
    
    //-----------------------------------
    //DEVICE

	params.WindowSize = dimension2d<u32>(0, 0);
    #ifdef _IRR_OSX_PLATFORM_
        params.DriverType=irr::video::EDT_OPENGL;
        //params.WindowSize = dimension2d<u32>(1920, 800); // For see The XCode Debug Window
        //params.WindowSize = dimension2d<u32>(1920, 1070);
        //params.WindowSize = dimension2d<u32>(1280, 690);
        params.WindowSize = dimension2du(1630, 1000);
    #else
		params.DriverType=EDT_DIRECT3D9;
        params.WindowSize = dimension2d<u32>(1680, 987);
    #endif
        params.Bits=32;
    #ifdef _IRR_OSX_PLATFORM_
        params.Fullscreen = false;
    #else
		if (!playOnly) {
			params.Fullscreen = false;
		} else {
			params.WindowSize = dimension2d<u32>(1280, 800);
			params.Fullscreen = false;
		}
    #endif
	params.WithAlphaChannel = true;
	params.Stencilbuffer=false;
	params.Vsync=false;
	params.AntiAlias=true;
    params.ZBufferBits = 32;
	params.EventReceiver=0;
	params.DriverMultithreaded = true;
	
	if (params.Fullscreen && params.WindowSize == dimension2d<u32>(0, 0)) {
		IrrlichtDevice *tempDevice = createDevice(EDT_NULL);
		params.WindowSize = tempDevice->getVideoModeList()->getDesktopResolution();
		tempDevice->closeDevice();
		tempDevice->drop();
	}

	devices = new CDevices(this, playOnly);
	devices->createDevice(params);
    
	device = devices->getDevice();
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

	pluginsManager = new CPluginsManager(devices);
	if (!playOnly) {
		contextMenuInstance = new CUIContextMenu(devices, pluginsManager);

		rightSceneTreeViewInstance = new CUIRightSceneTreeView(devices, contextMenuInstance);
		windowsManagerInstance = new CUIWindowsManager(devices);
	}

    //-----------------------------------
    
    //-----------------------------------
    //GUI SKIN

    IGUISkin *skin_window_classic = gui->createSkin(EGST_WINDOWS_CLASSIC);
	IGUIFont *FontM = gui->getFont("GUI/fontlucida.png");
    skin_window_classic->setFont(FontM);
    gui->setSkin(skin_window_classic);
    
    skin_window_classic->setColor(EGDC_BUTTON_TEXT, SColor(255,0xAA,0xAA,0xAA));
	skin_window_classic->setColor(EGDC_3D_HIGH_LIGHT, SColor(255,0x22,0x22,0x22));
	skin_window_classic->setColor(EGDC_3D_FACE, SColor(255,0x44,0x44,0x44));
	skin_window_classic->setColor(EGDC_WINDOW, SColor(255,0x66,0x66,0x66));
	skin_window_classic->setColor(EGDC_EDITABLE, SColor(255,0x66,0x66,0x66));
	skin_window_classic->setColor(EGDC_INACTIVE_CAPTION, SColor(255,0,0,0));
	//skin_window_classic->setColor(EGDC_HIGH_LIGHT, SColor(255, 65, 29, 65));
	skin_window_classic->setColor(EGDC_3D_LIGHT, SColor(255, 32, 32, 32));

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

	logWindow->setVisible(false);

    //-----------------------------------

    devices->getEventReceiver()->AddEventReceiver(this);
	if (!playOnly) {
		devices->getEventReceiver()->AddEventReceiver(contextMenuInstance);
		devices->getEventReceiver()->AddEventReceiver(rightSceneTreeViewInstance);
		devices->getEventReceiver()->AddEventReceiver(windowsManagerInstance);
	}
}

CCoreUserInterface::~CCoreUserInterface() {

}

ISSWEImporter *CCoreUserInterface::createImporter() {
	return (ISSWEImporter*)(new CImporter(devices));
}

IMonitor *CCoreUserInterface::addMonitor(irr::core::stringc path) {
	u32 existingNb = devices->getCoreData()->getMonitors()->size();
	pluginsManager->loadMonitorPlugin(path.make_upper().c_str());
	if (existingNb < devices->getCoreData()->getMonitors()->size())
		return devices->getCoreData()->getMonitors()->operator[](existingNb).getMonitor();
	else
		return 0;
}

void CCoreUserInterface::setLogEventWindowVisible(bool visible) {
	logVisible = visible;
	if (logWindow)
		logWindow->setVisible(logVisible);
}

void CCoreUserInterface::update() {

    devices->updateDevice();

	if (!devices->isOnlyForPlaying()) {
		contextMenuInstance->update();
		windowsManagerInstance->update();
		devices->getProcessesLogger()->update();
	}


	if (logWindow) {
		if (logWindow->getRelativePosition().getHeight() == 520) {
			logWindow->setRelativePosition(position2di(driver->getScreenSize().Width-320, driver->getScreenSize().Height-540));
		} else {
			logWindow->setRelativePosition(position2di(driver->getScreenSize().Width-1000, driver->getScreenSize().Height-620));
		}
	}
}

bool CCoreUserInterface::OnEvent(const SEvent &event) {

    if (devices->getVideoDriver()->getScreenSize() != windowSize) {
        windowSize = devices->getVideoDriver()->getScreenSize();
    }

    if (event.EventType == EET_GUI_EVENT) {
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (!devices->isOnlyForPlaying() && event.GUIEvent.Caller == contextMenuInstance->getConsoleButton()) {
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
			#ifndef SSWE_RELEASE
			if (event.KeyInput.Key == KEY_ESCAPE) {
				devices->getDevice()->closeDevice();
			}
			#endif
			if (event.KeyInput.Key == KEY_ESCAPE && devices->isOnlyForPlaying()) {
				devices->getSceneManager()->clear();
				devices->getVideoDriver()->removeAllTextures();
				devices->getDevice()->closeDevice();
			}

			if (event.KeyInput.Key == KEY_KEY_L && devices->isCtrlPushed() && !devices->isShiftPushed() && devices->isOnlyForPlaying()) {
				this->setLogEventWindowVisible(!this->isLogEventWindowVisible());
				if (!logVisible) {
					devices->getDevice()->getCursorControl()->setVisible(false);
					devices->getSceneManager()->setActiveCamera(devices->getFPSCamera());
				}
			}
			if (event.KeyInput.Key == KEY_KEY_L && devices->isCtrlPushed() && devices->isShiftPushed() && devices->isOnlyForPlaying()) {
				SEvent ev;
				ev.EventType = EET_GUI_EVENT;
				ev.GUIEvent.EventType = EGET_BUTTON_CLICKED;
				ev.GUIEvent.Caller = logWindow->getMaximizeButton();
				OnEvent(ev);

				devices->getSceneManager()->setActiveCamera((logWindow->getRelativePosition().getHeight() != 520) ? devices->getMayaCamera()
																												  : devices->getFPSCamera());
				(logWindow->getRelativePosition().getHeight() != 520) ? devices->getDevice()->getCursorControl()->setVisible(true)
																	  : devices->getDevice()->getCursorControl()->setVisible(false);
			}

			if (event.KeyInput.Key == KEY_KEY_F && devices->isCtrlPushed() && devices->isShiftPushed()) {
				if (!devices->isEditBoxEntered() && !devices->isOnlyForPlaying()) {
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

