//! This is just a copy of the CSceneNodeAnimatorCameraMaya.h
 
/** Original Copyright is this one !!!!!
//Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
**/

#include "stdafx.h"
 
#include "CCharacterControl.h"

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;

CCharacterControl::CCharacterControl(irr::scene::IAnimatedMeshSceneNode *node,
									 irr::scene::ICameraSceneNode *camera,
									 cp3d::core::IDevices *devices)
{
	this->node = node;
	this->camera = camera;
	this->devices = devices;

	/// Configure node
	node->setLoopMode(true);
	node->setRotation(core::vector3df(0.f, 90.f, 0.f));
	devices->getAnimationController()->applyAnimationFromNameToModel(node, "IDLE");
	currentAnimationType = ECA_IDLE;
	wantedAnimationType = ECA_IDLE;

	/// Get target of the camera and set callback
	boneNode = node->getJointNode("Head");

	/// Devices
	devices->getEventReceiver()->AddEventReceiver(this, 0, this);

	/// Set up keymap
	KeyMap.push_back(SKeyMap(EKA_MOVE_FORWARD, irr::KEY_KEY_Z));
	KeyMap.push_back(SKeyMap(EKA_MOVE_BACKWARD, irr::KEY_KEY_S));
	KeyMap.push_back(SKeyMap(EKA_STRAFE_LEFT, irr::KEY_KEY_Q));
	KeyMap.push_back(SKeyMap(EKA_STRAFE_RIGHT, irr::KEY_KEY_D));
	KeyMap.push_back(SKeyMap(EKA_CROUCH, irr::KEY_KEY_T));

	for (u32 i=0; i < EKA_COUNT; i++)
		CursorKeys[i] = false;

	direction = 0;
	zdirection = 0;

	currentRotationTransitionMovement = 0.f;
	shiftDown = false;
	isRotating = false;

	then = devices->getDevice()->getTimer()->getTime();

}
CCharacterControl::~CCharacterControl() {

}

void CCharacterControl::update() {
	/// Update to camera position in function of the bone scene node
	boneNode->updateAbsolutePosition();
	boneNode->updateAbsolutePositionOfAllChildren();

	/// Get angle offset
	core::line3d<f32> ray;
    ray.start = camera->getPosition();
    ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 1000.0f;
	core::vector3df toTarget(ray.end - node->getAbsolutePosition());
	toTarget.Y == 180.f ? toTarget.Y += 1.f : toTarget.Y += 0.f;

	/// Get direction of the displacement
	irr::core::vector3df cdirection = node->getRotation().rotationToDirection(irr::core::vector3df(1, 0, 0));

	/// Animate with current animation
	if (wantedAnimationType == ECA_IDLE && currentAnimationType != ECA_IDLE) {
		devices->getAnimationController()->applyAnimationFromNameToModel(node, "IDLE");
		currentAnimationType = ECA_IDLE;
	}

	const u32 now = devices->getDevice()->getTimer()->getTime();
	const f32 frameDeltaTime = (f32)(now - then) / 1000.f;

	if (CursorKeys[EKA_MOVE_FORWARD] || CursorKeys[EKA_MOVE_BACKWARD] || CursorKeys[EKA_STRAFE_LEFT] || CursorKeys[EKA_STRAFE_RIGHT]) {

		if (currentAnimationType == ECA_IDLE) {
			if (shiftDown)
				wantedAnimationType = ECA_RUN;
			else
				wantedAnimationType = ECA_WALK;
		}

		if (wantedAnimationType == ECA_RUN && currentAnimationType != ECA_RUN && shiftDown) {
			devices->getAnimationController()->applyAnimationFromNameToModel(node, "RUN");
			currentAnimationType = ECA_RUN;
		}
		if (wantedAnimationType == ECA_WALK && currentAnimationType != ECA_WALK && !shiftDown) {
			devices->getAnimationController()->applyAnimationFromNameToModel(node, "WALK");
			currentAnimationType = ECA_WALK;
		}

		/// Update character position
		devices->getDevice()->getLogger()->log(stringc(frameDeltaTime).c_str());
		then = now;
		node->setPosition(node->getPosition() + cdirection * frameDeltaTime*10.f * ((isRotating) ? 0.f : (currentAnimationType == ECA_WALK) ? 1.f : 3.f));
	}

	/// Update character rotation in function of the movement type
	if (CursorKeys[EKA_MOVE_FORWARD]) {
		calculateAngleMovement(toTarget.getHorizontalAngle().Y-90.f);
	} else if (CursorKeys[EKA_MOVE_BACKWARD]) {
		calculateAngleMovement(toTarget.getHorizontalAngle().Y+90.f);
	} else if (CursorKeys[EKA_STRAFE_RIGHT]) {
		calculateAngleMovement(toTarget.getHorizontalAngle().Y);
	} else if (CursorKeys[EKA_STRAFE_LEFT]) {
		calculateAngleMovement(toTarget.getHorizontalAngle().Y+180.f);
	}

	node->setRotation(core::vector3df(node->getRotation().X,
									  currentRotationTransitionMovement,
									  node->getRotation().Z));
	moveCameraControl();
}

