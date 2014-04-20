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

#include "CUIEditParticleFlags.h"
#include "CUIEditParticleInterpolators.h"
#include "CUIParticlesEditZone.h"
#include "CUIAddEmitter.h"
#include "CUIAddModifier.h"

CUIParticleEditor::CUIParticleEditor(CDevices *_devices, SParticleSystem *_ps) {
    
    devices = _devices;
    ps = _ps;
    
    IGUIEnvironment *gui = devices->getGUIEnvironment();
    
	window = gui->addWindow(rect<s32>(600, 200, 1550, 760), false, L"Particle System Editor", 0, -1);
    window->getMinimizeButton()->setVisible(true);
    window->getMaximizeButton()->setVisible(true);
    devices->getCore()->centerWindow(window, devices->getVideoDriver()->getScreenSize());
    
    menu = gui->addMenu(window, -1);
    menu->addItem(L"Particles Editor", -1, false);
    menu->addItem(L"File", -1, true, true);
    menu->addItem(L"Edit", -1, true, true);
    menu->addItem(L"Help", -1, true, true);
    
    u32 i=1;
    IGUIContextMenu *submenu = menu->getSubMenu(i++);
    submenu->addItem(L"Open saved configuration...");
    submenu->addItem(L"Save Configuration");
    submenu->addItem(L"Save Configuration As...");
    submenu->addSeparator();
    submenu->addItem(L"Concat With An Other Saved Configuration...");
    submenu->addSeparator();
    submenu->addItem(L"Close");
    
    submenu = menu->getSubMenu(i++);
    submenu->addItem(L"Create New System...");
    submenu->addItem(L"Create New Model...");
    submenu->addItem(L"Create Renderer...");
    submenu->addSeparator();
    submenu->addItem(L"Edit Script...");
    submenu->addSeparator();
    submenu->addItem(L"Clear Selected System");
    submenu->addItem(L"Clear Elements Of Selected Node");
    submenu->addItem(L"Clear All");
	submenu->addItem(L"Reset Systems");
    
    submenu = menu->getSubMenu(i++);
    submenu->addItem(L"How To...");
    submenu->addItem(L"SPARK documentation...");
    
    nodesEditor = new CGUINodesEditor(devices->getDevice(), window, -1, rect<s32>(10, 70, 940, 420));
    
    viewPort = new CGUIViewport(gui, window, 1, rect<s32>(10, 430, 310, 550)); 
    if (viewPort) {
		//viewPort->setScreenQuad(devices->getXEffect()->getScreenQuad());
        viewPort->setSceneManager(devices->getSceneManager());
		//viewPort->setRenderScreenQuad(true);
        viewPort->setOverrideColor(SColor(255, 0, 0, 0)); 
    }
    
    devices->getEventReceiver()->AddEventReceiver(this, window);
    
    SEvent event;
    event.EventType = EET_GUI_EVENT;
    event.GUIEvent.EventType = EGET_BUTTON_CLICKED;
    event.GUIEvent.Caller = window->getMaximizeButton();
    OnEvent(event);

	event.EventType = EET_MOUSE_INPUT_EVENT;
	event.MouseInput.Event = EMIE_LMOUSE_DOUBLE_CLICK;
	gui->setFocus(viewPort);
	OnEvent(event);
    
    //----------FILL FROM PS-----------------
    
    for (u32 i=0; i < ps->getSystems()->size(); i++) {
		SPK::IRR::IRRSystem *system = (SPK::IRR::IRRSystem*)ps->getSystems()->operator[](i);
        
        CGUINode *node = new CGUINode(devices->getGUIEnvironment(), nodesEditor, -1);
        node->setName(ps->getSystems()->operator[](i)->getName().c_str());
        node->setParent(0);
        node->setData(system);
        node->setDataType(EPSDT_SYSTEM);
        nodesEditor->addNode(node);
        node->addTextField(L"Name :", stringw(node->getName()).c_str());
        node->addCheckBox(L"AA-BB Computing", system->isAABBComputingEnabled());
		node->addCheckBox(L"Enable Auto-Update", system->isAutoUpdateEnabled());
		node->addCheckBox(L"Update Only if Visible", system->isUpdateOnlyWhenVisible());
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
            node2->addButton(L"Add Renderer");
            node2->addButton(L"Add Emitter");
			node2->addTextField(L"Friction", stringw(group->getFriction()).c_str());
			node2->addCheckBox(L"Enable Sorting", group->isSortingEnabled());
			node2->addCheckBox(L"Enable Distance Computation", group->isDistanceComputationEnabled());
			node2->addButton(L"Add Modifier...", L"Open dialog to add modifier (vortex, obstacle, etc)");
            
            CGUINode *nodeModel = new CGUINode(devices->getGUIEnvironment(), nodesEditor, -1);
            nodeModel->setName(group->getModel()->getName().c_str());
            nodeModel->setParent(node2);
            nodeModel->setData(group->getModel());
            nodeModel->setDataType(EPSDT_MODEL);
            nodesEditor->addNode(nodeModel);
			((IGUIWindow*)nodeModel->getInterface())->getCloseButton()->setVisible(false);
            nodeModel->addTextField(L"Name :", stringw(nodeModel->getName()).c_str());
            nodeModel->add2ParametersFields(L"Life Time ", L"Min", L"Max", group->getModel()->getLifeTimeMin(), group->getModel()->getLifeTimeMax());
            nodeModel->addButton(L"Edit Values...");
			nodeModel->addButton(L"Edit Interpolators...");

			if (group->getRenderer()) {
				SPK::IRR::IRRQuadRenderer *renderer = (SPK::IRR::IRRQuadRenderer*)group->getRenderer();
				CGUINode *nodeRenderer = new CGUINode(devices->getGUIEnvironment(), nodesEditor, -1);
				nodeRenderer->setName(group->getRenderer()->getName().c_str());
				nodeRenderer->setParent(node2);
				nodeRenderer->setData(group->getRenderer());
				nodeRenderer->setDataType(EPSDT_RENDERER);
				nodesEditor->addNode(nodeRenderer);
				nodeRenderer->addTextField(L"Name :", stringw(nodeRenderer->getName()).c_str());
				nodeRenderer->addDimension2DFields(L"Scale", vector2df(((SPK::IRR::IRRQuadRenderer *)group->getRenderer())->getScaleX(),
																	   ((SPK::IRR::IRRQuadRenderer *)group->getRenderer())->getScaleY())
												   );
				IGUIComboBox *texModecb = nodeRenderer->addComboBox(L"Texturing Mode");
				texModecb->addItem(L"Texturing None");
				texModecb->addItem(L"Texturing 2D");
				texModecb->addItem(L"Texturing 3D");
				texModecb->setSelected(((SPK::IRR::IRRQuadRenderer *)group->getRenderer())->getTexturingMode());
				IGUIComboBox *blendModecb = nodeRenderer->addComboBox(L"Blending Mode");
				blendModecb->addItem(L"Blending None");
				blendModecb->addItem(L"Blending Add");
				blendModecb->addItem(L"Blending Alpha");
				blendModecb->setSelected(((SPK::IRR::IRRQuadRenderer *)group->getRenderer())->getTexturingMode());
				nodeRenderer->addDimension2DFields(L"Atlas Dimension ", vector2df(((SPK::IRR::IRRQuadRenderer *)group->getRenderer())->getAtlasDimensions().Width,
																				 ((SPK::IRR::IRRQuadRenderer *)group->getRenderer())->getAtlasDimensions().Height)
												   );
				nodeRenderer->addButton(L"Configure Texture...");
				nodeRenderer->addCheckBox(L"Enable Alpha Test", renderer->isRenderingHintEnabled(SPK::ALPHA_TEST));
				nodeRenderer->addCheckBox(L"Enable Depth Test", renderer->isRenderingHintEnabled(SPK::DEPTH_TEST));
				nodeRenderer->addCheckBox(L"Enable Depth Write", renderer->isRenderingHintEnabled(SPK::DEPTH_WRITE));
				nodeRenderer->addCheckBox(L"Set Active", renderer->isActive());
				nodeRenderer->addTextField(L"Alpha test threshold", stringw(renderer->getAlphaTestThreshold()).c_str());
			}

            for (u32 k=0; k < group->getEmitters().size(); k++) {
                SPK::Emitter *emitter = group->getEmitters()[k];
                
                CGUINode *node3 = new CGUINode(devices->getGUIEnvironment(), nodesEditor, -1);
                node3->setName(emitter->getName().c_str());
                node3->setParent(node2);
                node3->setData(emitter);
                node3->setDataType(EPSDT_EMITTER);
                nodesEditor->addNode(node3);
                node3->addTextField(L"Name :", stringw(node3->getName()).c_str());
                node3->addTextField(L"Flow", stringw(emitter->getFlow()));
                node3->add2ParametersFields(L"Force ", L"Min", L"Max", emitter->getForceMin(), emitter->getForceMax());
                node3->addButton(L"Configure Zone...");
				node3->addCheckBox(L"Active", emitter->isActive());
				node3->addTextField(L"Tank",stringw(emitter->getTank()).c_str());
				node3->addTextField(L"Change Tank", stringw(emitter->getTank()).c_str());
				node3->addTextField(L"Flow", stringw(emitter->getFlow()).c_str());
				node3->addTextField(L"Change Flow", stringw(emitter->getFlow()).c_str());
            }
        }
    }
    
    //----------FILL FROM PS-----------------
    
    selectedNode = 0;
    
    paramNames.push_back("Flag None");
    paramNames.push_back("Flag Red");
    paramNames.push_back("Flag Green");
    paramNames.push_back("Flag Blue");
    paramNames.push_back("Flag Alpha");
    paramNames.push_back("Flag Size");
    paramNames.push_back("Flag Mass");
    paramNames.push_back("Flag Angle");
    paramNames.push_back("Flag Texture Index");
    paramNames.push_back("Flag Rotation Speed");

	windowViewPort = 0;

	CUIParticleExporter *exporter = new CUIParticleExporter(devices, ps);
	exporter->exportSystem(stringc(devices->getWorkingDirectory() + "ParticlesTests/testparticle.psc").c_str());

}

