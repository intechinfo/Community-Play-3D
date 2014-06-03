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

CExample4::CExample4(ISSWECoreUserInterface *core) {
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
	chief = (IAnimatedMeshSceneNode*)devices->getSceneManager()->getSceneNodeFromName("#object:chief");

	/// Apply IDLE animation
	devices->getAnimationController()->applyAnimationFromNameToModel(chief, "ALL");

	/// Apply ltdn
	boneNode = chief->getJointNode("neck");
	boneNode->addChild(devices->getMayaCamera());
	devices->getSceneManager()->setActiveCamera(devices->getMayaCamera());
}

void CExample4::run() {
	/// Let CP3D update the devices (SSWE will be replaced by CP3D lol )
	/// It is optimized to use parallelism etc, then better performances =D
	updateSSWEDevice(core);
}

void CExample4::update() {
	boneNode->updateAbsolutePosition();
	boneNode->updateAbsolutePositionOfAllChildren();
	//devices->getMayaCamera()->setTarget(boneNode->getAbsolutePosition());
}

bool CExample4::OnEvent(const SEvent &event) {

	return false;
}
