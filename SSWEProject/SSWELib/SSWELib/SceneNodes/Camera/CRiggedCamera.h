// Multiple Camera controller functions
// To be included in constructing camera rigs

// current functions are based (and active) on the EventReceiver (To be separated so it support multiple camera rigs at the same time

#ifndef __C_RIGGED_CAMERA_H_INCLUDED__
#define __C_RIGGED_CAMERA_H_INCLUDED__

#include <irrlicht.h>

using namespace irr;
using namespace core;

class CCameraRig : public IEventReceiver {
      
public:

	CCameraRig(IrrlichtDevice *_device, SKeyMap _keyMap[9]);
	~CCameraRig();

    bool OnEvent(const SEvent &event);

    irr::scene::ICameraSceneNode *getCameraSceneNode();

    void rigstate(stringw status=L"still");

	stringw getRigState() { return rigAction; }

	void OnAnimate(u32 TimeMS);

	void MouseRotate(scene::ICameraSceneNode *cam, u32 TimeMS);

	void KeysMove(scene::ISceneNode *footnode, scene::ICameraSceneNode *cam, 
                  bool smooth=false, float smoothvalue=0.1f, bool walksim=false,
                  bool novertmove=true);

	vector3df walksimulator(bool smooth);

	void doCrouch();

     void doTimedAction(u32 Timems, f32 limit);

	bool tiltTest(scene::ICameraSceneNode *cam, float desiredAngle=0) { 
		float currentAngle = RelativeRotation.Z;
		if (desiredAngle!=currentAngle)
			return false;
		else
			return true;
	}
   
	float Interpolate(float currentAngle, float desiredAngle=0.0f, float amount=0.5f);
          

	void createFPSrig();

    void changemode(bool wsm, bool smm, bool tlm, bool slm) {
		walksimmode=wsm; smoothmode=smm; tiltmode=tlm; slidemode=slm;
    }

private:

	vector3df target,targetvector;
	core::vector3d<f32> RelativeRotation;
	float movespeed, strafespeed, jumpspeed;
	int action, action1, action2, crouchstate;
	vector3df direction;
	u32 lastAnimationTime;
	f32 timeDiff, lastSprintTime, timeLimit;

	vector3df CamRot, CamPos;
	core::position2d<f32> cursor;
      
	scene::ICameraSceneNode *camera; 
	scene::ISceneNode *feetnode;
	scene::ISceneNode *headnode;

	float feetcounter, desiredcrouch, currentspeed, currentstrafespeed, tiltAngle, tiltSpeed, oldspeed;
	bool firstupdate;
	bool slidefront,slideback,slideleft,slideright;
	bool runmode, crouchmode, sprintmode, exhausted, pressed;
	core::stringw rigAction, lastAction; 
	bool gamemode;
	bool smoothmode, tiltmode, walksimmode, slidemode;

	IrrlichtDevice *device;
	SKeyMap keyMap[9];
};

class Gamecontroller {
public :
	Gamecontroller()
	{
		keyMap[0].Action = EKA_MOVE_FORWARD; 
		keyMap[0].KeyCode = KEY_UP; 
		keyMap[1].Action = EKA_MOVE_FORWARD; 
		keyMap[1].KeyCode = KEY_KEY_W; 

		keyMap[2].Action = EKA_MOVE_BACKWARD; 
		keyMap[2].KeyCode = KEY_DOWN; 
		keyMap[3].Action = EKA_MOVE_BACKWARD; 
		keyMap[3].KeyCode = KEY_KEY_S; 

		keyMap[4].Action = EKA_STRAFE_LEFT; 
		keyMap[4].KeyCode = KEY_LEFT; 
		keyMap[5].Action = EKA_STRAFE_LEFT; 
		keyMap[5].KeyCode = KEY_KEY_A; 

		keyMap[6].Action = EKA_STRAFE_RIGHT; 
		keyMap[6].KeyCode = KEY_RIGHT; 
		keyMap[7].Action = EKA_STRAFE_RIGHT; 
		keyMap[7].KeyCode = KEY_KEY_D;  

		keyMap[8].Action = EKA_JUMP_UP; 
		keyMap[8].KeyCode = KEY_SPACE ;
    
	}
	~Gamecontroller() {

	}

private:
	  SKeyMap keyMap[9];
};

#endif