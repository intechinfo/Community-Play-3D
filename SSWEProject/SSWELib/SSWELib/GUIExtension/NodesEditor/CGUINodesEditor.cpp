/*
 *  UIContextMenu.h
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "CGUINodesEditor.h"

void CGUINodesEditor::addNode(CGUINode *node) {
    if (node->getParent() == 0)
        node->setParent(rootNode);
    
    node->getParent()->addChild(node);
    node->createInterface(this);
    
    IGUIElement *parentNodeInterface = node->getParent()->getInterface();
    
    if (node->getParent() != rootNode) {
        u32 futherYPosition = -node->getInterface()->getRelativePosition().getHeight();
        
        core::list<CGUINode *>::ConstIterator it = node->getParent()->getChildren().begin();
        for (; it != node->getChildren().end(); ++it)
            futherYPosition += (*it)->getInterface()->getRelativePosition().getHeight();
        
        node->getInterface()->setRelativePosition(position2di(parentNodeInterface->getRelativePosition().UpperLeftCorner.X + parentNodeInterface->getRelativePosition().getWidth() + 50, 
                                                              parentNodeInterface->getRelativePosition().UpperLeftCorner.Y + futherYPosition + ((futherYPosition == 0) ? 0 : 10)
                                                              + (node->getParent() == rootNode ? 10 : 10))
                                                  );
    }
    
    recalculateTotals();
    
}

void CGUINodesEditor::getArrayOfNodes(CGUINode *node, core::array<CGUINode *> &outNodes) {
    const core::list<CGUINode *> list = node->getChildren();
    core::list<CGUINode *>::ConstIterator it = list.begin();
    
    for (; it != list.end(); ++it) {
        outNodes.push_back(*it);
        getArrayOfNodes(*it, outNodes);
    }
}

CGUINode *CGUINodesEditor::getSelectedNode() {
    IGUIElement *selement = Environment->getFocus();
    if (selement == 0)
        return 0;
    
    CGUINode *node = 0;
    
    core::array<CGUINode *> nodes;
    this->getArrayOfNodes(rootNode, nodes);
    
    bool founded = false;
    while (selement->getParent() != Environment->getRootGUIElement() && !founded) {
        for (u32 i=0; i < nodes.size(); i++) {
            if (nodes[i]->getInterface() == selement) {
                node = nodes[i];
                founded = true;
                break;
            }
        }
        selement = selement->getParent();
    }
    
    return node;
}

void CGUINodesEditor::recalculateTotals() {
    core::array<CGUINode *> currentNodes;
    getArrayOfNodes(rootNode, currentNodes);
    for (u32 i=0; i < currentNodes.size(); i++) {
        if (currentNodes[i]->getInterface()->getRelativePosition().LowerRightCorner.X > totalWidth) {
            totalWidth = currentNodes[i]->getInterface()->getRelativePosition().LowerRightCorner.X;
        }
        
        if (currentNodes[i]->getInterface()->getRelativePosition().LowerRightCorner.Y > totalHeight) {
            totalHeight = currentNodes[i]->getInterface()->getRelativePosition().LowerRightCorner.Y;
        }
    }
    
    ScrollBarH->setMax(totalWidth);
    ScrollBarV->setMax(totalHeight);
}

//! constructor
CGUINodesEditor::CGUINodesEditor(irr::IrrlichtDevice *_device, IGUIElement* parent,
			s32 id, core::rect<s32> rectangle, bool noclip)
: IGUIElement(EGUIET_ELEMENT, _device->getGUIEnvironment(), parent, id, rectangle)
{
	#ifdef _DEBUG
	setDebugName("CGUINodesEditor");
	#endif
	setNotClipped(noclip);
    
    clip = noclip;
    device = _device;

	// This element can be tabbed.
	setTabStop(false);
	setTabOrder(-1);
    
    IGUISkin* skin = Environment->getSkin();
	s32 s = skin->getSize( EGDS_SCROLLBAR_SIZE );

    ScrollBarV = Environment->addScrollBar(false, core::rect<s32>(RelativeRect.getWidth() - s, 0, RelativeRect.getWidth(),RelativeRect.getHeight() - s), this, 0);
    ScrollBarV->setSubElement(true);
    ScrollBarV->setPos( 0 );

    ScrollBarH = Environment->addScrollBar(true, core::rect<s32>(0, RelativeRect.getHeight() - s, RelativeRect.getWidth() - s, RelativeRect.getHeight()), this, 1);
    ScrollBarH->setSubElement(true);
    ScrollBarH->setPos( 0 );
    
    rootNode = new CGUINode(Environment, this, -1, true);
    rootNode->setParent(0);
    
    totalWidth = ScrollBarV->getRelativePosition().getHeight();
    totalHeight = ScrollBarH->getRelativePosition().getWidth();
    
    ScrollBarV->setMax(totalHeight);
    ScrollBarH->setMax(totalWidth);
    
    previousHeight = 0;
    previousWidth = 0;
    
    isMoving = false;
    lastMousePosition = device->getCursorControl()->getPosition();
}


//! destructor
CGUINodesEditor::~CGUINodesEditor()
{
    
}

//! called if an event happened.
bool CGUINodesEditor::OnEvent(const SEvent& event)
{
	if (!isEnabled())
		return IGUIElement::OnEvent(event);

	switch(event.EventType) {
            
        case EET_MOUSE_INPUT_EVENT:
            if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN) {
                if (Environment->hasFocus(this) &&
                    !AbsoluteClippingRect.isPointInside(core::position2d<s32>(event.MouseInput.X, event.MouseInput.Y)))
                {
                    Environment->removeFocus(this);
                    return false;
                }
                Environment->setFocus(this);
                isMoving = true;
                return true;
            }
            if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP) {
                isMoving = false;                
                lastMousePosition = device->getCursorControl()->getPosition();
                return true;
            }
            if (event.MouseInput.Event == EMIE_MOUSE_MOVED) {
                /*if (isMoving) {
                    core::array<CGUINode *> nodes;
                    this->getArrayOfNodes(rootNode, nodes);
                    
                    s32 xstep = 0;
                    s32 ystep = 0;
                    if (device->getCursorControl()->getPosition().X < lastMousePosition.X)
                        xstep = -(lastMousePosition.X - device->getCursorControl()->getPosition().X);
                    
                    if (device->getCursorControl()->getPosition().Y < lastMousePosition.Y)
                        ystep = -(lastMousePosition.Y - device->getCursorControl()->getPosition().Y);
                    
                    for (u32 i=0; i < nodes.size(); i++)
                        nodes[i]->getInterface()->setRelativePosition(position2di(nodes[i]->getInterface()->getRelativePosition().UpperLeftCorner.X + xstep,
                                                                                  nodes[i]->getInterface()->getRelativePosition().UpperLeftCorner.Y + ystep));

                    
                    lastMousePosition = device->getCursorControl()->getPosition();
                }*/
                return true;
            }
		break;
            
        case EET_GUI_EVENT: {
            if (event.GUIEvent.EventType == EGET_SCROLL_BAR_CHANGED) {
                if (event.GUIEvent.Caller == ScrollBarH || event.GUIEvent.Caller == ScrollBarV) {
                    core::array<CGUINode *> nodes;
                    this->getArrayOfNodes(rootNode, nodes);

                    u32 stepH = (ScrollBarH->getPos() < previousWidth ? -(previousWidth-ScrollBarH->getPos()) : previousWidth-ScrollBarH->getPos());
                    u32 stepV = (ScrollBarV->getPos() < previousHeight ? -(previousHeight-ScrollBarV->getPos()) : previousHeight-ScrollBarV->getPos());
                    
                    for (u32 i=0; i < nodes.size(); i++) {
                        nodes[i]->getInterface()->setRelativePosition(position2di(nodes[i]->getInterface()->getRelativePosition().UpperLeftCorner.X-stepH,
                                                                                  nodes[i]->getInterface()->getRelativePosition().UpperLeftCorner.Y-stepV)
                                                                      );
                    }
                    previousWidth = ScrollBarH->getPos();
                    previousHeight = ScrollBarV->getPos();
                }
            }
            
            core::array<CGUINode *> nodes;
            this->getArrayOfNodes(rootNode, nodes);
            for (u32 i=0; i < nodes.size(); i++)
                nodes[i]->OnEvent(event);
        }
            break;
            
        default:break;
	}

	return Parent ? Parent->OnEvent(event) : false;
}

