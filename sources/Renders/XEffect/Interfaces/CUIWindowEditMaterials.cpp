//
//  CUIWindowEditMaterials.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 23/02/13.
//
//
#include "stdafx.h"
#include "CUIWindowEditMaterials.h"

CUIWindowEditMaterials::CUIWindowEditMaterials(CDevices *_devices) {
    devices = _devices;
    
    devices->getEventReceiver()->AddEventReceiver(this);
}

CUIWindowEditMaterials::~CUIWindowEditMaterials() {

}

void CUIWindowEditMaterials::open() {
    materialsWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(130, 110, 700, 540), false, L"Material Renderers", 0, -1);
	materialsWindow->getMinimizeButton()->setVisible(true);
	materialsWindow->getMaximizeButton()->setVisible(true);
    
    menu = devices->getGUIEnvironment()->addMenu(materialsWindow);
    
    menu->addItem(L"File", -1, true, true);
	menu->addItem(L"Help", -1, true, true);
    
    fileMenu = menu->getSubMenu(0);
	fileMenu->addItem(L"Add A Package", 1);
	fileMenu->addItem(L"Export As Package", 2);
    fileMenu->addSeparator();
	fileMenu->addItem(L"Close", 3);
    
    helpMenu = menu->getSubMenu(1);
	helpMenu->addItem(L"How to ?", -1);
    
    devices->getGUIEnvironment()->addStaticText(L"Material Shaders :", rect<s32>(10, 60, 520, 80), false, true, materialsWindow, -1, false);
    editOGLMaterialShader = devices->getGUIEnvironment()->addButton(rect<s32>(460, 60, 520, 80), materialsWindow, -1, L"Edit", L"Edit shader");
    addOGLMaterialShader = devices->getGUIEnvironment()->addButton(rect<s32>(540, 60, 560, 80), materialsWindow, -1, L"+", L"Add a shader");
    removeOGLMaterialShader = devices->getGUIEnvironment()->addButton(rect<s32>(520, 60, 540, 80), materialsWindow, -1, L"-", L"Remove a shader");
    openGLMaterialsList = devices->getGUIEnvironment()->addListBox(rect<s32>(10, 80, 560, 380), materialsWindow, -1, true);
    
    devices->getGUIEnvironment()->addStaticText(L"Name :", rect<s32>(10, 390, 70, 410), false, true, materialsWindow, -1, false);
    materialName = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(70, 390, 280, 410), true, materialsWindow, -1);
    
    close = devices->getGUIEnvironment()->addButton(rect<s32>(460, 390, 560, 420), materialsWindow, -1, L"Close", L"Close this window");
    
    for (u32 i=0; i < devices->getCoreData()->getShaderCallbacks()->size(); i++) {
        stringw name = devices->getCoreData()->getShaderCallbacks()->operator[](i)->getName().c_str();
        openGLMaterialsList->addItem(name.c_str());
    }
    
    if (devices->getCoreData()->getShaderCallbacks()->size() == 0) {
        materialName->setEnabled(false);
        editOGLMaterialShader->setEnabled(false);
    }
}

