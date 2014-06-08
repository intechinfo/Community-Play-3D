// SSWECore.h : fichier d'en-tête principal pour la DLL SSWECore
//

#define SSWE_CORE_EXPORTS

#ifndef _IRR_LINUX_PLATFORM_
    #ifdef SSWE_CORE_EXPORTS
        #define SSWE_CORE_API __declspec(dllexport)
    #else
        #define SSWE_CORE_API __declspec(dllimport)
    #endif
#else
    #define SSWE_CORE_API
#endif

#if defined(_STDCALL_SUPPORTED)
	#define SSWECORECALLCONV __stdcall
#else
	#define SSWECORECALLCONV __cdecl
#endif