//! draws the element and its children
void CGUINodesEditor::draw()
{
	if( !IsVisible )
	{
		return;
	}
    
	IGUISkin* skin = Environment->getSkin();
	irr::video::IVideoDriver* driver = Environment->getVideoDriver();
    
	core::rect<s32>* clipRect = 0;
	if (clip)
	{
		clipRect = &AbsoluteClippingRect;
	}
    
	// draw background
	core::rect<s32> frameRect( AbsoluteRect );
    
    driver->draw2DRectangle( skin->getColor( EGDC_3D_HIGH_LIGHT ), frameRect,
                            clipRect );
    
	// draw the border
	frameRect.LowerRightCorner.Y = frameRect.UpperLeftCorner.Y + 1;
	driver->draw2DRectangle( skin->getColor( EGDC_3D_SHADOW ), frameRect,
                            clipRect );
    
	frameRect.LowerRightCorner.Y = AbsoluteRect.LowerRightCorner.Y;
	frameRect.LowerRightCorner.X = frameRect.UpperLeftCorner.X + 1;
	driver->draw2DRectangle( skin->getColor( EGDC_3D_SHADOW ), frameRect,
                            clipRect );
    
	frameRect = AbsoluteRect;
	frameRect.UpperLeftCorner.X = frameRect.LowerRightCorner.X - 1;
	driver->draw2DRectangle( skin->getColor( EGDC_3D_HIGH_LIGHT ), frameRect,
                            clipRect );
    
	frameRect = AbsoluteRect;
	frameRect.UpperLeftCorner.Y = AbsoluteRect.LowerRightCorner.Y - 1;
	frameRect.LowerRightCorner.Y = AbsoluteRect.LowerRightCorner.Y;
	driver->draw2DRectangle( skin->getColor( EGDC_3D_HIGH_LIGHT ), frameRect,
                            clipRect );
    
    
	// draw items
	core::rect<s32> clientClip( AbsoluteRect );
	clientClip.UpperLeftCorner.Y += 1;
	clientClip.UpperLeftCorner.X += 1;
	clientClip.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X;
	clientClip.LowerRightCorner.Y -= 1;
    
	if ( ScrollBarV )
		clientClip.LowerRightCorner.X -= skin->getSize( EGDS_SCROLLBAR_SIZE );
	if ( ScrollBarH )
		clientClip.LowerRightCorner.Y -= skin->getSize( EGDS_SCROLLBAR_SIZE );
    
	if( clipRect )
	{
		clientClip.clipAgainst( *clipRect );
	}
    
	frameRect = AbsoluteRect;
	frameRect.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X - skin->getSize( EGDS_SCROLLBAR_SIZE );
	//frameRect.LowerRightCorner.Y = AbsoluteRect.UpperLeftCorner.Y + ItemHeight;
    
	if ( ScrollBarV )
	{
		frameRect.UpperLeftCorner.Y -= ScrollBarV->getPos();
		frameRect.LowerRightCorner.Y -= ScrollBarV->getPos();
	}
    
	if ( ScrollBarH )
	{
		frameRect.UpperLeftCorner.X -= ScrollBarH->getPos();
		frameRect.LowerRightCorner.X -= ScrollBarH->getPos();
	}
    
    //Draw grid
    frameRect = AbsoluteClippingRect;
    frameRect.UpperLeftCorner.X += 1;
    frameRect.UpperLeftCorner.Y += 1;
    u32 relativeGridPosition = (10 * frameRect.getHeight())/500;
    for (u32 i=0; relativeGridPosition*i < frameRect.getHeight() - skin->getSize(EGDS_SCROLLBAR_SIZE); i+= relativeGridPosition) {
        driver->draw2DLine(core::vector2d<s32>(frameRect.UpperLeftCorner.X, frameRect.UpperLeftCorner.Y + relativeGridPosition*i),
                           core::vector2d<s32>(frameRect.UpperLeftCorner.X + frameRect.getWidth() - skin->getSize(EGDS_SCROLLBAR_SIZE), frameRect.UpperLeftCorner.Y + relativeGridPosition*i),
                           video::SColor(64, 255, 255, 255));
    }
    for (u32 i=0; relativeGridPosition*i < frameRect.getWidth() - skin->getSize(EGDS_SCROLLBAR_SIZE); i+= relativeGridPosition) {
        driver->draw2DLine(core::vector2d<s32>(frameRect.UpperLeftCorner.X + relativeGridPosition*i, frameRect.UpperLeftCorner.Y),
                           core::vector2d<s32>(frameRect.UpperLeftCorner.X + relativeGridPosition*i, frameRect.UpperLeftCorner.Y + frameRect.getHeight() - skin->getSize(EGDS_SCROLLBAR_SIZE)),
                           video::SColor(32, 255, 255, 255));
    }
    
    drawLinksRecursively(rootNode);

	IGUIElement::draw();
}

