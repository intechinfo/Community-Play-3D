//
//  CCoreObjectPlacement.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 05/10/12.
//
//
#include "stdafx.h"
#include "CCoreObjectPlacement.h"

CCoreObjectPlacement::CCoreObjectPlacement(ISceneManager *_smgr, ICursorControl *_cursorCtrl, CCollisionManager *_colMgr, ISceneManager *otherSmgr) {
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
	allowFreeMoving = false;
    
    arrowYLine = smgr->addArrowMesh("arrowYLine", 0x66BB00, 0x66BB00); //vert
	arrowYLineNode = smgr->addAnimatedMeshSceneNode(arrowYLine);
	arrowYLineNode->setVisible(false);
	arrowYLineNode->setName("editor:arrowY");
	arrowYLineNode->setMaterialType(EMT_SOLID);
	arrowYLineNode->setMaterialFlag(EMF_LIGHTING, false);
	arrowYLineNode->setMaterialFlag(EMF_ZBUFFER, false);
	arrowYLineNode->setScale(vector3df(10, 40, 10));
	colMgr->setCollisionFromBoundingBox(arrowYLineNode);
    
	arrowXLine = smgr->addArrowMesh("arrowXLine", 0xffff0000, 0xffff0000); //rouge
	arrowXLineNode = smgr->addAnimatedMeshSceneNode(arrowXLine);
	arrowXLineNode->setVisible(false);
	arrowXLineNode->setName("editor:arrowX");
	arrowXLineNode->setMaterialType(EMT_SOLID);
	arrowXLineNode->setMaterialFlag(EMF_LIGHTING, false);
	arrowXLineNode->setMaterialFlag(EMF_ZBUFFER, false);
	arrowXLineNode->setScale(vector3df(10, 40, 10));
	arrowXLineNode->setRotation(vector3df(0, 0, -90));
	colMgr->setCollisionFromBoundingBox(arrowXLineNode);

	arrowZLine = smgr->addArrowMesh("arrowZLine", 0x3300DD, 0x3300DD); //bleu
	arrowZLineNode = smgr->addAnimatedMeshSceneNode(arrowZLine);
	arrowZLineNode->setVisible(false);
	arrowZLineNode->setName("editor:arrowZ");
	arrowZLineNode->setMaterialType(EMT_SOLID);
	arrowZLineNode->setMaterialFlag(EMF_LIGHTING, false);
	arrowZLineNode->setMaterialFlag(EMF_ZBUFFER, false);
	arrowZLineNode->setScale(vector3df(10, 40, 10));
	arrowZLineNode->setRotation(vector3df(90, 0, 0));
	colMgr->setCollisionFromBoundingBox(arrowZLineNode);

	arrowYXZLine = smgr->addArrowMesh("arrowYXZLine", 0xf2a400, 0xf2a400); //orange
	arrowYXZLineNode = smgr->addAnimatedMeshSceneNode(arrowYXZLine);
	arrowYXZLineNode->setVisible(false);
	arrowYXZLineNode->setName("editor:arrowYXZ");
	arrowYXZLineNode->setMaterialType(EMT_SOLID);
	arrowYXZLineNode->setMaterialFlag(EMF_LIGHTING, false);
	arrowYXZLineNode->setMaterialFlag(EMF_ZBUFFER, false);
	arrowYXZLineNode->setScale(vector3df(10, 40, 10));
	arrowYXZLineNode->setRotation(vector3df(45, 0, -45));
	colMgr->setCollisionFromBoundingBox(arrowYXZLineNode);
    
    gridSceneNode = new CGridSceneNode(otherSmgr->getRootSceneNode(), otherSmgr);
    gridSceneNode->setName("editor:grid");//editor:Ygrid
    gridSceneNode->setMaterialFlag(EMF_ANTI_ALIASING, true);
    colMgr->setCollisionFromBoundingBox(gridSceneNode);

	yGridSceneNode = new CGridSceneNode(otherSmgr->getRootSceneNode(), otherSmgr);
    yGridSceneNode->setName("editor:Ygrid");
    yGridSceneNode->setMaterialFlag(EMF_ANTI_ALIASING, true);
    colMgr->setCollisionFromBoundingBox(gridSceneNode);
	yGridSceneNode->SetGridColor(SColor(0x66BB00));
	//yGridSceneNode->SetSize(300);
	yGridSceneNode->SetSpacing(1);
	yGridSceneNode->setVisible(false);
}

