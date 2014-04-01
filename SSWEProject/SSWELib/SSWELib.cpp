// SSWELib.cpp : définit les routines d'initialisation pour la DLL.
//

#include "stdafx.h"
#include "SSWELib.h"

#include <stdio.h>

#include "SSWELib/Device/Core/CCoreUserInterface.h"

#ifdef _IRR_OSX_PLATFORM_
#include <pthread.h>
static pthread_mutex_t cs_mutex =  PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
#endif

#ifndef _IRR_OSX_PLATFORM_
SSWE_LIB_API ISSWECoreUserInterface* SSWELIBCALLCONV createSSWEDevice(bool playOnly, irr::core::stringc argPath) {
#else
ISSWECoreUserInterface* createSSWEDevice(bool playOnly, irr::core::stringc argPath) {
#endif
	return (ISSWECoreUserInterface*)(new CCoreUserInterface(playOnly, argPath));
}

#ifndef _IRR_OSX_PLATFORM_
SSWE_LIB_API void SSWELIBCALLCONV updateSSWEDevice(ISSWECoreUserInterface *_coreUserInterface) {
#else
void updateSSWEDevice(ISSWECoreUserInterface *_coreUserInterface) {
#endif

	CCoreUserInterface *coreUserInterface = (CCoreUserInterface*)_coreUserInterface;

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
	coreUserInterface->getDevices()->getDOF()->add(skydome);
    skydome->setVisible(false);

	/*driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);
	ISceneNode* skyboxNode = smgr->addSkyBoxSceneNode(
		driver->getTexture("data/Lights/glacier_up.png"),
		driver->getTexture("data/Lights/glacier_dn.png"),
		driver->getTexture("data/Lights/glacier_lf.png"),
		driver->getTexture("data/Lights/glacier_rt.png"),
		driver->getTexture("data/Lights/glacier_ft.png"),
		driver->getTexture("data/Lights/glacier_bk.png"));
	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, true);
	skyboxNode->setName("editor:skydome");
	coreUserInterface->getDevices()->setSkyBox(skyboxNode);
	coreUserInterface->getDevices()->getDOF()->add(skyboxNode);
    skyboxNode->setVisible(true);
	coreUserInterface->getDevices()->getXEffect()->setSkyNode(skyboxNode);*/

	//ADD CLOUDS SYSTEM
	/*CCloudSceneNode *cloudLayer1 = new scene::CCloudSceneNode(smgr->getRootSceneNode(), coreUserInterface->getDevices()->getRenderingSceneManager());
	cloudLayer1->setTranslation(core::vector2d<f32>(0.008f, 0.0f));
	cloudLayer1->getMaterial(0).setTexture(0, driver->getTexture("shaders/Textures/Clouds/cloud01.png"));
	cloudLayer1->setCloudHeight(0.5f, 0.1f, -0.05f);

	CCloudSceneNode *cloudLayer2 = new scene::CCloudSceneNode(smgr->getRootSceneNode(), coreUserInterface->getDevices()->getRenderingSceneManager());
	cloudLayer2->setTranslation(core::vector2d<f32>(0.006f, 0.003f));
	cloudLayer2->getMaterial(0).setTexture(0, driver->getTexture("shaders/Textures/Clouds/cloud02.png"));
	cloudLayer2->setCloudHeight(0.4f, 0.05f, -0.1f);
	cloudLayer2->setTextureScale(0.5f);

	CCloudSceneNode *cloudLayer3 = new scene::CCloudSceneNode(smgr->getRootSceneNode(), coreUserInterface->getDevices()->getRenderingSceneManager());
	cloudLayer3->setTranslation(core::vector2d<f32>(0.006f, 0.003f));
	cloudLayer3->getMaterial(0).setTexture(0, driver->getTexture("shaders/Textures/Clouds/cloud03.png"));
	cloudLayer3->setCloudHeight(0.35f, 0.0f, -0.15f);
	cloudLayer3->setTextureScale(0.4f);*/
    
    //coreUserInterface->getDevices()->getXEffect()->addShadowToNode(cloudLayer1, coreUserInterface->getDevices()->getXEffectFilterType(), ESM_EXCLUDE);
    //coreUserInterface->getDevices()->getXEffect()->addShadowToNode(cloudLayer2, coreUserInterface->getDevices()->getXEffectFilterType(), ESM_EXCLUDE);
    //coreUserInterface->getDevices()->getXEffect()->addShadowToNode(cloudLayer3, coreUserInterface->getDevices()->getXEffectFilterType(), ESM_EXCLUDE);
    
    //coreUserInterface->getDevices()->getXEffect()->addNodeToDepthPass(cloudLayer1);
    //coreUserInterface->getDevices()->getXEffect()->addNodeToDepthPass(cloudLayer2);
    //coreUserInterface->getDevices()->getXEffect()->addNodeToDepthPass(cloudLayer3);
    
    //coreUserInterface->getDevices()->getXEffect()->addShadowToNode(skyboxNode, coreUserInterface->getDevices()->getXEffectFilterType(), ESM_EXCLUDE);
    //coreUserInterface->getDevices()->getXEffect()->addShadowToNode(skydome, coreUserInterface->getDevices()->getXEffectFilterType(), ESM_EXCLUDE);
    
	driver->beginScene(true, true, SColor(0x0));
	driver->endScene();
	if (!coreUserInterface->getDevices()->isOnlyForPlaying())
		coreUserInterface->getDevices()->getDevice()->maximizeWindow();

	//coreUserInterface->getDevices()->getRenderingSceneManager()->setActiveCamera(coreUserInterface->getDevices()->getMayaCamera());

    #ifndef _IRR_OSX_PLATFORM_
	if (!InitializeCriticalSectionAndSpinCount(&CriticalSection,  0x00000400))
		return;
    #endif

    #ifndef _IRR_OSX_PLATFORM_
	std::mutex mutex;
    #endif
    
	while (device->run()) {

		if (device->isWindowActive()) {
            
            #ifndef _IRR_OSX_PLATFORM_
				EnterCriticalSection(&CriticalSection);
			#endif
            
            #ifndef _IRR_OSX_PLATFORM_
            mutex.lock();
            #else
            pthread_mutex_lock( &cs_mutex );
            #endif

			coreUserInterface->getDevices()->updateEntities();

            driver->beginScene(true, true, SColor(0x0));

            coreUserInterface->update();

            driver->endScene();

            #ifndef _IRR_OSX_PLATFORM_
            mutex.unlock();
            #else
            pthread_mutex_unlock( &cs_mutex );
            #endif
            
            
			#ifndef _IRR_OSX_PLATFORM_
				LeaveCriticalSection(&CriticalSection);
			#endif
            
        }
	}

    #ifndef _IRR_OSX_PLATFORM_
	DeleteCriticalSection(&CriticalSection);
    #endif

    device->drop();


}
