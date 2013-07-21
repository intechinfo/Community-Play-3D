#ifndef _C_TERRAIN_PAGER_
#define _C_TERRAIN_PAGER_

#include <irrlicht.h>
#include "CTerrain.h"
#include <vector>	// TODO : USE IRR COLLECTION

using namespace irr;

class CCollisionManager;

//--------------------------------------------------------------------------------------
//TERRAIN PAGER PARAMETERS
//--------------------------------------------------------------------------------------
struct STerrainParameters
{
	core::array<core::stringc> MapListFileName;
	core::array<core::stringc> ColorMapListFileName;
	core::stringc detailTexturePath;
	CTerrain::E_TERRAIN_QUALITY Quality;
	f32 MapSize;
	core::vector3df Position;
	core::vector3df Scale;
	f32 LodTextureScale;
	core::dimension2di Dimension;
	f32 DistanceLoad;
	f32 DistanceUnload;
	f32 DistanceMaxRender;
	bool Fog;
	bool Debug;
};


//--------------------------------------------------------------------------------------
//BUFFER CLASS TO STORE TERRAIN INFORMATIONS
//--------------------------------------------------------------------------------------
class CBufferZone : public scene::ISceneNode
{
public:

	//CREATE BUFFER ZONE
	CBufferZone(f32 size, core::vector3df pos, core::vector3df scale,  scene::ISceneNode* parent,scene::ISceneManager* smgr,s32 id) : scene::ISceneNode(parent, smgr, id)
	{
		SceneManager = smgr;		
		setPosition(pos);
		setScale(scale);	
		core::vector3df min = pos * scale;
		core::vector3df max = (pos + core::vector3df(size,size*0.2f,size)) * scale;	
		bbox = core::aabbox3df(min, max);
		setVisible(true);
		setLoaded(false);
		setAutomaticCulling(scene::EAC_OFF);
	}	
	
	//RENDER BOUNDING BOX OF BUFFER ZONE
	virtual void render() 
	{
 		if (DebugDataVisible & scene::EDS_BBOX)
		{							
			video::SMaterial m;
			m.Lighting = false;			
			SceneManager->getVideoDriver()->setMaterial(m);
			video::SColor c (255,255,0,0);			
			SceneManager->getVideoDriver()->draw3DBox(this->getBoundingBox(), c);
		}		
	}

	void OnRegisterSceneNode()
	{
		if (IsVisible && loaded)
			SceneManager->registerNodeForRendering(this);
		ISceneNode::OnRegisterSceneNode();
	}

	virtual const core::aabbox3d<f32>& getBoundingBox() const { return bbox; }
	bool isLoaded() { return loaded; }
	void setLoaded(bool state) { loaded = state; }
	CTerrain * getObjectInside() { return object; }
	void setObjectInside(CTerrain * terrain) { object = terrain; }

private:
	core::aabbox3df bbox;
	bool loaded;	
	CTerrain * object;
};


//--------------------------------------------------------------------------------------
//TERRAIN PAGER CLASS
//--------------------------------------------------------------------------------------
class CTerrainPager : public scene::ISceneNode
{
    public:
		CTerrainPager(STerrainParameters param, scene::ISceneNode* parent, scene::ISceneManager* smgr, s32 id, IMetaTriangleSelector *meta);
        ~CTerrainPager();
		virtual void OnRegisterSceneNode();
		virtual void render();
		virtual const core::aabbox3d<f32>& getBoundingBox() const;
		void loadTerrain(u16 index);
		void computePosition(u16);

	private:
		core::aabbox3d<f32> Box;
		std::vector<CBufferZone*> zoneList;
		std::vector<c8*> heightMapList;		
		f32 posX, posZ;
		u16 x;
		STerrainParameters Param;

		IMetaTriangleSelector *Meta;
};

#endif