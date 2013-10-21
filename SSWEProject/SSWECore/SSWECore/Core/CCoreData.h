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

#include "../../../SSWELib/SSWELib/Renders/XEffect/Interfaces/CRenderCallback.h"
#include "../../../SSWELib/SSWELib/Renders/XEffect/Interfaces/CShaderCallback.h"
#include "../../../SSWERenders/Renders/XEffect/EffectCB.h"

#include "../../../SSWELib/SSWELib/SceneNodes/Clouds/CloudSceneNode.h"
#include "../../../SSWELib/SSWELib/SceneNodes/LensFlareSceneNode.h"
#include "../../../SSWELib/SSWELib/SceneNodes/SceneNodeAnimatorFollowCamera.h"
#include "../../../SSWELib/SSWELib/SceneNodes/WaterSurface/CWaterSurface.h"
#include "../../../SSWELib/SSWELib/SceneNodes/Terrains/CTerrainPager.h"

#include "../../../SSWELib/SSWELib/CharacterEdition/CAction.h"

#include <IMonitor.h>

#include "CCorePhysics.h"

//---------------------------------------------------------------------------------------------
//-----------------------------------HERITANCES------------------------------------------------
//---------------------------------------------------------------------------------------------

struct SData {

	SData() {
		SData(0, 0, "", ESNT_UNKNOWN);
	}

	SData(ISceneNode *_node, IMesh *_mesh, stringc _path, ESCENE_NODE_TYPE _type) {
		node = _node;
		mesh = _mesh;
		path = _path;
		type = _type;
		clonedNodes.clear();
	}

	//MAIN METHODS
	ISceneNode *getNode() { return node; }
	IMesh *getMesh() { return mesh; }
	stringc getPath() { return path; }
	ESCENE_NODE_TYPE getType() { return type; }

	void setNode(ISceneNode *_node) { node = _node; }
	void setMesh(IMesh *_mesh) { mesh = _mesh; }
	void setPath(stringw _path) { path = _path; }
	void setType(ESCENE_NODE_TYPE _type) { type = _type; }

	//CLONED NODES METHODS
	ISceneNode *cloneNode(vector3df position, CCore *core=0) {
		ISceneNode *cnode = 0;
		if (core)
			cnode = core->clone(node, path, core->getDevice()->getSceneManager());
		else
			cnode = node->clone();
		
		if (cnode) {
			cnode->setPosition(position);
			clonedNodes.push_back(cnode);
		}

		return cnode;
	}
	void addClonedNode(ISceneNode *cnode) {
		clonedNodes.push_back(cnode);
	}
	void removeClonedNode(ISceneNode *cnode) {
		ISceneNodeList::Iterator it = clonedNodes.begin();
		for (; it != clonedNodes.end(); ++it)
			if ((*it) == cnode) {
				(*it)->remove();
				clonedNodes.erase(it);
				break;
			}
	}
	ISceneNode *getClonedNode(ISceneNode *cnode) {
		ISceneNodeList::Iterator it = clonedNodes.begin();
		for (; it != clonedNodes.end(); ++it)
			if ((*it) == cnode) {
				return *it;
				break;
			}
	}
	ISceneNode *getClonedNode(u32 index) {
		u32 currentIndex;
		ISceneNodeList::Iterator it = clonedNodes.begin();
		for (currentIndex = 0; it != clonedNodes.end() && currentIndex < index; ++it)
			currentIndex++;

		return (*it);
	}
	u32 getClonedNodeCount() { return clonedNodes.size(); }
	ISceneNodeList *getClonedNodeList() { return &clonedNodes; }

private:
	ISceneNode *node;
	IMesh *mesh;
	stringc path;
	ESCENE_NODE_TYPE type;

	ISceneNodeList clonedNodes;
};

//---------------------------------------------------------------------------------------------
//-----------------------------------PLANAR MAPPING--------------------------------------------
//---------------------------------------------------------------------------------------------
struct SPlanarTextureMappingData {

	SPlanarTextureMappingData() {
		SPlanarTextureMappingData(0, 0, 0, 0, vector3df(0));
	}