void CCharacterControl::calculateAngleMovement(irr::f32 valueToReach) {
	if (currentRotationTransitionMovement == 180.f) {
		if (currentRotationTransitionMovement <= 180.f && valueToReach >= 180.f) {
			currentRotationTransitionMovement += 4.f;
		} else {
			currentRotationTransitionMovement -= 4.f;
		}
	}

	if (valueToReach < 0.f) {
		valueToReach = 360.f + valueToReach;
	}
	if (valueToReach > 360.f) {
		valueToReach = 360.f - valueToReach;
	}

	if (currentRotationTransitionMovement == 360.f)
		currentRotationTransitionMovement = 0.f;
	if (currentRotationTransitionMovement > 360.f)
		currentRotationTransitionMovement -= 360.f;
	if (currentRotationTransitionMovement < 0.f)
		currentRotationTransitionMovement += 360.f;

	stringc txt = "";
	txt += currentRotationTransitionMovement;
	txt += " ------ ";
	txt += valueToReach;
	devices->getDevice()->getLogger()->log(txt.c_str());

	if (currentRotationTransitionMovement >= 180.f && valueToReach >= 180.f) {
		if (currentRotationTransitionMovement < valueToReach) {
			currentRotationTransitionMovement += (valueToReach - currentRotationTransitionMovement > 4.f) ? 4.f : 0.f;
		} else {
			currentRotationTransitionMovement -= (currentRotationTransitionMovement - valueToReach > 4.f) ? 4.f : 0.f;
		}
		return;
	} else
	if (currentRotationTransitionMovement <= 180.f && valueToReach <= 180.f) {
		if (currentRotationTransitionMovement < valueToReach) {
			currentRotationTransitionMovement += (valueToReach - currentRotationTransitionMovement > 4.f) ? 4.f : 0.f;
		} else {
			currentRotationTransitionMovement -= (currentRotationTransitionMovement - valueToReach > 4.f) ? 4.f : 0.f;
		}
		return;
	} else
	if (currentRotationTransitionMovement >= 180.f && valueToReach <= 180.f) {
		if (this->getAngleWith(currentRotationTransitionMovement, 180.f) > valueToReach) {
			currentRotationTransitionMovement += (valueToReach + this->getAngleWith(currentRotationTransitionMovement, 180.f) > 4.f) ? 4.f : 0.f;
		} else {
			currentRotationTransitionMovement -= (valueToReach - currentRotationTransitionMovement < 4.f) ? 4.f : 0.f;
		}
		return;
	} else
	if (currentRotationTransitionMovement <= 180.f && valueToReach >= 180.f) {
		if (this->getAngleWith(currentRotationTransitionMovement, 180.f) > valueToReach) {
			currentRotationTransitionMovement += (this->getAngleWith(currentRotationTransitionMovement, 180.f) - valueToReach > 4.f) ? 4.f : 0.f;
		} else {
			currentRotationTransitionMovement -= (valueToReach - this->getAngleWith(currentRotationTransitionMovement, 180.f) > 4.f) ? 4.f : 0.f;
		}
		return;
	}
}

irr::f32 CCharacterControl::getAngleWith(irr::f32 angle, irr::f32 toAdd) {
	f32 t = angle + toAdd;
	if (t > 360.f)
		t -= 360.f;
	if (t < 0.f)
		t += 360.f;
	return t;
}

bool CCharacterControl::OnEvent(const irr::SEvent &event) {

	if (event.EventType == EET_KEY_INPUT_EVENT) {
		for (u32 i=0; i<KeyMap.size(); ++i) {
			if (KeyMap[i].KeyCode == event.KeyInput.Key) {
				CursorKeys[KeyMap[i].Action] = event.KeyInput.PressedDown;
			}
		}
		
		shiftDown = event.KeyInput.Shift;

		if (!event.KeyInput.PressedDown && (!CursorKeys[EKA_MOVE_FORWARD] && !CursorKeys[EKA_MOVE_BACKWARD]
			&& !CursorKeys[EKA_STRAFE_LEFT] && !CursorKeys[EKA_STRAFE_RIGHT]))
		{
			wantedAnimationType = ECA_IDLE;
		}

		return true;
	}

	return false;
}

void CCharacterControl::moveCameraControl() {
	core::position2d<f32> cursorPos = devices->getDevice()->getCursorControl()->getRelativePosition();
     
	f32 change_x = ( cursorPos.X - 0.5f ) * 256.0f;
	f32 change_y = ( cursorPos.Y - 0.5f ) * 256.0f;
	direction += change_x;
	zdirection -= change_y;
	if( zdirection < -90 )
		zdirection = -90;
	else
	if( zdirection > 90 )
		zdirection = 90;
	devices->getDevice()->getCursorControl()->setPosition( 0.5f, 0.5f );

	core::vector3df playerPos = node->getPosition();

	f32 xf = playerPos.X - cos( direction * core::PI / 180.0f ) * 44.0f * node->getScale().X;
	f32 yf = playerPos.Y - sin( zdirection * core::PI / 180.0f ) * 44.0f * node->getScale().X * 2.f;
	f32 zf = playerPos.Z + sin( direction * core::PI / 180.0f ) * 44.0f * node->getScale().X;

	camera->setTarget(boneNode->getAbsolutePosition());
	camera->setPosition(core::vector3df(xf, yf, zf));

	//node->setRotation(core::vector3df(node->getRotation().X, direction, node->getRotation().Z));
}
