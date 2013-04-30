//
//  CAction.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 07/11/12.
//
//

#include "CRotationAnimator.h"

namespace irr {
namespace scene {

CRotationAnimator::CRotationAnimator(const core::vector3df _startRotation, const core::vector3df &_endRotation, 
									 u32 _timeToComplete, bool _positive, u32 _startTime)
									 : ISceneNodeAnimatorFinishing(_startTime + timeToComplete),
									   startRotation(_startRotation), endRotation(_endRotation), timeToComplete(_timeToComplete), 
									   positive(_positive), startTime(_startTime) {
	#ifdef _DEBUG
	setDebugName("CSceneNodeAnimatorRotation");
	#endif

	if (endRotation.X < startRotation.X) {
		difference.X = startRotation.X - endRotation.X;
	} else {
		difference.X = endRotation.X - startRotation.X;
	}
	if (endRotation.Y < startRotation.Y) {
		difference.Y = startRotation.Y - endRotation.Y;
	} else {
		difference.Y = endRotation.Y - startRotation.Y;
	}
	if (endRotation.Z < startRotation.Z) {
		difference.Z = startRotation.Z - endRotation.Z;
	} else {
		difference.Z = endRotation.Z - startRotation.Z;
	}
}

void CRotationAnimator::animateNode(ISceneNode* node, u32 timeMs) {
	if (node) {
		const u32 diffTime = timeMs - startTime;
		if (diffTime != 0) {
			core::vector3df rot;
			if (diffTime >= timeToComplete) {
				rot = node->getRotation();
				HasFinished = true;
				node->setRotation(rot);
				//node->removeAnimator(this);
			} else {
				if (positive) {
					rot.X += (difference.X / timeToComplete)*diffTime;
					rot.Y += (difference.Y / timeToComplete)*diffTime;
					rot.Z += (difference.Z / timeToComplete)*diffTime;
				} else {
					rot.X -= (difference.X / timeToComplete)*diffTime;
					rot.Y -= (difference.Y / timeToComplete)*diffTime;
					rot.Z -= (difference.Z / timeToComplete)*diffTime;
				}
				node->setRotation(rot);
			}
		}
	}
}

void CRotationAnimator::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const {
	//out->addVector3d("Rotation", endRotation);
}

void CRotationAnimator::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options) {
	//endRotation = in->getAttributeAsVector3d("Rotation");
}

ISceneNodeAnimator* CRotationAnimator::createClone(ISceneNode* node, ISceneManager* newManager) {
	CRotationAnimator * newAnimator = new CRotationAnimator(startRotation, endRotation, timeToComplete, positive, startTime);
	return newAnimator;
}

} // end namespace scene
} // end namespace irr



