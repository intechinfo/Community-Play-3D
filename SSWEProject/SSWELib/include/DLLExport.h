// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the SSWEGENERICMONITOR_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// SSWEGENERICMONITOR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#pragma once

namespace cp3d {
	namespace audio {
		class IAudioManager;
	}
	namespace core {
		class ISSWELibPlugin;
        class IDevelomentInstance;
        class IDevices;
	}
	namespace video {
		class IMonitor;
	}
}

#ifdef _IRR_OSX_PLATFORM_

    #define _CP3D_PLUGIN_EXPORT_ __attribute__((visibility("default")))

#else

    #ifndef _CP3D_DLL_EXPORTS_H_
        #define _CP3D_DLL_EXPORTS_H_
        #ifdef __dll__
            #define _CP3D_PLUGIN_EXPORT_ __declspec(dllexport)
        #else
            #define _CP3D_PLUGIN_EXPORT_ __declspec(dllimport)
        #endif
    #endif

#endif

//MONITOR EXPORTS

#ifndef _IRR_LINUX_PLATFORM_
extern "C" _CP3D_PLUGIN_EXPORT_ cp3d::video::IMonitor* createMonitor();
#else
extern "C" cp3d::video::IMonitor* createMonitor();
#endif

//SSWELIB EXPORTS

#ifndef _IRR_LINUX_PLATFORM_
extern "C" _CP3D_PLUGIN_EXPORT_ cp3d::core::ISSWELibPlugin *createSSWELibPlugin();
#else
extern "C" cp3d::core::ISSWELibPlugin *createSSWELibPlugin();
#endif

//AUDIO EXPORTS

#ifndef _IRR_LINUX_PLATFORM_
extern "C" _CP3D_PLUGIN_EXPORT_ cp3d::audio::IAudioManager* createAudioManager();
#else
extern "C" cp3d::audio::IAudioManager* createAudioManager();
#endif

//DEVELOPMENT INSTANCE EXPORTS
#ifndef _IRR_LINUX_PLATFORM_
extern "C" _CP3D_PLUGIN_EXPORT_ cp3d::core::IDevelomentInstance *createDevelopmentInstance(cp3d::core::IDevices *devices);
#else
extern "C" cp3d::core::IDevelomentInstance* createDevelopmentInstance(cp3d::core::IDevices *devices);
#endif
