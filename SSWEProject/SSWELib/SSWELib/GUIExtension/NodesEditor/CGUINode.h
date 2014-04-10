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
    CGUINode(IGUIEnvironment* environment, CGUINodesEditor *_editor, s32 id, bool isRootNode = false, CGUINode *rootNode=0);
    ~CGUINode();

    bool OnEvent(const SEvent& event);
    
    //---------------------------------------------------------------------------------------------
    //--------------------------------------------DATAS--------------------------------------------
    //---------------------------------------------------------------------------------------------
    
    void addChild(CGUINode *node);
    void createChild();

	void removeChild(CGUINode *node);
	void removeAll();
    
    //-----------------------------------
	//DATAS
    void *getData() { return data; }
    void setData(void *_data) { data = _data; }

    s32 getDataType() { return dataType; }
    void setDataType(s32 _dataType) { dataType = _dataType; }

	s32 getDataType2() { return dataType2; }
	void setDataType2(s32 _dataType2) { dataType2 = _dataType2; }

    CGUINode *getParent() { return parentNode; }
    void setParent(CGUINode *_parent) { parentNode = _parent; }

    irr::core::list<CGUINode *> &getChildren() { return children; }
    //-----------------------------------
    
    //-----------------------------------
	//GUI ELEMENTS
    void createInterface(IGUIElement *parentElement);
    irr::gui::IGUIElement *getInterface() { return window; }
	void destroyInterface();
    
    void setName(irr::core::stringc _name) { name = _name; }
    irr::core::stringc getName() { return name; }
    
    //ELEMENTS TO ADD
    void addTextField(stringw name, core::stringw value);
    void addCheckBox(stringw name, bool checked);
    
    void addButton(stringw name, stringw tooltiptext = L"");
    
    void addVector3DFields(stringw name, vector3df v);
    void addDimension2DFields(stringw name, vector2df v);
    
    IGUIListBox *addListBoxField(stringw name);
    
    void add2ParametersFields(stringw name, stringw param1, stringw param2, f32 value1, f32 value2);
    
    IGUIComboBox *addComboBox(stringw name);
    
    void addCustomElement(IGUIElement *element);
    //-----------------------------------

private:
    
    //-----------------------------------
	//GUI
    irr::gui::IGUIEnvironment *gui;
    //-----------------------------------
    
    //-----------------------------------
	//DATAS
    s32 dataType;
	s32 dataType2;
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
    
    void calculateElementPosition(gui::IGUIElement *element, bool samePosition=false);
    //-----------------------------------
    
};


#endif

