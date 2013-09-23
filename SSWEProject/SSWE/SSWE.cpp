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

#ifdef IS_ERIO_AND_RELOU
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow) {
#else
int main(int argc, char* argv[]) {
#endif

	CCoreUserInterface *coreUserInterface = createSSWEDevice();

	updateSSWEDevice(coreUserInterface);

	/*wchar_t szMessage[300] = L"C:\\Users\\julienmoreau-mathis\\Desktop\\Dual_Hilbert_Cube_ABS+PVA.gcode";
	//char *t = reinterpret_cast<char *>(&szMessage);
	char t[300];
	for (u32 i=0; i < 300; i++) {
		t[i] = (char)szMessage[i];
	}

	printf("%s", t);

	char c;
	std::cin >> c;*/


	return EXIT_SUCCESS;
}
