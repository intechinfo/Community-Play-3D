// ExampleGame.cpp
// Start coding your game here
//
#include "stdafx.h"

#include "COcclusionTest.h"
#include "CWeaponTest.h"
#include "CDragonTest.h"

int main(int argc, char *argv[]) {

	ISSWECoreUserInterface *coreUserInterface = createSSWEDevice(true, "");
	coreUserInterface->setLogEventWindowVisible(false);

	/// Get and configure device
	IDevices *devices = coreUserInterface->getIDevices();
	devices->getDevice()->getLogger()->setLogLevel(irr::ELL_NONE);

	/// Configure Monitor
	coreUserInterface->addMonitor("SSWEGenericMonitor");
	devices->getXEffect()->getHDRManager()->setExposure(1.2f);
	devices->getXEffect()->setUseNormalPass(true);

	/// Import our scene
	ISSWEImporter *importer = coreUserInterface->createImporter();
	importer->importScene(irr::core::stringc(argv[2]).c_str());
	delete importer;

	/// Configure FPS Camera
	devices->getSceneManager()->setActiveCamera(devices->getFPSCamera());
	devices->getFPSCamera()->setPosition(devices->getMayaCamera()->getPosition());
	devices->getCollisionManager()->getFPSCameraSettings()->setEllipsoidRadius(irr::core::vector3df(5.f, 20.f, 5.f));
	devices->getCollisionManager()->getFPSCameraSettings()->setSettings(devices->getSceneManager(),
		devices->getCollisionManager()->getMetaTriangleSelectors(),
		devices->getFPSCamera());
	devices->getDevice()->getCursorControl()->setVisible(false);

	//devices->getXEffect()->setAmbientColor(irr::video::SColor(255, 0, 0, 0));

	/// Load our game play class
	//CUpdate *updater = new CUpdate(devices);

	/// Load our occlusion test class
	//COcclusionTest *occlusioNTest = new COcclusionTest(devices);

	/// Load our dragon test class
	CDragonTest *dragonTest = new CDragonTest(devices);

	/// Update our device
	/// Let CP3D updating the device, it will optimize everything for us :-)
	updateSSWEDevice(coreUserInterface);

	/// Remove the scene if means it's for debogage
	if (!(argc >= 4 && irr::core::stringc(argv[4]) != "debug"))
		remove(argv[2]);

	return 0;
}
