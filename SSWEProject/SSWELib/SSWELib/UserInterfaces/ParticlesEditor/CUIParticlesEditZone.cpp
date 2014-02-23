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

CUIParticlesEditZone::CUIParticlesEditZone(CDevices *_devices, SPK::Emitter *_emitter, IGUIElement *parent) {
    devices = _devices;
	emitter = _emitter;

	zoneTypeNames.push_back("AABOX");
	zoneTypeNames.push_back("CYLINDER");
	zoneTypeNames.push_back("LINE");
	zoneTypeNames.push_back("PLANE");
	zoneTypeNames.push_back("POINT");
	zoneTypeNames.push_back("RING");
	zoneTypeNames.push_back("SPHERE");

	//GUI
	IGUIEnvironment *gui = devices->getGUIEnvironment();
    
	window = gui->addWindow(rect<s32>(550, 210, 830, 410), true, L"Edit Zone", parent, -1);
	window->getCloseButton()->setVisible(false);
	devices->getCore()->centerWindow(window, dimension2du(parent->getRelativePosition().getSize()));

	gui->addStaticText(L"Zone : ", rect<s32>(10, 30, 60, 50), true, true, window, -1, true);
	zonecb = gui->addComboBox(rect<s32>(60, 30, 270, 50), window, -1);
	for (u32 i=0; i < zoneTypeNames.size(); i++) {
		zonecb->addItem(zoneTypeNames[i].c_str());
	}
	zonecb->setSelected(this->getZoneType());

	gui->addStaticText(L"Position : ", rect<s32>(10, 70, 270, 90), true, true, window, -1, true);
	gui->addStaticText(L"X : ", rect<s32>(20, 90, 50, 110), true, true, window, -1, true);
	positionx = gui->addEditBox(stringw(emitter->getZone()->getPosition().x).c_str(), rect<s32>(50, 90, 270, 110), true, window, -1);
	gui->addStaticText(L"Y : ", rect<s32>(20, 110, 50, 130), true, true, window, -1, true);
	positiony = gui->addEditBox(stringw(emitter->getZone()->getPosition().y).c_str(), rect<s32>(50, 110, 270, 130), true, window, -1);
	gui->addStaticText(L"Z : ", rect<s32>(20, 130, 50, 150), true, true, window, -1, true);
	positionz = gui->addEditBox(stringw(emitter->getZone()->getPosition().z).c_str(), rect<s32>(50, 130, 270, 150), true, window, -1);

	accept = gui->addButton(rect<s32>(20, 160, 120, 190), window, -1, L"Accept", L"Accept Zone");
	close = gui->addButton(rect<s32>(160, 160, 260, 190), window, -1, L"Cancel", L"Close this window");

	createExternalGUIElements();

	//FINISH
	devices->getEventReceiver()->AddEventReceiver(this, window);
}

CUIParticlesEditZone::~CUIParticlesEditZone() {

}

CUIParticlesEditZone::E_ZONE_TYPE CUIParticlesEditZone::getZoneType() {
	E_ZONE_TYPE type = EZT_UNKNOWN;
	stringc prefix = getZonePrefix();
	if (prefix == "AABOX") {
		type = EZT_AABOX;
	} else if (prefix == "CYLINDER") {
		type = EZT_CYLINDER;
	} else if (prefix == "LINE") {
		type = EZT_LINE;
	} else if (prefix == "PLANE") {
		type = EZT_PLANE;
	} else if (prefix == "POINT") {
		type = EZT_POINT;
	} else if (prefix == "RING") {
		type = EZT_RING;
	} else if (prefix == "SPHERE") {
		type = EZT_SPHERE;
	} else {
		type = EZT_UNKNOWN;
	}

	zoneType = type;
	return type;
}

