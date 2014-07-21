//
//  IAudioManager.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//

#ifndef __I_AUDIO_ELEMENT_3D_H_INCLUDED__
#define __I_AUDIO_ELEMENT_3D_H_INCLUDED__

#include "IAudioElement.h"

namespace cp3d {
namespace audio {

class IAudioElement3D : public IAudioElement {

public:

	/// ctor, default as a 3D audio element
	IAudioElement3D() : IAudioElement(EAET_3D) {
		DistanceFactor = 0.f;
		MinDistance = 1.f;
		MaxDistance = 1000.f;
	}

	/// Sets the new position of the sound
	virtual void setPosition(irr::core::vector3df position) {
		Position = position;
	}

	/// Sets the new distance factor of the sound
	virtual void setDistanceFactor(irr::f32 distanceFactor) {
		DistanceFactor = distanceFactor;
	}

	virtual void setMinMaxDistance(irr::f32 minDistance, irr::f32 maxDistance) {
		MinDistance = minDistance;
		MaxDistance = maxDistance;
	}

	/// Sets if update the 3d sound (position, etc.)
	void setUpdate(bool update) {
		Update = update;
	}

	/// Returns the position of the sound
	irr::core::vector3df getPosition() { return Position; }

	/// Returns the distance factor of the sound
	irr::f32 getDistanceFactor() { return DistanceFactor; }

	/// Retunrs the min distance of the sound
	irr::f32 getMinDistance() { return MinDistance; }

	/// Retunrs the max distance of the sound
	irr::f32 getMaxDistance() { return MaxDistance; }

	/// Retunrs if the sound is updating
	bool isUpdating() { return Update; }

protected:

	irr::core::vector3df Position;
	irr::f32 DistanceFactor;
	irr::f32 MinDistance, MaxDistance;

	bool Update;

};

} /// End namespace audio
} /// End namespace cp3d

#endif