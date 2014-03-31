// SSWELib.h : fichier d'en-tête principal pour la DLL SSWELib
//
#ifndef __C_SSWE_LIB_H_INCLUDED__
#define __C_SSWE_LIB_H_INCLUDED__

#include <ISSWECoreUserInterface.h>

#pragma once

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

#ifndef _IRR_OSX_PLATFORM_
extern "C" SSWE_LIB_API ISSWECoreUserInterface* SSWELIBCALLCONV createSSWEDevice(bool playOnly = false, irr::core::stringc argPath = "");
extern "C" SSWE_LIB_API void updateSSWEDevice(ISSWECoreUserInterface *_coreUserInterface);
#else
ISSWECoreUserInterface* createSSWEDevice(bool playOnly = false, irr::core::stringc argPath = "");
void updateSSWEDevice(ISSWECoreUserInterface *_coreUserInterface);
#endif

#endif
