//
//  CUIOpenFileDialog.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//
#include "stdafx.h"
#include <cassert>

#include "CExample3.h"

/// Use Irrlicht namespace
using namespace irr;
using namespace core;
using namespace scene;

CExample3::CExample3(cp3d::ISSWECoreUserInterface *core) {
	/// Get the core user interface
	this->core = core;
	/// Get the IDevices interface router
	devices = core->getIDevices();

	/// Add this class to the CP3D event listener
	/// Params : IEventReceiver pointer, IGUIWindow pointer if CP3D GUI Plugin, IUpdate pointer
	devices->getEventReceiver()->AddEventReceiver(this, 0, this);
}

CExample3::~CExample3() {

}

void CExample3::load() {

	/// Get the MP5 Node
	mp5Node = (IAnimatedMeshSceneNode*)devices->getSceneManager()->getSceneNodeFromName("#object:mp5");

	/// Apply IDLE animation
	devices->getAnimationController()->applyAnimationFromNameToModel(mp5Node, "IDLE");

	/// Set collisions to mp5 node (remove)
	devices->getCollisionManager()->getMetaTriangleSelectors()->removeTriangleSelector(mp5Node->getTriangleSelector());
	devices->getFPSCamera()->addChild(mp5Node);

	/// Configure mp5 node position in world
	devices->getFPSCamera()->addChild(mp5Node);
	mp5Node->setPosition(vector3df(3.f, 0.f, 0.f));
	mp5Node->setRotation(vector3df(0.f, -90.f, 0.f));

	reloadCallback = new CReloadCallback(this);
	fireCallback = new CFireCallback(this);
}

void CExample3::run() {
	/// Let CP3D update the devices (SSWE will be replaced by CP3D lol )
	/// It is optimized to use parallelism etc, then better performances =D
	updateSSWEDevice(core);
}

void CExample3::update() {

	/// if is fireing and current frame is > to 500 (0-9440) then re-apply the fire animation
	if (fireCallback->isFireing() && mp5Node->getFrameNr() > 500) {
		devices->getAnimationController()->applyAnimationFromNameToModel(mp5Node, "FIRE");
		mp5Node->setLoopMode(false);
		mp5Node->setAnimationEndCallback(fireCallback);
	}

}

bool CExample3::OnEvent(const SEvent &event) {

	/// Reload if key 'r' is pushed
	if (event.EventType == EET_KEY_INPUT_EVENT) {
		/// Check if player is not still reloaing
		if (event.KeyInput.Key == KEY_KEY_R && !reloadCallback->isReloading()) {
			devices->getAnimationController()->applyAnimationFromNameToModel(mp5Node, "RELOAD");
			mp5Node->setLoopMode(false);
			mp5Node->setAnimationEndCallback(reloadCallback);
			reloadCallback->setIsReloading(true);
		}

	}

	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		
		/// if left mouse button is pressed then fire !
		if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN && !reloadCallback->isReloading()) {
			devices->getAnimationController()->applyAnimationFromNameToModel(mp5Node, "FIRE");
			mp5Node->setLoopMode(false);
			mp5Node->setAnimationEndCallback(fireCallback);
			fireCallback->setIsFireing(true);
		}

		/// If left mouse button is left, stop fire
		if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP) {
			fireCallback->setIsFireing(false);
		}

	}

	return false;
}
