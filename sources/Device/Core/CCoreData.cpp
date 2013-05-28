//
//  CCoreData.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 25/09/12.
//
//

#include "stdafx.h"
#include "CCoreData.h"

CCoreData::CCoreData() {
    clearAllTheArrays();
}

CCoreData::~CCoreData() {

}

//---------------------------------------------------------------------------------------------
//----------------------------------CORE DATA FUNCTIONS --------------------------------------
//---------------------------------------------------------------------------------------------

void CCoreData::clear() {
    for (int i=0; i < terrainNodes.size(); i++) {
        terrainNodes[i]->remove();
    }
    for (int i=0; i < treesData.size(); i++) {
        treesData[i].getNode()->remove();
    }
    for (int i=0; i < objectsData.size(); i++) {
		objectsData[i].getNode()->remove();
    }
    for (int i=0; i < lightsData.size(); i++) {
		lightsData[i].getNode()->remove();
    }
}

void CCoreData::clearAllTheArrays() {
	//NODES
	terrainMeshes.clear();
	terrainMinPolysPerNode.clear();
    terrainNodes.clear();
    terrainPaths.clear();

	terrainsData.clear();
    treesData.clear();
	objectsData.clear();
    lightsData.clear();
	waterSurfaces.clear();

	planarMeshes.clear();

	//EFFECT SHADERS
    effectRenders.clear();
    effectRendersPaths.clear();

	//CALLBACKS
    effectRenderCallbacks.clear();
	materialRenderCallbacks.clear();

    shaderCallbacks.clear();
}

array<ISceneNode *> CCoreData::getAllSceneNodes() {
    array<ISceneNode *> nodes;
    nodes.clear();
    
    for (int i=0; i < terrainNodes.size(); i++) {
        nodes.push_back(terrainNodes[i]);
    }
	for (u32 i=0; i < terrainsData.size(); i++) {
		nodes.push_back(terrainsData[i].getNode());
	}
    for (int i=0; i < treesData.size(); i++) {
		nodes.push_back(treesData[i].getNode());
    }
    for (int i=0; i < objectsData.size(); i++) {
		nodes.push_back(objectsData[i].getNode());
    }
    for (int i=0; i < lightsData.size(); i++) {
		nodes.push_back(lightsData[i].getNode());
    }
	for (int i=0; i < volumeLightsData.size(); i++) {
		nodes.push_back(volumeLightsData[i].getNode());
	}
	for (int i=0; i < waterSurfaces.size(); i++) {
		nodes.push_back(waterSurfaces[i].getNode());
	}
    
    return nodes;
}

array<IMesh *> CCoreData::getAllMeshes() {
    array<IMesh *> meshes;
    meshes.clear();
    
    for (int i=0; i < terrainNodes.size(); i++) {
        meshes.push_back(terrainMeshes[i]);
    }
    for (int i=0; i < treesData.size(); i++) {
		meshes.push_back(treesData[i].getMesh());
    }
    for (int i=0; i < objectsData.size(); i++) {
		meshes.push_back(objectsData[i].getMesh());
    }

    return meshes;
}

s32 CCoreData::isMeshPlanared(ISceneNode *node) {
	s32 planared = -1;

	for (u32 i=0; i < planarMeshes.size(); i++) {
		if (planarMeshes[i] == node) {
			planared = i;
			break;
		}
	}

	return planared;
}

array<ISceneNode *> CCoreData::getArrayOfTreeNodes() {
	array<ISceneNode *> nodes;

	for (u32 i=0 ; i < treesData.size(); i++) {
		nodes.push_back(treesData[i].getNode());
	}

	return nodes;
}

array<ISceneNode *> CCoreData::getArrayOfObjectNodes() {
	array<ISceneNode *> nodes;

	for (u32 i=0; i < objectsData.size(); i++) {
		nodes.push_back(objectsData[i].getNode());
	}

	return nodes;
}

array<ISceneNode *> CCoreData::getArrayOfLightNodes() {
	array<ISceneNode *> nodes;

	for (u32 i=0; i < lightsData.size(); i++) {
		nodes.push_back(lightsData[i].getNode());
	}

	return nodes;
}

//---------------------------------------------------------------------------------------------
//----------------------------------CALLBACKS ------------------------------------------------
//---------------------------------------------------------------------------------------------
