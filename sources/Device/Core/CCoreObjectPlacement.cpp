//
//  CCoreObjectPlacement.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 05/10/12.
//
//

#include "CCoreObjectPlacement.h"

CCoreObjectPlacement::CCoreObjectPlacement(ISceneManager *_smgr, ICursorControl *_cusorCtrl, CCollisionManager *_colMgr) {
    smgr = _smgr;
    cursorCtrl = _cusorCtrl;
    colMgr = _colMgr;
    
    isMoving = false;
    nodeToPlace = 0;
    
    allowMoving = true;
    
    arrowVerticalLine = smgr->addArrowMesh("arrowVerticalLine", 0xffff0000, 0xffff0000);
	arrowVerticalLineNode = smgr->addAnimatedMeshSceneNode(arrowVerticalLine);
	arrowVerticalLineNode->setVisible(false);
	arrowVerticalLineNode->setName("editor:arrow");
	
	arrowVerticalLineNode->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	arrowVerticalLineNode->setMaterialFlag(EMF_LIGHTING, false);
	arrowVerticalLineNode->setScale(vector3df(10, 100, 10));
    
    gridSceneNode = new CGridSceneNode(smgr->getRootSceneNode(), smgr);
    gridSceneNode->setName("editor:grid");
    gridSceneNode->setMaterialFlag(EMF_ANTI_ALIASING, true);
    colMgr->setCollisionToAnAnimatedNode(gridSceneNode);
}

CCoreObjectPlacement::~CCoreObjectPlacement() {
    
}

void CCoreObjectPlacement::refresh(vector3df cursorPosition) {
    if (isMoving) {
        arrowVerticalLineNode->setPosition(cursorPosition);
        if (nodeToPlace) {
            nodeToPlace->setPosition(vector3df(arrowVerticalLineNode->getPosition().X,
                                               arrowVerticalLineNode->getPosition().Y+arrowVerticalLineNode->getScale().Y,
                                               arrowVerticalLineNode->getPosition().Z));
        }
    }
}

void CCoreObjectPlacement::setNodeToPlace(ISceneNode *node) {
    nodeToPlace = node;
    if (nodeToPlace) {
        if (nodeToPlace->getType() == ESNT_OCTREE || nodeToPlace->getType() == ESNT_ANIMATED_MESH) {
            nodeToPlace->setTriangleSelector(0);
        }
    }
}

bool CCoreObjectPlacement::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_KEY_INPUT_EVENT) {
		if (!event.KeyInput.PressedDown) {
			switch (event.KeyInput.Key) {
                    
                //---------------------
                    
				case KEY_SPACE:
                    if (allowMoving) {
                        arrowVerticalLineNode->setVisible(!arrowVerticalLineNode->isVisible());
                        if (isMoving==true) {
                            if (nodeToPlace) {
                                if (nodeToPlace->getType() == ESNT_OCTREE) {
                                    colMgr->setCollisionToAnOctTreeNode(nodeToPlace);
                                }
                                if (nodeToPlace->getType() == ESNT_ANIMATED_MESH) {
                                    colMgr->setCollisionToAnAnimatedNode(nodeToPlace);
                                }
                            }
                            setNodeToPlace(0);
                            isMoving = false;
                        } else {
                            isMoving = true;
                        }
                    }
                    break;
                    
                //--------------------- 
                
                default:
                    break;
            }
            
        }
    }
    
    if (event.EventType == EET_MOUSE_INPUT_EVENT) {
        if (event.MouseInput.Event == EMIE_MOUSE_WHEEL) {
            vector3df wheel = arrowVerticalLineNode->getScale();
            #ifdef _IRR_OSX_PLATFORM_
                arrowVerticalLineNode->setScale(vector3df(wheel.X, wheel.Y+event.MouseInput.Wheel*0.1, wheel.Z));
            #else
                arrowVerticalLineNode->setScale(vector3df(wheel.X, wheel.Y+event.MouseInput.Wheel, wheel.Z));
            #endif
        }
    }
    
    return false;
}
