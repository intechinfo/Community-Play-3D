//
//  CCoreData.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 25/09/12.
//
//

#ifndef __C_CORE_DATA_H_INCLUDED__
#define __C_CORE_DATA_H_INCLUDED__

#include "stdafx.h"
#include "CCore.h"

#include "../../Renders/XEffect/Interfaces/CRenderCallback.h"
#include "../../Renders/XEffect//Interfaces/CShaderCallback.h"

#include "../../SceneNodes/WaterSurface/CWaterSurface.h"

#include "../../CharacterEdition/CAction.h"

//---------------------------------------------------------------------------------------------
//-----------------------------------PLANAR MAPPING-------------------------------------------
//---------------------------------------------------------------------------------------------
struct SPlanarTextureMappingData {

	SPlanarTextureMappingData(f32 _resolutionS, f32 _resolutionT, u8 _axis, vector3df _offset, bool _general=false) {
		resolutionS = _resolutionS;
		resolutionT = _resolutionT;
		axis = _axis;
		offset = _offset;
		general = _general;
	}

	void setResolutionS(f32 _resolutionS) { resolutionS = _resolutionS; }
	void setResolutionT(f32 _resolutionT) { resolutionT = _resolutionT; }
	void setAxis(u8 _axis) { axis = _axis; }
	void setOffset(vector3df _offset) { offset = _offset; }
	void setIsGeneralPlanarTextureMapping(bool _general) { general = _general; }

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

struct SMeshWithTangentsData {

	SMeshWithTangentsData(ISceneNode *_node, bool _recalculateNormals=false, bool _smooth=false, bool _angleWeighted=false, 
						  bool _recalculateTangents=true) {
		node = _node;
		recalculateNormals = _recalculateNormals;
		smooth = _smooth;
		angleWeighted = _angleWeighted;
		recalculateTangents = _recalculateTangents;
	}

	void setNode(ISceneNode *_node) { node = _node; }
	void setIsNormalsRecalculated(bool _recalculateNormals) { recalculateNormals = _recalculateNormals; }
	void setIsSmoothed(bool _smooth) { smooth = _smooth; }
	void setIsAngleWeighted(bool _angleWeighted) { angleWeighted = _angleWeighted; }
	void setIsTangentsRecalculated(bool _recalculateTangents) { recalculateTangents = _recalculateTangents; }

	ISceneNode *getNode() { return node; }
	bool isNormalsRecalculated() { return recalculateNormals; }
	bool isSmoothed() { return smooth; }
	bool isAngleWeighted() { return angleWeighted; }
	bool isTangentsRecalculated() { return recalculateTangents; }

private:
	ISceneNode *node;
	
	bool recalculateNormals;
	bool smooth;
	bool angleWeighted;
	bool recalculateTangents;
};

//---------------------------------------------------------------------------------------------
//----------------------------------TERRAINS--------------------------------------------------
//---------------------------------------------------------------------------------------------

struct STerrainsData {
	
	STerrainsData(IMesh *_mesh, ISceneNode *_node, stringw _path, u32 _minPolysPerNode=0, ESCENE_NODE_TYPE _type=ESNT_MESH) {
		mesh = _mesh;
		node = _node;
		path = _path;
		minPolysPerNode = _minPolysPerNode;
		type = _type;
	}

	void setMesh(IMesh *_mesh) { mesh = _mesh; }
	void setNode(ISceneNode *_node) { node = _node; }
	void setPath(stringw _path) { path = _path; }
	void setMinPolysPerNode(u32 _minPolysPerNode) { minPolysPerNode = _minPolysPerNode; }
	void setType(ESCENE_NODE_TYPE _type) { type = _type; }

