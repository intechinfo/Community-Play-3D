#ifndef _CTerrain
#define _CTerrain

#include <irrlicht.h>

using namespace irr;


//----------------------------------------------------------------------------------------
//TILED TERRAIN CLASS
//----------------------------------------------------------------------------------------
class CTerrain : public scene::ISceneNode {
public:

	enum E_TERRAIN_QUALITY {
		ETQ_HIGH = 1,
		ETQ_MEDIUM = 2,
		ETQ_LOW = 4,
		ETQ_EXTRALOW = 8
	};

	CTerrain(c8* HeightmapFile,E_TERRAIN_QUALITY Quality,f32 ScaleTexture,scene::ISceneNode* parent,scene::ISceneManager* smgr,s32 id);
	virtual ~CTerrain();
	void setColorTexture(c8* FileName);
	void setDetailTexture(c8* FileName);
	void setRenderDistance(f32 Distance);
	scene::IMesh* getMesh();
	f32 getHeight(f32 x,f32 z);
	virtual void setMaterialType(const video::E_MATERIAL_TYPE Mat);
	virtual void OnRegisterSceneNode();
	virtual void render();
	virtual video::SMaterial& getMaterial(u32 i);
	virtual void setPosition(const core::vector3df &Pos);
	virtual void setScale(const core::vector3df &Scale);
	u32 getMaterialCount();
	virtual const core::aabbox3d<f32>& getBoundingBox() const;
	virtual void recalculateBoundingBox();
	virtual void setMaterialFlag(const video::E_MATERIAL_FLAG Mat, bool Flag);
	void setTexture(u32 NumTex,c8* FileName);
	void setShader(c8 * shaderFileName, scene::ISceneManager* smgr);

private:

	core::aabbox3d<f32> BoundingBox;
	video::SMaterial Material;
	scene::SMeshBufferLightMap** CTTileBuffer;
	void calculateNormals ( scene::SMeshBufferLightMap* pMeshBuffer,s32 Size);

	f32 RenderDistance;
	scene::SMesh* TerrainMesh;
	u16 NbsTiles;
};



//----------------------------------------------------------------------------------------
//SHADER CALLBACK
//----------------------------------------------------------------------------------------
class MyShaderCallBack : public video::IShaderConstantSetCallBack
{
public:
    scene::ISceneManager* smgr;	

    virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
    {
         video::IVideoDriver* driver = services->getVideoDriver();	

		// World * View * Projection transformation
		core::matrix4 worldViewProj;
        worldViewProj  = driver->getTransform(video::ETS_PROJECTION);
        worldViewProj *= driver->getTransform(video::ETS_VIEW);
        worldViewProj *= driver->getTransform(video::ETS_WORLD);        
        services->setVertexShaderConstant("mWorldViewProj",worldViewProj.pointer(),16);		

    }
};

#endif