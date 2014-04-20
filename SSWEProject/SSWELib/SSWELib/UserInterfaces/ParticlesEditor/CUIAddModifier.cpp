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

using namespace cp3d;
using namespace ps;

CUIAddModifier::CUIAddModifier(CDevices *_devices, IGUIElement *parent, SPK::Group *_group, CGUINodesEditor *_editor, CGUINode *_parentNode) {
    devices = _devices;
	group = _group;

	editor = _editor;
	parentNode = _parentNode;

	currentModifier = SPK::Collision::create(1.f, 1.f);
	group->addModifier(currentModifier);

	IGUIEnvironment *gui = devices->getGUIEnvironment();
	//GUI ELEMENTS
	window = gui->addWindow(rect<s32>(520, 160, 960, 540), true, L"Add Modifier", parent, -1);
	devices->getCore()->centerWindow(window, dimension2du(parent->getRelativePosition().getWidth(),
														  parent->getRelativePosition().getHeight()));

	gui->addStaticText(L"Type :", rect<s32>(10, 30, 60, 50), true, true, window, -1, true);
	modifierTypecb = gui->addComboBox(rect<s32>(60, 30, 430, 50), window, -1);
	modifierTypecb->addItem(L"Collision");
	modifierTypecb->addItem(L"Destroyer");
	modifierTypecb->addItem(L"Linear Force");
	//modifierTypecb->addItem(L"Modifier Group");
	modifierTypecb->addItem(L"Obstacle");
	modifierTypecb->addItem(L"Point Mass");
	modifierTypecb->addItem(L"Rotator");
	modifierTypecb->addItem(L"Vortex");

	//BUTTONS
	cancel = gui->addButton(rect<s32>(220, 340, 320, 370), window, -1, L"Cancel", L"Cancel operation");
	accept = gui->addButton(rect<s32>(330, 340, 430, 370), window, -1, L"Accept", L"Accept and add this emitter");

	editZone = 0;
	SEvent event;
	event.EventType = EET_GUI_EVENT;
	event.GUIEvent.EventType = EGET_COMBO_BOX_CHANGED;
	event.GUIEvent.Caller = modifierTypecb;
	OnEvent(event);

	//EVENT RECEIVER
	devices->getEventReceiver()->AddEventReceiver(this);
}

CUIAddModifier::~CUIAddModifier() {

}

void CUIAddModifier::close() {
	window->remove();
	group->removeModifier(currentModifier);
	delete currentModifier;
	devices->getEventReceiver()->RemoveEventReceiver(this);
}

bool CUIAddModifier::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			group->removeModifier(currentModifier);
			delete currentModifier;
			devices->getEventReceiver()->RemoveEventReceiver(this);
			delete this;
		}

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == cancel) {
				window->remove();
				group->removeModifier(currentModifier);
				delete currentModifier;
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}

			if (event.GUIEvent.Caller == accept) {
				createNode();
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
		}

		if (event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED) {
			if (event.GUIEvent.Caller == modifierTypecb) {
				if (editZone)
					editZone->remove();
				editZone = devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(10, 60, 430, 330), true, true, window, -1, true);

				group->removeModifier(currentModifier);
				delete currentModifier;
				currentModifier = 0;

				switch (modifierTypecb->getSelected()) {
				case EGMT_COLLISION:
					currentModifier = SPK::Collision::create();
					currentModifier->setName("Collision Modifier");
					addTwoParameters("Scale", "Elasticity", false, false);
					break;
				case EGMT_DESTROYER:
					currentModifier = SPK::Destroyer::create();
					currentModifier->setName("Destroyer Modifier");
					break;
				case EGMT_LINEAR_FORCE:
					currentModifier = SPK::LinearForce::create();
					currentModifier->setName("Linear Force Modifier");
					addVector3D1("Force :");
					break;
				case EGMT_OBSTACLE:
					currentModifier = SPK::Obstacle::create();
					currentModifier->setName("Obstacle Modifier");
					addTwoParameters("Bounding Ratio", "Friction", false, false);
					break;
				case EGMT_POINT_MASS:
					currentModifier = SPK::PointMass::create();
					addTwoParameters("Mass", "Min Distance", false, false);
					break;
				case EGMT_ROTATOR:
					currentModifier = SPK::Rotator::create();
					break;
				case EGMT_VORTEX:
					currentModifier = SPK::Vortex::create();
					addVector3D1("Position");
					addVector3D2("Direction");
					addTwoParameters("Rotation Speed", "Attraction Speed", true, true);
					break;
				default:break;
				}

				if (currentModifier)
					group->addModifier(currentModifier);
			}
		}
	}

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

