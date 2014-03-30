//
//  CUIEditEffects.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 20/11/12.
//
//
#include "stdafx.h"
#include "CUIWindowEditFilters.h"

#include "CFilterCallback.h"

CUIWindowEditFilters::CUIWindowEditFilters(CDevices *_devices) {
    devices = _devices;

	fcallback = new CCoreFilterCallback(devices);

	IGUIEnvironment *gui = devices->getGUIEnvironment();
	//GUI ELEMENTS
	//RENDERERS
	window = gui->addWindow(rect<s32>(460, 260, 1360, 680), false, L"Shaders Edition Window", 0, -1);
	window->getMaximizeButton()->setVisible(true);
	window->getMinimizeButton()->setVisible(true);
	devices->getCore()->centerWindow(window, devices->getVideoDriver()->getScreenSize());

	viewPort = new CGUIViewport(devices->getGUIEnvironment(), window, 1, rect<s32>(10, 60, 460, 350));
    if (viewPort) {
		//viewPort->setScreenQuad(devices->getXEffect()->getScreenQuad());
		viewPort->setScreenQuad(devices->getXEffect()->getScreenQuadPtr());
		viewPort->setSceneManager(devices->getXEffect()->getActiveSceneManager());
		viewPort->setRenderScreenQuad(true);
        viewPort->setOverrideColor(SColor(255, 0, 0, 0)); 
    }

	useVSM = gui->addCheckBox(devices->getXEffect()->isUsingVSMShadows(), rect<s32>(10, 360, 460, 380), window, -1, L"Use VSM Shadows");
	enableDepthPass = gui->addCheckBox(devices->getXEffect()->isDepthPassEnabled(), rect<s32>(10, 380, 460, 400), window, -1, L"Enable Depth Pass");

	//FILTERS
	filters = gui->addListBox(rect<s32>(490, 80, 890, 350), window, -1, true);
	gui->addStaticText(L"Filters :", rect<s32>(490, 60, 650, 80), false, true, window, -1, false);
	editCallback = gui->addButton(rect<s32>(650, 60, 750, 80), window, -1, L"Callback", L"Edit selected filter's callback");
	editFilter = gui->addButton(rect<s32>(750, 60, 850, 80), window, -1, L"Edit", L"Edit selected filter");
	addFilter = gui->addButton(rect<s32>(870, 60, 890, 80), window, -1, L"+", L"Add a filter");
	removeFilter = gui->addButton(rect<s32>(850, 60, 870, 80), window, -1, L"-", L"Remove selected filter");

	openLogger = gui->addButton(rect<s32>(490, 350, 590, 370), window, -1, L"Open Logs", L"Open Logs Window");

	close = gui->addButton(rect<s32>(790, 380, 890, 410), window, -1, L"Close", L"Close this window");

	//FILL FILTERS LIST
	CCoreData *cdata = devices->getCoreData();
	for (u32 i=0; i < cdata->getEffectFilters()->size(); i++) {
		filters->addItem(stringw(cdata->getEffectFilters()->operator[](i).getName()).c_str());
	}
	if (filters->getItemCount() > 0) {
		filters->setSelected(0);
	} else {
		filters->setSelected(-1);
	}

	//MENU
	menu = gui->addMenu(window);
	menu->addItem(L"File", -1, true, true);
	menu->addItem(L"Edit", -1, true, true);

	IGUIContextMenu *submenu = menu->getSubMenu(0);
	submenu->addItem(L"Load From File... (CTRL+O)", 1, true);
	submenu->addItem(L"Load From Package...", 2, true);
	submenu->addItem(L"Close", 3, true);

	submenu = menu->getSubMenu(1);
	submenu->addItem(L"Reload (CTRL+RETURN)", 1, true);

	//OTHERS
	devices->getEventReceiver()->AddEventReceiver(this, window);

	this->createLogWindow();
}

CUIWindowEditFilters::~CUIWindowEditFilters() {
	loggerWindow->remove();
}

void CUIWindowEditFilters::createLogWindow() {
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	loggerWindow = gui->addWindow(rect<s32>(380, 110, 1370, 480), false, L"Filters Log", 0, -1);
	loggerWindow->getCloseButton()->setVisible(false);

	logEditBox = gui->addEditBox(L"", rect<s32>(10, 30, 980, 330), true, loggerWindow, -1);
	logEditBox->setMultiLine(true);
	logEditBox->setWordWrap(true);
	logEditBox->setAutoScroll(true);

	closeLogWindow = gui->addButton(rect<s32>(880, 340, 980, 360), loggerWindow, -1, L"Close", L"Close Logs Window");
	devices->getCore()->centerWindow(loggerWindow, devices->getVideoDriver()->getScreenSize());

	loggerWindow->setVisible(false);
}

