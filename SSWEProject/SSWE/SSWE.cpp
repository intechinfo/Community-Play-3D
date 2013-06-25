////
////  main.cpp
////  MacOSX
////
////  Created by Julien Moreau-Mathis on 25/09/12.
////
////
//
////ENUMS 17
//

#include "stdafx.h"
#include <CGenericMonitor.h>

#ifdef SSWE_RELEASE
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#ifndef _IRR_OSX_PLATFORM_
#include <Windows.h>
#endif

struct SGenericMonitor {
	HINSTANCE hdll;
	IMonitor *genericMonitor;

	void freeGenericMonitor() {
		FreeLibrary(hdll);
	}
};

SGenericMonitor getGenericMonitor() {
	SGenericMonitor gm;

	//WITH DYNAMIC LOAD
	//DECLARE OUR DLL INSTANCE
	HINSTANCE hdll = NULL;
	//DECLARE OUR MONITOR
	IMonitor* genericMonitor = NULL;
	//TYPE DEFINITION FOR THE RETURNED POINTER OF GENERIC MONITOR IN DLL
	typedef void* (*pvFunctv)();
	pvFunctv CreateFoo;

	//GET ADRESS OF THE FUNCTION
	//createMonitor IS IN THE FILE DLLExport.h WHERE IT RENTURNS A POINTER TO OUR MONITOR
	//BECAUSE GENERIC MONITOR IS THE DEFAULT MONITOR, CHECK IF WE ARE RELEASE OR DEBUG
	#ifdef SSWE_RELEASE
		hdll = LoadLibrary(L"SSWEGenericMonitor.dll"); //LOAD THE LIBRARY
	#else
		hdll = LoadLibrary(L"SSWEGenericMonitor_d.dll"); //LOAD THE LIBRARY
	#endif
	CreateFoo = reinterpret_cast < pvFunctv > (GetProcAddress( hdll, "createMonitor" ));

	//CHECK IF THE PROCESS createMonitor EXISTS
	if (CreateFoo == NULL) {
		std::cout << "DLL was not found..." << std::endl;
		int a;
		std::cin >> a;
		exit(EXIT_FAILURE);
	}

	//CAST THE PREVIOSU RETURNED MONITOR TO A IMONITOR POINTER
	genericMonitor = static_cast < IMonitor* > (CreateFoo());

	gm.genericMonitor = genericMonitor;
	gm.hdll = hdll;

	return gm;
}

