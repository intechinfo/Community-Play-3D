#ifndef __C_SCENE_SCRIPTING_H_INCLUDED__
#define __C_SCENE_SCRIPTING_H_INCLUDED__

class CDevices;

#include <irrlicht.h>

using namespace irr;
using namespace scene;
using namespace io;
using namespace core;

class CSceneScripting {

public:

	CSceneScripting(CDevices *_devices);
	~CSceneScripting();

	void loadMesh(irr::core::stringc path, irr::scene::ESCENE_NODE_TYPE type=irr::scene::ESNT_UNKNOWN);

	//MESHES
	s32 addTerrainMeshSceneNode(irr::core::stringc path, ISceneNode *parent=0);
	s32 addTerrainOctTreeSceneNode(irr::core::stringc path, irr::u32 minPolys=1024, ISceneNode *parent=0);
	s32 addTreeMeshSceneNode(irr::core::stringc path, irr::u32 minPolys=1024, ISceneNode *parent=0);
	s32 addObjectMeshSceneNode(irr::core::stringc path, ISceneNode *parent=0);
	s32 addLightSceneNode(ISceneNode *parent=0);
	s32 addVolumeLightSceneNode(ISceneNode *parent=0);

	u32 getTerrainCount();
	u32 getTreeCount();
	u32 getObjectCount();
	u32 getLightCount();
	u32 getVolumeLightCount();

	//FACTORY
	s32 addCubeSceneNode(ISceneNode *parent=0);
	s32 addSphereSceneNode(ISceneNode *parent=0);
	s32 addHillPlaneMesh(ISceneNode *parent=0);
	s32 addBillBoardSceneNode(ISceneNode *parent=0);

	//METHODS
	u32 getTerrainNodeMaterialCount(u32 ti);
	s32 getTerrainByName(stringc name);
	stringc getTerrainMaterialName(s32 ti, s32 mi);
	void setTerrainMaterialNumber(s32 ti, s32 mi, s32 mati);
	s32 getMaterialIDByName(stringc name);

private:

	//-----------------------------------
	//DATAS
	CDevices *devices;
	irr::scene::ISceneManager *smgr;
	//-----------------------------------

};

#endif
