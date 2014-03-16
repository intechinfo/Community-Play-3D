/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "stdafx.h"
#include "CUIParticlesEditor.h"

using namespace SPK;
using namespace SPK::IRR;

CUIParticlesEditor::CUIParticlesEditor(CDevices *_devices) {
    
    devices = _devices;
    
    //----------TEST-----------------
    SParticleSystem ps("test");
    ps.createBaseNode(devices->getSceneManager());
    
    System::setClampStep(true,0.1f);
	System::useAdaptiveStep(0.001f,0.01f);

    IRRQuadRenderer* fireRenderer = IRRQuadRenderer::create(devices->getDevice());
	fireRenderer->setScale(0.3f,0.3f);
	fireRenderer->setTexture(devices->getVideoDriver()->getTexture("ParticlesTests/fire2.bmp"));
	fireRenderer->setTexturingMode(TEXTURE_2D);
	fireRenderer->setBlending(BLENDING_ADD);
	fireRenderer->enableRenderingHint(DEPTH_WRITE,false);
	fireRenderer->setAtlasDimensions(2,2);
    fireRenderer->setName("Fire Renderer");
    
    IRRQuadRenderer* smokeRenderer = IRRQuadRenderer::create(devices->getDevice());
	smokeRenderer->setScale(0.3f,0.3f);
	smokeRenderer->setTexture(devices->getVideoDriver()->getTexture("ParticlesTests/explosion.png"));
	smokeRenderer->setTexturingMode(TEXTURE_2D);
	smokeRenderer->setBlending(BLENDING_ALPHA);
	smokeRenderer->enableRenderingHint(DEPTH_WRITE,false);
	smokeRenderer->setAtlasDimensions(2,2);
    smokeRenderer->setName("Smoke Renderer");
    
    Model* fireModel = Model::create(FLAG_RED | FLAG_GREEN | FLAG_BLUE | FLAG_ALPHA | FLAG_SIZE | FLAG_ANGLE | FLAG_TEXTURE_INDEX,
                                     FLAG_RED | FLAG_GREEN | FLAG_ALPHA | FLAG_ANGLE,
                                     FLAG_RED | FLAG_GREEN | FLAG_TEXTURE_INDEX | FLAG_ANGLE,
                                     FLAG_SIZE);
	fireModel->setParam(PARAM_RED,0.8f,0.9f,0.8f,0.9f);
	fireModel->setParam(PARAM_GREEN,0.5f,0.6f,0.5f,0.6f);
	fireModel->setParam(PARAM_BLUE,0.3f);
	fireModel->setParam(PARAM_ALPHA,0.4f,0.0f);
	fireModel->setParam(PARAM_ANGLE,0.0f,2.0f * PI,0.0f,2.0f * PI);
	fireModel->setParam(PARAM_TEXTURE_INDEX,0.0f,4.0f);
    fireModel->setParam(PARAM_SIZE, 10.0);
	fireModel->setLifeTime(1.0f,1.5f);
    fireModel->setName("Fire Model");
    
	Interpolator* interpolator = fireModel->getInterpolator(PARAM_SIZE);
	interpolator->addEntry(0.5f,2.0f,5.0f);
	interpolator->addEntry(1.0f,0.0f);
    
	Model* smokeModel = Model::create(FLAG_RED | FLAG_GREEN | FLAG_BLUE | FLAG_ALPHA | FLAG_SIZE | FLAG_ANGLE | FLAG_TEXTURE_INDEX,
                                      FLAG_RED | FLAG_GREEN | FLAG_SIZE | FLAG_ANGLE,
                                      FLAG_TEXTURE_INDEX | FLAG_ANGLE,
                                      FLAG_ALPHA);
	smokeModel->setParam(PARAM_RED,0.3f,0.2f);
	smokeModel->setParam(PARAM_GREEN,0.25f,0.2f);
	smokeModel->setParam(PARAM_BLUE,0.2f);
	smokeModel->setParam(PARAM_ALPHA,0.2f,0.0f);
	smokeModel->setParam(PARAM_SIZE,5.0,10.0f);
	smokeModel->setParam(PARAM_TEXTURE_INDEX,0.0f,4.0f);
	smokeModel->setParam(PARAM_ANGLE,0.0f,2.0f * PI,0.0f,2.0f * PI);
	smokeModel->setLifeTime(5.0f,5.0f);
    smokeModel->setName("Smoke Model");
    
	interpolator = smokeModel->getInterpolator(PARAM_ALPHA);
	interpolator->addEntry(0.0f,0.0f);
	interpolator->addEntry(0.2f,0.2f);
	interpolator->addEntry(1.0f,0.0f);
    
	// Emitters
	// The emitters are arranged so that the fire looks realistic
	StraightEmitter* fireEmitter1 = StraightEmitter::create(Vector3D(0.0f,1.0f,0.0f));
	fireEmitter1->setZone(Sphere::create(Vector3D(0.0f,-1.0f,0.0f),0.5f));
	fireEmitter1->getZone()->setName("SPHERE:emitter1");
	fireEmitter1->setFlow(40);
	fireEmitter1->setForce(1.0f,2.5f);
    fireEmitter1->setName("Fire Emitter 1");
    
	StraightEmitter* fireEmitter2 = StraightEmitter::create(Vector3D(1.0f,0.6f,0.0f));
	fireEmitter2->setZone(Sphere::create(Vector3D(0.15f,-1.2f,0.075f),0.1f));
	fireEmitter2->getZone()->setName("SPHERE:emitter2");
	fireEmitter2->setFlow(15);
	fireEmitter2->setForce(0.5f,1.5f);
    fireEmitter2->setName("Fire Emitter 2");
    
	StraightEmitter* fireEmitter3 = StraightEmitter::create(Vector3D(-0.6f,0.8f,-0.8f));
	fireEmitter3->setZone(Sphere::create(Vector3D(-0.375f,-1.15f,-0.375f),0.3f));
	fireEmitter3->getZone()->setName("SPHERE:emitter3");
	fireEmitter3->setFlow(15);
	fireEmitter3->setForce(0.5f,1.5f);
    fireEmitter3->setName("Fire Emitter 3");
    
	StraightEmitter* fireEmitter4 = StraightEmitter::create(Vector3D(-0.8f,0.5f,0.2f));
	fireEmitter4->setZone(Sphere::create(Vector3D(-0.255f,-1.2f,0.225f),0.2f));
	fireEmitter4->getZone()->setName("SPHERE:emitter4");
	fireEmitter4->setFlow(10);
	fireEmitter4->setForce(0.5f,1.5f);
    fireEmitter4->setName("Fire Emitter 4");
    
	StraightEmitter* fireEmitter5 = StraightEmitter::create(Vector3D(0.1f,0.8f,-1.0f));
	fireEmitter5->setZone(Sphere::create(Vector3D(-0.075f,-1.2f,-0.3f),0.2f));
	fireEmitter5->getZone()->setName("SPHERE:emitter5");
	fireEmitter5->setFlow(10);
	fireEmitter5->setForce(0.5f,1.5f);
    fireEmitter5->setName("Fire Emitter 5");
    
	Emitter *smokeEmitter = SphericEmitter::create(Vector3D(0.0f,1.0f,0.0f),0.0f,0.5f * PI);
	smokeEmitter->setZone(Sphere::create(Vector3D(),1.2f));
	smokeEmitter->getZone()->setName("SPHERE:emitter");
	smokeEmitter->setFlow(25);
	smokeEmitter->setForce(0.5f,1.0f);
    smokeEmitter->setName("smoke Emitter");
    
	// Groups
	Group *fireGroup = Group::create(fireModel,135);
	fireGroup->addEmitter(fireEmitter1);
	fireGroup->addEmitter(fireEmitter2);
	fireGroup->addEmitter(fireEmitter3);
	fireGroup->addEmitter(fireEmitter4);
	fireGroup->addEmitter(fireEmitter5);
	fireGroup->setRenderer(fireRenderer);
	fireGroup->setGravity(Vector3D(0.0f,3.0f,0.0f));
	fireGroup->enableAABBComputing(true);
    fireGroup->setName("Fire Group");
    
	Group *smokeGroup = Group::create(smokeModel,135);
	smokeGroup->addEmitter(smokeEmitter);
	smokeGroup->setRenderer(smokeRenderer);
	smokeGroup->setGravity(Vector3D(0.0f,0.4f,0.0f));
	smokeGroup->enableAABBComputing(true);
    smokeGroup->setName("Smoke Group");
	
	// System
	System *particleSystem = IRRSystem::create(ps.getBaseNode(), devices->getSceneManager());
	particleSystem->addGroup(smokeGroup);
	particleSystem->addGroup(fireGroup);
	particleSystem->enableAABBComputing(true);
    particleSystem->setName("Particle System");
    ps.getSystems()->push_back(particleSystem);
    
    devices->getCoreData()->getParticleSystems()->push_back(ps);
    ps.getBaseNode()->setName(L"ParticleTest");
    devices->getCoreData()->getObjectsData()->push_back(SObjectsData(0, ps.getBaseNode(), L""));
    
    /*SParticleSystem ps("New Particle System");
    ps.createBaseNode(devices->getSceneManager());
    devices->getCoreData()->getParticleSystems()->push_back(ps);*/
    //----------TEST-----------------
    
    IGUIEnvironment *gui = devices->getGUIEnvironment();
    
	window = gui->addWindow(rect<s32>(400, 30, 901, 360), false, L"Edit Particle Systems", 0, -1);
    window->getMinimizeButton()->setVisible(true);
    devices->getCore()->centerWindow(window, devices->getVideoDriver()->getScreenSize());
    
    edit = gui->addButton(rect<s32>(10, 30, 90, 50), window, -1, L"Edit", L"Edit selected particle system");
    add = gui->addButton(rect<s32>(330, 30, 350, 50), window, -1, L"+", L"Add a new particle system");
    remove = gui->addButton(rect<s32>(310, 30, 330, 50), window, -1, L"-", L"Remove the selected particle system");
    
    particleSystems = gui->addListBox(rect<s32>(10, 50, 350, 290), window, -1, true);
    
    gui->addStaticText(L"Name : ", rect<s32>(10, 300, 70, 320), true, true, window, -1, true);
    name = gui->addEditBox(L"", rect<s32>(70, 300, 350, 320), true, window, -1);
    
    update = gui->addCheckBox(false, rect<s32>(360, 50, 490, 70), window, -1, L"Update");
    
    if (devices->getCoreData()->getParticleSystems()->size() == 0) {
        edit->setEnabled(false);
        remove->setEnabled(false);
        update->setEnabled(false);
        name->setEnabled(false);
    } else {
        for (u32 i=0; i < devices->getCoreData()->getParticleSystems()->size(); i++) {
            particleSystems->addItem(stringw(devices->getCoreData()->getParticleSystems()->operator[](i).getName()).c_str());
        }
        particleSystems->setSelected(0);
        update->setChecked(devices->getCoreData()->getParticleSystems()->operator[](0).isUpdating());
        name->setText(stringw(devices->getCoreData()->getParticleSystems()->operator[](0).getName()).c_str());
    }
    
    devices->getEventReceiver()->AddEventReceiver(this, window);
    
    //CUIParticleEditor *pse = new CUIParticleEditor(devices, &devices->getCoreData()->getParticleSystems()->operator[](0));
}