	SPlanarTextureMappingData(ISceneNode *_node, f32 _resolutionS, f32 _resolutionT, u8 _axis, vector3df _offset) {
		node = _node;

		resolutionS = _resolutionS;
		resolutionT = _resolutionT;
		axis = _axis;
		offset = _offset;
		general = false;
	}

	SPlanarTextureMappingData(ISceneNode *_node, f32 _resolutionS) {
		SPlanarTextureMappingData(0, 0, 0, 0, vector3df(0));

		node = _node;

		resolutionS = _resolutionS;
		general = true;
	}

	void setNode(ISceneNode *_node) { node = _node; }
	void setResolutionS(f32 _resolutionS) { resolutionS = _resolutionS; }
	void setResolutionT(f32 _resolutionT) { resolutionT = _resolutionT; }
	void setAxis(u8 _axis) { axis = _axis; }
	void setOffset(vector3df _offset) { offset = _offset; }
	void setIsGeneralPlanarTextureMapping(bool _general) { general = _general; }

	ISceneNode *getNode() { return node; }
	f32 getResolutionS() { return resolutionS; }
	f32 getResolutionT() { return resolutionT; }
	u8 getAxis() { return axis; }
	vector3df getOffset() { return offset; }
	bool isGeneralPlanarTextureMapping() { return general; }

private:
	ISceneNode *node;

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
//MESHES
struct STerrainsData : public SData {
	
	STerrainsData() : SData(0, 0, "", ESNT_UNKNOWN) {
		STerrainsData(0, 0, "", 0, ESNT_UNKNOWN);
	}

	STerrainsData(IMesh *_mesh, ISceneNode *_node, stringw _path, u32 _minPolysPerNode=0, ESCENE_NODE_TYPE _type=ESNT_MESH)
		: SData(_node, _mesh, _path, _type)
	{
		minPolysPerNode = _minPolysPerNode;
	}

	void setMinPolysPerNode(u32 _minPolysPerNode) { minPolysPerNode = _minPolysPerNode; }
	u32 getMinPolysPerNode() { return minPolysPerNode; }

private:
	u32 minPolysPerNode;
};

//HEIGHT MAPS
struct STerrainHMData : STerrainsData {

	STerrainHMData() : STerrainsData(0, 0, "", ESNT_UNKNOWN) {
		STerrainHMData(0, 0, "", ETPS_9);
	}

	STerrainHMData(IMesh *_mesh, ITerrainSceneNode *_node, stringw _path, E_TERRAIN_PATCH_SIZE _patchSize)
		: STerrainsData(_mesh, _node, _path, ESNT_TERRAIN)
	{
		patchSize = _patchSize;
	}

	void setPatchSize(E_TERRAIN_PATCH_SIZE _patchSize) { patchSize = _patchSize; }
	E_TERRAIN_PATCH_SIZE getPatchSize() { return patchSize; }

private:
	E_TERRAIN_PATCH_SIZE patchSize;
};

//---------------------------------------------------------------------------------------------
//----------------------------------TREES---------------------------------------------------
//---------------------------------------------------------------------------------------------

struct SSWE_CORE_API STreesData : SData {

	STreesData(IMesh *_mesh, ISceneNode *_node, stringw _path, ESCENE_NODE_TYPE _type=ESNT_OCTREE, u32 _minPolysPerNode=0)
		: SData(_node, _mesh, _path, _type)
	{
		SData::setNode(_node);
		SData::setMesh(_mesh);
		SData::setPath(_path);
		SData::setType(_type);
		minPolysPerNode = _minPolysPerNode;
	}

	STreesData(STerrainsData terrainData) : SData(terrainData.getNode(), terrainData.getMesh(), terrainData.getPath(), terrainData.getType()) {
		minPolysPerNode = terrainData.getMinPolysPerNode();
	}

	void setMinPolysPerNode(u32 _minPolysPerNode) { minPolysPerNode = _minPolysPerNode; }
	u32 getMinPolysPerNode() { return minPolysPerNode; }

private:
	u32 minPolysPerNode;
};

//---------------------------------------------------------------------------------------------
//----------------------------------OBJECTS---------------------------------------------------
//---------------------------------------------------------------------------------------------

struct SObjectsData : SData {

