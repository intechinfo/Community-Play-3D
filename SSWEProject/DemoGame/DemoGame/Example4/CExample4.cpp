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

class CUpdate : public irr::IEventReceiver, public cp3d::core::IUpdate {

public:

	CUpdate(cp3d::core::IDevices *devices) {
		this->devices = devices;

		lastTime = devices->getDevice()->getTimer()->getTime();

		isFireing = false;

		devices->getFPSCamera()->setNearValue(0.1f);
		node = (irr::scene::IAnimatedMeshSceneNode*)devices->getSceneManager()->getSceneNodeFromName("#object:mp5_animated");
		if (node) {
			devices->getCollisionManager()->getMetaTriangleSelectors()->removeTriangleSelector(node->getTriangleSelector());
			node->setTriangleSelector(0);
			devices->getFPSCamera()->addChild(node);
			node->setPosition(irr::core::vector3df(7.f, 0.f, 0.f));
			node->setRotation(irr::core::vector3df(0.f, -90.f, 0.f));

			devices->getAnimationController()->applyAnimationToModel(node, 0);
			node->setLoopMode(true);

			tipNode = node->getJointNode("tip");
			
			devices->getXEffect()->setUseMotionBlur(false);
			devices->getXEffect()->setUseDepthOfField(false);
		}

		devices->getEventReceiver()->AddEventReceiver(this, 0, this);

	}
	~CUpdate() { }

	void update() {
		/// Update our weapon position for more realism
		f32 elapsedTime = (f32)(devices->getDevice()->getTimer()->getTime() - lastTime);
		bool mustBeFixedX = (node->getPosition().X <= 7.06f && node->getPosition().X >= 6.94f);
		bool mustBeFixedY = (node->getPosition().Y <= 0.06f && node->getPosition().Y >= -0.06f);

		node->setPosition(irr::core::vector3df((mustBeFixedX) ? 7.f : (node->getPosition().X < 7.f) ? node->getPosition().X + elapsedTime / 1000.f
																									: node->getPosition().X -elapsedTime / 1000.f,
											   (mustBeFixedY) ? 0.f : (node->getPosition().Y < 0.f) ? node->getPosition().Y + elapsedTime / 1000.f
																								    : node->getPosition().Y - elapsedTime / 1000.f,
											   node->getPosition().Z));

		lastTime = devices->getDevice()->getTimer()->getTime();

		if (isFireing) {
			if (node->getFrameNr() > 400) {
				devices->getAnimationController()->applyAnimationToModel(node, 2);
				tipNode->updateAbsolutePosition();
				tipNode->updateAbsolutePositionOfAllChildren();
				irr::scene::IBillboardSceneNode *fireNode = devices->getSceneManager()->addBillboardSceneNode(tipNode, irr::core::dimension2df(20.f, 20.f));
				irr::video::ITexture *texture = devices->getVideoDriver()->getTexture("GUI/particle.bmp");
				fireNode->getMaterial(0).TextureLayer[0].Texture = texture;
				fireNode->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
				fireNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
				fireNode->addAnimator(devices->getSceneManager()->createDeleteAnimator(50));
			}
		}
	}

	bool OnEvent(const irr::SEvent &event) {

		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
			if (event.MouseInput.Event == irr::EMIE_MOUSE_MOVED) {
				/// Simulate weapon movement / position
				/// Make our weapon going between [6.f, 8.f] for the X position
				if (node->getPosition().X >= 6.f && node->getPosition().X <= 8.f && node->getPosition().Y >= -1.f && node->getPosition().Y <= 1.f) {
					f32 xOffset = (node->getPosition().X + (0.5f - devices->getDevice()->getCursorControl()->getRelativePosition().X));
					f32 yOffset = (node->getPosition().Y + (0.5f - devices->getDevice()->getCursorControl()->getRelativePosition().Y));

					if (xOffset > 6.f && xOffset < 8.f && yOffset > -1.f && yOffset < 1.f)
						node->setPosition(irr::core::vector3df(xOffset, yOffset, node->getPosition().Z));

				} else {
					if (node->getPosition().X < 6.f)
						node->setPosition(irr::core::vector3df(6.f, node->getPosition().Y, node->getPosition().Z));
					else
						node->setPosition(irr::core::vector3df(8.f, node->getPosition().Y, node->getPosition().Z));
					if (node->getPosition().Y < -1.f)
						node->setPosition(irr::core::vector3df(node->getPosition().X, -1.f, node->getPosition().Z));
					else
						node->setPosition(irr::core::vector3df(node->getPosition().X, 1.f, node->getPosition().Z));
				}
			}

			if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN) {
				isFireing = true;
			}

