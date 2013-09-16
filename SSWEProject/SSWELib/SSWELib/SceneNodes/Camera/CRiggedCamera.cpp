#include "stdafx.h"
#include "CRiggedCamera.h"

CCameraRig::CCameraRig(IrrlichtDevice *_device) {
	device = _device;
	firstupdate=true;
	rigAction=(L"still");
	createFPSrig();
	camera = 0;
}

CCameraRig::~CCameraRig() {

}

void CCameraRig::rigstate(stringw status) {
	rigAction=status;
}

scene::ICameraSceneNode *CCameraRig::getCameraSceneNode() {
	return this->camera;
}

void CCameraRig::OnAnimate(u32 TimeMS) {
	// function to animate and update the rig and camera
	if (!gamemode) {
		//if (!tiltTest(camera,tiltAngle)) 
		timeDiff = (f32) (TimeMS - lastAnimationTime); // Find the difference in time between each call.
		lastAnimationTime=TimeMS;
		//printf("The time diff is: %f\n",timeDiff);
		MouseRotate (camera,TimeMS);
		KeysMove(camera,camera,smoothmode,0,walksimmode,false);
		doCrouch(); doTimedAction(TimeMS,timeLimit);
		if ((currentspeed==0) && (action2==0))
			rigstate("still");
		// If player jumped over the level
		if (camera->getPosition().Y < -700) {
			camera->setPosition(CamPos+vector3df(0,3250,0)); 
			camera->setTarget(vector3df(0,0,0)); 
			RelativeRotation.X=30.0f;RelativeRotation.Y=0.0f;
			camera->updateAbsolutePosition();
			// TODO: Make the below code work to update the other animators
			// Need to update animators there. 
			//core::list<ISceneNodeAnimator*>::ConstIterator ait = camera->getAnimators().begin();
			//for (; ait != camera->getAnimators().end(); ++ait)
			//(*ait)->animateNode(camera,TimeMS);   
		}
		camera->updateAbsolutePosition();
    } else {

	} 
}

