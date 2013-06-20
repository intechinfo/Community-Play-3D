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

void CSceneScripting::addTerrainMeshSceneNode(irr::core::stringc path, ISceneNode *parent) {
	irr::scene::IMesh *mesh = smgr->getMesh(path.c_str());
	if (mesh) {
		IMeshSceneNode *node = smgr->addMeshSceneNode(mesh, parent, -1);
		if (node) {
			node->setName(path.c_str());
			STerrainsData tdata(mesh, node, path, 0, ESNT_MESH);
			devices->getCoreData()->getTerrainsData()->push_back(tdata);
		}
	}
}
void CSceneScripting::addTerrainOctTreeSceneNode(irr::core::stringc path, irr::u32 minPolys, ISceneNode *parent) {
	irr::scene::IMesh *mesh = smgr->getMesh(path.c_str());
	if (mesh) {
		IMeshSceneNode *node = smgr->addOctreeSceneNode(mesh, parent, -1, minPolys);
		if (node) {
			node->setName(path.c_str());
			STerrainsData tdata(mesh, node, path, minPolys, ESNT_OCTREE);
			devices->getCoreData()->getTerrainsData()->push_back(tdata);
		}
	}
}
void CSceneScripting::addTreeMeshSceneNode(irr::core::stringc path, irr::u32 minPolys, ISceneNode *parent) {
	irr::scene::IMesh *mesh = smgr->getMesh(path.c_str());
	if (mesh) {
		IMeshSceneNode *node = smgr->addOctreeSceneNode(mesh, parent, -1, minPolys);
		if (node) {
			node->setName(path.c_str());
			STreesData tdata(mesh, node, path, ESNT_OCTREE, minPolys);
			devices->getCoreData()->getTreesData()->push_back(tdata);
		}
	}
}
void CSceneScripting::addObjectMeshSceneNode(irr::core::stringc path, ISceneNode *parent) {
	IAnimatedMesh *mesh = smgr->getMesh(path.c_str());
	if (mesh) {
		IAnimatedMeshSceneNode *node = smgr->addAnimatedMeshSceneNode(mesh, parent);
		if (node) {
			node->setName(path.c_str());
			SObjectsData odata(mesh, node, path);
			devices->getCoreData()->getObjectsData()->push_back(odata);
		}
	}
}
void CSceneScripting::addLightSceneNode(ISceneNode *parent) {
	ILightSceneNode *node = smgr->addLightSceneNode();
	if (node) {
		node->setName(stringc("#light:new_light").c_str());
		SLightsData ldata(node);
		devices->getCoreData()->getLightsData()->push_back(ldata);

		SShadowLight shadowLight(1024, vector3df(0), vector3df(0));
		devices->getXEffect()->addShadowLight(shadowLight);
	}
}
void CSceneScripting::addVolumeLightSceneNode(ISceneNode *parent) {
	IVolumeLightSceneNode *node = smgr->addVolumeLightSceneNode();
	if (node) {
		node->setName(stringc("#volumeLight:new_volume_light").c_str());
		SVolumeLightsData ldata(node);
		devices->getCoreData()->getVolumeLightsData()->push_back(ldata);
	}
}

//FACTORY
void CSceneScripting::addCubeSceneNode(ISceneNode *parent) {
	ISceneNode *node = smgr->addCubeSceneNode();
	if (node) {
		node->setName(stringc("#object:new_cube").c_str());
		SObjectsData odata(0, node, "cube");
		devices->getCoreData()->getObjectsData()->push_back(odata);
	}
}
void CSceneScripting::addSphereSceneNode(ISceneNode *parent) {
	ISceneNode *node = smgr->addSphereSceneNode();
	if (node) {
		node->setName("#object:new_sphere");
		SObjectsData odata(0, node, "sphere");
		devices->getCoreData()->getObjectsData()->push_back(odata);
	}
}
void CSceneScripting::addHillPlaneMesh(ISceneNode *parent) {
	IAnimatedMesh *mesh = smgr->addHillPlaneMesh("#new_hille_plane_mesh", 
												 dimension2df(25, 25), dimension2du(25, 25),
												 0, 0, dimension2df(0.f, 0.f), dimension2df(20.f, 20.f));
	ISceneNode *node = smgr->addAnimatedMeshSceneNode(mesh);
	if (node) {
		node->setName("#object:new_hill_plane_mesh");
		SObjectsData odata(mesh, node, "hillPlaneMesh");
		devices->getCoreData()->getObjectsData()->push_back(odata);
	}
}
void CSceneScripting::addBillBoardSceneNode(ISceneNode *parent) {
	IBillboardSceneNode *node = smgr->addBillboardSceneNode();
	if (node) {
		node->setName("#object:new_billboard");
		SObjectsData odata(0, node, "billboard");
		devices->getCoreData()->getObjectsData()->push_back(odata);
	}
}

/*

----------------------------------------------------
----------CFILESYSTEM PSEUDO CLASS----------------
----------------------------------------------------CFileSystem = {

}
--Returns table of full file names in directory path
function CFileSystem:getFileList(path, resolve)
    local fs = FileSystem.new()
    local t = {}
    for k,v in ipairs(fs:getFileList(path, resolve)) do
        t[k] = v
    end
    return t
end
--Returns files count of the directory path
function CFileSystem:getFileCount(path, resolve)
    local fs = FileSystem.new()
    return fs:getFileCount(path, resolve)
end
----------------------------------------------------
-------------------MY APPLICATION------------------
----------------------------------------------------
--Adds objects from directory path, using CFileSystem
function addObjectsFromDirectory(path, resolve)
    local files = CFileSystem:getFileList(path, resolve)
    local filesCount = CFileSystem:getFileCount(path, resolve)
    local sn = Scene.new()

    for i=0,filesCount do
        myMesh = sn:loadMesh(files[i])
        sn:addObjectMeshSceneNode(myMesh)
    end
end
--Adds terrains from directory path, using CFileSystem
function addTerrainsFromDirectory(path, resolve)
    local files = CFileSystem:getFileList(path, resolve)
    local filesCount = CFileSystem:getFileCount(path, resolve)
    local sn = Scene.new()

    for i=0,filesCount do
        myMesh = sn:loadMesh(files[i])
        sn:addTerrainMeshSceneNode(myMesh)
    end
end

addObjectsFromDirectory("tata/", true)
addTerrainsFromDirectory("tata/", true)

*/