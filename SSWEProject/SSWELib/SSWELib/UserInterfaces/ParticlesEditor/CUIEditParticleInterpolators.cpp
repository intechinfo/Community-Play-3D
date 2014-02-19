//
//  CUIEditParticleFlags.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/01/2014.
//
//

#include "stdafx.h"
#include "CUIEditParticleInterpolators.h"

CUIParticleEditorInterpolators::CUIParticleEditorInterpolators(CDevices *_devices, SPK::Model *_model, IGUIElement *parent) {
    devices = _devices;

	model = _model;
    
    IGUIEnvironment *gui = devices->getGUIEnvironment();
    //GUI
	stringw windowName = L"Particle System Editor Interpolators : ";
	windowName += model->getName().c_str();

	window = gui->addWindow(rect<s32>(660, 270, 1360, 330), false, windowName.c_str(), parent, -1);
    window->getMinimizeButton()->setVisible(false);
    window->getMaximizeButton()->setVisible(false);
    devices->getCore()->centerWindow(window, (dimension2du)parent->getRelativePosition().getSize());
    
    devices->getEventReceiver()->AddEventReceiver(this);

	SPK::Interpolator *it = model->getInterpolator(SPK::PARAM_BLUE);
	std::set<SPK::InterpolatorEntry>::iterator t = it->getGraph().begin();
	for (; t != it->getGraph().end(); ++t) {

	}
}

CUIParticleEditorInterpolators::~CUIParticleEditorInterpolators() {
    
}

bool CUIParticleEditorInterpolators::OnEvent(const SEvent &event) {

    return false;
}
