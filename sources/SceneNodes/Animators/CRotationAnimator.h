//
//  CAction.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 07/11/12.
//
//

#ifndef __C_ROTATION_ANIMATOR_H_INCLUDED__
#define __C_ROTATION_ANIMATOR_H_INCLUDED__

#include "ISceneNodeAnimatorFinishing.h"

namespace irr { //RESPECTING NAMESPACES
namespace scene { //RESPECTING NAMESPACES

class CRotationAnimator : public ISceneNodeAnimatorFinishing {

public:

	CRotationAnimator(const core::vector3df _startRotation, const core::vector3df &_endRotation, 
					  u32 _timeToComplete, bool _positive, u32 _startTime);
	
	virtual void animateNode(ISceneNode* node, u32 timeMs);

	virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const;

	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

	virtual ESCENE_NODE_ANIMATOR_TYPE getType() const { return ESNAT_ROTATION; }

	virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager=0);

private:

	core::vector3df startRotation;
	core::vector3df endRotation;
	core::vector3df difference;

	u32 timeToComplete;
	u32 startTime;

	bool positive;

};

} //END NAMESPACE IRR
} //END NAMESPACE SCENE

#endif