void CUIWindowEditFilters::reloadFilters() {
	ELOG_LEVEL logLevel = devices->getDevice()->getLogger()->getLogLevel();
	devices->getDevice()->getLogger()->setLogLevel(ELL_INFORMATION);
	stringw logText = L"--------------------------------------------\n\n";
	logText += L"Compiling Filters\n\n";
	logEditBox->setText(L"");

	devices->getXEffect()->removeAllPostProcessingEffects(false);

	array<SFilter> *arrayf = devices->getCoreData()->getEffectFilters();
	array<stringc> newCodes;

	for (u32 i=0; i < arrayf->size(); i++) {
		logText += arrayf->operator[](i).getName().c_str();
		logText += "\n";

		if (arrayf->operator[](i).getPixelShader() == "") {
			newCodes.push_back("wanted_error();");
		} else {
			newCodes.push_back(arrayf->operator[](i).getPixelShader());
		}
	}

	logText += L"\n--------------------------------------------\n\n";

	logEditBox->setText(logText.c_str());
	bool success = true;

	irr::core::array<s32> materialTypes = devices->getXEffect()->reloadPostProcessingEffects(newCodes);
	stringc errors = "";
	for (u32 i=0; i < materialTypes.size(); i++) {
		if (materialTypes[i] == -1) {
			errors += "Error when loading ";
			errors += arrayf->operator[](i).getName();
			errors += "\n";
		} else {
			devices->getXEffect()->setPostProcessingRenderCallback(materialTypes[i], arrayf->operator[](i).getPostProcessingCallback());
			arrayf->operator[](i).setCallback(((CFilterCallback*)arrayf->operator[](i).getPostProcessingCallback())->getCallback());
			((CFilterCallback*)arrayf->operator[](i).getPostProcessingCallback())->materialType = materialTypes[i];
		}
		arrayf->operator[](i).setMaterial(materialTypes[i]);
	}

	if (errors != "")
		devices->addInformationDialog("Errors", errors, EMBF_OK, true);

	stringw text = logEditBox->getText();
	if (success) {
		text += "Success";
	} else {
		text += "Failure";
	}
	logEditBox->setText(text.c_str());

	devices->getDevice()->getLogger()->setLogLevel(logLevel);

}

