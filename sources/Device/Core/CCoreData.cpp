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
    for (int i=0; i < objectNodes.size(); i++) {
        objectNodes[i]->remove();
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
    
    objectNodes.clear();
    objectPaths.clear();
    
    lightsData.clear();

	waterSurfaceNodes.clear();
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
    for (int i=0; i < objectNodes.size(); i++) {
        nodes.push_back(objectNodes[i]);
    }
    for (int i=0; i < lightsData.size(); i++) {
		nodes.push_back(lightsData[i].getNode());
    }
    
    return nodes;
}

array<ISceneNode *> CCoreData::getAllMeshes() {
    array<ISceneNode *> nodes;
    nodes.clear();
    
    for (int i=0; i < terrainNodes.size(); i++) {
        nodes.push_back(terrainNodes[i]);
    }
    for (int i=0; i < treesData.size(); i++) {
		nodes.push_back(treesData[i].getNode());
    }
    for (int i=0; i < objectNodes.size(); i++) {
        nodes.push_back(objectNodes[i]);
    }

    return nodes;
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

array<ISceneNode *> CCoreData::getArrayOfLightNodes() {
	array<ISceneNode *> nodes;

	for (u32 i=0; i < lightsData.size(); i++) {
		nodes.push_back(lightsData[i].getNode());
	}

	return nodes;
}