bool CUIWindowEditMaterials::OnEvent(const SEvent &event) {
    
	if (event.EventType == EET_USER_EVENT) {
		if (event.UserEvent.UserData1 == ECUE_REACTIVE_MINIMIZED_WINDOW) {
			if (event.UserEvent.UserData2 == materialsWindow->getReferenceCount()) {
				devices->getEventReceiver()->RemoveMinimizedWindow(this);
			}
		}
	}

    if (event.EventType == EET_GUI_EVENT) {

		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == materialsWindow) {
				SEvent ev;
				ev.EventType = EET_GUI_EVENT;
				ev.GUIEvent.EventType = EGET_BUTTON_CLICKED;
				ev.GUIEvent.Caller = close;
				ev.GUIEvent.Element = close;
				OnEvent(ev);
			}
		}

		if (event.GUIEvent.EventType == EGET_MENU_ITEM_SELECTED) {
			if (event.GUIEvent.Caller == fileMenu) {
				s32 itemMenuID = fileMenu->getItemCommandId(fileMenu->getSelectedItem());
				if (itemMenuID == 1) {
					openShaderPackage = devices->createFileOpenDialog(L"", CGUIFileSelector::EFST_OPEN_DIALOG, materialsWindow);
				} else if (itemMenuID == 2) {
					saveShaderPackage = devices->createFileOpenDialog(L"", CGUIFileSelector::EFST_SAVE_DIALOG, materialsWindow);
				} else if (itemMenuID == 3) {
					materialsWindow->remove();
					materialsWindow = 0;
					devices->getEventReceiver()->RemoveEventReceiver(this);
					delete this;
				}
			}
		}
        
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            IGUIElement *element = event.GUIEvent.Caller;

            if (element == addOGLMaterialShader) {
                CShaderCallback *callback = new CShaderCallback();
                callback->setName("New Material Type");
                callback->setDevice(devices->getDevice());
                devices->getCoreData()->getShaderCallbacks()->push_back(callback);
                
                stringw name = callback->getName().c_str();

                openGLMaterialsList->addItem(name.c_str());
                
                materialName->setEnabled(true);
                editOGLMaterialShader->setEnabled(true);
            }
            
            if (element == removeOGLMaterialShader) {
                s32 selected;
                selected = openGLMaterialsList->getSelected();
                if (selected != -1) {
					openGLMaterialsList->removeItem(selected);
                    delete devices->getCoreData()->getShaderCallbacks()->operator[](selected);
                    devices->getCoreData()->getShaderCallbacks()->erase(selected);
                } else {
                    devices->addInformationDialog(L"Information", L"Please select a material before trying to remove one...", EMBF_OK);
                }
                
                bool empty;
                empty = (openGLMaterialsList->getItemCount() == 0);
                if (empty) {
                    materialName->setEnabled(false);
                    editOGLMaterialShader->setEnabled(false);
                    
                }
            }
            
            //EDIT MATERIAL
            if (element == editOGLMaterialShader) {
				s32 selected;
                selected = openGLMaterialsList->getSelected();
				if (selected != -1) {
					CUIWindowEditMaterialsCallback *editCallback = new CUIWindowEditMaterialsCallback(devices);
					editCallback->open(devices->getCoreData()->getShaderCallbacks()->operator[](selected));
				}
            }

			if (element == close) {
                materialsWindow->remove();
				materialsWindow = 0;
                devices->getEventReceiver()->RemoveEventReceiver(this);
                delete this;
            }
        }

        if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
			if (event.GUIEvent.Caller == openShaderPackage) {
				io::path extension, filename;
				core::getFileNameExtension(extension, openShaderPackage->getFileName());
                extension.make_lower();
				if (extension == ".spkg") {
					IFileSystem *fileSystem = devices->getDevice()->getFileSystem();
					if (fileSystem->addZipFileArchive(stringc(openShaderPackage->getFileName()).c_str())) {
						stringc path_file = stringc(openShaderPackage->getFileName());

						stringc vertexLines = devices->getCore()->getStringcFromIReadFile("vertex.vbs");
						stringc pixelLines = devices->getCore()->getStringcFromIReadFile("pixel.fbs");
						stringc constantsLines = devices->getCore()->getStringcFromIReadFile("constants.cbs");

						CShaderCallback *callback = new CShaderCallback();
						callback->setName(fileSystem->getFileBasename(openShaderPackage->getFileName()).c_str());
						callback->setDevice(devices->getDevice());
						devices->getCoreData()->getShaderCallbacks()->push_back(callback);
						stringw name = callback->getName().c_str();
						openGLMaterialsList->addItem(name.c_str());
						materialName->setEnabled(true);
						editOGLMaterialShader->setEnabled(true);

						callback->setVertexShader(vertexLines.c_str());
						callback->setPixelShader(pixelLines.c_str());
						callback->setConstants(constantsLines.c_str());
						callback->buildMaterial(devices->getVideoDriver());
					} else {
						devices->addErrorDialog("Error Archive", L"Error when opening the archive\nMaybe the archive is corrupt...", EMBF_OK);
					}
				} else {
					devices->addWarningDialog(L"Sorry", L"This is not a valid shader package", EMBF_OK);
				}
			}
        }
        
        if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
            if (event.GUIEvent.Caller == openGLMaterialsList) {
                IGUIListBox *listbox = (IGUIListBox *)event.GUIEvent.Caller;
                if (listbox->getItemCount() > 0) {
                    stringw name = devices->getCoreData()->getShaderCallbacks()->operator[](listbox->getSelected())->getName().c_str();
                    materialName->setText(name.c_str());
                }
            }
        }
        
        if (event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED) {

        }
        
        if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
            if (event.GUIEvent.Caller == materialName) {
                stringw name = materialName->getText();
                s32 selected;
                selected = openGLMaterialsList->getSelected();
                openGLMaterialsList->setItem(openGLMaterialsList->getSelected(), name.c_str(), 0);
                devices->getCoreData()->getShaderCallbacks()->operator[](selected)->setName(name.c_str());
            }
        }
        
    }
    
    return false;
}
