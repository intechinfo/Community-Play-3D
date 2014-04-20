//
//  CUIEditParticleFlags.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/01/2014.
//
//

#include "stdafx.h"
#include "CUIAddEmitter.h"

#include "CUIParticleEditor.h"

CUIAddEmitter::CUIAddEmitter(CDevices *_devices, IGUIElement *parent, SPK::Group *_group, CGUINodesEditor *_editor, CGUINode *_parentNode) {
    devices = _devices;
	group = _group;

	editor = _editor;
	parentNode = _parentNode;

	//SETUP GUI ELEMENTS TO 0
	dirx = 0;
	diry = 0;
	dirz = 0;
	angleA = 0;
	angleB = 0;
	invertedcb = 0;

	IGUIEnvironment *gui = devices->getGUIEnvironment();
	//WINDOW
	window = gui->addWindow(rect<s32>(590, 180, 930, 560), true, L"Add Emitter", parent, -1);
	devices->getCore()->centerWindow(window, dimension2du(parent->getRelativePosition().getWidth(), parent->getRelativePosition().getHeight()));
	gui->addStaticText(L"Emitter :", rect<s32>(10, 30, 330, 50), true, true, window, -1, true);

	//EMITTER TYPE
	gui->addStaticText(L"Type :", rect<s32>(10, 60, 60, 80), true, true, window, -1, true);
	emitterTypecb = gui->addComboBox(rect<s32>(60, 60, 330, 80), window, -1);
	emitterTypecb->addItem(L"Straight Emitter");
	emitterTypecb->addItem(L"Static Emitter");
	emitterTypecb->addItem(L"Spheric Emitter");
	emitterTypecb->addItem(L"Random Emitter");
	emitterTypecb->addItem(L"Normal Emitter");

	//EDITION ZONE
	editionZonet = gui->addStaticText(L"", rect<s32>(10, 90, 330, 330), true, true, window, -1, true);

	//BUTTONS
	close = gui->addButton(rect<s32>(230, 340, 330, 370), window, -1, L"Cancel", L"Cancel and close");
	accept = gui->addButton(rect<s32>(120, 340, 220, 370), window, -1, L"Accept", L"Accept and close");

	//EMITTER
	currentEmitter = SPK::StraightEmitter::create();
	group->addEmitter(currentEmitter);
	currentEmitterType = ECET_STRAIGHT;
	createDirectionGUI();

	//EVENT RECEIVER
	devices->getEventReceiver()->AddEventReceiver(this);
}

CUIAddEmitter::~CUIAddEmitter() {
    
}

