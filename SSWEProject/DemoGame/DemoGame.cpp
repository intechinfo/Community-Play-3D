// DemoGame.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"

#include <ISSWECoreUserInterface.h>

#include "DemoGame/Character/CCharacterControl.h"

int main(int argc, char *argv[])
{
	/// Create and configure our interface
	ISSWECoreUserInterface *coreInterface = createSSWEDevice(true, "");
	coreInterface->setLogEventWindowVisible(true);
	coreInterface->getIDevices()->getDevice()->getLogger()->setLogLevel(ELL_INFORMATION);

	IDevices *devices = coreInterface->getIDevices();
	devices->setXEffectDrawable(false);
	devices->getXEffect()->setAmbientColor(video::SColor(255, 32, 32, 32));

	/// Set up scene and rendering
	ISSWEImporter *importer = coreInterface->createImporter();
	importer->importScene("LTestDemoGame.world");
	delete importer;

	coreInterface->addMonitor("SSWEGenericMonitor");

	/// Set up our Camera
	irr::scene::ICameraSceneNode *camera = devices->getSceneManager()->addCameraSceneNode();

	devices->getSceneManager()->setActiveCamera(camera);

	/// Manage our gameplay with character
	CCharacterControl *characterControl = new CCharacterControl((irr::scene::IAnimatedMeshSceneNode*)devices->getSceneManager()->getSceneNodeFromName("#object:character"), 
																camera, devices);
	

	/// Render our game
	devices->getDevice()->getCursorControl()->setVisible(false);
	updateSSWEDevice(coreInterface);

	return 0;
}

