//
//  CExporter.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#ifndef __C_DEFAULT_AUDIO_MANAGER_H_INCLUDED__
#define __C_DEFAULT_AUDIO_MANAGER_H_INCLUDED__

/*
include irrlicht.h to get irrlicht methods
include ISSWELibPlugin.h to enable plugin system
include DLLExport.h to precise how the createSSWELibPlugin will be compiled
*/

#include <irrlicht.h>
#include <IAudioManager.h>

#include "CAudioElement2D.h"
#include "CAudioElement3D.h"

namespace cp3d {
namespace audio {

class CDefaultAudioManager : public cp3d::audio::IAudioManager {

public:

	CDefaultAudioManager();
	~CDefaultAudioManager();

	bool initialize();

	void close(bool deleteElements = true);

	IAudioElement *add2DAudioElementFromFile(irr::core::stringc path);
	IAudioElement3D *add3DAudioElementFromFile(irr::core::stringc path, irr::scene::ISceneManager *smgr);

private:

	/// FMOD
	FMOD::System     *system;
    FMOD::Sound      *sound1;
    FMOD::Channel    *channel;
    FMOD_RESULT       result;

};

} /// End namespace audio
} /// End namespace cp3d

#endif
