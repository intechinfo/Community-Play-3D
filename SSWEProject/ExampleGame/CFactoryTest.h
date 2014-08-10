#ifndef __C_FACTORY_TEST_H_INCLUDED__
#define __C_FACTORY_TEST_H_INCLUDED__

#include "stdafx.h"

class CFactoryTest : public irr::IEventReceiver, public cp3d::core::IUpdate {

public:

	CFactoryTest(cp3d::core::IDevices *devices) {
		this->devices = devices;
		offset = 0;
		lastTime = 0;

		irr::scene::IAnimatedMeshSceneNode *terro = (irr::scene::IAnimatedMeshSceneNode*)devices->getSceneManager()->getSceneNodeFromName("#object:terrorist");
		devices->getAnimationController()->applyAnimationFromNameToModel(terro, "KILLED");
		terro->setLoopMode(true);

		node = (irr::scene::IAnimatedMeshSceneNode*)devices->getSceneManager()->getSceneNodeFromName("#object:chief");
		devices->getAnimationController()->applyAnimationFromNameToModel(node, "KILL");
		node->setLoopMode(true);

		head = node->getJointNode("RightEye");
		head->updateAbsolutePosition();

		irr::scene::ICameraSceneNode *camera = devices->getSceneManager()->addCameraSceneNode();

		irr::core::vector3df position = node->getPosition();
		position.Y += 20.f;
		irr::scene::ISceneNodeAnimator *anim = devices->getSceneManager()->createFlyCircleAnimator(position, 15.f, 0.0005f);
		camera->addAnimator(anim);
		anim->drop();
		camera->setFarValue(1000.f);

		devices->getDevice()->getTimer()->setSpeed(0.03f);
		devices->getEventReceiver()->AddEventReceiver(this, 0, this);
	}
	~CFactoryTest() { }

	void update() {
		head->updateAbsolutePosition();

		devices->getSceneManager()->getActiveCamera()->setTarget(head->getAbsolutePosition() + cos(lastTime / 1000.f) * 20.f);

		lastTime = devices->getDevice()->getTimer()->getTime();
	}

	bool OnEvent(const SEvent &event) { return false; }

private:

	cp3d::core::IDevices *devices;
	irr::scene::IAnimatedMeshSceneNode *node;
	irr::scene::IBoneSceneNode *head;
	
	irr::u32 lastTime;
	irr::f32 offset;

};

#endif
