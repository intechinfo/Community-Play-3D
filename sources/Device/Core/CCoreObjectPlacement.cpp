//
//  CCoreObjectPlacement.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 05/10/12.
//
//

#include "CCoreObjectPlacement.h"

CCoreObjectPlacement::CCoreObjectPlacement(ISceneManager *_smgr, ICursorControl *_cursorCtrl, CCollisionManager *_colMgr) {
    smgr = _smgr;
    cursorCtrl = _cursorCtrl;
    colMgr = _colMgr;

	arrowType = Undefined;

	scaleArrow = vector3df(10, 40, 10);

	selectedArrow = NULL;
    
    isMoving = false;
    nodeToPlace = 0;
    lightNode = 0;

	collisionManager = smgr->getSceneCollisionManager();
    
    allowMoving = true;
    
    arrowYLine = smgr->addArrowMesh("arrowYLine", 0x66BB00, 0x66BB00);
	arrowYLineNode = smgr->addAnimatedMeshSceneNode(arrowYLine);
	arrowYLineNode->setVisible(false);
	arrowYLineNode->setName("editor:arrowY");
	arrowYLineNode->setMaterialType(EMT_SOLID);
	arrowYLineNode->setMaterialFlag(EMF_LIGHTING, false);
	//arrowYLineNode->setMaterialFlag(EMF_ZBUFFER, false);
	arrowYLineNode->setScale(vector3df(10, 40, 10));
	colMgr->setCollisionFromBoundingBox(arrowYLineNode);
    
	arrowXLine = smgr->addArrowMesh("arrowXLine", 0xffff0000, 0xffff0000);
	arrowXLineNode = smgr->addAnimatedMeshSceneNode(arrowXLine);
	arrowXLineNode->setVisible(false);
	arrowXLineNode->setName("editor:arrowX");
	arrowXLineNode->setMaterialType(EMT_SOLID);
	arrowXLineNode->setMaterialFlag(EMF_LIGHTING, false);
	//arrowXLineNode->setMaterialFlag(EMF_ZBUFFER, false);
	arrowXLineNode->setScale(vector3df(10, 40, 10));
	arrowXLineNode->setRotation(vector3df(0, 0, -90));
	colMgr->setCollisionFromBoundingBox(arrowXLineNode);

	arrowZLine = smgr->addArrowMesh("arrowZLine", 0x3300DD, 0x3300DD);
	arrowZLineNode = smgr->addAnimatedMeshSceneNode(arrowZLine);
	arrowZLineNode->setVisible(false);
	arrowZLineNode->setName("editor:arrowZ");
	arrowZLineNode->setMaterialType(EMT_SOLID);
	arrowZLineNode->setMaterialFlag(EMF_LIGHTING, false);
	//arrowZLineNode->setMaterialFlag(EMF_ZBUFFER, false);
	arrowZLineNode->setScale(vector3df(10, 40, 10));
	arrowZLineNode->setRotation(vector3df(90, 0, 0));
	colMgr->setCollisionFromBoundingBox(arrowZLineNode);

	arrowYXZLine = smgr->addArrowMesh("arrowYXZLine", 0xf2a400, 0xf2a400);
	arrowYXZLineNode = smgr->addAnimatedMeshSceneNode(arrowYXZLine);
	arrowYXZLineNode->setVisible(false);
	arrowYXZLineNode->setName("editor:arrowYXZ");
	arrowYXZLineNode->setMaterialType(EMT_SOLID);
	arrowYXZLineNode->setMaterialFlag(EMF_LIGHTING, false);
	//arrowYXZLineNode->setMaterialFlag(EMF_ZBUFFER, false);
	arrowYXZLineNode->setScale(vector3df(10, 40, 10));
	arrowYXZLineNode->setRotation(vector3df(45, 0, -45));
	colMgr->setCollisionFromBoundingBox(arrowYXZLineNode);

    coneMesh = smgr->getGeometryCreator()->createConeMesh(50, 50, 50);
    lightCone = smgr->addMeshSceneNode(coneMesh);
    lightCone->setMaterialFlag(EMF_WIREFRAME, true);
    lightCone->setMaterialFlag(EMF_LIGHTING, false);
    lightCone->setVisible(false);
    
    gridSceneNode = new CGridSceneNode(smgr->getRootSceneNode(), smgr);
    gridSceneNode->setName("editor:grid");
    gridSceneNode->setMaterialFlag(EMF_ANTI_ALIASING, true);
    colMgr->setCollisionFromBoundingBox(gridSceneNode);
}

