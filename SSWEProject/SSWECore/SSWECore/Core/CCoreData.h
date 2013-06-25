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

#include "../../../SSWELib/SSWELib/SceneNodes/WaterSurface/CWaterSurface.h"

#include "../../../SSWELib/SSWELib/CharacterEdition/CAction.h"

//---------------------------------------------------------------------------------------------
//-----------------------------------PLANAR MAPPING--------------------------------------------
//---------------------------------------------------------------------------------------------
struct SSWE_CORE_API SPlanarTextureMappingData {

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

struct SSWE_CORE_API SMeshWithTangentsData {

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

struct SSWE_CORE_API STerrainsData {
	
	STerrainsData() {
		STerrainsData(0, 0, "", 0, ESNT_UNKNOWN);
	}

	STerrainsData(IMesh *_mesh, ISceneNode *_node, stringw _path, u32 _minPolysPerNode=0, ESCENE_NODE_TYPE _type=ESNT_MESH) {
		mesh = _mesh;
		node = _node;
		path = _path;
		minPolysPerNode = _minPolysPerNode;
		type = _type;
	}

	void setMesh(IMesh *_mesh) { mesh = _mesh; }
	void setNode(ISceneNode *_node) { node = _node; }
	void setPath(stringc _path) { path = _path; }
	void setMinPolysPerNode(u32 _minPolysPerNode) { minPolysPerNode = _minPolysPerNode; }
	void setType(ESCENE_NODE_TYPE _type) { type = _type; }

	IMesh *getMesh() { return mesh; }
	ISceneNode *getNode() { return node; }
	stringc getPath() { return path; }
	u32 getMinPolysPerNode() { return minPolysPerNode; }
	ESCENE_NODE_TYPE getType() { return type; }

private:
	IMesh *mesh;
	ISceneNode *node;
	u32 minPolysPerNode;
	stringc path;
	ESCENE_NODE_TYPE type;
};

//---------------------------------------------------------------------------------------------
//----------------------------------TREES---------------------------------------------------
//---------------------------------------------------------------------------------------------

struct SSWE_CORE_API STreesData {
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

struct SSWE_CORE_API SObjectsData {

	SObjectsData(IMesh *_mesh, ISceneNode *_node, stringw _path) {
		mesh = _mesh;
		node = _node;
		path = _path;

		actions.clear();
	}

	void setMesh(IMesh *_mesh) { mesh = _mesh; }
	void setNode(ISceneNode *_node) { node = _node; }
	void setPath(stringw _path) { path = _path; }
	void setActions(array<CAction *> *_actions) {
		actions.clear();
		for (u32 i=0; i < _actions->size(); i++) {
			actions.push_back(_actions->operator[](i));
		}
	}

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
//----------------------------------LIGHTS-----------------------------------------------------
//---------------------------------------------------------------------------------------------

struct SSWE_CORE_API SLightsData {

	SLightsData(ISceneNode *_node, IMeshSceneNode *_lensFlareMeshSceneNode=0, IBillboardSceneNode *_lensFlareBillBoardSceneNode=0,
				CLensFlareSceneNode *_lensFlareSceneNode=0) {

		node = _node;

		lfdata.setLensFlareMeshSceneNode(_lensFlareMeshSceneNode);
		lfdata.setLensFlareBillboardSceneNode(_lensFlareBillBoardSceneNode);
		lfdata.setLensFlareSceneNode(_lensFlareSceneNode);
	}

	// LENS FLARE
	ISceneNode *getNode() { return node; }
	IMeshSceneNode *getLensFlareMeshSceneNode() { return lfdata.getLensFlareMeshSceneNode(); }
	IBillboardSceneNode *getLensFlareBillBoardSceneNode() { return lfdata.getLensFlareBillBoardSceneNode(); }
	CLensFlareSceneNode *getLensFlareSceneNode() { return lfdata.getLensFlareSceneNode(); }
	f32 getLensFlareStrengthFactor() { return lfdata.getStrengthFactor(); }

	void setLight(ISceneNode *_node) { node = _node; }
	void setLensFlareMeshSceneNode(IMeshSceneNode *_node) { lfdata.setLensFlareMeshSceneNode(_node); }
	void setLensFlareBillboardSceneNode(IBillboardSceneNode *_node) { lfdata.setLensFlareBillboardSceneNode(_node); }
	void setLensFlareSceneNode(CLensFlareSceneNode *_node) { lfdata.setLensFlareSceneNode(_node); }
	void setLensFlareStrengthFactor(f32 strengthFactor) { lfdata.setStrengthFactor(strengthFactor); }

	// LIGHT SHAFTS
	void createLightShafts(EffectHandler *effect, const u32 numberOfPlanes);
	void createLightShaftsCallback(EffectHandler *effect) { lsdata.createCallback(effect); }
	void createLightShaftsTextures(EffectHandler *effect, u32 resolution) { lsdata.createTextures(effect, resolution); }

	void destroyLightShafts() { lsdata.destroyLightShafts(); }
	void destroyLightShaftsCallback() { delete lsdata.getCallback(); }

	array<ISceneNode *> *getLightShaftsBS() { return lsdata.getBillBoardSet(); }
	LightShaftsCB *getLightShaftsCallback() { return lsdata.getCallback(); }
	bool isLightShaftsEnable() { return lsdata.isLightShaftsEnabled(); }

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

	struct SLightShafts {
	public:
		SLightShafts() {
			billBoardSet.clear();
			callback = 0;
			enable = false;
		}

