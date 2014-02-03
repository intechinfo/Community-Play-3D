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
	return static_cast< void* >(new CCGenericMonitor);
}

<<<<<<< HEAD
CCGenericMonitor::CCGenericMonitor() {

=======
CGenericMonitor::CGenericMonitor() {
    
>>>>>>> 98982be63c0eb4af676bb57f976eb999ecfc21e8
}

CCGenericMonitor::~CCGenericMonitor() {

}

void CCGenericMonitor::init() {
	monitorName = "Generic Monitor";
    
    irr::core::stringc USBPort = "/dev/tty.usbmodem1411";
}

void CCGenericMonitor::setActiveCamera(irr::scene::ICameraSceneNode *camera) {
	monitorCamera = camera;
}

void CCGenericMonitor::setSceneManager(irr::scene::ISceneManager *sceneManager) {
	smgr = sceneManager;
	driver = smgr->getVideoDriver();
}

<<<<<<< HEAD
void CCGenericMonitor::drawScene() {
=======
void CGenericMonitor::drawScene() {
    
>>>>>>> 98982be63c0eb4af676bb57f976eb999ecfc21e8
	if (renderRenderer) {
		renderer->setActiveSceneManager(smgr);
		renderer->update(renderFull);
    } else {
        smgr->drawAll();
    }
    
    irr::ILogger *logger = renderer->getIrrlichtDevice()->getLogger();

}

void CCGenericMonitor::drawGUI() {
	gui->drawAll();
}
