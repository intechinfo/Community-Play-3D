//
//  CRenderCallback.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 28/11/12.
//
//
#include "stdafx.h"
#include "CUIScriptEditor.h"

#include "../../../UserInterfaces/CodeEditor/CUICodeEditor.h"

CUIScriptEditor::CUIScriptEditor(CDevices *_devices) {
	devices = _devices;
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	resizeCodeEditor = true;
	//-----------------------------------
    //GUI ELEMENTS
	window = gui->addWindow(rect<s32>(250, 110, 450, 700), false, L"Scripts", 0, -1);
	window->getMaximizeButton()->setVisible(true);
	window->getMinimizeButton()->setVisible(true);

	runScript = gui->addButton(rect<s32>(10, 30, 180, 60), window, -1, L"Run Script", L"Runs the selected script");

	fileName = gui->addEditBox(L"", rect<s32>(10, 80, 150, 100), true, window, -1);
	addFile = gui->addButton(rect<s32>(150, 80, 170, 100), window, -1, L"+", L"Adds a new file script");
	removeFile = gui->addButton(rect<s32>(170, 80, 190, 100), window, -1, L"-", L"Removes the selected file script");

	files = gui->addListBox(rect<s32>(10, 100, 190, 580), window, -1, true);

	codeEditor = new CUICodeEditor(devices, 0, false);
	codeEditor->setAutoSave(true);
	codeEditor->setEnabled(false);
	//-----------------------------------

	for (u32 i=0; i < devices->getCoreData()->getScriptFiles()->size(); i++) {
		files->addItem(devices->getCoreData()->getScriptFiles()->operator[](i).getName().c_str());
	}
	if (files->getItemCount() == 0) {
		runScript->setEnabled(false);
		fileName->setEnabled(false);
	}

	devices->getEventReceiver()->AddEventReceiver(this, window);
}

CUIScriptEditor::~CUIScriptEditor() {

}

bool CUIScriptEditor::OnEvent(const SEvent &event) {

	if (window) {
		codeEditor->setVisible(window->isVisible());
	}

	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == EMIE_MOUSE_MOVED) {
			if (resizeCodeEditor)
				codeEditor->resize(position2di(window->getRelativePosition().UpperLeftCorner.X+window->getRelativePosition().getWidth(),
											   window->getRelativePosition().UpperLeftCorner.Y));
			else
				devices->getGUIEnvironment()->getRootGUIElement()->bringToFront(window);
		}
	}

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window) {
				codeEditor->close();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
		}

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == window->getMaximizeButton()) {
				codeEditor->maximize(rect<s32>(0, 75, devices->getVideoDriver()->getScreenSize().Width,
											   devices->getVideoDriver()->getScreenSize().Height-20));
				resizeCodeEditor = !resizeCodeEditor;
			}

			if (event.GUIEvent.Caller == addFile) {
				irr::core::stringc test = "scene = Scene.new()\n";
				test += "node = scene:getSceneNodeByName(\"#object:myName\")\n";
				test += "mc = scene:getTerrainMaterialCount(node)\n";
				SScriptFile scptf(test.c_str(), "unnamed");
				devices->getCoreData()->getScriptFiles()->push_back(scptf);
				files->addItem(scptf.getName().c_str());
				runScript->setEnabled(true);
				codeEditor->setEnabled(true);
			}
			if (event.GUIEvent.Caller == removeFile) {
				if (files->getSelected() != -1) {
					devices->getCoreData()->getScriptFiles()->erase(files->getSelected());
					files->removeItem(files->getSelected());
					codeEditor->setTextData(0);
					if (files->getItemCount() == 0) {
						runScript->setEnabled(false);
						fileName->setEnabled(false);
						codeEditor->setEnabled(false);
					}
				}
			}
			if (event.GUIEvent.Caller == runScript) {
				if (files->getSelected() != -1) {
					devices->getScripting()->setScriptFile(devices->getCoreData()->getScriptFiles()->operator[](files->getSelected()).getFile());
					devices->getScripting()->setScriptName(devices->getCoreData()->getScriptFiles()->operator[](files->getSelected()).getName());
					//CScripting *c = devices->getScripting();
					//std::thread runScript_t(&CScripting::runScript_t, *c);
					//runScript_t.detach();
					devices->getScripting()->runScript_t();
				}
			}
		}

		if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
			if (event.GUIEvent.Caller == files) {
				if (files->getSelected() != -1) {
					codeEditor->setTextData(devices->getCoreData()->getScriptFiles()->operator[](files->getSelected()).getFilePointer());
					fileName->setEnabled(true);
					codeEditor->setEnabled(true);
					fileName->setText(devices->getCoreData()->getScriptFiles()->operator[](files->getSelected()).getName().c_str());
				}
			}
		}

		if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
			if (event.GUIEvent.Caller == fileName) {
				if (files->getSelected() != -1) {
					devices->getCoreData()->getScriptFiles()->operator[](files->getSelected()).setName(fileName->getText());
					files->setItem(files->getSelected(), fileName->getText(), 0);
				}
			}
		}
	}

	return false;
}