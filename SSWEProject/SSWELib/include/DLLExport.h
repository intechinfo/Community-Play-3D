// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SSWEGENERICMONITOR_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SSWEGENERICMONITOR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

//MONITOR EXPORTS
#ifndef MONITOR_DLL_EXPORTS_H
#define MONITOR_DLL_EXPORTS_H

#ifdef __dll__
#define MONITOR_EXPORT __declspec(dllexport)
#else
#define MONITOR_EXPORT __declspec(dllimport)
#endif

extern "C" MONITOR_EXPORT void* createMonitor();

#endif

//SSWELIB EXPORTS
#ifndef SSWELIB_DLL_EXPORTS_H
#define SSWELIB_DLL_EXPORTS_H

#ifdef __dll__
#define SSWELIB_EXPORT __declspec(dllexport)
#else
#define SSWELIB_EXPORT __declspec(dllimport)
#endif

extern "C" SSWELIB_EXPORT void* createSSWELibPlugin();

#endif
