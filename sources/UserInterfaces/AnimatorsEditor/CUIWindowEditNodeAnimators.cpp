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

//test ->
		/*
		ISceneNodeAnimatorFactory *factory;
		ISceneNodeAnimator *a;
		ISceneNodeAnimator *b;
		ISceneNodeAnimator *c;
		a = factory->createSceneNodeAnimator(0, 0);
		b = factory->createSceneNodeAnimator(0, 0);
		c = factory->createSceneNodeAnimator(0, 0);
		IAttributes *ia;
		IAttributes *ib;
		IAttributes *ic;
		a->deserializeAttributes(ia);
		b->deserializeAttributes(ib);
		c->deserializeAttributes(ic);
		ia->addBool("test", true);
		ib->addBool("test", false);
		ic->addBool("testeuuh", true);
		ia->addString("name", "a");
		ib->addString("name", "b");
		ic->addString("name", "c");
		*/
//<- test

		editWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(450, 100, 1110, 560), false, L"Edit Node's Animators", 0, -1);
		/*
		static text pour du texte : Node Selected : (2, 20, 100, 40) / Animators (10, 50, 140, 70)
		static text pour des zones : 10, 50, 140, 130 / 10, 140, 140, 410 / 150, 50, 650, 410
		combobox : choix du node (110, 20, 650, 40) / choix de l'animator (20, 70, 130, 90)
		bouton : Add (20, 100, 130, 120) / Remove (20, 380, 130, 400) / Apply (10, 420, 110, 450) / Close (120, 420, 220, 450)
		scrolling : fen�tre de droite (620, 60, 640, 400)
		*/
		//Node Selected : (choix du node)
		devices->getGUIEnvironment()->addStaticText(L"Node Selected : ", rect<s32>(2, 20, 110, 40), true, false, editWindow, -1, false);
		nodeSelectedComboBox = devices->getGUIEnvironment()->addComboBox(rect<s32>(110, 20, 650, 40), editWindow, -1);
		array<ISceneNode*> sceneNodeArray = devices->getCoreData()->getAllSceneNodes();
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

//test ->
		/*
		IAttributes *ia2;
		IAttributes *ib2;
		IAttributes *ic2;
		a->serializeAttributes(ia2);
		b->serializeAttributes(ib2);
		c->serializeAttributes(ic2);
		nodeSelectedComboBox->addItem(stringw(ia2->getAttributeAsString("name")).c_str(), 0);
		nodeSelectedComboBox->addItem(stringw(ib2->getAttributeAsString("name")).c_str(), 0);
		nodeSelectedComboBox->addItem(stringw(ic2->getAttributeAsString("name")).c_str(), 0);
		*/
//<- test

		//Zone de liste d'animators associ�s au node
		devices->getGUIEnvironment()->addStaticText(L"", rect<s32>(10, 140, 140, 410), true, false, editWindow, -1, false);
			//Ajouter liste des animators du node
		removeButton = devices->getGUIEnvironment()->addButton(rect<s32>(20, 380, 130, 400), editWindow, -1, L"Remove", L"Clic to remove selected animator of the node");

		//Zone de param�tres de l'animator s�lectionn�
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