void CUIAddModifier::addTwoParameters(stringw name1, stringw name2, bool addedVector1, bool addedVector2) {
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

void CUIAddModifier::modifyVector1(SPK::Vector3D v) {
	SPK::LinearForce *linearForceModifier = dynamic_cast<SPK::LinearForce*>(currentModifier);
	if (linearForceModifier) { // It's a linear force modifier
		linearForceModifier->setForce(v);
	}

	SPK::Vortex *vortexModifier = dynamic_cast<SPK::Vortex*>(currentModifier);
	if (vortexModifier) { // It's a vortex modifier
		vortexModifier->setPosition(v);
	}
}

void CUIAddModifier::modifyVector2(SPK::Vector3D v) {
	SPK::Vortex *vortexModifier = dynamic_cast<SPK::Vortex*>(currentModifier);
	if (vortexModifier) { //It's a vortex modifier
		vortexModifier->setDirection(v);
	}
}

void CUIAddModifier::modifyTwoParameters(float f1, float f2) {
	SPK::Collision *collisionModifier = dynamic_cast<SPK::Collision*>(currentModifier);
	if (collisionModifier) { //It's a collision modifier
		collisionModifier->setScale(f1);
		collisionModifier->setElasticity(f2);
	}

	SPK::Obstacle *obstacleModifier = dynamic_cast<SPK::Obstacle*>(currentModifier);
	if (obstacleModifier) { // It's an obstacle modifier
		obstacleModifier->setBouncingRatio(f1);
		obstacleModifier->setFriction(f2);
	}

	SPK::PointMass *pointMassModifier = dynamic_cast<SPK::PointMass*>(currentModifier);
	if (pointMassModifier) { // It's a point mass modifier
		pointMassModifier->setMass(f1);
		pointMassModifier->setMinDistance(f2);
	}

	SPK::Vortex *vortexModifier = dynamic_cast<SPK::Vortex*>(currentModifier);
	if (vortexModifier) { //It's a vortex modifier
		vortexModifier->setRotationSpeed(f1, true);
		vortexModifier->setAttractionSpeed(f2, true);
	}
}

void CUIAddModifier::fillTriggerCB(IGUIComboBox *cb) {
	cb->addItem(L"Always");
	cb->addItem(L"Inside Zone");
	cb->addItem(L"Outside Zone");
	cb->addItem(L"Intersect Zone");
	cb->addItem(L"Enter Zone");
	cb->addItem(L"Exit Zone");
}

void CUIAddModifier::fillForceFactorCB(IGUIComboBox *cb) {
	cb->addItem(L"Interpolator Life Time");
	cb->addItem(L"Interpolator Age");
	cb->addItem(L"Interpolator Param");
	cb->addItem(L"Interpolator Velocity");
}

void CUIAddModifier::createNode(SPK::Modifier *modifier) {
	if (modifier)
		currentModifier = modifier;

	array<stringw> paramNames;
	paramNames.push_back("Flag None");
    paramNames.push_back("Flag Red");
    paramNames.push_back("Flag Green");
    paramNames.push_back("Flag Blue");
    paramNames.push_back("Flag Alpha");
    paramNames.push_back("Flag Size");
    paramNames.push_back("Flag Mass");
    paramNames.push_back("Flag Angle");
    paramNames.push_back("Flag Texture Index");
    paramNames.push_back("Flag Rotation Speed");

	CGUINode *node = new CGUINode(devices->getGUIEnvironment(), editor, -1);
	node->setName(currentModifier->getName().c_str());
    node->setParent(parentNode);
    node->setData(currentModifier);
	node->setDataType(EPSDT_MODIFIER);
    editor->addNode(node);
    node->addTextField(L"Name :", stringw(node->getName()).c_str());

	SPK::Collision *collisionModifier = dynamic_cast<SPK::Collision*>(currentModifier);
	if (collisionModifier) {
		node->addTextField("Scale", stringw(collisionModifier->getScale()).c_str());
		node->addTextField("Elasticity", stringw(collisionModifier->getElasticity()).c_str());
		return;
	}

	SPK::Destroyer *destroyerModifier = dynamic_cast<SPK::Destroyer*>(currentModifier);
	if (destroyerModifier) {
		node->addButton("Configure Zone...");
		IGUIComboBox *b = node->addComboBox(L"Trigger");
		fillTriggerCB(b);
		b->setSelected(destroyerModifier->getTrigger());
		return;
	}

	SPK::LinearForce *linearForceModifier = dynamic_cast<SPK::LinearForce*>(currentModifier);
	if (linearForceModifier) {
		node->addButton("Configure Zone...");
		IGUIComboBox *b = node->addComboBox(L"Trigger");
		fillTriggerCB(b);
		b->setSelected(linearForceModifier->getTrigger());

		node->addVector3DFields("Force", devices->getCore()->getVector3dfFromSpark(linearForceModifier->getForce()));

		b = node->addComboBox(L"Force Factor");
		fillForceFactorCB(b);

		b = node->addComboBox(L"Param");
		for (u32 i=0; i < paramNames.size(); i++)
			b->addItem(paramNames[i].c_str());

		return;
	}

	SPK::Obstacle *obstacleModifier = dynamic_cast<SPK::Obstacle*>(currentModifier);
	if (obstacleModifier) {
		node->addButton("Configure Zone...");

		IGUIComboBox *b = node->addComboBox(L"Trigger");
		fillTriggerCB(b);

		node->addTextField("Bouncing Ratio", stringw(obstacleModifier->getBouncingRatio()).c_str());
		node->addTextField("Friction", stringw(obstacleModifier->getFriction()).c_str());

		return;
	}

	SPK::PointMass *pointMassModifier = dynamic_cast<SPK::PointMass*>(currentModifier);
	if (pointMassModifier) {
		node->addButton("Configure Zone...");

		IGUIComboBox *b = node->addComboBox(L"Trigger");
		fillTriggerCB(b);

		node->addTextField("Mass", stringw(pointMassModifier->getMass()).c_str());
		node->addTextField("Min Distance", stringw(pointMassModifier->getMinDistance()).c_str());

		return;
	}

	SPK::Vortex *vortexModifier = dynamic_cast<SPK::Vortex*>(currentModifier);
	if (vortexModifier) {
		node->addVector3DFields(L"Position", devices->getCore()->getVector3dfFromSpark(vortexModifier->getPosition()));
		node->addVector3DFields(L"Direction", devices->getCore()->getVector3dfFromSpark(vortexModifier->getDirection()));

		node->addTextField("Rotation Speed", stringw(vortexModifier->getRotationSpeed()).c_str());
		node->addTextField("Attraction Speed", stringw(vortexModifier->getAttractionSpeed()).c_str());

		node->addCheckBox(L"Set Rot Speed Angular", true);
		node->addCheckBox(L"Set Attract Speed Linear", true);

		return;
	}

}
