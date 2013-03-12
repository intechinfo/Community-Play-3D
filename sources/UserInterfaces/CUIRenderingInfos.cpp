//
//  CUIRenderingInfos.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#include "CUIRenderingInfos.h"

CUIRenderingInfos::CUIRenderingInfos(CDevices *_devices) {
    devices = _devices;
    devices->getEventReceiver()->AddEventReceiver(this);
    
    window = devices->getGUIEnvironment()->addWindow(rect<s32>(250, 150, 530, 385), false, L"Current Rendering Infos", 0, -1);
    window->getCloseButton()->remove();
    
    fps = devices->getGUIEnvironment()->addStaticText(L"FPS : ", rect<s32>(10, 30, 200, 50), true, false, window, -1, true);
    driver = devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(10, 60, 200, 80), true, true, window, -1, true);
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
    resolution = devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(10, 90, 200, 110), true, false, window, -1, true);
    
    devices->getGUIEnvironment()->addStaticText(L"* Since last update", rect<s32>(10, 140, 150, 160), true, false, window, -1, true);
    
    close = devices->getGUIEnvironment()->addButton(rect<s32>(160, 190, 260, 220), window, -1, L"Close", L"Close This Window");
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
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            if (event.GUIEvent.Caller == close) {
                window->remove();
                devices->getEventReceiver()->RemoveEventReceiver(this);
                delete this;
            }
        }
    }
    
    return false;
}