bool CUIWindowEditFilters::OnEvent(const SEvent &event) {

	if (event.EventType == EET_LOG_TEXT_EVENT) {
		stringw textLogEvent = logEditBox->getText();
		textLogEvent += event.LogEvent.Text;
		textLogEvent += L"\n";
		logEditBox->setText(textLogEvent.c_str());
	}

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window) {
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
		}

		//MENU
		if (event.GUIEvent.EventType == EGET_MENU_ITEM_SELECTED) {
			if (event.GUIEvent.Caller == menu->getSubMenu(0)) {
				if (menu->getSubMenu(0)->getSelectedItem() == 0) {
					openShader = devices->createFileOpenDialog(L"Load From File...", CGUIFileSelector::EFST_OPEN_DIALOG,
						devices->getGUIEnvironment()->getRootGUIElement(), false);
				}
				if (menu->getSubMenu(0)->getSelectedItem() == 1) {
					//LOAD FROM PACKAGE
				}
				if (menu->getSubMenu(0)->getSelectedItem() == 2) {
					devices->getEventReceiver()->RemoveEventReceiver(this);
					window->remove();
					delete this;
				}
			}
			if (event.GUIEvent.Caller == menu->getSubMenu(1)) {
				if (menu->getSubMenu(1)->getSelectedItem() == 0) {
					this->reloadFilters();
				}
			}
		}

		//FILE SELECTION
		if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
			if (event.GUIEvent.Caller == openShader) {
				SFilter f;
				f.createLuaState();
				f.setPixelShader(devices->getCore()->getStringcFromFile(openShader->getFileName()).replace("\r", "\n"));
				f.setMaterial(devices->getXEffect()->addPostProcessingEffectFromString(f.getPixelShader().c_str()));
				
				stringw name = openShader->getFileName();
				name.remove(devices->getDevice()->getFileSystem()->getFileDir(name));
				f.setName(name);

				devices->getCoreData()->getEffectFilters()->push_back(f);
				u32 count = devices->getCoreData()->getEffectFilters()->size();
				fcallback->createLuaState(devices->getCoreData()->getEffectFilters()->operator[](count-1).getLuaState());
				CFilterCallback *cb = new CFilterCallback(f.getMaterial(), f.getLuaState(), f.getCallback());
				if (devices->getCoreData()->getEffectFilters()->operator[](count-1).getMaterial() != -1) {
					devices->getXEffect()->setPostProcessingRenderCallback(devices->getCoreData()->getEffectFilters()->operator[](count-1).getMaterial(), cb);
				}
				devices->getCoreData()->getEffectFilters()->operator[](count-1).setPostProcessingCallback(cb);

				filters->addItem(name.c_str());
				filters->setSelected(filters->getItemCount()-1);
			}
		}

		//CHECKBOXES
		if (event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED) {
			if (event.GUIEvent.Caller == useVSM) {
				devices->getXEffect()->setUseVSMShadows(useVSM->isChecked());
			}
			if (event.GUIEvent.Caller == enableDepthPass) {
				devices->getXEffect()->enableDepthPass(enableDepthPass->isChecked());
			}
		}

		//BUTTONS
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == close) {
				devices->getEventReceiver()->RemoveEventReceiver(this);
				window->remove();
				delete this;
			}

			if (event.GUIEvent.Caller == addFilter) {
				SFilter f;
				f.createLuaState();
				fcallback->createLuaState(f.getLuaState());
				CFilterCallback *cb = new CFilterCallback(f.getMaterial(), f.getLuaState(), f.getCallback());
				f.setPostProcessingCallback(cb);
				devices->getCoreData()->getEffectFilters()->push_back(f);
				filters->addItem(stringw(f.getName()).c_str());
				filters->setSelected(filters->getItemCount()-1);
			}
			if (event.GUIEvent.Caller == removeFilter) {
				if (filters->getSelected() != -1) {
					s32 selected = filters->getSelected();
					filters->removeItem(selected);
					devices->getXEffect()->removePostProcessingEffect(devices->getCoreData()->getEffectFilters()->operator[](selected).getMaterial());
					devices->getCoreData()->getEffectFilters()->operator[](selected).destroyLuaState();
					devices->getCoreData()->getEffectFilters()->erase(selected);
				}
			}
			if (event.GUIEvent.Caller == editFilter) {
				s32 selected = filters->getSelected();
				if (selected != -1) {
					CUICodeEditor *codeEditor = new CUICodeEditor(devices, devices->getCoreData()->getEffectFilters()->operator[](selected).getPixelShaderPtr(), true);
					codeEditor->setAutoSave(true);
				}
			}
			if (event.GUIEvent.Caller == editCallback) {
				s32 selected = filters->getSelected();
				if (selected != -1) {
					//CUICodeEditor *codeEditor = new CUICodeEditor(devices, devices->getCoreData()->getEffectFilters()->operator[](selected).getCallbackPtr(), true);
					u32 indice = devices->getXEffect()->getPostProcessingEffectSize() - filters->getItemCount() + filters->getSelected();
					CFilterCallback *callbackF = (CFilterCallback*)devices->getXEffect()->getPostProcessingEffectCallbackPtr(indice);
					CUICodeEditor *codeEditor = new CUICodeEditor(devices, callbackF->getCallbackPtr(), true);
					codeEditor->setAutoSave(true);
				}
			}
			if (event.GUIEvent.Caller == openLogger) {
				loggerWindow->setVisible(true);
				devices->getGUIEnvironment()->getRootGUIElement()->bringToFront(loggerWindow);
			}
			if (event.GUIEvent.Caller == closeLogWindow) {
				loggerWindow->setVisible(false);
			}
		}
	}

	if (event.EventType == EET_KEY_INPUT_EVENT) {
		if (!event.KeyInput.PressedDown) {
			if (devices->isCtrlPushed()) {
				if (event.KeyInput.Key == KEY_RETURN) {
					if (filters->getSelected() != -1) {
						this->reloadFilters();
						//std::thread t(&CUIWindowEditFilters::reloadFilters, *this);
						//t.detach();
					}
				}
				if (event.KeyInput.Key == KEY_KEY_O) {
					openShader = devices->createFileOpenDialog(L"Load From File...", CGUIFileSelector::EFST_OPEN_DIALOG,
						devices->getGUIEnvironment()->getRootGUIElement(), false);
				}
			}
		}
	}

	return false;
}
