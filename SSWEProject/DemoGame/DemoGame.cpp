// DemoGame.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"

#include <ISSWECoreUserInterface.h>

//#include "DemoGame/Character/CCharacterControl.h"
#include "DemoGame/Example1/CExample1.h"
#include "DemoGame/Example2/CExample2.h"

int exampleNumber = 1;

int main(int argc, char *argv[])
{
	/// Create and configure our interface
	ISSWECoreUserInterface *coreInterface = createSSWEDevice(true, "");
	coreInterface->setLogEventWindowVisible(false);
	coreInterface->getIDevices()->getDevice()->getLogger()->setLogLevel(ELL_INFORMATION);

	/// Get the main interface, working as a router;
	IDevices *devices = coreInterface->getIDevices();

	/// Add our default monitor the examples
	/// Just give the name of the .dll monitor located in ~Plugins/Monitors/
	/// without the extension .dll, .so or .dylib for multiplateform support
	coreInterface->addMonitor("SSWEGenericMonitor");

	CExample1 *example = new CExample1(coreInterface);
	if (exampleNumber == 1) {
		/// Learn how to load a saved scene and configure default FPS camera
		example->load("Example1.world");
		example->run();
	} else
	if (exampleNumber == 2) {
		/// Learn how to use animated models + how to handle events!
		example->load("Example2.world");
		CExample2 *example2 = new CExample2(coreInterface);
		example2->load();
		example2->run();
	}

	/// Old example lines, CCharacterControl
	/*
	/// Set up scene and rendering
	ISSWEImporter *importer = coreInterface->createImporter();
	importer->importScene("lmetro1012.world");
	delete importer;

	/// Set up our Camera
	irr::scene::ICameraSceneNode *camera = devices->getSceneManager()->addCameraSceneNode();

	devices->getSceneManager()->setActiveCamera(camera);

	/// Manage our gameplay with character
	CCharacterControl *characterControl = new CCharacterControl((irr::scene::IAnimatedMeshSceneNode*)devices->getSceneManager()->getSceneNodeFromName("#object:character"), 
																camera, devices);
	
	

	/// Render our game
	devices->getDevice()->getCursorControl()->setVisible(false);
	updateSSWEDevice(coreInterface);

	*/

	return 0;
}

