// SSWECore.h : fichier d'en-tête principal pour la DLL SSWECore
//

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

#define SSWE_CORE_EXPORTS
#ifdef SSWE_CORE_EXPORTS
	#define SSWE_CORE_API __declspec(dllexport)
#else
	#define SSWE_CORE_API __declspec(dllimport)
#endif

#if defined(_STDCALL_SUPPORTED)
	#define SSWECORECALLCONV __stdcall
#else
	#define SSWECORECALLCONV __cdecl
#endif
