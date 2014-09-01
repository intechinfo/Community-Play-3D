// ExampleGame.cpp
// Start coding your game here
//
#include "stdafx.h"
#include "CGUIInformations.h"

#include "COcclusionTest.h"
#include "CWeaponTest.h"
#include "CDragonTest.h"
#include "CFactoryTest.h"

using namespace cp3d;
using namespace cp3d::core;

int main(int argc, char *argv[]) {

	ISSWECoreUserInterface *coreUserInterface = createSSWEDevice(true, "");
	coreUserInterface->setLogEventWindowVisible(false);

	/// Get and configure device
	IDevices *devices = coreUserInterface->getIDevices();
	devices->getDevice()->getLogger()->setLogLevel(irr::ELL_NONE);

	/// Configure Monitor
    #if defined(_IRR_OSX_PLATFORM_) || defined(_IRR_LINUX_PLATFORM_)
    coreUserInterface->addMonitor("libSSWEGenericMonitor");
    #else
	coreUserInterface->addMonitor("SSWEGenericMonitor");
    #endif
	devices->getXEffect()->getHDRManager()->setExposure(1.f);
    devices->getXEffect()->setUseNormalPass(false);

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

    /// IMPORTANT:
    /// Creates an instance of the CGUIInformations class.
    /// It allows you to view informations about rendering (FPS, triangles, etc.)
    CGUIInformations *guiInformations = new CGUIInformations(devices);

	//devices->getXEffect()->setAmbientColor(irr::video::SColor(255, 0, 0, 0));

	/// Load our game play class
	//CUpdate *updater = new CUpdate(devices);

	/// Load our occlusion test class
	//COcclusionTest *occlusioNTest = new COcclusionTest(devices);

	/// Load our dragon test class
	//CDragonTest *dragonTest = new CDragonTest(devices);

	/// Load our factory test
	//CFactoryTest *factoryTest = new CFactoryTest(devices);

	/// Update our device
	/// Let CP3D updating the device, it will optimize everything for us :-)
	updateSSWEDevice(coreUserInterface);

	/// Remove the scene if means it's for debogage
	if (!(argc >= 4 && irr::core::stringc(argv[4]) != "debug"))
		remove(argv[2]);

	return 0;
}
