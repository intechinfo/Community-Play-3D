/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include "CUIAddAudioElement.h"

CUIAddAudioElement::CUIAddAudioElement(CDevices *_devices) {
	devices = _devices;
	gui = devices->getGUIEnvironment();

	devices->getEventReceiver()->AddEventReceiver(this);
}

CUIAddAudioElement::~CUIAddAudioElement() {

}

void CUIAddAudioElement::open() {
	openFromFileDialog = devices->createFileOpenDialog(L"Select your sound...", CGUIFileSelector::EFST_OPEN_DIALOG, 0, false);
}

bool CUIAddAudioElement::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
			if (event.GUIEvent.Caller == openFromFileDialog) {

				if (devices->getCoreData()->getAudioPlugins()->size() > 0) {

					SAudio a(devices->getSceneManager());
					a.setPath(stringc(openFromFileDialog->getFileName()));

					cp3d::audio::IAudioElement *element;
					element = devices->getCoreData()->getAudioPlugins()->operator[](0).getAudioManager()->add3DAudioElementFromFile(a.getPath(),
																																	devices->getSceneManager());
					element->play();

					a.setAudioElement(element);

					devices->getCoreData()->getAudioData()->push_back(a);

					devices->getEventReceiver()->sendUserEvent(ECUE_NODE_ADDED);

				} else {
					devices->addErrorDialog(L"No audio plugin loaded", L"No audio plugin loaded, please add minimum one audio plugin", EMBF_OK);
				}

				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
		}
	}

	return false;
}
