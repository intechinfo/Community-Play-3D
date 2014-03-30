//! This is just a copy of the CSceneNodeAnimatorCameraMaya.h
 
/** Original Copyright is this one !!!!!
//Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
**/
 
#ifndef __C_CHARACTER_CONTROL_H_INCLUDED__
#define __C_CHARACTER_CONTROL_H_INCLUDED__
 
#include <irrlicht.h>

#include "../Camera/CThirdCameraSceneNode.h"

class IDevices;

enum E_CHARACTER_ANIMATIONS {
	ECA_WALK = 0,
	ECA_RUN,
	ECA_IDLE,
	ECA_TURN,

	ECA_NEED_TO_CHANGE
};

class CCharacterControl : irr::IEventReceiver, public IUpdate {

public:

	CCharacterControl(irr::scene::IAnimatedMeshSceneNode *node,
					  irr::scene::ICameraSceneNode *camera,
					  IDevices *devices);
	~CCharacterControl();

	void update();

	bool OnEvent(const irr::SEvent &event);

private:

	void moveCameraControl();
	f32 direction;
	f32 zdirection;

	/// Datas
	irr::scene::IAnimatedMeshSceneNode *node;
	irr::scene::ICameraSceneNode *camera;
	irr::scene::IBoneSceneNode *boneNode;

	/// Devices
	IDevices *devices;

	/// Touches
	core::array<SKeyMap> KeyMap;
	bool CursorKeys[EKA_COUNT];
	bool shiftDown;

	irr::f32 currentRotationTransitionMovement;
	irr::u32 then;

	E_CHARACTER_ANIMATIONS currentAnimationType;
	E_CHARACTER_ANIMATIONS wantedAnimationType;
	bool isRotating;

	/// Methods
	void calculateAngleMovement(irr::f32 valueToReach);
	irr::f32 getAngleWith(irr::f32 angle, irr::f32 toAdd);

};

#endif