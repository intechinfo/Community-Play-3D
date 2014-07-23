//
//  CUIOpenFileDialog.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//
#include "stdafx.h"
#include <cassert>

#include "CExample1.h"

CExample1::CExample1(cp3d::ISSWECoreUserInterface *core) {
	/// Get the core user interface
	this->core = core;
	/// Get the IDevices interface router
	devices = core->getIDevices();
}

CExample1::~CExample1() {

}

void CExample1::load(irr::core::stringc file) {
	/// Import the scene, and then delete the importer
	ISSWEImporter *importer = core->createImporter();
	importer->importScene(file.c_str());
	delete importer;

	/// Configure our FPS camera. Just get it and keep configuration from the .world file
	/// Configuration were done when importing the world
	irr::scene::ICameraSceneNode *fpsCamera = devices->getFPSCamera();
	devices->getSceneManager()->setActiveCamera(fpsCamera);
	fpsCamera->setPosition(irr::core::vector3df(102, 128, -80));
	/// Apply collisions to camera using the ICollisionManager class and IFPSCameraSettings struct
	devices->getCollisionManager()->getFPSCameraSettings()->setSettings(devices->getSceneManager(),
		devices->getCollisionManager()->getMetaTriangleSelectors(), fpsCamera);

	/// Get a scene node, and then remove collisions to it if we want :-)
	/// Don't pay attention to the shadows error today, the rendering pipeline is still in work
	irr::scene::ISceneNode *smokePlaneNode = devices->getSceneManager()->getSceneNodeFromName("#object:smoke_plan");
	if (smokePlaneNode) {
		devices->getCollisionManager()->getMetaTriangleSelectors()->removeTriangleSelector(smokePlaneNode->getTriangleSelector());
		smokePlaneNode->setTriangleSelector(0);
	}

	/// You can also get the data structure of the scene node with getISDataOfSceneNode
	cp3d::core::ISData *sceneNodeData = devices->getCoreData()->getISDataOfSceneNode(smokePlaneNode);
	/// You can now access to all methods of the scene node structure!

	/// Finish
	devices->getDevice()->getCursorControl()->setVisible(false);
}

void CExample1::run() {
	/// Let CP3D update the devices (SSWE will be replaced by CP3D lol )
	/// It is optimized to use parallelism etc, then better performances =D
	updateSSWEDevice(core);
}
