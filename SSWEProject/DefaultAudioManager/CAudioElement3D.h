//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#ifndef __C_DEFAULT_AUDIO_ELEMENT_3D_H_INCLUDED__
#define __C_DEFAULT_AUDIO_ELEMENT_3D_H_INCLUDED__

/*
include irrlicht.h to get irrlicht methods
include ISSWELibPlugin.h to enable plugin system
include DLLExport.h to precise how the createSSWELibPlugin will be compiled
*/

#include <irrlicht.h>

#include <IAudioElement3D.h>

#include "fmod.hpp"
#include "fmod_errors.h"

namespace cp3d {
namespace audio {

class CAudioElement3D : public cp3d::audio::IAudioElement3D {

public:

	CAudioElement3D(FMOD::System *system, irr::scene::ISceneManager *smgr, irr::core::stringc path = "");
	~CAudioElement3D();

	void setPosition(irr::core::vector3df position);

	void setDistanceFactor(irr::f32 distanceFactor);

	void setMinMaxDistance(irr::f32 minDistance, irr::f32 maxDistance);

	void pause();

	bool load(irr::core::stringc path);

	bool play();

	void close();

	void getSpectrum(irr::f32 *spectrumTable, irr::s32 numElements) { }

	irr::u32 getCurrentTime();

	irr::u32 getDuration();

	void setCurrentTime(irr::u32 time);

	void setLoopMode(bool enable);

private:

	irr::scene::ISceneManager *smgr;
	irr::scene::ICameraSceneNode *camera;

	FMOD_RESULT result;
	FMOD::System *system;
	FMOD::Sound *sound;
	FMOD::Channel *channel;

	irr::f32 setting;
	FMOD_VECTOR position;
	FMOD_VECTOR velocity;
	FMOD_VECTOR listenerpos;

	bool paused;

};

} /// End namespace audio
} /// End namespace audio

#endif
