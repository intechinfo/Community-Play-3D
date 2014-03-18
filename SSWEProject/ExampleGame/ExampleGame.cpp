// ExampleGame.cpp
// Start coding your game here
//

#include "stdafx.h"

class CUpdate : public irr::IEventReceiver, public IUpdate {

public:

	CUpdate(IDevices *devices) {
		devices->getFPSCamera()->setNearValue(0.1f);
		irr::scene::IAnimatedMeshSceneNode *node = (irr::scene::IAnimatedMeshSceneNode*)devices->getSceneManager()->getSceneNodeFromName("#object:mp5_animated");
		if (node) {
			devices->getCollisionManager()->getMetaTriangleSelectors()->removeTriangleSelector(node->getTriangleSelector());
			devices->getFPSCamera()->addChild(node);
			node->setPosition(irr::core::vector3df(7.f, 0.f, 0.f));
			node->setRotation(irr::core::vector3df(0.f, -90.f, 0.f));

			devices->applyAnimationToModel(node, 0);

			devices->getXEffect()->setUseMotionBlur(true);
			devices->getXEffect()->setUseDepthOfField(true);
		}

	}
	~CUpdate() { }

	void update() {

	}

	bool OnEvent(const SEvent &event) {

		return false;
	}

private:

	IDevices *devices;

};

int main(int argc, char *argv[]) {

	ISSWECoreUserInterface *coreUserInterface = createSSWEDevice(true, "");
	coreUserInterface->setLogEventWindowVisible(false);

	/// Get and configure device
	IDevices *devices = coreUserInterface->getIDevices();
	devices->getDevice()->getLogger()->setLogLevel(irr::ELL_NONE);

	/// Configure Monitor
	coreUserInterface->addMonitor("SSWEGENERICMONITOR");

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

	/// Load our game play class
	CUpdate *updater = new CUpdate(devices);

	/// Update our device
	updateSSWEDevice(coreUserInterface);

	remove(argv[2]);

	return 0;
}