#ifdef IS_ERIO_AND_RELOU
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow) {
#else
int main(int argc, char* argv[]) {
#endif

	//WITHOUT DYNAMIC LOAD
	/*CCoreUserInterface *coreUserInterface_ = createSSWEDevice();

	CDevices *devices_ = coreUserInterface_->getDevices();
	CGenericMonitor *generic_Monitor = new CGenericMonitor();
	generic_Monitor->setGUIEnvironment(devices_->getGUIEnvironment());
	generic_Monitor->setName("Generic Monitor");
	generic_Monitor->setToolsSceneManager(devices_->getSecondSceneManager());
	generic_Monitor->setXEffect(devices_->getXEffect());
	devices_->getMonitorRegister()->registerMonitor(generic_Monitor);

	updateSSWEDevice(coreUserInterface_);*/

	SGenericMonitor gm = getGenericMonitor();
	IMonitor *genericMonitor = gm.genericMonitor;

	CCoreUserInterface *coreUserInterface = createSSWEDevice();

	CDevices *devices = coreUserInterface->getDevices();
	genericMonitor->init();
	genericMonitor->setGUIEnvironment(devices->getGUIEnvironment());
	genericMonitor->setName("Generic Monitor");
	genericMonitor->setToolsSceneManager(devices->getSecondSceneManager());
	genericMonitor->setXEffect(devices->getXEffect());
	devices->getMonitorRegister()->registerMonitor(genericMonitor);

	updateSSWEDevice(coreUserInterface);

	gm.freeGenericMonitor();

	return EXIT_SUCCESS;
}

//#include "../../sources/Device/Core/CCoreUserInterface.h"
//
//#ifdef _MSC_VER
//#pragma comment(lib, "Irrlicht.lib")
//#endif
//
//#ifdef IS_ERIO_AND_RELOU
//#include <Windows.h>
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow){
//#else
//int main() {
//#endif
//    //---------------------------------------------------------------------------------------------
//    //----------------------------------GUI DEVICE CREATION----------------------------------------
//    //---------------------------------------------------------------------------------------------
//
//    CCoreUserInterface *coreUserInterface = new CCoreUserInterface();
//
//    //---------------------------------------------------------------------------------------------
//    //---------------------------------------TESTS-------------------------------------------------
//    //---------------------------------------------------------------------------------------------
//
//	IrrlichtDevice *device = coreUserInterface->getDevices()->getDevice();
//    IVideoDriver *driver = coreUserInterface->getVideoDriver();
//    ISceneManager *smgr = coreUserInterface->getSceneManager();
//	IGUIEnvironment *gui = coreUserInterface->getGUIEnvironment();
//
//	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, true);
//	driver->setTextureCreationFlag(ETCF_OPTIMIZED_FOR_QUALITY, true);
//
//	driver->setAllowZWriteOnTransparent(true);
//
//	//CWaterSurface *water = new CWaterSurface(smgr, vector3df(0, 0, 0));
//	//water->getWaterNode()->setMaterialType((E_MATERIAL_TYPE)coreUserInterface->getDevices()->getCoreData()->getShaderCallbacks()->operator[](0)->getMaterial());
//
//	ISceneNode *skydome = smgr->addSkyDomeSceneNode(driver->getTexture("data/Lights/skydome.jpg"), 16, 8, 0.95f, 2.0f);
//	skydome->setName("editor:skydome");
//	coreUserInterface->getDevices()->setSkydome(skydome);
//
//	//coreUserInterface->getDevices()->getXEffect()->addShadowToNode(skydome, coreUserInterface->getDevices()->getXEffectFilterType(), ESM_NO_SHADOW);
//
//	/*ICameraSceneNode *camera = smgr->addCameraSceneNode();
//	camera->setInputReceiverEnabled(false);
//	camera->setParent(coreUserInterface->getDevices()->getMayaCamera());
//	camera->setPosition(vector3df(10, 0, 0));
//	smgr->setActiveCamera(coreUserInterface->getDevices()->getMayaCamera());
//
//	while (device->run()) {
//
//		driver->setViewPort(rect<s32>(0, 0, driver->getScreenSize().Width, driver->getScreenSize().Height));
//
//		driver->beginScene(true, true, SColor(0x0));
//
//		smgr->setActiveCamera(coreUserInterface->getDevices()->getMayaCamera());
//		driver->setViewPort(rect<s32>(0,0,driver->getScreenSize().Width/2,driver->getScreenSize().Height));
//		smgr->drawAll();
//		camera->setTarget(coreUserInterface->getDevices()->getMayaCamera()->getTarget());
//
//		smgr->setActiveCamera(camera);
//		driver->setViewPort(rect<s32>(driver->getScreenSize().Width/2,0,driver->getScreenSize().Width,driver->getScreenSize().Height));
//		smgr->drawAll();
//
//		smgr->setActiveCamera(coreUserInterface->getDevices()->getMayaCamera());
//
//		driver->endScene();
//
//	}*/
//
//	//FOR ERIOR
//	/*CWaterSurface *wat = new CWaterSurface(smgr, coreUserInterface->getDevices()->getXEffect()->getScreenQuad().rt[1], 0);
//	wat->getWaterNode()->setName("#water:test");
//	SWaterSurfacesData wdata(wat);
//	coreUserInterface->getDevices()->getCoreData()->getWaterSurfaces()->push_back(wdata);*/
//
//    //---------------------------------------------------------------------------------------------
//    //-----------------------------------RUNNING DEVICE--------------------------------------------
//    //---------------------------------------------------------------------------------------------
//
//	while (device->run()) {
//
//        if (device->isWindowActive()) {
//            driver->beginScene(true, true, SColor(0x0));
//
//            coreUserInterface->update();
//
//            driver->endScene();
//        }
//
//    }
//
//    device->drop();
//
//    return EXIT_SUCCESS;
//}
//
