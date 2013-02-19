//
//  CCoreObjectPlacement.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 05/10/12.
//
//

#ifndef __C_CORE_OBJECT_PLACEMENT_H_INCLUDED__
#define __C_CORE_OBJECT_PLACEMENT_H_INCLUDED__

#include "../CCollisionManager.h"
#include "../../SceneNodes/CGridSceneNode.h"

class CCoreObjectPlacement : public IEventReceiver {
  
public:
    
    CCoreObjectPlacement(ISceneManager *_smgr, ICursorControl *_cusorCtrl, CCollisionManager *_colMgr);
    ~CCoreObjectPlacement();
    
    bool OnEvent(const SEvent &event);
    
    void setNodeToPlace(ISceneNode *node);
    ISceneNode *getNodeToPlace() { return nodeToPlace; }
    void setLightNode(ISceneNode *node);
    
    void refresh(ISceneNode *cursorPosition);
    
    IAnimatedMeshSceneNode *getArrowVerticalLine() { return arrowVerticalLineNode; }
    CGridSceneNode *getGridSceneNode() { return gridSceneNode; }
    
    bool isPlacing() { return isMoving; }
    void setAllowMoving(bool _allowMoving) { allowMoving = _allowMoving; }
    
private:
    
    ISceneManager *smgr;
    ICursorControl *cursorCtrl;
    CCollisionManager *colMgr;
    
    vector3df position;
    
    ISceneNode *nodeToPlace;
    
    IAnimatedMesh *arrowVerticalLine;
    IAnimatedMeshSceneNode *arrowVerticalLineNode;
    
    ISceneNode *lightCone, *lightNode;
    IMesh *coneMesh;
    
    CGridSceneNode *gridSceneNode;
    
    bool isMoving;
    bool allowMoving;
};

#endif