CUIParticlesEditor::~CUIParticlesEditor() {

}


bool CUIParticlesEditor::OnEvent(const SEvent &event) {

    if (event.EventType == EET_GUI_EVENT) {
        //WINDOW
        if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
            if (event.GUIEvent.Caller == window) {
                devices->getEventReceiver()->RemoveEventReceiver(this);
                delete this;
            }
        }
        
        //BUTTONS
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            if (event.GUIEvent.Caller == remove) {
                u32 selected = particleSystems->getSelected();
                if (selected != -1) {
                    devices->getCoreData()->getParticleSystems()->operator[](selected).clear();
                    devices->getCoreData()->getParticleSystems()->operator[](selected).destroyScript();
                    devices->getCoreData()->getParticleSystems()->erase(selected);
                    particleSystems->removeItem(selected);
                    
                    particleSystems->setSelected(0);
                    
                    if (particleSystems->getItemCount() == 0) {
                        remove->setEnabled(false);
                        update->setEnabled(false);
                        edit->setEnabled(false);
                        name->setEnabled(false);
                    }
                }
            }
            if (event.GUIEvent.Caller == add) {
                SParticleSystem ps("new particle system");
                ps.createBaseNode(devices->getSceneManager());
                devices->getCoreData()->getParticleSystems()->push_back(ps);
                
                particleSystems->addItem(stringw(ps.getName()).c_str());
                particleSystems->setSelected(particleSystems->getItemCount()-1);
                
                remove->setEnabled(true);
                update->setEnabled(true);
                edit->setEnabled(true);
                name->setEnabled(true);
                
                name->setText(stringw(devices->getCoreData()->getParticleSystems()->operator[](particleSystems->getSelected()).getName()).c_str());
            }
            if (event.GUIEvent.Caller == edit) {
                if (particleSystems->getSelected() != -1)
                    CUIParticleEditor *pse = new CUIParticleEditor(devices, &devices->getCoreData()->getParticleSystems()->operator[](particleSystems->getSelected()));
            }
        }
        
        //LIST BOX
        if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
            if (event.GUIEvent.Caller == particleSystems) {
                if (particleSystems->getSelected() != -1) {
                    update->setChecked(devices->getCoreData()->getParticleSystems()->operator[](particleSystems->getSelected()).isUpdating());
                    name->setText(stringw(devices->getCoreData()->getParticleSystems()->operator[](particleSystems->getSelected()).getName()).c_str());
                }
            }
        }
        
        //EDIT BOX
        if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
            if (event.GUIEvent.Caller == name) {
                if (particleSystems->getSelected() != -1) {
                    devices->getCoreData()->getParticleSystems()->operator[](particleSystems->getSelected()).setName(stringc(name->getText()));
                    particleSystems->setItem(particleSystems->getSelected(), name->getText(), 0);
                }
            }
        }
        
        //CHECK BOX
        if (event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED) {
            if (event.GUIEvent.Caller == update) {
                if (particleSystems->getSelected() != -1) {
                    devices->getCoreData()->getParticleSystems()->operator[](particleSystems->getSelected()).setUpdate(update->isChecked());
                }
            }
        }
    }
    
	return false;
}
