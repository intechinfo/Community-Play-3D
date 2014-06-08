// SSWERenders.h : fichier d'en-tête principal pour la DLL SSWERenders
//

#define SSWE_RENDERS_EXPORTS

#ifndef _IRR_LINUX_PLATFORM_
    #ifdef SSWE_RENDERS_EXPORTS
        #define SSWE_RENDERS_API __declspec(dllexport)
    #else
        #define SSWE_RENDERS_API __declspec(dllimport)
    #endif
#else
    #define SSWE_RENDERS_API
#endif

#if defined(_STDCALL_SUPPORTED)
    #define SSWERENDERSCALLCONV __stdcall
#else
    #define SSWERENDERSCALLCONV __cdecl
#endif
