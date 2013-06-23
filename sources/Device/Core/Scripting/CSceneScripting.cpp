//
//  CRenderCallback.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 28/11/12.
//
//
#include "stdafx.h"
#include "CSceneScripting.h"

#include "../../CDevices.h"

CSceneScripting::CSceneScripting(CDevices *_devices) {
	devices = _devices;
	smgr = devices->getSceneManager();
}

CSceneScripting::~CSceneScripting() {

}

//MESHES
void CSceneScripting::loadMesh(irr::core::stringc path, irr::scene::ESCENE_NODE_TYPE type) {
	irr::scene::IMesh *mesh = smgr->getMesh(path.c_str());
}

s32 CSceneScripting::addTerrainMeshSceneNode(irr::core::stringc path, ISceneNode *parent) {
	irr::scene::IMesh *mesh = smgr->getMesh(path.c_str());
	if (mesh) {
		IMeshSceneNode *node = smgr->addMeshSceneNode(mesh, parent, -1);
		if (node) {
			node->setName(path.c_str());
			STerrainsData tdata(mesh, node, path, 0, ESNT_MESH);
			devices->getCoreData()->getTerrainsData()->push_back(tdata);
			return devices->getCoreData()->getTerrainsData()->size();
		}
	} else {
		return -1;
	}
}
s32 CSceneScripting::addTerrainOctTreeSceneNode(irr::core::stringc path, irr::u32 minPolys, ISceneNode *parent) {
	irr::scene::IMesh *mesh = smgr->getMesh(path.c_str());
	if (mesh) {
		IMeshSceneNode *node = smgr->addOctreeSceneNode(mesh, parent, -1, minPolys);
		if (node) {
			node->setName(path.c_str());
			STerrainsData tdata(mesh, node, path, minPolys, ESNT_OCTREE);
			devices->getCoreData()->getTerrainsData()->push_back(tdata);
			return devices->getCoreData()->getTerrainsData()->size();
		}
	} else {
		return -1;
	}
}
s32 CSceneScripting::addTreeMeshSceneNode(irr::core::stringc path, irr::u32 minPolys, ISceneNode *parent) {
	irr::scene::IMesh *mesh = smgr->getMesh(path.c_str());
	if (mesh) {
		IMeshSceneNode *node = smgr->addOctreeSceneNode(mesh, parent, -1, minPolys);
		if (node) {
			node->setName(path.c_str());
			STreesData tdata(mesh, node, path, ESNT_OCTREE, minPolys);
			devices->getCoreData()->getTreesData()->push_back(tdata);
			return devices->getCoreData()->getTreesData()->size();
		}
	} else {
		return -1;
	}
}
s32 CSceneScripting::addObjectMeshSceneNode(irr::core::stringc path, ISceneNode *parent) {
	IAnimatedMesh *mesh = smgr->getMesh(path.c_str());
	if (mesh) {
		IAnimatedMeshSceneNode *node = smgr->addAnimatedMeshSceneNode(mesh, parent);
		if (node) {
			node->setName(path.c_str());
			SObjectsData odata(mesh, node, path);
			devices->getCoreData()->getObjectsData()->push_back(odata);
			return devices->getCoreData()->getObjectsData()->size();
		}
	} else {
		return -1;
	}
}
s32 CSceneScripting::addLightSceneNode(ISceneNode *parent) {
	ILightSceneNode *node = smgr->addLightSceneNode();
	if (node) {
		node->setName(stringc("#light:new_light").c_str());
		SLightsData ldata(node);
		devices->getCoreData()->getLightsData()->push_back(ldata);

		SShadowLight shadowLight(1024, vector3df(0), vector3df(0));
		devices->getXEffect()->addShadowLight(shadowLight);
		return devices->getCoreData()->getLightsData()->size();
	} else {
		return -1;
	}
}
s32 CSceneScripting::addVolumeLightSceneNode(ISceneNode *parent) {
	IVolumeLightSceneNode *node = smgr->addVolumeLightSceneNode();
	if (node) {
		node->setName(stringc("#volumeLight:new_volume_light").c_str());
		SVolumeLightsData ldata(node);
		devices->getCoreData()->getVolumeLightsData()->push_back(ldata);
		return devices->getCoreData()->getVolumeLightsData()->size();
	} else {
		return -1;
	}
}

u32 CSceneScripting::getTerrainCount() { return devices->getCoreData()->getTerrainsData()->size(); }
u32 CSceneScripting::getTreeCount() { return devices->getCoreData()->getTreesData()->size(); }
u32 CSceneScripting::getObjectCount() { return devices->getCoreData()->getObjectsData()->size(); }
u32 CSceneScripting::getLightCount() { return devices->getCoreData()->getLightsData()->size(); }
u32 CSceneScripting::getVolumeLightCount() { return devices->getCoreData()->getVolumeLightsData()->size(); }

