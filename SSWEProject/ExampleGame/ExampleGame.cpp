// ExampleGame.cpp
// Start coding your game here
//

#include "stdafx.h"

class CUpdate : public irr::IEventReceiver, public IUpdate {

public:

	CUpdate(IDevices *devices) {
		this->devices = devices;

		lastTime = devices->getDevice()->getTimer()->getTime();

		devices->getFPSCamera()->setNearValue(0.1f);
		node = (irr::scene::IAnimatedMeshSceneNode*)devices->getSceneManager()->getSceneNodeFromName("#object:mp5_animated");
		if (node) {
			devices->getCollisionManager()->getMetaTriangleSelectors()->removeTriangleSelector(node->getTriangleSelector());
			devices->getFPSCamera()->addChild(node);
			node->setPosition(irr::core::vector3df(7.f, 0.f, 0.f));
			node->setRotation(irr::core::vector3df(0.f, -90.f, 0.f));

			devices->applyAnimationToModel(node, 0);
			
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
		}

		return false;
	}

private:

	/// Our IDevices interaface
	IDevices *devices;

	/// To simulate our weapon movement / position
	irr::scene::IAnimatedMeshSceneNode *node;
	u32 lastTime;

};

int main(int argc, char *argv[]) {

	ISSWECoreUserInterface *coreUserInterface = createSSWEDevice(true, "");
	coreUserInterface->setLogEventWindowVisible(false);

	/// Get and configure device
	IDevices *devices = coreUserInterface->getIDevices();
	devices->getDevice()->getLogger()->setLogLevel(irr::ELL_NONE);

	/// Configure Monitor
	coreUserInterface->addMonitor("SSWEGenericMonitor");

	/// Import our scene
	ISSWEImporter *importer = coreUserInterface->createImporter();
	importer->importScene(irr::core::stringc(argv[2]).c_str());
	delete importer;

	/// Configure FPS Camera
	devices->getSceneManager()->setActiveCamera(devices->getFPSCamera());
	devices->getFPSCamera()->setPosition(devices->getMayaCamera()->getPosition());
	devices->getCollisionManager()->getFPSCameraSettings()->setSettings(devices->getSceneManager(),
		devices->getCollisionManager()->getMetaTriangleSelectors(),
		devices->getFPSCamera());
	devices->getDevice()->getCursorControl()->setVisible(false);

	//devices->getXEffect()->setAmbientColor(irr::video::SColor(255, 0, 0, 0));

	/// Load our game play class
	//CUpdate *updater = new CUpdate(devices);

	/// Update our device
	/// Let CP3D updating the device, it will optimize everything for us :-)
	updateSSWEDevice(coreUserInterface);

	/// Remove the scene if means it's for debogage
	if (!(argc >= 4 && irr::core::stringc(argv[4]) != "debug"))
		remove(argv[2]);

	return 0;
}
