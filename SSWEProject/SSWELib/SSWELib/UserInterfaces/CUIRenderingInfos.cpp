//
//  CUIRenderingInfos.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//
#include "stdafx.h"
#include "CUIRenderingInfos.h"

CUIRenderingInfos::CUIRenderingInfos(CDevices *_devices) {
    devices = _devices;
    
    window = devices->getGUIEnvironment()->addWindow(rect<s32>(220, 110, 850, 450), false, L"Current Rendering Infos", 0, -1);
    
    fps = devices->getGUIEnvironment()->addStaticText(L"FPS : ", rect<s32>(10, 30, 620, 50), true, false, window, -1, true);
    driver = devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(10, 50, 620, 70), true, true, window, -1, true);
    stringw driverType = L"Driver Type : ";
    switch (devices->getVideoDriver()->getDriverType()) {
        case EDT_OPENGL:
            driverType += L"OpenGL";
            break;
        case EDT_DIRECT3D9:
            driverType += L"Direct3D 9";
            break;
        case EDT_DIRECT3D8:
            driverType += L"Direct3D 8";
            break;
        default:
            break;
    }
    driver->setText(driverType.c_str());
    resolution = devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(10, 70, 620, 90), true, false, window, -1, true);
    
	stringw moreInfos = "3D Engine : Irrlicht ";
	moreInfos += devices->getDevice()->getVersion();
	moreInfos += "\nOperating System Version : ";
	moreInfos += devices->getDevice()->getOSOperator()->getOperatingSystemVersion();
	moreInfos += "\nOperation System Version : ";
	moreInfos += devices->getDevice()->getOSOperator()->getOperationSystemVersion();
	devices->getGUIEnvironment()->addStaticText(moreInfos.c_str(), rect<s32>(10, 100, 620, 330), true, false, window, -1, true);

	devices->getEventReceiver()->AddEventReceiver(this, window);
}

CUIRenderingInfos::~CUIRenderingInfos() {
    
}

bool CUIRenderingInfos::OnEvent(const SEvent &event) {
    
    stringw fpsW = L"FPS : ";
    fpsW += devices->getCore()->getStrNumberU32(devices->getVideoDriver()->getFPS()).c_str();
    fps->setText(fpsW.c_str());
    
    stringw resolW = L"Resolution : ";
    resolW += devices->getCore()->getStrNumberU32(devices->getVideoDriver()->getScreenSize().Width);
    resolW += "x";
    resolW += devices->getCore()->getStrNumberU32(devices->getVideoDriver()->getScreenSize().Height);
    resolution->setText(resolW.c_str());
    
    if (event.EventType == EET_GUI_EVENT) {
        if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
            if (event.GUIEvent.Caller == window) {
                window->remove();
                devices->getEventReceiver()->RemoveEventReceiver(this);
                delete this;
            }
        }
    }
    
    return false;
}