CCoreObjectPlacement::~CCoreObjectPlacement() {
    
}

void CCoreObjectPlacement::refresh(ISceneNode *cursorPosition) {
    if (isMoving) {
		arrowYLineNode->setPosition(nodeToPlace->getAbsolutePosition());
		arrowXLineNode->setPosition(nodeToPlace->getAbsolutePosition());
		arrowZLineNode->setPosition(nodeToPlace->getAbsolutePosition());
		arrowYXZLineNode->setPosition(nodeToPlace->getAbsolutePosition());
    }
    if (lightNode) {
        coneMesh->drop();
        line3d<f32> distanceLight(lightNode->getPosition(), lightNode->getRotation());
        coneMesh = smgr->getGeometryCreator()->createConeMesh(((ILightSceneNode *)lightNode)->getRadius(), distanceLight.getLength(), 50);
        lightCone->remove();
        lightCone = smgr->addMeshSceneNode(coneMesh);
        lightCone->setMaterialFlag(EMF_WIREFRAME, true);
        lightCone->setMaterialFlag(EMF_LIGHTING, false);
        lightCone->setPosition(lightNode->getPosition());
        lightCone->setRotation(lightNode->getRotation());
    }
}

void CCoreObjectPlacement::setLightNode(ISceneNode *node) {
    lightNode = node;
    if (lightNode) {
        lightCone->setVisible(true);
    } else {
        lightCone->setVisible(false);
    }
}