CUIParticleEditor::~CUIParticleEditor() {

}

bool CUIParticleEditor::OnEvent(const SEvent &event) {

	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == EMIE_LMOUSE_DOUBLE_CLICK && devices->getGUIEnvironment()->getFocus() == viewPort
			&& viewPort->getParent() == window)
		{
			windowViewPort = devices->getGUIEnvironment()->addWindow(rect<s32>(0, 0, 500, 500), false, L"View Port", window, -1);
			devices->getCore()->centerWindow(windowViewPort, dimension2du(window->getRelativePosition().getWidth(), window->getRelativePosition().getHeight()));
			viewPort->remove();
			viewPort = new CGUIViewport(devices->getGUIEnvironment(), windowViewPort, 1, rect<s32>(10, 20, 490, 490));
			if (viewPort) {
				viewPort->setSceneManager(devices->getSceneManager());
				viewPort->setOverrideColor(SColor(255, 0, 0, 0)); 
			}
			nodesEditor->setRelativePosition(rect<s32>(10, 70, window->getRelativePosition().getWidth()-10, window->getRelativePosition().getHeight()-10));
		}
	}

    if (event.EventType == EET_GUI_EVENT) {
        //WINDOW
        if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
            if (event.GUIEvent.Caller == window) {
                devices->getEventReceiver()->RemoveEventReceiver(this);
                delete this;
            }

			if (windowViewPort && event.GUIEvent.Caller == windowViewPort) {
				viewPort->remove();
				viewPort = new CGUIViewport(devices->getGUIEnvironment(), window, 1, rect<s32>(10, 430, 310, 550)); 
				if (viewPort) {
					viewPort->setSceneManager(devices->getSceneManager());
					viewPort->setOverrideColor(SColor(255, 0, 0, 0)); 
				}
				nodesEditor->setRelativePosition(rect<s32>(10, 70, window->getRelativePosition().getWidth()-10, window->getRelativePosition().getHeight()-200));
				viewPort->setRelativePosition(rect<s32>(10, nodesEditor->getRelativePosition().LowerRightCorner.Y+5,
                                                        viewPort->getRelativePosition().LowerRightCorner.X, window->getRelativePosition().getHeight()-5));
				windowViewPort = 0;
			}
        }

        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            if (event.GUIEvent.Caller == window->getMaximizeButton()) {
                devices->getCore()->maximizeWindow(window, rect<s32>(600, 200, 1550, 760));
                nodesEditor->setRelativePosition(rect<s32>(10, 70, window->getRelativePosition().getWidth()-10, window->getRelativePosition().getHeight()-200));
                viewPort->setRelativePosition(rect<s32>(10, nodesEditor->getRelativePosition().LowerRightCorner.Y+5,
                                                        viewPort->getRelativePosition().LowerRightCorner.X, window->getRelativePosition().getHeight()-5));
            }
            
            //ADD GROUP
            if (event.GUIEvent.Caller == addModelClose) {
                addModelWindow->remove();
            }
            if (event.GUIEvent.Caller == addModelOK) {
                addGroup();
            }
        }

        if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
            if (event.GUIEvent.Caller == openRendererTexture) {
                if (selectedNode == 0) {
                    return false;
                }
                E_PS_DATA_TYPE psdt = (E_PS_DATA_TYPE)selectedNode->getDataType();
                SPK::IRR::IRRQuadRenderer *renderer;
                if (psdt == EPSDT_RENDERER) {
                    renderer = (SPK::IRR::IRRQuadRenderer*)selectedNode->getData();
                } else {
                    return false;
                }
                ITexture *tex = devices->getVideoDriver()->getTexture(openRendererTexture->getFileName());
                if (tex != 0) {
                    renderer->setTexture(tex);
                } else {
                    devices->addInformationDialog(L"Texture Information", L"Cannot load this texture...", EMBF_OK);
                }
                selectedNode = 0;
            }

			if (event.GUIEvent.Caller == exportSystemDialog) {
				stringw filename = exportSystemDialog->getFileName();
				if (filename == "")
					filename = "unamed";

				filename += ".psc";

				CUIParticleExporter *exporter = new CUIParticleExporter(devices, ps);
				exporter->exportSystem(filename);
			}
        }
        if (event.GUIEvent.EventType == EGET_FILE_CHOOSE_DIALOG_CANCELLED) {
            if (event.GUIEvent.Caller == openRendererTexture) {
				if (selectedNode != 0) {

				}
                selectedNode = 0;
            }
        }
        
        if (event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED) {
            if (addModelWindow) {
                for (u32 i=0; i < paramNames.size(); i++) {
                    if (event.GUIEvent.Caller == addModelFlagsEnabled[i]) {
                        if (!addModelFlagsEnabled[i]->isChecked()) {
                            addModelFlagsMutable[i]->setEnabled(false);
                            addModelFlagsMutable[i]->setChecked(false);
                            addModelFlagsrandom[i]->setEnabled(false);
                            addModelFlagsrandom[i]->setChecked(false);
                            addModelFlagsInterpolated[i]->setEnabled(false);
                            addModelFlagsInterpolated[i]->setChecked(false);
                        } else {
                            addModelFlagsMutable[i]->setEnabled(true);
                            addModelFlagsrandom[i]->setEnabled(true);
                            addModelFlagsInterpolated[i]->setEnabled(true);
                        }
                    }
                    if (event.GUIEvent.Caller == addModelFlagsInterpolated[i]) {
                        if (addModelFlagsInterpolated[i]->isChecked()) {
                            addModelFlagsMutable[i]->setChecked(false);
                        } else {
                            addModelFlagsMutable[i]->setEnabled(true);
                        }
                    }
                    if (event.GUIEvent.Caller == addModelFlagsMutable[i]) {
                        if (addModelFlagsMutable[i]->isChecked()) {
                            addModelFlagsInterpolated[i]->setChecked(false);
                        } else {
                            addModelFlagsInterpolated[i]->setEnabled(true);
                        }
                    }
                }
            }
        }
        
        if (event.GUIEvent.EventType == EGET_MENU_ITEM_SELECTED) {
            if (event.GUIEvent.Caller == menu->getSubMenu(1)) {
				if (menu->getSubMenu(1)->getSelectedItem() == 2) {
					exportSystemDialog = devices->createFileOpenDialog("Export system to...", CGUIFileSelector::EFST_SAVE_DIALOG, devices->getGUIEnvironment()->getRootGUIElement(), false);
					return true;
				}
                if (menu->getSubMenu(1)->getSelectedItem() == 6) {
                    devices->getEventReceiver()->RemoveEventReceiver(this);
                    window->remove();
                    delete this;
					return true;
                }
            }
            if (event.GUIEvent.Caller == menu->getSubMenu(2)) {
                if (menu->getSubMenu(2)->getSelectedItem() == 0) {
                    using namespace SPK;
                    using namespace SPK::IRR;
                    System *system = IRRSystem::create(ps->getBaseNode(), devices->getSceneManager());
                    system->enableAABBComputing(true);
                    system->setName("Particle System");
					devices->getXEffect()->addShadowToNode((IRRSystem*)system, devices->getXEffectFilterType(), ESM_EXCLUDE);
                    ps->getSystems()->push_back(system);
                    
                    CGUINode *node = new CGUINode(devices->getGUIEnvironment(), nodesEditor, -1);
                    node->setName(system->getName().c_str());
                    node->setParent(0);
                    node->setData(system);
                    node->setDataType(EPSDT_SYSTEM);
                    nodesEditor->addNode(node);
                    node->addTextField(L"Name :", stringw(node->getName()).c_str());
                    node->addCheckBox(L"AA-BB Computing", system->isAABBComputingEnabled());
					node->addCheckBox(L"Enable Auto-Update", ((IRRSystem*)system)->isAutoUpdateEnabled());
					node->addCheckBox(L"Update Only if Visible", ((IRRSystem*)system)->isUpdateOnlyWhenVisible());
                    node->addButton(L"Add Group", L"Adds a group to this system");
                }
				if (menu->getSubMenu(2)->getSelectedItem() == 9) {
					for (u32 i=0; i < ps->getSystems()->size(); i++)
						ps->getSystems()->operator[](i)->empty();
				}
            }
        }

		if (event.GUIEvent.EventType == (EGUI_EVENT_TYPE)ESE_GRAPH_NODE_REMOVED) {
			if (event.GUIEvent.Caller == nodesEditor) {
				CGUINode *node = (CGUINode*)event.GUIEvent.Element;
				E_PS_DATA_TYPE psdt = (E_PS_DATA_TYPE)node->getDataType();
				if (psdt == EPSDT_GROUP) {
					SPK::IRR::IRRSystem *system = (SPK::IRR::IRRSystem*)node->getParent()->getData();
					system->removeGroup((SPK::Group*)node->getData());
					return true;
				} else
				if (psdt == EPSDT_RENDERER) {
					SPK::IRR::IRRRenderer *renderer = (SPK::IRR::IRRRenderer*)node->getData();
					((SPK::Group*)node->getParent()->getData())->setRenderer(0);
				} else
				if (psdt == EPSDT_EMITTER) {
					SPK::Emitter *emitter = (SPK::Emitter*)node->getData();
					((SPK::Group*)node->getParent()->getData())->removeEmitter(emitter);
					delete emitter;
				} else
				if (psdt == EPSDT_MODIFIER) {
					SPK::Modifier *modifier = (SPK::Modifier*)node->getData();
					((SPK::Group*)node->getParent()->getData())->removeModifier(modifier);
					delete modifier;
				} else
				if (psdt == EPSDT_SYSTEM) {
					SPK::IRR::IRRSystem *system = (SPK::IRR::IRRSystem*)node->getData();
					for (u32 i=0; i < ps->getSystems()->size(); i++) {
						if (ps->getSystems()->operator[](i) == system) {
							ps->getSystems()->erase(i);
							devices->getXEffect()->removeShadowFromNode(system);
							devices->getXEffect()->removeNodeFromDepthPass(system);
							devices->getXEffect()->removeNodeFromLightScatteringPass(system);
							break;
						}
					}
					for (u32 i=0; i < system->getGroups().size(); i++) {
						for (u32 j=0; j < system->getGroups()[i]->getEmitters().size(); j++) {
							system->getGroups()[i]->removeEmitter(system->getGroups()[i]->getEmitters()[j]);
						}
						for (u32 j=0; j < system->getGroups()[i]->getModifiers().size(); j++) {
							system->getGroups()[i]->removeModifier(system->getGroups()[i]->getModifiers()[j]);
						}
						system->removeGroup(system->getGroups()[i]);
					}
					system->removeAll();
					system->remove();
				}
			}
		}
        
        if (event.GUIEvent.EventType == (EGUI_EVENT_TYPE)ESE_GRAPH_NODE_CHANGED) {
            if (event.GUIEvent.Caller == nodesEditor) {
                CGUINode *node = nodesEditor->getSelectedNode();
                if (node) {
                    E_PS_DATA_TYPE psdt = (E_PS_DATA_TYPE)node->getDataType();
                    stringc name = event.GUIEvent.Element->getName();
                    //SYSTEM
					//---------------------------------------------------------------------------------------------
					//---------------------------------------SYSTEM------------------------------------------------
					//---------------------------------------------------------------------------------------------
                    if (psdt == EPSDT_SYSTEM) {
                        SPK::IRR::IRRSystem *system = (SPK::IRR::IRRSystem*)node->getData();
                        if (name == "Name :") {
                            ((SPK::System*)system)->setName(stringc(event.GUIEvent.Element->getText()).c_str());
                            node->getInterface()->setText(event.GUIEvent.Element->getText());
                        }
                        if (name == "AA-BB Computing") {
                            system->enableAABBComputing(((IGUICheckBox*)event.GUIEvent.Element)->isChecked());
                        }
						if (name == "Update Only if Visible") {
							system->setAutoUpdateEnabled(system->isAutoUpdateEnabled(), ((IGUICheckBox*)event.GUIEvent.Element)->isChecked());
						}
						if (name == "Enable Auto-Update") {
							system->setAutoUpdateEnabled(((IGUICheckBox*)event.GUIEvent.Element)->isChecked(), system->isUpdateOnlyWhenVisible());
						}
                        if (name == "Add Group") {
                            selectedNode = node;
                            openAddModel();
                        }
                    }
                    //GROUP
					//---------------------------------------------------------------------------------------------
					//----------------------------------------GROUP------------------------------------------------
					//---------------------------------------------------------------------------------------------
                    if (psdt == EPSDT_GROUP) {
                        SPK::Group *group = (SPK::Group*)node->getData();
                        if (name == "Name :") {
                            group->setName(stringc(event.GUIEvent.Element->getText()).c_str());
                            node->getInterface()->setText(event.GUIEvent.Element->getText());
                        }
                        //GRAVITY
                        if (name == "GravityX") {
                            f32 xvalue = devices->getCore()->getF32(event.GUIEvent.Element->getText());
                            SPK::Vector3D v = group->getGravity();
                            group->setGravity(SPK::Vector3D(xvalue, v.y, v.z));
                        }
                        if (name == "GravityY") {
                            f32 yvalue = devices->getCore()->getF32(event.GUIEvent.Element->getText());
                            SPK::Vector3D v = group->getGravity();
                            group->setGravity(SPK::Vector3D(v.x, yvalue, v.z));
                        }
                        if (name == "GravityZ") {
                            f32 zvalue = devices->getCore()->getF32(event.GUIEvent.Element->getText());
                            SPK::Vector3D v = group->getGravity();
                            group->setGravity(SPK::Vector3D(v.x, v.y, zvalue));
                        }
						//VALUES
						if (name == "Friction") {
							f32 fvalue = devices->getCore()->getF32(event.GUIEvent.Element->getText());
							group->setFriction(fvalue);
						}
						//FLAGS
                        if (name == "AA-BB Computing") {
                            group->enableAABBComputing(((IGUICheckBox *)event.GUIEvent.Element)->isChecked());
                        }
						if (name == "Enable Sorting") {
							group->enableSorting(((IGUICheckBox*)event.GUIEvent.Element)->isChecked());
						}
						if (name == "Enable Distance Computation") {
							group->enableDistanceComputation(((IGUICheckBox*)event.GUIEvent.Element)->isChecked());
						}
                        if (name == "Add Renderer") {
                            if (group->getRenderer() == 0) {
                                using namespace SPK;
                                using namespace SPK::IRR;
                                IRRQuadRenderer* renderer = IRRQuadRenderer::create(devices->getDevice());
                                renderer->setScale(0.3f,0.3f);
                                renderer->setTexturingMode(TEXTURE_2D);
                                renderer->setTexture(devices->getVideoDriver()->getTexture("ParticlesTests/fire2.bmp"));
                                renderer->setBlending(BLENDING_ADD);
                                renderer->enableRenderingHint(DEPTH_WRITE,false);
                                renderer->setAtlasDimensions(2,2);
                                renderer->setName("New Renderer");
                                
                                CGUINode *nodeRenderer = new CGUINode(devices->getGUIEnvironment(), nodesEditor, -1);
                                nodeRenderer->setName(renderer->getName().c_str());
                                nodeRenderer->setParent(node);
                                nodeRenderer->setData(renderer);
                                nodeRenderer->setDataType(EPSDT_RENDERER);
                                nodesEditor->addNode(nodeRenderer);
                                
                                nodeRenderer->addTextField(L"Name :", stringw(nodeRenderer->getName()).c_str());
                                nodeRenderer->addDimension2DFields(L"Scale", vector2df(((SPK::IRR::IRRQuadRenderer *)renderer)->getScaleX(),
                                                                                       ((SPK::IRR::IRRQuadRenderer *)renderer)->getScaleY())
                                                                   );
                                IGUIComboBox *texModecb = nodeRenderer->addComboBox(L"Texturing Mode");
                                texModecb->addItem(L"Texturing None");
                                texModecb->addItem(L"Texturing 2D");
                                texModecb->addItem(L"Texturing 3D");
                                texModecb->setSelected(((SPK::IRR::IRRQuadRenderer *)renderer)->getTexturingMode());
                                IGUIComboBox *blendModecb = nodeRenderer->addComboBox(L"Blending Mode");
                                blendModecb->addItem(L"Blending None");
                                blendModecb->addItem(L"Blending Add");
                                blendModecb->addItem(L"Blending Alpha");
                                blendModecb->setSelected(((SPK::IRR::IRRQuadRenderer *)renderer)->getTexturingMode());
                                nodeRenderer->addDimension2DFields(L"Atlas Dimension ", vector2df(((SPK::IRR::IRRQuadRenderer *)renderer)->getAtlasDimensions().Width,
                                                                                                  ((SPK::IRR::IRRQuadRenderer *)renderer)->getAtlasDimensions().Height)
                                                                   );
                                nodeRenderer->addButton(L"Configure Texture...");
								nodeRenderer->addCheckBox(L"Enable Alpha Test", renderer->isRenderingHintEnabled(SPK::ALPHA_TEST));
								nodeRenderer->addCheckBox(L"Enable Depth Test", renderer->isRenderingHintEnabled(SPK::DEPTH_TEST));
								nodeRenderer->addCheckBox(L"Enable Depth Write", renderer->isRenderingHintEnabled(SPK::DEPTH_WRITE));
								nodeRenderer->addCheckBox(L"Set Active", renderer->isActive());
								nodeRenderer->addTextField(L"Alpha test threshold", stringw(renderer->getAlphaTestThreshold()).c_str());
                                
                                group->setRenderer(renderer);
                                
                                event.GUIEvent.Element->setName("Remove Renderer");
                                event.GUIEvent.Element->setText(L"Remove Renderer");
                            }
                        }
                        if (name == "Add Emitter") {
							CUIAddEmitter *addEmitter = new CUIAddEmitter(devices, window, group, nodesEditor, node);
                        }

						if (name == "Add Modifier...") {
							CUIAddModifier *addModifier = new CUIAddModifier(devices, window, group, nodesEditor, node);
						}
                    }
                    //MODEL
					//---------------------------------------------------------------------------------------------
					//---------------------------------------MODEL-------------------------------------------------
					//---------------------------------------------------------------------------------------------
                    if (psdt == EPSDT_MODEL) {
                        SPK::Model *model = (SPK::Model*)node->getData();
                        if (name == "Name :") {
                            model->setName(stringc(event.GUIEvent.Element->getText()).c_str());
                            node->getInterface()->setName(event.GUIEvent.Element->getText());
                        }
                        //LIFE TIME
                        if (name == "Life Time Min") {
                            f32 value = devices->getCore()->getF32(event.GUIEvent.Element->getText());
                            model->setLifeTime(value, model->getLifeTimeMax());
                        }
                        if (name == "Life Time Max") {
                            f32 value = devices->getCore()->getF32(event.GUIEvent.Element->getText());
                            model->setLifeTime(model->getLifeTimeMin(), value);
                        }
                        //FLAGS EDITOR
                        if (name == "Edit Values...") {
                            CUIParticleEditorFlags *flagsEditor = new CUIParticleEditorFlags(devices, model, window, paramNames);
                        }
                        if (name == "Edit Interpolators...") {
							CUIParticleEditorInterpolators *interpolatorsEditor = new CUIParticleEditorInterpolators(devices, model, window);
                        }
                    }
                    //RENDERER
					//---------------------------------------------------------------------------------------------
					//---------------------------------------RENDERER----------------------------------------------
					//---------------------------------------------------------------------------------------------
                    if (psdt == EPSDT_RENDERER) {
                        SPK::IRR::IRRQuadRenderer *renderer = (SPK::IRR::IRRQuadRenderer*)node->getData();
                        if (name == "Name :") {
                            renderer->setName(stringc(event.GUIEvent.Element->getText()).c_str());
                            node->getInterface()->setName(event.GUIEvent.Element->getText());
                        }
                        //SCALE
                        if (name == "ScaleW") {
                            f32 wvalue = devices->getCore()->getF32(event.GUIEvent.Element->getText());
                            renderer->setScale(wvalue, renderer->getScaleY());
                        }
                        if (name == "ScaleH") {
                            f32 hvalue = devices->getCore()->getF32(event.GUIEvent.Element->getText());
                            renderer->setScale(renderer->getScaleX(), hvalue);
                        }
                        //ATLAS DIMENSION
                        if (name == "Atlas Dimension W") {
                            u32 wvalue = devices->getCore()->getF32(event.GUIEvent.Element->getText());
                            if (wvalue > 0)
                                renderer->setAtlasDimensions(dimension2du(wvalue, renderer->getAtlasDimensions().Height));
                        }
                        if (name == "Atlas Dimension H") {
                            u32 hvalue = devices->getCore()->getF32(event.GUIEvent.Element->getText());
                            if (hvalue > 0)
                                renderer->setAtlasDimensions(dimension2du(renderer->getAtlasDimensions().Width, hvalue));
                        }
                        //TEXTURING MODE
                        if (name == "Texturing Mode") {
                            renderer->setTexturingMode((SPK::TexturingMode)((IGUIComboBox *)event.GUIEvent.Element)->getSelected());
                        }
                        //BLENDING MODE
                        if (name == "Blending Mode") {
                            renderer->setBlending((SPK::BlendingMode)((IGUIComboBox *)event.GUIEvent.Element)->getSelected());
                        }
                        if (name == "Configure Texture...") {
                            selectedNode = node;
                            openRendererTexture = devices->createFileOpenDialog(L"Choose Texture...", CGUIFileSelector::EFST_OPEN_DIALOG, true);
                        }
						//RENDERING HINT
						if (name == "Enable Alpha Test") {
							renderer->enableRenderingHint(SPK::ALPHA_TEST, ((IGUICheckBox*)event.GUIEvent.Element)->isChecked());
						}
						if (name == "Enable Depth Test") {
							renderer->enableRenderingHint(SPK::DEPTH_TEST, ((IGUICheckBox*)event.GUIEvent.Element)->isChecked());
						}
						if (name == "Enable Depth Write") {
							renderer->enableRenderingHint(SPK::DEPTH_WRITE, ((IGUICheckBox*)event.GUIEvent.Element)->isChecked());
						}
						//SET ACTIVE
						if (name == "Set Active") {
							renderer->setActive(((IGUICheckBox*)event.GUIEvent.Element)->isChecked());
						}
						//ALPHA TEST THRESHOLD
						if (name == "Alpha test threshold") {
							f32 valueatt = devices->getCore()->getF32(event.GUIEvent.Element->getText());
							renderer->setAlphaTestThreshold(valueatt);
						}
                    }
                    //EMITTER
					//---------------------------------------------------------------------------------------------
					//---------------------------------------EMITTER-----------------------------------------------
					//---------------------------------------------------------------------------------------------
                    if (psdt == EPSDT_EMITTER) {
                        SPK::Emitter *emitter = (SPK::Emitter*)node->getData();
                        if (name == "Name :") {
                            emitter->setName(stringc(event.GUIEvent.Element->getText()).c_str());
                            node->getInterface()->setName(event.GUIEvent.Element->getText());
                        } else
                        if (name == "Flow") {
							if (stringc(event.GUIEvent.Element->getText()) != "") {
								emitter->setFlow(devices->getCore()->getF32(event.GUIEvent.Element->getText()));
							}
                        } else
                        //FORCE
                        if (name == "Force Min") {
                            f32 force = devices->getCore()->getF32(event.GUIEvent.Element->getText());
                            emitter->setForce(force, emitter->getForceMax());
                        } else
                        if (name == "Force Max") {
                            f32 force = devices->getCore()->getF32(event.GUIEvent.Element->getText());
                            emitter->setForce(emitter->getForceMin(), force);
                        } else
                        //ZONES
                        if (name == "Configure Zone...") {
							CUIParticlesEditZone *editZone = new CUIParticlesEditZone(devices, emitter, window);
                        } else
						//ACTIVE
						if (name == "Active") {
							emitter->setActive(((IGUICheckBox*)event.GUIEvent.Element)->isChecked());
						} else
						//TANK
						if (name == "Tank") {
							f32 value = devices->getCore()->getS32(event.GUIEvent.Element->getText());
							emitter->setTank(value);
						} else
						if (name == "Change Tank") {
							f32 value = devices->getCore()->getS32(event.GUIEvent.Element->getText());
							emitter->changeTank(value);
						} else
						//FLOW
						if (name == "Flow") {
							f32 value = devices->getCore()->getF32(event.GUIEvent.Element->getText());
							emitter->setFlow(value);
						} else
						if (name == "Change Flow") {
							f32 value = devices->getCore()->getF32(event.GUIEvent.Element->getText());
							emitter->changeFlow(value);
						}
                    }
					//---------------------------------------------------------------------------------------------
					//---------------------------------------MODIFIER----------------------------------------------
					//---------------------------------------------------------------------------------------------
					if (psdt == EPSDT_MODIFIER) {
						SPK::Modifier *modifier = (SPK::Modifier*)node->getData();
						if (name == "Name :") {
							modifier->setName(stringc(event.GUIEvent.Element->getText()).c_str());
						} else
						/// TRIGGERS
						if (name == "Trigger") {
							SPK::Destroyer *destroyerModifier = dynamic_cast<SPK::Destroyer*>(modifier);
							if (destroyerModifier)
								destroyerModifier->setTrigger((SPK::ModifierTrigger)((IGUIComboBox*)event.GUIEvent.Element)->getSelected());

							SPK::LinearForce *linearForceModifier = dynamic_cast<SPK::LinearForce*>(modifier);
							if (linearForceModifier)
								linearForceModifier->setTrigger((SPK::ModifierTrigger)((IGUIComboBox*)event.GUIEvent.Element)->getSelected());

							SPK::Obstacle *obstacleModifier = dynamic_cast<SPK::Obstacle*>(modifier);
							if (obstacleModifier)
								obstacleModifier->setTrigger((SPK::ModifierTrigger)((IGUIComboBox*)event.GUIEvent.Element)->getSelected());

							SPK::PointMass *pointMassModifier = dynamic_cast<SPK::PointMass*>(modifier);
							if (pointMassModifier)
								pointMassModifier->setTrigger((SPK::ModifierTrigger)((IGUIComboBox*)event.GUIEvent.Element)->getSelected());
						}
						/// Collision Modifier
						if (name == "Scale") {
							f32 value = devices->getCore()->getF32(event.GUIEvent.Element->getText());
							((SPK::Collision*)modifier)->setScale(value);
						} else
						if (name == "Elasticity") {
							f32 value = devices->getCore()->getF32(event.GUIEvent.Element->getText());
							((SPK::Collision*)modifier)->setElasticity(value);
						} else
						/// Linear Force
						if (name == "ForceX" || name == "ForceY" || name == "ForceZ") {
                            f32 value = devices->getCore()->getF32(event.GUIEvent.Element->getText());
							SPK::Vector3D v = ((SPK::LinearForce*)modifier)->getForce();
							if (name == "ForceX")
								((SPK::LinearForce*)modifier)->setForce(SPK::Vector3D(value, v.y, v.z));
							else if (name == "ForceY")
								((SPK::LinearForce*)modifier)->setForce(SPK::Vector3D(v.x, value, v.z));
							else
								((SPK::LinearForce*)modifier)->setForce(SPK::Vector3D(v.x, v.y, value));
                        } else
						if (name == "Force Factor") {
							((SPK::LinearForce*)modifier)->setFactor((SPK::ForceFactor)((IGUIComboBox*)event.GUIEvent.Element)->getSelected(),
																	 ((SPK::LinearForce*)modifier)->getFactorParam());
						} else
						if (name == "Param") {
							((SPK::LinearForce*)modifier)->setFactor(((SPK::LinearForce*)modifier)->getFactorType(),
																	 (SPK::ModelParam)((IGUIComboBox*)event.GUIEvent.Element)->getSelected());
						} else
						/// Obstacle
						if (name == "Bouncing Ratio") {
							f32 value = devices->getCore()->getF32(event.GUIEvent.Element->getText());
							((SPK::Obstacle*)modifier)->setBouncingRatio(value);
						} else
						if (name == "Friction") {
							f32 value = devices->getCore()->getF32(event.GUIEvent.Element->getText());
							((SPK::Obstacle*)modifier)->setFriction(value);
						} else
						/// Point Mass
						if (name == "Mass") {
							f32 value = devices->getCore()->getF32(event.GUIEvent.Element->getText());
							((SPK::PointMass*)modifier)->setMass(value);
						} else
						if (name == "Min Distance") {
							f32 value = devices->getCore()->getF32(event.GUIEvent.Element->getText());
							((SPK::PointMass*)modifier)->setMinDistance(value);
						} else
						/// Vortex
						if (name == "PositionX" || name == "PositionY" || name == "PositionZ") {
							f32 value = devices->getCore()->getF32(event.GUIEvent.Element->getText());
							SPK::Vector3D v = ((SPK::Vortex*)modifier)->getPosition();
							if (name == "PositionX")
								((SPK::Vortex*)modifier)->setPosition(SPK::Vector3D(value, v.y, v.z));
							else if (name == "PositionY")
								((SPK::Vortex*)modifier)->setPosition(SPK::Vector3D(v.x, value, v.z));
							else
								((SPK::Vortex*)modifier)->setPosition(SPK::Vector3D(v.x, v.y, value));
						} else
						if (name == "DirectionX" || name == "DirectionY" || name == "DirectionZ") {
							f32 value = devices->getCore()->getF32(event.GUIEvent.Element->getText());
							SPK::Vector3D v = ((SPK::Vortex*)modifier)->getDirection();
							if (name == "DirectionX")
								((SPK::Vortex*)modifier)->setDirection(SPK::Vector3D(value, v.y, v.z));
							else if (name == "DirectionY")
								((SPK::Vortex*)modifier)->setDirection(SPK::Vector3D(v.x, value, v.z));
							else
								((SPK::Vortex*)modifier)->setDirection(SPK::Vector3D(v.x, v.y, value));
						} else
						if (name == "Rotation Speed") {
							f32 value = devices->getCore()->getF32(event.GUIEvent.Element->getText());
							((SPK::Vortex*)modifier)->setRotationSpeed(value, ((SPK::Vortex*)modifier)->isRotationSpeedAngular());
						} else
						if (name == "Attraction Speed") {
							f32 value = devices->getCore()->getF32(event.GUIEvent.Element->getText());
							((SPK::Vortex*)modifier)->setAttractionSpeed(value, ((SPK::Vortex*)modifier)->isAttractionSpeedLinear());
						}
					}
                }
            }
        }
    }
    
	return false;
}

