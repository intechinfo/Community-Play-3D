#ifndef __I_SSWE_CORE_DATA_H_INCLUDED__
#define __I_SSWE_CORE_DATA_H_INCLUDED__

#include <irrlicht.h>

#include <ISSWECore.h>
#include <ISSWERender.h>

#include <ISSWEShaderCallback.h>
class CShaderCallback;

struct ISData {

public:

	ISData() { }

	enum E_ISDATA_PHYSIC_TYPE {
		EIPT_NONE = 0,
		EIPT_RIGID_BODY,
		EIPT_LIQUID_BODY,
		EIPT_SOFT_BODY
	};

	//MAIN METHODS
	virtual irr::scene::ISceneNode *getNode() = 0;
	virtual irr::scene::IMesh *getMesh() = 0;
	virtual irr::core::stringc getPath() = 0;
	virtual irr::scene::ESCENE_NODE_TYPE getType() = 0;

	virtual void setNode(irr::scene::ISceneNode *_node) = 0;
	virtual void setMesh(irr::scene::IMesh *_mesh) = 0;
	virtual void setPath(irr::core::stringw _path) = 0;
	virtual void setType(irr::scene::ESCENE_NODE_TYPE _type) = 0;

	//CLONED NODES METHODS
	virtual irr::scene::ISceneNode *cloneNode(irr::core::vector3df position, ISSWECore *core=0) = 0;

	virtual void addClonedNode(irr::scene::ISceneNode *cnode) = 0;
    virtual void removeClonedNodes() = 0;
	virtual void removeClonedNode(irr::scene::ISceneNode *cnode) = 0;

	virtual irr::scene::ISceneNode *getClonedNode(irr::scene::ISceneNode *cnode) = 0;
	virtual irr::scene::ISceneNode *getClonedNode(irr::u32 index) = 0;

	virtual irr::u32 getClonedNodeCount() = 0;

	//PHYSICS
	virtual bool isPhysicEnabled() = 0;
	virtual void setEnablePhysics(bool enable) = 0;

	virtual E_ISDATA_PHYSIC_TYPE getBodyType() = 0;
	virtual void setBodyType(E_ISDATA_PHYSIC_TYPE type) = 0;

};

class ISSWELibPlugin;

class ISSWECoreData {
    
public: 
    
    //-----------------------------------
	//METHODS
	virtual void clearAllTheArrays() = 0;
	virtual void clear() = 0;
    
    virtual irr::core::array<irr::core::stringw> getSceneNodeAnimatorsNames(irr::IrrlichtDevice *_device, irr::u32 idx=0) = 0;
    virtual irr::u32 getAmountOfSceneNodeAnimators() = 0;
    
	virtual irr::core::array<irr::scene::ISceneNode *> getAllSceneNodes() = 0;
	virtual irr::core::array<irr::scene::IMesh *> getAllMeshes() = 0;
    
    virtual void removeSceneNode(irr::scene::ISceneNode *node, ISSWERender *effect) = 0;
    virtual ISData *copySDataOfSceneNode(irr::scene::ISceneNode *node) = 0;
	virtual ISData *getISDataOfSceneNode(irr::scene::ISceneNode *node) = 0;
	//-----------------------------------
    
	//-----------------------------------
	//GET IRRLICHT NODES
	//TERRAINS
	virtual irr::u32 getTerrainsCount() = 0;
	virtual void addTerrainNode(irr::scene::ISceneNode *node, 
								irr::scene::IMesh *mesh,
								irr::core::stringw path,
								irr::scene::E_TERRAIN_PATCH_SIZE patchSize) = 0;
	virtual void addTerrainNode(irr::scene::ISceneNode *node, 
								irr::scene::IMesh *mesh,
								irr::core::stringw path,
								irr::u32 minPolysPerNode = 0) = 0;
    
	//TREES
	/// Trees here
    
	//OBJECTS
	virtual irr::u32 getObjectNodeIndice(irr::scene::ISceneNode *node) = 0;
	virtual void addObjectNode(irr::scene::ISceneNode *node, irr::scene::IMesh *mesh, irr::core::stringw path) = 0;
    
	//LIGHTS
	/// Lights here
    
	//VOLUME LIGHTS
	/// Volume lights here
    
	//WATER SURFACES
	/// Water surfaces here

	//-----------------------------------
    
	//-----------------------------------
	//GET EFFECT SHADERS
	/// Effect shaders here (materials + filters)
	//-----------------------------------
    
	//-----------------------------------
	//SCRIPTS
	/// Scripts here
	//-----------------------------------

	//-----------------------------------
	//PLUGINS
	virtual void destroySSWEPlugin(ISSWELibPlugin *plugin) = 0;
	//-----------------------------------
    
};


#endif