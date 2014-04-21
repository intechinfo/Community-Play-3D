//
//  CUIEditParticleFlags.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/01/2014.
//
//

#include "stdafx.h"
#include "CUIEditParticleInterpolators.h"

CUIParticleEditorInterpolators::CUIParticleEditorInterpolators(CDevices *_devices, SPK::Model *_model, IGUIElement *parent) {
    devices = _devices;
	model = _model;
    
    IGUIEnvironment *gui = devices->getGUIEnvironment();

	array<stringw> paramNames;
    paramNames.push_back("Flag Red");
    paramNames.push_back("Flag Green");
    paramNames.push_back("Flag Blue");
    paramNames.push_back("Flag Alpha");
    paramNames.push_back("Flag Size");
    paramNames.push_back("Flag Mass");
    paramNames.push_back("Flag Angle");
    paramNames.push_back("Flag Texture Index");
    paramNames.push_back("Flag Rotation Speed");

    //GUI
	stringw windowName = L"Particle System Editor Interpolators : ";
	windowName += model->getName().c_str();

	window = gui->addWindow(rect<s32>(610, 220, 1310, 700), true, windowName.c_str(), parent, -1);
    window->getMinimizeButton()->setVisible(false);
    window->getMaximizeButton()->setVisible(false);
    devices->getCore()->centerWindow(window, (dimension2du)parent->getRelativePosition().getSize());

	paramsList = gui->addListBox(rect<s32>(10, 30, 240, 310), window, -1, true);

	enableLooping = gui->addCheckBox(false, rect<s32>(10, 310, 240, 330), window, -1, L"Enable Looping");
	gui->addStaticText(L"", rect<s32>(10, 340, 240, 350), true, true, window, -1, true);
	gui->addStaticText(L"Scale X Variation :", rect<s32>(10, 360, 130, 380), true, true, window, -1, true);
	scaleXVariationeb = gui->addEditBox(L"", rect<s32>(130, 360, 240, 380), true, window, -1);
	gui->addStaticText(L"Offset X Variation :", rect<s32>(10, 380, 130, 400), true, true, window, -1, true);
	offsetXVariationeb = gui->addEditBox(L"", rect<s32>(130, 380, 240, 400), true, window, -1);

	gui->addStaticText(L"Entries :", rect<s32>(260, 30, 560, 50), true, true, window, -1, true);
	editEntry = gui->addButton(rect<s32>(560, 30, 650, 50), window, -1, L"Edit", L"Edit selected entry");
	addEntryb = gui->addButton(rect<s32>(670, 30, 690, 50), window, -1, L"+", L"Add an entry");
	removeEntry = gui->addButton(rect<s32>(650, 30, 670, 50), window, -1, L"-", L"Remove selected entry");
	entriesTable = gui->addTable(rect<s32>(260, 50, 690, 430), window, -1, true);
	entriesTable->addColumn(L"X", 0);
	entriesTable->addColumn(L"Y", 1);
	entriesTable->addColumn(L"Y1", 2);
	entriesTable->setColumnWidth(0, entriesTable->getRelativePosition().getWidth() / 3);
	entriesTable->setColumnWidth(1, entriesTable->getRelativePosition().getWidth() / 3);
	entriesTable->setColumnWidth(2, entriesTable->getRelativePosition().getWidth() / 3);

	close = gui->addButton(rect<s32>(590, 440, 690, 470), window, -1, L"Close", L"Close this window");

	//FILL VALUES OF THE GUI
	interpolators.clear();
	if (model->getNbInterpolated() > 0) {

		bool firstSPKParam = false;
		for (u32 i=0; i < 8; i++) {
			if (model->isInterpolated(static_cast<SPK::ModelParam>(i))) {
				SPK::Interpolator *inter = model->getInterpolator(static_cast<SPK::ModelParam>(i));
				interpolators.push_back(inter);

				paramsList->addItem(paramNames[i].c_str());
				if (firstSPKParam == false) {
					scaleXVariationeb->setText(stringw(inter->getScaleXVariation()).c_str());
					offsetXVariationeb->setText(stringw(inter->getOffsetXVariation()).c_str());

					firstSPKParam = true;
				}
			}
		}
	}

	paramsList->setSelected(0);
	fillTable();

	//ENTRIES TYPE EDITION
	entryEditType = EEDT_UNKNOWN;
	editEntryWindow = 0;
	xeditentry = 0;
	xeditentry = 0;
	xeditentry = 0;
	editEntryOk = 0;
	editEntryCancel = 0;
    
    devices->getEventReceiver()->AddEventReceiver(this);
}

