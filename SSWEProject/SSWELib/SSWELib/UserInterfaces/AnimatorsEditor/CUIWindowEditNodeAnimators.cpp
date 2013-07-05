//
//  CUIWindowEditNodeAnimators.cpp
//  Windows
//
//  Created by Arthur Caron on 12/06/13.
//
//

#include "stdafx.h"
#include "CUIWindowEditNodeAnimators.h"

CUIWindowEditNodeAnimators::CUIWindowEditNodeAnimators(CDevices *_devices) {
	devices = _devices;
	devices->getEventReceiver()->AddEventReceiver(this);

	nodeToEdit = 0;
	nodeToEditPrefix = L"";
	animatorSelected = NULL;

	currentBrowse = -1;
}

CUIWindowEditNodeAnimators::~CUIWindowEditNodeAnimators() {
	
}

stringw CUIWindowEditNodeAnimators::getAnimatorNameById(u32 enumIndex) {
	//! Fly circle scene node animator
	if(enumIndex == 0) {
		return "fly circle";
	}
	//! Fly straight scene node animator
	if(enumIndex == 1) {
		return "fly straight";
	}
	//! Follow spline scene node animator
	if(enumIndex == 2) {
		return "follow spline";
	}
	//! Rotation scene node animator
	if(enumIndex == 3) {
		return "rotation";
	}
	//! Texture scene node animator
	if(enumIndex == 4) {
		return "texture";
	}
	//! Deletion scene node animator
	if(enumIndex == 5) {
		return "deletion";
	}
	//! Collision respose scene node animator
	if(enumIndex == 6) {
		return "collision response";
	}
	//! FPS camera animator
	if(enumIndex == 7) {
		return "camera FPS";
	}
	//! Maya camera animator
	if(enumIndex == 8) {
		return "camera MAYA";
	}
	//! Unknown scene node animator
	return "Unknown";
}

u32 CUIWindowEditNodeAnimators::getAnimatorIdByName(stringw name) {
	if(name == "fly circle") {
		return 0;
	}
	if(name == "fly straight") {
		return 1;
	}
	if(name == "follow spline") {
		return 2;
	}
	if(name == "rotation") {
		return 3;
	}
	if(name == "texture") {
		return 4;
	}
	if(name == "deletion") {
		return 5;
	}
	if(name == "collision response") {
		return 6;
	}
	if(name == "camera FPS") {
		return 7;
	}
	if(name == "camera MAYA") {
		return 8;
	}
	//! Unknown scene node animator
	return -1;
}

