//
//  CExporter.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#include "stdafx.h"
#include "CGenericMonitor.h"

void *createMonitor() {
	return static_cast< void* >(new CCGenericMonitor);
}

CCGenericMonitor::CCGenericMonitor() {

}

CCGenericMonitor::~CCGenericMonitor() {

}

void CCGenericMonitor::init() {
	monitorName = "Generic Monitor";
}

void CCGenericMonitor::setActiveCamera(irr::scene::ICameraSceneNode *camera) {
	monitorCamera = camera;
}

void CCGenericMonitor::setSceneManager(irr::scene::ISceneManager *sceneManager) {
	smgr = sceneManager;
	driver = smgr->getVideoDriver();
}

void CCGenericMonitor::drawScene() {
	if (renderRenderer) {
		renderer->setActiveSceneManager(smgr);
		renderer->update(renderFull);
    } else {
        smgr->drawAll();
    }
}

void CCGenericMonitor::drawGUI() {
	gui->drawAll();
}
