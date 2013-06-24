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

CNodeFactory::CNodeFactory(CDevices *_devices) {
	devices = _devices;
}

CNodeFactory::~CNodeFactory() {

}

void CNodeFactory::createCubeSceneNode() {
	IMeshSceneNode *cube = devices->getSceneManager()->addCubeSceneNode();
	cube->setName("#object:new_cube");
	cube->setMaterialFlag(EMF_LIGHTING, false);
	devices->getXEffect()->addShadowToNode(cube, devices->getXEffectFilterType(), ESM_BOTH);
	SObjectsData odata(cube->getMesh(), cube, "cube");
	devices->getCoreData()->getObjectsData()->push_back(odata);
	devices->getCollisionManager()->setCollisionFromBoundingBox(cube);
}

void CNodeFactory::createSphereSceneNode() {
	IMeshSceneNode *sphere = devices->getSceneManager()->addSphereSceneNode();
	sphere->setName("#object:new_sphere");
	sphere->setMaterialFlag(EMF_LIGHTING, false);
	devices->getXEffect()->addShadowToNode(sphere, devices->getXEffectFilterType(), ESM_BOTH);
	SObjectsData odata(sphere->getMesh(), sphere, "sphere");
	devices->getCoreData()->getObjectsData()->push_back(odata);
	devices->getCollisionManager()->setCollisionFromBoundingBox(sphere);
}

void CNodeFactory::createPlaneMeshSceneNode() {
	IAnimatedMesh *planeMesh = devices->getSceneManager()->addHillPlaneMesh("#object:new_hille_plane_mesh", 
																					dimension2df(25, 25), 
																					dimension2du(25, 25));
	ISceneNode *planeNode = devices->getSceneManager()->addMeshSceneNode(planeMesh->getMesh(0), 0, -1, vector3df(0, 0, 0));
	planeNode->setName("#object:new_hille_plane_mesh");
	planeNode->setMaterialFlag(EMF_LIGHTING, false);
	devices->getXEffect()->addShadowToNode(planeNode, devices->getXEffectFilterType(), ESM_RECEIVE);
	SObjectsData odata(planeMesh, planeNode, "hillPlaneMesh");
	devices->getCoreData()->getObjectsData()->push_back(odata);
	devices->getCollisionManager()->setCollisionFromBoundingBox(planeNode);
}

void CNodeFactory::createBillBoardSceneNode() {
	IBillboardSceneNode *billboard = devices->getSceneManager()->addBillboardSceneNode();
	billboard->setName("#object:new_bill_board");
	billboard->setMaterialFlag(EMF_LIGHTING, false);
	devices->getXEffect()->addShadowToNode(billboard, devices->getXEffectFilterType(), ESM_EXCLUDE);
	SObjectsData odata(0, billboard, "billboard");
	devices->getCoreData()->getObjectsData()->push_back(odata);
}

void CNodeFactory::createLightSceneNode() {
	ISceneNode *light = devices->getSceneManager()->addLightSceneNode();
	if (light) {
		light->setName("#light:new_light");
		SLightsData ldata(light, 0, 0, 0);
		devices->getCoreData()->getLightsData()->push_back(ldata);

		SShadowLight shadowLight(1024, vector3df(0,0,0), vector3df(0,0,0), SColor(255, 255, 255, 255), 20.0f, 1000.f, 89.99f * DEGTORAD, false);
		devices->getXEffect()->addShadowLight(shadowLight);
	}
}
