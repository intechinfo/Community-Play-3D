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
    coreUserInterface->getDevices()->getDevice()->getLogger()->setLogLevel(ELL_NONE);
    
    //---------------------------------------------------------------------------------------------
    //---------------------------------------TESTS-------------------------------------------------
    //---------------------------------------------------------------------------------------------
    
    IVideoDriver *driver = coreUserInterface->getVideoDriver();
    ISceneManager *smgr = coreUserInterface->getSceneManager();
    
    //---------------------------------------------------------------------------------------------
    //-----------------------------------RUNNING DEVICE--------------------------------------------
    //---------------------------------------------------------------------------------------------
    
    while (coreUserInterface->getDevices()->getDevice()->run()) {
        
        if (coreUserInterface->getDevices()->getDevice()->isWindowActive()) {
            coreUserInterface->getVideoDriver()->beginScene(true, true, SColor(0x0));
            
            coreUserInterface->update();
            
            coreUserInterface->getVideoDriver()->endScene();
        }
        
    }
    
    coreUserInterface->getDevices()->getDevice()->drop();
    
    return EXIT_SUCCESS;
}
