// SSWERenders.h : fichier d'en-tête principal pour la DLL SSWERenders
//

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

#define SSWE_RENDERS_EXPORTS
#ifdef SSWE_RENDERS_EXPORTS
	#define SSWE_RENDERS_API __declspec(dllexport)
#else
	#define SSWE_RENDERS_API __declspec(dllimport)
#endif

#if defined(_STDCALL_SUPPORTED)
	#define SSWERENDERSCALLCONV __stdcall
#else
	#define SSWERENDERSCALLCONV __cdecl
#endif
