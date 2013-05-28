// SSWELib.cpp : définit les routines d'initialisation pour la DLL.
//

#include "stdafx.h"
#include "SSWELib.h"

SSWE_LIB_API CCoreUserInterface* SSWELIBCALLCONV createSSWEDevice() {
	return new CCoreUserInterface();
}

SSWE_LIB_API void updateSSWEDevice(CCoreUserInterface *coreUserInterface) {
	//CCoreUserInterface *coreUserInterface = new CCoreUserInterface();

	IrrlichtDevice *device = coreUserInterface->getDevices()->getDevice();
    IVideoDriver *driver = coreUserInterface->getVideoDriver();
    ISceneManager *smgr = coreUserInterface->getSceneManager();
	IGUIEnvironment *gui = coreUserInterface->getGUIEnvironment();

	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, true);
	driver->setTextureCreationFlag(ETCF_OPTIMIZED_FOR_QUALITY, true);
	driver->setAllowZWriteOnTransparent(true);

	ISceneNode *skydome = smgr->addSkyDomeSceneNode(driver->getTexture("data/Lights/skydome_o.jpg"), 16, 8, 0.95f, 2.0f);
	skydome->setName("editor:skydome");
	coreUserInterface->getDevices()->setSkydome(skydome);

	/*IVolumeLightSceneNode *n = smgr->addVolumeLightSceneNode(0, -1, 32, 32, SColor(255, 64, 64, 64), SColor(255, 64, 64, 64));
	if (n) {
		n->setPosition(vector3df(-19, 0, 4));
		n->setScale(core::vector3df(246.0f, 246.0f, 246.0f));
		n->getMaterial(0).setTexture(0, smgr->getVideoDriver()->getTexture("data/Lights/lightFalloff.png"));
		n->setName("#vlight:test");
	}
	coreUserInterface->getDevices()->getCoreData()->getVolumeLightsData()->push_back(SVolumeLightsData(n));*/

	while (device->run()) {

        if (device->isWindowActive()) {
            driver->beginScene(true, true, SColor(0x0));

            coreUserInterface->update();

            driver->endScene();
        }
	}

    device->drop();
}
