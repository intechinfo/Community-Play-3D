//
//  CUIOpenFileDialog.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//
#include "stdafx.h"
#include <cassert>

#include "CExample4.h"

/// Use Irrlicht namespace
using namespace irr;
using namespace core;
using namespace scene;

CExample4::CExample4(cp3d::ISSWECoreUserInterface *core) {
	/// Get the core user interface
	this->core = core;
	/// Get the IDevices interface router
	devices = core->getIDevices();

	/// Add this class to the CP3D event listener
	/// Params : IEventReceiver pointer, IGUIWindow pointer if CP3D GUI Plugin, IUpdate pointer
	devices->getEventReceiver()->AddEventReceiver(this, 0, this);
}

CExample4::~CExample4() {

}

void CExample4::load() {

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

	reloading = false;
	fireing = false;
}

void CExample4::run() {
	/// Let CP3D update the devices (SSWE will be replaced by CP3D lol )
	/// It is optimized to use parallelism etc, then better performances =D
	updateSSWEDevice(core);
}

void CExample4::update() {

	/// if is fireing and current frame is > to 500 (0-9440) then re-apply the fire animation
	if (fireing) {
		if (mp5Node->getFrameNr() > 500) {
			devices->getAnimationController()->applyAnimationFromNameToModel(mp5Node, "FIRE");
		}
	}

}

bool CExample4::OnEvent(const SEvent &event) {

	/// Reload if key 'r' is pushed
	if (event.EventType == EET_KEY_INPUT_EVENT) {

		/// Set reload weapon
		if (event.KeyInput.Key == KEY_KEY_R && !reloading) {
			reloading = true;
			mp5Node->setLoopMode(false);
			devices->getAnimationController()->applyAnimationFromNameToModel(mp5Node, "RELOAD");

			/// Set the custom callback (lambda function)
			devices->getAnimationController()->setCustomCallback(mp5Node, [=](irr::scene::IAnimatedMeshSceneNode *node){
				devices->getAnimationController()->applyAnimationFromNameToModel(node, "IDLE");
				reloading = false;
				node->setLoopMode(true);
			});

		}

	}

	if (event.EventType == EET_MOUSE_INPUT_EVENT) {

		if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN && !reloading) {
			fireing = true;
			devices->getAnimationController()->applyAnimationFromNameToModel(mp5Node, "FIRE");
		}

		if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP && !reloading) {
			fireing = false;
			mp5Node->setLoopMode(false);
			devices->getAnimationController()->setCustomCallback(mp5Node, [=](irr::scene::IAnimatedMeshSceneNode *node) {
				devices->getAnimationController()->applyAnimationFromNameToModel(mp5Node, "IDLE");
			});
		}

	}

	return false;
}
