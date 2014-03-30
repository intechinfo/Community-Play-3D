// Copyright (C) 2008-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#include "stdafx.h"
#include "CCharacterLookAt.h"

namespace irr
{
namespace scene
{

//! constructor
CSceneNodeAnimatorLookAt::CSceneNodeAnimatorLookAt(ISceneNode* node) : Target(node)
{
	#ifdef _DEBUG
	setDebugName("CSceneNodeAnimatorLookAt");
	#endif
	if (Target)
		Target->grab();
}

//! destructor
CSceneNodeAnimatorLookAt::~CSceneNodeAnimatorLookAt()
{
	if (Target)
		Target->drop();
}

//! animates a scene node
void CSceneNodeAnimatorLookAt::animateNode(ISceneNode* node, u32 timeMs)
{
	//note: is the Target part of the scene?
	if(!Target || !node || !Target->getParent())
	{
		return;
	}

	//If this is a camera scene node it already understands targets.
	const scene::ESCENE_NODE_TYPE type = node->getType();

	if(type != ESNT_CAMERA || type != ESNT_CAMERA_FPS || type != ESNT_CAMERA_MAYA)
	{
		//note: copied from vector3d<T>::getHorizontalAngle()
		const core::vector3df toTarget(Target->getAbsolutePosition() - node->getAbsolutePosition());
		node->setRotation(toTarget.getHorizontalAngle());
		node->updateAbsolutePosition();
	}
	else //this is a camera, use setTarget
	{
		((scene::ICameraSceneNode*)node)->setTarget(Target->getAbsolutePosition());
	}
}


ISceneNodeAnimator* CSceneNodeAnimatorLookAt::createClone(ISceneNode* node, ISceneManager* newManager)
{
	return new CSceneNodeAnimatorLookAt(Target);
}

} // end namespace scene
} // end namespace irr