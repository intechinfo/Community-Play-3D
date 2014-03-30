//! This is just a copy of the CSceneNodeAnimatorCameraMaya.h
 
/** Original Copyright is this one !!!!!
//Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
**/

#include "stdafx.h"
 
#include "CThirdCameraSceneNode.h"
 
//#include "ICameraSceneNode.h"
//#include "ISceneManager.h"
 
using namespace irr;
 
//! constructor
CSceneNodeAnimatorCameraOrbital::CSceneNodeAnimatorCameraOrbital(
    ICursorControl *cursorControl, irr::scene::ISceneNode *node,
	core::vector3df center, f32 zoom, f32 rotateSpeed, f32 zoomSpeed,
    f32 minimumZoom, SKeyMap* keyMapArray, u32 keyMapSize)
    : OldCamera(0), Center(center), CurrentZoom(zoom), ZoomSpeed(zoomSpeed),
    RotateSpeed(rotateSpeed), RotX(0.0f), RotY(0.0f), MinimumZoom(minimumZoom), firstUpdate(true)
{
    #ifdef _DEBUG
    setDebugName("CSceneNodeAnimatorCameraOrbital");
    #endif
 
    allKeysUp();
 
    // create key map
    if (!keyMapArray || !keyMapSize)
    {
        // create default key map
        KeyMap.push_back(SKeyMap(EKA_MOVE_FORWARD, KEY_UP));
        KeyMap.push_back(SKeyMap(EKA_MOVE_BACKWARD, KEY_DOWN));
        KeyMap.push_back(SKeyMap(EKA_STRAFE_LEFT, KEY_LEFT));
        KeyMap.push_back(SKeyMap(EKA_STRAFE_RIGHT, KEY_RIGHT));
    }
    else
    {
        // create custom key map
        setKeyMap(keyMapArray, keyMapSize);
    }

	this->cursorControl = cursorControl;

	this->node = node;
 
//  inversion = 1.f;
}
 
//! destructor
CSceneNodeAnimatorCameraOrbital::~CSceneNodeAnimatorCameraOrbital(){}
 
//! It is possible to send mouse and key events to the camera. Most cameras
//! may ignore this input, but camera scene nodes which are created for
//! example with scene::ISceneManager::addMayaCameraSceneNode or
//! scene::ISceneManager::addMeshViewerCameraSceneNode, may want to get this input
//! for changing their position, look at target or whatever.
bool CSceneNodeAnimatorCameraOrbital::OnEvent(const SEvent& event)
{
    switch(event.EventType)
    {
    case EET_KEY_INPUT_EVENT:
        for (u32 i=0; i<KeyMap.size(); ++i)
        {
            if (KeyMap[i].KeyCode == event.KeyInput.Key)
            {
                CursorKeys[KeyMap[i].Action] = event.KeyInput.PressedDown;
                return true;
            }
        }
        break;
 
    case EET_MOUSE_INPUT_EVENT:
        if(event.MouseInput.Event == EMIE_MOUSE_WHEEL)
        {
            if (event.MouseInput.Wheel > 0 )
            {
                CurrentZoom -= ZoomSpeed;
                if(CurrentZoom < MinimumZoom)
                    CurrentZoom = MinimumZoom;
            }
            else
                CurrentZoom += ZoomSpeed;
 
            return true;
        }
		if (event.MouseInput.Event == EMIE_MOUSE_MOVED)
		{
			CursorPos = cursorControl->getRelativePosition();
			return true;
		}
        break;
 
    default:
        break;
    }
    return false;
}
 
