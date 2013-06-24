//
//  CUIWindowAddVolumeLight.cpp
//  MacOSX
//
//  Created by Arthur Caron on 28/05/2013
//
//
#include "stdafx.h"
#include "CUIWindowAddVolumeLight.h"

CUIWindowAddVolumeLight::CUIWindowAddVolumeLight(CDevices *_devices, IGUIListBox *_lightsListBox) {
    devices = _devices;
    devices->getEventReceiver()->AddEventReceiver(this);
    
    volumeLightsListBox = _lightsListBox;
}

CUIWindowAddVolumeLight::~CUIWindowAddVolumeLight() {
    
}

void CUIWindowAddVolumeLight::open() {
    
    addVolumeLightWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(450, 100, 750, 460), true, L"Add a volume light node", 0, -1);
    
    devices->getGUIEnvironment()->addStaticText(L"Name : ", rect<s32>(5, 25, 55, 50), false, false, addVolumeLightWindow, -1, false);
    volumeLightNameEditBox = devices->getGUIEnvironment()->addEditBox(L"myName", rect<s32>(55, 25, 295, 45), true, addVolumeLightWindow, -1);

	devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(1, 46, 298, 49), true, true, addVolumeLightWindow, -1, true);
    //Parent
	devices->getGUIEnvironment()->addStaticText(L"Parent : ", rect<s32>(5, 55, 65, 80), false, false, addVolumeLightWindow, -1, false);
	volumeLightParentComboBox = devices->getGUIEnvironment()->addComboBox(rect<s32>(65, 50, 295, 75), addVolumeLightWindow, -1);
	volumeLightParentComboBox->addItem(L"root", 0);
	array<ISceneNode*> arrayLights = devices->getCoreData()->getArrayOfLightNodes();
	for(u32 i = 0; i < arrayLights.size(); i++) {
		volumeLightParentComboBox->addItem(stringw(arrayLights[i]->getName()).c_str(), 0);
	}
	//SubdivU
	devices->getGUIEnvironment()->addStaticText(L"Subdiv U : ", rect<s32>(5, 85, 75, 110), false, false, addVolumeLightWindow, -1, false);
    volumeLightSubdivUEditBox = devices->getGUIEnvironment()->addEditBox(L"32", rect<s32>(75, 80, 122, 105), true, addVolumeLightWindow, -1);
	//SubdivV
	devices->getGUIEnvironment()->addStaticText(L"Subdiv V : ", rect<s32>(5, 115, 75, 140), false, false, addVolumeLightWindow, -1, false);
    volumeLightSubdivVEditBox = devices->getGUIEnvironment()->addEditBox(L"32", rect<s32>(75, 110, 122, 135), true, addVolumeLightWindow, -1);

	devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(1, 136, 298, 139), true, true, addVolumeLightWindow, -1, true);
	//FootColor
	devices->getGUIEnvironment()->addStaticText(L"Color Foot (Alpha, Red, Green, Blue) : ", rect<s32>(5, 140, 295, 160), false, false, addVolumeLightWindow, -1, false);
	devices->getGUIEnvironment()->addStaticText(L"A : ", rect<s32>(5, 165, 27, 190), false, false, addVolumeLightWindow, -1, false);
	volumeLightAFootColorEditBox = devices->getGUIEnvironment()->addEditBox(L"255", rect<s32>(27, 160, 74, 185), true, addVolumeLightWindow, -1);
	devices->getGUIEnvironment()->addStaticText(L"R : ", rect<s32>(79, 165, 101, 190), false, false, addVolumeLightWindow, -1, false);
	volumeLightRFootColorEditBox = devices->getGUIEnvironment()->addEditBox(L"255", rect<s32>(101, 160, 146, 185), true, addVolumeLightWindow, -1);
	devices->getGUIEnvironment()->addStaticText(L"G : ", rect<s32>(151, 165, 173, 190), false, false, addVolumeLightWindow, -1, false);
	volumeLightGFootColorEditBox = devices->getGUIEnvironment()->addEditBox(L"255", rect<s32>(173, 160, 218, 185), true, addVolumeLightWindow, -1);
	devices->getGUIEnvironment()->addStaticText(L"B : ", rect<s32>(223, 165, 241, 190), false, false, addVolumeLightWindow, -1, false);
	volumeLightBFootColorEditBox = devices->getGUIEnvironment()->addEditBox(L"255", rect<s32>(241, 160, 290, 185), true, addVolumeLightWindow, -1);
	//TailColor
	devices->getGUIEnvironment()->addStaticText(L"Color Tail (Alpha, Red, Green, Blue) : ", rect<s32>(5, 190, 295, 210), false, false, addVolumeLightWindow, -1, false);
	devices->getGUIEnvironment()->addStaticText(L"A : ", rect<s32>(5, 215, 27, 240), false, false, addVolumeLightWindow, -1, false);
	volumeLightATailColorEditBox = devices->getGUIEnvironment()->addEditBox(L"255", rect<s32>(27, 210, 74, 235), true, addVolumeLightWindow, -1);
	devices->getGUIEnvironment()->addStaticText(L"R : ", rect<s32>(79, 215, 101, 240), false, false, addVolumeLightWindow, -1, false);
	volumeLightRTailColorEditBox = devices->getGUIEnvironment()->addEditBox(L"255", rect<s32>(101, 210, 146, 235), true, addVolumeLightWindow, -1);
	devices->getGUIEnvironment()->addStaticText(L"G : ", rect<s32>(151, 215, 173, 240), false, false, addVolumeLightWindow, -1, false);
	volumeLightGTailColorEditBox = devices->getGUIEnvironment()->addEditBox(L"255", rect<s32>(173, 210, 218, 235), true, addVolumeLightWindow, -1);
	devices->getGUIEnvironment()->addStaticText(L"B : ", rect<s32>(223, 215, 241, 240), false, false, addVolumeLightWindow, -1, false);
	volumeLightBTailColorEditBox = devices->getGUIEnvironment()->addEditBox(L"255", rect<s32>(241, 210, 290, 235), true, addVolumeLightWindow, -1);

	devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(1, 236, 298, 239), true, true, addVolumeLightWindow, -1, true);
	//Position
	devices->getGUIEnvironment()->addStaticText(L"Position : ", rect<s32>(5, 245, 74, 270), false, false, addVolumeLightWindow, -1, false);
	devices->getGUIEnvironment()->addStaticText(L"X : ", rect<s32>(79, 245, 101, 270), false, false, addVolumeLightWindow, -1, false);
	volumeLightXPositionEditBox = devices->getGUIEnvironment()->addEditBox(L"0", rect<s32>(101, 240, 146, 265), true, addVolumeLightWindow, -1);
	devices->getGUIEnvironment()->addStaticText(L"Y : ", rect<s32>(151, 245, 173, 270), false, false, addVolumeLightWindow, -1, false);
	volumeLightYPositionEditBox = devices->getGUIEnvironment()->addEditBox(L"0", rect<s32>(173, 240, 218, 265), true, addVolumeLightWindow, -1);
	devices->getGUIEnvironment()->addStaticText(L"Z : ", rect<s32>(223, 245, 241, 270), false, false, addVolumeLightWindow, -1, false);
	volumeLightZPositionEditBox = devices->getGUIEnvironment()->addEditBox(L"0", rect<s32>(241, 240, 290, 265), true, addVolumeLightWindow, -1);
	//Rotation
	devices->getGUIEnvironment()->addStaticText(L"Rotation : ", rect<s32>(5, 275, 74, 300), false, false, addVolumeLightWindow, -1, false);
	devices->getGUIEnvironment()->addStaticText(L"X : ", rect<s32>(79, 275, 101, 300), false, false, addVolumeLightWindow, -1, false);
	volumeLightXRotationEditBox = devices->getGUIEnvironment()->addEditBox(L"0", rect<s32>(101, 270, 146, 295), true, addVolumeLightWindow, -1);
	devices->getGUIEnvironment()->addStaticText(L"Y : ", rect<s32>(151, 275, 173, 300), false, false, addVolumeLightWindow, -1, false);
	volumeLightYRotationEditBox = devices->getGUIEnvironment()->addEditBox(L"0", rect<s32>(173, 270, 218, 295), true, addVolumeLightWindow, -1);
	devices->getGUIEnvironment()->addStaticText(L"Z : ", rect<s32>(223, 275, 241, 300), false, false, addVolumeLightWindow, -1, false);
	volumeLightZRotationEditBox = devices->getGUIEnvironment()->addEditBox(L"0", rect<s32>(241, 270, 290, 295), true, addVolumeLightWindow, -1);
	//Scale
	devices->getGUIEnvironment()->addStaticText(L"Scale : ", rect<s32>(5, 305, 74, 330), false, false, addVolumeLightWindow, -1, false);
	devices->getGUIEnvironment()->addStaticText(L"X : ", rect<s32>(79, 305, 101, 330), false, false, addVolumeLightWindow, -1, false);
	volumeLightXScaleEditBox = devices->getGUIEnvironment()->addEditBox(L"20", rect<s32>(101, 300, 146, 325), true, addVolumeLightWindow, -1);
	devices->getGUIEnvironment()->addStaticText(L"Y : ", rect<s32>(151, 305, 173, 330), false, false, addVolumeLightWindow, -1, false);
	volumeLightYScaleEditBox = devices->getGUIEnvironment()->addEditBox(L"20", rect<s32>(173, 300, 218, 325), true, addVolumeLightWindow, -1);
	devices->getGUIEnvironment()->addStaticText(L"Z : ", rect<s32>(223, 305, 241, 330), false, false, addVolumeLightWindow, -1, false);
	volumeLightZScaleEditBox = devices->getGUIEnvironment()->addEditBox(L"20", rect<s32>(241, 300, 290, 325), true, addVolumeLightWindow, -1);

    acceptButton = devices->getGUIEnvironment()->addButton(rect<s32>(5, 330, 70, 355), addVolumeLightWindow, -1, L"Accept", L"Accept and add a light node");
    closeButton = devices->getGUIEnvironment()->addButton(rect<s32>(80, 330, 145, 355), addVolumeLightWindow, -1, L"Cancel", L"Close this window");
}