	SObjectsData() : SData(0, 0, "", ESNT_UNKNOWN) {
		SObjectsData(0, 0, "");
	}

	SObjectsData(IMesh *_mesh, ISceneNode *_node, stringw _path)
		: SData(_node, _mesh, _path, ESNT_ANIMATED_MESH)
	{
		actions.clear();
	}

	void setActions(array<CAction *> *_actions) {
		actions.clear();
		for (u32 i=0; i < _actions->size(); i++) {
			actions.push_back(_actions->operator[](i));
		}
	}

	array<CAction *> *getActions() { return &actions; }

private:
	array<CAction *> actions;
};

//---------------------------------------------------------------------------------------------
//----------------------------------LIGHTS-----------------------------------------------------
//---------------------------------------------------------------------------------------------

struct SSWE_CORE_API SLightsData : SData {

	SLightsData(ISceneNode *_node, IMeshSceneNode *_lensFlareMeshSceneNode=0, 
				IBillboardSceneNode *_lensFlareBillBoardSceneNode=0,
				CLensFlareSceneNode *_lensFlareSceneNode=0)
				: SData(_node, 0, L"", ESNT_LIGHT)
	{
		lfdata.setLensFlareMeshSceneNode(_lensFlareMeshSceneNode);
		lfdata.setLensFlareBillboardSceneNode(_lensFlareBillBoardSceneNode);
		lfdata.setLensFlareSceneNode(_lensFlareSceneNode);
	}

	// LENS FLARE
	IMeshSceneNode *getLensFlareMeshSceneNode() { return lfdata.getLensFlareMeshSceneNode(); }
	IBillboardSceneNode *getLensFlareBillBoardSceneNode() { return lfdata.getLensFlareBillBoardSceneNode(); }
	CLensFlareSceneNode *getLensFlareSceneNode() { return lfdata.getLensFlareSceneNode(); }
	f32 getLensFlareStrengthFactor() { return lfdata.getStrengthFactor(); }

	void setLensFlareMeshSceneNode(IMeshSceneNode *_node) { lfdata.setLensFlareMeshSceneNode(_node); }
	void setLensFlareBillboardSceneNode(IBillboardSceneNode *_node) { lfdata.setLensFlareBillboardSceneNode(_node); }
	void setLensFlareSceneNode(CLensFlareSceneNode *_node) { lfdata.setLensFlareSceneNode(_node); }
	void setLensFlareStrengthFactor(f32 strengthFactor) { lfdata.setStrengthFactor(strengthFactor); }

private:

	ISceneNode *node;

	struct SLensFlare {
	public:
		IMeshSceneNode *getLensFlareMeshSceneNode() { return lensFlareMeshSceneNode; }
		IBillboardSceneNode *getLensFlareBillBoardSceneNode() { return lensFlareBillBoardSceneNode; }
		CLensFlareSceneNode *getLensFlareSceneNode() { return lensFlareSceneNode; }
		f32 getStrengthFactor() { return strengthFactor; }

		void setLensFlareMeshSceneNode(IMeshSceneNode *_node) { lensFlareMeshSceneNode = _node; }
		void setLensFlareBillboardSceneNode(IBillboardSceneNode *_node) { lensFlareBillBoardSceneNode = _node; }
		void setLensFlareSceneNode(CLensFlareSceneNode *_node) { lensFlareSceneNode = _node; }
		void setStrengthFactor(f32 _strengthFactor) { strengthFactor = _strengthFactor; }
	private:
		IMeshSceneNode *lensFlareMeshSceneNode;
		IBillboardSceneNode *lensFlareBillBoardSceneNode;
		CLensFlareSceneNode *lensFlareSceneNode;
		f32 strengthFactor;
	} lfdata;
};

//---------------------------------------------------------------------------------------------
//----------------------------------VOLUME LIGHTS---------------------------------------------
//---------------------------------------------------------------------------------------------

struct SSWE_CORE_API SVolumeLightsData : SData {

