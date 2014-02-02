#ifndef __I_SSWE_CORE_DATA_H_INCLUDED__
#define __I_SSWE_CORE_DATA_H_INCLUDED__

#include <irrlicht.h>

#include <ISSWERender.h>

#include <ISSWEShaderCallback.h>
class CShaderCallback;

struct SData;
struct SFilter;
struct STerrainsData;
struct SObjectsData;
struct SLightsData;
struct STreesData;
struct SVolumeLightsData;
struct SWaterSurfacesData;
struct SPlanarTextureMappingData;
struct SScriptFile;
struct SMonitor;

class ISSWECoreData {
    
public: 
    
    //-----------------------------------
	//METHODS
	virtual void clearAllTheArrays() = 0;
	virtual void clear() = 0;
    
    virtual irr::core::array<stringw> getSceneNodeAnimatorsNames(irr::IrrlichtDevice *_device, irr::u32 idx=0) = 0;
    virtual irr::u32 getAmountOfSceneNodeAnimators() = 0;
    
	virtual irr::core::array<irr::scene::ISceneNode *> getAllSceneNodes() = 0;
	virtual irr::core::array<irr::scene::IMesh *> getAllMeshes() = 0;
    
	virtual irr::s32 isMeshPlanared(irr::scene::ISceneNode *node) = 0;
    
	virtual irr::core::array<irr::scene::ISceneNode *> getArrayOfTerrainNodes() = 0;
	virtual irr::core::array<irr::scene::ISceneNode *> getArrayOfTreeNodes() = 0;
	virtual irr::core::array<irr::scene::ISceneNode *> getArrayOfObjectNodes() = 0;
	virtual irr::core::array<irr::scene::ISceneNode *> getArrayOfLightNodes() = 0;
	virtual irr::core::array<irr::scene::ISceneNode *> getArrayOfVolumeLightNodes() = 0;
	virtual irr::core::array<irr::scene::ISceneNode *> getArrayOfWaterSurfaceNodes() = 0;
    
    virtual void removeSceneNode(irr::scene::ISceneNode *node, ISSWERender *effect) = 0;
    virtual SData *copySDataOfSceneNode(irr::scene::ISceneNode *node) = 0;
	//-----------------------------------
    
	//-----------------------------------
	//GET IRRLICHT NODES
	//TERRAINS
	virtual irr::core::array<STerrainsData> *getTerrainsData() = 0;
	virtual irr::core::array<SData> *getTerrainsSData() = 0;
	virtual irr::u32 getTerrainsCount() = 0;
    
	//TREES
	virtual irr::core::array<STreesData> *getTreesData() = 0;
	virtual irr::core::array<SData> *getTreesSData() = 0;
    
	//OBJECTS
	virtual irr::core::array<SObjectsData> *getObjectsData() = 0;
	virtual irr::core::array<SData> *getObjectsSData() = 0;
	virtual irr::u32 getObjectNodeIndice(irr::scene::ISceneNode *node) = 0;
    
	//LIGHTS
	virtual irr::core::array<SLightsData> *getLightsData() = 0;
	virtual irr::core::array<SData> *getLightsSData() = 0;
    
	virtual irr::core::array<SVolumeLightsData> *getVolumeLightsData() = 0;
	virtual irr::core::array<SData> *getVolumeLightsSData() = 0;
    
	virtual irr::core::array<SWaterSurfacesData> *getWaterSurfaces() = 0;
	virtual irr::core::array<SData> *getWaterSurfacesSData() = 0;
    
	virtual irr::core::array<SPlanarTextureMappingData> *getPlanarTextureMappingValues() = 0;
	//-----------------------------------
    
	//-----------------------------------
	//GET EFFECT SHADERS
	virtual irr::core::array<SFilter> *getEffectFilters() = 0;
	virtual irr::core::array<CShaderCallback *> *getShaderCallbacks() = 0;
	//-----------------------------------
    
	//-----------------------------------
	//SCRIPTS
	virtual irr::core::array<SScriptFile> *getScriptFiles() = 0;
	//-----------------------------------
    
};


#endif