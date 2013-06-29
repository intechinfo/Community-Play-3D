/*
 *  devices.h
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __C_COLLISION_MANAGER_H_INCLUDED__
#define __C_COLLISION_MANAGER_H_INCLUDED__

#include "stdafx.h"
#include "Core/CCore.h"

struct SFPSCameraSettings {

	SFPSCameraSettings() {
		ellipsoidRadius = core::vector3df(30,60,30);
		gravityPerSecond = core::vector3df(0,-10.0f,0);
		ellipsoidTranslation = core::vector3df(0,0,0);

		slidingValue = 0.0005f;

		anim = 0;
		cameraAnimator = 0;
	}

	//METHODS
	ISceneNodeAnimator *setSettings(ISceneManager *smgr, IMetaTriangleSelector *meta, ISceneNode *camera) {
		anim = smgr->createCollisionResponseAnimator(meta, camera, ellipsoidRadius, gravityPerSecond, ellipsoidTranslation, slidingValue);
		if (anim) {
			camera->addAnimator(anim);
		}
		return anim;
	}
	void unsetSettings(ISceneNode *camera) {
		if (anim) {
			camera->removeAnimator(anim);
			delete anim;
			anim = 0;
		}
	}

	//ELLIPSOID RADIUS
	void setEllipsoidRadius(vector3df _ellipsoidRadius) { ellipsoidRadius = _ellipsoidRadius; }
	vector3df getEllipsoidRadius() { return ellipsoidRadius; }

	//GRAVITY
	void setGravityPerSecond(vector3df _gravityPerSecond) { gravityPerSecond = _gravityPerSecond; }
	vector3df getGravityPerSecond() { return ellipsoidRadius; }

	//ELLIPSOID TRANSLATION
	void setEllipsoidTranslation(vector3df _ellipsoidTranslation) { ellipsoidTranslation = _ellipsoidTranslation; }
	vector3df getEllipsoidTranslation() { return ellipsoidTranslation; }

	//SLIDING VALUE
	void setSlidingValue(f32 _slidingValue) { slidingValue = _slidingValue; }
	f32 getSlidingValue() { return slidingValue; }

	//ANIMATORS
	ISceneNodeAnimator *getAnimator() { return anim; }
	ISceneNodeAnimatorCameraFPS *getCameraAnimator() { return cameraAnimator; }

private:

	//ANIMATORS
	ISceneNodeAnimator *anim;
	ISceneNodeAnimatorCameraFPS *cameraAnimator;

	//VALUES
	vector3df ellipsoidRadius;
	vector3df gravityPerSecond;
	vector3df ellipsoidTranslation;

	f32 slidingValue;
};

class SSWE_CORE_API CCollisionManager {
    
public:
	
	CCollisionManager(ISceneManager *_smgr);
	~CCollisionManager();
	
    //-----------------------------------
    //METHODS
    void setCollisionToAnOctTreeNode(ISceneNode *node);
    void setCollisionToAnAnimatedNode(ISceneNode *node);
    void setCollisionFromBoundingBox(ISceneNode *node);
    
	SFPSCameraSettings getFPSCameraSettings() { return fpsCameraSettings; }
    void createAnimatorCollisionCamera(ISceneNode *camera);
    
    IMetaTriangleSelector *getMetaTriangleSelectors() { return meta; }
    //-----------------------------------
	
private:
    
    //-----------------------------------
    //DEVICE PROPERTIES
    ISceneManager *smgr;

	SFPSCameraSettings fpsCameraSettings;
    //-----------------------------------
    
    //-----------------------------------
    //ATTRIBUTES
    IMetaTriangleSelector *meta;
    //-----------------------------------
    
};

#endif
