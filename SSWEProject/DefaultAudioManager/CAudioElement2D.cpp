//
//  CUIWindowEditGrid.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 08/10/12.
//
//
#include "Stdafx.h"

#include "CAudioElement2D.h"

namespace cp3d {
namespace audio {

CAudioElement::CAudioElement(FMOD::System *system, irr::core::stringc path) {
	assert(Type == EAET_2D);

	this->system = system;
	channel = 0;

	if (path != "")
		load(path);
}

CAudioElement::~CAudioElement() {
	this->close();
}

void CAudioElement::pause() {
	if (!channel)
		return;

	channel->getPaused(&paused);
	channel->setPaused(!paused);
}

bool CAudioElement::load(irr::core::stringc path) {
	result = result = system->createSound(path.c_str(), FMOD_HARDWARE, 0, &sound);

	assert(result == FMOD_OK);
	return result == FMOD_OK;
}

bool CAudioElement::play() {
	result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
	assert(result == FMOD_OK);

	return result == FMOD_OK;
}

void CAudioElement::getSpectrum(irr::f32 *spectrumTable, irr::s32 numElements) {
	if (!channel)
		return;

	channel->getSpectrum(spectrumTable, numElements, 0, FMOD_DSP_FFT_WINDOW_RECT);
}

void CAudioElement::setCurrentTime(irr::u32 time) {
	channel->setPosition(time, FMOD_TIMEUNIT_MS);
}

irr::u32 CAudioElement::getCurrentTime() {
	if (!channel) return 0;

	irr::u32 time;
	channel->getPosition(&time, FMOD_TIMEUNIT_MS);

	return time;
}

irr::u32 CAudioElement::getDuration() {
	if (!sound) return 0;

	irr::u32 time;
	sound->getLength(&time, FMOD_TIMEUNIT_MS);

	return time;
}

void CAudioElement::setLoopMode(bool enable) {
	if (enable)
		channel->setMode(FMOD_LOOP_NORMAL);
	else
		channel->setMode(FMOD_LOOP_OFF);

	IAudioElement::setLoopMode(enable);
}

void CAudioElement::close() {
	result = sound->release();

	assert(result == FMOD_OK);
}

} /// End namespace audio
} /// End namespace audio