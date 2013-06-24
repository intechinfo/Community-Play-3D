// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SSWEOCULUSRIFTPLUGIN_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SSWEOCULUSRIFTPLUGIN_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SSWEOCULUSRIFTPLUGIN_EXPORTS
#define SSWEOCULUSRIFTPLUGIN_API __declspec(dllexport)
#else
#define SSWEOCULUSRIFTPLUGIN_API __declspec(dllimport)
#endif

#include "OculusRift.h"

// This class is exported from the SSWEOculusRiftPlugin.dll
class SSWEOCULUSRIFTPLUGIN_API CSSWEOculusRiftPlugin {
public:
	CSSWEOculusRiftPlugin();
	// TODO: add your methods here.
};

extern SSWEOCULUSRIFTPLUGIN_API int nSSWEOculusRiftPlugin;

SSWEOCULUSRIFTPLUGIN_API int fnSSWEOculusRiftPlugin(void);