void CCameraRig::MouseRotate(scene::ICameraSceneNode *cam, u32 TimeMS) {
	// Animate the camera using the information coming from the inputs
	// (Mouse/Keyboard input at the moment
	//
	gui::ICursorControl* cursorcontrol;
	core::position2d<f32> centercursor;
	//
	cursorcontrol=device->getCursorControl(); // Capture the mouse cursor
	core::position2d<f32> cursorpos;
	// Move the position
	core::vector3df pos = cam->getPosition();
	direction = vector3df(0,0,1000); // First direction vector 1000 unit in front of the local player axis
	if (firstupdate) // First time use of this setup- Init the camera rig
	{
		cursorcontrol->setPosition(0.5f,0.5f);
		centercursor=cursorcontrol->getRelativePosition();
		RelativeRotation.X=10;
		RelativeRotation.Y=180;
		firstupdate=false;
		currentspeed=0.0f; desiredcrouch=0; 
		currentstrafespeed=0.0f; tiltAngle=0; tiltSpeed=0;
		// Setup the default speed
		movespeed=5.0f; strafespeed=3.0f; jumpspeed=5.0f; oldspeed=movespeed;
		slidefront=false;
		slideback=false;
		slideleft=false;
		slideright=false;
		action=0;
		action1=0;
		action2=0;
		lastAnimationTime=TimeMS;
		rigAction=L"still"; lastAction=rigAction; pressed=false;
		runmode=false; crouchmode=false; crouchstate=0; sprintmode=false; exhausted=false; timeLimit=0;
		//RelativeRotation.X=-45;
	}
	cursorpos = cursorcontrol->getRelativePosition(); // Get the mouse position
	core::matrix4 mat,mat1; // Define the matrix to be used for finding directional vectors
	if (!core::equals(cursorpos.X, centercursor.X) || !core::equals(cursorpos.Y, centercursor.Y)) {
		RelativeRotation.X *= -1.0f;
		RelativeRotation.Y *= -1.0f;
		RelativeRotation.X = core::clamp(RelativeRotation.X + (0.5f - cursorpos.Y) * 100.0f,-88.0f,+88.0f);
		RelativeRotation.Y += (0.5f - cursorpos.X) * 100;
		RelativeRotation.X *= -1.0f;
		RelativeRotation.Y *= -1.0f;
		cursorcontrol->setPosition(0.5f,0.5f);
		centercursor=cursorcontrol->getRelativePosition();
		//printf("Relative rotation=%f\n",camera->getRotation().Y);
		camera->setRotation(vector3df(RelativeRotation.X,RelativeRotation.Y,RelativeRotation.Z));
		if (RelativeRotation.Y > 359.0f) RelativeRotation.Y -= 360.0f;
		//if (RelativeRotation.Y < -359.0f) RelativeRotation.Y += 360.0f;
		if (RelativeRotation.Y < -0.0f) RelativeRotation.Y += 360.0f;
		// input the angle of rotation into the transformation matrix
		// TODO - Try to calculate the upvector of the cam to find the new base rotation then add the relative one to it.
		mat.setRotationDegrees(core::vector3df(RelativeRotation.X,RelativeRotation.Y,0));
		mat.transformVect(direction); // Find the direction vector from the matrix
		// Rotate the UPVector of the camera for the Z local axis (this now work)
		vector3df upv = vector3df(0,1,0);
		RelativeRotation.Z=Interpolate(RelativeRotation.Z,tiltAngle,tiltSpeed); // tiltValue
		//RelativeRotation.Z=tiltAngle;
		upv.rotateXYBy(RelativeRotation.Z,vector3df(0,0,0)); // Local Axis rotation
		upv.rotateXZBy(-RelativeRotation.Y,vector3df(0,0,0)); // Current camera rotation Y
		upv.rotateXZBy(-RelativeRotation.X,vector3df(0,0,0)); // Current camera rotation X
		cam->setUpVector(upv); //setup the UPvector
	}
	cam->setTarget(pos+direction); // Set the new target
	cam->updateAbsolutePosition();  
}

