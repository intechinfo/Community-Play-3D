//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#ifndef __C_DEFAULT_AUDIO_ELEMENT_H_INCLUDED__
#define __C_DEFAULT_AUDIO_ELEMENT_H_INCLUDED__

/*
include irrlicht.h to get irrlicht methods
include ISSWELibPlugin.h to enable plugin system
include DLLExport.h to precise how the createSSWELibPlugin will be compiled
*/

#include <irrlicht.h>

#include <IAudioElement.h>

#include "fmod.hpp"
#include "fmod_errors.h"

namespace cp3d {
namespace audio {

class CAudioElement : public cp3d::audio::IAudioElement {

public:

	CAudioElement(FMOD::System *system, irr::core::stringc path = "");
	~CAudioElement();

	void pause();

	bool load(irr::core::stringc path);

	bool play();

	void close();

	void getSpectrum(irr::f32 *spectrumTable, irr::s32 numElements);

	irr::u32 getCurrentTime();

	irr::u32 getDuration();

	void setCurrentTime(irr::u32 time);

	void setLoopMode(bool enable);

private:

	FMOD_RESULT result;
	FMOD::System *system;
	FMOD::Sound *sound;
	FMOD::Channel *channel;

	bool paused;
};

} /// End namespace audio
} /// End namespace audio

#endif