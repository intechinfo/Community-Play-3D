//
//  CUIEditParticleFlags.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/01/2014.
//
//

#include "stdafx.h"
#include "CUIAddModifier.h"

#include "CUIParticleEditor.h"

CUIAddModifier::CUIAddModifier(CDevices *_devices, IGUIElement *parent, SPK::Group *_group, CGUINodesEditor *_editor, CGUINode *_parentNode) {
    devices = _devices;
	group = _group;

	editor = _editor;
	parentNode = _parentNode;

	currentModifier = SPK::Collision::create(1.f, 1.f);
	group->addModifier(currentModifier);

	IGUIEnvironment *gui = devices->getGUIEnvironment();
	//GUI ELEMENTS
	window = gui->addWindow(rect<s32>(530, 170, 1380, 760), true, L"Add Modifier", parent, -1);
	devices->getCore()->centerWindow(window, dimension2du(parent->getRelativePosition().getWidth(),
														  parent->getRelativePosition().getHeight()));

	gui->addStaticText(L"Type :", rect<s32>(420, 30, 470, 50), true, true, window, -1, true);
	modifierTypecb = gui->addComboBox(rect<s32>(470, 30, 840, 50), window, -1);
	modifierTypecb->addItem(L"Collision");
	modifierTypecb->addItem(L"Destroyer");
	modifierTypecb->addItem(L"Linear Force");
	modifierTypecb->addItem(L"Modifier Group");
	modifierTypecb->addItem(L"Obstacle");
	modifierTypecb->addItem(L"Point Mass");
	modifierTypecb->addItem(L"Rotator");
	modifierTypecb->addItem(L"Vortex");

	editZone = gui->addStaticText(L"", rect<s32>(420, 60, 840, 330), true, true, window, -1, true);

	//EVENT RECEIVER
	devices->getEventReceiver()->AddEventReceiver(this);

	addVector3D1("vector1");
	addVector3D2("vector2");
}

CUIAddModifier::~CUIAddModifier() {

}

bool CUIAddModifier::OnEvent(const SEvent &event) {

    return false;
}

void CUIAddModifier::addVector3D1(stringw name) {
	IGUIEnvironment *gui = devices->getGUIEnvironment();
	gui->addStaticText(name.c_str(), rect<s32>(10, 10, 410, 30), true, true, editZone, -1, true);

	gui->addStaticText(L"X :", rect<s32>(20, 30, 50, 50), true, true, editZone, -1, true);
	v1_1 = gui->addEditBox(L"0.0", rect<s32>(50, 30, 410, 50), true, editZone, -1);
	gui->addStaticText(L"Y :", rect<s32>(20, 50, 50, 70), true, true, editZone, -1, true);
	v1_2 = gui->addEditBox(L"0.0", rect<s32>(50, 50, 410, 70), true, editZone, -1);
	gui->addStaticText(L"Z :", rect<s32>(20, 70, 50, 90), true, true, editZone, -1, true);
	v1_3 = gui->addEditBox(L"0.0", rect<s32>(50, 70, 410, 90), true, editZone, -1);
}

void CUIAddModifier::addVector3D2(stringw name) {
	IGUIEnvironment *gui = devices->getGUIEnvironment();
	gui->addStaticText(name.c_str(), rect<s32>(10, 90, 410, 110), true, true, editZone, -1, true);

	gui->addStaticText(L"X :", rect<s32>(20, 110, 50, 130), true, true, editZone, -1, true);
	v2_1 = gui->addEditBox(L"0.0", rect<s32>(50, 110, 410, 130), true, editZone, -1);
	gui->addStaticText(L"Y :", rect<s32>(20, 130, 50, 150), true, true, editZone, -1, true);
	v2_2 = gui->addEditBox(L"0.0", rect<s32>(50, 130, 410, 150), true, editZone, -1);
	gui->addStaticText(L"Z :", rect<s32>(20, 150, 50, 170), true, true, editZone, -1, true);
	v2_3 = gui->addEditBox(L"0.0", rect<s32>(50, 150, 410, 170), true, editZone, -1);
}

void CUIAddModifier::addTwoParameters(bool addedVector1, bool addedVector2) {

}

