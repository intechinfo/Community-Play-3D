// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SSWEGENERICMONITOR_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SSWEGENERICMONITOR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SSWEGENERICMONITOR_EXPORTS
#define SSWEGENERICMONITOR_API __declspec(dllexport)
#else
#define SSWEGENERICMONITOR_API __declspec(dllimport)
#endif

// This class is exported from the SSWEGenericMonitor.dll
class SSWEGENERICMONITOR_API CSSWEGenericMonitor {
public:
	CSSWEGenericMonitor(void);
	// TODO: add your methods here.
};

extern SSWEGENERICMONITOR_API int nSSWEGenericMonitor;

SSWEGENERICMONITOR_API int fnSSWEGenericMonitor(void);
