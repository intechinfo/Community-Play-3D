#ifndef __C_DRAGON_TEST_H_INCLUDED__
#define __C_DRAGON_TEST_H_INCLUDED__

#include "stdafx.h"

class CDragonTest : public irr::IEventReceiver, public IUpdate {

public:

	CDragonTest(IDevices *devices) {
		this->devices = devices;
		offset = 0;
		lastTime = 0;

		node = (irr::scene::IAnimatedMeshSceneNode*)devices->getSceneManager()->getSceneNodeFromName("#object:dragon");
		if (!node)
			exit(0);

		head = node->getJointNode("b_MF_Face");
		if (!head)
			exit(0);

		devices->getAnimationController()->applyAnimationFromNameToModel(node, "BOSS");

		//devices->getXEffect()->setUseHDRPass(false);
		devices->getEventReceiver()->AddEventReceiver(this, 0, this);
	}
	~CDragonTest() { }

	void update() {
		head->updateAbsolutePosition();
		devices->getSceneManager()->getActiveCamera()->setTarget(head->getAbsolutePosition() + cos(lastTime / 1000.f) * 20.f);

		lastTime = devices->getDevice()->getTimer()->getTime();
	}

	bool OnEvent(const SEvent &event) { return false; }

private:

	IDevices *devices;
	irr::scene::IAnimatedMeshSceneNode *node;
	irr::scene::IBoneSceneNode *head;
	
	irr::u32 lastTime;
	irr::f32 offset;

};

#endif