CCoreObjectPlacement::~CCoreObjectPlacement() {
    
}

void CCoreObjectPlacement::refresh(ISceneNode *cursorPosition) {
    if (isMoving && nodeToPlace) {
		arrowYLineNode->setPosition(nodeToPlace->getAbsolutePosition());
		arrowXLineNode->setPosition(nodeToPlace->getAbsolutePosition());
		arrowZLineNode->setPosition(nodeToPlace->getAbsolutePosition());
		arrowYXZLineNode->setPosition(nodeToPlace->getAbsolutePosition());
		yGridSceneNode->setPosition(vector3df(0, nodeToPlace->getAbsolutePosition().Y, 0));
    } else {
		setArrowVisible(false);
		isMoving = false;
		nodeToPlace = 0;
		setArrowType(Undefined);
	}
}

void CCoreObjectPlacement::setLightNode(ISceneNode *node) {
    lightNode = node;
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
        if (nodeToPlace->getType() == ESNT_OCTREE || nodeToPlace->getType() == ESNT_ANIMATED_MESH
			|| nodeToPlace->getType() == ESNT_MESH) {
            nodeToPlace->setTriangleSelector(0);
        }
    }
}

void CCoreObjectPlacement::setArrowVisible(bool set) {
	if (allowMoving) {
		if(set && nodeToPlace) {
			arrowYLineNode->setVisible(true);
			arrowXLineNode->setVisible(true);
			arrowZLineNode->setVisible(true);
			if(arrowType == Scale) {
				arrowYXZLineNode->setVisible(true);
			}
			else {
				arrowYXZLineNode->setVisible(false);
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

f32 CCoreObjectPlacement::getSmallestArrow() {
	f32 smallestArrow;
	if(arrowYLineNode->getScale().Y <= arrowXLineNode->getScale().Y) {
		if(arrowYLineNode->getScale().Y <= arrowZLineNode->getScale().Y) {
			smallestArrow = arrowYLineNode->getScale().Y;
		}
		else {
			smallestArrow = arrowZLineNode->getScale().Y;
		}
	}
	else if(arrowXLineNode->getScale().Y <= arrowZLineNode->getScale().Y) {
		smallestArrow = arrowXLineNode->getScale().Y;
	}
	else {
		smallestArrow = arrowZLineNode->getScale().Y;
	}
	return smallestArrow;
}

void CCoreObjectPlacement::setArrowType(ArrowType AT) {
	arrowType = AT;
	if (!nodeToPlace)
		return;

	f32 ySize = nodeToPlace->getScale().Y;
	f32 xSize = nodeToPlace->getScale().X;
	f32 zSize = nodeToPlace->getScale().Z;
	f32 smallestArrow = getSmallestArrow();
	setArrowVisible(true);
	switch (arrowType) {
		case Position:
			yGridSceneNode->setVisible(true);
			arrowYLineNode->setScale(vector3df(10, 40, 10));
			arrowZLineNode->setScale(vector3df(10, 40, 10));
			arrowXLineNode->setScale(vector3df(10, 40, 10));
			break;
		case Rotation:
			yGridSceneNode->setVisible(false);
			allowFreeMoving = false;
			arrowYLineNode->setScale(vector3df(10, 40 + (nodeToPlace->getRotation().Y / 6), 10));
			arrowXLineNode->setScale(vector3df(10, 40 + (nodeToPlace->getRotation().X / 6), 10));
			arrowZLineNode->setScale(vector3df(10, 40 + (nodeToPlace->getRotation().Z / 6), 10));
			break;
		case Scale:
			yGridSceneNode->setVisible(false);
			allowFreeMoving = false;
		
			if (ySize >= 0,1 && ySize < 1) {
				arrowYLineNode->setScale(vector3df(10, 10 + (30 * ySize), 10));
			}
			else if (ySize == 1) {
				arrowYLineNode->setScale(vector3df(10, 45, 10));
			}
			else if (ySize > 1) {
				arrowYLineNode->setScale(vector3df(10, 50 + (10 * ySize), 10));
			}

			if (xSize >= 0,1 && xSize < 1) {
				arrowXLineNode->setScale(vector3df(10, 10 + (30 * xSize), 10));
			}
			else if (xSize == 1) {
				arrowXLineNode->setScale(vector3df(10, 45, 10));
			}
			else if (xSize > 1) {
				arrowXLineNode->setScale(vector3df(10, 50 + (10 * xSize), 10));
			}

			if (zSize >= 0,1 && zSize < 1) {
				arrowZLineNode->setScale(vector3df(10, 10 + (30 * zSize), 10));
			}
			else if (zSize == 1) {
				arrowZLineNode->setScale(vector3df(10, 45, 10));
			}
			else if (zSize > 1) {
				arrowZLineNode->setScale(vector3df(10, 50 + (10 * zSize), 10));
			}

			setScaleToNode(arrowYXZLineNode, smallestArrow);
			break;
		default:
			yGridSceneNode->setVisible(false);
			arrowYLineNode->setScale(vector3df(10, 40, 10));
			arrowZLineNode->setScale(vector3df(10, 40, 10));
			arrowXLineNode->setScale(vector3df(10, 40, 10));
			break;
	}
}

void CCoreObjectPlacement::setScaleToNode(IAnimatedMeshSceneNode *arrowLineNode, f32 valueToAdd) {
	if(arrowLineNode == arrowYLineNode) {
		if(valueToAdd >= 13 && valueToAdd < 40) {
			arrowLineNode->setScale(vector3df(10, valueToAdd, 10));
			nodeToPlace->setScale(vector3df(nodeToPlace->getScale().X, (arrowLineNode->getScale().Y - 10) / 30, nodeToPlace->getScale().Z));
		}
		else if (valueToAdd >= 40 && valueToAdd <= 50) {
			arrowLineNode->setScale(vector3df(10, valueToAdd, 10));
			nodeToPlace->setScale(vector3df(nodeToPlace->getScale().X, 1, nodeToPlace->getScale().Z));
		}
		else if (valueToAdd > 50) {
			arrowLineNode->setScale(vector3df(10, valueToAdd, 10));
			nodeToPlace->setScale(vector3df(nodeToPlace->getScale().X, (arrowLineNode->getScale().Y - 40) / 10, nodeToPlace->getScale().Z));
		}
	}
	else if(arrowLineNode == arrowXLineNode) {
		if(valueToAdd >= 13 && valueToAdd < 40) {
			arrowLineNode->setScale(vector3df(10, valueToAdd, 10));
			nodeToPlace->setScale(vector3df((arrowLineNode->getScale().Y - 10) / 30, nodeToPlace->getScale().Y, nodeToPlace->getScale().Z));
		}
		else if (valueToAdd >= 40 && valueToAdd <= 50) {
			arrowLineNode->setScale(vector3df(10, valueToAdd, 10));
			nodeToPlace->setScale(vector3df(1, nodeToPlace->getScale().Y, nodeToPlace->getScale().Z));
		}
		else if (valueToAdd > 50) {
			arrowLineNode->setScale(vector3df(10, valueToAdd, 10));
			nodeToPlace->setScale(vector3df((arrowLineNode->getScale().Y - 40) / 10, nodeToPlace->getScale().Y, nodeToPlace->getScale().Z));
		}
	}
	else if(arrowLineNode == arrowZLineNode) {
		if(valueToAdd >= 13 && valueToAdd < 40) {
			arrowLineNode->setScale(vector3df(10, valueToAdd, 10));
			nodeToPlace->setScale(vector3df(nodeToPlace->getScale().X, nodeToPlace->getScale().Y, (arrowLineNode->getScale().Y - 10) / 30));
		}
		else if (valueToAdd >= 40 && valueToAdd <= 50) {
			arrowLineNode->setScale(vector3df(10, valueToAdd, 10));
			nodeToPlace->setScale(vector3df(nodeToPlace->getScale().X, nodeToPlace->getScale().Y, 1));
		}
		else if (valueToAdd > 50) {
			arrowLineNode->setScale(vector3df(10, valueToAdd, 10));
			nodeToPlace->setScale(vector3df(nodeToPlace->getScale().X, nodeToPlace->getScale().Y, (arrowLineNode->getScale().Y - 40) / 10));
		}
	}
	else if(arrowLineNode == arrowYXZLineNode) {
		arrowYXZLineNode->setScale(vector3df(10, valueToAdd, 10));
	}
}

bool CCoreObjectPlacement::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_KEY_INPUT_EVENT) {
		if (!event.KeyInput.PressedDown) {
			switch (event.KeyInput.Key) {
                    
                case KEY_SPACE:
					if(nodeToPlace != gridSceneNode && allowFreeMoving == false) {
						if(arrowType == Position) {
							core::line3d<f32> ray = collisionManager->getRayFromScreenCoordinates(cursorCtrl->getPosition());
							vector3df intersection;
							triangle3df hitTriangle;
							ISceneNode *selectedMouseNode = collisionManager->getSceneNodeAndCollisionPointFromRay(ray, intersection,
																										   hitTriangle, 0, 0);
							plane.setPlane(nodeToPlace->getPosition(), core::vector3df(0, -1, 0));
							allowFreeMoving = true;
							if(findAndSetMousePositionInPlane()) {
								nodeToPlace->setPosition(vector3df(mousePositionInPlane.X, nodeToPlace->getPosition().Y, mousePositionInPlane.Z));
							}
						}
					}
					else {
						allowFreeMoving = false;
						smgr->getActiveCamera()->setInputReceiverEnabled(true);
					}
                    break;
                
                default:
                    break;
            }
            
        }
    }

    if (event.EventType == EET_MOUSE_INPUT_EVENT && isMoving) {
		if (event.MouseInput.Event == EMIE_MOUSE_WHEEL) {
			if(nodeToPlace != gridSceneNode && selectedArrow == NULL) {
				if(arrowType == Position && allowFreeMoving == true) {
					core::line3d<f32> ray = collisionManager->getRayFromScreenCoordinates(cursorCtrl->getPosition());
					vector3df intersection;
					triangle3df hitTriangle;
					ISceneNode *selectedMouseNode = collisionManager->getSceneNodeAndCollisionPointFromRay(ray, intersection,
																										   hitTriangle, 0, 0);
					plane.setPlane(nodeToPlace->getPosition(), core::vector3df(0, -1, 0));
					if(findAndSetMousePositionInPlane()) {
						nodeToPlace->setPosition(vector3df(mousePositionInPlane.X, nodeToPlace->getPosition().Y + event.MouseInput.Wheel, mousePositionInPlane.Z));
					}
				}
				else if(arrowType == Scale) {
					f32 smallestArrow = getSmallestArrow();
					if (nodeToPlace->getType() == ESNT_BILLBOARD) {
						IBillboardSceneNode *bbnode = (IBillboardSceneNode *)nodeToPlace;
						bbnode->setSize(dimension2d<f32>(bbnode->getSize().Width + event.MouseInput.Wheel,
														 bbnode->getSize().Height + event.MouseInput.Wheel));
					} else {
						setScaleToNode(arrowYLineNode, arrowYLineNode->getScale().Y + (event.MouseInput.Wheel * (arrowYLineNode->getScale().Y / smallestArrow)));
						setScaleToNode(arrowXLineNode, arrowXLineNode->getScale().Y + (event.MouseInput.Wheel * (arrowXLineNode->getScale().Y / smallestArrow)));
						setScaleToNode(arrowZLineNode, arrowZLineNode->getScale().Y + (event.MouseInput.Wheel * (arrowZLineNode->getScale().Y / smallestArrow)));

						setScaleToNode(arrowYXZLineNode, smallestArrow);
					}
				}
			}
		}
		if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP) {
			selectedArrow = NULL;
			allowFreeMoving = false;
			smgr->getActiveCamera()->setInputReceiverEnabled(true);
		}
		if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN) {
			if(allowFreeMoving == true) {
				allowFreeMoving = false;
				smgr->getActiveCamera()->setInputReceiverEnabled(true);
			}
			else if(selectedArrow == NULL) {
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
				scaleArrow = arrowYXZLineNode->getScale();
			}
		}
		if (event.MouseInput.Event == EMIE_MOUSE_MOVED) {
			core::list<ISceneNodeAnimator *>::ConstIterator manimator = smgr->getActiveCamera()->getAnimators().begin();
			if(selectedArrow != NULL && nodeToPlace != gridSceneNode) {
				if(selectedArrow == arrowYLineNode) {
					if(findAndSetMousePositionInPlane()) {
						switch (arrowType) {
							case Position:
								nodeToPlace->setPosition(vector3df(nodeToPlace->getPosition().X, (mousePositionInPlane.Y - mousePosition.Y), nodeToPlace->getPosition().Z));
								break;
							case Rotation:
								if((scaleArrow.Y + (mousePositionInPlane.Y - mousePosition.Y)) > 40 && (scaleArrow.Y + (mousePositionInPlane.Y - mousePosition.Y)) <= 100) {
									arrowYLineNode->setScale(vector3df(10, scaleArrow.Y + (mousePositionInPlane.Y - mousePosition.Y), 10));
									nodeToPlace->setRotation(vector3df(nodeToPlace->getRotation().X, (arrowYLineNode->getScale().Y - 40) * 6, nodeToPlace->getRotation().Z));
								}
								else if((scaleArrow.Y + (mousePositionInPlane.Y - mousePosition.Y)) >= 35 && (scaleArrow.Y + (mousePositionInPlane.Y - mousePosition.Y)) <= 40) {
									arrowYLineNode->setScale(vector3df(10, 40, 10));
									nodeToPlace->setRotation(vector3df(nodeToPlace->getRotation().X, 0, nodeToPlace->getRotation().Z));
								}
								break;
							case Scale:
								setScaleToNode(arrowYLineNode, scaleArrow.Y + (mousePositionInPlane.Y - mousePosition.Y));
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
								if((scaleArrow.Y + (mousePositionInPlane.X - mousePosition.X)) >= 40 && (scaleArrow.Y + (mousePositionInPlane.X - mousePosition.X)) <= 100) {
									arrowXLineNode->setScale(vector3df(10, scaleArrow.Y + (mousePositionInPlane.X - mousePosition.X), 10));
									nodeToPlace->setRotation(vector3df((arrowXLineNode->getScale().Y - 40) * 6, nodeToPlace->getRotation().Y, nodeToPlace->getRotation().Z));
								}
								else if((scaleArrow.Y + (mousePositionInPlane.X - mousePosition.X)) >= 35 && (scaleArrow.Y + (mousePositionInPlane.X - mousePosition.X)) <= 40) {
									arrowXLineNode->setScale(vector3df(10, 40, 10));
									nodeToPlace->setRotation(vector3df(0, nodeToPlace->getRotation().Y, nodeToPlace->getRotation().Z));
								}
								break;
							case Scale:
								setScaleToNode(arrowXLineNode, scaleArrow.Y + (mousePositionInPlane.X - mousePosition.X));
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
								if((scaleArrow.Y + (mousePositionInPlane.Z - mousePosition.Z)) >= 40 && (scaleArrow.Y + (mousePositionInPlane.Z - mousePosition.Z)) <= 100) {
									arrowZLineNode->setScale(vector3df(10, scaleArrow.Y + (mousePositionInPlane.Z - mousePosition.Z), 10));
									nodeToPlace->setRotation(vector3df(nodeToPlace->getRotation().X, nodeToPlace->getRotation().Y, (arrowZLineNode->getScale().Y - 40) * 6));
								}
								else if((scaleArrow.Y + (mousePositionInPlane.Z - mousePosition.Z)) >= 35 && (scaleArrow.Y + (mousePositionInPlane.Z - mousePosition.Z)) <= 40) {
									arrowZLineNode->setScale(vector3df(10, 40, 10));
									nodeToPlace->setRotation(vector3df(nodeToPlace->getRotation().X, nodeToPlace->getRotation().Y, 0));
								}
								break;
							case Scale:
								setScaleToNode(arrowZLineNode, scaleArrow.Y + (mousePositionInPlane.Z - mousePosition.Z));
								break;
							default:
								break;
						}
					}
				}
				f32 smallestArrow = getSmallestArrow();

				setScaleToNode(arrowYXZLineNode, smallestArrow);
			}
			else if (allowFreeMoving == true) {
				if(findAndSetMousePositionInPlane()) {
					nodeToPlace->setPosition(vector3df(mousePositionInPlane.X, nodeToPlace->getPosition().Y, mousePositionInPlane.Z));
				}
			}
		}
    }
    
    return false;
}
