//
//  CUIOpenFileDialog.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//
#include "stdafx.h"
#include <cassert>

#include "CExample6.h"

/// Use Irrlicht namespace
using namespace irr;
using namespace core;
using namespace scene;

CExample6::CExample6(cp3d::ISSWECoreUserInterface *core) {
	/// Get the core user interface
	this->core = core;
	/// Get the IDevices interface router
	devices = core->getIDevices();

	/// Initialize data
	direction = 0;
	zdirection = 0;

	/// Register the class
	devices->getEventReceiver()->AddEventReceiver(this, 0, this);
}

CExample6::~CExample6() {

}

void CExample6::load() {
	/// Import scene
	ISSWEImporter *importer = core->createImporter();
	importer->importScene("Example6.world");
	delete importer;

	/// Get and create nodes
	player = (IAnimatedMeshSceneNode*)devices->getSceneManager()->getSceneNodeFromName("#object:murphy");
	if (!player)
		exit(0);
	devices->getAnimationController()->applyAnimationFromNameToModel(player, "IDLE");

	boneNode = player->getJointNode("Head");

	camera = devices->getSceneManager()->addCameraSceneNode();

	/// Hide cursor
	devices->getDevice()->getCursorControl()->setVisible(false);
	/// Configure HDR
	devices->getXEffect()->getHDRManager()->setExposure(1.2f);
}

void CExample6::run() {
	/// Let CP3D update the devices (SSWE will be replaced by CP3D lol )
	/// It is optimized to use parallelism etc, then better performances =D
	updateSSWEDevice(core);
}

void CExample6::update() {
	/// Update bone (head)
	boneNode->updateAbsolutePosition();

	//-----------------------------------
	/// Update camera
	core::position2d<f32> cursorPos = devices->getDevice()->getCursorControl()->getRelativePosition();
     
	f32 change_x = (cursorPos.X - 0.5f) * 256.0f;
	f32 change_y = (cursorPos.Y - 0.5f) * 256.0f;

	direction += change_x;
	zdirection -= change_y;

	if(zdirection < -90)
		zdirection = -90;
	else
	if(zdirection > 90)
		zdirection = 90;

	devices->getDevice()->getCursorControl()->setPosition(0.5f, 0.5f);

	vector3df playerPos = player->getPosition();

	f32 xf = playerPos.X - cos(direction * core::PI / 180.0f) * 44.0f * player->getScale().X;
	f32 yf = playerPos.Y - sin(zdirection * core::PI / 180.0f) * 44.0f * player->getScale().X * 2.f;
	f32 zf = playerPos.Z + sin(direction * core::PI / 180.0f) * 44.0f * player->getScale().X;

	if (yf < player->getPosition().Y)
		yf = player->getPosition().Y;

	camera->setTarget(boneNode->getAbsolutePosition());
	camera->setPosition(vector3df(xf, yf, zf));
	//-----------------------------------
}

bool CExample6::OnEvent(const SEvent &event) {

	return false;
}
