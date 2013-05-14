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
    
	typedef enum {
		Undefined,
		Position,
		Rotation,
		Scale
	} ArrowType;

    CCoreObjectPlacement(ISceneManager *_smgr, ICursorControl *_cursorCtrl, CCollisionManager *_colMgr, ISceneManager *otherSmgr);
    ~CCoreObjectPlacement();
    
    bool OnEvent(const SEvent &event);
    
    void setNodeToPlace(ISceneNode *node);
    ISceneNode *getNodeToPlace() { return nodeToPlace; }
    void setLightNode(ISceneNode *node);
	void setCollisionToNormal();
	void setArrowVisible(bool set);
	bool findAndSetMousePositionInPlane();
	f32 getSmallestArrow();
	void setArrowType(ArrowType AT);
	void setScaleToNode(IAnimatedMeshSceneNode *arrowLineNode, f32 valueToAdd);
    
    void refresh(ISceneNode *cursorPosition);
    
    CGridSceneNode *getGridSceneNode() { return gridSceneNode; }
	CGridSceneNode *getYGridSceneNode() { return yGridSceneNode; }
    
    bool isPlacing() { return isMoving; }
	ArrowType getArrowType() { return arrowType; }
    void setAllowMoving(bool _allowMoving) { allowMoving = _allowMoving; }
    
private:
    
    ISceneManager *smgr;
    ICursorControl *cursorCtrl;
    CCollisionManager *colMgr;

	ArrowType arrowType;
    
    /*vector3df cameraPosition;
	vector3df cameraRotation;*/
	vector3df mousePosition;
	vector3df mousePositionInPlane;
	vector3df scaleArrow;

	plane3df plane;

	line3d<f32> ray;
    
    ISceneNode *nodeToPlace;
	ISceneNode *selectedArrow;
    
    IAnimatedMesh *arrowYLine;
    IAnimatedMeshSceneNode *arrowYLineNode;
	IAnimatedMesh *arrowXLine;
    IAnimatedMeshSceneNode *arrowXLineNode;
	IAnimatedMesh *arrowZLine;
    IAnimatedMeshSceneNode *arrowZLineNode;
	IAnimatedMesh *arrowYXZLine;
    IAnimatedMeshSceneNode *arrowYXZLineNode;

    ISceneNode *lightCone, *lightNode;
    IMesh *coneMesh;
    
	ISceneCollisionManager *collisionManager;

    CGridSceneNode *gridSceneNode;
	CGridSceneNode *yGridSceneNode;
    
    bool isMoving;
    bool allowMoving;
	bool allowFreeMoving;
};

#endif
