//
//  CCoreData.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 25/09/12.
//
//

#include "CCoreData.h"

CCoreData::CCoreData() {
    clearAllTheArrays();
}

CCoreData::~CCoreData() {

}

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
		lightsData[i].getNode()->removeAll();
		lightsData[i].getNode()->remove();
    }
}

void CCoreData::clearAllTheArrays() {
	terrainMeshes.clear();
	terrainMinPolysPerNode.clear();
    terrainNodes.clear();
    terrainPaths.clear();
    
    treesData.clear();
    
	objectsData.clear();
    
    lightsData.clear();

	waterSurfaces.clear();
	planarMeshes.clear();
    
    effectRenders.clear();
    effectRendersPaths.clear();
    effectRenderCallbacks.clear();
    
    shaderCallbacks.clear();
}

array<ISceneNode *> CCoreData::getAllSceneNodes() {
    array<ISceneNode *> nodes;
    nodes.clear();
    
    for (int i=0; i < terrainNodes.size(); i++) {
        nodes.push_back(terrainNodes[i]);
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

void CCoreData::enableRTTWaterSurfaces(bool enable) {

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
