//
//  CUIEditEffects.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 20/11/12.
//
//

#include "CUIWindowEditEffects.h"

CUIWindowEditEffects::CUIWindowEditEffects(CDevices *_devices) {
    devices = _devices;
    
    devices->getEventReceiver()->AddEventReceiver(this);
    
    openingShader = false;
}

CUIWindowEditEffects::~CUIWindowEditEffects() {
    
}

void CUIWindowEditEffects::open() {
    
    //-----------------------------------
    //MAIN WINDOW
    effectsWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(400, 50, 920, 550),
                                                            false, L"Shaders Edition Window", 0, -1);
    effectsWindow->getCloseButton()->remove();
    
    menu = devices->getGUIEnvironment()->addMenu(effectsWindow);
    
    menu->addItem(L"File", -1, true, true);
	menu->addItem(L"Help", -1, true, true);
    
    submenu = menu->getSubMenu(0);
	submenu->addItem(L"Open a shader", CXT_EDIT_WINDOW_EFFECTS_EVENTS_OPEN);
    submenu->addSeparator();
	submenu->addItem(L"Close", CXT_EDIT_WINDOW_EFFECTS_EVENTS_CLOSE);
    
    submenu = menu->getSubMenu(1);
	submenu->addItem(L"How to ?", -1);
    
    viewPort = new CGUIViewport(devices->getGUIEnvironment(), effectsWindow, 1, 
                                rect<s32>(10, 70, 500, 270), false);
    if (viewPort) {
        viewPort->setSceneManager(devices->getSceneManager());
        viewPort->setOverrideColor(SColor(255, 0, 0, 0)); 
    }
    
    devices->getGUIEnvironment()->addStaticText(L"Shaders : ", rect<s32>(10, 280, 90, 300), true, true, effectsWindow, -1, false);
    
    add = devices->getGUIEnvironment()->addButton(rect<s32>(100, 280, 200, 300), effectsWindow, -1, L"Add", L"Add a shader");
    remove = devices->getGUIEnvironment()->addButton(rect<s32>(210, 280, 310, 300), effectsWindow, -1, L"Remove", L"Add a shader");
    
    shadersList = devices->getGUIEnvironment()->addListBox(rect<s32>(10, 310, 320, 490), effectsWindow, -1, true);
    
    active = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(330, 320, 400, 340), effectsWindow, 
                                                       -1, L"Active");
    
    editCallBack = devices->getGUIEnvironment()->addButton(rect<s32>(410, 320, 510, 340), effectsWindow, -1, L"Edit CallBack", 
                                                           L"Edit The CallBack Of The Shader");
    
    close = devices->getGUIEnvironment()->addButton(rect<s32>(414, 463, 514, 493), effectsWindow, -1, L"Close", L"Close the window");
    
    if (devices->getCoreData()->getEffectRenders()->size() == 0) {
        editCallBack->setEnabled(false);
        active->setEnabled(false);
    }
    for (u32 i=0; i < devices->getCoreData()->getEffectRenders()->size(); i++) {
        stringw name = devices->getCoreData()->getEffectRendersPaths()->operator[](i).c_str();
        name.remove(devices->getDevice()->getFileSystem()->getWorkingDirectory().c_str());
        name.remove('/');
        shadersList->addItem(name.c_str());
    }
    //-----------------------------------
    
    //-----------------------------------
    //CALLBACKS WINDOW
    editionWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(490, 210, 905, 640),
                                                            false, L"Callbacks Edition Window", 0, -1);
    editionWindow->getCloseButton()->remove();
    editionWindow->setVisible(false);
    
    //PIXEL SHADERS
    devices->getGUIEnvironment()->addStaticText(L"Pixel Shader Values", rect<s32>(5, 30, 195, 50), false, true, editionWindow, -1, false);
    //pApplyButton = devices->getGUIEnvironment()->addButton(rect<s32>(120, 30, 190, 50), editionWindow, -1, L"Apply", L"Apply all pixel constants");
    
    devices->getGUIEnvironment()->addStaticText(L"Value :", rect<s32>(5, 60, 55, 80), false, true, editionWindow, -1, false);
    pvalue = devices->getGUIEnvironment()->addEditBox(L"0", rect<s32>(45, 60, 195, 80), true, editionWindow, -1);
    devices->getGUIEnvironment()->addStaticText(L"Name :", rect<s32>(5, 90, 55, 110), false, true, editionWindow, -1, false);
    pname = devices->getGUIEnvironment()->addEditBox(L"No Name", rect<s32>(45, 90, 195, 110), true, editionWindow, -1);
    
    pAdd = devices->getGUIEnvironment()->addButton(rect<s32>(10, 120, 80, 140), editionWindow, -1, L"Add", L"Add a constant");
    pRemove = devices->getGUIEnvironment()->addButton(rect<s32>(95, 120, 175, 140), editionWindow, -1, L"Remove", L"Remove the selected constant");
    
    pList = devices->getGUIEnvironment()->addListBox(rect<s32>(10, 150, 200, 380), editionWindow, -1, true);
    
    pvalue->setEnabled(false);
    pname->setEnabled(false);
    
    //VERTEX SHADERS
    devices->getGUIEnvironment()->addStaticText(L"Vertex Shader Values", rect<s32>(205, 30, 395, 50), false, true, editionWindow, -1, false);
    //vApplyButton = devices->getGUIEnvironment()->addButton(rect<s32>(330, 30, 400, 50), editionWindow, -1, L"Apply", L"Apply all vertex constants");
    
    devices->getGUIEnvironment()->addStaticText(L"Value :", rect<s32>(205, 60, 255, 80), false, true, editionWindow, -1, false);
    vvalue = devices->getGUIEnvironment()->addEditBox(L"0", rect<s32>(245, 60, 395, 80), true, editionWindow, -1);
    devices->getGUIEnvironment()->addStaticText(L"Name :", rect<s32>(205, 90, 255, 110), false, true, editionWindow, -1, false);
    vname = devices->getGUIEnvironment()->addEditBox(L"No Name", rect<s32>(245, 90, 395, 110), true, editionWindow, -1);
    
    vAdd = devices->getGUIEnvironment()->addButton(rect<s32>(210, 120, 290, 140), editionWindow, -1, L"Add", L"Add a constant");
    vRemove = devices->getGUIEnvironment()->addButton(rect<s32>(300, 120, 380, 140), editionWindow, -1, L"Remove", L"Remove the selected constant");
    
    vList = devices->getGUIEnvironment()->addListBox(rect<s32>(205, 150, 395, 380), editionWindow, -1, true);
    
    //CLOSE BUTTON
    closeButton = devices->getGUIEnvironment()->addButton(rect<s32>(300, 390, 400, 420), editionWindow, -1, L"Close", L"Close this window");
    
    //-----------------------------------
    
    matrix4 viewProj;
}

