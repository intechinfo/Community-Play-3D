#ifndef __C_TERRAIN_SYSTEM_H_INCLUDED__
#define  __C_TERRAIN_SYSTEM_H_INCLUDED__

#include "stdafx.h"
#include <irrlicht.h>

class CTerrainSystem : public irr::scene::ISceneNode {
public:
	//-----------------------------------
	//CTERRAINSYSTEM METHODS
	CTerrainSystem(irr::scene::ISceneManager *smgr, irr::core::stringc hgtmap, irr::u32 divFactor);
    virtual ~CTerrainSystem();
	//-----------------------------------

	//-----------------------------------
	//ISCENENODE METHODS
	virtual void OnRegisterSceneNode();
	virtual void render();

	virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const { return box; }
	//-----------------------------------

private:
	//-----------------------------------
	//NODE DATAS
	irr::core::aabbox3d<irr::f32> box;

	irr::scene::ISceneManager *smgr;
	irr::video::IVideoDriver *driver;
	//-----------------------------------

	//-----------------------------------
	//DATAS
	irr::core::list<irr::scene::ITerrainSceneNode *> tlist;
	//-----------------------------------

};

#endif