//! This is just a copy of the CSceneNodeAnimatorCameraMaya.h
 
/** Original Copyright is this one !!!!!
//Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
**/
 
#ifndef __C_SCENE_NODE_ANIMATOR_CAMERA_ORBITAL_H_INCLUDED__
#define __C_SCENE_NODE_ANIMATOR_CAMERA_ORBITAL_H_INCLUDED__
 
#include <irrlicht.h>
 
//#include "ICameraSceneNode.h"
//#include "SKeyMap.h"
//#include "irrArray.h"
 
namespace irr
{
 
//! Special scene node animator for Orbital cameras
class CSceneNodeAnimatorCameraOrbital : public scene::ISceneNodeAnimator
{
public:

    //! Constructor
    CSceneNodeAnimatorCameraOrbital(ICursorControl *cursorControl, irr::scene::ISceneNode *node = 0,
		core::vector3df center = core::vector3df(0.f,0.f,0.f), f32 zoom = 10.f,
        f32 rotateSpeed = 1.f, f32 zoomSpeed = 1.f, f32 minimumZoom = 1.f, SKeyMap* keyMapArray = 0, u32 keyMapSize = 0);
 
    //! Destructor
    virtual ~CSceneNodeAnimatorCameraOrbital();
 
    //! Animates the scene node, currently only works on cameras
    virtual void animateNode(scene::ISceneNode* node, u32 timeMs);

    //! Event receiver
    virtual bool OnEvent(const SEvent& event);
 
    //! Set the radius
    virtual void setZoom(f32 zoom);

	//! Set the center
	virtual void setCenter(core::vector3df center) { Center = center; }

    //! Set the rotation speed
    virtual void setRotateSpeed(f32 rotateSpeed);
 
    //! Set the zoom speed
    virtual void setZoomSpeed(f32 zoomSpeed);
 
    //! Set the minimum zoom
    virtual void setMinimumZoom(f32 minimumZoom);
 
    //! Returns the radius
    virtual f32 getZoom() const;
 
    //! Returns the rotation speed
    virtual f32 getRotateSpeed() const;
 
    //! Returns the zoom speed
    virtual f32 getZoomSpeed() const;
 
    //! Returns the minimum speed
    virtual f32 getMinimumZoom() const;
 
    //! Sets the keyboard mapping for this animator (old style)
    //! \param keymap: an array of keyboard mappings, see SKeyMap
    //! \param count: the size of the keyboard map array
    virtual void setKeyMap(SKeyMap *map, u32 count);
 
    //! Sets the keyboard mapping for this animator
    //! \param keymap The new keymap array
    virtual void setKeyMap(const core::array<SKeyMap>& keymap);
 
    //! Gets the keyboard mapping for this animator
    virtual const core::array<SKeyMap>& getKeyMap() const;
 
    //! This animator will receive events when attached to the active camera
    virtual bool isEventReceiverEnabled() const
    {
        return true;
    }
 
    //! Returns type of the scene node
    virtual scene::ESCENE_NODE_ANIMATOR_TYPE getType() const
    {
        return scene::ESNAT_CAMERA_MAYA;
    }
 
    //! Creates a clone of this animator.
    /** Please note that you will have to drop
    (IReferenceCounted::drop()) the returned pointer after calling
    this. */
    virtual ISceneNodeAnimator* createClone(scene::ISceneNode* node, scene::ISceneManager* newManager=0);
 
private:
 
    void allKeysUp();
    void modulo360();
    bool isMouseKeyDown(s32 key) const;
 
    core::array<SKeyMap> KeyMap;          // Array KeyMap
    bool CursorKeys[EKA_COUNT];

    scene::ICameraSceneNode* OldCamera;
    core::vector3df Center;
    core::vector3df OldTarget;
    core::vector3df LastCameraTarget;   // to find out if the camera target was moved outside this animator
    f32 CurrentZoom;
    f32 ZoomSpeed;
    f32 RotateSpeed;
    f32 TranslateSpeed;
    f32 RotX, RotY;
    f32 MinimumZoom;
    f32 timeDiff;
    f32 inversion;
    u32 lastAnimationTime;
    bool firstUpdate;

	core::position2d<f32> CenterCursor, CursorPos;

	irr::scene::ISceneNode *node;

	ICursorControl *cursorControl;
};
 
} // end namespace irr
 
#endif // __C_SCENE_NODE_ANIMATOR_CAMERA_ORBITAL_H_INCLUDED__