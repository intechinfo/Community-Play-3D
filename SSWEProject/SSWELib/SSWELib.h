// SSWELib.h : fichier d'en-tête principal pour la DLL SSWELib
//
#ifndef __C_SSWE_LIB_H_INCLUDED__
#define __C_SSWE_LIB_H_INCLUDED__

#include <ISSWECoreUserInterface.h>

#pragma once

#define SSWE_LIB_EXPORTS
#ifndef _IRR_LINUX_PLATFORM_
    #ifdef SSWE_LIB_EXPORTS
        #define SSWE_LIB_API __declspec(dllexport)
    #else
        #define SSWE_LIB_API __declspec(dllimport)
    #endif
#else
    #define SSWE_LIB_API
#endif

#if defined(_STDCALL_SUPPORTED)
	#define SSWELIBCALLCONV __stdcall
#else
	#define SSWELIBCALLCONV __cdecl
#endif

#ifdef _IRR_WINDOWS_API_
extern "C" SSWE_LIB_API cp3d::ISSWECoreUserInterface* SSWELIBCALLCONV createSSWEDevice(bool playOnly = false, irr::core::stringc argPath = "");
extern "C" SSWE_LIB_API void updateSSWEDevice(cp3d::ISSWECoreUserInterface *_coreUserInterface);
#else
ISSWECoreUserInterface* createSSWEDevice(bool playOnly = false, irr::core::stringc argPath = "");
void updateSSWEDevice(ISSWECoreUserInterface *_coreUserInterface);
#endif

#endif
