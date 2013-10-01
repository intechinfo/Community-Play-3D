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

	CCameraRig(IrrlichtDevice *_device);
	~CCameraRig();

	////////////////////////////////////////////////////////////////////////////////
	// Event receiver with all the function attributed to the rig
	// MAJOR change will be required there, need to be able to set the keys
	////////////////////////////////////////////////////////////////////////////////
    bool OnEvent(const SEvent &event);

    irr::scene::ICameraSceneNode *getCameraSceneNode();

	////////////////////////////////////////////////////////////////////////////////
	// Simple TEXT rigstate function:
	// Parameter contain an string (wide) for the status of the rig
	////////////////////////////////////////////////////////////////////////////////      
    void rigstate(stringw status=L"still");
	////////////////////////////////////////////////////////////////////////////////
	// Tima based animation function
	// This will animate the rig and calculate the time
	// This "animate function" do the Rig assembly
	////////////////////////////////////////////////////////////////////////////////
	void OnAnimate(u32 TimeMS);
	////////////////////////////////////////////////////////////////////////////////
	// Get the mouse information X,Y to update the rotation of the node
	// The rotation calculated is applied to the cam TARGET
	////////////////////////////////////////////////////////////////////////////////
	void MouseRotate(scene::ICameraSceneNode *cam, u32 TimeMS);
	////////////////////////////////////////////////////////////////////////////////
	// Time based movements, using the keyboard 
	// Moves supported: Forward, backward, strafe: left & right, jump
	// Most camera rigs function are used in there 
	////////////////////////////////////////////////////////////////////////////////
	void KeysMove(scene::ISceneNode *footnode, scene::ICameraSceneNode *cam, 
                  bool smooth=false, float smoothvalue=0.1f, bool walksim=false,
                  bool novertmove=true);
	////////////////////////////////////////////////////////////////////////////////
	// Sine wave walk simulator, used by the KeysMove function (subfunction)
	// 
	////////////////////////////////////////////////////////////////////////////////
	vector3df walksimulator(bool smooth);
	////////////////////////////////////////////////////////////////////////////////
	// Crouch function
	// Use the "interpolate" function to animate the movement to the desired
	// position
	////////////////////////////////////////////////////////////////////////////////
	void doCrouch();
	////////////////////////////////////////////////////////////////////////////////
	// Timed action
	// This is used for the SPRINT feature
	////////////////////////////////////////////////////////////////////////////////
     void doTimedAction(u32 Timems, f32 limit);
	////////////////////////////////////////////////////////////////////////////////
	// Testing function
	// Check if the interpolation as reached the limit
	////////////////////////////////////////////////////////////////////////////////
	bool tiltTest(scene::ICameraSceneNode *cam, float desiredAngle=0) { 
		float currentAngle = RelativeRotation.Z;
		if (desiredAngle!=currentAngle)
			return false;
		else
			return true;
	}

////////////////////////////////////////////////////////////////////////////////
// Interpolate motion to animate the rig elements
// Time base animate. Will return the current info to be interpolated
// Was used to interpolate angles at first
// Now do much more (smooth use this)
////////////////////////////////////////////////////////////////////////////////     
	float Interpolate(float currentAngle, float desiredAngle=0.0f, float amount=0.5f);
          

	void createFPSrig();
////////////////////////////////////////////////////////////////////////////////
// Function to enable/disable certain rig functions
// 
////////////////////////////////////////////////////////////////////////////////      
    void changemode(bool wsm, bool smm, bool tlm, bool slm) {
		walksimmode=wsm; smoothmode=smm; tiltmode=tlm; slidemode=slm;
    }

private:

	vector3df target,targetvector; // the Camera Targets
	core::vector3d<f32> RelativeRotation; // this is the relative rotation based on the mouse movement
	float movespeed, strafespeed, jumpspeed; // the speed variable on the rig 
	int action, action1, action2, crouchstate;
	vector3df direction;
	u32 lastAnimationTime;
	f32 timeDiff, lastSprintTime, timeLimit;

	vector3df CamRot, CamPos;
	core::position2d<f32> cursor; // Gui cursor
      
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
};

class Gamecontroller {
public :
	Gamecontroller()
	// Initialise the FPS Keymap for a Qwerty keyboard
	// To add: Define 3 types of keyboard: Azerty, Qwerty, and Dvorak mappings
	// To do: This class is not used right now with the camera rig. 
	// Will need to update the camera rig to use it.
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
		//delete keyMap;
	}

private:
	  SKeyMap keyMap[9];
};

#endif