			if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP) {
				isFireing = false;
				node->setAnimationEndCallback(new COnFireFinished(this));
				node->setLoopMode(false);
			}
		}

		if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
			if (!event.KeyInput.PressedDown && event.KeyInput.Key == irr::KEY_KEY_R) {
				devices->getAnimationController()->applyAnimationToModel(node, 1);
				node->setAnimationEndCallback(new COnReloadFinished(this));
				node->setLoopMode(false);
			}
		}

		return false;
	}

private:

	/// Our IDevices interaface
	cp3d::core::IDevices *devices;

	/// To simulate our weapon movement / position
	irr::scene::IAnimatedMeshSceneNode *node;
	u32 lastTime;

	/// ON RELOAD FINISHED
	class COnReloadFinished : public irr::scene::IAnimationEndCallBack {
	public:
		COnReloadFinished(CUpdate *_updater) : updater(_updater) { }

		virtual void OnAnimationEnd(irr::scene::IAnimatedMeshSceneNode* _node) {
			updater->devices->getAnimationController()->applyAnimationToModel(_node, 0);
			_node->setLoopMode(true);
			_node->setAnimationEndCallback(0);
			delete this;
		}

	private:
		CUpdate *updater;
	};

	/// ON FIRE FINISHED
	class COnFireFinished : public irr::scene::IAnimationEndCallBack {
	public:
		COnFireFinished(CUpdate *_updater) : updater(_updater) { }

		virtual void OnAnimationEnd(irr::scene::IAnimatedMeshSceneNode* _node) {
			updater->devices->getAnimationController()->applyAnimationToModel(_node, 0);
			updater->isFireing = false;
			_node->setLoopMode(true);
			_node->setAnimationEndCallback(0);
			delete this;
		}

	private:
		CUpdate *updater;
	};

	bool isFireing;
	irr::scene::IBoneSceneNode *tipNode;

};

CExample4::CExample4(cp3d::ISSWECoreUserInterface *core) {
	/// Get the core user interface
	this->core = core;
	/// Get the IDevices interface router
	devices = core->getIDevices();

	updater = new CUpdate(devices);

	/// Add this class to the CP3D event listener
	/// Params : IEventReceiver pointer, IGUIWindow pointer if CP3D GUI Plugin, IUpdate pointer
	devices->getEventReceiver()->AddEventReceiver(this, 0, this);
}

CExample4::~CExample4() {

}

void CExample4::load() {
	devices->getFPSCamera()->setPosition(devices->getMayaCamera()->getPosition());
	devices->getSceneManager()->setActiveCamera(devices->getFPSCamera());
	devices->getCollisionManager()->getFPSCameraSettings()->setSettings(devices->getSceneManager(),
		devices->getCollisionManager()->getMetaTriangleSelectors(), devices->getFPSCamera());

	devices->setXEffectDrawable(true);
	devices->getXEffect()->setUseMotionBlur(true);
}

void CExample4::run() {
	/// Let CP3D update the devices (SSWE will be replaced by CP3D lol )
	/// It is optimized to use parallelism etc, then better performances =D
	updateSSWEDevice(core);
}

void CExample4::update() {

}

bool CExample4::OnEvent(const SEvent &event) {

	return false;
}