	IMesh *getMesh() { return mesh; }
	ISceneNode *getNode() { return node; }
	stringw getPath() { return path; }
	u32 getMinPolysPerNode() { return minPolysPerNode; }
	ESCENE_NODE_TYPE getType() { return type; }

private:
	IMesh *mesh;
	ISceneNode *node;
	u32 minPolysPerNode;
	stringw path;
	ESCENE_NODE_TYPE type;
};

//---------------------------------------------------------------------------------------------
//----------------------------------TREES---------------------------------------------------
//---------------------------------------------------------------------------------------------

struct STreesData {
	STreesData(IMesh *_mesh, ISceneNode *_node, stringw _path, ESCENE_NODE_TYPE _type=ESNT_OCTREE, u32 _minPolysPerNode=0) {
		mesh = _mesh;
		node = _node;
		path = _path;
		type = _type;
		minPolysPerNode = _minPolysPerNode;
	}

	STreesData(STerrainsData terrainData) {
		mesh = terrainData.getMesh();
		node = terrainData.getNode();
		minPolysPerNode = terrainData.getMinPolysPerNode();
		path = terrainData.getPath();

		type = ESNT_OCTREE;
	}

	void setMesh(IMesh *_mesh) { mesh = _mesh; }
	void setNode(ISceneNode *_node) { node = _node; }
	void setPath(stringw _path) { path = _path; }
	void setMinPolysPerNode(u32 _minPolysPerNode) { minPolysPerNode = _minPolysPerNode; }
	void setType(ESCENE_NODE_TYPE _type) { type = _type; }

	IMesh *getMesh() { return mesh; }
	ISceneNode *getNode() { return node; }
	u32 getMinPolysPerNode() { return minPolysPerNode; }
	stringw getPath() { return path; }
	ESCENE_NODE_TYPE getType() { return type; }

private:
	IMesh *mesh;
	ISceneNode *node;
	u32 minPolysPerNode;
	stringw path;
	ESCENE_NODE_TYPE type;
};

//---------------------------------------------------------------------------------------------
//----------------------------------OBJECTS---------------------------------------------------
//---------------------------------------------------------------------------------------------

struct SObjectsData {

	SObjectsData(IMesh *_mesh, ISceneNode *_node, stringw _path) {
		mesh = _mesh;
		node = _node;
		path = _path;
	}

	void setMesh(IMesh *_mesh) { mesh = _mesh; }
	void setNode(ISceneNode *_node) { node = _node; }
	void setPath(stringw _path) { path = _path; }

	IMesh *getMesh() { return mesh; }
	ISceneNode *getNode() { return node; }
	stringw getPath() { return path; }

	array<CAction *> *getActions() { return &actions; }

private:
	IMesh *mesh;
	ISceneNode *node;
	stringw path;

	array<CAction *> actions;
};

//---------------------------------------------------------------------------------------------
//----------------------------------LIGHTS----------------------------------------------------
//---------------------------------------------------------------------------------------------

struct SLightsData {

	SLightsData(ISceneNode *_node, IMeshSceneNode *_lensFlareMeshSceneNode=0, IBillboardSceneNode *_lensFlareBillBoardSceneNode=0,
				CLensFlareSceneNode *_lensFlareSceneNode=0) {
		node = _node;
		lensFlareMeshSceneNode  = _lensFlareMeshSceneNode;
		lensFlareBillBoardSceneNode = _lensFlareBillBoardSceneNode;
		lensFlareSceneNode = _lensFlareSceneNode;
	}

	ISceneNode *getNode() { return node; }
	IMeshSceneNode *getLensFlareMeshSceneNode() { return lensFlareMeshSceneNode; }
	IBillboardSceneNode *getLensFlareBillBoardSceneNode() { return lensFlareBillBoardSceneNode; }
	CLensFlareSceneNode *getLensFlareSceneNode() { return lensFlareSceneNode; }

	void setLight(ISceneNode *_node) { node = _node; }
	void setLensFlareMeshSceneNode(IMeshSceneNode *_node) { lensFlareMeshSceneNode = _node; }
	void setLensFlareBillboardSceneNode(IBillboardSceneNode *_node) { lensFlareBillBoardSceneNode = _node; }
	void setLensFlareSceneNode(CLensFlareSceneNode *_node) { lensFlareSceneNode = _node; }

private:
	ISceneNode *node;
	IMeshSceneNode *lensFlareMeshSceneNode;
	IBillboardSceneNode *lensFlareBillBoardSceneNode;
	CLensFlareSceneNode *lensFlareSceneNode;
};

//---------------------------------------------------------------------------------------------
//----------------------------------VOLUME LIGHTS---------------------------------------------
//---------------------------------------------------------------------------------------------

struct SVolumeLightsData {