void CCameraRig::KeysMove(scene::ISceneNode *footnode, scene::ICameraSceneNode *cam, 
                  bool smooth, float smoothvalue, bool walksim,
                  bool novertmove)
	// Moving part of the code. Theses codes below for the node / camera moving
	// To do: Separate, then apply some parameters (moving node, speed)
	// Also: do the NOvertical movement
	{
		core::vector3df pos = footnode->getPosition();
		//core::vector3df movedir = direction; // Get the Front/back -- Dependant from the rotate function
		core::vector3df movedir = cam->getTarget()-cam->getPosition(); // Get the Front/back -- Dependant of the camera target.
		if (novertmove) movedir.Y=0.0f;
		movedir.normalize();
		if (smoothvalue==0) smoothvalue=0.1f; //default smoothvalue if 0 is entered
		//if (action) printf("movespeed=%f",movespeed);
		float desiredspeed = movespeed;
		float smoothfac = 0;
		if (action==1 || (slidefront==true && smooth)) {
			if (smooth) {
				if (slidefront) // Released the keyboard, objective is to get to 0
				{
					desiredspeed=0;
					smoothfac=smoothvalue*3;
				} else {
					smoothfac=smoothvalue;
				}
				currentspeed=Interpolate(currentspeed,desiredspeed,smoothfac);
				if (currentspeed==0)
					slidefront=false;
				pos += movedir * ((timeDiff * currentspeed)/15);
			} else {
				pos+= (movedir * ((timeDiff * movespeed)/15));
			}
		}// Move Forward

		if (action==2 || (slideback==true && smooth)) {
			if (smooth) {
				if (slideback) // Released the keyboard, objective is to get to 0
				{   desiredspeed=0; 
						smoothfac=smoothvalue*3;
					} else {
						smoothfac=smoothvalue;
				}
                 currentspeed=Interpolate(currentspeed,-desiredspeed,smoothfac);
                 if (currentspeed==0) slideback=false;
                 pos -= movedir * ((timeDiff * (-currentspeed))/15);
			} else {
				pos -= (movedir * ((timeDiff * movespeed)/15));
			}
		}
		// Walk simulation
		feetnode->setPosition(vector3df(0,0,0));
		if (walksim) {feetnode->setPosition(walksimulator(smooth));}
		// End walk sim
		core::vector3df strafevect = direction;
		strafevect = strafevect.crossProduct(vector3df(0,1,0)); // Get the strafe Vector
		if (novertmove) strafevect.Y=0.0f; // no vertical move implementation
		strafevect.normalize();
		if (action1==3 || (slideleft==true && smooth))  {
			if (smooth) {
				if (slideleft) {
					desiredspeed=0;
					smoothfac=smoothvalue*3;
				} else {
					smoothfac=smoothvalue;
				}
				currentstrafespeed=Interpolate(currentstrafespeed,-desiredspeed,smoothfac);
				if (currentstrafespeed==0) slideleft=false;
				pos -= strafevect * ((timeDiff * (-currentstrafespeed))/15);
			} else {
				pos -= (strafevect * ((timeDiff * strafespeed)/15));
			}
		} // Strafe left
		if (action1==4 || (slideright==true && smooth)) {
			if (smooth) {
				if (slideright) {
					desiredspeed=0; 
					smoothfac=smoothvalue*3;
				} else {
					smoothfac=smoothvalue;
				}
				currentstrafespeed=Interpolate(currentstrafespeed,desiredspeed,smoothfac);
				if (currentstrafespeed==0) slideright=false;
				pos += strafevect * ((timeDiff * currentstrafespeed) / 15);
			} else {
				pos += (strafevect * ((timeDiff * strafespeed)/15));
			}
		} // Strafe Right
		if (action2==5) pos += (vector3df(0,1,0) * ((timeDiff * jumpspeed) / 15)); // Jump
		footnode->setPosition(pos); // Set the camera Position
		cam->setTarget(cam->getPosition()+direction); // Set the new target  
		//feetnode->setRotation(vector3df(RelativeRotation.X,RelativeRotation.Y,0));
		cam->updateAbsolutePosition(); 
}

vector3df CCameraRig::walksimulator(bool smooth) {
	//recalculate for simulating walk
	if (feetcounter>360)
		feetcounter=0;

	float current=0;
        
	if (smooth){
		if (action==1 || action==2) current=currentspeed;  //smoothed
		if (action1==3 || action1==4)  current=currentstrafespeed;// strafing
		if (current<0) current=-current; 
		feetcounter += ((current*timeDiff)/15)*3.5f;
	} else {
		if (action==1 || action==2) feetcounter += ((movespeed * timeDiff)/15)*3.5f; // non smooth
		if (action1==3 || action1==4) feetcounter += ((strafespeed*timeDiff)/15)*3.5f; //strafing;
	}
	//  Base the sin position on the "feetcounter"
	float piover180 = 0.0174532925f;
	float upmove=sin(feetcounter * piover180);
	vector3df walkrotation = vector3df(upmove,0,0);
	walkrotation.rotateXZBy(RelativeRotation.Y,vector3df(0,0,0));
	if (action==1 || action==2 || action1==3 || action1==4) 
	return vector3df(0,(upmove*((currentspeed+strafespeed))/2),0);
}

void CCameraRig::doCrouch() {
	float currentcrouch=headnode->getPosition().Y;
	if (currentcrouch<1.0f) {
		currentcrouch=Interpolate(currentcrouch, desiredcrouch, 2.5f);
		headnode->setPosition(vector3df(headnode->getPosition().Z,currentcrouch,headnode->getPosition().Z));
	} 
}