CUIParticleEditorInterpolators::~CUIParticleEditorInterpolators() {
    
}

void CUIParticleEditorInterpolators::fillTable() {
	entriesTable->clearRows();
	if (paramsList->getSelected() != -1) {
		SPK::Interpolator *inter = interpolators[paramsList->getSelected()];

		std::set<SPK::InterpolatorEntry>::const_iterator it = inter->getGraph().begin();
		u32 ti = 0;
		for (; it != inter->getGraph().end(); ++it) {
			entriesTable->addRow(ti);
			entriesTable->setCellText(ti, 0, stringw(it->x));
			entriesTable->setCellText(ti, 1, stringw(it->y0));
			entriesTable->setCellText(ti, 2, stringw(it->y1));
			entriesTable->setCellData(ti, 0, (void*)(inter));
			ti++;
		}

		scaleXVariationeb->setText(stringw(inter->getScaleXVariation()).c_str());
		offsetXVariationeb->setText(stringw(inter->getOffsetXVariation()).c_str());
		enableLooping->setChecked(inter->isLoopingEnabled());
	}
}

void CUIParticleEditorInterpolators::openAddEditWindow(f32 x, f32 y, f32 y1) {
	IGUIEnvironment *gui = devices->getGUIEnvironment();
	editEntryWindow = gui->addWindow(rect<s32>(600, 630, 920, 730), true, L"Edit Entry", window, -1);
	editEntryWindow->getCloseButton()->setVisible(false);
	devices->getCore()->centerWindow(editEntryWindow, (dimension2du)window->getRelativePosition().getSize());

	gui->addStaticText(L"X :", rect<s32>(10, 30, 50, 50), true, true, editEntryWindow, -1, true);
	xeditentry = gui->addEditBox(stringw(x).c_str(), rect<s32>(50, 30, 180, 50), true, editEntryWindow, -1);
	gui->addStaticText(L"Y :", rect<s32>(10, 50, 50, 70), true, true, editEntryWindow, -1, true);
	yeditentry = gui->addEditBox(stringw(y).c_str(), rect<s32>(50, 50, 180, 70), true, editEntryWindow, -1);
	gui->addStaticText(L"Y1 :", rect<s32>(10, 70, 50, 90), true, true, editEntryWindow, -1, true);
	y1editentry = gui->addEditBox(stringw(y1).c_str(), rect<s32>(50, 70, 180, 90), true, editEntryWindow, -1);

	editEntryOk = gui->addButton(rect<s32>(210, 30, 310, 50), editEntryWindow, -1, L"Ok", L"Change values and close this window");
	editEntryCancel = gui->addButton(rect<s32>(210, 70, 310, 90), editEntryWindow, -1, L"Cancel", L"Cancel and close this window");
}

void CUIParticleEditorInterpolators::removeEditWindow() {
	editEntryWindow->remove();
	editEntryWindow = 0;
	xeditentry = 0;
	xeditentry = 0;
	xeditentry = 0;
	editEntryOk = 0;
	editEntryCancel = 0;
	entryEditType = EEDT_UNKNOWN;
}

