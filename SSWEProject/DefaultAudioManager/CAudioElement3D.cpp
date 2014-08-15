//
//  CUIWindowEditGrid.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 08/10/12.
//
//
#include "Stdafx.h"

#include "CAudioElement3D.h"

namespace cp3d {
namespace audio {

CAudioElement3D::CAudioElement3D(FMOD::System *system, irr::scene::ISceneManager *smgr, irr::core::stringc path)
	: IAudioElement3D()
{
	assert(Type == EAET_3D);

	this->system = system;
	channel = 0;

	this->smgr = smgr;
	camera = smgr->getActiveCamera();

	position.x = 0.0f; position.y = 0.f; position.z = 0.f;
	velocity.x = 0.f; velocity.y = 0.f; velocity.z = 0.f;

	system->set3DSettings(setting, setting, setting);

	if (path != "")
		load(path);
}

CAudioElement3D::~CAudioElement3D() {
	this->close();
}

void CAudioElement3D::pause() {
	if (!channel)
		return;

	channel->getPaused(&paused);
	channel->setPaused(!paused);
}

void CAudioElement3D::setPosition(irr::core::vector3df position) {
	if (!channel)
		return;

	camera = smgr->getActiveCamera();

	velocity.x = ( (position.X-Position.X) / 50.f );
    velocity.y = ( (position.Z-Position.Z) / 50.f );
	velocity.z = ( (position.Y-Position.Y) / 50.f );

	listenerpos.x = camera->getPosition().X;
	listenerpos.y = camera->getPosition().Z;
	listenerpos.z = camera->getPosition().Y;

	irr::core::vector3df f = camera->getTarget()-camera->getAbsolutePosition();
	f.normalize();
	FMOD_VECTOR forward = {f.X, f.Z, f.Y};

	f = camera->getUpVector();
	f.normalize();
	FMOD_VECTOR up = {f.X, f.Z, f.Y};

	FMOD_VECTOR channelPosition = {position.X, position.Z, position.Y};

	channel->set3DAttributes(&channelPosition, &velocity);
	system->set3DListenerAttributes(0, &listenerpos, &velocity, &forward, &up);

	system->update();

	IAudioElement3D::setPosition(position);
}

void CAudioElement3D::setDistanceFactor(irr::f32 distanceFactor) {
	if (distanceFactor == DistanceFactor)
		return;

	IAudioElement3D::setDistanceFactor(distanceFactor);
}

void CAudioElement3D::setMinMaxDistance(irr::f32 minDistance, irr::f32 maxDistance) {
	sound->set3DMinMaxDistance(minDistance, maxDistance);

	IAudioElement3D::setMinMaxDistance(minDistance, maxDistance);
}

bool CAudioElement3D::load(irr::core::stringc path) {
	result = system->createSound(path.c_str(), FMOD_3D | FMOD_HARDWARE, 0, &sound);
	assert(result == FMOD_OK);

	sound->set3DMinMaxDistance(MinDistance, MaxDistance);
	assert(result == FMOD_OK);

	return result == FMOD_OK;
}

bool CAudioElement3D::play() {
	result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);

	sound->set3DMinMaxDistance(MinDistance, MaxDistance);
	result = channel->set3DAttributes(&position, &velocity);

	assert(result == FMOD_OK);

	return result == FMOD_OK;
}

irr::u32 CAudioElement3D::getCurrentTime() {
	if (!channel) return 0;

	irr::u32 time;
	channel->getPosition(&time, FMOD_TIMEUNIT_MS);

	return time;
}

irr::u32 CAudioElement3D::getDuration() {
	if (!sound) return 0;

	irr::u32 time;
	sound->getLength(&time, FMOD_TIMEUNIT_MS);

	return time;
}

void CAudioElement3D::setCurrentTime(irr::u32 time) {
	if (!channel)
		return;

	channel->setPosition(time, FMOD_TIMEUNIT_MS);
}

void CAudioElement3D::setLoopMode(bool enable) {
	if (enable)
		channel->setMode(FMOD_LOOP_NORMAL);
	else
		channel->setMode(FMOD_LOOP_OFF);

	IAudioElement3D::setLoopMode(enable);
}

void CAudioElement3D::close() {
	result = sound->release();

	assert(result == FMOD_OK);
}

} /// End namespace audio
} /// End namespace audio
