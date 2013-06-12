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

	currentBrowse = -1;
}

CUIWindowEditNodeAnimators::~CUIWindowEditNodeAnimators() {
	
}

void CUIWindowEditNodeAnimators::open(ISceneNode *node, stringw prefix) {
	nodeToEdit = (ISceneNode *)node;

	if (nodeToEdit == 0) {
		devices->getGUIEnvironment()->addMessageBox(L"Warning",
													L"No node to edit, maybe an intern error or your node is out \n\n"
													L"Please try again");
	} else {
		nodeToEditPrefix.append(prefix);
		editWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(450, 100, 1110, 560), false, L"Edit Node's Animators", 0, -1);
		/*
		static text pour du texte : Node Selected : (2, 20, 100, 40) / Animators (10, 50, 140, 70)
		static text pour des zones : 10, 50, 140, 130 / 10, 140, 140, 410 / 150, 50, 650, 410
		combobox : choix du node (110, 20, 650, 40) / choix de l'animator (20, 70, 130, 90)
		bouton : Add (20, 100, 130, 120) / Remove (20, 380, 130, 400) / Apply (10, 420, 110, 450) / Close (120, 420, 220, 450)
		scrolling : fenêtre de droite (620, 60, 640, 400)
		*/
		//Node Selected : (choix du node)
		devices->getGUIEnvironment()->addStaticText(L"Node Selected : ", rect<s32>(2, 20, 110, 40), true, false, editWindow, -1, false);
		nodeSelectedComboBox = devices->getGUIEnvironment()->addComboBox(rect<s32>(110, 20, 650, 40), editWindow, -1);

		//Zone d'ajout d'animator
		devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(10, 50, 140, 130), true, false, editWindow, -1, false);
		devices->getGUIEnvironment()->addStaticText(L"Animators", rect<s32>(10, 50, 140, 70), false, false, editWindow, 1, false);
		animatorSelectedComboBox = devices->getGUIEnvironment()->addComboBox(rect<s32>(20, 70, 130, 90), editWindow, -1);
		addButton = devices->getGUIEnvironment()->addButton(rect<s32>(20, 100, 130, 120), editWindow, -1, L"Add", L"Clic to add selected animator to the node");

		//Zone de liste d'animators associés au node
		devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(10, 140, 140, 410), true, false, editWindow, -1, false);
			//Ajouter liste des animators du node
		removeButton = devices->getGUIEnvironment()->addButton(rect<s32>(20, 380, 130, 400), editWindow, -1, L"Remove", L"Clic to remove selected animator of the node");

		//Zone de paramètres de l'animator sélectionné
		devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(150, 50, 650, 410), true, false, editWindow, -1, false);
		devices->getGUIEnvironment()->addScrollBar(false, rect<s32>(620, 60, 640, 400), editWindow, -1);

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
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if(event.GUIEvent.Caller == applyButton) {
				if(nodeToEdit != 0 /*) {*/ && isValid()) {
					/*stringw volumeLightNodeName = L"#volumeLight:";
					volumeLightNodeName += name;
					nodeToEdit->setName(volumeLightNodeName.c_str());
					nodeToEdit->setSubDivideU(subdivU);
					nodeToEdit->setSubDivideV(subdivV);
					nodeToEdit->setFootColor(SColor(aFootColor, rFootColor, gFootColor, bFootColor));
					nodeToEdit->setTailColor(SColor(aTailColor, rTailColor, gTailColor, bTailColor));
					nodeToEdit->getMaterial(0).setTexture(0, devices->getSceneManager()->getVideoDriver()->getTexture("data/Lights/lightFalloff.png"));
					editWindow->remove();*/
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
