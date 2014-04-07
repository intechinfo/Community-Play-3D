//
//  CUIEditParticleFlags.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/01/2014.
//
//

#include "stdafx.h"
#include "CUIEditParticleFlags.h"

CUIParticleEditorFlags::CUIParticleEditorFlags(CDevices *_devices, SPK::Model *_model, IGUIElement *parent, array<stringw> &modelParams) {
    devices = _devices;

	model = _model;
    
    IGUIEnvironment *gui = devices->getGUIEnvironment();
    //GUI
	stringw windowName = L"Particle System Editor Parameters : ";
	windowName += model->getName().c_str();

	window = gui->addWindow(rect<s32>(660, 270, 1360, 330), true, windowName.c_str(), parent, -1);
    window->getMinimizeButton()->setVisible(true);
    window->getMaximizeButton()->setVisible(true);
    devices->getCore()->centerWindow(window, (dimension2du)parent->getRelativePosition().getSize());
    
    devices->getEventReceiver()->AddEventReceiver(this);
    
    SPK::Model *model = _model;
    u32 currenty = 25;
    for (int i=0; i < (int)SPK::PARAM_CUSTOM_0; i++) {
        bool add = false;

        add = model->isEnabled(static_cast<SPK::ModelParam>(i));
        
        if (add) {
			u32 nbValues = model->getNbValues(static_cast<SPK::ModelParam>(i));
			if (nbValues > 0) {
				paramsElements.push_back(gui->addStaticText(modelParams[i+1].c_str(), rect<s32>(10, currenty, 140, currenty+20), true, true, window, i, true));
				if (nbValues == 1) {
					gui->addStaticText(L"Death : ", rect<s32>(140, currenty, 200, currenty+20), true, true, window, -1, true);
					IGUIEditBox *edit = gui->addEditBox(stringw(model->getParamValue(static_cast<SPK::ModelParam>(i), 0)).c_str(),
														rect<s32>(200, currenty, 270, currenty+20), true, window, i);
					edit->setName("Death0");
				}
				if (nbValues == 2) {
					//VALUE 1
					if (model->isRandom(static_cast<SPK::ModelParam>(i))) {
						gui->addStaticText(L"Rand 0 :", rect<s32>(140, currenty, 200, currenty+20), true, true, window, -1, true);
					} else {
						gui->addStaticText(L"Birth :", rect<s32>(140, currenty, 200, currenty+20), true, true, window, -1, true);
					}
					IGUIEditBox *edit1 = gui->addEditBox(stringw(model->getParamValue(static_cast<SPK::ModelParam>(i), 0)).c_str(),
														rect<s32>(200, currenty, 270, currenty+20), true, window, i);
					if (model->isRandom(static_cast<SPK::ModelParam>(i))) {
						edit1->setName("Rand0");
					} else {
						edit1->setName("Birth");
					}
					//VALUE 2
					if (model->isRandom(static_cast<SPK::ModelParam>(i))) {
						gui->addStaticText(L"Rand 1 :", rect<s32>(270, currenty, 330, currenty+20), true, true, window, -1, true);
					} else {
						gui->addStaticText(L"Death :", rect<s32>(270, currenty, 330, currenty+20), true, true, window, -1, true);
					}
					IGUIEditBox *edit2 = gui->addEditBox(stringw(model->getParamValue(static_cast<SPK::ModelParam>(i), 1)).c_str(),
														rect<s32>(330, currenty, 400, currenty+20), true, window, i);
					if (model->isRandom(static_cast<SPK::ModelParam>(i))) {
						edit2->setName("Rand1");
					} else {
						edit2->setName("Death");
					}
				}
				if (nbValues == 4) {
					gui->addStaticText(L"MinBirth :", rect<s32>(140, currenty, 200, currenty+20), true, true, window, -1, true);
					IGUIEditBox *edit = gui->addEditBox(stringw(model->getParamValue(static_cast<SPK::ModelParam>(i), 0)).c_str(),
														rect<s32>(200, currenty, 270, currenty+20), true, window, i);
					edit->setName("MinBirth");

					gui->addStaticText(L"MaxBirth :", rect<s32>(270, currenty, 330, currenty+20), true, true, window, -1, true);
					edit = gui->addEditBox(stringw(model->getParamValue(static_cast<SPK::ModelParam>(i), 1)).c_str(),
										   rect<s32>(330, currenty, 400, currenty+20), true, window, i);
					edit->setName("MaxBirth");

					gui->addStaticText(L"MinDeath :", rect<s32>(400, currenty, 470, currenty+20), true, true, window, -1, true);
					edit = gui->addEditBox(stringw(model->getParamValue(static_cast<SPK::ModelParam>(i), 1)).c_str(),
										   rect<s32>(470, currenty, 540, currenty+20), true, window, i);
					edit->setName("MinDeath");

					gui->addStaticText(L"MaxDeath :", rect<s32>(540, currenty, 610, currenty+20), true, true, window, -1, true);
					edit = gui->addEditBox(stringw(model->getParamValue(static_cast<SPK::ModelParam>(i), 1)).c_str(),
										   rect<s32>(610, currenty, 680, currenty+20), true, window, i);
					edit->setName("MaxDeath");
				}
				fillWindow(20, window);
				currenty += 20;
			}
        }
    }
}