void CUIParticleEditor::addGroup() {
    if (selectedNode == 0) {
        return;
    }
    
    SPK::System *system=0;
    E_PS_DATA_TYPE psdt = (E_PS_DATA_TYPE)selectedNode->getDataType();
    if (psdt == EPSDT_SYSTEM) {
        system = (SPK::System*)selectedNode->getData();
    } else {
        return;
    }
    
	int enableFlags = 0;
    int mutableFlags = 0;
    int randomFlags = 0;
    int interpolatedFlags = 0;
    
    for (size_t i=0; i < 10; i++) {
		int tempFlag = 1 << static_cast<SPK::ModelParamFlag>(i);
        if (addModelFlagsEnabled[i]->isChecked()) {
			enableFlags += tempFlag;
        }
        if (addModelFlagsMutable[i]->isChecked()) {
            mutableFlags += tempFlag;
        }
        if (addModelFlagsrandom[i]->isChecked()) {
            randomFlags += tempFlag;
        }
        if (addModelFlagsInterpolated[i]->isChecked()) {
            interpolatedFlags += tempFlag;
        }
    }

	enableFlags /= 2;
	mutableFlags /= 2;
	randomFlags /= 2;
	interpolatedFlags /= 2;

	//int enabledtest = SPK::FLAG_RED | SPK::FLAG_GREEN | SPK::FLAG_BLUE | SPK::FLAG_ALPHA | SPK::FLAG_SIZE;
	//int enabledtest2 = SPK::FLAG_RED | SPK::FLAG_GREEN | SPK::FLAG_BLUE;

    SPK::Model* model = SPK::Model::create(enableFlags, mutableFlags, randomFlags, interpolatedFlags);
    model->setName("New Model");
    
    SPK::Group *group = SPK::Group::create(model, devices->getCore()->getF32(addModelCapacity->getText()));
    group->setName("New Group");

	system->addGroup(group);
    
    createGroup(selectedNode, model, group, system);
    
    addModelWindow->remove();
    addModelWindow = 0;
    selectedNode = 0;
    
}

