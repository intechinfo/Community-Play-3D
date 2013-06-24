//
//  CExporter.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//
#include "stdafx.h"
#include "CGenericMonitor.h"

CGenericMonitor::CGenericMonitor() {
	enabled = true;
	renderFullPostTraitements = false;
	renderXEffect = true;
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
}

void CGenericMonitor::drawScene() {
	if (renderXEffect) {
		irr::core::matrix4 viewProj;
		effect->setActiveSceneManager(smgr);
		effect->update(renderFullPostTraitements);
    } else {
        smgr->drawAll();
    }

    effectSmgr->drawAll();
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