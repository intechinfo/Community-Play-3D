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

#include "../../GUIExtension/SoundViewer/CGUISoundViewer.h"

CUIAddAudioElement::CUIAddAudioElement(CDevices *_devices) {
	devices = _devices;
	gui = devices->getGUIEnvironment();

	audioElement = 0;
	//-----------------------------------
    /// Gui Elements

	window = gui->addWindow(rect<s32>(450, 500, 1100, 670), false, L"Add a new sound", 0, -1);
	devices->getCore()->centerWindow(window, devices->getVideoDriver()->getScreenSize());

	soundViewer = new CGUISoundViewer(audioElement, devices->getDevice(), window, rect<s32>(10, 30, 522, 110));

	open = gui->addButton(rect<s32>(540, 30, 640, 50), window, -1, L"Open...", L"Open the sound to add...");
	loopMode = gui->addCheckBox(true, rect<s32>(540, 60, 640, 80), window, -1, L"Loop Mode");
	loadAs3D = gui->addCheckBox(true, rect<s32>(540, 90, 640, 110), window, -1, L"Load as 3D");

	informations = gui->addStaticText(L"", rect<s32>(230, 120, 640, 160), true, true, window, -1, true);

	accept = gui->addButton(rect<s32>(10, 130, 110, 160), window, -1, L"OK", L"Add the sound to the scene");
	close = gui->addButton(rect<s32>(120, 130, 220, 160), window, -1, L"Close", L"Cancel operation");

	//-----------------------------------

	devices->getEventReceiver()->AddEventReceiver(this, window);
}

CUIAddAudioElement::~CUIAddAudioElement() {

}

void CUIAddAudioElement::closeWindow() {
	if (audioElement)
		devices->getCoreData()->getAudioPlugins()->operator[](0).getAudioManager()->removeAudioElement(audioElement);

	if (window)
		window->remove();
	devices->getEventReceiver()->RemoveEventReceiver(this);
	delete this;
}

void CUIAddAudioElement::removeAudioElement() {
	if (!audioElement) return;

	if (devices->getCoreData()->getAudioPlugins()->size() > 0) {
		devices->getCoreData()->getAudioPlugins()->operator[](0).getAudioManager()->removeAudioElement(audioElement);
	}

	soundViewer->setAudioElement(0);
}

bool CUIAddAudioElement::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window) {
				window = 0;
				closeWindow();
			}
		}

		if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
			if (event.GUIEvent.Caller == openFromFileDialog) {

				if (devices->getCoreData()->getAudioPlugins()->size() > 0) {
					removeAudioElement();

					irr::core::stringc path = openFromFileDialog->getFileName();

					audioElement = devices->getCoreData()->getAudioPlugins()->operator[](0)
						.getAudioManager()->add2DAudioElementFromFile(path);
					audioElement->play();
					audioElement->setLoopMode(true);

					soundViewer->setAudioElement(audioElement);

					informations->setText(stringw(path).c_str());

				} else {
					devices->addErrorDialog(L"No audio plugin loaded", L"No audio plugin loaded, please add minimum one audio plugin", EMBF_OK);
				}

			}
		}

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			/// Choose sound
			if (event.GUIEvent.Caller == open) {
				openFromFileDialog = devices->createFileOpenDialog(L"Select your sound...", CGUIFileSelector::EFST_OPEN_DIALOG, 0, false);
			}

			/// Audio Element chosen
			if (event.GUIEvent.Caller == accept) {
				if (audioElement) {

					SAudio a(devices->getSceneManager());
					a.setPath(audioElement->getPath());

					removeAudioElement();

					cp3d::audio::IAudioElement *element = 0;
					if (loadAs3D->isChecked()) {
						element = devices->getCoreData()->getAudioPlugins()->operator[](0)
							.getAudioManager()->add3DAudioElementFromFile(a.getPath(), devices->getSceneManager());
						element->play();
					} else {
						element = devices->getCoreData()->getAudioPlugins()->operator[](0)
							.getAudioManager()->add2DAudioElementFromFile(a.getPath());
					}

					if (loopMode->isChecked())
						element->setLoopMode(true);

					a.setAudioElement(element);

					devices->getCoreData()->getAudioData()->push_back(a);

					devices->getEventReceiver()->sendUserEvent(ECUE_NODE_ADDED);

					closeWindow();
				}
			}

			if (event.GUIEvent.Caller == close) {
				closeWindow();
			}
		}
	}

	return false;
}