bool CUIWindowAddVolumeLight::isValid() {
	CCore *core = devices->getCore();
	name = stringw(volumeLightNameEditBox->getText()).c_str();
	subdivU = core->getU32(volumeLightSubdivUEditBox->getText());
	subdivV = core->getU32(volumeLightSubdivVEditBox->getText());
	aFootColor = core->getU32(volumeLightAFootColorEditBox->getText());
	rFootColor = core->getU32(volumeLightRFootColorEditBox->getText());
	gFootColor = core->getU32(volumeLightGFootColorEditBox->getText());
	bFootColor = core->getU32(volumeLightBFootColorEditBox->getText());
	aTailColor = core->getU32(volumeLightATailColorEditBox->getText());
	rTailColor = core->getU32(volumeLightRTailColorEditBox->getText());
	gTailColor = core->getU32(volumeLightGTailColorEditBox->getText());
	bTailColor = core->getU32(volumeLightBTailColorEditBox->getText());
	position = core->getVector3df(volumeLightXPositionEditBox->getText(), volumeLightYPositionEditBox->getText(), volumeLightZPositionEditBox->getText());
	rotation = core->getVector3df(volumeLightXRotationEditBox->getText(), volumeLightYRotationEditBox->getText(), volumeLightZRotationEditBox->getText());
	scale = core->getVector3df(volumeLightXScaleEditBox->getText(), volumeLightYScaleEditBox->getText(), volumeLightZScaleEditBox->getText());

	if(name.empty()) {
		devices->addErrorDialog(L"Error Volume Light Creator", L"You must enter a name when creating a Volume Light", EMBF_OK);
		return false;
	}
	else {
		if(subdivU == 0) {
			devices->addErrorDialog(L"Error Volume Light Creator", L"You must enter a value greater than 1 for Subdiv U", EMBF_OK);
			return false;
		}
		else {
			if(subdivV == 0) {
			devices->addErrorDialog(L"Error Volume Light Creator", L"You must enter a value greater than 1 for Subdiv V", EMBF_OK);
			return false;
			}
			else {
				if(aFootColor < 0 || aFootColor > 255 || rFootColor < 0 || rFootColor > 255 || gFootColor < 0 || gFootColor > 255 ||
						bFootColor < 0 || bFootColor > 255 || aTailColor < 0 || aTailColor > 255 || rTailColor < 0 || rTailColor > 255 ||
						gTailColor < 0 || gTailColor > 255 || bTailColor < 0 || bTailColor > 255) {
					devices->addErrorDialog(L"Error Volume Light Creator", L"Color values must be between 0 and 255", EMBF_OK);
					return false;
				}
				else {
					return true;
				}
			}
		}
	}
}