CUIParticlesEditZone::SVector3DParams CUIParticlesEditZone::create3DParams(stringw name, s32 relativePositionElementY) {
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	IGUIStaticText *dimensionText = gui->addStaticText(name.c_str(), rect<s32>(10, 70, 270, 90), true, true, window, -1, true);
	IGUIStaticText *xdimensionText = gui->addStaticText(L"X : ", rect<s32>(20, 90, 50, 110), true, true, window, -1, true);
	IGUIEditBox *dimensionx = gui->addEditBox(stringw(emitter->getZone()->getPosition().x).c_str(), rect<s32>(50, 90, 270, 110), true, window, -1);
	IGUIStaticText *ydimensionText = gui->addStaticText(L"Y : ", rect<s32>(20, 110, 50, 130), true, true, window, -1, true);
	IGUIEditBox *dimensiony = gui->addEditBox(stringw(emitter->getZone()->getPosition().y).c_str(), rect<s32>(50, 110, 270, 130), true, window, -1);
	IGUIStaticText *zdimensionText = gui->addStaticText(L"Z : ", rect<s32>(20, 130, 50, 150), true, true, window, -1, true);
	IGUIEditBox *dimensionz = gui->addEditBox(stringw(emitter->getZone()->getPosition().z).c_str(), rect<s32>(50, 130, 270, 150), true, window, -1);

	dimensionText->setRelativePosition(position2di(10, relativePositionElementY+=20));
	xdimensionText->setRelativePosition(position2di(20, relativePositionElementY+20));
	dimensionx->setRelativePosition(position2di(50, relativePositionElementY+=20));
	ydimensionText->setRelativePosition(position2di(20, relativePositionElementY+20));
	dimensiony->setRelativePosition(position2di(50, relativePositionElementY+=20));
	zdimensionText->setRelativePosition(position2di(20, relativePositionElementY+20));
	dimensionz->setRelativePosition(position2di(50, relativePositionElementY+=20));

	otherElements.push_back(xdimensionText);
	otherElements.push_back(ydimensionText);
	otherElements.push_back(zdimensionText);
	elements.push_back(dimensionText);
	elements.push_back(dimensionx);
	elements.push_back(dimensiony);
	elements.push_back(dimensionz);

	SVector3DParams params;
	params.x = dimensionx;
	params.y = dimensiony;
	params.z = dimensionz;

	return params;
}

CUIParticlesEditZone::S2Params CUIParticlesEditZone::create2Params(stringw name1, stringw name2, s32 relativePositionElementY) {
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	IGUIStaticText *radiusText = gui->addStaticText(name1.c_str(), rect<s32>(10, 210, 70, 230), true, true, window, -1, true);
	IGUIEditBox *editRadius = gui->addEditBox(stringw(((SPK::Cylinder*)emitter->getZone())->getRadius()).c_str(), rect<s32>(70, 210, 270, 230), true, window, -1);
	IGUIStaticText *lengthText = gui->addStaticText(name2.c_str(), rect<s32>(10, 210, 70, 230), true, true, window, -1, true);
	IGUIEditBox *editLength = gui->addEditBox(stringw(((SPK::Cylinder*)emitter->getZone())->getLength()).c_str(), rect<s32>(70, 210, 270, 230), true, window, -1);

	radiusText->setRelativePosition(position2di(10, relativePositionElementY+20));
	editRadius->setRelativePosition(position2di(70, relativePositionElementY+=20));
	lengthText->setRelativePosition(position2di(10, relativePositionElementY+20));
	editLength->setRelativePosition(position2di(70, relativePositionElementY+=20));

	otherElements.push_back(radiusText);
	otherElements.push_back(lengthText);

	elements.push_back(editRadius);
	elements.push_back(editLength);

	S2Params params;
	params.param1 = editRadius;
	params.param2 = editLength;

	return params;
}

IGUIElement *CUIParticlesEditZone::create1Param(stringw name, s32 relativePositionElementY) {
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	IGUIStaticText *lengthText = gui->addStaticText(name.c_str(), rect<s32>(10, 210, 70, 230), true, true, window, -1, true);
	IGUIEditBox *editLength = gui->addEditBox(stringw(((SPK::Sphere*)emitter->getZone())->getRadius()).c_str(), rect<s32>(70, 210, 270, 230), true, window, -1);

	lengthText->setRelativePosition(position2di(10, relativePositionElementY));
	editLength->setRelativePosition(position2di(70, relativePositionElementY));

	elements.push_back(editLength);
	otherElements.push_back(lengthText);

	return editLength;
}

