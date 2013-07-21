#include "stdafx.h"

#include "../../SSWECore/SSWECore/CCollisionManager.h"

#include "CTerrainPager.h"
#include <irrlicht.h>

using namespace irr;

CTerrainPager::CTerrainPager(STerrainParameters param, scene::ISceneNode* parent, 
			scene::ISceneManager* smgr, s32 id, IMetaTriangleSelector *meta) : scene::ISceneNode(parent, smgr, id)
{
	this->SceneManager = smgr;

	Meta = meta;
	Param = param;
	Param.DistanceMaxRender = param.DistanceMaxRender * Param.Scale.X; // set with scale factor

	u16 NbTiles = Param.Dimension.Width * Param.Dimension.Height;
	
	posX = 0.0f;
	posZ = 0.0f;
	x=0;

	for(u16 i=0; i<NbTiles; i++) {
		heightMapList.push_back(strdup(Param.MapListFileName[i].c_str()));
	}

	for(u16 i=0; i<NbTiles; i++) {		
		CBufferZone * zone = new CBufferZone(Param.MapSize, core::vector3df(posX, 0, posZ), Param.Scale, this, smgr, 0);
			zoneList.push_back(zone);
			computePosition(i);
	}

	setAutomaticCulling(scene::EAC_OFF);
}

CTerrainPager::~CTerrainPager() {

}

//----------------------------------------------------------------------------------------
//COMPUTE TILE POSITION
//----------------------------------------------------------------------------------------
void CTerrainPager::computePosition(u16) {
	f32 imageSize = Param.MapSize;

	x++;
	if(x >= Param.Dimension.Width) {
		posX = 0.0f;
		posZ += imageSize;
		x=0;
	}
	else {
		posX += imageSize;
	}
}

//----------------------------------------------------------------------------------------
//LOAD TERRAIN
//----------------------------------------------------------------------------------------
void CTerrainPager::loadTerrain(u16 index) {

	CTerrain * Terrain = new CTerrain(heightMapList[index], Param.Quality, Param.LodTextureScale, this, this->SceneManager, -1);
	Terrain->setPosition(zoneList[index]->getPosition());
	Terrain->setScale(zoneList[index]->getScale());
	Terrain->setRenderDistance(Param.DistanceMaxRender);

	Terrain->setColorTexture(strdup(Param.ColorMapListFileName[index].c_str()));
	Terrain->setDetailTexture(strdup(Param.detailTexturePath.c_str()));

	ITriangleSelector *selector = SceneManager->createTriangleSelector(Terrain->getMesh(), Terrain);
	Terrain->setTriangleSelector(selector);
	Meta->addTriangleSelector(selector);

	if(Param.Fog)
		Terrain->setMaterialFlag(video::EMF_FOG_ENABLE, true);

	if(Param.Debug) {
		this->zoneList[index]->setDebugDataVisible(scene::EDS_BBOX);
	}

	zoneList[index]->setLoaded(true);
    zoneList[index]->setObjectInside(Terrain);

	Terrain->drop();
}


//----------------------------------------------------------------------------------------
//CHECK IF A TERRAIN NEED TO BE LOAD OR UNLOAD
//----------------------------------------------------------------------------------------
void CTerrainPager::render() {
  	scene::ICameraSceneNode* cam = SceneManager->getActiveCamera();
	const scene::SViewFrustum* frustum = cam->getViewFrustum();
	video::IVideoDriver* Driver = SceneManager->getVideoDriver();
	core::vector3df Pos = cam->getPosition();	
	Pos.Y = getPosition().Y;
	Driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	size_t size = zoneList.size();
	for (u32 i=0; i<size; i++) {		
		f32 ActualDistance = zoneList[i]->getBoundingBox().getCenter().getDistanceFrom(Pos);
		bool load = zoneList[i]->isLoaded();

		if(ActualDistance < Param.DistanceLoad && load == false) {
			loadTerrain(i);
		}
		else if(ActualDistance > Param.DistanceUnload && load == true) { 
			zoneList[i]->remove();
			zoneList[i]->setLoaded(false);					
		}		
	}
}

void CTerrainPager::OnRegisterSceneNode() {
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}


const core::aabbox3d<f32>& CTerrainPager::getBoundingBox() const {
	return Box;
}