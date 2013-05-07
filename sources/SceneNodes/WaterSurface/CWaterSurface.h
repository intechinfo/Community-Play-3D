#ifndef CLASS_WATER_SURFACE_H_INC
#define CLASS_WATER_SURFACE_H_INC

#include <Irrlicht.h>

#define CLIP_PLANE_OFFSET 200.0f

class CWaterSurface : public irr::scene::ISceneNode
{
    private :
    
    irr::video::IVideoDriver     *driver;
    irr::scene::ISceneManager *SceneManager;
    
    irr::scene::ICameraSceneNode *refCam;
    irr::video::ITexture         *refRTT, *originRTT;
    irr::core::dimension2du      refRTTSize;
    
    irr::scene::IMeshSceneNode   *waterNode;

	bool drawScene;
    
    public :
    
    CWaterSurface(irr::scene::ISceneManager* smgr, irr::core::vector3df position = irr::core::vector3df(0,0,0), irr::f32 surfaceTileSize = 25.0f, 
                  irr::u32 surfaceTileCount = 50, bool SinWave = true, bool Refraction = true, irr::core::dimension2du RTTSize = irr::core::dimension2du(512, 512), 
                  irr::scene::ISceneNode* parent = 0, irr::s32 id = -1, irr::video::E_MATERIAL_TYPE mat = irr::video::EMT_SOLID);
    ~CWaterSurface();
    
    void OnRegisterSceneNode();
    void OnAnimate(irr::u32 timeMs);
    void render();
    const irr::core::aabbox3df& getBoundingBox() const;
    
    irr::scene::IMeshSceneNode* getWaterNode() { return waterNode; }
    const irr::core::dimension2du     getRTTSize()         { return refRTTSize; }
    const bool                        isSinWaveEnabled();
    const bool                        isRefractionEnabled();

    void setSinWaveEnabled    (bool Enabled);
    void setRefractionEnabled (bool Enabled);

	void setOriginRTT(irr::video::ITexture *tex);

	void setDrawScene(bool _drawScene) { drawScene = _drawScene; }
	bool isDrawingScene() { return drawScene; }
};

#endif