//! OnAnimate() is called just before rendering the whole scene.
void CSceneNodeAnimatorCameraOrbital::animateNode(scene::ISceneNode *node, u32 timeMs)
{
    if (!node || node->getType() != scene::ESNT_CAMERA)
        return;
 
    scene::ICameraSceneNode* camera = static_cast<scene::ICameraSceneNode*>(node);

    // If the camera isn't the active camera, and receiving input, then don't process it.
    if (!camera->isInputReceiverEnabled()) {
		firstUpdate = true;
        return;
	}
 
    if (firstUpdate)
    {
        camera->updateAbsolutePosition();
		if (cursorControl)
		{
			cursorControl->setPosition(0.5f, 0.5f);
			CursorPos = CenterCursor = cursorControl->getRelativePosition();
		}
        lastAnimationTime = timeMs;
		allKeysUp();
		RotX = 180.f;
        firstUpdate = false;
    }

    scene::ISceneManager * smgr = camera->getSceneManager();
    if (smgr && smgr->getActiveCamera() != camera)
        return;
 
    // get time
    timeDiff = static_cast<f32>(timeMs - lastAnimationTime) * 0.1f;
    lastAnimationTime = timeMs;
 
    if (OldCamera != camera)
    {
        LastCameraTarget = OldTarget = camera->getTarget();
        OldCamera = camera;
    }
    else
    {
        OldTarget += camera->getTarget() - LastCameraTarget;
    }
 
    // set rot -------------------------------------
 
//    if(RotY > 90.f && RotY < 270.f)
//        inversion = -1.f;
//    else
//        inversion = 1.f;
 
    // adjust camera position based on current key input
    if (CursorKeys[EKA_STRAFE_LEFT])
        RotX += (RotateSpeed * timeDiff);// * inversion;

    if (CursorKeys[EKA_STRAFE_RIGHT])
        RotX -= (RotateSpeed * timeDiff);// * inversion;

    if (CursorKeys[EKA_MOVE_FORWARD])
        RotY += (RotateSpeed * timeDiff);

    if (CursorKeys[EKA_MOVE_BACKWARD])
        RotY -= (RotateSpeed * timeDiff);

	RotY -= (0.5f - CursorPos.Y) * RotateSpeed;
	RotX -= (0.5f - CursorPos.X) * RotateSpeed;

	// Do the fix as normal, special case below
	// reset cursor position to the centre of the window.
	cursorControl->setPosition(0.5f, 0.5f);
	CenterCursor = cursorControl->getRelativePosition();

	// needed to avoid problems when the event receiver is disabled
	CursorPos = CenterCursor;

	// Special case, mouse is whipped outside of window before it can update.
	video::IVideoDriver* driver = smgr->getVideoDriver();
	core::vector2d<u32> mousepos(u32(cursorControl->getPosition().X), u32(cursorControl->getPosition().Y));
	core::rect<u32> screenRect(0, 0, driver->getScreenSize().Width, driver->getScreenSize().Height);

	// Only if we are moving outside quickly.
	bool reset = !screenRect.isPointInside(mousepos);

	if(reset)
	{
		// Force a reset.
		cursorControl->setPosition(0.5f, 0.5f);
		CenterCursor = cursorControl->getRelativePosition();
		CursorPos = CenterCursor;
 	}

    modulo360(); //Not really necessary

    f32 nRotX = RotX;
    f32 nRotY = RotY;
    f32 nZoom = CurrentZoom;

    // Set pos -------------------------------------

    core::vector3df pos = OldTarget;
    pos.X += nZoom;

    pos.rotateXYBy(nRotY, OldTarget);
    pos.rotateXZBy(-nRotX, OldTarget);

    const core::vector3df correctionOfTheError = core::vector3df(0.f,0.f,-100.f); //I don't know why but the camera is originally set on (0.f, 0.f, 100.f)

	camera->setPosition(correctionOfTheError + Center + pos);
    camera->setTarget(correctionOfTheError + Center + OldTarget);

    // Rotation Error ----------------------------

    // jox: fixed bug: jitter when rotating to the top and bottom of y
    pos.set(0.f, 1.f, 0.f);
    pos.rotateXYBy(-nRotY);
    pos.rotateXZBy(-nRotX+180.f);
    camera->setUpVector(pos);
    LastCameraTarget = camera->getTarget();
}

//! Sets all keys up
void CSceneNodeAnimatorCameraOrbital::allKeysUp()
{
    for (u32 i=0; i<EKA_COUNT; ++i)
        CursorKeys[i] = false;
}
 
//! Sets the radius
void CSceneNodeAnimatorCameraOrbital::setZoom(f32 zoom)
{
    CurrentZoom = zoom;
}
 
//! Sets the rotation speed
void CSceneNodeAnimatorCameraOrbital::setRotateSpeed(f32 rotateSpeed)
{
    RotateSpeed = rotateSpeed;
}
 
//! Sets the zoom speed
void CSceneNodeAnimatorCameraOrbital::setZoomSpeed(f32 zoomSpeed)
{
    ZoomSpeed = zoomSpeed;
}
 
//! Sets the minimum zoom
void CSceneNodeAnimatorCameraOrbital::setMinimumZoom(f32 minimumZoom)
{
    MinimumZoom = minimumZoom;
}
 
//! Gets the radius
f32 CSceneNodeAnimatorCameraOrbital::getZoom() const
{
    return CurrentZoom;
}
 
//! Gets the rotation speed
f32 CSceneNodeAnimatorCameraOrbital::getRotateSpeed() const
{
    return RotateSpeed;
}
 
//! Gets the zoom speed
f32 CSceneNodeAnimatorCameraOrbital::getZoomSpeed() const
{
    return ZoomSpeed;
}
 
//! Gets the minimum zoom
f32 CSceneNodeAnimatorCameraOrbital::getMinimumZoom() const
{
    return MinimumZoom;
}
 
//! Sets the keyboard mapping for this animator
void CSceneNodeAnimatorCameraOrbital::setKeyMap(SKeyMap *map, u32 count)
{
    // clear the keymap
    KeyMap.clear();
 
    // add actions
    for (u32 i=0; i<count; ++i)
    {
        KeyMap.push_back(map[i]);
    }
}
 
//! Sets the keyboard mapping for this animator
void CSceneNodeAnimatorCameraOrbital::setKeyMap(const core::array<SKeyMap>& keymap)
{
    KeyMap = keymap;
}
 
//! Gets the keyboard mapping for this animator
const core::array<SKeyMap>& CSceneNodeAnimatorCameraOrbital::getKeyMap() const
{
    return KeyMap;
}
 
void CSceneNodeAnimatorCameraOrbital::modulo360()
{
    if(RotX < 0.f)
        RotX = 360.f;
    if(RotX > 360.f)
        RotX = 0.f;
    if(RotY < 0.f)
        RotY = 360.f;
    if(RotY > 360.f)
        RotY = 0.f;
}
 
scene::ISceneNodeAnimator* CSceneNodeAnimatorCameraOrbital::createClone(scene::ISceneNode* node, scene::ISceneManager* newManager)
{
    CSceneNodeAnimatorCameraOrbital * newAnimator =
        new CSceneNodeAnimatorCameraOrbital(cursorControl, node, Center, CurrentZoom, RotateSpeed, ZoomSpeed, MinimumZoom, 0, 0);
    newAnimator->setKeyMap(KeyMap);
    return newAnimator;
}
