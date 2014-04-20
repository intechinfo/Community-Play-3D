/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "stdafx.h"
#include "CUIParticlesEditZone.h"

using namespace SPK;
using namespace SPK::IRR;

using namespace cp3d;
using namespace ps;

CUIParticlesEditZone::CUIParticlesEditZone(CDevices *_devices, SPK::Emitter *_emitter, IGUIElement *parent) {
    devices = _devices;

	emitter = _emitter;
	existedZone = emitter->getZone();

	currentZone = SPK::AABox::create();
	emitter->setZone(currentZone);

	zoneTypeNames.push_back("AABOX");
	zoneTypeNames.push_back("CYLINDER");
	zoneTypeNames.push_back("LINE");
	zoneTypeNames.push_back("PLANE");
	zoneTypeNames.push_back("POINT");
	zoneTypeNames.push_back("RING");
	zoneTypeNames.push_back("SPHERE");

	//GUI
	IGUIEnvironment *gui = devices->getGUIEnvironment();
    
	window = gui->addWindow(rect<s32>(520, 160, 960, 540), true, L"Edit Zone", parent, -1);
	devices->getCore()->centerWindow(window, dimension2du(parent->getRelativePosition().getWidth(),
														  parent->getRelativePosition().getHeight()));

	gui->addStaticText(L"Zone : ", rect<s32>(10, 30, 60, 50), true, true, window, -1, true);
	zonecb = gui->addComboBox(rect<s32>(60, 30, 430, 50), window, -1);
	for (u32 i=0; i < zoneTypeNames.size(); i++) {
		zonecb->addItem(zoneTypeNames[i].c_str());
	}

	editZone = 0;
	SEvent event;
	event.EventType = EET_GUI_EVENT;
	event.GUIEvent.EventType = EGET_COMBO_BOX_CHANGED;
	event.GUIEvent.Caller = zonecb;
	OnEvent(event);

	accept = gui->addButton(rect<s32>(330, 340, 430, 370), window, -1, L"Accept", L"Accept Zone");
	close = gui->addButton(rect<s32>(220, 340, 320, 370), window, -1, L"Cancel", L"Close this window");

	//FINISH
	devices->getEventReceiver()->AddEventReceiver(this, window);
}

CUIParticlesEditZone::~CUIParticlesEditZone() {

}

bool CUIParticlesEditZone::OnEvent(const SEvent &event) {
	if (event.EventType == EET_GUI_EVENT) {

		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			emitter->setZone(existedZone);
			delete currentZone;
			devices->getEventReceiver()->RemoveEventReceiver(this);
			delete this;
		}

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == close) {
				window->remove();
				emitter->setZone(existedZone);
				delete currentZone;
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}

			if (event.GUIEvent.Caller == accept) {
				emitter->setZone(currentZone);
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
		}

		if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
			IGUIElement *caller = event.GUIEvent.Caller;
			if (caller == v1_1 || caller == v1_2 || caller == v1_3) {
				SPK::Vector3D v(devices->getCore()->getF32(v1_1->getText()),
								devices->getCore()->getF32(v1_2->getText()),
								devices->getCore()->getF32(v1_3->getText()));
				modifyVector1(v);
			}

			if (caller == v2_1 || caller == v2_2 || caller == v2_3) {
				SPK::Vector3D v(devices->getCore()->getF32(v2_1->getText()),
								devices->getCore()->getF32(v2_2->getText()),
								devices->getCore()->getF32(v2_3->getText()));
				modifyVector2(v);
			}

			if (caller == eb1 || caller == eb2) {
				modifyTwoParameters(devices->getCore()->getF32(eb1->getText()),
									devices->getCore()->getF32(eb2->getText()));
			}

			if (caller == eb3) {
				((SPK::Sphere*)currentZone)->setRadius(devices->getCore()->getF32(eb3->getText()));
			}
		}

		if (event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED) {
			if (event.GUIEvent.Caller == zonecb) {
				if (editZone)
					editZone->remove();
				editZone = devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(10, 60, 430, 330), true, true, window, -1, true);

				emitter->setZone(0);
				delete currentZone;
				currentZone = 0;

				switch (zonecb->getSelected()) {
				case EZT_AABOX:
					currentZone = SPK::AABox::create();
					addVector3D1("Position");
					addVector3D2("Dimension");
					break;
				case EZT_CYLINDER:
					currentZone = SPK::Cylinder::create();
					addVector3D1("Position");
					addVector3D2("Dimension");
					addTwoParameters("Radius", "Length", true, true);
					break;
				case EZT_LINE:
					currentZone = SPK::Line::create();
					addVector3D1("Point 1");
					addVector3D2("Point 2");
					break;
				case EZT_PLANE:
					currentZone = SPK::Plane::create();
					addVector3D1("Position");
					addVector3D2("Normal");
					break;
				case EZT_POINT:
					currentZone = SPK::Point::create();
					addVector3D1("Position");
					break;
				case EZT_RING:
					currentZone = SPK::Ring::create();
					addVector3D1("Position");
					addVector3D2("Normal");
					addTwoParameters("Min Radius", "Max Radius", true, true);
					break;
				case EZT_SPHERE: {
					currentZone = SPK::Sphere::create();
					addVector3D1("Position");
					s32 yPosition = 90;
					devices->getGUIEnvironment()->addStaticText(L"Radius", rect<s32>(10, yPosition, 180, yPosition+20), true, true, editZone, -1, true);
					eb3 = devices->getGUIEnvironment()->addEditBox(L"0.0", rect<s32>(180, yPosition, 410, yPosition+20), true, editZone, -1);
				}
					break;
				default:break;
				}

				if (currentZone)
					emitter->setZone(currentZone);
			}
		}
	}
	return false;
}