void CCoreObjectPlacement::setCollisionToNormal() {
	if (nodeToPlace) {
        if (nodeToPlace->getType() == ESNT_OCTREE) {
            colMgr->setCollisionToAnOctTreeNode(nodeToPlace);
        }
        if (nodeToPlace->getType() == ESNT_ANIMATED_MESH) {
            colMgr->setCollisionToAnAnimatedNode(nodeToPlace);
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

void CCoreObjectPlacement::setArrowVisible(bool set) {
	if (allowMoving) {
		if(set) {
			arrowYLineNode->setVisible(true);
			arrowXLineNode->setVisible(true);
			arrowZLineNode->setVisible(true);
			if(arrowType == Scale) {
				arrowYXZLineNode->setVisible(true);
			}
			isMoving = true;
		}
		else {
			arrowYLineNode->setVisible(false);
			arrowXLineNode->setVisible(false);
			arrowZLineNode->setVisible(false);
			arrowYXZLineNode->setVisible(false);
			setNodeToPlace(0);
            isMoving = false;
		}
    }
}

bool CCoreObjectPlacement::findAndSetMousePositionInPlane() {
	ray = collisionManager->getRayFromScreenCoordinates(cursorCtrl->getPosition());
	if(plane.getIntersectionWithLine(ray.start, ray.getVector(), mousePositionInPlane)) {
		return true;
	}
	else {
		return false;
	}
}

void CCoreObjectPlacement::setArrowType(ArrowType AT) {
	arrowType = AT;
	switch (arrowType) {
		case Position:
			arrowYLineNode->setScale(vector3df(10, 40, 10));
			arrowZLineNode->setScale(vector3df(10, 40, 10));
			arrowXLineNode->setScale(vector3df(10, 40, 10));
			break;
		case Rotation:
			arrowYLineNode->setScale(vector3df(10, 40 + (nodeToPlace->getRotation().Y / 6), 10));
			arrowXLineNode->setScale(vector3df(10, 40 + (nodeToPlace->getRotation().X / 6), 10));
			arrowZLineNode->setScale(vector3df(10, 40 + (nodeToPlace->getRotation().Z / 6), 10));
			break;
		case Scale:
			if (nodeToPlace->getScale().Y >= 0,1 && nodeToPlace->getScale().Y < 1) {
				arrowYLineNode->setScale(vector3df(10, 10 + (30 * nodeToPlace->getScale().Y), 10));
			}
			else if (nodeToPlace->getScale().Y == 1) {
				arrowYLineNode->setScale(vector3df(10, 45, 10));
			}
			else if (nodeToPlace->getScale().Y > 1) {
				arrowYLineNode->setScale(vector3df(10, 50 + (10 * nodeToPlace->getScale().Y), 10));
			}
			if (nodeToPlace->getScale().X >= 0,1 && nodeToPlace->getScale().X <= 1) {
				arrowXLineNode->setScale(vector3df(10, 10 + (30 * nodeToPlace->getScale().X), 10));
			}
			else if (nodeToPlace->getScale().X == 1) {
				arrowXLineNode->setScale(vector3df(10, 45, 10));
			}
			else if (nodeToPlace->getScale().X > 1) {
				arrowXLineNode->setScale(vector3df(10, 50 + (10 * nodeToPlace->getScale().X), 10));
			}
			if (nodeToPlace->getScale().Z >= 0,1 && nodeToPlace->getScale().Z <= 1) {
				arrowZLineNode->setScale(vector3df(10, 10 + (30 * nodeToPlace->getScale().Z), 10));
			}
			else if (nodeToPlace->getScale().Z == 1) {
				arrowZLineNode->setScale(vector3df(10, 45, 10));
			}
			else if (nodeToPlace->getScale().Z > 1) {
				arrowZLineNode->setScale(vector3df(10, 50 + (10 * nodeToPlace->getScale().Z), 10));
			}
			break;
		default:
			arrowYLineNode->setScale(vector3df(10, 40, 10));
			arrowZLineNode->setScale(vector3df(10, 40, 10));
			arrowXLineNode->setScale(vector3df(10, 40, 10));
			break;
	}
}

bool CCoreObjectPlacement::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_KEY_INPUT_EVENT) {
		if (!event.KeyInput.PressedDown) {
			switch (event.KeyInput.Key) {
                    
                //---------------------
                    
				/*case KEY_SPACE:
                    setArrowVisible();
                    break;*/
                    
                //--------------------- 
                
                default:
                    break;
            }
            
        }
    }
    
    if (event.EventType == EET_MOUSE_INPUT_EVENT && isMoving) {
		if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP) {
			if(selectedArrow == NULL) {
				core::line3d<f32> ray = collisionManager->getRayFromScreenCoordinates(cursorCtrl->getPosition());
				vector3df intersection;
				triangle3df hitTriangle;
				ISceneNode *selectedMouseNode = collisionManager->getSceneNodeAndCollisionPointFromRay(ray, intersection,
																									   hitTriangle, 0, 0);
				if(selectedMouseNode == arrowYLineNode) { 
					selectedArrow = arrowYLineNode;
					plane.setPlane(nodeToPlace->getPosition(), core::vector3df(-1, 0, 0));
					scaleArrow = arrowYLineNode->getScale();
				}
				else if(selectedMouseNode == arrowXLineNode) { 
					selectedArrow = arrowXLineNode;
					plane.setPlane(nodeToPlace->getPosition(), core::vector3df(0, 0, -1));
					scaleArrow = arrowXLineNode->getScale();
				}
				else if(selectedMouseNode == arrowZLineNode) {
					selectedArrow = arrowZLineNode;
					plane.setPlane(nodeToPlace->getPosition(), core::vector3df(0, -1, 0));
					scaleArrow = arrowZLineNode->getScale();
				}
				if(findAndSetMousePositionInPlane()) {
					switch (arrowType) {
						case Position:
							mousePosition = mousePositionInPlane - nodeToPlace->getPosition();
							break;
						case Rotation:
							mousePosition = mousePositionInPlane;
							break;
						case Scale:
							mousePosition = mousePositionInPlane;
							break;
						default:
							break;
					}
				}
			}
			else {
				selectedArrow = NULL;
			}
		}
		if (event.MouseInput.Event == EMIE_MOUSE_MOVED) {
			if(selectedArrow != NULL) {
				if(selectedArrow == arrowYLineNode) {
					if(findAndSetMousePositionInPlane()) {
						switch (arrowType) {
							case Position:
								nodeToPlace->setPosition(vector3df(nodeToPlace->getPosition().X, (mousePositionInPlane.Y - mousePosition.Y), nodeToPlace->getPosition().Z));
								break;
							case Rotation:
								if((scaleArrow.Y + (mousePositionInPlane.Y - mousePosition.Y)) >= 40 && (scaleArrow.Y + (mousePositionInPlane.Y - mousePosition.Y)) <= 100)
								{
									arrowYLineNode->setScale(vector3df(10, scaleArrow.Y + (mousePositionInPlane.Y - mousePosition.Y), 10));
									nodeToPlace->setRotation(vector3df(nodeToPlace->getRotation().X, (arrowYLineNode->getScale().Y - 40) * 6, nodeToPlace->getRotation().Z));
								}
								break;
							case Scale:
								if((scaleArrow.Y + (mousePositionInPlane.Y - mousePosition.Y)) >= 13 && (scaleArrow.Y + (mousePositionInPlane.Y - mousePosition.Y)) < 40)
								{
									arrowYLineNode->setScale(vector3df(10, scaleArrow.Y + (mousePositionInPlane.Y - mousePosition.Y), 10));
									nodeToPlace->setScale(vector3df(nodeToPlace->getScale().X, (arrowYLineNode->getScale().Y - 10) / 30, nodeToPlace->getScale().Z));
								}
								else if ((scaleArrow.Y + (mousePositionInPlane.Y - mousePosition.Y)) >= 40 && (scaleArrow.Y + (mousePositionInPlane.Y - mousePosition.Y)) <= 50) {
									arrowYLineNode->setScale(vector3df(10, scaleArrow.Y + (mousePositionInPlane.Y - mousePosition.Y), 10));
									nodeToPlace->setScale(vector3df(nodeToPlace->getScale().X, 1, nodeToPlace->getScale().Z));
								}
								else if ((scaleArrow.Y + (mousePositionInPlane.Y - mousePosition.Y)) > 50) {
									arrowYLineNode->setScale(vector3df(10, scaleArrow.Y + (mousePositionInPlane.Y - mousePosition.Y), 10));
									nodeToPlace->setScale(vector3df(nodeToPlace->getScale().X, (arrowYLineNode->getScale().Y - 40) / 10, nodeToPlace->getScale().Z));
								}
								break;
							default:
								break;
						}
					}
				}
				else if(selectedArrow == arrowXLineNode) {
					if(findAndSetMousePositionInPlane()) {
						switch (arrowType) {
							case Position:
								nodeToPlace->setPosition(vector3df((mousePositionInPlane.X - mousePosition.X), nodeToPlace->getPosition().Y, nodeToPlace->getPosition().Z));
								break;
							case Rotation:
								if((scaleArrow.Y + (mousePositionInPlane.X - mousePosition.X)) >= 40 && (scaleArrow.Y + (mousePositionInPlane.X - mousePosition.X)) <= 100)
								{
									arrowXLineNode->setScale(vector3df(10, scaleArrow.Y + (mousePositionInPlane.X - mousePosition.X), 10));
									nodeToPlace->setRotation(vector3df((arrowXLineNode->getScale().Y - 40) * 6, nodeToPlace->getRotation().Y, nodeToPlace->getRotation().Z));
								}
								break;
							case Scale:
								if((scaleArrow.Y + (mousePositionInPlane.X - mousePosition.X)) >= 13 && (scaleArrow.Y + (mousePositionInPlane.X - mousePosition.X)) < 40)
								{
									arrowXLineNode->setScale(vector3df(10, scaleArrow.Y + (mousePositionInPlane.X - mousePosition.X), 10));
									nodeToPlace->setScale(vector3df((arrowXLineNode->getScale().Y - 10) / 30, nodeToPlace->getScale().Y, nodeToPlace->getScale().Z));
								}
								else if ((scaleArrow.Y + (mousePositionInPlane.X - mousePosition.X)) >= 40 && (scaleArrow.Y + (mousePositionInPlane.X - mousePosition.X)) <= 50) {
									arrowXLineNode->setScale(vector3df(10, scaleArrow.Y + (mousePositionInPlane.X - mousePosition.X), 10));
									nodeToPlace->setScale(vector3df(1, nodeToPlace->getScale().Y, nodeToPlace->getScale().Z));
								}
								else if ((scaleArrow.Y + (mousePositionInPlane.X - mousePosition.X)) > 50) {
									arrowXLineNode->setScale(vector3df(10, scaleArrow.Y + (mousePositionInPlane.X - mousePosition.X), 10));
									nodeToPlace->setScale(vector3df((arrowXLineNode->getScale().Y - 40) / 10, nodeToPlace->getScale().Y, nodeToPlace->getScale().Z));
								}
								break;
							default:
								break;
						}
					}
				}
				else if(selectedArrow == arrowZLineNode) {
					if(findAndSetMousePositionInPlane()) {
						switch (arrowType) {
							case Position:
								nodeToPlace->setPosition(vector3df(nodeToPlace->getPosition().X, nodeToPlace->getPosition().Y, (mousePositionInPlane.Z - mousePosition.Z)));
								break;
							case Rotation:
								if((scaleArrow.Y + (mousePositionInPlane.Z - mousePosition.Z)) >= 40 && (scaleArrow.Y + (mousePositionInPlane.Z - mousePosition.Z)) <= 100)
								{
									arrowZLineNode->setScale(vector3df(10, scaleArrow.Y + (mousePositionInPlane.Z - mousePosition.Z), 10));
									nodeToPlace->setRotation(vector3df(nodeToPlace->getRotation().X, nodeToPlace->getRotation().Y, (arrowZLineNode->getScale().Y - 40) * 6));
								}
								break;
							case Scale:
								if((scaleArrow.Y + (mousePositionInPlane.Z - mousePosition.Z)) >= 13 && (scaleArrow.Y + (mousePositionInPlane.Z - mousePosition.Z)) < 40)
								{
									arrowZLineNode->setScale(vector3df(10, scaleArrow.Y + (mousePositionInPlane.Z - mousePosition.Z), 10));
									nodeToPlace->setScale(vector3df(nodeToPlace->getScale().X, nodeToPlace->getScale().Y, (arrowZLineNode->getScale().Y - 10) / 30));
								}
								else if ((scaleArrow.Y + (mousePositionInPlane.Z - mousePosition.Z)) >= 40 && (scaleArrow.Y + (mousePositionInPlane.Z - mousePosition.Z)) <= 50) {
									arrowZLineNode->setScale(vector3df(10, scaleArrow.Y + (mousePositionInPlane.Z - mousePosition.Z), 10));
									nodeToPlace->setScale(vector3df(nodeToPlace->getScale().X, nodeToPlace->getScale().Y, 1));
								}
								else if ((scaleArrow.Y + (mousePositionInPlane.Z - mousePosition.Z)) > 50) {
									arrowZLineNode->setScale(vector3df(10, scaleArrow.Y + (mousePositionInPlane.Z - mousePosition.Z), 10));
									nodeToPlace->setScale(vector3df(nodeToPlace->getScale().X, nodeToPlace->getScale().Y, (arrowZLineNode->getScale().Y - 40) / 10));
								}
								break;
							default:
								break;
						}
					}
				}
			}
		}
    }
    
    return false;
}