void CUIParticleEditor::openAddModel() {
    
    IGUIEnvironment *gui = devices->getGUIEnvironment();
    
	addModelWindow = gui->addWindow(rect<s32>(680, 560, 1370, 910), true, L"Add Group", window, -1);
	devices->getCore()->centerWindow(addModelWindow, dimension2du(window->getRelativePosition().getWidth(),
																  window->getRelativePosition().getHeight()));
    
    gui->addStaticText(L"Enabled Flags : ", rect<s32>(10, 30, 110, 50), true, true, addModelWindow, -1, true);
    gui->addStaticText(L"Mutable Flags : ", rect<s32>(170, 30, 270, 50), true, true, addModelWindow, -1, true);
    gui->addStaticText(L"Random Flags : ", rect<s32>(350, 30, 450, 50), true, true, addModelWindow, -1, true);
    gui->addStaticText(L"Interpolated : ", rect<s32>(520, 30, 620, 50), true, true, addModelWindow, -1, true);
    
    for (u32 i=0; i < paramNames.size(); i++) {
        addModelFlagsEnabled[i] = gui->addCheckBox(false, rect<s32>(10, 50, 160, 70), addModelWindow, i, paramNames[i].c_str());
        (addModelFlagsMutable[i] = gui->addCheckBox(false, rect<s32>(180, 50, 330, 70), addModelWindow, i, paramNames[i].c_str()))->setEnabled(false);
        (addModelFlagsrandom[i] = gui->addCheckBox(false, rect<s32>(350, 50, 500, 70), addModelWindow, i, paramNames[i].c_str()))->setEnabled(false);
        (addModelFlagsInterpolated[i] = gui->addCheckBox(false, rect<s32>(520, 50, 670, 70), addModelWindow, i, paramNames[i].c_str()))->setEnabled(false);
        
        addModelFlagsEnabled[i]->setRelativePosition(position2di(10, 50+(20*i)));
        addModelFlagsMutable[i]->setRelativePosition(position2di(180, 50+(20*i)));
        addModelFlagsrandom[i]->setRelativePosition(position2di(350, 50+(20*i)));
        addModelFlagsInterpolated[i]->setRelativePosition(position2di(520, 50+(20*i)));
    }
    
    addModelOK = gui->addButton(rect<s32>(580, 310, 680, 340), addModelWindow, -1, L"Add", L"Add group with selected configuration");
    addModelClose = gui->addButton(rect<s32>(470, 310, 570, 340), addModelWindow, -1, L"Cancel", L"Cancle add group operation");
    
    gui->addStaticText(L"", rect<s32>(150, 30, 160, 300), true, true, addModelWindow, -1, true);
    gui->addStaticText(L"", rect<s32>(330, 30, 340, 300), true, true, addModelWindow, -1, true);
    gui->addStaticText(L"", rect<s32>(500, 30, 510, 300), true, true, addModelWindow, -1, true);
    
    gui->addStaticText(L"Capacity :", rect<s32>(10, 320, 90, 340), true, true, addModelWindow, -1, true);
    addModelCapacity = gui->addEditBox(L"135", rect<s32>(90, 320, 190, 340), true, addModelWindow, -1);
}

