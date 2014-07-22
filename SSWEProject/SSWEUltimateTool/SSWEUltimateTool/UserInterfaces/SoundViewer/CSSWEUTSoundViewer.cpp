//
//  CExporter.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#include "Stdafx.h"
#include "CSSWEUTSoundViewer.h"

using namespace irr;
using namespace gui;
using namespace core;
using namespace scene;
using namespace video;

#define SPECTRUM_SIZE 512

CSoundViewer::CSoundViewer(cp3d::core::IDevices *_devices, stringc _workingDirectory) {
	/// Set datas
	devices = _devices;
	workingDirectory = _workingDirectory;

	/// Audio manager
	amgr = devices->getCoreData()->getAudioManager(0);
	if (!amgr) exit(0);

	audioElement = 0;

	/// Gui
	IGUIEnvironment *gui = devices->getGUIEnvironment();
	(window = gui->addWindow(rect<s32>(500, 190, 1200, 582), false, L"Sound Viewer", 0, -1))->getCloseButton()->setVisible(false);
	devices->getCore()->centerWindow(window, devices->getVideoDriver()->getScreenSize());

	spectrumViewer = new CGUISpectrum(0, devices->getDevice(), window, -1, rect<s32>(10, 30, 522, 350), false);
	(timeline = gui->addScrollBar(true, rect<s32>(10, 360, 530, 380), window, -1))->setEnabled(true);

	loadButton = gui->addButton(rect<s32>(540, 30, 690, 50), window, -1, L"Load...", L"Load a new song to play");
	playButton = gui->addButton(rect<s32>(540, 60, 690, 80), window, -1, L"Play / Pause", L"Play or pause the current sound");
	stopButton = gui->addButton(rect<s32>(540, 90, 690, 110), window, -1, L"Stop", L"Stops the current sound");

	closeButton = gui->addButton(rect<s32>(540, 350, 690, 380), window, -1, L"Close", L"Close this window");

	/// Configure events
	devices->getEventReceiver()->AddEventReceiver(this, window, this);
}

CSoundViewer::~CSoundViewer() {

}

void CSoundViewer::update() {
	if (!audioElement)
		return;

	spectrumViewer->updateAbsolutePosition();

	timeline->setMax(audioElement->getDuration());
	timeline->setPos(audioElement->getCurrentTime());
}

bool CSoundViewer::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {

			if (event.GUIEvent.Caller == loadButton) {
				soundDialog = devices->createFileOpenDialog(L"Load a sound", CGUIFileSelector::EFST_OPEN_DIALOG, 0, true);
				return true;
			} else

			if (event.GUIEvent.Caller == playButton) {
				if (audioElement)
					audioElement->pause();
			} else

			if (event.GUIEvent.Caller == stopButton) {
				if (audioElement) {
					spectrumViewer->setAudioElement(0);
					amgr->removeAudioElement(audioElement);
					audioElement = 0;
				}
			} else

			if (event.GUIEvent.Caller == closeButton) {
				amgr->removeAudioElement(audioElement);
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}

		} else

		if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
			if (event.GUIEvent.Caller == soundDialog) {
				if (audioElement) {
					amgr->removeAudioElement(audioElement);
					audioElement = 0;
				}
				audioElement = amgr->add2DAudioElementFromFile(stringc(soundDialog->getFileName()));
				audioElement->play();
				spectrumViewer->setAudioElement(audioElement);
				return true;
			}
		} else

		if (event.GUIEvent.EventType == EGET_SCROLL_BAR_CHANGED) {
			if (event.GUIEvent.Caller == timeline) {
				if (audioElement)
					audioElement->setCurrentTime(timeline->getPos());
			}
		}
	}

	return false;
}
