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

	/// ctor, default : 2D Sound
	IAudioElement(E_AUDIO_ELEMENT_TYPE type = EAET_2D) {
		Type = type;
	}

	/// Make the sound pause. It already paused, the sound plays again
	virtual void pause() = 0;

	/// Load a sound from the file located at "path"
	virtual bool load(irr::core::stringc path) = 0;

	/// Play the sound
	virtual bool play() = 0;

	/// Close the sound. Equivalent to Dispose in C#
	virtual void close() = 0;

	/// Gets the spectrum of the current sound
	/// spectrumTable, the float table that filled by the sound
	/// numElements, the number of spectrum elements
	virtual void getSpectrum(irr::f32 *spectrumTable, irr::s32 numElements) = 0;

	/// Set the current time of the sound
	virtual void setCurrentTime(irr::u32 time) = 0;

	/// Retunrs the current time of the sound
	virtual irr::u32 getCurrentTime() = 0;

	/// Get total time of the sound, duration
	virtual irr::u32 getDuration() = 0;

	/// Set loop mode enabled or disabled
	virtual void setLoopMode(bool enable) {
		LoopMode = enable;
	}

	bool isLoopMode() {
		return LoopMode;
	}

	/// Retunrs the type of the sound : 2D or 3D
	E_AUDIO_ELEMENT_TYPE getType() {
		return Type;
	}

	/// Sets the audio file path
	void setPath(irr::core::stringc path) {
		Path = path;
	}

	/// Retunrs the audio file path
	irr::core::stringc getPath() {
		return Path;
	}

protected:

	E_AUDIO_ELEMENT_TYPE Type;
	irr::core::stringc Path;
	bool LoopMode;

};

} /// End namespace audio
} /// End namespace cp3d

#endif