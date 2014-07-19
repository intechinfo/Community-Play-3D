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

	/// Main methods
	/// Returns the node stored into the data
	virtual irr::scene::ISceneNode *getNode() = 0;
	/// Returns the node's mesh stored into the data. If no mesh then 0 is returned
	virtual irr::scene::IMesh *getMesh() = 0;
	/// Returns the physical path of the mesh. Empty string is returned if no mesh
	virtual irr::core::stringc getPath() = 0;
	/// Returns the node's type stored into the data
	virtual irr::scene::ESCENE_NODE_TYPE getType() = 0;

	/// Sets the new scene node of the data. Warning, it doesn't remove the last scene node
	virtual void setNode(irr::scene::ISceneNode *_node) = 0;
	/// Sets the new mesh of the data. Warning, it doesn't remove the last mesh
	virtual void setMesh(irr::scene::IMesh *_mesh) = 0;
	/// Sets the new mesh's path
	virtual void setPath(irr::core::stringw _path) = 0;
	/// Sets the new node type
	virtual void setType(irr::scene::ESCENE_NODE_TYPE _type) = 0;

	/// Clones methods
	/// Clones the node stored into the data. Returnrs the cloned node.
	virtual irr::scene::ISceneNode *cloneNode(irr::core::vector3df position, ISSWECore *core=0) = 0;

	/// I don't what it does
	virtual void addClonedNode(irr::scene::ISceneNode *cnode) = 0;
	/// Removes all the cloned nodes. Keeps the original scene node
    virtual void removeClonedNodes() = 0;
	/// Removes the given node from the cloned nodes of the data
	virtual void removeClonedNode(irr::scene::ISceneNode *cnode) = 0;

	/// I don't know why this method is here...
	virtual irr::scene::ISceneNode *getClonedNode(irr::scene::ISceneNode *cnode) = 0;
	/// Returns the cloned node at index "index"
	virtual irr::scene::ISceneNode *getClonedNode(irr::u32 index) = 0;

	/// Returns de cloned node count
	virtual irr::u32 getClonedNodeCount() = 0;

protected:
	/// Physics
	bool enablePhysics;
	E_ISDATA_PHYSIC_TYPE bodyType;
	void *pBodyPtr;

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