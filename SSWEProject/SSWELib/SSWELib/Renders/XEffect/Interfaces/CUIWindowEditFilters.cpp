//
//  CUIEditEffects.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 20/11/12.
//
//
#include "stdafx.h"
#include "CUIWindowEditFilters.h"

CUIWindowEditFilters::CUIWindowEditFilters(CDevices *_devices) {
    devices = _devices;

	IGUIEnvironment *gui = devices->getGUIEnvironment();
	//GUI ELEMENTS
	//RENDERERS
	window = gui->addWindow(rect<s32>(460, 260, 1360, 680), false, L"Shaders Edition Window", 0, -1);
	window->getMaximizeButton()->setVisible(true);
	window->getMinimizeButton()->setVisible(true);
	devices->getCore()->centerWindow(window, devices->getVideoDriver()->getScreenSize());

	viewPort = new CGUIViewport(devices->getGUIEnvironment(), window, 1, rect<s32>(10, 60, 460, 350));
    if (viewPort) {
		viewPort->setScreenQuad(devices->getXEffect()->getScreenQuad());
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

	//OTHERS
	devices->getEventReceiver()->AddEventReceiver(this, window);
}

CUIWindowEditFilters::~CUIWindowEditFilters() {
    
}

bool CUIWindowEditFilters::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window) {
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
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
				devices->getCoreData()->getEffectFilters()->push_back(f);
				filters->addItem(stringw(f.getName()).c_str());
				filters->setSelected(filters->getItemCount()-1);
			}
			if (event.GUIEvent.Caller == removeFilter) {
				if (filters->getSelected() != -1) {
					s32 selected = filters->getSelected();
					filters->removeItem(selected);
					devices->getXEffect()->removePostProcessingEffect(devices->getCoreData()->getEffectFilters()->operator[](selected).getMaterial());
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
					CUICodeEditor *codeEditor = new CUICodeEditor(devices, devices->getCoreData()->getEffectFilters()->operator[](selected).getCallbackPtr(), true);
					codeEditor->setAutoSave(true);
				}
			}
		}
	}

	return false;
}