void CCameraRig::doTimedAction(u32 Timems, f32 limit) {
    bool decide=true;
    if ((sprintmode && (currentspeed>0)) || (!sprintmode && exhausted)) {
		f32 elapsed = Timems-lastSprintTime;
        //printf("elapsed time: %f \n",elapsed);
        if (elapsed<limit && !exhausted) { rigstate("Sprint"); }
        if (elapsed>limit && !exhausted) { rigstate("Sprint Exausted"); movespeed=oldspeed; exhausted=true; elapsed=0; lastSprintTime=Timems;} 
        if (elapsed>limit && exhausted) { exhausted=false; rigstate("Can Sprint now");}
        //{sprintmode=false; if (exhausted) exhausted=false; else exhausted=true; movespeed=oldspeed;}
    }
}

float CCameraRig::Interpolate(float currentAngle, float desiredAngle, float amount) {
	// This function will allow to set the angle we need on the Z axis of the camera for tilt action
	if (amount==0) amount=0.5f;
	if (desiredAngle>currentAngle)  {
		currentAngle+=(amount*timeDiff)/15;
		if (currentAngle>=desiredAngle)
			return desiredAngle;
		else
			return currentAngle;
		//printf ("Current: %f, Desired: %f\n",currentAngle,desiredAngle);
	} else if (desiredAngle<currentAngle)  {
		currentAngle-=(amount*timeDiff)/15; 
		if (currentAngle<=desiredAngle)
			return desiredAngle;
		else 
			return currentAngle;
		//printf ("Current: %f, Desired: %f\n",currentAngle,desiredAngle);
	}
	//printf ("Current: %f, Desired: %f\n",currentAngle,desiredAngle); 
	return currentAngle;
}

void CCameraRig::createFPSrig() {
	// Creation of the player RIG here
	// NOTE: This will have to be created in the RIG class (some part of it)
	//
	// First pivot for feet for this rig 
	scene::ISceneNodeAnimatorCollisionResponse* anim;
	feetnode = device->getSceneManager()->addEmptySceneNode();
	//Second pivot point where the camera is attached
	headnode = device->getSceneManager()->addEmptySceneNode(feetnode);
	//printf ("Create a new camera for the scene\n");
	camera = device->getSceneManager()->addCameraSceneNode(headnode); // Basic camera to be used for the new camera rig.
	// game.camera = smgr->addCameraSceneNodeFPS(0, 120.0f, 220.0f, -1,MyControl.keyMap, 9, true,0.25f); 
	camera->setFarValue(4500.0); // Define the limit of the view (will render less poly)

	camera->setAspectRatio(1.3);  // 1.6 for 16/10 display, enter 1.3 for 4/3 display or windowed
	camera->setFOV(45 * irr::core::DEGTORAD); // Set a standard FOV
	camera->setAspectRatio(1.f * device->getVideoDriver()->getScreenSize().Width / device->getVideoDriver()->getScreenSize().Height);
	smoothmode=true;
	tiltmode=true;
	walksimmode=true;
	slidemode=true;
	gamemode=false;
}

