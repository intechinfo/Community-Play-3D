//
//  CUIWindowEditVolumeLight.cpp
//  Windows
//
//  Created by Arthur Caron on 10/06/13.
//
//

#include "stdafx.h"
#include "CUIWindowEditVolumeLight.h"

CUIWindowEditVolumeLight::CUIWindowEditVolumeLight(CDevices *_devices) {
	devices = _devices;
	devices->getEventReceiver()->AddEventReceiver(this);


	nodeToEdit = 0;
	nodeToEditPrefix = L"";

	currentBrowse = -1;
}

CUIWindowEditVolumeLight::~CUIWindowEditVolumeLight() {
	
}

void CUIWindowEditVolumeLight::open(ISceneNode *node, stringw prefix) {
	nodeToEdit = (IVolumeLightSceneNode *)node;

	if (nodeToEdit == 0) {
		devices->getGUIEnvironment()->addMessageBox(L"Warning",
													L"No node to edit, maybe an intern error or your node is out \n\n"
													L"Please try again");
	} else {
		nodeToEditPrefix.append(prefix);
		editWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(450, 100, 750, 460), true, L"Add a volume light node", 0, -1);
		//Name
		stringc prefix = devices->getCore()->getNodeNamePrefix(nodeToEdit);
		prefix += ':';
		stringw nodeName = nodeToEdit->getName();
		nodeName.remove(prefix);
		devices->getGUIEnvironment()->addStaticText(L"Name : ", rect<s32>(5, 25, 55, 50), false, false, editWindow, -1, false);
		volumeLightNameEditBox = devices->getGUIEnvironment()->addEditBox(nodeName.c_str(), rect<s32>(55, 25, 295, 45), true, editWindow, -1);

		devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(1, 46, 298, 49), true, true, editWindow, -1, true);
		//Parent
		devices->getGUIEnvironment()->addStaticText(L"Parent : ", rect<s32>(5, 55, 65, 80), false, false, editWindow, -1, false);
		volumeLightParentComboBox = devices->getGUIEnvironment()->addComboBox(rect<s32>(65, 50, 295, 75), editWindow, -1);
		volumeLightParentComboBox->addItem(L"root", 0);
		array<ISceneNode*> arrayLights = devices->getCoreData()->getArrayOfLightNodes();
		for(u32 i = 0; i < arrayLights.size(); i++) {
			volumeLightParentComboBox->addItem(stringw(arrayLights[i]->getName()).c_str(), 0);
		}
		//SubdivU
		devices->getGUIEnvironment()->addStaticText(L"Subdiv U : ", rect<s32>(5, 85, 75, 110), false, false, editWindow, -1, false);
		volumeLightSubdivUEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getSubDivideU()).c_str(), rect<s32>(75, 80, 122, 105), true, editWindow, -1);
		
		//SubdivV
		devices->getGUIEnvironment()->addStaticText(L"Subdiv V : ", rect<s32>(5, 115, 75, 140), false, false, editWindow, -1, false);
		volumeLightSubdivVEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getSubDivideV()).c_str(), rect<s32>(75, 110, 122, 135), true, editWindow, -1);

		devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(1, 136, 298, 139), true, true, editWindow, -1, true);
		//FootColor
		devices->getGUIEnvironment()->addStaticText(L"Color Foot (Alpha, Red, Green, Blue) : ", rect<s32>(5, 140, 295, 160), false, false, editWindow, -1, false);
		devices->getGUIEnvironment()->addStaticText(L"A : ", rect<s32>(5, 165, 27, 190), false, false, editWindow, -1, false);
		volumeLightAFootColorEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getFootColor().getAlpha()).c_str(), rect<s32>(27, 160, 74, 185), true, editWindow, -1);
		devices->getGUIEnvironment()->addStaticText(L"R : ", rect<s32>(79, 165, 101, 190), false, false, editWindow, -1, false);
		volumeLightRFootColorEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getFootColor().getRed()).c_str(), rect<s32>(101, 160, 146, 185), true, editWindow, -1);
		devices->getGUIEnvironment()->addStaticText(L"G : ", rect<s32>(151, 165, 173, 190), false, false, editWindow, -1, false);
		volumeLightGFootColorEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getFootColor().getGreen()).c_str(), rect<s32>(173, 160, 218, 185), true, editWindow, -1);
		devices->getGUIEnvironment()->addStaticText(L"B : ", rect<s32>(223, 165, 241, 190), false, false, editWindow, -1, false);
		volumeLightBFootColorEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getFootColor().getBlue()).c_str(), rect<s32>(241, 160, 290, 185), true, editWindow, -1);
		//TailColor
		devices->getGUIEnvironment()->addStaticText(L"Color Tail (Alpha, Red, Green, Blue) : ", rect<s32>(5, 190, 295, 210), false, false, editWindow, -1, false);
		devices->getGUIEnvironment()->addStaticText(L"A : ", rect<s32>(5, 215, 27, 240), false, false, editWindow, -1, false);
		volumeLightATailColorEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getTailColor().getAlpha()).c_str(), rect<s32>(27, 210, 74, 235), true, editWindow, -1);
		devices->getGUIEnvironment()->addStaticText(L"R : ", rect<s32>(79, 215, 101, 240), false, false, editWindow, -1, false);
		volumeLightRTailColorEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getTailColor().getRed()).c_str(), rect<s32>(101, 210, 146, 235), true, editWindow, -1);
		devices->getGUIEnvironment()->addStaticText(L"G : ", rect<s32>(151, 215, 173, 240), false, false, editWindow, -1, false);
		volumeLightGTailColorEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getTailColor().getGreen()).c_str(), rect<s32>(173, 210, 218, 235), true, editWindow, -1);
		devices->getGUIEnvironment()->addStaticText(L"B : ", rect<s32>(223, 215, 241, 240), false, false, editWindow, -1, false);
		volumeLightBTailColorEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getTailColor().getBlue()).c_str(), rect<s32>(241, 210, 290, 235), true, editWindow, -1);

		devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(1, 236, 298, 239), true, true, editWindow, -1, true);
		
		applyButton = devices->getGUIEnvironment()->addButton(rect<s32>(5, 330, 70, 355), editWindow, -1, L"Accept", L"Accept and add a light node");
		closeButton = devices->getGUIEnvironment()->addButton(rect<s32>(80, 330, 145, 355), editWindow, -1, L"Cancel", L"Close this window");
	}
}

bool CUIWindowEditVolumeLight::isValid() {
	CCore *core = devices->getCore();
	name = stringw(volumeLightNameEditBox->getText());
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

bool CUIWindowEditVolumeLight::OnEvent(const SEvent &event) {
	if (event.EventType == EET_GUI_EVENT) {
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if(event.GUIEvent.Caller == applyButton) {
				if(isValid() && nodeToEdit != 0) {
					stringw volumeLightNodeName = L"#volumeLight:";
					volumeLightNodeName += name;
					nodeToEdit->setName(volumeLightNodeName.c_str());
					nodeToEdit->setSubDivideU(subdivU);
					nodeToEdit->setSubDivideV(subdivV);
					nodeToEdit->setFootColor(SColor(aFootColor, rFootColor, gFootColor, bFootColor));
					nodeToEdit->setTailColor(SColor(aTailColor, rTailColor, gTailColor, bTailColor));
					nodeToEdit->getMaterial(0).setTexture(0, devices->getSceneManager()->getVideoDriver()->getTexture("data/Lights/lightFalloff.png"));
					editWindow->remove();
					return true;
				}
            }
			if(event.GUIEvent.Caller == closeButton) {
                editWindow->remove();
				return true;
			}
        }
    }
	return false;
}