	SVolumeLightsData(IVolumeLightSceneNode *_node) {
		node = _node;
	}

	ISceneNode *getNode() { return node; }
	
	void setNode(ISceneNode *_node) { node = _node; }

private:
	ISceneNode *node;
};

//---------------------------------------------------------------------------------------------
//----------------------------------WATER SURFACES--------------------------------------------
//---------------------------------------------------------------------------------------------

struct SWaterSurfacesData {
public:
	SWaterSurfacesData(CWaterSurface *_waterSurface, CShaderCallback *_callback, stringw _packagePath = L"", stringw _meshPath = L"") {
		waterSurface = _waterSurface;
		callback = _callback;
		packagePath = _packagePath;
		meshPath = _meshPath;
    }

	void remove() { delete this; }
	void setwaterSurface(CWaterSurface *_waterSurface) { waterSurface = _waterSurface; }
	void setCallback(CShaderCallback *_callback) { callback = _callback; }
	void setPackagePath(stringw _packagePath) { packagePath = _packagePath; }
	void setMeshPath(stringw _meshPath) { meshPath = _meshPath; }

	CWaterSurface *getWaterSurface() { return waterSurface; }
	IMesh *getMesh() { return waterSurface->getWaterMesh(); }
	ISceneNode *getNode() { return waterSurface->getWaterNode(); }
	CShaderCallback *getShaderCallback() { return callback; }
	stringw getMeshPath() { return meshPath; }
	stringw getPackagePath() { return packagePath; }

private:
	CWaterSurface *waterSurface;
	CShaderCallback *callback;
	stringw packagePath;
	stringw meshPath;

};

//---------------------------------------------------------------------------------------------
//----------------------------------CORE DATA CLASS-------------------------------------------
//---------------------------------------------------------------------------------------------

class SSWE_CORE_API CCoreData {

public:

	CCoreData();
	~CCoreData();

	//-----------------------------------
	//METHODS
	void clearAllTheArrays();
	void clear();

	array<ISceneNode *> getAllSceneNodes();
	array<IMesh *> getAllMeshes();

	s32 isMeshPlanared(ISceneNode *node);

	array<ISceneNode *> getArrayOfLightNodes();
	array<ISceneNode *> getArrayOfTreeNodes();
	array<ISceneNode *> getArrayOfObjectNodes();
	//-----------------------------------

	//-----------------------------------
	//GET IRRLICHT NODES
	array<STerrainsData> *getTerrainsData() { return &terrainsData; }
	array<IMesh *> *getTerrainMeshes() { return &terrainMeshes; }
	array<u32> *getTerrainMinPolysPerNode() { return &terrainMinPolysPerNode; }
	array<ISceneNode *> *getTerrainNodes() { return &terrainNodes; }
	array<stringw> *getTerrainPaths() { return &terrainPaths; }

	array<STreesData> *getTreesData() { return &treesData; }

	array<SObjectsData> *getObjectsData() { return &objectsData; }

	array<SLightsData> *getLightsData() { return &lightsData; }

	array<SVolumeLightsData> *getVolumeLightsData() { return &volumeLightsData; }

	array<SWaterSurfacesData> *getWaterSurfaces() { return &waterSurfaces; }

	array<SPlanarTextureMappingData> *getPlanarTextureMappingValues() { return &planarTextureMappingValues; }
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
	array<STerrainsData> terrainsData;
	array<IMesh *> terrainMeshes;
	array<u32> terrainMinPolysPerNode;
	array<ISceneNode *> terrainNodes;
	array<stringw> terrainPaths;

	array<STreesData> treesData;

	array<SObjectsData> objectsData;

	array<SLightsData> lightsData;

	array<SVolumeLightsData> volumeLightsData;

	array<SWaterSurfacesData> waterSurfaces;

	array<SPlanarTextureMappingData> planarTextureMappingValues;
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
