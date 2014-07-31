//
//  CUIOpenFileDialog.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//
#include "stdafx.h"
#include <cassert>

#include "CExample5.h"

/// Use Irrlicht namespace
using namespace irr;
using namespace core;
using namespace scene;

CExample5::CExample5(cp3d::ISSWECoreUserInterface *core) {
	/// Get the core user interface
	this->core = core;
	/// Get the IDevices interface router
	devices = core->getIDevices();
}

CExample5::~CExample5() {

}

void CExample5::load() {
	/// Get the node factory
	cp3d::scene::IFactory *factory = devices->getNodeFactory();

	/// add a plane and configure it using the lambda function
	factory->createPlaneMeshSceneNode([=](ISceneNode *node) {
		node->getMaterial(0).setTexture(0, devices->getVideoDriver()->getTexture("data/Lights/kitchen_tile.jpg"));
	});

	/// Add some cubes in the air with a rotation animator
	bool inverse = true; /// If we inverse the rotation
	for (u32 i=0; i < 50; i++) {
		factory->createCubeSceneNode([=](ISceneNode *node) {
			f32 diff = 100.f;
			node->setPosition(vector3df(cos(f32(i)) * diff, 50.f-f32(i)+25.f, sin(f32(i)) * diff));
			node->setScale(vector3df(0.3f, 0.3f, 0.3f));
			node->getMaterial(0).setTexture(0, devices->getVideoDriver()->getTexture("data/Lights/tech_texture.jpg"));

			/// Create a rotation animator and add it to the current cube node
			ISceneNodeAnimator *anim = devices->getSceneManager()->createRotationAnimator(vector3df(0.3f, 0.3f, 0.3f) * ((inverse) ? -1.f : 1.f));
			node->addAnimator(anim);
			anim->drop();
		});
		inverse = !inverse; /// Inverse the rotation or not
	}

	/// Finally, add a light
	factory->createLightSceneNode([=](ISceneNode *node){
		node->setPosition(vector3df(-141.f, 200.f, -127.f));
		node->setRotation(vector3df(50.f, 46.f, 0.f));
		devices->getXEffect()->getShadowLight(0).setShadowMapResolution(4096);
		devices->getXEffect()->getShadowLight(0).setAutoRecalculate(true);
	});

	/// Configure the FPS Camera
	devices->getSceneManager()->setActiveCamera(devices->getFPSCamera());
	devices->getFPSCamera()->setPosition(vector3df(0.f, 100.f, 0.f));
	
	/// Get the first animator of the FPS camera and allow vertical movements
	core::list<ISceneNodeAnimator *>::ConstIterator it = devices->getFPSCamera()->getAnimators().begin();
	((ISceneNodeAnimatorCameraFPS*)*it)->setVerticalMovement(true);

	/// Add a filter for fun with lambda functions for the callback program
	desaturation = 0.4f;
	devices->getXEffect()->addPostProcessingEffectFromFile("shaders/HLSL/desaturate.hlsl",
		[=](cp3d::video::ISSWERender *render, irr::s32 materialType) {
			render->setPostProcessingEffectConstant(materialType, "Desaturation", &desaturation, 1);
		},
		[](cp3d::video::ISSWERender *render, irr::s32 materialType) {
			/// Nothing for post render :)
		}
	);

	/// Hide the mouse cursor
	devices->getDevice()->getCursorControl()->setVisible(false);
}

void CExample5::run() {
	/// Let CP3D update the devices (SSWE will be replaced by CP3D lol )
	/// It is optimized to use parallelism etc, then better performances =D
	updateSSWEDevice(core);
}
