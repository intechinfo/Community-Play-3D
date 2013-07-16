//
//  CUIEditEffects.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 20/11/12.
//
//
#include "stdafx.h"
#include "CUIWindowEditEffects.h"

CUIWindowEditEffects::CUIWindowEditEffects(CDevices *_devices) {
    devices = _devices;
    
    openingShader = false;
}

CUIWindowEditEffects::~CUIWindowEditEffects() {
    
}

void CUIWindowEditEffects::open() {

    //-----------------------------------
    //MAIN WINDOW
    effectsWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(10, 20, 1030, 610), false, L"Shaders Edition Window", 0, -1);
    //effectsWindow->getCloseButton()->remove();
	effectsWindow->getMaximizeButton()->setVisible(true);
	effectsWindow->getMinimizeButton()->setVisible(true);
    
    menu = devices->getGUIEnvironment()->addMenu(effectsWindow);
    
    menu->addItem(L"File", -1, true, true);
	menu->addItem(L"Help", -1, true, true);
    
    submenu = menu->getSubMenu(0);
	submenu->addItem(L"Open a shader", CXT_EDIT_WINDOW_EFFECTS_EVENTS_OPEN);
    submenu->addSeparator();
	submenu->addItem(L"Close", CXT_EDIT_WINDOW_EFFECTS_EVENTS_CLOSE);
    
    submenu = menu->getSubMenu(1);
	submenu->addItem(L"How to ?", -1);
    
    enableDepthPass = devices->getGUIEnvironment()->addCheckBox(devices->getXEffect()->isDepthPassEnabled(), rect<s32>(10, 50, 160, 70), effectsWindow, 
                                                                -1, L"Enable Depth Pass");
    
	viewPort = devices->getGUIEnvironment()->addImage(rect<s32>(10, 70, 500, 430), effectsWindow, -1);
	viewPort->setImage(devices->getXEffect()->getScreenQuad().rt[1]);
	viewPort->setScaleImage(true);
    /*viewPort = new CGUIViewport(devices->getGUIEnvironment(), effectsWindow, 1, 
                                rect<s32>(10, 70, 580, 270));
    if (viewPort) {
        viewPort->setSceneManager(devices->getSceneManager());
        viewPort->setOverrideColor(SColor(255, 0, 0, 0)); 
    }*/
    
    devices->getGUIEnvironment()->addStaticText(L"Shaders :", rect<s32>(510, 50, 610, 70), false, true, effectsWindow, -1, false);
    
    oglAdd = devices->getGUIEnvironment()->addButton(rect<s32>(630, 50, 650, 70), effectsWindow, -1, L"+", L"Add a shader");
    oglRemove = devices->getGUIEnvironment()->addButton(rect<s32>(610, 50, 630, 70), effectsWindow, -1, L"-", L"Add a shader");
    
    shadersList = devices->getGUIEnvironment()->addListBox(rect<s32>(510, 70, 1010, 270), effectsWindow, -1, true);
    
    active = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(940, 50, 1010, 70), effectsWindow, -1, L"Active");

    close = devices->getGUIEnvironment()->addButton(rect<s32>(910, 550, 1010, 580), effectsWindow, -1, L"Close", L"Close the window");
    
    if (devices->getCoreData()->getEffectRenders()->size() == 0) {
        active->setEnabled(false);
    }

    for (u32 i=0; i < devices->getCoreData()->getEffectRenders()->size(); i++) {
		stringw name = devices->getCoreData()->getEffectRendersPaths()->operator[](i).c_str();
		stringw windowsName = devices->getWorkingDirectory().c_str();
        name.make_lower();
		windowsName.make_lower();
        name.remove(windowsName);
        shadersList->addItem(name.c_str());
    }
    //-----------------------------------

    //-----------------------------------
    //CALLBACKS WINDOW
    editionWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(510, 280, 1010, 540), false, L"Callbacks Edition Window", effectsWindow, -1);
    editionWindow->getCloseButton()->remove();
	editionWindow->setDraggable(false);
	editionWindow->setDrawTitlebar(false);

    //PIXEL SHADERS
    pEditWindow = devices->getGUIEnvironment()->addButton(rect<s32>(130, 30, 200, 50), editionWindow, -1, L"Edit", L"Edit Window For complex scripts");
    devices->getGUIEnvironment()->addStaticText(L"Pixel Shader Values", rect<s32>(5, 30, 130, 50), false, true, editionWindow, -1, false);
    
    devices->getGUIEnvironment()->addStaticText(L"Value :", rect<s32>(5, 60, 55, 80), false, true, editionWindow, -1, false);
	pvalue = devices->getGUIEnvironment()->addEditBox(L"0", rect<s32>(45, 60, editionWindow->getRelativePosition().getWidth()-5, 80), true, editionWindow, -1);
    pvalue->setMultiLine(true);
    devices->getGUIEnvironment()->addStaticText(L"Name :", rect<s32>(5, 90, 55, 110), false, true, editionWindow, -1, false);
    pname = devices->getGUIEnvironment()->addEditBox(L"No Name", rect<s32>(45, 90, editionWindow->getRelativePosition().getWidth()-5, 110), true, editionWindow, -1);
    
    pAdd = devices->getGUIEnvironment()->addButton(rect<s32>(10, 120, 80, 140), editionWindow, -1, L"Add", L"Add a constant");
    pRemove = devices->getGUIEnvironment()->addButton(rect<s32>(95, 120, 175, 140), editionWindow, -1, L"Remove", L"Remove the selected constant");
    
    pList = devices->getGUIEnvironment()->addListBox(rect<s32>(10, 150, editionWindow->getRelativePosition().getWidth()-5,
															   editionWindow->getRelativePosition().getHeight()-15), editionWindow, 
													 -1, true);
    
    pvalue->setEnabled(false);
    pname->setEnabled(false);
    
    //-----------------------------------
    
    //POINTER TO viewProj FOR EFFECTS
    matrix4 viewProj;

	devices->getEventReceiver()->AddEventReceiver(this, effectsWindow);
}

