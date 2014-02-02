//
//  CUIEditParticleFlags.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/01/2014.
//
//

#include "stdafx.h"
#include "CUIEditParticleFlags.h"

CUIParticleEditorFlags::CUIParticleEditorFlags(CDevices *_devices, SPK::Model *_model, E_PARTICLE_FLAG_TYPE _type, IGUIElement *parent, array<stringw> &modelParams) {
    devices = _devices;
    
    IGUIEnvironment *gui = devices->getGUIEnvironment();
    //GUI
    window = gui->addWindow(rect<s32>(660, 270, 1030, 330), false, L"Particle System Editor", parent, -1);
    window->getMinimizeButton()->setVisible(true);
    window->getMaximizeButton()->setVisible(true);
    devices->getCore()->centerWindow(window, (dimension2du)parent->getRelativePosition().getSize());
    
    devices->getEventReceiver()->AddEventReceiver(this);
    
    SPK::Model *model = _model;
    u32 currenty = 25;
    for (int i=0; i < (int)SPK::PARAM_ROTATION_SPEED; i++) {
        bool add = false;
        
        add = model->isEnabled(static_cast<SPK::ModelParam>(i));
        
        if (add) {
            paramsElements.push_back(gui->addStaticText(modelParams[i+1].c_str(), rect<s32>(10, currenty, 140, currenty+20), true, true, window, i, true));
            fillWindow(currenty, window);
            currenty += 20;
        }
    }
}

CUIParticleEditorFlags::~CUIParticleEditorFlags() {
    
}

void CUIParticleEditorFlags::fillWindow(u32 currenty, IGUIElement *element) {
    window->setRelativePosition(rect<s32>(window->getRelativePosition().UpperLeftCorner.X,
                                          window->getRelativePosition().UpperLeftCorner.Y,
                                          window->getRelativePosition().LowerRightCorner.X,
                                          window->getRelativePosition().LowerRightCorner.Y + currenty - 25));
}

bool CUIParticleEditorFlags::OnEvent(const SEvent &event) {
    
    return false;
}