bool CUIWindowEditEffects::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_GUI_EVENT) {
        
        if (event.GUIEvent.EventType == EGET_MENU_ITEM_SELECTED) {
            
            IGUIContextMenu *tempMenu = (IGUIContextMenu *)event.GUIEvent.Caller;
            
            switch (tempMenu->getItemCommandId(tempMenu->getSelectedItem())) {
                //-----------------------------------
                //CONTEXT MENU FILE EVENT
                case CXT_EDIT_WINDOW_EFFECTS_EVENTS_OPEN:
                    devices->createFileOpenDialog(L"Select the shader");
                    openingShader = true;
                    break;
                    
                case CXT_EDIT_WINDOW_EFFECTS_EVENTS_CLOSE:
                    effectsWindow->remove();
                    devices->getEventReceiver()->RemoveEventReceiver(this);
                    delete this;
                    break;
                    
                default:
                    break;
            }
        }
        
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            
            if (event.GUIEvent.Caller == remove) {
                if (shadersList->getItemCount() > 0) {
                    devices->getXEffect()->removePostProcessingEffect(devices->getCoreData()->getEffectRenders()->operator[](shadersList->getSelected()));
                    devices->getCoreData()->getEffectRenders()->erase(shadersList->getSelected());
                    devices->getCoreData()->getEffectRendersPaths()->erase(shadersList->getSelected());
                    devices->getCoreData()->getEffectRenderCallbacks()->erase(shadersList->getSelected());
                    
                    shadersList->removeItem(shadersList->getSelected());
                    
                    if (shadersList->getItemCount() == 0) {
                        editCallBack->setEnabled(false);
                        active->setEnabled(false);
                    }
                } else {
                    devices->addInformationDialog(L"Information",
                                                  L"Please select an item before...",
                                                  EMBF_OK);
                    
                }
            }
            
            if (event.GUIEvent.Caller == add) {
                devices->createFileOpenDialog(L"Select the shader");
                openingShader = true;
            }
            
            if (event.GUIEvent.Caller == close) {
                effectsWindow->remove();
                devices->getEventReceiver()->RemoveEventReceiver(this);
                delete this;
            }
            
            if (event.GUIEvent.Caller == editCallBack) {
                if (shadersList->getSelected() != -1) { 
                    editionWindow->setVisible(true);
                    editionWindow->setDrawBackground(true);
                    pList->clear();
                    for (u32 i=0; i < devices->getCoreData()->getEffectRenderCallbacks()->operator[](shadersList->getSelected())->getPixelValuesNames()->size(); i++) {
                        stringw name = L"";
                        name += devices->getCoreData()->getEffectRenderCallbacks()->operator[](shadersList->getSelected())->getPixelValuesNames()->operator[](i).c_str();
                        pList->addItem(name.c_str());
                    }
                    pname->setText(L"No Name");
                    pvalue->setText(L"0.0");
                    
                    if (pList->getItemCount() == 0) {
                        pname->setEnabled(false);
                        pvalue->setEnabled(false);
                    } else {
                        pname->setEnabled(true);
                        pvalue->setEnabled(true);
                        pList->setSelected(0);
                        
                        stringw text = L"";
                        text += devices->getCoreData()->getEffectRenderCallbacks()->operator[](shadersList->getSelected())->getPixelValuesNames()->operator[](pList->getSelected());
                        pname->setText(text.c_str());
                        
                        stringw value = L"";
                        value += devices->getCoreData()->getEffectRenderCallbacks()->operator[](shadersList->getSelected())->getPixelValues()->operator[](pList->getSelected());
                        pvalue->setText(value.c_str());
                    }
                    
                    shadersList->setEnabled(false);
                    editCallBack->setEnabled(false);
                } else {
                    devices->addInformationDialog(L"Information",
                                                  L"Please select an item in the list before.", EMBF_OK);
                }
            }
            
            if (event.GUIEvent.Caller == closeButton) {
                editionWindow->setVisible(false);
                shadersList->setEnabled(true);
                editCallBack->setEnabled(true);
            }
            
            //EDIT CALLBACKS
            if (event.GUIEvent.Caller == pAdd) {
                if (shadersList->getSelected() != -1) {
                    pList->addItem(L"No Name");
                    
                    devices->getCoreData()->getEffectRenderCallbacks()->operator[](shadersList->getSelected())->getPixelValues()->push_back(0.0);
                    devices->getCoreData()->getEffectRenderCallbacks()->operator[](shadersList->getSelected())->getPixelValuesNames()->push_back("No Name");
                    
                    pname->setEnabled(true);
                    pvalue->setEnabled(true);
                }
            }
            
            if (event.GUIEvent.Caller == pRemove) {
                if (pList->getSelected() != -1 && shadersList->getSelected() != 1) {
                    
                    devices->getCoreData()->getEffectRenderCallbacks()->operator[](shadersList->getSelected())->getPixelValues()->erase(pList->getSelected());
                    devices->getCoreData()->getEffectRenderCallbacks()->operator[](shadersList->getSelected())->getPixelValuesNames()->erase(pList->getSelected());
                    
                    pList->removeItem(pList->getSelected());
                    
                    if (pList->getItemCount() == 0) {
                        pname->setEnabled(false);
                        pvalue->setEnabled(false);
                    }
                } else {
                    devices->addInformationDialog(L"Information", L"Please Select a value before.", EMBF_OK);
                }
            }
        }
        
        if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
            
            if (event.GUIEvent.Caller == pname) {
                if (shadersList->getSelected() != -1 && pList->getSelected() != -1) {
                    stringc newName = "";
                    newName += pname->getText();
                    devices->getCoreData()->getEffectRenderCallbacks()->operator[](shadersList->getSelected())->getPixelValuesNames()->operator[](pList->getSelected()) = newName.c_str();
                    pList->setItem(pList->getSelected(), pname->getText(), 0);
                }
            }
            
            if (event.GUIEvent.Caller == pvalue) {
                if (shadersList->getSelected() != -1 && pList->getSelected() != -1) {
                    stringc newValue = "";
                    newValue += pvalue->getText();
                    float value = devices->getCore()->getF32(newValue.c_str());
                    devices->getCoreData()->getEffectRenderCallbacks()->operator[](shadersList->getSelected())->getPixelValues()->operator[](pList->getSelected()) = value;
                }
            }
        }
        
        if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
            if (openingShader) {
                IGUIFileOpenDialog *dialog = (IGUIFileOpenDialog *)event.GUIEvent.Caller;
                
                stringw name = L"";
                name += dialog->getFileName();
                #ifdef _IRR_OSX_PLATFORM_
                    name.remove(devices->getDevice()->getFileSystem()->getWorkingDirectory().c_str());
                #else
                    stringw windowsName = devices->getDevice()->getFileSystem()->getWorkingDirectory().c_str();
                    windowsName.make_lower();
                    name.remove(devices->getDevice()->getFileSystem()->getWorkingDirectory().c_str());
                #endif
                name.remove('/');
                
                stringw path = L"";
                path = dialog->getFileName();
                s32 render = devices->getXEffect()->addPostProcessingEffectFromFile(path.c_str());
                devices->getCoreData()->getEffectRenders()->push_back(render);
                devices->getCoreData()->getEffectRendersPaths()->push_back(path);
                
                CEffectRenderCallback *callback = new CEffectRenderCallback(render);
                callback->clearPixelValues();
                callback->clearVertexValues();
                devices->getCoreData()->getEffectRenderCallbacks()->push_back(callback);
                devices->getXEffect()->setPostProcessingRenderCallback(render, callback);
                
                shadersList->addItem(name.c_str());
                
                editCallBack->setEnabled(true);
                active->setEnabled(true);
                
                openingShader = false;
            }
        }
        
        if (event.GUIEvent.EventType == EGET_FILE_CHOOSE_DIALOG_CANCELLED) {
            openingShader = false;
        }
        
        if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
            if (event.GUIEvent.Caller == shadersList && shadersList->getItemCount() > 0) {
                if (devices->getXEffect()->postProcessingEffectExists(devices->getCoreData()->getEffectRenders()->operator[](shadersList->getSelected()))) {
                    active->setChecked(true);
                } else {
                    active->setChecked(false);
                }
            }
            
            if (event.GUIEvent.Caller == pList) {
                if (pList->getSelected() != -1 && shadersList->getSelected() != -1) {
                    stringw text = L"";
                    text += devices->getCoreData()->getEffectRenderCallbacks()->operator[](shadersList->getSelected())->getPixelValuesNames()->operator[](pList->getSelected());
                    pname->setText(text.c_str());
                    
                    stringw value = L"";
                    value += devices->getCoreData()->getEffectRenderCallbacks()->operator[](shadersList->getSelected())->getPixelValues()->operator[](pList->getSelected());
                    pvalue->setText(value.c_str());
                }
            }
        }
        
        if (event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED) {
            if (event.GUIEvent.Caller == active) {
                if (active->isChecked()) {
                    if (!devices->getXEffect()->postProcessingEffectExists(devices->getCoreData()->getEffectRenders()->operator[](shadersList->getSelected()))) {
                        devices->getXEffect()->addPostProcessingEffect(devices->getCoreData()->getEffectRenders()->operator[](shadersList->getSelected()));
                    }
                } else {
                    if (devices->getXEffect()->postProcessingEffectExists(devices->getCoreData()->getEffectRenders()->operator[](shadersList->getSelected()))) {
                        devices->getXEffect()->removePostProcessingEffect(devices->getCoreData()->getEffectRenders()->operator[](shadersList->getSelected()));
                    }
                }
            }
        }
    }
    
    return false;
}
