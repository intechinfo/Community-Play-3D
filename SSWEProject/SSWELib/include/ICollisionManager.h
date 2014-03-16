#ifndef __I_COLLISION_MANAGER_H_INCLUDED__
#define __I_COLLISION_MANAGER_H_INCLUDED__

#include <irrlicht.h>

class IFPSCameraSettings {
public:
	//METHODS
	virtual irr::scene::ISceneNodeAnimator *setSettings(irr::scene::ISceneManager *smgr, irr::scene::IMetaTriangleSelector *meta, irr::scene::ISceneNode *camera) = 0;
	virtual void unsetSettings(irr::scene::ISceneNode *camera) = 0;
    
	//ELLIPSOID RADIUS
	virtual void setEllipsoidRadius(irr::core::vector3df _ellipsoidRadius) = 0;
	virtual irr::core::vector3df getEllipsoidRadius() = 0;
    
	//GRAVITY
	virtual void setGravityPerSecond(irr::core::vector3df _gravityPerSecond) = 0;
	virtual irr::core::vector3df getGravityPerSecond() = 0;
    
	//ELLIPSOID TRANSLATION
	virtual void setEllipsoidTranslation(irr::core::vector3df _ellipsoidTranslation) = 0;
	virtual irr::core::vector3df getEllipsoidTranslation() = 0;
    
	//SLIDING VALUE
	virtual void setSlidingValue(irr::f32 _slidingValue) = 0;
	virtual irr::f32 getSlidingValue() = 0;
    
	//ANIMATORS
	virtual irr::scene::ISceneNodeAnimator *getAnimator() = 0;
    
	virtual irr::scene::ISceneNodeAnimatorCameraFPS *getCameraAnimator() = 0;
	virtual void setSceneNodeAnimatorCameraFPS(irr::scene::ISceneNodeAnimatorCameraFPS *_cameraAnimator) = 0;
};

class ICollisionManager
{
public: 
	virtual void reset() = 0;
	
    //-----------------------------------
    //METHODS
	virtual void setCollisionToAnUnknownNode(irr::scene::IMesh *mesh, irr::scene::ISceneNode *node) = 0;
    virtual irr::scene::ITriangleSelector *setCollisionToAnOctTreeNode(irr::scene::ISceneNode *node) = 0;
    virtual void setCollisionToAnAnimatedNode(irr::scene::ISceneNode *node) = 0;
    virtual void setCollisionFromBoundingBox(irr::scene::ISceneNode *node) = 0;
    
	virtual IFPSCameraSettings *getFPSCameraSettings() = 0;
    virtual void createAnimatorCollisionCamera(irr::scene::ISceneNode *camera) = 0;
    
    virtual irr::scene::IMetaTriangleSelector *getMetaTriangleSelectors() = 0;
    //-----------------------------------
};

#endif