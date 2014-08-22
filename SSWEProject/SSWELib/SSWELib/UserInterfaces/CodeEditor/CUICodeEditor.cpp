/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include "CUICodeEditor.h"

CUICodeEditor::CUICodeEditor(CDevices *_devices, stringw *code, bool _single) {
	devices = _devices;
	gui = devices->getGUIEnvironment();

	textData = code;

	autoSave = false;
	alwaysBringToFront = false;
	single = _single;

	//-----------------------------------
    //GUI ELEMENTS
	window = gui->addWindow(rect<s32>(320, 140, 1011, 615), false, L"Code Editor", 0, -1);
	if (single) {
		window->getMaximizeButton()->setVisible(true);
		window->getMinimizeButton()->setVisible(true);
	} else {
		window->getCloseButton()->setVisible(false);
	}

	menu = gui->addMenu(window, -1);
	u32 i1 = menu->addItem(L"File", -1, true, true);
	u32 i2 = menu->addItem(L"Edit", -1, true, true);
	u32 i3 = menu->addItem(L"Tools", -1, true, true);

	menu->getSubMenu(0)->addItem(L"Save", 1);
	if (single) {
		menu->getSubMenu(0)->addItem(L"Close", 2);
	}

	menu->getSubMenu(1)->addItem(L"Copy", 1);
	menu->getSubMenu(1)->addItem(L"Paste", 2);
	menu->getSubMenu(1)->addItem(L"Cut", 3);
	menu->getSubMenu(1)->addSeparator();
	menu->getSubMenu(1)->addItem(L"Undo", 4);
	menu->getSubMenu(1)->addItem(L"Redo", 5);
	menu->getSubMenu(1)->addSeparator();
	menu->getSubMenu(1)->addItem(L"Remove from clipboard", 6);

	menu->getSubMenu(2)->addItem(L"Auto-Save", 1, true, false, autoSave, true);
	menu->getSubMenu(2)->addSeparator();
	menu->getSubMenu(2)->addItem(L"Load From File...", 2);
	menu->getSubMenu(2)->addItem(L"Save To File...", 3);

	codeEditor = new CGUIEditBoxIRB(L"\n", true, true, gui, window, -1, rect<s32>(10, 50, 680, 460), devices->getDevice());
	codeEditor->addLUAKeywords();
	codeEditor->addCppKeywords();
	codeEditor->addShaderKeywords();
	codeEditor->addConstantsShaderKeyWords();
	codeEditor->setOverrideColor(SColor(255, 32, 32, 32));
    codeEditor->setBackgroundColor(SColor(255, 200, 200, 200));
    codeEditor->setLineCountColors(SColor(255, 32, 32, 32), SColor(200, 64, 120, 180), SColor(255, 200, 200, 224));
    codeEditor->setSelectionColors(SColor(180, 0, 96, 64), SColor(255, 255, 255, 255), SColor(180, 0, 128, 96));
	codeEditor->setWordWrap(false);
	if (textData != 0)
		codeEditor->setText(textData->c_str());
	//-----------------------------------
	if (single)
		devices->getEventReceiver()->AddEventReceiver(this, window);
	else
		devices->getEventReceiver()->AddEventReceiver(this);

}

CUICodeEditor::~CUICodeEditor() {

}

void CUICodeEditor::maximize(rect<s32> position) {
	if (window->getRelativePosition().getWidth() == position.getWidth()) {
		window->setRelativePosition(rect<s32>(320, 140, 1011, 615));
		codeEditor->setRelativePosition(rect<s32>(10, 50, 680, 460));
	} else {
		window->setRelativePosition(position);
		codeEditor->setRelativePosition(rect<s32>(codeEditor->getRelativePosition().UpperLeftCorner.X, codeEditor->getRelativePosition().UpperLeftCorner.Y,
												  window->getRelativePosition().getWidth()-10, window->getRelativePosition().getHeight()-10));
	}

	window->setDraggable(!window->getRelativePosition().getWidth() == devices->getVideoDriver()->getScreenSize().Width);
}

void CUICodeEditor::setAutoSave(bool _autoSave) {
	autoSave = _autoSave;
	menu->getSubMenu(2)->setItemChecked(0, autoSave);
}

void CUICodeEditor::setTextData(stringw *code) {
	textData = code;
	if (textData != 0)
		codeEditor->setText(textData->c_str());
	else
		codeEditor->setText(L"");
}