CUIParticleEditorFlags::~CUIParticleEditorFlags() {
    
}

void CUIParticleEditorFlags::fillWindow(u32 currenty, IGUIElement *element) {
    window->setRelativePosition(rect<s32>(window->getRelativePosition().UpperLeftCorner.X,
                                          window->getRelativePosition().UpperLeftCorner.Y,
                                          window->getRelativePosition().LowerRightCorner.X,
                                          window->getRelativePosition().LowerRightCorner.Y + currenty));
}

bool CUIParticleEditorFlags::OnEvent(const SEvent &event) {
    
	if (event.EventType == EET_GUI_EVENT) {

		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window) {
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
				return false;
			}
		}

		IGUIElement *element = event.GUIEvent.Caller;
		while (element->getParent() != devices->getGUIEnvironment()->getRootGUIElement()) {
			if (element == window) {
				break;
			}
			element = element->getParent();
		}

		if (element == devices->getGUIEnvironment()->getRootGUIElement())
			return false;

		if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
			IGUIEditBox *edit = (IGUIEditBox *)event.GUIEvent.Caller;
			size_t id = edit->getID();
			SPK::ModelParam param = static_cast<SPK::ModelParam>(id);
			f32 value = devices->getCore()->getF32(edit->getText());
			stringc name = edit->getName();

			if (name == "Param") {
				model->setParam(param, value);
			}

			if (name == "Death0") {
				model->setParam(param, value);
			}

			if (name == "Rand0" || name == "Rand1" || name == "Birth" || name == "Death") {
				f32 rand0 = model->getParamValue(param, 0);
				f32 rand1 = model->getParamValue(param, 1);
				if (name == "Rand0" || name == "Birth")
					model->setParam(param, value, rand1);
				else
					model->setParam(param, rand0, value);
			}
			if (name == "MinBirth" || name == "MaxBirth" || name == "MinDeath" || name == "MaxDeath") {
				f32 minBirth = model->getParamValue(param, 0);
				f32 maxBirth = model->getParamValue(param, 1);
				f32 minDeath = model->getParamValue(param, 2);
				f32 maxDeath = model->getParamValue(param, 3);
				if (name == "MinBirth")
					model->setParam(param, value, maxBirth, minDeath, maxDeath);
				else if (name == "MaxBirth")
					model->setParam(param, minBirth, value, minDeath, maxDeath);
				else if (name == "MinDeath")
					model->setParam(param, minBirth, maxBirth, value, maxDeath);
				else
					model->setParam(param, minBirth, maxBirth, minDeath, value);
			}
		}
	}

    return false;
}