	SVolumeLightsData(IVolumeLightSceneNode *_node) : SData(_node, 0, L"", ESNT_VOLUME_LIGHT)
	{

	}

private:

};

//---------------------------------------------------------------------------------------------
//----------------------------------WATER SURFACES---------------------------------------------
//---------------------------------------------------------------------------------------------

struct SWaterSurfacesData : SData {
public:
	SWaterSurfacesData(CWaterSurface *_waterSurface, CShaderCallback *_callback, stringw _packagePath = L"", stringw _meshPath = L"")
		: SData(_waterSurface->getWaterNode(), _waterSurface->getWaterMesh(), _meshPath, ESNT_WATER_SURFACE)
	{
		waterSurface = _waterSurface;
		callback = _callback;
		packagePath = _packagePath;
    }

	//void remove() { delete this; }
	void setwaterSurface(CWaterSurface *_waterSurface) { waterSurface = _waterSurface; }
	void setCallback(CShaderCallback *_callback) { callback = _callback; }
	void setPackagePath(stringw _packagePath) { packagePath = _packagePath; }

	CWaterSurface *getWaterSurface() { return waterSurface; }
	CShaderCallback *getShaderCallback() { return callback; }
	stringw getPackagePath() { return packagePath; }

private:
	CWaterSurface *waterSurface;
	CShaderCallback *callback;
	stringw packagePath;
};

//---------------------------------------------------------------------------------------------
//-----------------------------------------OTHERS----------------------------------------------
//---------------------------------------------------------------------------------------------

struct SScriptFile;

struct SOther : SData {
public:
	SOther(stringw _name) : SData(0, 0, L"", ESNT_UNKNOWN)
	{
		name = _name;
	}
	SOther(stringw _name, IMesh *mesh, ISceneNode *node, stringw path, ESCENE_NODE_TYPE type)
		: SData(node, mesh, path, type)
	{
		name = _name;
	}

	void setName(stringw _name) { name = _name; }
	void setScript(stringw _script) { script = _script; }

	stringw getName() { return name; }
	stringw getScript() { return script; }

private:
	stringw name;
	stringw script;
};

//---------------------------------------------------------------------------------------------
//-----------------------------------------SUNS------------------------------------------------
//---------------------------------------------------------------------------------------------

struct SSun : SData {
public:
	SSun(stringw _name) : SData(0, 0, L"", ESNT_LIGHT)
	{

	}

private:

};

//---------------------------------------------------------------------------------------------
//----------------------------------SCRIPTS----------------------------------------------------
//---------------------------------------------------------------------------------------------

struct SScriptFile {
public:
	SScriptFile(stringw _file, stringw _name) {
		file = _file;
		name = _name;
	}

	stringw getFile() { return file; }
	stringw getName() { return name; }
	stringw *getFilePointer() { return &file; }
	stringw *getNamePointer() { return &name; }

	void setFile(stringw _file) { file = _file; }
	void setName(stringw _name) { name = _name; }

private:
	stringw file;
	stringw name;
};

//---------------------------------------------------------------------------------------------
//----------------------------------PLUGINS----------------------------------------------------
//---------------------------------------------------------------------------------------------

struct SMonitor {
public:
	SMonitor(IMonitor *_monitor, HINSTANCE _hdll) {
		monitor = _monitor;
		hdll = _hdll;
	}

	SMonitor() {
		SMonitor(0, 0);
	}

	IMonitor *getMonitor() { return monitor; }
	void setMonitor(IMonitor *_monitor) { monitor = _monitor; }

	HINSTANCE getInstance() { return hdll; }
	void setInstance(HINSTANCE _hdll) { hdll = _hdll; }

	void freeInstance() {
		if (hdll) {
			FreeLibrary(hdll);
		}
	}

private:

	IMonitor *monitor;
	HINSTANCE hdll;
};

//---------------------------------------------------------------------------------------------
//----------------------------------PHYSICS----------------------------------------------------
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
//----------------------------------CORE DATA CLASS--------------------------------------------
//---------------------------------------------------------------------------------------------

class SSWE_CORE_API CCoreData {

public:

