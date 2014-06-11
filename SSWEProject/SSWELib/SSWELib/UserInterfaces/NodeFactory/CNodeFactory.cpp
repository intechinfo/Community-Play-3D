/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include "CNodeFactory.h"

#include "../../../../SSWERenders/Renders/Materials/CNormalMappingMaterial.h"

#include <irrbullet.h>

CNodeFactory::CNodeFactory(CDevices *_devices) {
	devices = _devices;
}

CNodeFactory::~CNodeFactory() {

}

void CNodeFactory::createCubeSceneNode() {
	IMeshSceneNode *cube = devices->getSceneManager()->addCubeSceneNode(50.f);
	cube->setName("#object:new_cube");
	cube->setMaterialFlag(EMF_LIGHTING, false);
	devices->getXEffect()->addShadowToNode(cube, devices->getXEffectFilterType(), ESM_BOTH);
	SObjectsData odata(cube->getMesh(), cube, "cube");
	devices->getCoreData()->getObjectsData()->push_back(odata);
	devices->getCollisionManager()->setCollisionFromBoundingBox(cube);

	devices->getEventReceiver()->sendUserEvent(ECUE_NODE_ADDED);
}

void CNodeFactory::createSphereSceneNode() {
	IMeshSceneNode *sphere = devices->getSceneManager()->addSphereSceneNode();
	sphere->setName("#object:new_sphere");
	sphere->setMaterialFlag(EMF_LIGHTING, false);
	devices->getXEffect()->addShadowToNode(sphere, devices->getXEffectFilterType(), ESM_BOTH);
	SObjectsData odata(sphere->getMesh(), sphere, "sphere");
	devices->getCoreData()->getObjectsData()->push_back(odata);
	devices->getCollisionManager()->setCollisionFromBoundingBox(sphere);

	devices->getEventReceiver()->sendUserEvent(ECUE_NODE_ADDED);
}

void CNodeFactory::createPlaneMeshSceneNode() {
	IAnimatedMesh *planeMesh = devices->getSceneManager()->addHillPlaneMesh("#object:new_hille_plane_mesh",
																			dimension2df(25, 25), dimension2du(25, 25),
																			0, 0, dimension2df(0.f, 0.f), dimension2df(20.f, 20.f));
	IAnimatedMeshSceneNode *planeNode = devices->getSceneManager()->addAnimatedMeshSceneNode(planeMesh, 0, -1, vector3df(0));
	planeNode->setName("#object:new_hille_plane_mesh");
	planeNode->setMaterialFlag(EMF_LIGHTING, false);
	devices->getXEffect()->addShadowToNode(planeNode, devices->getXEffectFilterType(), ESM_RECEIVE);
	SObjectsData odata(planeMesh, planeNode, "hillPlaneMesh");
	devices->getCoreData()->getObjectsData()->push_back(odata);
	devices->getCollisionManager()->setCollisionFromBoundingBox(planeNode);

	devices->getEventReceiver()->sendUserEvent(ECUE_NODE_ADDED);
}

void CNodeFactory::createBillBoardSceneNode() {
	IBillboardSceneNode *billboard = devices->getSceneManager()->addBillboardSceneNode();
	billboard->setName("#object:new_bill_board");
	billboard->setMaterialFlag(EMF_LIGHTING, false);
	devices->getXEffect()->addShadowToNode(billboard, devices->getXEffectFilterType(), ESM_EXCLUDE);
	SObjectsData odata(0, billboard, "billboard");
	devices->getCoreData()->getObjectsData()->push_back(odata);

	devices->getEventReceiver()->sendUserEvent(ECUE_NODE_ADDED);
}

void CNodeFactory::createLightSceneNode() {
	ILightSceneNode *light = devices->getSceneManager()->addLightSceneNode();
	if (light) {
		light->setName("#light:new_light");
		light->setPosition(vector3df(0, 200, 0));
		SLightsData ldata(light, 0, 0, 0);
		devices->getCoreData()->getLightsData()->push_back(ldata);

		SShadowLight shadowLight(1024, vector3df(0,0,0), vector3df(0,0,0), SColor(255, 255, 255, 255), 20.0f, 1000.f, 89.99f * DEGTORAD, false);
		devices->getXEffect()->addShadowLight(shadowLight);

		#ifndef _IRR_WINDOWS_API_
        devices->getNormalMappingMaterial()->addLight(light);
		#endif

		devices->getEventReceiver()->sendUserEvent(ECUE_NODE_ADDED);
	}
}
