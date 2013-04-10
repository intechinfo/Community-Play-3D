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

int main() {

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

	//ISceneNode *skydome = smgr->addSkyDomeSceneNode(driver->getTexture("data/Lights/skydome.jpg"), 16, 8, 0.95f, 2.0f);
	//coreUserInterface->getDevices()->setSkydome(skydome);
	//coreUserInterface->getDevices()->getXEffect()->addShadowToNode(skydome, coreUserInterface->getDevices()->getXEffectFilterType(), ESM_NO_SHADOW);

    //---------------------------------------------------------------------------------------------
    //-----------------------------------RUNNING DEVICE--------------------------------------------
    //---------------------------------------------------------------------------------------------

	while (device->run()) {
        
        if (device->isWindowActive()) {
            driver->beginScene(true, true, SColor(0x0));

            coreUserInterface->update();
            
            driver->endScene();
        }
        
    }
    
    device->drop();

    return EXIT_SUCCESS;
}
