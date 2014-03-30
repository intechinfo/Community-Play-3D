// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "stdafx.h"

#if defined(_WIN32)

#include "CCameraFPS360Controller.h"
#include <Windows.h>

#pragma comment(lib, "XInput.lib")

namespace irr
{
namespace scene
{

//! constructor
CCameraAnimatorFPS360Controller::CCameraAnimatorFPS360Controller(gui::ICursorControl* cursorControl, f32 rotateSpeed, f32 moveSpeed, f32 jumpSpeed,
		SKeyMap* keyMapArray, u32 keyMapSize, bool noVerticalMovement, bool invertY)
:   CursorControl(cursorControl), MaxVerticalAngle(88.0f),
	MoveSpeed(moveSpeed), RotateSpeed(rotateSpeed), JumpSpeed(jumpSpeed),
	MouseYDirection(invertY ? -1.0f : 1.0f),
	LastAnimationTime(0), firstUpdate(true), firstInput(true), NoVerticalMovement(noVerticalMovement)
{
	#ifdef _DEBUG
	setDebugName("CCameraFPS360Controller");
	#endif

	_controllerNum = 0;
}


//! destructor
CCameraAnimatorFPS360Controller::~CCameraAnimatorFPS360Controller()
{

}


//! It is possible to send mouse and key events to the camera. Most cameras
//! may ignore this input, but camera scene nodes which are created for
//! example with scene::ISceneManager::addMayaCameraSceneNode or
//! scene::ISceneManager::addFPSCameraSceneNode, may want to get this input
//! for changing their position, look at target or whatever.
bool CCameraAnimatorFPS360Controller::OnEvent(const SEvent& evt)
{
	return false;
}


void CCameraAnimatorFPS360Controller::animateNode(ISceneNode* node, u32 timeMs)
{
	if (!node || node->getType() != ESNT_CAMERA)
		return;

	ICameraSceneNode* camera = static_cast<ICameraSceneNode*>(node);

	if (firstUpdate)
	{
		camera->updateAbsolutePosition();
		if (CursorControl )
		{
			CursorControl->setPosition(0.5f, 0.5f);
			CursorPos = CenterCursor = CursorControl->getRelativePosition();
		}

		LastAnimationTime = timeMs;

		firstUpdate = false;
	}

	// If the camera isn't the active camera, and receiving input, then don't process it.
	if(!camera->isInputReceiverEnabled())
	{
		firstInput = true;
		return;
	}

	if ( firstInput )
	{
		firstInput = false;
	}

	scene::ISceneManager * smgr = camera->getSceneManager();
	if(smgr && smgr->getActiveCamera() != camera)
		return;

	// get time
	f32 timeDiff = (f32) ( timeMs - LastAnimationTime );
	LastAnimationTime = timeMs;

	// update position
	core::vector3df pos = camera->getPosition();

	// Update rotation
	core::vector3df target = (camera->getTarget() - camera->getAbsolutePosition());
	core::vector3df relativeRotation = target.getHorizontalAngle();

	if (this->IsConnected())
	{
		/// Get XBOX Controller states and values + Dead Zones + direction
		float LX = this->getState().Gamepad.sThumbRX;
		float LY = this->getState().Gamepad.sThumbRY;

		//determine how far the controller is pushed
		float magnitude = sqrt(LX*LX + LY*LY);

		//determine the direction the controller is pushed
		float normalizedLX = LX / magnitude;
		float normalizedLY = LY / magnitude;

		float normalizedMagnitude = 0;

		/// Calculate Right Thumb dead zone
		if (this->IsConnected() && magnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			//clip the magnitude at its expected maximum value
			if (magnitude > 32767) magnitude = 32767;
  
			//adjust magnitude relative to the end of the dead zone
			magnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

			//optionally normalize the magnitude with respect to its expected range
			//giving a magnitude value of 0.0 to 1.0
			normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		}
		else //if the controller is in the deadzone zero out the magnitude
		{
			magnitude = 0.0;
			normalizedMagnitude = 0.0;
		}

		relativeRotation.Y -= -normalizedLX * normalizedMagnitude * RotateSpeed;
		relativeRotation.X -= normalizedLY * normalizedMagnitude * RotateSpeed * MouseYDirection;

		if (relativeRotation.X > MaxVerticalAngle*2 &&
			relativeRotation.X < 360.0f-MaxVerticalAngle)
		{
			relativeRotation.X = 360.0f-MaxVerticalAngle;
		}
		else
		if (relativeRotation.X > MaxVerticalAngle &&
			relativeRotation.X < 360.0f-MaxVerticalAngle)
		{
			relativeRotation.X = MaxVerticalAngle;
		}

		CursorControl->setPosition(0.5f + (0.5f * LX) / 32767, 0.5f + (0.5f * LY) / 32767);

		LX = this->getState().Gamepad.sThumbLX;
		LY = this->getState().Gamepad.sThumbLY;

		//determine how far the controller is pushed
		magnitude = sqrt(LX*LX + LY*LY);

		//determine the direction the controller is pushed
		normalizedLX = LX / magnitude;
		normalizedLY = LY / magnitude;

		normalizedMagnitude = 0;

		/// Calculate Left Thumb dead zone
		if (this->IsConnected() && magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			//clip the magnitude at its expected maximum value
			if (magnitude > 32767) magnitude = 32767;
  
			//adjust magnitude relative to the end of the dead zone
			magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

			//optionally normalize the magnitude with respect to its expected range
			//giving a magnitude value of 0.0 to 1.0
			normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		}
		else //if the controller is in the deadzone zero out the magnitude
		{
			magnitude = 0.0;
			normalizedMagnitude = 0.0;
		}

		video::IVideoDriver* driver = smgr->getVideoDriver();
		core::vector2d<u32> mousepos(u32(CursorControl->getPosition().X), u32(CursorControl->getPosition().Y));
		core::rect<u32> screenRect(0, 0, driver->getScreenSize().Width, driver->getScreenSize().Height);

		// Only if we are moving outside quickly.
		bool reset = !screenRect.isPointInside(mousepos);

		if(reset)
		{
			// Force a reset.
			CursorControl->setPosition(0.5f, 0.5f);
			CenterCursor = CursorControl->getRelativePosition();
			CursorPos = CenterCursor;
 		}

		// set target
		target.set(0,0, core::max_(1.f, pos.getLength()));
		core::vector3df movedir = target;

		core::matrix4 mat;
		mat.setRotationDegrees(core::vector3df(relativeRotation.X, relativeRotation.Y, 0));
		mat.transformVect(target);

		if (NoVerticalMovement)
		{
			mat.setRotationDegrees(core::vector3df(0, relativeRotation.Y, 0));
			mat.transformVect(movedir);
		}
		else
		{
			movedir = target;
		}

		movedir.normalize();

		pos += normalizedLY * normalizedMagnitude * movedir * timeDiff * MoveSpeed;

		// strafing
		core::vector3df strafevect = target;
		strafevect = strafevect.crossProduct(camera->getUpVector());

		if (NoVerticalMovement)
			strafevect.Y = 0.0f;

		strafevect.normalize();

		pos += -normalizedLX * normalizedMagnitude * strafevect * timeDiff * MoveSpeed;

		// For jumping, we find the collision response animator attached to our camera
		// and if it's not falling, we tell it to jump.
		/*if (CursorKeys[EKA_JUMP_UP])
		{
			const ISceneNodeAnimatorList& animators = camera->getAnimators();
			ISceneNodeAnimatorList::ConstIterator it = animators.begin();
			while(it != animators.end())
			{
				if(ESNAT_COLLISION_RESPONSE == (*it)->getType())
				{
					ISceneNodeAnimatorCollisionResponse * collisionResponse =
						static_cast<ISceneNodeAnimatorCollisionResponse *>(*it);

					if(!collisionResponse->isFalling())
						collisionResponse->jump(JumpSpeed);
				}

				it++;
			}
		}*/
	}

	// write translation
	camera->setPosition(pos);

	// write right target
	target += pos;
	camera->setTarget(target);
}

//! Sets the rotation speed
void CCameraAnimatorFPS360Controller::setRotateSpeed(f32 speed)
{
	RotateSpeed = speed;
}


//! Sets the movement speed
void CCameraAnimatorFPS360Controller::setMoveSpeed(f32 speed)
{
	MoveSpeed = speed;
}


//! Gets the rotation speed
f32 CCameraAnimatorFPS360Controller::getRotateSpeed() const
{
	return RotateSpeed;
}


// Gets the movement speed
f32 CCameraAnimatorFPS360Controller::getMoveSpeed() const
{
	return MoveSpeed;
}


//! Sets the keyboard mapping for this animator
void CCameraAnimatorFPS360Controller::setKeyMap(SKeyMap *map, u32 count)
{

}

void CCameraAnimatorFPS360Controller::setKeyMap(const core::array<SKeyMap>& keymap)
{

}

const core::array<SKeyMap>& CCameraAnimatorFPS360Controller::getKeyMap() const
{
	return 0;
}


//! Sets whether vertical movement should be allowed.
void CCameraAnimatorFPS360Controller::setVerticalMovement(bool allow)
{
	NoVerticalMovement = !allow;
}


//! Sets whether the Y axis of the mouse should be inverted.
void CCameraAnimatorFPS360Controller::setInvertMouse(bool invert)
{
	if (invert)
		MouseYDirection = -1.0f;
	else
		MouseYDirection = 1.0f;
}


ISceneNodeAnimator* CCameraAnimatorFPS360Controller::createClone(ISceneNode* node, ISceneManager* newManager)
{
	CCameraAnimatorFPS360Controller * newAnimator =
		new CCameraAnimatorFPS360Controller(CursorControl, RotateSpeed, MoveSpeed, JumpSpeed,
											0, 0, NoVerticalMovement);
	return newAnimator;
}

XINPUT_STATE CCameraAnimatorFPS360Controller::getState() {
	// Zeroise the state
    ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

    // Get the state
    XInputGetState(_controllerNum, &_controllerState);

    return _controllerState;
}

bool CCameraAnimatorFPS360Controller::IsConnected()
{
    // Zeroise the state
    ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

    // Get the state
    DWORD Result = XInputGetState(_controllerNum, &_controllerState);

    if(Result == ERROR_SUCCESS)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CCameraAnimatorFPS360Controller::Vibrate(u32 leftVal, u32 rightVal)
{
    // Create a Vibraton State
    XINPUT_VIBRATION Vibration;

    // Zeroise the Vibration
    ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

    // Set the Vibration Values
    Vibration.wLeftMotorSpeed = leftVal;
    Vibration.wRightMotorSpeed = rightVal;

    // Vibrate the controller
    XInputSetState(_controllerNum, &Vibration);
}


} // namespace scene
} // namespace irr

#endif
