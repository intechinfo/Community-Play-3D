#ifndef __I_COLLISION_MANAGER_H_INCLUDED__
#define __I_COLLISION_MANAGER_H_INCLUDED__

#include <irrlicht.h>

class IFPSCameraSettings {
public:
	//METHODS
	virtual ISceneNodeAnimator *setSettings(ISceneManager *smgr, IMetaTriangleSelector *meta, ISceneNode *camera) = 0;
	virtual void unsetSettings(ISceneNode *camera) = 0;
    
	//ELLIPSOID RADIUS
	virtual void setEllipsoidRadius(vector3df _ellipsoidRadius) = 0;
	virtual vector3df getEllipsoidRadius() = 0;
    
	//GRAVITY
	virtual void setGravityPerSecond(vector3df _gravityPerSecond) = 0;
	virtual vector3df getGravityPerSecond() = 0;
    
	//ELLIPSOID TRANSLATION
	virtual void setEllipsoidTranslation(vector3df _ellipsoidTranslation) = 0;
	virtual vector3df getEllipsoidTranslation() = 0;
    
	//SLIDING VALUE
	virtual void setSlidingValue(f32 _slidingValue) = 0;
	virtual f32 getSlidingValue() = 0;
    
	//ANIMATORS
	virtual ISceneNodeAnimator *getAnimator() = 0;
    
	virtual ISceneNodeAnimatorCameraFPS *getCameraAnimator() = 0;
	virtual void setSceneNodeAnimatorCameraFPS(ISceneNodeAnimatorCameraFPS *_cameraAnimator) = 0;
};

class ICollisionManager
{
public: 
	virtual void reset() = 0;
	
    //-----------------------------------
    //METHODS
	virtual void setCollisionToAnUnknownNode(IMesh *mesh, ISceneNode *node) = 0;
    virtual ITriangleSelector *setCollisionToAnOctTreeNode(ISceneNode *node) = 0;
    virtual void setCollisionToAnAnimatedNode(ISceneNode *node) = 0;
    virtual void setCollisionFromBoundingBox(ISceneNode *node) = 0;
    
	virtual IFPSCameraSettings *getFPSCameraSettings() = 0;
    virtual void createAnimatorCollisionCamera(ISceneNode *camera) = 0;
    
    virtual IMetaTriangleSelector *getMetaTriangleSelectors() = 0;
    //-----------------------------------
};

#endif