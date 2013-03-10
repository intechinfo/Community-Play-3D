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
#include "../../Renders/XEffect//Interfaces/CShaderCallback.h"

#include "../../SceneNodes/Water Surface/CWaterSurface.h"

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
    array<IMeshSceneNode *> *getLfMeshNodes() { return &lfMeshNodes; }
    array<IBillboardSceneNode *> *getLfBillBoardSceneNodes() { return &lfBillBoardSceneNodes; }
    array<CLensFlareSceneNode *> *getLensFlareSceneNodes() {return &lensFlareSceneNodes; }
    
    array<IVolumeLightSceneNode *> *getVolumeLights() { return &volumeLightSceneNodes; }
    
    array<CWaterSurface *> *getWaterSurfaces() { return &waterSurfaceNodes; }
    //-----------------------------------
    
    //-----------------------------------
    //GET EFFECT SHADERS
    array<s32> *getEffectRenders() { return &effectRenders; }
    array<stringw> *getEffectRendersPaths() { return &effectRendersPaths; }
    
    array<s32> *getMaterialRenders() { return &materialRenders; }
    array<stringw> *getMaterialRenderspaths() { return &materialRendersPaths; }
    
    //GET EFFECT CALLBACKS
    array<CEffectRenderCallback *> *getEffectRenderCallbacks() { return &effectRenderCallbacks; }
    array<CEffectRenderCallback *> *getMaterialRenderCallbacks() { return &materialRenderCallbacks; }
    
    array<CShaderCallback *> *getShaderCallbacks() { return &shaderCallbacks; }
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
    array<IMeshSceneNode *> lfMeshNodes;
    array<IBillboardSceneNode *> lfBillBoardSceneNodes;
    array<CLensFlareSceneNode *> lensFlareSceneNodes;
    
    array<IVolumeLightSceneNode *> volumeLightSceneNodes;
    
    array<CWaterSurface *> waterSurfaceNodes;
    //-----------------------------------
    
    //-----------------------------------
    //EFFECT SHADERS
    array<s32> effectRenders;
    array<stringw> effectRendersPaths;
    
    array<s32> materialRenders;
    array<stringw> materialRendersPaths;
    
    //CALLBACKS
    array<CEffectRenderCallback *> effectRenderCallbacks;
    array<CEffectRenderCallback *> materialRenderCallbacks;
    
    array<CShaderCallback *> shaderCallbacks;
    //-----------------------------------
    
};

#endif
