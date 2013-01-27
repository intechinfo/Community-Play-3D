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
    for (int i=0; i < treeNodes.size(); i++) {
        treeNodes[i]->remove();
    }
    for (int i=0; i < objectNodes.size(); i++) {
        objectNodes[i]->remove();
    }
    for (int i=0; i < lightNodes.size(); i++) {
        lightNodes[i]->remove();
    }
}

void CCoreData::clearAllTheArrays() {
    terrainNodes.clear();
    terrainPaths.clear();
    
    treeNodes.clear();
    treePaths.clear();
    
    objectNodes.clear();
    objectPaths.clear();
    
    lightNodes.clear();
    shadowLights.clear();
    
    effectRenders.clear();
    effectRendersPaths.clear();
    effectRenderCallbacks.clear();
}

array<ISceneNode *> CCoreData::getAllSceneNodes() {
    array<ISceneNode *> nodes;
    nodes.clear();
    
    for (int i=0; i < terrainNodes.size(); i++) {
        nodes.push_back(terrainNodes[i]);
    }
    for (int i=0; i < treeNodes.size(); i++) {
        nodes.push_back(treeNodes[i]);
    }
    for (int i=0; i < objectNodes.size(); i++) {
        nodes.push_back(objectNodes[i]);
    }
    for (int i=0; i < lightNodes.size(); i++) {
        nodes.push_back(lightNodes[i]);
    }
    
    return nodes;
}

array<ISceneNode *> CCoreData::getAllMeshes() {
    array<ISceneNode *> nodes;
    nodes.clear();
    
    for (int i=0; i < terrainNodes.size(); i++) {
        nodes.push_back(terrainNodes[i]);
    }
    for (int i=0; i < treeNodes.size(); i++) {
        nodes.push_back(treeNodes[i]);
    }
    for (int i=0; i < objectNodes.size(); i++) {
        nodes.push_back(objectNodes[i]);
    }
    
    return nodes;
}
