/*
 *  UIContextMenu.h
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __C_GUI_NODES_EDITOR_H_INCLUDED__
#define __C_GUI_NODES_EDITOR_H_INCLUDED__

//#include <irrlicht.h>
#include "../../Device/CDevices.h"

#include "CGUINode.h"

class CGUINodesEditor : public IGUIElement
{
public:
    
    //---------------------------------------------------------------------------------------------
    //-----------------------------------------GUI ELEMENT-----------------------------------------
    //---------------------------------------------------------------------------------------------

    //! constructor
    CGUINodesEditor(irr::IrrlichtDevice *_devices, IGUIElement* parent,
                    s32 id, core::rect<s32> rectangle, bool noclip=false);

    //! destructor
    virtual ~CGUINodesEditor();

    //! called if an event happened.
    virtual bool OnEvent(const SEvent& event);

    //! draws the element and its children
    virtual void draw();
    
    void setRelativePosition(const core::rect<s32>& r);

    //! Writes attributes of the element.
    virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const;

    //! Reads attributes of the element
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);
    
    //! returns the irrlicht device
    irr::IrrlichtDevice *getDevice() { return device; }
    
    //---------------------------------------------------------------------------------------------
    //-----------------------------------------DATAS-----------------------------------------------
    //---------------------------------------------------------------------------------------------
    
    //-----------------------------------
	//NODES
    CGUINode *getRootNode() { return rootNode; }
    
    void addNode(CGUINode *node);
    void getArrayOfNodes(CGUINode *node, irr::core::array<CGUINode *> &outNodes);

    CGUINode *getSelectedNode();

	void removeNode(CGUINode *node);
    //-----------------------------------
    
    //-----------------------------------
	//GUI ELEMENTS
    void drawLinksRecursively(CGUINode *node);
    //-----------------------------------

private:
    
    //-----------------------------------
	//GUI ELEMENTS
    irr::gui::IGUIScrollBar *ScrollBarV, *ScrollBarH;
    
    void recalculateTotals();
    irr::u32 totalHeight, totalWidth;
    irr::u32 previousHeight, previousWidth;
    //-----------------------------------
    
    //-----------------------------------
	//DATAS
    irr::IrrlichtDevice *device;
    CGUINode *rootNode;
    bool clip;
    
    bool isMoving;
    core::vector2di lastMousePosition;
    //-----------------------------------
};

#endif

