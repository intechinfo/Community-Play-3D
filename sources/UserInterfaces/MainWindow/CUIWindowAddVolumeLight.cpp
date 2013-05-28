//
//  CUIWindowAddVolumeLight.cpp
//  MacOSX
//
//  Created by Arthur Caron on 28/05/2013
//
//
#include "stdafx.h"
#include "CUIWindowAddVolumeLight.h"

CUIWindowAddVolumeLight::CUIWindowAddVolumeLight(CDevices *_devices, IGUIListBox *_lightsListBox) {
    devices = _devices;
    devices->getEventReceiver()->AddEventReceiver(this);
    
    volumeLightsListBox = _lightsListBox;
}

CUIWindowAddVolumeLight::~CUIWindowAddVolumeLight() {
    
}

void CUIWindowAddVolumeLight::open() {
    
    addVolumeLightWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(450, 100, 750, 240), 
                                                              true, L"Add a volume light node", 0, -1);
    
    devices->getGUIEnvironment()->addStaticText(L"Name : (#volumeLight: will be added automatically)", 
                                                rect<s32>(5, 25, 250, 85), false, false, addVolumeLightWindow, -1, false);
    
    addVolumeLightEditBox = devices->getGUIEnvironment()->addEditBox(L"myName", rect<s32>(5, 50, 210, 80), true, 
                                                                addVolumeLightWindow, -1);
    
    acceptButton = devices->getGUIEnvironment()->addButton(rect<s32>(5, 110, 70, 135), addVolumeLightWindow, 
                                            -1, L"Accept", 
                                            L"Accept and add a light node");
    
    closeButton = devices->getGUIEnvironment()->addButton(rect<s32>(80, 110, 145, 135), addVolumeLightWindow, 
                                            -1, L"Cancel", L"Close this window");
    
}

bool CUIWindowAddVolumeLight::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_GUI_EVENT) {
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if(event.GUIEvent.Caller == acceptButton) {
				IVolumeLightSceneNode *volumeLight = devices->getSceneManager()->addVolumeLightSceneNode(0, -1, 32, 32, SColor(255, 64, 64, 64), SColor(255, 64, 64, 64));
				if (volumeLight) {
					volumeLight->setPosition(vector3df(-19, 0, 4));
					volumeLight->setScale(core::vector3df(246.0f, 246.0f, 246.0f));
					volumeLight->getMaterial(0).setTexture(0, devices->getSceneManager()->getVideoDriver()->getTexture("data/Lights/lightFalloff.png"));
					stringw volumeLightNodeName = L"#volumeLight:";
                    volumeLightNodeName += addVolumeLightEditBox->getText();
                    volumeLight->setName(volumeLightNodeName.c_str());

					devices->getCoreData()->getVolumeLightsData()->push_back(SVolumeLightsData(volumeLight));
					volumeLightsListBox->addItem(volumeLightNodeName.c_str());
					addVolumeLightWindow->remove();
				}
				return true;
            }
			if(event.GUIEvent.Caller == closeButton) {
                addVolumeLightWindow->remove();
				return true;
			}
        }
    }
    
    return false;
}