//FACTORY
s32 CSceneScripting::addCubeSceneNode(ISceneNode *parent) {
	ISceneNode *node = smgr->addCubeSceneNode();
	if (node) {
		node->setName(stringc("#object:new_cube").c_str());
		SObjectsData odata(0, node, "cube");
		devices->getCoreData()->getObjectsData()->push_back(odata);
		return devices->getCoreData()->getObjectsData()->size();
	} else {
		return -1;
	}
}
s32 CSceneScripting::addSphereSceneNode(ISceneNode *parent) {
	ISceneNode *node = smgr->addSphereSceneNode();
	if (node) {
		node->setName("#object:new_sphere");
		SObjectsData odata(0, node, "sphere");
		devices->getCoreData()->getObjectsData()->push_back(odata);
		return devices->getCoreData()->getObjectsData()->size();
	} else {
		return -1;
	}
}
s32 CSceneScripting::addHillPlaneMesh(ISceneNode *parent) {
	IAnimatedMesh *mesh = smgr->addHillPlaneMesh("#new_hille_plane_mesh", 
												 dimension2df(25, 25), dimension2du(25, 25),
												 0, 0, dimension2df(0.f, 0.f), dimension2df(20.f, 20.f));
	ISceneNode *node = smgr->addAnimatedMeshSceneNode(mesh);
	if (node) {
		node->setName("#object:new_hill_plane_mesh");
		SObjectsData odata(mesh, node, "hillPlaneMesh");
		devices->getCoreData()->getObjectsData()->push_back(odata);
		return devices->getCoreData()->getObjectsData()->size();
	} else {
		return -1;
	}
}
s32 CSceneScripting::addBillBoardSceneNode(ISceneNode *parent) {
	IBillboardSceneNode *node = smgr->addBillboardSceneNode();
	if (node) {
		node->setName("#object:new_billboard");
		SObjectsData odata(0, node, "billboard");
		devices->getCoreData()->getObjectsData()->push_back(odata);
		return devices->getCoreData()->getObjectsData()->size();
	} else {
		return -1;
	}
}

//METHODS
u32 CSceneScripting::getTerrainNodeMaterialCount(u32 ti) {
	if (devices->getCoreData()->getTerrainsData()->size() > ti) {
		u32 mc = devices->getCoreData()->getTerrainsData()->operator[](ti).getNode()->getMaterialCount()-1;
		return mc;
	} else {
		return -1;
	}
}
s32 CSceneScripting::getTerrainByName(stringc name) {
	s32 i=-1;

	for (u32 t=0; t < devices->getCoreData()->getTerrainsData()->size(); t++) {
		if (stringc(devices->getCoreData()->getTerrainsData()->operator[](t).getNode()->getName()) == name) {
			i = t;
			break;
		}
	}

	return i;
}
stringc CSceneScripting::getTerrainMaterialName(s32 ti, s32 mi) {
	stringc name;
	bool founded = false;
	SMaterial mat = devices->getCoreData()->getTerrainsData()->operator[](ti).getNode()->getMaterial(mi);

	if (mat.MaterialType > devices->getCore()->getNumberOfBuildInMaterialTypes()) {
		for (u32 i=0; i < devices->getCoreData()->getShaderCallbacks()->size(); i++) {
			if (mat.MaterialType == devices->getCoreData()->getShaderCallbacks()->operator[](i)->getMaterial()) {
				name = devices->getCoreData()->getShaderCallbacks()->operator[](i)->getName();
				break;
			}
		}
	} else {
		name = devices->getCore()->getArrayOfBuildInMaterialTypes()[mat.MaterialType];
	}

	name.make_upper();
	return name;
}
void CSceneScripting::setTerrainMaterialNumber(s32 ti, s32 mi, s32 mati) {
	devices->getCoreData()->getTerrainsData()->operator[](ti).getNode()->getMaterial(mi).MaterialType = (E_MATERIAL_TYPE)mati;
}
s32 CSceneScripting::getMaterialIDByName(stringc name) {
	s32 i=0;
	bool founded = false;

	for (u32 bi = 0; bi < devices->getCore()->getNumberOfBuildInMaterialTypes(); bi++) {
		if (devices->getCore()->getArrayOfBuildInMaterialTypes()[bi] == name) {
			i = bi;
			founded = true;
			break;
		}
	}

	if (!founded) {
		for (u32 ci = 0; ci < devices->getCoreData()->getShaderCallbacks()->size(); ci++) {
			if (devices->getCoreData()->getShaderCallbacks()->operator[](ci)->getName() == name) {
				i = devices->getCoreData()->getShaderCallbacks()->operator[](ci)->getMaterial();
				break;
			}
		}
	}

	return i;
}