void CGUINodesEditor::drawLinksRecursively(CGUINode *node) {
    core::list<CGUINode *>::ConstIterator it = node->getChildren().begin();
    for (; it != node->getChildren().end(); ++it) {
        if ((*it)->getChildren().size() != 0) {
            
            if ((*it) != rootNode) {
                core::list<CGUINode *>::ConstIterator childNode = (*it)->getChildren().begin();
                for (; childNode != (*it)->getChildren().end(); ++childNode) {
                    core::rect<s32> absPos = (*childNode)->getInterface()->getAbsoluteClippingRect();
                    core::rect<s32> itAbsPos = (*it)->getInterface()->getAbsoluteClippingRect();
                    
                    Environment->getVideoDriver()->draw2DLine(core::vector2di(absPos.UpperLeftCorner.X, absPos.UpperLeftCorner.Y + absPos.getHeight()/2), 
                                                              core::vector2di(itAbsPos.UpperLeftCorner.X + itAbsPos.getWidth(), itAbsPos.UpperLeftCorner.Y + itAbsPos.getHeight()/2),
                                                              SColor(255, 255, 255, 255));
                }
            }
            
            drawLinksRecursively((*it));
        }
    }
}

void CGUINodesEditor::setRelativePosition(const core::rect<s32>& r)
{
    IGUIElement::setRelativePosition(r);
    
    IGUISkin* skin = Environment->getSkin();
    s32 s = skin->getSize( EGDS_SCROLLBAR_SIZE );
    
    ScrollBarV->setRelativePosition(core::rect<s32>(RelativeRect.getWidth() - s, 0, RelativeRect.getWidth(),RelativeRect.getHeight() - s));
    ScrollBarH->setRelativePosition(core::rect<s32>(0, RelativeRect.getHeight() - s, RelativeRect.getWidth() - s, RelativeRect.getHeight()));
}

//! Reads attributes of the element
void CGUINodesEditor::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0)
{
	updateAbsolutePosition();
}

void CGUINodesEditor::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const {
    
}