		array<ISceneNode *> *getBillBoardSet() { return &billBoardSet; }
		LightShaftsCB *getCallback() { return callback; }
		bool isLightShaftsEnabled() { return enable; }

		void createCallback(EffectHandler *effect) {
			callback = new LightShaftsCB(effect);
		}
		void create(ICameraSceneNode *lightCamera, const u32 NumberOfPlanes, stringc LIGHT_SHAFTS_V, stringc LIGHT_SHAFTS_P) {

			LightShafts = lightCamera->getSceneManager()->getVideoDriver()->getGPUProgrammingServices()->
				addHighLevelShaderMaterial(LIGHT_SHAFTS_V.c_str(), "vertexMain", EVST_VS_2_0,
										   LIGHT_SHAFTS_P.c_str(), "pixelMain", EPST_PS_2_0, 
										   callback);

			float DistanceBetweenPlanes = (lightCamera->getFarValue() - lightCamera->getNearValue())/NumberOfPlanes;
			vector3df *FrustumCorners = getWorldSpaceCorners(lightCamera);

			float NearWidth  = (FrustumCorners[0] - FrustumCorners[1]).getLength(),
				  NearHeigth = (FrustumCorners[1] - FrustumCorners[2]).getLength(),
				  FarWidth   = (FrustumCorners[4] - FrustumCorners[5]).getLength(),
				  FarHeigth  = (FrustumCorners[5] - FrustumCorners[6]).getLength();

			float WidthStep  = (FarWidth-NearWidth)/NumberOfPlanes,
				  HeigthStep = (FarHeigth-NearHeigth)/NumberOfPlanes;

			for(s32 i = 0; i < NumberOfPlanes; i++) {
				vector3df position = vector3df(0, 0,-lightCamera->getNearValue() -i*DistanceBetweenPlanes);
				//IBillboardSceneNode *bb = lightCamera->getSceneManager()->addBillboardSceneNode();
				ISceneNode *bb = lightCamera->getSceneManager()->addCubeSceneNode();
				bb->setParent(callback->lightCamera);
				bb->setPosition(position);
				bb->setScale(vector3df(NearWidth + i*WidthStep, NearHeigth + i*HeigthStep, 0.5));
				//bb->setSize(dimension2d<f32>(NearWidth + i*WidthStep, NearHeigth + i*HeigthStep));
				//bb->setColor(SColor(255, 255, 255, 255));
				bb->setMaterialFlag(EMF_LIGHTING, false);
				if (callback) {
					//bb->setMaterialType((E_MATERIAL_TYPE)LightShafts);
					callback->uLightFarClipDistance = DistanceBetweenPlanes;
					callback->lightCamera->setPosition(lightCamera->getPosition());
				}
				billBoardSet.push_back(bb);
			}
			enable = true;
		}
		void createTextures(EffectHandler *effect, u32 id) {
			IVideoDriver *driver = effect->getIrrlichtDevice()->getVideoDriver();
			for (u32 i=0; i < billBoardSet.size(); i++) {
				billBoardSet[i]->setMaterialTexture(0, effect->getShadowMapTexture(effect->getShadowLight(id).getShadowMapResolution(), 
																				   false, 
																				   id));
				billBoardSet[i]->setMaterialTexture(1, driver->getTexture("shaders/Textures/LS/Cookie.png"));
				billBoardSet[i]->setMaterialTexture(2, driver->getTexture("shaders/Textures/LS/Noise.png"));
				effect->addShadowToNode(billBoardSet[i], EFT_NONE, ESM_EXCLUDE);
			}
		}

		void destroyCallback() { delete callback; }
		void destroyLightShafts() {
			for (u32 i=0; i < billBoardSet.size(); i++) {
				billBoardSet[i]->remove();
			}
			billBoardSet.clear();
			enable = false;
		}

	private:
		array<ISceneNode *> billBoardSet;
		LightShaftsCB *callback;
		irr::s32 LightShafts;
		bool enable;

		vector3df *getWorldSpaceCorners(ICameraSceneNode *cam) {
			const SViewFrustum *f = cam->getViewFrustum();
			vector3df *vec[8] = {
				&irr::core::vector3df(f->getNearRightUp()),
				&irr::core::vector3df(f->getNearLeftUp()),
				&irr::core::vector3df(f->getNearLeftDown()),
				&irr::core::vector3df(f->getNearRightDown()),
				&irr::core::vector3df(f->getFarRightUp()),
				&irr::core::vector3df(f->getFarLeftUp()),
				&irr::core::vector3df(f->getFarLeftDown()),
				&irr::core::vector3df(f->getFarRightDown())
			};

			return *vec;
		}
	} lsdata;
};

//---------------------------------------------------------------------------------------------
//----------------------------------VOLUME LIGHTS---------------------------------------------
//---------------------------------------------------------------------------------------------

struct SSWE_CORE_API SVolumeLightsData {

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

struct SSWE_CORE_API SWaterSurfacesData {
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
//----------------------------------SCRIPTS----------------------------------------------------
//---------------------------------------------------------------------------------------------

struct SScriptFile {
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
	array<STerrainsData> *getTerrainsData() { return &terrainsData; }

	array<STreesData> *getTreesData() { return &treesData; }

	array<SObjectsData> *getObjectsData() { return &objectsData; }
	u32 getObjectNodeIndice(ISceneNode *node);

	array<SLightsData> *getLightsData() { return &lightsData; }

	array<SVolumeLightsData> *getVolumeLightsData() { return &volumeLightsData; }

	array<SWaterSurfacesData> *getWaterSurfaces() { return &waterSurfaces; }

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

};

#endif
