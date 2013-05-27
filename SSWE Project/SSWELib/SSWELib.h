// SSWELib.h�: fichier d'en-t�te principal pour la DLL SSWELib
//

//#pragma once

#include "../../sources/Device/Core/CCoreUserInterface.h"

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

#define SSWE_LIB_EXPORTS
#ifdef SSWE_LIB_EXPORTS
	#define SSWE_LIB_API __declspec(dllexport)
#else
	#define SSWE_LIB_API __declspec(dllimport)
#endif

#if defined(_STDCALL_SUPPORTED)
	#define SSWELIBCALLCONV __stdcall
#else
	#define SSWELIBCALLCONV __cdecl
#endif

extern "C" SSWE_LIB_API CCoreUserInterface* SSWELIBCALLCONV createSSWEDevice();
extern "C" SSWE_LIB_API void updateSSWEDevice(CCoreUserInterface *coreUserInterface);