bool CUIParticleEditorInterpolators::OnEvent(const SEvent &event) {

	//EDIT ENTRY
	if ((event.EventType == EET_GUI_EVENT && event.GUIEvent.EventType == EGET_BUTTON_CLICKED && event.GUIEvent.Caller == editEntry
		|| event.EventType == EET_MOUSE_INPUT_EVENT && event.MouseInput.Event == EMIE_LMOUSE_DOUBLE_CLICK)
		&& devices->getGUIEnvironment()->getFocus() == entriesTable && entriesTable->getSelected() != -1) {

			SPK::Interpolator *inter = interpolators[paramsList->getSelected()];
			std::set<SPK::InterpolatorEntry>::const_iterator it = inter->getGraph().begin();
			u32 i=0;
			while (i < entriesTable->getSelected()) {
				++it;
				i++;
			}
			openAddEditWindow(it->x, it->y0, it->y1);

			entryEditType = EEDT_EDIT;
	}

	//GUI EVENTS MANAGEMENT
	if (event.EventType == EET_GUI_EVENT) {
		//CLOSE
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			devices->getEventReceiver()->RemoveEventReceiver(this);
			delete this;
		}

		//CHECKBOXES
		if (event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED) {
			if (event.GUIEvent.Caller == enableLooping) {
				if (paramsList->getSelected() != -1) {
					SPK::Interpolator *inter = interpolators[paramsList->getSelected()];
					inter->enableLooping(enableLooping->isChecked());
				}
			}
		}

		//EDIT BOXES
		if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
			if (paramsList->getSelected() != -1) {
				SPK::Interpolator *inter = interpolators[paramsList->getSelected()];
				if (event.GUIEvent.Caller == scaleXVariationeb) {
					irr::f32 scv = devices->getCore()->getF32(scaleXVariationeb->getText());
					inter->setScaleXVariation(scv);
				}
				if (event.GUIEvent.Caller == offsetXVariationeb) {
					irr::f32 ocv = devices->getCore()->getF32(offsetXVariationeb->getText());
					inter->setOffsetXVariation(ocv);
				}
			}
		}

		//LIST BOX CHANGED
		if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
			if (event.GUIEvent.Caller == paramsList) {
				fillTable();
			}
		}

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			//CLOSE
			if (event.GUIEvent.Caller == close) {
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}

			//REMOVE ENTRY
			if (event.GUIEvent.Caller == removeEntry) {
				if (entriesTable->getSelected() != -1) {
					SPK::Interpolator *inter = interpolators[paramsList->getSelected()];
					if (entriesTable->getSelected() >= inter->getGraph().size()) {
						return false;
					}

					std::set<SPK::InterpolatorEntry>::const_iterator it = inter->getGraph().begin();
					u32 i=0;
					while (i < entriesTable->getSelected()) {
						++it;
						i++;
					}
					inter->getGraph().erase(it);

					entriesTable->removeRow(entriesTable->getSelected());
				}
			}
			if (event.GUIEvent.Caller == addEntryb) {
				openAddEditWindow(0, 0, 0);
				entryEditType = EEDT_ADD;
			}

			//EDIT/ADD ENTRY
			if (event.GUIEvent.Caller == editEntryCancel) {
				removeEditWindow();
			}

			if (event.GUIEvent.Caller == editEntryOk) {
				f32 x = devices->getCore()->getF32(xeditentry->getText());
				f32 y = devices->getCore()->getF32(yeditentry->getText());
				f32 y1 = devices->getCore()->getF32(y1editentry->getText());

				if (entryEditType == EEDT_ADD) {
					SPK::Interpolator *inter = interpolators[paramsList->getSelected()];
					if (!inter->addEntry(x, y, y1)) {
						removeEditWindow();
						return false;
					}
					u32 ti = entriesTable->getRowCount();
					entriesTable->addRow(ti);
					entriesTable->setCellText(ti, 0, stringw(x));
					entriesTable->setCellText(ti, 1, stringw(y));
					entriesTable->setCellText(ti, 2, stringw(y1));
					entriesTable->setCellData(ti, 0, (void*)(inter));
				} else if (entryEditType == EEDT_EDIT) {
					//SAVE EXISTING GRAPH
					SPK::Interpolator *inter = interpolators[paramsList->getSelected()];
					std::set<SPK::InterpolatorEntry>::const_iterator it = inter->getGraph().begin();
					array<SPK::InterpolatorEntry> entriesArray;
					for (; it != inter->getGraph().end(); ++it) {
						SPK::InterpolatorEntry ent;
						ent.x = it->x;
						ent.y0 = it->y0;
						ent.y1 = it->y1;
						entriesArray.push_back(ent);
					}
					//MODIFY SELECTED VALUE
					entriesArray[entriesTable->getSelected()].x = x;
					entriesArray[entriesTable->getSelected()].y0 = y;
					entriesArray[entriesTable->getSelected()].y1 = y1;

					//RE-FILL GRAPH
					inter->clearGraph();
					for (u32 i=0; i < entriesArray.size(); i++) {
						inter->addEntry(entriesArray[i].x, entriesArray[i].y0, entriesArray[i].y1);
					}
					//UPDATE TABLE
					entriesTable->setCellText(entriesTable->getSelected(), 0, stringw(x).c_str());
					entriesTable->setCellText(entriesTable->getSelected(), 1, stringw(y).c_str());
					entriesTable->setCellText(entriesTable->getSelected(), 2, stringw(y1).c_str());
					entriesTable->setCellData(entriesTable->getSelected(), 0, (void*)(inter));
				}

				removeEditWindow();
			}
		}
	}

    return false;
}