void CUIParticlesEditZone::createExternalGUIElements() {
	for (u32 i=0; i < elements.size(); i++) {
		elements[i]->remove();
	}
	for (u32 i=0; i < otherElements.size(); i++) {
		otherElements[i]->remove();
	}
	elements.clear();
	otherElements.clear();

	E_ZONE_TYPE type = getZoneType();
	s32 relativePositionElementY = positionz->getRelativePosition().LowerRightCorner.Y+20;
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	//AABOX
	if (type == EZT_AABOX) {
		SVector3DParams params = this->create3DParams("Dimension :", relativePositionElementY);
		relativePositionElementY+=20*4;

		params.x->setName("AABOX:dimx");
		params.y->setName("AABOX:dimy");
		params.z->setName("AABOX:dimz");
	} else if (type == EZT_CYLINDER) { //CYLINDER
		SVector3DParams params = this->create3DParams("Direction :", relativePositionElementY);
		relativePositionElementY+=20*4;
		S2Params params2 = this->create2Params("Radius", "Length", relativePositionElementY);

		params.x->setName("CYLINDER:dirx");
		params.y->setName("CYLINDER:diry");
		params.z->setName("CYLINDER:dirz");
		params2.param1->setName("CYLINDER:radius");
		params2.param2->setName("CYLINDER:length");
	} else if (type == EZT_LINE) { //LINE
		SVector3DParams params = this->create3DParams("Point 1:", relativePositionElementY);
		relativePositionElementY+=20*4;
		SVector3DParams params2 = this->create3DParams("Point 2:", relativePositionElementY);

		params.x->setName("LINE:p0x");
		params.y->setName("LINE:p0y");
		params.z->setName("LINE:p0z");
		params2.x->setName("LINE:p1x");
		params2.y->setName("LINE:p1y");
		params2.z->setName("LINE:p1z");
	} else if (type == EZT_PLANE) { //PLANE
		SVector3DParams params = this->create3DParams("Normal :", relativePositionElementY);

		params.x->setName("PLANE:normalx");
		params.y->setName("PLANE:normaly");
		params.z->setName("PLANE:normalz");
	} else if (type == EZT_RING) { //RING
		SVector3DParams params = this->create3DParams("Normal :", relativePositionElementY);
		relativePositionElementY+=20*4;
		S2Params params2 = this->create2Params("Min Radius :", "Max radius :", relativePositionElementY);

		params.x->setName("RING:normalx");
		params.y->setName("RING:normaly");
		params.z->setName("RING:normalz");
		params2.param1->setName("RING:minRadius");
		params2.param2->setName("RING:maxRadius");
	} else if (type == EZT_SPHERE) { //SPHERE
		this->create1Param("Radius :", relativePositionElementY)->setName("SPHERE:radius");
	}

	//window->setRelativePosition(rect<s32>(550, 210, 830, 410));
	window->setRelativePosition(rect<s32>(window->getRelativePosition().UpperLeftCorner.X,
										  window->getRelativePosition().UpperLeftCorner.Y,
										  window->getRelativePosition().LowerRightCorner.X,
										  window->getRelativePosition().UpperLeftCorner.Y + (410-210) + 40 + (20*elements.size())
										  )
							   );

	accept->setRelativePosition(rect<s32>(20, 160, 120, 190));
	close->setRelativePosition(rect<s32>(160, 160, 260, 190));
	accept->setRelativePosition(position2di(accept->getRelativePosition().UpperLeftCorner.X, accept->getRelativePosition().UpperLeftCorner.Y + 40 + (20*elements.size())));
	close->setRelativePosition(position2di(close->getRelativePosition().UpperLeftCorner.X, close->getRelativePosition().UpperLeftCorner.Y + + 40 + (20*elements.size())));

}

