//
//  CUIOpenFileDialog.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//
#include "stdafx.h"

#include "CRenderCore.h"
#include "../CDevices.h"

CRenderCore::CRenderCore(CDevices *_devices) {
	devices = _devices;

	//INITIALIZE MEMBERS
	skybox = 0;
	for (u32 i=0; i < 6; i++)
		rt[i] = devices->getVideoDriver()->addRenderTargetTexture(dimension2du(1024, 1024));

	camera = devices->getSceneManager()->addCameraSceneNode();
	devices->getSceneManager()->setActiveCamera(devices->getMayaCamera());
}

CRenderCore::~CRenderCore() {
	for (u32 i=0; i < 6; i++)
		rt[i]->drop();

	camera->remove();

	skybox->remove();
}

void CRenderCore::update() {
	if (!skybox || lastUpdatePosition.getDistanceFrom(devices->getSceneManager()->getActiveCamera()->getPosition()) > 100.f) {
		ICameraSceneNode* activeCam = devices->getSceneManager()->getActiveCamera();
		devices->getSceneManager()->setActiveCamera(camera);
		camera->setPosition(activeCam->getPosition());
		camera->setAspectRatio(1.0f);
		camera->setFOV(1.573f);
		camera->setFarValue(100000.0f);

		if (skybox)
			skybox->remove();

		devices->getXEffect()->setAllShadowLightsRecalculate();
		//RENDER TOP
		camera->setTarget(camera->getPosition() + vector3df(0, 10, 0));
		camera->setRotation(vector3df(0, -180, 0));
		devices->getXEffect()->update(true, rt[0]);
		camera->setRotation(vector3df(0, 0, 0));
		//RENDER DOWN
		camera->setTarget(camera->getPosition() + vector3df(0, -10, 0));
		devices->getXEffect()->update(true, rt[1]);
		//RENDER LEFT
		camera->setTarget(camera->getPosition() + vector3df(10, 0, 0));
		devices->getXEffect()->update(true, rt[2]);
		//RENDER RIGHT
		camera->setTarget(camera->getPosition() + vector3df(-10, 0, 0));
		devices->getXEffect()->update(true, rt[3]);
		//RENDER FRONT
		camera->setTarget(camera->getPosition() + vector3df(0, 0, -10));
		devices->getXEffect()->update(true, rt[4]);
		//RENDER BACK
		camera->setTarget(camera->getPosition() + vector3df(0, 0, 10));
		devices->getXEffect()->update(true, rt[5]);

		skybox = devices->getSceneManager()->addSkyBoxSceneNode(rt[0], rt[1], rt[2], rt[3], rt[4], rt[5]);

		devices->getSceneManager()->setActiveCamera(activeCam);
		lastUpdatePosition = activeCam->getPosition();
	}
}