bool CUIWindowAddVolumeLight::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_GUI_EVENT) {
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if(event.GUIEvent.Caller == acceptButton) {
				if(isValid()) {
					IVolumeLightSceneNode *volumeLight = devices->getSceneManager()->addVolumeLightSceneNode(0, -1, subdivU, subdivV, 
																									SColor(aFootColor, rFootColor, gFootColor, bFootColor), 
																									SColor(aTailColor, rTailColor, gTailColor, bTailColor));
					if (volumeLight) {
						volumeLight->setPosition(position);
						volumeLight->setRotation(rotation);
						volumeLight->setScale(scale);
						volumeLight->getMaterial(0).setTexture(0, devices->getSceneManager()->getVideoDriver()->getTexture("data/Lights/lightFalloff.png"));
						stringw volumeLightNodeName = L"#volumeLight:";
						volumeLightNodeName += name;
						volumeLight->setName(volumeLightNodeName.c_str());

						devices->getCoreData()->getVolumeLightsData()->push_back(SVolumeLightsData(volumeLight));
						volumeLightsListBox->addItem(volumeLightNodeName.c_str());
						addVolumeLightWindow->remove();
					}
					return true;
				}
            }
			if(event.GUIEvent.Caller == closeButton) {
                addVolumeLightWindow->remove();
				return true;
			}
        }
    }
    
    return false;
}