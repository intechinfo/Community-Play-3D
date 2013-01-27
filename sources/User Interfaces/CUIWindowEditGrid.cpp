//
//  CUIWindowEditGrid.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 08/10/12.
//
//

#include "CUIWindowEditGrid.h"

CUIWindowEditGrid::CUIWindowEditGrid(CDevices *_devices) {
    devices = _devices;
    
    devices->getEventReceiver()->AddEventReceiver(this);
}

CUIWindowEditGrid::~CUIWindowEditGrid() {
    
}

void CUIWindowEditGrid::open() {
    
    editGridWindow = devices->getGUIEnvironment()->addWindow(rect<s32>((devices->getVideoDriver()->getScreenSize().Width/2)-170, 50, 
                                                                       (devices->getVideoDriver()->getScreenSize().Width/2)+170, 
                                                                       220), false, L"Edit Grid Window", 0, -1);
    devices->getGUIEnvironment()->addStaticText(L"Accent Line Offset : ", rect<s32>(5, 25, 140, 45), true, true, editGridWindow, -1, true);
    accentLineOffset = devices->getGUIEnvironment()->addEditBox(
                    devices->getCore()->getStrNumber(devices->getObjectPlacement()->getGridSceneNode()->GetAccentlineOffset()).c_str(),
                    rect<s32>(150, 25, 300, 45), true, editGridWindow, CXT_EDIT_WINDOW_GRID_EVENTS_ACCENT_OFFSET_EDITBOX);
    
    devices->getGUIEnvironment()->addStaticText(L"Grid Size : ", rect<s32>(5, 55, 140, 75), true, true, editGridWindow, -1, true);
    size = devices->getGUIEnvironment()->addEditBox(
                        devices->getCore()->getStrNumber(devices->getObjectPlacement()->getGridSceneNode()->GetSize()).c_str(),
                        rect<s32>(150, 55, 300, 75), true, editGridWindow, CXT_EDIT_WINDOW_GRID_EVENTS_SIZE_EDITBOX);
    
    devices->getGUIEnvironment()->addStaticText(L"Grid Spacing : ", rect<s32>(5, 85, 140, 105), true, true, editGridWindow, -1, true);
    spacing = devices->getGUIEnvironment()->addEditBox(
                                devices->getCore()->getStrNumber(devices->getObjectPlacement()->getGridSceneNode()->GetSpacing()).c_str(),
                                rect<s32>(150, 85, 300, 105), true, editGridWindow, CXT_EDIT_WINDOW_GRID_EVENTS_SPACING_EDITBOX);
    
    devices->getGUIEnvironment()->addButton(rect<s32>(5, 125, 90, 155), editGridWindow, CXT_EDIT_WINDOW_GRID_EVENTS_CLOSE, 
											L"Close", L"Close This Window");
}

bool CUIWindowEditGrid::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_GUI_EVENT) {
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            
            s32 id = event.GUIEvent.Caller->getID();
            if (id == CXT_EDIT_WINDOW_GRID_EVENTS_CLOSE) {
                editGridWindow->remove();
            }
            
        }
        
        if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
            
            s32 id = event.GUIEvent.Caller->getID();
            
            switch (id) {
                case CXT_EDIT_WINDOW_GRID_EVENTS_ACCENT_OFFSET_EDITBOX: {
                    stringc accentLineOffset_c = accentLineOffset->getText();
                    devices->getObjectPlacement()->getGridSceneNode()->SetAccentlineOffset(
                                                                       devices->getCore()->getU32(accentLineOffset_c.c_str()));
                }
                    break;
                    
                case CXT_EDIT_WINDOW_GRID_EVENTS_SIZE_EDITBOX: {
                    stringc size_c = size->getText();
                    devices->getObjectPlacement()->getGridSceneNode()->SetSize(devices->getCore()->getU32(size_c.c_str()));
                }
                    break;
                    
                case CXT_EDIT_WINDOW_GRID_EVENTS_SPACING_EDITBOX: {
                    stringc spacing_c = spacing->getText();
                    u32 spacing_u32 = devices->getCore()->getU32(spacing_c.c_str());
                    if (spacing_u32 != 0) {
                        devices->getObjectPlacement()->getGridSceneNode()->SetSpacing(spacing_u32);
                    }
                }
                    break;
                    
                default:
                    break;
            }
            
        }
    }
    
    return false;
}