stringc CUIParticlesEditZone::getZonePrefix() {
	if (emitter->getZone() == 0) {
		return "";
	}
	stringc prefix = "";
	stringc name = emitter->getZone()->getName().c_str();

	for (u32 i=0; i < name.size(); i++) {
		if (name[i] == ':') {
			break;
		} else {
			prefix += emitter->getZone()->getName().at(i);
		}
	}
	return prefix;
}

bool CUIParticlesEditZone::OnEvent(const SEvent &event) {
    
	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
			if (event.GUIEvent.Caller->getParent() == window) {
				stringc name = event.GUIEvent.Caller->getName();
				IGUIEditBox *editbox = (IGUIEditBox*)event.GUIEvent.Caller;

				if (name == "AABOX::dimx" || name == "AABOX:dimy" || name == "AABOX:dimz") {
					SPK::Vector3D v = ((SPK::AABox*)emitter->getZone())->getDimension();
					SPK::AABox *aabox = (SPK::AABox*)emitter->getZone();
					f32 value = devices->getCore()->getF32(editbox->getText());
					if (name == "AABOX:dimx") {
						f32 x = devices->getCore()->getF32(editbox->getText());
						aabox->setDimension(SPK::Vector3D(value, v.y, v.z));
					}
					if (name == "AABOX:dimy") {
						f32 y = devices->getCore()->getF32(editbox->getText());
						aabox->setDimension(SPK::Vector3D(v.x, value, v.z));
					}
					if (name == "AABOX:dimz") {
						f32 z = devices->getCore()->getF32(editbox->getText());
						aabox->setDimension(SPK::Vector3D(v.x, v.y, value));
					}
				}

				if (name == "CYLINDER::dirx" || name == "CYLINDER:diry" || name == "CYLINDER:dirz"
					|| name == "CYLINDER:radius" || name == "CYLINDER:length") {
					SPK::Vector3D v = ((SPK::AABox*)emitter->getZone())->getDimension();
					SPK::Cylinder *cyl = (SPK::Cylinder*)emitter->getZone();
					f32 value = devices->getCore()->getF32(editbox->getText());
					if (name == "CYLINDER:dirx") {
						cyl->setDirection(SPK::Vector3D(value, v.y, v.z));
					}
					if (name == "CYLINDER:diry") {
						cyl->setDirection(SPK::Vector3D(v.x, value, v.z));
					}
					if (name == "CYLINDER:dirz") {
						cyl->setDirection(SPK::Vector3D(v.x, v.y, value));
					}
					if (name == "CYLINDER:radius") {
						cyl->setRadius(value);
					}
					if (name == "CYLINDER:length") {
						cyl->setLength(value);
					}
				}

				if (name == "LINE::p0x" || name == "LINE:p0y" || name == "LINE:p0z"
					|| name == "LINE::p1x" || name == "LINE:p1y" || name == "LINE:p1z") {

					SPK::Vector3D v1 = ((SPK::Line*)emitter->getZone())->getBound(0);
					SPK::Vector3D v2 = ((SPK::Line*)emitter->getZone())->getBound(1);
					SPK::Line *line = (SPK::Line*)emitter->getZone();
					f32 value = devices->getCore()->getF32(editbox->getText());

					if (name == "LINE:p0x") {
						line->setBounds(SPK::Vector3D(value, v1.y, v1.z), SPK::Vector3D(v2.x, v2.y, v2.z));
					}
					if (name == "LINE:p0y") {
						line->setBounds(SPK::Vector3D(v1.x, value, v1.z), SPK::Vector3D(v2.x, v2.y, v2.z));
					}
					if (name == "LINE:p0z") {
						line->setBounds(SPK::Vector3D(v1.x, v1.y, value), SPK::Vector3D(v2.x, v2.y, v2.z));
					}
					if (name == "LINE:p1x") {
						line->setBounds(SPK::Vector3D(v1.x, v1.y, v1.z), SPK::Vector3D(value, v2.y, v2.z));
					}
					if (name == "LINE:p1y") {
						line->setBounds(SPK::Vector3D(v1.x, v1.y, v1.z), SPK::Vector3D(v2.x, value, v2.z));
					}
					if (name == "LINE:p1z") {
						line->setBounds(SPK::Vector3D(v1.x, v1.y, v1.z), SPK::Vector3D(v2.x, v2.y, value));
					}
				}

				if (name == "PLANE::normalx" || name == "PLANE:normaly" || name == "PLANE:normalz") {
					SPK::Vector3D v = ((SPK::Plane*)emitter->getZone())->getNormal();
					SPK::Plane *plane = (SPK::Plane*)emitter->getZone();
					f32 value = devices->getCore()->getF32(editbox->getText());

					if (name == "PLANE:normalx") {
						plane->setNormal(SPK::Vector3D(value, v.y, v.z));
					}
					if (name == "PLANE:normaly") {
						plane->setNormal(SPK::Vector3D(v.x, value, v.z));
					}
					if (name == "PLANE:normalz") {
						plane->setNormal(SPK::Vector3D(v.x, v.y, value));
					}
				}

				if (name == "RING::p0x" || name == "RING:p0y" || name == "RING:p0z"
					|| name == "RING::p1x" || name == "RING:p1y") {

					SPK::Vector3D v = ((SPK::Ring*)emitter->getZone())->getNormal();
					SPK::Ring *ring = (SPK::Ring*)emitter->getZone();
					f32 value = devices->getCore()->getF32(editbox->getText());

					if (name == "RING:normalx") {
						ring->setNormal(SPK::Vector3D(value, v.y, v.z));
					}
					if (name == "RING:normaly") {
						ring->setNormal(SPK::Vector3D(v.x, value, v.z));
					}
					if (name == "RING:normalz") {
						ring->setNormal(SPK::Vector3D(v.x, v.y, value));
					}
					if (name == "RING:minRadius") {
						ring->setRadius(value, ring->getMaxRadius());
					}
					if (name == "RING:maxRadius") {
						ring->setRadius(ring->getMinRadius(), value);
					}
				}
				//params.x->setName("RING:normalx");
				//params.y->setName("RING:normaly");
				//params.z->setName("RING:normalz");
				//params2.param1->setName("RING:minRadius");
				//params2.param2->setName("RING:maxRadius");

				if (name == "SPHERE:radius") {
					SPK::Vector3D v = ((SPK::Sphere*)emitter->getZone())->getRadius();
					SPK::Sphere *plane = (SPK::Sphere*)emitter->getZone();
					f32 value = devices->getCore()->getF32(editbox->getText());


				}
			}
		}

		if (event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED) {
			if (event.GUIEvent.Caller == zonecb) {
				zoneType = (E_ZONE_TYPE)zonecb->getSelected();
				SPK::Zone *zone = 0;
				if (zoneType == EZT_AABOX) {
					zone = SPK::AABox::create(SPK::Vector3D(0, 0, 0), SPK::Vector3D(0, 0, 0));
					zone->setName("AABOX:box");
				} else if (zoneType == EZT_CYLINDER) {
					zone = SPK::Cylinder::create();
					zone->setName("CYLINDER:cylinder");
				} else if (zoneType == EZT_LINE) {
					zone = SPK::Line::create();
					zone->setName("LINE:line");
				} else if (zoneType == EZT_PLANE) {
					zone = SPK::Plane::create();
					zone->setName("PLANE:plane");
				} else if (zoneType == EZT_POINT) {
					zone = SPK::Point::create();
					zone->setName("POINT:point");
				} else if (zoneType == EZT_RING) {
					zone = SPK::Ring::create();
					zone->setName("RING:ring");
				} else if (zoneType == EZT_SPHERE) {
					zone = SPK::Sphere::create();
					zone->setName("SPHERE:sphere");
				}

				if (zone != 0) {
					emitter->setZone(zone);
				} else {
					zoneType = EZT_UNKNOWN;
				}
				createExternalGUIElements();
			}
		}

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == close) {
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
		}
	}

	return false;
}
