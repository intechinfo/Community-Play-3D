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

	ISceneNode *skydome = smgr->addSkyDomeSceneNode(driver->getTexture("data/Lights/skydome.jpg"), 16, 8, 0.95f, 2.0f);
	skydome->setName("editor:skydome");
	coreUserInterface->getDevices()->setSkydome(skydome);

	IVolumeLightSceneNode *vol = smgr->addVolumeLightSceneNode(0, -1, 10, 100, video::SColor(0, 255, 255, 255), video::SColor(0, 255, 255, 255), vector3df(0, 0, 0), vector3df(0, 0, 0), vector3df(50, 50, 50));
	vol->setMaterialTexture(0, driver->getTexture("wall for volume light testing/portal7.bmp"));
	coreUserInterface->getDevices()->getXEffect()->addShadowToNode(vol, EFT_16PCF, ESM_EXCLUDE);
	CUIWindowEditNode *en = new CUIWindowEditNode(coreUserInterface->getDevices());
	en->open(vol, "#object");

	while (device->run()) {

        if (device->isWindowActive()) {
            driver->beginScene(true, true, SColor(0x0));

            coreUserInterface->update();

            driver->endScene();
        }
	}

    device->drop();
}
