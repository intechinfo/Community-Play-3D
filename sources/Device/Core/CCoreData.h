//
//  CCoreData.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 25/09/12.
//
//

#ifndef __C_CORE_DATA_H_INCLUDED__
#define __C_CORE_DATA_H_INCLUDED__

#include "CCore.h"
#include "../../Renders/XEffect/Interfaces/CRenderCallback.h"

class CCoreData {
    
public:
    
    CCoreData();
    ~CCoreData();
    
    //-----------------------------------
    //METHODS
    void clearAllTheArrays();
    void clear();
    
    array<ISceneNode *> getAllSceneNodes();
    array<ISceneNode *> getAllMeshes();
    //-----------------------------------
    
    //-----------------------------------
    //GET IRRLICHT NODES
    array<ISceneNode *> *getTerrainNodes() { return &terrainNodes; }
    array<stringw> *getTerrainPaths() { return &terrainPaths; }
    
    array<ISceneNode *> *getTreeNodes() { return &treeNodes; }
    array<stringw> *getTreePaths() { return &treePaths; }
    
    array<ISceneNode *> *getObjectNodes() { return &objectNodes; }
    array<stringw> *getObjectPaths() { return &objectPaths; }
    
    array<ISceneNode *> *getLightsNodes() { return &lightNodes; }
    array<SShadowLight> *getShadowLights() { return &shadowLights; }
    
    array<s32> *getDynamicLights() { return &dynamicLights; }
    //-----------------------------------
    
    //-----------------------------------
    //GET EFFECT SHADERS
    array<s32> *getEffectRenders() { return &effectRenders; }
    array<stringw> *getEffectRendersPaths() { return &effectRendersPaths; }
    
    array<CEffectRenderCallback *> *getEffectRenderCallbacks() { return &effectRenderCallbacks; }
    //-----------------------------------
    
private:
    
    //-----------------------------------
    //IRRLICHT NODES
    array<ISceneNode *> terrainNodes;
    array<stringw> terrainPaths;
    
    array<ISceneNode *> treeNodes;
    array<stringw> treePaths;
    
    array<ISceneNode *> objectNodes;
    array<stringw> objectPaths;
    
    array<ISceneNode *> lightNodes;
    array<SShadowLight> shadowLights;
    
    array<s32> dynamicLights;
    //-----------------------------------
    
    //-----------------------------------
    //EFFECT SHADERS
    array<s32> effectRenders;
    array<stringw> effectRendersPaths;
    
    array<CEffectRenderCallback *> effectRenderCallbacks;
    //-----------------------------------
    
};

#endif
