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

#include <DLLExport.h>

cp3d::video::IMonitor *createMonitor() {
	return (cp3d::video::IMonitor*) new CGenericMonitor();
}

CGenericMonitor::CGenericMonitor() {
    
}

CGenericMonitor::~CGenericMonitor() {

}

void CGenericMonitor::init() {
	monitorName = "Generic Monitor";
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
}

void CGenericMonitor::drawGUI() {
	gui->drawAll();
}
