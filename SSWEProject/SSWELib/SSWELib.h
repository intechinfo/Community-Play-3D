// SSWELib.h : fichier d'en-tête principal pour la DLL SSWELib
//

#pragma once

#include "SSWELib/Device/Core/CCoreUserInterface.h"

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

#ifdef _IRR_OSX_PLATFORM_
extern "C" SSWE_LIB_API CCoreUserInterface* SSWELIBCALLCONV createSSWEDevice();
extern "C" SSWE_LIB_API void updateSSWEDevice(CCoreUserInterface *coreUserInterface);
#elif _SSWE_LINUX_
extern "C" CCoreUserInterface* createSSWEDevice();
extern "C" void updateSSWEDevice(CCoreUserInterface *coreUserInterface);
#else
extern "C" SSWE_LIB_API CCoreUserInterface* SSWELIBCALLCONV createSSWEDevice();
extern "C" SSWE_LIB_API void updateSSWEDevice(CCoreUserInterface *coreUserInterface);
#endif
