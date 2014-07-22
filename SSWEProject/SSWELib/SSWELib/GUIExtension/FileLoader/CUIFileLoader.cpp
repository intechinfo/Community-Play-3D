#include "stdafx.h"

#include "CUIFileLoader.h"

CUIFileLoader::CUIFileLoader(CDevices *_devices) {
    devices = _devices;

	selector = 0;
    
    removeOldTexture = false;
    oldTexture = 0;

	devices->getEventReceiver()->AddEventReceiver(this);
}

CUIFileLoader::~CUIFileLoader() {
    
}

IGUIFileOpenDialog *CUIFileLoader::create(stringw name, CGUIFileSelector::E_FILESELECTOR_TYPE type, IGUIElement *parent, bool modal) {
    IGUIEnvironment *gui = devices->getGUIEnvironment();
    
    //GUI ELEMENTS
	window = gui->addWindow(rect<s32>(120, 100, 970, 610), false, name.c_str(), 0, -1);
    window->getMaximizeButton()->setVisible(true);
    devices->getCore()->centerWindow(window, devices->getVideoDriver()->getScreenSize());
    normalWidth = window->getRelativePosition().getWidth();
    
    selector = new CGUIFileSelector(name.c_str(), gui, window, -1, type);
    selector->setRelativePosition(rect<s32>(10, 30, 530, 500));
    selector->setDrawWindow(false);
    
    preview = gui->addImage(rect<s32>(540, 30, 840, 220), window, -1, L"Preview", false);
    preview->setScaleImage(true);
    
    gui->addStaticText(L"", rect<s32>(540, 230, 840, 240), true, true, window, -1, false);
    infostxt = gui->addStaticText(L"Informations :", rect<s32>(540, 250, 840, 450), true, true, window, -1, true);
    
    accept = gui->addButton(rect<s32>(630, 470, 730, 500), window, -1, L"Ok", L"Choose selected element");
	close = gui->addButton(rect<s32>(740, 470, 840, 500), window, -1, L"Close", L"Close");
    
    SEvent ev;
    ev.EventType = EET_GUI_EVENT;
    ev.GUIEvent.EventType = EGET_BUTTON_CLICKED;
    ev.GUIEvent.Caller = window->getMaximizeButton();
    this->OnEvent(ev);
    
    return selector;
}

bool CUIFileLoader::OnEvent(const irr::SEvent &event) {
    
    if (event.EventType == EET_GUI_EVENT) {
        if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
            if (event.GUIEvent.Caller == window) {
                devices->getEventReceiver()->RemoveEventReceiver(this);
                delete this;
            }
        }
        
        if (event.GUIEvent.EventType == EGET_FILE_SELECTED || event.GUIEvent.EventType == EGET_FILE_CHOOSE_DIALOG_CANCELLED) {
            if (event.GUIEvent.Caller == selector) {
                window->remove();
                devices->getEventReceiver()->RemoveEventReceiver(this);
                delete this;
            }
        }
        
        if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
            if (event.GUIEvent.Caller == selector->getLstBox()) {
                IVideoDriver *driver = devices->getVideoDriver();
                if (removeOldTexture) {
                    driver->removeTexture(oldTexture);
                }
                
                bool found = false;
                for (u32 i=0; i < driver->getTextureCount(); i++) {
                    stringc path = selector->getFileName();
                    stringc npath = path;
                    npath.remove(devices->getWorkingDirectory().c_str());
                    
                    stringc texPath = driver->getTextureByIndex(i)->getName().getPath();
                    if (texPath == path || texPath == npath) {
                        found = true;
                        break;
                    }
                }
                
                if (found)
                    removeOldTexture = false;
                else
                    removeOldTexture = true;
                
                oldTexture = driver->getTexture(selector->getFileName());
                preview->setImage(oldTexture);
            }
        }
        
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            //WINDOW
            if (event.GUIEvent.Caller == window->getMaximizeButton()) {
                if (window->getRelativePosition().getWidth() != normalWidth) {
                    window->setRelativePosition(rect<s32>(120, 100, 970, 610));
                    selector->setRelativePosition(rect<s32>(10, 30, 530, 500));
                    
                    infostxt->setRelativePosition(rect<s32>(540, 250, 840, 450));
                    
                    accept->setRelativePosition(vector2di(630, 470));
                    close->setRelativePosition(vector2di(740, 470));
                    
                    window->setDraggable(true);
                } else {
                    window->setRelativePosition(rect<s32>(0, 80, window->getRelativePosition().getWidth()+1,
                                                          devices->getVideoDriver()->getScreenSize().Height-20));
                    selector->setRelativePosition(rect<s32>(10, 30, 530, window->getRelativePosition().getHeight()-20));
                    
                    accept->setRelativePosition(vector2di(630, window->getRelativePosition().getHeight()-45));
                    close->setRelativePosition(vector2di(740, window->getRelativePosition().getHeight()-45));
                    
                    infostxt->setRelativePosition(rect<s32>(540, 250, 840, accept->getRelativePosition().UpperLeftCorner.Y-20));
                    
                    window->setDraggable(false);
                }
            }
            
            //BUTTONS
            if (event.GUIEvent.Caller == close) {
                SEvent ev;
                ev.EventType = EET_GUI_EVENT;
                ev.GUIEvent.EventType = EGET_FILE_CHOOSE_DIALOG_CANCELLED;
                ev.GUIEvent.Caller = selector;
                devices->getEventReceiver()->OnEvent(ev);
            }
            if (event.GUIEvent.Caller == accept) {
                SEvent ev;
                ev.EventType = EET_GUI_EVENT;
                ev.GUIEvent.EventType = EGET_FILE_SELECTED;
                ev.GUIEvent.Caller = selector;
                devices->getEventReceiver()->OnEvent(ev);
            }
        }
    }
    
    return false;
}
