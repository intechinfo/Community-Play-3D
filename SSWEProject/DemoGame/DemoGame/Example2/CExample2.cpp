//
//  CUIOpenFileDialog.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//
#include "stdafx.h"
#include <cassert>

#include "CExample2.h"

CExample2::CExample2(ISSWECoreUserInterface *core) {
	/// Get the core user interface
	this->core = core;
	/// Get the IDevices interface router
	devices = core->getIDevices();

	/// Add this class to the event listener of CP3D
	devices->getEventReceiver()->AddEventReceiver(this);
}

CExample2::~CExample2() {

}

void CExample2::load() {
	chiefNode = (irr::scene::IAnimatedMeshSceneNode*)devices->getSceneManager()->getSceneNodeFromName("#object:chief_alone");
	/// Deactivate the animation loop.
	/// If animation loop is activated, animation callbacks will never appear!
	chiefNode->setLoopMode(false);

	/// Add help
	irr::gui::IGUIEnvironment *gui = devices->getGUIEnvironment();
	gui->addStaticText(L"1 - apply animation 1\n2- apply animation 2\n3 - apply with animation callback", irr::core::rect<s32>(0, 0, 500, 100), false, false, 0, -1, true);
}

void CExample2::run() {
	/// Let CP3D update the devices (SSWE will be replaced by CP3D lol )
	/// It is optimized to use parallelism etc, then better performances =D
	updateSSWEDevice(core);
}

bool CExample2::OnEvent(const SEvent &event) {

	/// It's a key input event
	if (event.EventType == EET_KEY_INPUT_EVENT) {

		/// The key wasn't pressed down, but left
		/// Apply animation using two methods : with index and with name
		if (!event.KeyInput.PressedDown) {

			/// Using the name
			if (event.KeyInput.Key == KEY_KEY_1) {
				devices->getAnimationController()->applyAnimationFromNameToModel(chiefNode, "ALL");
				// Is same as : devices->getAnimationController()->applyAnimationToModel(chiefNode, 0);
			}

			/// Using the index
			if (event.KeyInput.Key == KEY_KEY_2) {
				devices->getAnimationController()->applyAnimationToModel(chiefNode, 1);
			}

			/// Using animation callbacks
			if (event.KeyInput.Key == KEY_KEY_3) {
				devices->getAnimationController()->applyAnimationToModel(chiefNode, 0);
				chiefNode->setAnimationEndCallback(new CExample2AnimationCallback(this));
			}
		}
	}

	return false;
}
