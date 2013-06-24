// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SSWEGENERICMONITOR_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SSWEGENERICMONITOR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#define SSWE_GENERIC_MONITOR_EXPORTS
#ifdef SSWE_GENERIC_MONITOR_EXPORTS
	#define SSWE_GENERIC_MONITOR_API __declspec(dllexport)
#else
	#define SSWE_GENERIC_MONITOR_API __declspec(dllimport)
#endif

#if defined(_STDCALL_SUPPORTED)
	#define SSWEGENERICMONITORCALLCONV __stdcall
#else
	#define SSWEGENERICMONITORCALLCONV __cdecl
#endif

