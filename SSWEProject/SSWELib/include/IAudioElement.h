//
//  IAudioManager.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//

#ifndef __I_AUDIO_ELEMENT_H_INCLUDED__
#define __I_AUDIO_ELEMENT_H_INCLUDED__

namespace cp3d {
namespace audio {

enum E_AUDIO_ELEMENT_TYPE {
	EAET_2D = 0,
	EAET_3D,
	EAET_COUNT
};

class IAudioElement {

public:

	IAudioElement(E_AUDIO_ELEMENT_TYPE type = EAET_2D) {
		Type = type;
	}

	virtual void pause() = 0;

	virtual bool load(irr::core::stringc path) = 0;

	virtual void play() = 0;

	virtual void close() = 0;

	virtual void getSpectrum(irr::f32 *spectrumTable, irr::s32 numElements) = 0;

	virtual void setCurrentTime(irr::u32 time) = 0;

	virtual irr::u32 getCurrentTime() = 0;

	virtual irr::u32 getDuration() = 0;

protected:

	E_AUDIO_ELEMENT_TYPE Type;

};

} /// End namespace audio
} /// End namespace cp3d

#endif