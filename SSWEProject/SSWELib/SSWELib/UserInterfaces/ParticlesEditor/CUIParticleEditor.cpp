/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "stdafx.h"
#include "CUIParticleEditor.h"

CUIParticleEditor::CUIParticleEditor(CDevices *_devices, SParticleSystem *_ps) {
    
    devices = _devices;
    ps = _ps;
    
    IGUIEnvironment *gui = devices->getGUIEnvironment();
    
	window = gui->addWindow(rect<s32>(600, 200, 1550, 760), false, L"Particle System Editor", 0, -1);
    window->getMinimizeButton()->setVisible(true);
    window->getMaximizeButton()->setVisible(true);
    devices->getCore()->centerWindow(window, devices->getVideoDriver()->getScreenSize());
    
    gui->addMenu(window, -1)->addItem(L"PARTICLES EDITOR");
    
    nodesEditor = new CGUINodesEditor(devices->getDevice(), window, -1, rect<s32>(10, 70, 940, 420));
    
    devices->getEventReceiver()->AddEventReceiver(this, window);
    
    SEvent event;
    event.EventType = EET_GUI_EVENT;
    event.GUIEvent.EventType = EGET_BUTTON_CLICKED;
    event.GUIEvent.Caller = window->getMaximizeButton();
    OnEvent(event);
    
    //----------FILL FROM PS-----------------
    
    for (u32 i=0; i < ps->getSystems()->size(); i++) {
        SPK::System *system = ps->getSystems()->operator[](i);
        
        CGUINode *node = new CGUINode(devices->getGUIEnvironment(), nodesEditor, -1);
        node->setName(ps->getSystems()->operator[](i)->getName().c_str());
        node->setParent(0);
        node->setData(system);
        node->setDataType(EPSDT_SYSTEM);
        nodesEditor->addNode(node);
        node->addTextField(L"Name :", stringw(node->getName()).c_str());
        node->addCheckBox(L"AA-BB Computing", system->isAABBComputingEnabled());
        node->addButton(L"Add Group", L"Adds a group to this system");
        
        for (u32 j=0; j < system->getGroups().size(); j++) {
            SPK::Group *group = system->getGroups()[j];
            
            CGUINode *node2 = new CGUINode(devices->getGUIEnvironment(), nodesEditor, -1);
            node2->setName(group->getName().c_str());
            node2->setParent(node);
            node2->setData(group);
            node2->setDataType(EPSDT_GROUP);
            nodesEditor->addNode(node2);
            node2->addTextField(L"Name :", stringw(node2->getName()).c_str());
            node2->addVector3DFields(L"Gravity", devices->getCore()->getVector3dfFromSpark(group->getGravity()));
            node2->addCheckBox(L"AA-BB Computing", group->isAABBComputingEnabled());
            
            CGUINode *nodeModel = new CGUINode(devices->getGUIEnvironment(), nodesEditor, -1);
            nodeModel->setName(group->getModel()->getName().c_str());
            nodeModel->setParent(node2);
            nodeModel->setData(group->getModel());
            nodeModel->setDataType(EPSDT_MODEL);
            nodesEditor->addNode(nodeModel);
            nodeModel->addTextField(L"Name :", stringw(nodeModel->getName()).c_str());
            nodeModel->add2ParametersFields(L"Life Time", L"Min", L"Max", group->getModel()->getLifeTimeMin(), group->getModel()->getLifeTimeMax());
            
            CGUINode *nodeRenderer = new CGUINode(devices->getGUIEnvironment(), nodesEditor, -1);
            nodeRenderer->setName(group->getRenderer()->getName().c_str());
            nodeRenderer->setParent(node2);
            nodeRenderer->setData(group->getRenderer());
            nodeRenderer->setDataType(EPSDT_RENDERER);
            nodesEditor->addNode(nodeRenderer);
            nodeRenderer->addTextField(L"Name :", stringw(nodeRenderer->getName()).c_str());
            
            for (u32 k=0; k < group->getEmitters().size(); k++) {
                SPK::Emitter *emitter = group->getEmitters()[k];
                
                CGUINode *node3 = new CGUINode(devices->getGUIEnvironment(), nodesEditor, -1);
                node3->setName(emitter->getName().c_str());
                node3->setParent(node2);
                node3->setData(emitter);
                node3->setDataType(EPSDT_EMITTER);
                nodesEditor->addNode(node3);
                node3->addTextField(L"Name :", stringw(node3->getName()).c_str());
            }
        }
    }
    
    //----------FILL FROM PS-----------------

}

CUIParticleEditor::~CUIParticleEditor() {
    
}

bool CUIParticleEditor::OnEvent(const SEvent &event) {

    if (event.EventType == EET_GUI_EVENT) {
        //WINDOW
        if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
            if (event.GUIEvent.Caller == window) {
                devices->getEventReceiver()->RemoveEventReceiver(this);
                delete this;
            }
        }
        
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            if (event.GUIEvent.Caller == window->getMaximizeButton()) {
                devices->getCore()->maximizeWindow(window, rect<s32>(600, 200, 1550, 760));
                nodesEditor->setRelativePosition(rect<s32>(10, 70, window->getRelativePosition().getWidth()-10, window->getRelativePosition().getHeight()-200));
            }
        }
        
        if (event.GUIEvent.EventType == (EGUI_EVENT_TYPE)ESE_GRAPH_NODE_CHANGED) {
            if (event.GUIEvent.Caller == nodesEditor) {
                CGUINode *node = nodesEditor->getSelectedNode();
                if (node) {
                    E_PS_DATA_TYPE psdt = (E_PS_DATA_TYPE)node->getDataType();
                    if (psdt == EPSDT_SYSTEM) {
                        stringc name = event.GUIEvent.Element->getName();
                        SPK::System *system = (SPK::System*)node->getData();
                        if (name == "Name :") {
                            system->setName(stringc(event.GUIEvent.Element->getText()).c_str());
                            node->getInterface()->setText(event.GUIEvent.Element->getText());
                        }
                        if (name == "AA-BB Computing") {
                            system->enableAABBComputing(((IGUICheckBox*)event.GUIEvent.Element)->isChecked());
                        }
                        if (name == "Add Group") {
                            //SELECT ADD GROUP
                        }
                    }
                }
            }
        }
    }
    
	return false;
}
