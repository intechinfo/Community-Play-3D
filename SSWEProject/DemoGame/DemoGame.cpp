// DemoGame.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"

#include <ISSWECoreUserInterface.h>

//#include "DemoGame/Character/CCharacterControl.h"
#include "DemoGame/Example1/CExample1.h"
#include "DemoGame/Example2/CExample2.h"
#include "DemoGame/Example3/CExample3.h"
#include "DemoGame/Example4/CExample4.h"
#include "DemoGame/Example5/CExample5.h"
#include "DemoGame/Example6/CExample6.h"

int exampleNumber = 4;

int main(int argc, char *argv[])
{

	std::cout << "Choose your example :" << std::endl;
	std::cout << "1 - Simple Example" << std::endl;
	std::cout << "2 - Example with animated models + callbacks" << std::endl;
	std::cout << "3 - FPS Gameplay example with animated weapon (classic animation callback)" << std::endl;
	std::cout << "4 - FPS Gameplay example with animated weapon (advanced animation callback)" << std::endl;
	std::cout << "5 - Playing with the node factory" << std::endl;
	std::cout << "6 - Playing with Martin" << std::endl;
	std::cin >> exampleNumber;

	/// Create and configure our interface
	cp3d::ISSWECoreUserInterface *coreInterface = createSSWEDevice(true, "");
	coreInterface->setLogEventWindowVisible(false);
	coreInterface->getIDevices()->getDevice()->getLogger()->setLogLevel(ELL_INFORMATION);

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
	} else
	if (exampleNumber == 3) {
		/// Lear combination with IEventReceiver and IUpdate + animations with a weapon
		example->load("Example3.world");
		CExample3 *example3 = new CExample3(coreInterface);
		example3->load();
		example3->run();
	} else
	if (exampleNumber == 4) {
		/// Learn how to use animations with lambda functions
		example->load("Example4.world");
		CExample4 *example4 = new CExample4(coreInterface);
		example4->load();
		example4->run();
	} else
	if (exampleNumber == 5) {
		/// Learn how to use node factory
		CExample5 *example5 = new CExample5(coreInterface);
		example5->load();
		example5->run();
	} else
	if (exampleNumber == 6) {
		/// Learn something
		CExample6 *example6 = new CExample6(coreInterface);
		example6->load();
		example6->run();
	}

	//devices->getCoreData()->clear();
	//devices->getCoreData()->clearAllTheArrays();

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