void CUIParticleEditor::createGroup(CGUINode *node, SPK::Model *model, SPK::Group *group, SPK::System *system) {
    CGUINode *modelNode = new CGUINode(devices->getGUIEnvironment(), nodesEditor, -1);
    modelNode->setName(model->getName().c_str());
    modelNode->setData(model);
    modelNode->setDataType(EPSDT_MODEL);
    
    CGUINode *groupNode = new CGUINode(devices->getGUIEnvironment(), nodesEditor, -1);
    groupNode->setName(group->getName().c_str());
    groupNode->setData(group);
    groupNode->setDataType(EPSDT_GROUP);
    
    modelNode->setParent(groupNode);
    groupNode->setParent(node);
    
    nodesEditor->addNode(groupNode);
    nodesEditor->addNode(modelNode);
    
    modelNode->addTextField(L"Name :", stringw(modelNode->getName()).c_str());
    modelNode->add2ParametersFields(L"Life Time ", L"Min", L"Max", model->getLifeTimeMin(), model->getLifeTimeMax());
    modelNode->addButton(L"Edit Values...");
    modelNode->addButton(L"Edit Interpolators...");
	((IGUIWindow*)modelNode->getInterface())->getCloseButton()->setVisible(false);
    
    groupNode->addTextField(L"Name :", stringw(groupNode->getName()).c_str());
    groupNode->addVector3DFields(L"Gravity", devices->getCore()->getVector3dfFromSpark(group->getGravity()));
    groupNode->addCheckBox(L"AA-BB Computing", group->isAABBComputingEnabled());
    groupNode->addButton(L"Add Renderer");
    groupNode->addButton(L"Add Emitter");
	groupNode->addTextField(L"Friction", stringw(group->getFriction()).c_str());
	groupNode->addCheckBox(L"Enable Sorting", group->isSortingEnabled());
	groupNode->addCheckBox(L"Enable Distance Computation", group->isDistanceComputationEnabled());
	groupNode->addButton(L"Add Modifier...", L"Open dialog to add modifier (vortex, obstacle, etc)");
}