bool CUIAddEmitter::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window) {
				group->removeEmitter(currentEmitter);
				delete currentEmitter;
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
		}

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == close) {
				group->removeEmitter(currentEmitter);
				delete currentEmitter;
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}

			if (event.GUIEvent.Caller == accept) {
				CGUINode *node = new CGUINode(devices->getGUIEnvironment(), editor, -1);
                node->setName(currentEmitter->getName().c_str());
                node->setParent(parentNode);
                node->setData(currentEmitter);
				node->setDataType(EPSDT_EMITTER);
				editor->addNode(node);
                node->addTextField(L"Name :", stringw(node->getName()).c_str());
                node->addTextField(L"Flow", stringw(currentEmitter->getFlow()));
                node->add2ParametersFields(L"Force ", L"Min", L"Max", currentEmitter->getForceMin(), currentEmitter->getForceMax());
                node->addButton(L"Configure Zone...");
				node->addCheckBox(L"Active", currentEmitter->isActive());
				node->addTextField(L"Tank",stringw(currentEmitter->getTank()).c_str());
				node->addTextField(L"Change Tank", stringw(currentEmitter->getTank()).c_str());
				node->addTextField(L"Flow", stringw(currentEmitter->getFlow()).c_str());
				node->addTextField(L"Change Flow", stringw(currentEmitter->getFlow()).c_str());

				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
		}

		if (event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED) {
			if (event.GUIEvent.Caller == emitterTypecb) {
				clearEditionZone();
				currentEmitterType = (E_CURRENT_EMITTER_TYPE)emitterTypecb->getSelected();
				group->removeEmitter(currentEmitter);
				currentEmitter = 0;

				delete currentEmitter;
				if (currentEmitterType == ECET_STRAIGHT) {
					currentEmitter = SPK::StraightEmitter::create();
					currentEmitter->setName("Emitter : Straight Emitter");
					createDirectionGUI();
				} else if (currentEmitterType == ECET_STATIC) {
					currentEmitter = SPK::StaticEmitter::create();
					currentEmitter->setName("Emitter : Static Emitter");
				} else if (currentEmitterType == ECET_SPHERIC) {
					currentEmitter = SPK::SphericEmitter::create();
					currentEmitter->setName("Emitter : Spheric Emitter");
					createDirectionGUI();
					createAngleGUI();
				} else if (currentEmitterType == ECET_RANDOM) {
					currentEmitter = SPK::RandomEmitter::create();
					currentEmitter->setName("Emitter : Random Emitter");
				} else if (currentEmitterType == ECET_NORMAL) {
					currentEmitter = SPK::NormalEmitter::create();
					currentEmitter->setName("Emitter : Normal Emitter");
					invertedcb = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(10, 10, 310, 30), editionZonet, -1, L"Invert");
				}

				if (currentEmitter) {
					currentEmitter->setFlow(100);
					currentEmitter->setName("New Emitter");
					group->addEmitter(currentEmitter);
				}
			}
		}

		if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
			if (event.GUIEvent.Caller == dirx || event.GUIEvent.Caller == diry || event.GUIEvent.Caller == dirz) {
				SPK::Vector3D v(devices->getCore()->getF32(dirx->getText()),
								devices->getCore()->getF32(diry->getText()),
								devices->getCore()->getF32(dirz->getText()));
				if (currentEmitterType == ECET_STRAIGHT)
					((SPK::StraightEmitter*)currentEmitter)->setDirection(v);
				else if (currentEmitterType == ECET_SPHERIC)
					((SPK::SphericEmitter*)currentEmitter)->setDirection(v);
			}
			if (event.GUIEvent.Caller == angleA || event.GUIEvent.Caller == angleB) {
				((SPK::SphericEmitter*)currentEmitter)->setAngles(devices->getCore()->getF32(angleA->getText()),
																  devices->getCore()->getF32(angleB->getText()));
			}
		}

		if (event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED) {
			if (event.GUIEvent.Caller == invertedcb && currentEmitterType == ECET_NORMAL) {
				((SPK::NormalEmitter*)currentEmitter)->setInverted(invertedcb->isChecked());
			}
		}
	}

    return false;
}

void CUIAddEmitter::createDirectionGUI() {
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	gui->addStaticText(L"Direction :", rect<s32>(10, 10, 80, 30), true, true, editionZonet, -1, true);
	gui->addStaticText(L"X :", rect<s32>(10, 30, 40, 50), true, true, editionZonet, -1, true);
	dirx = gui->addEditBox(L"0.0", rect<s32>(40, 30, 310, 50), true, editionZonet, -1);
	gui->addStaticText(L"Y :", rect<s32>(10, 50, 40, 70), true, true, editionZonet, -1, true);
	diry = gui->addEditBox(L"0.0", rect<s32>(40, 50, 310, 70), true, editionZonet, -1);
	gui->addStaticText(L"Z :", rect<s32>(10, 70, 40, 90), true, true, editionZonet, -1, true);
	dirz = gui->addEditBox(L"0.0", rect<s32>(40, 70, 310, 90), true, editionZonet, -1);
}

void CUIAddEmitter::createAngleGUI() {
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	gui->addStaticText(L"Angle A :", rect<s32>(10, 110, 70, 130), true, true, editionZonet, -1, true);
	angleA = gui->addEditBox(L"0.0", rect<s32>(70, 110, 310, 130), true, editionZonet, -1);
	gui->addStaticText(L"Angle B :", rect<s32>(10, 130, 70, 150), true, true, editionZonet, -1, true);
	angleB = gui->addEditBox(L"0.0", rect<s32>(70, 130, 310, 150), true, editionZonet, -1);
}

void CUIAddEmitter::clearEditionZone() {
	editionZonet->remove();
	editionZonet = devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(10, 90, 330, 330), true, true, window, -1, true);

	dirx = 0;
	diry = 0;
	dirz = 0;

	angleA = 0;
	angleB = 0;

	invertedcb = 0;
}
