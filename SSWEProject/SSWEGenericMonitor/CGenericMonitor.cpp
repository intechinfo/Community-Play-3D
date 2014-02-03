//
//  CExporter.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#include "stdafx.h"
#include "CGenericMonitor.h"

#include <iostream>

void *createMonitor() {
	return static_cast< void* > (new CGenericMonitor);
}

CGenericMonitor::CGenericMonitor() {
    
}

CGenericMonitor::~CGenericMonitor() {

}

void CGenericMonitor::init() {
	monitorName = "Generic Monitor";
    
    irr::core::stringc USBPort = "/dev/tty.usbmodem1411";
}

void CGenericMonitor::setActiveCamera(irr::scene::ICameraSceneNode *camera) {
	monitorCamera = camera;
}

void CGenericMonitor::setSceneManager(irr::scene::ISceneManager *sceneManager) {
	smgr = sceneManager;
	driver = smgr->getVideoDriver();
}

void CGenericMonitor::drawScene() {
    
	if (renderRenderer) {
		renderer->setActiveSceneManager(smgr);
		renderer->update(renderFull);
    } else {
        smgr->drawAll();
    }
    
    irr::ILogger *logger = renderer->getIrrlichtDevice()->getLogger();

}

void CGenericMonitor::drawGUI() {
	gui->drawAll();
}
