#ifndef __C_OCCLUSION_TEST_H_INCLUDED__
#define __C_OCCLUSION_TEST_H_INCLUDED__

#include "stdafx.h"

class COcclusionTest : public irr::IEventReceiver, public cp3d::core::IUpdate {

public:

	COcclusionTest(cp3d::core::IDevices *devices) {
		this->devices = devices;

		node = devices->getSceneManager()->getSceneNodeFromName("#light:new_light_flare_mesh");
		if (!node)
			exit(0);

		devices->getXEffect()->setUseMotionBlur(true);

		devices->getMonitorRegister()->getMonitor(0)->setRenderingFullTraitement(true);
		devices->getEventReceiver()->AddEventReceiver(this, 0, this);

		irr::scene::ICameraSceneNode *camera = devices->getSceneManager()->addCameraSceneNode();
		irr::scene::ISceneNodeAnimator *anim = devices->getSceneManager()->createFlyCircleAnimator(irr::core::vector3df(0.f, 25.f, 0.f),
			170.f, 0.0004f);
		camera->addAnimator(anim);
		anim->drop();
		devices->getSceneManager()->setActiveCamera(camera);
		camera->setTarget(irr::core::vector3df(0.f, 50.f, 0.f));
	}
	~COcclusionTest() { }

	void update() {
		irr::f32 test = devices->getVideoDriver()->getOcclusionQueryResult(node);
		devices->getGUIEnvironment()->getSkin()->getFont()->draw(irr::core::stringc(test).c_str(), irr::core::rect<s32>(0, 0, 50, 800),
																 irr::video::SColor(255, 0, 0, 0));
	}

	bool OnEvent(const SEvent &event) { return false; }

private:

	cp3d::core::IDevices *devices;
	irr::scene::ISceneNode *node;

};

#endif
