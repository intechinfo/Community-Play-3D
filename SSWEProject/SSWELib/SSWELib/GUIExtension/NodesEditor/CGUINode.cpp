/*
 *  UIContextMenu.h
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include "CGUINode.h"

#include "CGUINodesEditor.h"

//! constructor
CGUINode::CGUINode(IGUIEnvironment* environment, CGUINodesEditor *_editor, s32 id, bool isRootNode, CGUINode *rootNode) {
    gui = environment;
    isRoot = isRootNode;
    
    parentNode = 0;
    
    dataType = -1;
    data = 0;
    window = 0;
    
    editor = _editor;
    
    name = "new node";
}

CGUINode::~CGUINode() {
    
}

void CGUINode::addTextField(stringw name, stringw value) {
    if (window) {
        IGUIStaticText *t = gui->addStaticText(name.c_str(), rect<s32>(1, 20, 80, 40), true, true, window, -1, true);
        IGUIEditBox *y = gui->addEditBox((value != L"" ? value.c_str() : L""), rect<s32>(80, 20, 180, 40), true, window, -1);
        y->setName(name.c_str());
        
        this->calculateElementPosition(t);
        this->calculateElementPosition(y, true);
    }
}

void CGUINode::addCheckBox(stringw name, bool checked) {
    if (window) {
        IGUICheckBox *cb = gui->addCheckBox(checked, rect<s32>(1, 40, 180, 60), window, -1, name.c_str());
        cb->setName(name.c_str());
        
        this->calculateElementPosition(cb);
    }
}

void CGUINode::addButton(stringw name, stringw tooltiptext) {
    if (window) {
        IGUIButton *b = gui->addButton(rect<s32>(1, 60, 179, 80), window, -1, name.c_str(), tooltiptext.c_str());
        b->setName(name.c_str());
        
        this->calculateElementPosition(b);
    }
}

void CGUINode::addVector3DFields(stringw name, vector3df v) {
    if (window) {
        IGUIStaticText *t = gui->addStaticText(L"", rect<s32>(0, 80, 180, 160), true, true, window, -1, true);
        t->setName(name.c_str());
        this->calculateElementPosition(t);
        
        gui->addStaticText(name.c_str(), rect<s32>(1, 0, 179, 20), true, true, t, -1, true);
        
        gui->addStaticText(L"X :", rect<s32>(10, 20, 40, 40), true, true, t, -1, true);
        gui->addStaticText(L"Y :", rect<s32>(10, 40, 40, 60), true, true, t, -1, true);
        gui->addStaticText(L"Z :", rect<s32>(10, 60, 40, 80), true, true, t, -1, true);
        
        gui->addEditBox(stringw(v.X).c_str(), rect<s32>(40, 20, 180, 40), true, t, -1)->setName(stringw(name + "X"));
        gui->addEditBox(stringw(v.Y).c_str(), rect<s32>(40, 40, 180, 60), true, t, -1)->setName(stringw(name + "Y"));
        gui->addEditBox(stringw(v.Z).c_str(), rect<s32>(40, 60, 180, 80), true, t, -1)->setName(stringw(name + "Z"));
    }
}

void CGUINode::addDimension2DFields(stringw name, vector2df v) {
    if (window) {
        IGUIStaticText *t = gui->addStaticText(L"", rect<s32>(1, 160, 180, 220), true, true, window, -1, true);
        t->setName(name.c_str());
        this->calculateElementPosition(t);
        
        gui->addStaticText(name.c_str(), rect<s32>(1, 0, 179, 20), true, true, t, -1, true);
        
        gui->addStaticText(L"W :", rect<s32>(10, 20, 40, 40), true, true, t, -1, true);
        gui->addStaticText(L"H :", rect<s32>(10, 40, 40, 60), true, true, t, -1, true);
        
        gui->addEditBox(stringw(v.X).c_str(), rect<s32>(40, 20, 180, 40), true, t, -1)->setName(stringw(name + "W"));
        gui->addEditBox(stringw(v.Y).c_str(), rect<s32>(40, 40, 180, 60), true, t, -1)->setName(stringw(name + "H"));
    }
}

void CGUINode::add2ParametersFields(stringw name, stringw param1, stringw param2, f32 value1, f32 value2) {
    if (window) {
        IGUIStaticText *t = gui->addStaticText(L"", rect<s32>(1, 160, 180, 220), true, true, window, -1, true);
        t->setName(name.c_str());
        this->calculateElementPosition(t);
        
        gui->addStaticText(name.c_str(), rect<s32>(1, 0, 179, 20), true, true, t, -1, true);
        
        gui->addStaticText(param1.c_str(), rect<s32>(10, 20, 40, 40), true, true, t, -1, true);
        gui->addStaticText(param2.c_str(), rect<s32>(10, 40, 40, 60), true, true, t, -1, true);
        
        gui->addEditBox(stringw(value1).c_str(), rect<s32>(40, 20, 180, 40), true, t, -1)->setName(stringw(name + param1).c_str());
        gui->addEditBox(stringw(value2).c_str(), rect<s32>(40, 40, 180, 60), true, t, -1)->setName(stringw(name + param2).c_str());
    }
}

IGUIListBox *CGUINode::addListBoxField(stringw name) {
    if (window) {
        IGUIStaticText *t = gui->addStaticText(L"", rect<s32>(1, 220, 180, 380), true, true, window, -1, true);
        t->setName(name.c_str());
        this->calculateElementPosition(t);
        
        gui->addStaticText(name.c_str(), rect<s32>(1, 0, 180, 20), true, true, t, -1, true);
        
        IGUIButton *edit = gui->addButton(rect<s32>(1, 20, 100, 40), t, -1, L"Edit", L"Edit selected element");
        edit->setName(stringc(name + "edit").c_str());
        IGUIButton *add = gui->addButton(rect<s32>(160, 20, 180, 40), t, -1, L"+", L"Add a new element");
        edit->setName(stringc(name + "+").c_str());
        IGUIButton *remove = gui->addButton(rect<s32>(140, 20, 160, 40), t, -1, L"-", L"Remove selected element");
        edit->setName(stringc(name + "-").c_str());
        
        IGUIListBox *listb = gui->addListBox(rect<s32>(1, 40, 180, 160), t, -1, true);
        listb->setName(stringc(name + "list").c_str());
        
        return listb;
    }
    
    return 0;
}

IGUIComboBox *CGUINode::addComboBox(stringw name) {
    if (window) {
        IGUIStaticText *t = gui->addStaticText(L"", rect<s32>(1, 380, 180, 420), true, true, window, -1, true);
        t->setName(name.c_str());
        this->calculateElementPosition(t);
        
        gui->addStaticText(name.c_str(), rect<s32>(1, 0, 180, 20), true, true, t, -1, true);
        
		rect<s32> normalPos = t->getRelativePosition();
		IGUIComboBox *c = gui->addComboBox(rect<s32>(normalPos.UpperLeftCorner.X+1, normalPos.UpperLeftCorner.Y+20, 
													 normalPos.UpperLeftCorner.X+180, normalPos.UpperLeftCorner.Y+40),
										   this->window, -1);
        c->setName(stringc(name + "cb").c_str());
        
        return c;
        
    }
    
    return 0;
}

void CGUINode::addCustomElement(IGUIElement *element) {
    if (window) {
        window->addChild(element);
        
        this->calculateElementPosition(element);
    }
}

void CGUINode::calculateElementPosition(gui::IGUIElement *element, bool samePosition) {
    core::list<IGUIElement *>::ConstIterator it = window->getChildren().begin();
    
    u32 maxY = 0;
    s32 lastLowerCornerY = 0;
    for (; it != window->getChildren().end() && (*it) != element; ++it) {
        if (lastLowerCornerY < (*it)->getRelativePosition().LowerRightCorner.Y) {
            maxY += (*it)->getRelativePosition().getHeight();
            lastLowerCornerY = (*it)->getRelativePosition().LowerRightCorner.Y;
        }
    }
    
    element->setRelativePosition(position2di(element->getRelativePosition().UpperLeftCorner.X, maxY+5 - (samePosition ? element->getRelativePosition().getHeight() : 0) ));
    
    if (window->getRelativePosition().getHeight()-20 < element->getRelativePosition().LowerRightCorner.Y) {
        window->setRelativePosition(rect<s32>(window->getRelativePosition().UpperLeftCorner.X,
                                              window->getRelativePosition().UpperLeftCorner.Y,
                                              window->getRelativePosition().LowerRightCorner.X,
                                              window->getRelativePosition().LowerRightCorner.Y + element->getRelativePosition().getHeight()));
    }
}

void CGUINode::addChild(CGUINode *node) {
    children.push_back(node);
}

void CGUINode::createChild() {
    CGUINode *node = new CGUINode(gui, editor, -1);
    node->setParent(this);
    editor->addNode(node);
}

void CGUINode::createInterface(IGUIElement *parentElement) {
    window = gui->addWindow(rect<s32>(490, 60, 670, 140), false, stringw(name).c_str(), editor, -1);
    window->setRelativePosition(position2di(10, 10));
}

void CGUINode::destroyInterface() {
	window->remove();
	window = 0;
}

void CGUINode::removeChild(CGUINode *node, bool firstElement) {
	if (!firstElement)
		node->getInterface()->remove();
	core::list<CGUINode*>::Iterator it = children.begin();
	for (; it != children.end(); ++it) {
		if (*it == node) {
			(*it)->removeAll();
			children.erase(it);
			break;
		}
	}
	delete node;
}

void CGUINode::removeAll() {
	core::list<CGUINode*>::Iterator it = children.begin();
	for (; it != children.end(); ++it) {
		(*it)->removeAll();
		(*it)->destroyInterface();

		SEvent ev;
		ev.EventType = EET_GUI_EVENT;
		ev.GUIEvent.EventType = (EGUI_EVENT_TYPE)ESE_GRAPH_NODE_REMOVED;
		ev.GUIEvent.Caller = editor;
		ev.GUIEvent.Element = (IGUIElement*)*it;
		editor->getDevice()->getEventReceiver()->OnEvent(ev);

		delete *it;
	}
	children.clear();
}

bool CGUINode::OnEvent(const SEvent& event) {
    
    if (event.EventType == EET_GUI_EVENT) {
        
        bool test = (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED ||
                     event.GUIEvent.EventType == EGET_BUTTON_CLICKED ||
                     event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED ||
                     event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED
                     );
        
        if (window && event.GUIEvent.Caller != 0 && test)
        {
            IGUIElement *element = event.GUIEvent.Caller;
            IGUIElement *selement = element;
            while (selement != window->getParent() && selement->getParent() != gui->getRootGUIElement()) {
                if (selement == window) {
                    SEvent ev;
                    ev.EventType = EET_GUI_EVENT;
                    ev.GUIEvent.EventType = (EGUI_EVENT_TYPE)ESE_GRAPH_NODE_CHANGED;
                    ev.GUIEvent.Caller = editor;
                    ev.GUIEvent.Element = element;
                    gui->setFocus(element);
                    
                    editor->getDevice()->getEventReceiver()->OnEvent(ev);
					
                    break;
                }
                selement = selement->getParent();
            }
        }

		if (window && event.GUIEvent.Caller != 0 && event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window) {
				SEvent ev;
				ev.EventType = EET_GUI_EVENT;
				ev.GUIEvent.EventType = (EGUI_EVENT_TYPE)ESE_GRAPH_NODE_REMOVED;
				ev.GUIEvent.Caller = ev.GUIEvent.Caller = editor;
				ev.GUIEvent.Element = (IGUIElement*)this;

				editor->getDevice()->getEventReceiver()->OnEvent(ev);

				if (parentNode)
					parentNode->removeChild(this, true);
			}
		}
    }
    
    return false;
}
