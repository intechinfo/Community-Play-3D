/*
 *  UIContextMenu.h
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __C_GUI_NODE_H_INCLUDED__
#define __C_GUI_NODE_H_INCLUDED__

#include <irrlicht.h>

#include <ISSWEEvents.h>

class CGUINodesEditor;

class CGUINode : public irr::IEventReceiver
{
public:

    //---------------------------------------------------------------------------------------------
    //-----------------------------------------GUI ELEMENT-----------------------------------------
    //---------------------------------------------------------------------------------------------

    //! constructor
    CGUINode(irr::gui::IGUIEnvironment* environment, CGUINodesEditor *_editor, irr::s32 id, bool isRootNode = false, CGUINode *rootNode=0);
    ~CGUINode();

    bool OnEvent(const irr::SEvent& event);

    //---------------------------------------------------------------------------------------------
    //--------------------------------------------DATAS--------------------------------------------
    //---------------------------------------------------------------------------------------------

    void addChild(CGUINode *node);
    void createChild();

    //-----------------------------------
	//DATAS
    void *getData() { return data; }
    void setData(void *_data) { data = _data; }

    irr::s32 getDataType() { return dataType; }
    void setDataType(irr::s32 _dataType) { dataType = _dataType; }

    CGUINode *getParent() { return parentNode; }
    void setParent(CGUINode *_parent) { parentNode = _parent; }

    irr::core::list<CGUINode *> &getChildren() { return children; }
    //-----------------------------------

    //-----------------------------------
	//GUI ELEMENTS
    void createInterface(irr::gui::IGUIElement *parentElement);
    irr::gui::IGUIElement *getInterface() { return window; }

    void setName(irr::core::stringc _name) { name = _name; }
    irr::core::stringc getName() { return name; }

    //ELEMENTS TO ADD
    void addTextField(irr::core::stringw name, irr::core::stringw value);
    void addCheckBox(irr::core::stringw name, bool checked);

    void addButton(irr::core::stringw name, irr::core::stringw tooltiptext = L"");

    void addVector3DFields(irr::core::stringw name, irr::core::vector3df v);
    void addDimension2DFields(irr::core::stringw name, irr::core::vector2df v);

    irr::gui::IGUIListBox *addListBoxField(irr::core::stringw name);

    void add2ParametersFields(irr::core::stringw name, irr::core::stringw param1, irr::core::stringw param2, irr::f32 value1, irr::f32 value2);

    irr::gui::IGUIComboBox *addComboBox(irr::core::stringw name);

    void addCustomElement(irr::gui::IGUIElement *element);
    //-----------------------------------

private:

    //-----------------------------------
	//GUI
    irr::gui::IGUIEnvironment *gui;
    //-----------------------------------

    //-----------------------------------
	//DATAS
    irr::s32 dataType;
    void *data;

    CGUINode *parentNode;
    irr::core::list<CGUINode *> children;

    bool isRoot;

    CGUINodesEditor *editor;
    //-----------------------------------

    //-----------------------------------
	//GUI ELEMENTS
    irr::gui::IGUIWindow *window;
    irr::core::stringc name;

    void calculateElementPosition(irr::gui::IGUIElement *element, bool samePosition=false);
    //-----------------------------------

};


#endif

