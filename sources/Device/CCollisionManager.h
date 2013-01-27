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

#include "Core/CCore.h"

class CCollisionManager {
    
public:
	
	CCollisionManager(ISceneManager *_smgr);
	~CCollisionManager();
	
    //-----------------------------------
    //METHODS
    void setCollisionToAnOctTreeNode(ISceneNode *node);
    void setCollisionToAnAnimatedNode(ISceneNode *node);
    
    void createAnimatorCollisionCamera(ISceneNode *camera);
    
    IMetaTriangleSelector *getMetaTriangleSelectors() { return meta; }
    //-----------------------------------
	
private:
    
    //-----------------------------------
    //DEVICE PROPERTIES
    ISceneManager *smgr;
    //-----------------------------------
    
    //-----------------------------------
    //ATTRIBUTES
    IMetaTriangleSelector *meta;
    ISceneNodeAnimator *animCollisions;
    //-----------------------------------
    
};

#endif
