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
		editWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(134, 267, 550, 580), true, L"Add a volume light node", 0, -1);
		//Name
		stringc prefix = devices->getCore()->getNodeNamePrefix(nodeToEdit);
		prefix += ':';
		stringw nodeName = nodeToEdit->getName();
		nodeName.remove(prefix);
		devices->getGUIEnvironment()->addStaticText(L"Name : ", rect<s32>(6, 23, 66, 43), false, false, editWindow, -1, false);
		volumeLightNameEditBox = devices->getGUIEnvironment()->addEditBox(nodeName.c_str(), rect<s32>(66, 23, 406, 43), true, editWindow, -1);

		//Parent
		devices->getGUIEnvironment()->addStaticText(L"Parent : ", rect<s32>(6, 53, 66, 73), false, false, editWindow, -1, false);
		volumeLightParentComboBox = devices->getGUIEnvironment()->addComboBox(rect<s32>(66, 53, 406, 73), editWindow, -1);
		volumeLightParentComboBox->addItem(L"root", 0);
		array<ISceneNode*> arrayLights = devices->getCoreData()->getArrayOfLightNodes();
		for(u32 i = 0; i < arrayLights.size(); i++) {
			volumeLightParentComboBox->addItem(stringw(arrayLights[i]->getName()).c_str(), 0);
		}
		//SubdivU
		devices->getGUIEnvironment()->addStaticText(L"Subdiv U : ", rect<s32>(6, 113, 76, 133), false, false, editWindow, -1, false);
		volumeLightSubdivUEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getSubDivideU()).c_str(), rect<s32>(76, 113, 196, 133), true, editWindow, -1);
		
		//SubdivV
		devices->getGUIEnvironment()->addStaticText(L"Subdiv V : ", rect<s32>(206, 113, 276, 134), false, false, editWindow, -1, false);
		volumeLightSubdivVEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getSubDivideV()).c_str(), rect<s32>(276, 113, 396, 133), true, editWindow, -1);

		//FootColor
		devices->getGUIEnvironment()->addStaticText(L"Color Foot (Alpha, Red, Green, Blue) : ", rect<s32>(10, 160, 250, 180), false, false, editWindow, -1, false);
		devices->getGUIEnvironment()->addStaticText(L"A : ", rect<s32>(10, 190, 40, 210), false, false, editWindow, -1, false);
		volumeLightAFootColorEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getFootColor().getAlpha()).c_str(), rect<s32>(40, 190, 110, 210), true, editWindow, -1);
		devices->getGUIEnvironment()->addStaticText(L"R : ", rect<s32>(110, 190, 140, 210), false, false, editWindow, -1, false);
		volumeLightRFootColorEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getFootColor().getRed()).c_str(), rect<s32>(140, 190, 210, 210), true, editWindow, -1);
		devices->getGUIEnvironment()->addStaticText(L"G : ", rect<s32>(210, 190, 240, 210), false, false, editWindow, -1, false);
		volumeLightGFootColorEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getFootColor().getGreen()).c_str(), rect<s32>(240, 190, 310, 210), true, editWindow, -1);
		devices->getGUIEnvironment()->addStaticText(L"B : ", rect<s32>(310, 190, 340, 210), false, false, editWindow, -1, false);
		volumeLightBFootColorEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getFootColor().getBlue()).c_str(), rect<s32>(340, 190, 410, 210), true, editWindow, -1);
		//TailColor
		devices->getGUIEnvironment()->addStaticText(L"Color Tail (Alpha, Red, Green, Blue) : ", rect<s32>(10, 220, 250, 240), false, false, editWindow, -1, false);
		devices->getGUIEnvironment()->addStaticText(L"A : ", rect<s32>(10, 240, 40, 260), false, false, editWindow, -1, false);
		volumeLightATailColorEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getTailColor().getAlpha()).c_str(), rect<s32>(40, 240, 110, 260), true, editWindow, -1);
		devices->getGUIEnvironment()->addStaticText(L"R : ", rect<s32>(110, 240, 140, 260), false, false, editWindow, -1, false);
		volumeLightRTailColorEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getTailColor().getRed()).c_str(), rect<s32>(140, 240, 210, 260), true, editWindow, -1);
		devices->getGUIEnvironment()->addStaticText(L"G : ", rect<s32>(210, 240, 240, 260), false, false, editWindow, -1, false);
		volumeLightGTailColorEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getTailColor().getGreen()).c_str(), rect<s32>(240, 240, 310, 260), true, editWindow, -1);
		devices->getGUIEnvironment()->addStaticText(L"B : ", rect<s32>(310, 240, 340, 260), false, false, editWindow, -1, false);
		volumeLightBTailColorEditBox = devices->getGUIEnvironment()->addEditBox(stringw(nodeToEdit->getTailColor().getBlue()).c_str(), rect<s32>(340, 240, 410, 260), true, editWindow, -1);
		
		applyButton = devices->getGUIEnvironment()->addButton(rect<s32>(6, 273, 86, 300), editWindow, -1, L"Accept", L"Accept and add a light node");
		closeButton = devices->getGUIEnvironment()->addButton(rect<s32>(96, 273, 176, 300), editWindow, -1, L"Close", L"Close this window");
	}
}

bool CUIWindowEditVolumeLight::isValid() {
	CCore *core = devices->getCore();
	name = stringw(volumeLightNameEditBox->getText());
	subdivU = core->getS32(volumeLightSubdivUEditBox->getText());
	subdivV = core->getS32(volumeLightSubdivVEditBox->getText());
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
		if(subdivU < 0) {
			devices->addErrorDialog(L"Error Volume Light Creator", L"You must enter a value greater than 1 for Subdiv U", EMBF_OK);
			return false;
		}
		else {
			if(subdivV < 0) {
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
					SMaterial mat = nodeToEdit->getMaterial(0);
					nodeToEdit->setName(volumeLightNodeName.c_str());
					nodeToEdit->setSubDivideU(subdivU);
					nodeToEdit->setSubDivideV(subdivV);
					nodeToEdit->setFootColor(SColor(aFootColor, rFootColor, gFootColor, bFootColor));
					nodeToEdit->setTailColor(SColor(aTailColor, rTailColor, gTailColor, bTailColor));
					nodeToEdit->getMaterial(0) = mat;
				}
            }
			if(event.GUIEvent.Caller == closeButton) {
                editWindow->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
        }
    }
	return false;
}