bool CUIWindowEditEffects::OnEvent(const SEvent &event) {
    
	if (event.EventType == EET_USER_EVENT) {
		if (event.UserEvent.UserData1 == ECUE_REACTIVE_MINIMIZED_WINDOW) {
			if (event.UserEvent.UserData2 == effectsWindow->getReferenceCount()) {
				//devices->getEventReceiver()->RemoveMinimizedWindow(this);
			}
		}
	}

    if (event.EventType == EET_GUI_EVENT) {
        
        if (event.GUIEvent.EventType == EGET_MENU_ITEM_SELECTED) {
            
            IGUIContextMenu *tempMenu = (IGUIContextMenu *)event.GUIEvent.Caller;
            
            switch (tempMenu->getItemCommandId(tempMenu->getSelectedItem())) {
                //-----------------------------------
                //CONTEXT MENU FILE EVENT
                case CXT_EDIT_WINDOW_EFFECTS_EVENTS_OPEN:
                    devices->createFileOpenDialog(L"Select the shader", 0);
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

		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == effectsWindow) {
				SEvent ev;
				ev.EventType = EET_GUI_EVENT;
				ev.GUIEvent.EventType = EGET_BUTTON_CLICKED;
				ev.GUIEvent.Caller = close;
				ev.GUIEvent.Element = close;
				OnEvent(ev);
			}
		}
        
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            
			//MAIN WINDOW
			if (event.GUIEvent.Caller == effectsWindow->getMinimizeButton()) {
				//devices->getEventReceiver()->AddMinimizedWindow(this, effectsWindow);
			}

            if (event.GUIEvent.Caller == oglRemove) {
                if (shadersList->getItemCount() > 0 && shadersList->getSelected() != -1) {
                    devices->getXEffect()->removePostProcessingEffect(devices->getCoreData()->getEffectRenders()->operator[](shadersList->getSelected()));
                    devices->getCoreData()->getEffectRenders()->erase(shadersList->getSelected());
                    devices->getCoreData()->getEffectRendersPaths()->erase(shadersList->getSelected());
                    devices->getCoreData()->getEffectRenderCallbacks()->erase(shadersList->getSelected());
                    
                    shadersList->removeItem(shadersList->getSelected());
                    
                    if (shadersList->getItemCount() == 0) {
                        active->setEnabled(false);
                    }
                } else {
                    devices->addInformationDialog(L"Information",
                                                  L"Please select an item before...",
                                                  EMBF_OK);
                    
                }
            }
            
            if (event.GUIEvent.Caller == oglAdd) {
				devices->createFileOpenDialog(L"Select the shader (.GLSL)", CGUIFileSelector::EFST_OPEN_DIALOG, effectsWindow);
                openingShader = true;
            }
            
            if (event.GUIEvent.Caller == close) {
                effectsWindow->remove();
                devices->getEventReceiver()->RemoveEventReceiver(this);
                delete this;
            }
            
            //EDIT CALLBACKS
            if (event.GUIEvent.Caller == pEditWindow) {
				s32 selected = shadersList->getSelected();
				devices->getCoreData()->getEffectRenderCallbacks()->operator[](selected)->modifyPixelValue(pList->getSelected(), devices);
            }
            
            if (event.GUIEvent.Caller == pAdd) {
                if (shadersList->getSelected() != -1) {
                    pList->addItem(L"No Name");
                    s32 selected = -1;
                    selected = shadersList->getSelected();
                    stringc returnedValue_c = "setReturnedValueAsFloat()\n\nvalue =  1\n\nreturnValue(value, 1)";
                    stringc valueName = "No Name";
                    devices->getCoreData()->getEffectRenderCallbacks()->operator[](selected)->getPixelValues()->push_back(returnedValue_c);
                    devices->getCoreData()->getEffectRenderCallbacks()->operator[](selected)->getPixelValuesNames()->push_back(valueName);
                    
                    pname->setEnabled(true);
                    pvalue->setEnabled(true);
                }
            }
            
            if (event.GUIEvent.Caller == pRemove) {
                if (pList->getSelected() != -1 && shadersList->getSelected() != 1) {
                    s32 selected = -1;
                    selected = shadersList->getSelected();
                    devices->getCoreData()->getEffectRenderCallbacks()->operator[](selected)->getPixelValues()->erase(pList->getSelected());
                    devices->getCoreData()->getEffectRenderCallbacks()->operator[](selected)->getPixelValuesNames()->erase(pList->getSelected());
                    
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
                    s32 selected = -1;
                    selected = shadersList->getSelected();
                    devices->getCoreData()->getEffectRenderCallbacks()->operator[](selected)->getPixelValuesNames()->operator[](pList->getSelected()) = newName.c_str();
                    pList->setItem(pList->getSelected(), pname->getText(), 0);
                }
            }
            
            if (event.GUIEvent.Caller == pvalue) {
                if (shadersList->getSelected() != -1 && pList->getSelected() != -1) {
                    stringc newValue = "";
                    s32 selected = -1;
                    selected = shadersList->getSelected();
                    newValue += pvalue->getText();
                    devices->getCoreData()->getEffectRenderCallbacks()->operator[](selected)->getPixelValues()->operator[](pList->getSelected()) = newValue;
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
                    //stringw windowsName = devices->getDevice()->getFileSystem()->getWorkingDirectory().c_str();
                    //windowsName.make_lower();
                    //name.remove(windowsName);
					name.remove(devices->getWorkingDirectory().c_str());
                #endif
                
                stringw path = L"";
                path = dialog->getFileName();
                
                io::path extension;
                core::getFileNameExtension(extension, path);
                extension.make_lower();
                
                s32 render;
                render = devices->getXEffect()->addPostProcessingEffectFromFile(path.c_str());

                devices->getCoreData()->getEffectRenders()->push_back(render);
                devices->getCoreData()->getEffectRendersPaths()->push_back(path);
                
                CEffectRenderCallback *callback = new CEffectRenderCallback(render, devices->getDevice());
                callback->clearPixelValues();
                callback->clearVertexValues();
                devices->getCoreData()->getEffectRenderCallbacks()->push_back(callback);
                devices->getXEffect()->setPostProcessingRenderCallback(render, callback);
                shadersList->addItem(name.c_str());

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
				u32 selected = shadersList->getSelected();
                editionWindow->setVisible(true);
                editionWindow->setDrawBackground(true);
                pList->clear();
                for (u32 i=0; i < devices->getCoreData()->getEffectRenderCallbacks()->operator[](selected)->getPixelValuesNames()->size(); i++) {
                    stringw name = L"";
                    name += devices->getCoreData()->getEffectRenderCallbacks()->operator[](selected)->getPixelValuesNames()->operator[](i).c_str();
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
                    text += devices->getCoreData()->getEffectRenderCallbacks()->operator[](selected)->getPixelValuesNames()->operator[](pList->getSelected());
                    pname->setText(text.c_str());
                        
                    stringw value = L"";
                    value += devices->getCoreData()->getEffectRenderCallbacks()->operator[](selected)->getPixelValues()->operator[](pList->getSelected());
                    pvalue->setText(value.c_str());
                }
            }
            
            if (event.GUIEvent.Caller == pList) {
                s32 selected;
                if (pList->getSelected() != -1 && shadersList->getSelected() != -1) {
                    selected = shadersList->getSelected();
                    stringw text = L"";
                    text += devices->getCoreData()->getEffectRenderCallbacks()->operator[](selected)->getPixelValuesNames()->operator[](pList->getSelected());
                    pname->setText(text.c_str());
                    
                    stringw value = L"";
                    value += devices->getCoreData()->getEffectRenderCallbacks()->operator[](selected)->getPixelValues()->operator[](pList->getSelected());
                    pvalue->setText(value.c_str());
                }
            }
        }
        
        if (event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED) {
            if (event.GUIEvent.Caller == active) {
				if (shadersList->getSelected() != -1) {
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
            
            if (event.GUIEvent.Caller == enableDepthPass) {
                devices->getXEffect()->enableDepthPass(enableDepthPass->isChecked());
				devices->getXEffect()->setUseVSMShadows(enableDepthPass->isChecked());
				for (u32 i=0; i < devices->getXEffect()->getShadowLightCount(); i++) {
					devices->getXEffect()->getShadowLight(i).setRecalculate(true);
				}
            }
        }
    }
    
    return false;
}
