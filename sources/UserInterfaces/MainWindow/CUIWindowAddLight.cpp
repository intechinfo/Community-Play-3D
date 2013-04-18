//
//  CUIWindowAddOctTree.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 27/09/12.
//
//

#include "CUIWindowAddLight.h"

CUIWindowAddLight::CUIWindowAddLight(CDevices *_devices, IGUIListBox *_lightsListBox) {
    devices = _devices;
    devices->getEventReceiver()->AddEventReceiver(this);
    
    lightsListBox = _lightsListBox;
}

CUIWindowAddLight::~CUIWindowAddLight() {
    
}

void CUIWindowAddLight::open() {
    
    addLightWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(450, 100, 750, 240), 
                                                              true, L"Add a light node", 0, -1);
    
    devices->getGUIEnvironment()->addStaticText(L"Name : (#light: will be added automatically)", 
                                                rect<s32>(5, 25, 250, 85), false, false, addLightWindow, -1, false);
    
    addLightEditBox = devices->getGUIEnvironment()->addEditBox(L"myName", rect<s32>(5, 50, 210, 80), true, 
                                                                addLightWindow, -1);
    
    devices->getGUIEnvironment()->addButton(rect<s32>(5, 110, 70, 135), addLightWindow, 
                                            CXT_WINDOW_ADD_LIGHT_EVENTS_ACCEPT, L"Accept", 
                                            L"Accept and add a light node");
    
    devices->getGUIEnvironment()->addButton(rect<s32>(80, 110, 145, 135), addLightWindow, 
                                            CXT_WINDOW_ADD_LIGHT_EVENTS_CLOSE, L"Cancel", L"Close this window");
    
}

bool CUIWindowAddLight::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_GUI_EVENT) {
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            s32 id = event.GUIEvent.Caller->getID();
            switch (id) {
                    
                case CXT_WINDOW_ADD_LIGHT_EVENTS_ACCEPT: {
                    ILightSceneNode *light = devices->getSceneManager()->addLightSceneNode();
                    if (light) {
                        stringw lightNodeName = L"#light:";
                        lightNodeName += addLightEditBox->getText();
                        light->setName(lightNodeName.c_str());
                        
                        SShadowLight shadowLight = SShadowLight(1024, vector3df(0,0,0), vector3df(0,0,0), 
                                                   SColor(255, 255, 255, 255), 20.0f, 5000.f, 90.f * DEGTORAD);
                        devices->getXEffect()->addShadowLight(shadowLight);
                        
						SLightsData ldata(light);
						devices->getCoreData()->getLightsData()->push_back(ldata);
                        
                        lightsListBox->addItem(lightNodeName.c_str());
                        
                        addLightWindow->remove();
                    } else {
                        devices->addWarningDialog(L"Warning", 
                                                  L"Error when adding a light \n"
                                                  L"Please try again \n", EMBF_OK);
                    }
                }
                    break;
                    
                case CXT_WINDOW_ADD_LIGHT_EVENTS_CLOSE:
                    addLightWindow->remove();
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    return false;
}
