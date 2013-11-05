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

#ifdef SSWE_RELEASE
    #ifndef _IRR_OSX_PLATFORM_
        #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
    #endif
#endif

#ifndef _IRR_OSX_PLATFORM_
#include <Windows.h>
#endif

#include "../SSWELib/SSWELib.h"

#ifdef IS_ERIO_AND_RELOU
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow) {
#else
int main(int argc, char* argv[]) {
#endif

	CCoreUserInterface *coreUserInterface = createSSWEDevice();

	updateSSWEDevice(coreUserInterface);


	return EXIT_SUCCESS;
}
