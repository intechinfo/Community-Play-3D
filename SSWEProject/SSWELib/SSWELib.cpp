// SSWELib.cpp : définit les routines d'initialisation pour la DLL.
//

#include "stdafx.h"
#include "SSWELib.h"

void test(CCoreUserInterface *coreUserInterface) {

}

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

	//std::thread test_t(&test, coreUserInterface);
	//test_t.detach();

	driver->beginScene(true, true, SColor(0x0));
	driver->endScene();
	coreUserInterface->getDevices()->getDevice()->maximizeWindow();

	if (!InitializeCriticalSectionAndSpinCount(&CriticalSection,  0x00000400))
		return;

	while (device->run()) {

        if (device->isWindowActive()) {
            driver->beginScene(true, true, SColor(0x0));

            coreUserInterface->update();

            driver->endScene();
        }
	}

	DeleteCriticalSection(&CriticalSection);

    device->drop();
}
