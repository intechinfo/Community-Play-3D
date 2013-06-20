#ifndef __C_SCENE_SCRIPTING_H_INCLUDED__
#define __C_SCENE_SCRIPTING_H_INCLUDED__

class CDevices;

#include <irrlicht.h>

class CSceneScripting {

public:

	CSceneScripting(CDevices *_devices);
	~CSceneScripting();

	void loadMesh(irr::core::stringc path, irr::scene::ESCENE_NODE_TYPE type=irr::scene::ESNT_UNKNOWN);

	//MESHES
	void addTerrainMeshSceneNode(irr::core::stringc path, ISceneNode *parent=0);
	void addTerrainOctTreeSceneNode(irr::core::stringc path, irr::u32 minPolys=1024, ISceneNode *parent=0);
	void addTreeMeshSceneNode(irr::core::stringc path, irr::u32 minPolys=1024, ISceneNode *parent=0);
	void addObjectMeshSceneNode(irr::core::stringc path, ISceneNode *parent=0);
	void addLightSceneNode(ISceneNode *parent=0);
	void addVolumeLightSceneNode(ISceneNode *parent=0);

	//FACTORY
	void addCubeSceneNode(ISceneNode *parent=0);
	void addSphereSceneNode(ISceneNode *parent=0);
	void addHillPlaneMesh(ISceneNode *parent=0);
	void addBillBoardSceneNode(ISceneNode *parent=0);

private:

	//-----------------------------------
	//DATAS
	CDevices *devices;
	irr::scene::ISceneManager *smgr;
	//-----------------------------------

};

#endif
