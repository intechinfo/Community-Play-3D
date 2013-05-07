//
//  main.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 25/09/12.
//
//

//ENUMS 17

#include "Device/Core/CCoreUserInterface.h"

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

#ifdef IS_ERIO_AND_RELOU
#include <Windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow){
#else
int main() {
#endif
    //---------------------------------------------------------------------------------------------
    //----------------------------------GUI DEVICE CREATION----------------------------------------
    //---------------------------------------------------------------------------------------------

    CCoreUserInterface *coreUserInterface = new CCoreUserInterface();

    //---------------------------------------------------------------------------------------------
    //---------------------------------------TESTS-------------------------------------------------
    //---------------------------------------------------------------------------------------------

	IrrlichtDevice *device = coreUserInterface->getDevices()->getDevice();
    IVideoDriver *driver = coreUserInterface->getVideoDriver();
    ISceneManager *smgr = coreUserInterface->getSceneManager();
	IGUIEnvironment *gui = coreUserInterface->getGUIEnvironment();

	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, true);
	driver->setTextureCreationFlag(ETCF_OPTIMIZED_FOR_QUALITY, true);

	//CWaterSurface *water = new CWaterSurface(smgr, vector3df(0, 0, 0));
	//water->getWaterNode()->setMaterialType((E_MATERIAL_TYPE)coreUserInterface->getDevices()->getCoreData()->getShaderCallbacks()->operator[](0)->getMaterial());

	ISceneNode *skydome = smgr->addSkyDomeSceneNode(driver->getTexture("data/Lights/skydome_o.jpg"), 16, 8, 0.95f, 2.0f);
	coreUserInterface->getDevices()->setSkydome(skydome);

	//coreUserInterface->getDevices()->getXEffect()->addShadowToNode(skydome, coreUserInterface->getDevices()->getXEffectFilterType(), ESM_NO_SHADOW);

	/*ICameraSceneNode *camera = smgr->addCameraSceneNode();
	camera->setInputReceiverEnabled(false);
	camera->setParent(coreUserInterface->getDevices()->getMayaCamera());
	camera->setPosition(vector3df(10, 0, 0));
	smgr->setActiveCamera(coreUserInterface->getDevices()->getMayaCamera());

	while (device->run()) {

		driver->setViewPort(rect<s32>(0, 0, driver->getScreenSize().Width, driver->getScreenSize().Height));

		driver->beginScene(true, true, SColor(0x0));

		smgr->setActiveCamera(coreUserInterface->getDevices()->getMayaCamera());
		driver->setViewPort(rect<s32>(0,0,driver->getScreenSize().Width/2,driver->getScreenSize().Height));
		smgr->drawAll();
		camera->setTarget(coreUserInterface->getDevices()->getMayaCamera()->getTarget());

		smgr->setActiveCamera(camera);
		driver->setViewPort(rect<s32>(driver->getScreenSize().Width/2,0,driver->getScreenSize().Width,driver->getScreenSize().Height));
		smgr->drawAll();

		smgr->setActiveCamera(coreUserInterface->getDevices()->getMayaCamera());

		driver->endScene();

	}*/

	//FOR ERIO

	/*CWaterSurface *water = new CWaterSurface(smgr, vector3df(0, 0, 0));
	water->getWaterNode()->setName("#water:test");
	water->getWaterNode()->setMaterialType((E_MATERIAL_TYPE)coreUserInterface->getDevices()->getCoreData()->getShaderCallbacks()->operator[](0)->getMaterial());
	water->setSinWaveEnabled(true);
	water->setRefractionEnabled(true);
	coreUserInterface->getDevices()->getCoreData()->getWaterSurfaces()->push_back(water->getWaterNode());*/

    //---------------------------------------------------------------------------------------------
    //-----------------------------------RUNNING DEVICE--------------------------------------------
    //---------------------------------------------------------------------------------------------

	while (device->run()) {

        if (device->isWindowActive()) {
            driver->beginScene(true, true, SColor(0x0));

            coreUserInterface->update();

			/*if (coreUserInterface->getDevices()->isXEffectDrawable()) {
				water->setOriginRTT(coreUserInterface->getDevices()->getXEffect()->getScreenQuad().rt[1]);
			} else {
				water->setOriginRTT(0);
			}*/

            driver->endScene();
        }

    }

    device->drop();

    return EXIT_SUCCESS;
}