bool CUICodeEditor::OnEvent(const SEvent &event) {

	if (event.EventType == EET_KEY_INPUT_EVENT) {
		if (autoSave) {
			stringw newText = codeEditor->getText();
			if (textData)
				textData->operator=(newText);
		}
	}
    
    if (event.EventType == EET_USER_EVENT) {
        if (event.UserEvent.UserData1 == ECUE_WINDOW_CLOSED) {
			if ((IGUIWindow *)event.UserEvent.UserData2 == window->getParent()) {
                this->close();
                return true;
            }
        }
    }

	if (alwaysBringToFront && !single) {
		devices->getGUIEnvironment()->getRootGUIElement()->bringToFront(window);
	}

	if (event.EventType == EET_USER_EVENT) {
		if (event.UserEvent.UserData1 == ECUE_REACTIVE_MINIMIZED_WINDOW) {
			if (event.UserEvent.UserData2 == window->getReferenceCount()) {
				//devices->getEventReceiver()->RemoveMinimizedWindow(this);
			}
		}
	}

	if (event.EventType == EET_GUI_EVENT) {

		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window) {
				this->close();
				return true;
			}
		}

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == window->getMinimizeButton()) {
				//devices->getEventReceiver()->AddMinimizedWindow(this, window);
			}
			if (event.GUIEvent.Caller == window->getMaximizeButton()) {
				maximize(rect<s32>(0, 75, devices->getVideoDriver()->getScreenSize().Width,
								   devices->getVideoDriver()->getScreenSize().Height));
			}
		}

		if (event.GUIEvent.EventType == EGET_MENU_ITEM_SELECTED) {
			if (event.GUIEvent.Caller == menu->getSubMenu(0)) {
				s32 id = menu->getSubMenu(0)->getItemCommandId(menu->getSubMenu(0)->getSelectedItem());
				if (id == 1) {
					stringw newText = codeEditor->getText();
					if (textData)
						textData->operator=(newText);
				}
				if (id == 2) {
					devices->getEventReceiver()->RemoveEventReceiver(this);
					window->remove();
					delete this;
					return true;
				}
			}
			if (event.GUIEvent.Caller == menu->getSubMenu(1)) {
				s32 id = menu->getSubMenu(1)->getItemCommandId(menu->getSubMenu(1)->getSelectedItem());
				if (id == 1) {
					codeEditor->copy();
				}
				if (id == 2) {
					codeEditor->paste();
				}
				if (id == 3) {
					codeEditor->cut();
				}
				if (id == 4) {
					codeEditor->undo();
				}
				if (id == 5) {
					codeEditor->redo();
				}
				if (id == 6) {
					textData->remove(devices->getDevice()->getOSOperator()->getTextFromClipboard());
					codeEditor->setText(textData->c_str());
				}
			}
			if (event.GUIEvent.Caller == menu->getSubMenu(2)) {
				s32 id = menu->getSubMenu(2)->getItemCommandId(menu->getSubMenu(2)->getSelectedItem());
				if (id == 1) {
					autoSave = !autoSave;
				}
				if (id == 2) {
					openFromFileDialog = devices->createFileOpenDialog(L"Open From File...", CGUIFileSelector::EFST_OPEN_DIALOG, window);
				}
			}
		}

		if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
			if (event.GUIEvent.Caller == openFromFileDialog) {
				stringw textFromFile = devices->getCore()->getStringcFromFile(openFromFileDialog->getFileName());
				if (autoSave) {
					textData->operator=(textFromFile);
				}
				codeEditor->setText(textFromFile.c_str());
			}
		}
	}

	return false;
}

void CUICodeEditor::resize(rect<s32> position) {
	window->setRelativePosition(position);
}

void CUICodeEditor::resize(position2di position) {
	window->setRelativePosition(position);
}

IGUIElement *CUICodeEditor::getCodeEditorElement() {
	if (window && codeEditor)
		return codeEditor;
	else
		return 0;
}

IGUIElement *CUICodeEditor::getWindow() {
	if (window)
		return window;
	else
		return 0;
}

void CUICodeEditor::setVisible(bool visible) {
	window->setVisible(visible);
}

void CUICodeEditor::close() {
	devices->getEventReceiver()->RemoveEventReceiver(this);
	window->remove();
	delete this;
}

void CUICodeEditor::addLUAKeywords() { codeEditor->addLUAKeywords(); }
void CUICodeEditor::addCppKeywords() { codeEditor->addCppKeywords(); }
void CUICodeEditor::addShaderKeywords() { codeEditor->addShaderKeywords(); }
void CUICodeEditor::addConstantsShaderKeyWords() { codeEditor->addConstantsShaderKeyWords(); }

void CUICodeEditor::setBackgroundColor(SColor color) {
	codeEditor->setBackgroundColor(color);
}

void CUICodeEditor::addKeyword(stringc keyword, SColor color, bool matchCase) {
	codeEditor->addKeyword(keyword, color, matchCase);
}
void CUICodeEditor::clearKeyWords() {
	codeEditor->clearKeywords();
}
