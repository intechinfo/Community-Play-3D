// Copyright (C) 2008-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_SCENE_NODE_ANIMATOR_LOOK_AT_H_INCLUDED__
#define __C_SCENE_NODE_ANIMATOR_LOOK_AT_H_INCLUDED__

#include <irrlicht.h>

namespace irr
{
namespace scene
{
	class CSceneNodeAnimatorLookAt : public ISceneNodeAnimator
	{
	public:

		//! constructor
		CSceneNodeAnimatorLookAt(ISceneNode* target);

		//! destructor
		~CSceneNodeAnimatorLookAt();

		//! animates a scene node
		virtual void animateNode(ISceneNode* node, u32 timeMs);

		// not yet implemented
		//virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const;
		//virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

		//! Returns type of the scene node animator
		virtual ESCENE_NODE_ANIMATOR_TYPE getType() const { return ESNAT_UNKNOWN; }

		//! Creates a clone of this animator.
		/** Please note that you will have to drop
		(IReferenceCounted::drop()) the returned pointer after calling this.*/
		virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager=0);

	private:
		// node to point to
		ISceneNode* Target;
	};


} // end namespace scene
} // end namespace irr

#endif
