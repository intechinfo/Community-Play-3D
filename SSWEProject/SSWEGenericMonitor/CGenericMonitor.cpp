//
//  CExporter.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//
#include "stdafx.h"
#include "CGenericMonitor.h"

void* createMonitor() {
	return static_cast< void* > (new CGenericMonitor);
}

CGenericMonitor::CGenericMonitor() {

}

CGenericMonitor::~CGenericMonitor() {

}

void CGenericMonitor::init(irr::IrrlichtDevice *device) {
	monitorName = "Generic Monitor";

	enabled = true;
	renderFullPostTraitements = false;
	renderXEffect = true;
}

void CGenericMonitor::setActiveCamera(irr::scene::ICameraSceneNode *camera) {
	monitorCamera = camera;
}

void CGenericMonitor::setSceneManager(irr::scene::ISceneManager *sceneManager) {
	smgr = sceneManager;
	driver = smgr->getVideoDriver();
}

void CGenericMonitor::drawScene() {
	if (renderXEffect) {
		irr::core::matrix4 viewProj;
		effect->setActiveSceneManager(smgr);
		effect->update(renderFullPostTraitements);
    } else {
        smgr->drawAll();
    }
}

void CGenericMonitor::renderXEffectFullPostTraitement(irr::video::ITexture *texture) {
	if (renderFullPostTraitements && renderXEffect) {
		irr::video::IVideoDriver *driver = smgr->getVideoDriver();
		irr::core::rect<s32> rt1Rect = irr::core::rect<s32>(0, 0, texture->getSize().Width, texture->getSize().Height);
		driver->draw2DImage(texture, rt1Rect, rt1Rect);
	}
}

void CGenericMonitor::drawGUI() {
	gui->drawAll();
}
