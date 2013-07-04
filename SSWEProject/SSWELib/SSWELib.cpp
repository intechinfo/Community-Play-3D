// SSWELib.cpp : définit les routines d'initialisation pour la DLL.
//

#include "stdafx.h"
#include "SSWELib.h"

SSWE_LIB_API CCoreUserInterface* SSWELIBCALLCONV createSSWEDevice() {
	return new CCoreUserInterface();
}

SSWE_LIB_API void updateSSWEDevice(CCoreUserInterface *coreUserInterface) {

	IrrlichtDevice *device = coreUserInterface->getDevices()->getDevice();
    IVideoDriver *driver = coreUserInterface->getVideoDriver();
    ISceneManager *smgr = coreUserInterface->getSceneManager();
	IGUIEnvironment *gui = coreUserInterface->getGUIEnvironment();

	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, true);
	driver->setTextureCreationFlag(ETCF_OPTIMIZED_FOR_QUALITY, true);
	driver->setAllowZWriteOnTransparent(true);

	ISceneNode *skydome = smgr->addSkyDomeSceneNode(driver->getTexture("data/Lights/skydome.jpg"), 32, 32, 0.95f, 2.0f);
	skydome->setName("editor:skydome");
	coreUserInterface->getDevices()->setSkydome(skydome);

	driver->beginScene(true, true, SColor(0x0));
	driver->endScene();
	coreUserInterface->getDevices()->getDevice()->maximizeWindow();

	coreUserInterface->getDevices()->getRenderingSceneManager()->setActiveCamera(coreUserInterface->getDevices()->getMayaCamera());

	if (!InitializeCriticalSectionAndSpinCount(&CriticalSection,  0x00000400))
		return;

	std::mutex mutex;
	while (device->run()) {

        if (device->isWindowActive()) {
			#ifndef _IRR_OSX_PLATFORM_
				mutex.lock();
				EnterCriticalSection(&CriticalSection);
			#endif

			coreUserInterface->getDevices()->updateEntities();

            driver->beginScene(true, true, SColor(0x0));

            coreUserInterface->update();

            driver->endScene();

			#ifndef _IRR_OSX_PLATFORM_
				mutex.unlock();
				LeaveCriticalSection(&CriticalSection);
			#endif
        }
	}

	DeleteCriticalSection(&CriticalSection);

    device->drop();


}