void CUIWindowEditNodeAnimators::fillMainArea() {
	mainArea = devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(150, 50, 650, 410), true, false, editWindow, -1, false);
	IAttributes *attributes = devices->getDevice()->getFileSystem()->createEmptyAttributes(devices->getVideoDriver());
	if(nodeAnimatorsListBox->getSelected() >= 0 && nodeAnimatorsListBox->getSelected() < nodeAnimators.size()) {
		animatorSelected = nodeAnimators[nodeAnimatorsListBox->getSelected()];
	}
	else {
		if(nodeAnimators.size() > 0) {
			animatorSelected = nodeAnimators[0];
			nodeAnimatorsListBox->setSelected(0);
		}
		else {
			animatorSelected = NULL;
		}
	}
	if(animatorSelected != NULL) {
		animatorSelected->serializeAttributes(attributes, 0);
		arrayGUIElement.clear();
		s32 y = 5; // +30
		s32 y2 = 25; // +30
		for(u32 i = 0; i < attributes->getAttributeCount(); i++) {
			const c8* name = attributes->getAttributeName(i);
			u32 type = attributes->getAttributeType(name);
			if(type == 0) { // INT
				devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				s32 var = attributes->getAttributeAsInt(i);
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp = devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				//intEditBox = devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 1) { // FLOAT
				devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				f32 var = attributes->getAttributeAsFloat(i);
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp = devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				//floatEditBox = devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 2) { // STRING
				devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				stringc var = attributes->getAttributeAsString(i);
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp = devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				//stringEditBox = devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 3) { // BOOL [NOT IMPLEMENTED]
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp;
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				devices->getGUIEnvironment()->addStaticText(L"Bool : not implemented yet", rect<s32>(5, y, 495, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 4) { // ENUM [NOT IMPLEMENTED]
				devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				const c8* var = attributes->getAttributeAsEnumeration(i);
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp = devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				//enumEditBox = devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 5) { // COLOR [NOT IMPLEMENTED]
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp;
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				devices->getGUIEnvironment()->addStaticText(L"Color : not implemented yet", rect<s32>(5, y, 495, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 6) { // COLORF [NOT IMPLEMENTED]
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp;
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				devices->getGUIEnvironment()->addStaticText(L"Colorf : not implemented yet", rect<s32>(5, y, 495, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 7) { // VECTOR3D
				devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				vector3df var = attributes->getAttributeAsVector3d(i);
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp1 = devices->getGUIEnvironment()->addEditBox(stringw(var.X).c_str(), rect<s32>(115, y, 240, y2), true, mainArea, -1);
				IGUIElement *elementTmp2 = devices->getGUIEnvironment()->addEditBox(stringw(var.Y).c_str(), rect<s32>(240, y, 365, y2), true, mainArea, -1);
				IGUIElement *elementTmp3 = devices->getGUIEnvironment()->addEditBox(stringw(var.Z).c_str(), rect<s32>(365, y, 495, y2), true, mainArea, -1);
				arrayTmp.push_back(elementTmp1);
				arrayTmp.push_back(elementTmp2);
				arrayTmp.push_back(elementTmp3);
				arrayGUIElement.push_back(arrayTmp);
				//vector3dXEditBox = devices->getGUIEnvironment()->addEditBox(stringw(varx).c_str(), rect<s32>(115, y, 240, y2), true, mainArea, -1);
				//vector3dYEditBox = devices->getGUIEnvironment()->addEditBox(stringw(vary).c_str(), rect<s32>(240, y, 365, y2), true, mainArea, -1);
				//vector3dZEditBox = devices->getGUIEnvironment()->addEditBox(stringw(varz).c_str(), rect<s32>(365, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 9) { // VECTOR2D
				devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				vector2df var = attributes->getAttributeAsVector2d(i);
				f32 varx = var.X;
				f32 vary = var.Y;
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp1 = devices->getGUIEnvironment()->addEditBox(stringw(var.X).c_str(), rect<s32>(110, y, 300, y2), true, mainArea, -1);
				IGUIElement *elementTmp2 = devices->getGUIEnvironment()->addEditBox(stringw(var.Y).c_str(), rect<s32>(300, y, 490, y2), true, mainArea, -1);
				arrayTmp.push_back(elementTmp1);
				arrayTmp.push_back(elementTmp2);
				arrayGUIElement.push_back(arrayTmp);
				//vector2dXEditBox = devices->getGUIEnvironment()->addEditBox(stringw(varx).c_str(), rect<s32>(110, y, 300, y2), true, mainArea, -1);
				//vector2dYEditBox = devices->getGUIEnvironment()->addEditBox(stringw(vary).c_str(), rect<s32>(300, y, 490, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 10) { // RECT [NOT IMPLEMENTED]
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp;
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				devices->getGUIEnvironment()->addStaticText(L"Rect : not implemented yet", rect<s32>(5, y, 495, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 11) { // MATRIX [NOT IMPLEMENTED]
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp;
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				devices->getGUIEnvironment()->addStaticText(L"Matrix : not implemented yet", rect<s32>(5, y, 495, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 12) { // QUATERNION [NOT IMPLEMENTED]
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp;
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				devices->getGUIEnvironment()->addStaticText(L"Quaternion : not implemented yet", rect<s32>(5, y, 495, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 13) { // BBOX [NOT IMPLEMENTED]
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp;
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				devices->getGUIEnvironment()->addStaticText(L"Bbox : not implemented yet", rect<s32>(5, y, 495, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 14) { // PLANE [NOT IMPLEMENTED]
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp;
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				devices->getGUIEnvironment()->addStaticText(L"Plane : not implemented yet", rect<s32>(5, y, 495, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 15) { // TRIANGLE3D [NOT IMPLEMENTED]
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp;
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				devices->getGUIEnvironment()->addStaticText(L"Triangle3D : not implemented yet", rect<s32>(5, y, 495, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 16) { // LINE2D [NOT IMPLEMENTED]
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp;
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				devices->getGUIEnvironment()->addStaticText(L"Line2D : not implemented yet", rect<s32>(5, y, 495, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 17) { // LINE3D [NOT IMPLEMENTED]
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp;
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				devices->getGUIEnvironment()->addStaticText(L"Line3D : not implemented yet", rect<s32>(5, y, 495, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 18) { // STRINGWARRAY [NOT IMPLEMENTED]
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp;
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				devices->getGUIEnvironment()->addStaticText(L"StringWArray : not implemented yet", rect<s32>(5, y, 495, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 19) { // FLOATARRAY [NOT IMPLEMENTED]
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp;
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				devices->getGUIEnvironment()->addStaticText(L"FloatArray : not implemented yet", rect<s32>(5, y, 495, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 20) { // INTARRAY [NOT IMPLEMENTED]
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp;
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				devices->getGUIEnvironment()->addStaticText(L"IntArray : not implemented yet", rect<s32>(5, y, 495, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 21) { // BINARY [NOT IMPLEMENTED]
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp;
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				devices->getGUIEnvironment()->addStaticText(L"Binary : not implemented yet", rect<s32>(5, y, 495, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 22) { // TEXTURE [NOT IMPLEMENTED]
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp;
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				devices->getGUIEnvironment()->addStaticText(L"Texture : not implemented yet", rect<s32>(5, y, 495, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 23) { // USER_POINTER [NOT IMPLEMENTED]
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp;
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				devices->getGUIEnvironment()->addStaticText(L"UserPointer : not implemented yet", rect<s32>(5, y, 495, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(115, y, 495, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else if(type == 24) { // DIMENSION2D [NOT IMPLEMENTED]
				array<IGUIElement*> arrayTmp;
				IGUIElement *elementTmp;
				arrayTmp.push_back(elementTmp);
				arrayGUIElement.push_back(arrayTmp);
				devices->getGUIEnvironment()->addStaticText(L"Dimension2D : not implemented yet", rect<s32>(5, y, 495, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addStaticText((stringw(name) + L" : ").c_str(), rect<s32>(5, y, 115, y2), true, false, mainArea, -1, false);
				//devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(110, y, 300, y2), true, mainArea, -1);
				//devices->getGUIEnvironment()->addEditBox(stringw(var).c_str(), rect<s32>(300, y, 490, y2), true, mainArea, -1);
				y += 30;
				y2 += 30;
			}
			else { // UNKNOWN
			}
		}
	}
}

void CUIWindowEditNodeAnimators::saveChanges() {
	IAttributes *attributes = devices->getDevice()->getFileSystem()->createEmptyAttributes(devices->getVideoDriver());
	IAttributes *newAttributes = devices->getDevice()->getFileSystem()->createEmptyAttributes(devices->getVideoDriver());
	/*if(nodeAnimatorsListBox->getSelected() >= 0 && nodeAnimatorsListBox->getSelected() < nodeAnimators.size()) {
		animatorSelected = nodeAnimators[nodeAnimatorsListBox->getSelected()];
	}
	else {
		if(nodeAnimators.size() > 0) {
			animatorSelected = nodeAnimators[0];
			nodeAnimatorsListBox->setSelected(0);
		}
		else {
			animatorSelected = NULL;
		}
	}*/
	if(animatorSelected != NULL) {
		animatorSelected->serializeAttributes(attributes, 0);
		CCore *core = devices->getCore();
		for(u32 i = 0; i < attributes->getAttributeCount(); i++) {
			const c8* name = attributes->getAttributeName(i);
			u32 type = attributes->getAttributeType(name);
			if(type == 0) { // INT
				newAttributes->addInt(name, core->getS32(stringc(arrayGUIElement[i][0]->getText()).c_str()));
			}
			else if(type == 1) { // FLOAT
				newAttributes->addFloat(name, core->getF32(stringc(arrayGUIElement[i][0]->getText()).c_str()));
			}
			else if(type == 2) { // STRING
				newAttributes->addString(name, arrayGUIElement[i][0]->getText());
			}
			else if(type == 3) { // BOOL
				newAttributes->addBool(name, attributes->getAttributeAsBool(i));
			}
			/*else if(type == 4) { // ENUM
				newAttributes->addEnum (name, attributes->getAttributeAsEnumeration(i));
			}*/
			else if(type == 5) { // COLOR
				newAttributes->addColor(name, attributes->getAttributeAsColor(i));
			}
			else if(type == 6) { // COLORF
				newAttributes->addColorf(name, attributes->getAttributeAsColorf(i));
			}
			else if(type == 7) { // VECTOR3D
						const wchar_t *t1 = arrayGUIElement[i][0]->getText();
				newAttributes->addVector3d(name, vector3df(core->getF32(stringc(arrayGUIElement[i][0]->getText()).c_str()), 
										core->getF32(stringc(arrayGUIElement[i][1]->getText()).c_str()), core->getF32(stringc(arrayGUIElement[i][2]->getText()).c_str())));
			}
			else if(type == 9) { // VECTOR2D
				newAttributes->addVector2d(name, vector2df(core->getF32(stringc(arrayGUIElement[i][0]->getText()).c_str()), 
										core->getF32(stringc(arrayGUIElement[i][1]->getText()).c_str())));
			}
			else if(type == 10) { // RECT
				newAttributes->addRect(name, attributes->getAttributeAsRect(i));
			}
			else if(type == 11) { // MATRIX
				newAttributes->addMatrix(name, attributes->getAttributeAsMatrix(i));
			}
			else if(type == 12) { // QUATERNION
				newAttributes->addQuaternion(name, attributes->getAttributeAsQuaternion(i));
			}
			else if(type == 13) { // BBOX
				newAttributes->addBox3d(name, attributes->getAttributeAsBox3d(i));
			}
			else if(type == 14) { // PLANE
				newAttributes->addPlane3d(name, attributes->getAttributeAsPlane3d(i));
			}
			else if(type == 15) { // TRIANGLE3D
				newAttributes->addTriangle3d(name, attributes->getAttributeAsTriangle3d(i));
			}
			else if(type == 16) { // LINE2D
				newAttributes->addLine2d(name, attributes->getAttributeAsLine2d(i));
			}
			else if(type == 17) { // LINE3D
				newAttributes->addLine3d(name, attributes->getAttributeAsLine3d(i));
			}
			else if(type == 18) { // STRINGWARRAY
				newAttributes->addArray(name, attributes->getAttributeAsArray(i));
			}
			else if(type == 19) { // FLOATARRAY
				newAttributes->addArray(name, attributes->getAttributeAsArray(i));
			}
			else if(type == 20) { // INTARRAY
				newAttributes->addArray(name, attributes->getAttributeAsArray(i));
			}
			/*else if(type == 21) { // BINARY
				newAttributes->addBinary(name, attributes->getAttributeAsBinaryData(i));
			}*/
			else if(type == 22) { // TEXTURE
				newAttributes->addTexture(name, attributes->getAttributeAsTexture(i));
			}
			else if(type == 23) { // USER_POINTER
				newAttributes->addUserPointer(name, attributes->getAttributeAsUserPointer(i));
			}
			else if(type == 24) { // DIMENSION2D
				newAttributes->addDimension2d(name, attributes->getAttributeAsDimension2d(i));
			}
			else { // UNKNOWN
			}
		}
		animatorSelected->deserializeAttributes(newAttributes, 0);
	}
}

void CUIWindowEditNodeAnimators::open(ISceneNode *node, stringw prefix) {
	nodeToEdit = (ISceneNode *)node;
	
	if (nodeToEdit == 0) {
		devices->getGUIEnvironment()->addMessageBox(L"Warning",
													L"No node to edit, maybe an intern error or your node is out \n\n"
													L"Please try again");
	} else {
		nodeToEditPrefix.append(prefix);
		for(ISceneNodeAnimator *sNA : nodeToEdit->getAnimators()) {
			nodeAnimators.push_back(sNA);
		}
		factory = devices->getSceneManager()->getSceneNodeAnimatorFactory(0);

		for(u32 i = 0; i < factory->getCreatableSceneNodeAnimatorTypeCount(); i++) {
			arrayAnimators.push_back(factory->createSceneNodeAnimator(factory->getCreateableSceneNodeAnimatorType(i), 0));
		}

		editWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(450, 100, 1110, 560), false, L"Edit Node's Animators", 0, -1);
		
		//Zone de choix du node
		devices->getGUIEnvironment()->addStaticText(L"Node Selected : ", rect<s32>(2, 20, 110, 40), true, false, editWindow, -1, false);
		nodeSelectedComboBox = devices->getGUIEnvironment()->addComboBox(rect<s32>(110, 20, 650, 40), editWindow, -1);
		sceneNodeArray = devices->getCoreData()->getAllSceneNodes();
		for(u32 i = 0; i < sceneNodeArray.size(); i++) {
			nodeSelectedComboBox->addItem(stringw(sceneNodeArray[i]->getName()).c_str(), 0);
			if(sceneNodeArray[i]->getName() == nodeToEdit->getName()) {
				nodeSelectedComboBox->setSelected(i);
			}
		}
		
		//Zone d'ajout d'animator
		devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(10, 50, 140, 130), true, false, editWindow, -1, false);
		devices->getGUIEnvironment()->addStaticText(L"Animators", rect<s32>(10, 50, 140, 70), false, false, editWindow, 1, false);
		animatorSelectedComboBox = devices->getGUIEnvironment()->addComboBox(rect<s32>(20, 70, 130, 90), editWindow, -1);
		addButton = devices->getGUIEnvironment()->addButton(rect<s32>(20, 100, 130, 120), editWindow, -1, L"Add", L"Clic to add selected animator to the node");
		for(u32 i = 0; i < arrayAnimators.size(); i++) {
			animatorSelectedComboBox->addItem(getAnimatorNameById(arrayAnimators[i]->getType()).c_str(), 0);
		}

		//Zone de liste d'animators associés au node
		devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(10, 140, 140, 410), true, false, editWindow, -1, false);
		nodeAnimatorsListBox = devices->getGUIEnvironment()->addListBox(rect<s32>(10, 140, 140, 370), editWindow, -1, false);
		removeButton = devices->getGUIEnvironment()->addButton(rect<s32>(20, 380, 130, 400), editWindow, -1, L"Remove", L"Clic to remove selected animator of the node");
		for(u32 i = 0; i < nodeAnimators.size(); i++) {
			nodeAnimatorsListBox->addItem(getAnimatorNameById(nodeAnimators[i]->getType()).c_str(), 0);
		}

		//Zone de paramètres de l'animator sélectionné
		fillMainArea();
		//devices->getGUIEnvironment()->addScrollBar(false, rect<s32>(620, 60, 640, 400), editWindow, -1);

		//Zone des bouttons Accept et Cancel
		applyButton = devices->getGUIEnvironment()->addButton(rect<s32>(10, 420, 110, 450), editWindow, -1, L"Accept", L"Accept and apply modifications");
		closeButton = devices->getGUIEnvironment()->addButton(rect<s32>(120, 420, 220, 450), editWindow, -1, L"Cancel", L"Close this window");
	}
}

bool CUIWindowEditNodeAnimators::isValid() {
	//Virer cette fonction ?
	return true;
}

bool CUIWindowEditNodeAnimators::OnEvent(const SEvent &event) {
	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			saveChanges();
            editWindow->remove();
			devices->getEventReceiver()->RemoveEventReceiver(this);
			return true;
		}

        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if(event.GUIEvent.Caller == addButton) {
				saveChanges();
				mainArea->remove();
				animatorSelected = factory->createSceneNodeAnimator(ESCENE_NODE_ANIMATOR_TYPE(factory->getCreateableSceneNodeAnimatorType(animatorSelectedComboBox->getSelected())), nodeToEdit);
				nodeAnimators.push_back(animatorSelected);
				nodeAnimatorsListBox->addItem(getAnimatorNameById(animatorSelected->getType()).c_str(), 0);
				nodeAnimatorsListBox->setSelected(nodeAnimators.size() - 1);
				fillMainArea();
			}
			if(event.GUIEvent.Caller == removeButton) {
				if(nodeAnimatorsListBox->getSelected() != -1 && nodeAnimatorsListBox->getSelected() < nodeAnimatorsListBox->getItemCount() 
													&& nodeAnimatorsListBox->getSelected() < nodeAnimators.size()) {
					nodeToEdit->removeAnimator(nodeAnimators[nodeAnimatorsListBox->getSelected()]);
					nodeAnimators.erase(nodeAnimatorsListBox->getSelected());
					nodeAnimatorsListBox->removeItem(nodeAnimatorsListBox->getSelected());
					mainArea->remove();
					fillMainArea();
				}
			}
			if(event.GUIEvent.Caller == applyButton) {
				if(nodeToEdit != 0 /*) {*/ && isValid()) {
					saveChanges();
					return true;
				}
            }
			if(event.GUIEvent.Caller == closeButton) {
				saveChanges();
                editWindow->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				return true;
			}
        }
		if(event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED) {
			if(event.GUIEvent.Caller == nodeSelectedComboBox) {
				saveChanges();
				mainArea->remove();
				nodeToEdit = sceneNodeArray[nodeSelectedComboBox->getSelected()];
				nodeAnimators.clear();
				nodeAnimatorsListBox->clear();
				for(ISceneNodeAnimator *sNA : nodeToEdit->getAnimators()) {
					nodeAnimators.push_back(sNA);
				}
				for(u32 i = 0; i < nodeAnimators.size(); i++) {
					nodeAnimatorsListBox->addItem(getAnimatorNameById(nodeAnimators[i]->getType()).c_str(), 0);
				}
				fillMainArea();
			}
		}
		if(event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
			if(event.GUIEvent.Caller == nodeAnimatorsListBox) {
				saveChanges();
				mainArea->remove();
				if(nodeAnimatorsListBox->getSelected() != -1 && nodeAnimatorsListBox->getSelected() < nodeAnimatorsListBox->getItemCount() 
													&& nodeAnimatorsListBox->getSelected() < nodeAnimators.size()) {
				animatorSelected = nodeAnimators[nodeAnimatorsListBox->getSelected()];
				fillMainArea();
				}
			}
		}
    }
	return false;
}