	CCoreData();
	~CCoreData();

	//-----------------------------------
	//METHODS
	void clearAllTheArrays();
	void clear();

	array<stringw> getSceneNodeAnimatorsNames(IrrlichtDevice *_device, u32 idx=0);
	u32 getAmountOfSceneNodeAnimators();

	array<ISceneNode *> getAllSceneNodes();
	array<IMesh *> getAllMeshes();

	s32 isMeshPlanared(ISceneNode *node);

	array<ISceneNode *> getArrayOfTerrainNodes();
	array<ISceneNode *> getArrayOfTreeNodes();
	array<ISceneNode *> getArrayOfObjectNodes();
	array<ISceneNode *> getArrayOfLightNodes();
	array<ISceneNode *> getArrayOfVolumeLightNodes();
	array<ISceneNode *> getArrayOfWaterSurfaceNodes();
	//-----------------------------------

	//-----------------------------------
	//GET IRRLICHT NODES
	//TERRAINS
	array<STerrainsData> *getTerrainsData() { return &terrainsData; }
	array<SData> *getTerrainsSData() {
		array<SData> *datas = new array<SData>();
		for (u32 i=0; i < terrainsData.size(); i++)
			datas->push_back(terrainsData[i]);

		return datas;
	}
	u32 getTerrainsCount() { return terrainsData.size(); }

	//TREES
	array<STreesData> *getTreesData() { return &treesData; }
	array<SData> *getTreesSData() {
		array<SData> *datas = new array<SData>();
		for (u32 i=0; i < treesData.size(); i++)
			datas->push_back(treesData[i]);

		return datas;
	}

	//OBJECTS
	array<SObjectsData> *getObjectsData() { return &objectsData; }
	array<SData> *getObjectsSData() {
		array<SData> *datas = new array<SData>();
		for (u32 i=0; i < objectsData.size(); i++)
			datas->push_back(objectsData[i]);

		return datas;
	}
	u32 getObjectNodeIndice(ISceneNode *node);

	//LIGHTS
	array<SLightsData> *getLightsData() { return &lightsData; }
	array<SData> *getLightsSData() {
		array<SData> *datas = new array<SData>();
		for (u32 i=0; i < lightsData.size(); i++)
			datas->push_back(lightsData[i]);

		return datas;
	}

	array<SVolumeLightsData> *getVolumeLightsData() { return &volumeLightsData; }
	array<SData> *getVolumeLightsSData() {
		array<SData> *datas = new array<SData>();
		for (u32 i=0; i < volumeLightsData.size(); i++)
			datas->push_back(volumeLightsData[i]);

		return datas;
	}

	array<SWaterSurfacesData> *getWaterSurfaces() { return &waterSurfaces; }
	array<SData> *getWaterSurfacesSData() {
		array<SData> *datas = new array<SData>();
		for (u32 i=0; i < waterSurfaces.size(); i++)
			datas->push_back(waterSurfaces[i]);

		return datas;
	}

	array<SPlanarTextureMappingData> *getPlanarTextureMappingValues() { return &planarTextureMappingValues; }
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

	//-----------------------------------
	//SCRIPTS
	array<SScriptFile> *getScriptFiles() { return &scriptFiles; }
	//-----------------------------------

	//-----------------------------------
	//PLUGINS
	array<SMonitor> *getMonitors() { return &monitors; }
	//-----------------------------------

private:

	//-----------------------------------
	//IRRLICHT NODES
	array<STerrainsData> terrainsData;

	array<STreesData> treesData;

	array<SObjectsData> objectsData;

	array<SLightsData> lightsData;

	array<SVolumeLightsData> volumeLightsData;

	array<SWaterSurfacesData> waterSurfaces;

	array<SPlanarTextureMappingData> planarTextureMappingValues;
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

	//-----------------------------------
	//SCRIPTS
	array<SScriptFile> scriptFiles;
	//-----------------------------------

	//-----------------------------------
	//PLUGINS
	array<SMonitor> monitors;
	//-----------------------------------

};

#endif