bool CCameraRig::OnEvent(const SEvent& event) { 
	//data data;
	//float movespeed, strafespeed, jumpspeed; // tge speed variable on the rig     
	// Keyboard, mouse handling   
	// To do: Use the SKeymap class and their action (add new actions also)
	//        Use a parameter instead of a full class and take a paramenter for EventReceiver.
	//        Put back the Event receiver in place only for other game function (ESC->quit, TAB->menu etc)
          
	if(event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_SPACE) { 
		if (event.KeyInput.PressedDown) {action2=5;rigstate(L"Jumping");} else {action2=0;}
		//printf ("Jump! \n");
		return true;
	}
	if(event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_KEY_W) {
		if (event.KeyInput.PressedDown) {action=1; rigstate(L"moving forward");} else {action=0;if (slidemode) slidefront=true; else slidefront=false;}
		return true;  
	} 
	if(event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_KEY_S) { 
		if (event.KeyInput.PressedDown) {action=2; rigstate(L"moving backward");} else {action=0;if (slidemode) slideback=true; else slideback=false;}
		return true;
	} 
	if(event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_KEY_D) {
        if (event.KeyInput.PressedDown) {
			action1=3;
			if (tiltmode) {
				tiltAngle=-4.5f*(strafespeed/3); tiltSpeed=0.2f;
			}
			rigstate(L"strafe left");
        } else {
			action1=0; tiltAngle=0;
			if (slidemode)
				slideleft=true; 
			else
				slideleft=false;
		}
        return true;
    } 
	if(event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_KEY_A) {      
		if (event.KeyInput.PressedDown) {
			action1=4;
			if (tiltmode) {
				tiltAngle=4.5f*(strafespeed/3);tiltSpeed=0.2f;
			}
			rigstate(L"strafe right");
		} else {
			action1=0; tiltAngle=0;
			if (slidemode)
				slideright=true;
			else
				slideright=false;
		}
		return true;
	}
	if(event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_KEY_Q) {      
		if (event.KeyInput.PressedDown) {
			tiltAngle=35;  tiltSpeed=3; rigstate(L"tilt left");}
		else {
			tiltAngle=0;
		} 
		return true;
	}
	if(event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_KEY_E) {
		if (event.KeyInput.PressedDown) {
			tiltAngle=-35; tiltSpeed=3; rigstate(L"tilt right");
		}
		else {
			tiltAngle=0;
		} 
		return true;
	}
	if(event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_KEY_C) {     
		if (event.KeyInput.PressedDown) {
			if (crouchstate==0) { desiredcrouch=-30.0f; movespeed=0.5f; strafespeed=0.5f; jumpspeed=3.0f; rigstate(L"Knee"); }
			if (crouchstate==1) { desiredcrouch=-50.0f; movespeed=0.5f; strafespeed=0.5f; jumpspeed=1.5f; rigstate(L"Crouch"); }
			if (crouchstate==2) { desiredcrouch=0.0f; movespeed=5.0f; strafespeed=3.0f; jumpspeed=5.0f;rigstate("Back up"); }
                    
		} else {
			crouchstate+=1; if (crouchstate>2) crouchstate=0;
		}
			return true;
	}
	if(event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_CONTROL) {      
		if (event.KeyInput.PressedDown) {
			if (!crouchmode || crouchstate==2) {
				desiredcrouch=-30.0f;
				crouchstate=1;
				crouchmode=true;
				movespeed=0.5f;
				strafespeed=0.5f;
				jumpspeed=3.0f;
				rigstate(L"Knee");
			} else {
				desiredcrouch=0.0f;
				crouchstate=0;
				crouchmode=false;
				movespeed=5.0f;
				strafespeed=3.0f;
				jumpspeed=5.0;
				rigstate("Back up");
			}
		} 
		return true;
	}
	if(event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_SHIFT ) {      
		if (event.KeyInput.PressedDown) {
			// This will increase the speed or make it back to default.
			// To do: input theses as parameters.
			if (!sprintmode && !exhausted) {oldspeed=movespeed; movespeed*=2; timeLimit=2000.0f; sprintmode=true; lastSprintTime=device->getTimer()->getRealTime();}
			if (!sprintmode && exhausted) {timeLimit=9000.0f; sprintmode=true; lastSprintTime=device->getTimer()->getRealTime();}
			if (sprintmode && exhausted) {movespeed=oldspeed;}
		} else {
			sprintmode=false; movespeed=oldspeed;
		}
		return true; 
	} 
	if(event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_CAPITAL ) {      
		if (event.KeyInput.PressedDown) 
		// This will increase the speed or make it back to default.
		// To do: input theses as parameters.
		if (runmode) {rigstate("Run mode");movespeed=5.0f; strafespeed=3.0f; jumpspeed=5.0; runmode=false;} else {rigstate("Walk mode");movespeed=2.0f; strafespeed=1.0f; jumpspeed=3.0f; runmode=true;}
		return true;
	}

	return false; 
}