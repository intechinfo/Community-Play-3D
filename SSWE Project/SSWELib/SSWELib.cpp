// SSWELib.cpp�: d�finit les routines d'initialisation pour la DLL.
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

	ISceneNode *skydome = smgr->addSkyDomeSceneNode(driver->getTexture("data/Lights/skydome.jpg"), 16, 8, 0.95f, 2.0f);
	skydome->setName("editor:skydome");
	coreUserInterface->getDevices()->setSkydome(skydome);

	IVolumeLightSceneNode * n = smgr->addVolumeLightSceneNode(0, -1, 32, 32, SColor(0, 180, 180, 180), SColor(0, 0, 0, 0));
	if (n) {
		n->setName("#vlight:volume_light_test");
		n->setScale(core::vector3df(46.0f, 45.0f, 46.0f));
		n->getMaterial(0).setTexture(0, smgr->getVideoDriver()->getTexture("data/Lights/lightFalloff.png"));

		SVolumeLightsData vldata(n);
		coreUserInterface->getDevices()->getCoreData()->getVolumeLightsData()->push_back(vldata);
	}

	while (device->run()) {

        if (device->isWindowActive()) {
            driver->beginScene(true, true, SColor(0x0));

            coreUserInterface->update();

            driver->endScene();
        }
	}

    device->drop();
}
