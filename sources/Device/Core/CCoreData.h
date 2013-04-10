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

#include "../../SceneNodes/WaterSurface/CWaterSurface.h"

struct SPlanarTextureMapping {

	SPlanarTextureMapping(f32 _resolutionS, f32 _resolutionT, u8 _axis, vector3df _offset, bool _general=false) {
		resolutionS = _resolutionS;
		resolutionT = _resolutionT;
		axis = _axis;
		offset = _offset;
		general = _general;
	}

	f32 getResolutionS() { return resolutionS; }
	f32 getResolutionT() { return resolutionT; }
	u8 getAxis() { return axis; }
	vector3df getOffset() { return offset; }
	bool isGeneralPlanarTextureMapping() { return general; }

private:
	f32 resolutionS;
	f32 resolutionT;
	u8 axis;
	vector3df offset;
	bool general;
};

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

	s32 isMeshPlanared(ISceneNode *node);
	//-----------------------------------

	//-----------------------------------
	//GET IRRLICHT NODES
	array<IMesh *> *getTerrainMeshes() { return &terrainMeshes; }
	array<u32> *getTerrainMinPolysPerNode() { return &terrainMinPolysPerNode; }
	array<ISceneNode *> *getTerrainNodes() { return &terrainNodes; }
	array<stringw> *getTerrainPaths() { return &terrainPaths; }

	array<ISceneNode *> *getTreeNodes() { return &treeNodes; }
	array<stringw> *getTreePaths() { return &treePaths; }

	array<IMesh *> *getObjectMeshes() { return &objectMeshes; }
	array<ISceneNode *> *getObjectNodes() { return &objectNodes; }
	array<stringw> *getObjectPaths() { return &objectPaths; }

	array<ISceneNode *> *getLightsNodes() { return &lightNodes; }
	array<IMeshSceneNode *> *getLfMeshNodes() { return &lfMeshNodes; }
	array<IBillboardSceneNode *> *getLfBillBoardSceneNodes() { return &lfBillBoardSceneNodes; }
	array<CLensFlareSceneNode *> *getLensFlareSceneNodes() {return &lensFlareSceneNodes; }

	array<IVolumeLightSceneNode *> *getVolumeLights() { return &volumeLightSceneNodes; }

	array<CWaterSurface *> *getWaterSurfaces() { return &waterSurfaceNodes; }

	array<SPlanarTextureMapping> *getPlanarTextureMappingValues() { return &planarTextureMappingValues; }
	array<ISceneNode *> *getPlanarMeshes() { return &planarMeshes; }
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
	array<IMesh *> terrainMeshes;
	array<u32> terrainMinPolysPerNode;
	array<ISceneNode *> terrainNodes;
	array<stringw> terrainPaths;

	array<ISceneNode *> treeNodes;
	array<stringw> treePaths;

	array<IMesh *> objectMeshes;
	array<ISceneNode *> objectNodes;
	array<stringw> objectPaths;

	array<ISceneNode *> lightNodes;
	array<IMeshSceneNode *> lfMeshNodes;
	array<IBillboardSceneNode *> lfBillBoardSceneNodes;
	array<CLensFlareSceneNode *> lensFlareSceneNodes;

	array<IVolumeLightSceneNode *> volumeLightSceneNodes;

	array<CWaterSurface *> waterSurfaceNodes;

	array<SPlanarTextureMapping> planarTextureMappingValues;
	array<ISceneNode *> planarMeshes;
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