void CUIParticlesEditZone::addVector3D1(stringw name) {
	IGUIEnvironment *gui = devices->getGUIEnvironment();
	gui->addStaticText(name.c_str(), rect<s32>(10, 10, 410, 30), true, true, editZone, -1, true);

	gui->addStaticText(L"X :", rect<s32>(20, 30, 50, 50), true, true, editZone, -1, true);
	v1_1 = gui->addEditBox(L"0.0", rect<s32>(50, 30, 410, 50), true, editZone, -1);
	gui->addStaticText(L"Y :", rect<s32>(20, 50, 50, 70), true, true, editZone, -1, true);
	v1_2 = gui->addEditBox(L"0.0", rect<s32>(50, 50, 410, 70), true, editZone, -1);
	gui->addStaticText(L"Z :", rect<s32>(20, 70, 50, 90), true, true, editZone, -1, true);
	v1_3 = gui->addEditBox(L"0.0", rect<s32>(50, 70, 410, 90), true, editZone, -1);
}

void CUIParticlesEditZone::addVector3D2(stringw name) {
	IGUIEnvironment *gui = devices->getGUIEnvironment();
	gui->addStaticText(name.c_str(), rect<s32>(10, 90, 410, 110), true, true, editZone, -1, true);

	gui->addStaticText(L"X :", rect<s32>(20, 110, 50, 130), true, true, editZone, -1, true);
	v2_1 = gui->addEditBox(L"0.0", rect<s32>(50, 110, 410, 130), true, editZone, -1);
	gui->addStaticText(L"Y :", rect<s32>(20, 130, 50, 150), true, true, editZone, -1, true);
	v2_2 = gui->addEditBox(L"0.0", rect<s32>(50, 130, 410, 150), true, editZone, -1);
	gui->addStaticText(L"Z :", rect<s32>(20, 150, 50, 170), true, true, editZone, -1, true);
	v2_3 = gui->addEditBox(L"0.0", rect<s32>(50, 150, 410, 170), true, editZone, -1);
}

void CUIParticlesEditZone::addTwoParameters(stringw name1, stringw name2, bool addedVector1, bool addedVector2) {
	IGUIEnvironment *gui = devices->getGUIEnvironment();
	irr::s32 yPosition = 10;

	if (addedVector1)
		yPosition = 90;
	if (addedVector2)
		yPosition = 170;

	gui->addStaticText(name1.c_str(), rect<s32>(10, yPosition, 180, yPosition+20), true, true, editZone, -1, true);
	eb1 = gui->addEditBox(L"0.0", rect<s32>(180, yPosition, 410, yPosition+20), true, editZone, -1);
	gui->addStaticText(name2.c_str(), rect<s32>(10, yPosition+20, 180, yPosition+40), true, true, editZone, -1, true);
	eb2 = gui->addEditBox(L"0.0", rect<s32>(180, yPosition+20, 410, yPosition+40), true, editZone, -1);
}

void CUIParticlesEditZone::modifyVector1(SPK::Vector3D v) {
	SPK::AABox *aabox = dynamic_cast<SPK::AABox*>(currentZone);
	if (aabox) {
		aabox->setPosition(v);
	}

	SPK::Cylinder *cylinder = dynamic_cast<SPK::Cylinder*>(currentZone);
	if (cylinder) {
		cylinder->setPosition(v);
	}

	SPK::Line *line = dynamic_cast<SPK::Line*>(currentZone);
	if (line) {
		line->setBounds(v, line->getBound(1));
	}

	SPK::Plane *plane = dynamic_cast<SPK::Plane*>(currentZone);
	if (plane) {
		plane->setPosition(v);
	}

	SPK::Point *point = dynamic_cast<SPK::Point*>(currentZone);
	if (point) {
		point->setPosition(v);
	}

	SPK::Ring *ring = dynamic_cast<SPK::Ring*>(currentZone);
	if (ring) {
		ring->setPosition(v);
	}

	SPK::Sphere *sphere = dynamic_cast<SPK::Sphere*>(currentZone);
	if (sphere) {
		sphere->setPosition(v);
	}

}

void CUIParticlesEditZone::modifyVector2(SPK::Vector3D v) {
	SPK::AABox *aabox = dynamic_cast<SPK::AABox*>(currentZone);
	if (aabox) {
		aabox->setDimension(v);
	}

	SPK::Cylinder *cylinder = dynamic_cast<SPK::Cylinder*>(currentZone);
	if (cylinder) {
		cylinder->setDirection(v);
	}

	SPK::Line *line = dynamic_cast<SPK::Line*>(currentZone);
	if (line) {
		line->setBounds(line->getBound(0), v);
	}

	SPK::Plane *plane = dynamic_cast<SPK::Plane*>(currentZone);
	if (plane) {
		plane->setNormal(v);
	}

	SPK::Ring *ring = dynamic_cast<SPK::Ring*>(currentZone);
	if (ring) {
		ring->setNormal(v);
	}
}

void CUIParticlesEditZone::modifyTwoParameters(float f1, float f2) {
	SPK::Cylinder *cylinder = dynamic_cast<SPK::Cylinder*>(currentZone);
	if (cylinder) {
		cylinder->setRadius(f1);
		cylinder->setLength(f2);
	}

	SPK::Ring *ring = dynamic_cast<SPK::Ring*>(currentZone);
	if (ring) {
		ring->setRadius(f1, f2);
	}
}
