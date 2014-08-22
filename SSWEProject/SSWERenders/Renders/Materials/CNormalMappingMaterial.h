//
//  CNormalMappingMaterial.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 21/05/2014.
//
//

#ifndef __C_NORMAL_MAPPING_RENDERER_H_INCLUDED__
#define __C_NORMAL_MAPPING_RENDERER_H_INCLUDED__

#include <SSWERenders.h>
#include <irrlicht.h>

class SSWE_RENDERS_API CNormalMappingMaterial : public irr::video::IShaderConstantSetCallBack {

public:

    //-----------------------------------
    //CTOR & DTOR
    CNormalMappingMaterial();
    ~CNormalMappingMaterial();
    //-----------------------------------

    //-----------------------------------
    //LIGHTS METHODS
    irr::scene::ILightSceneNode *getLightSceneNode(irr::u32 indice) { return lights[indice]; }
    irr::u32 getLightCount() { return lights.size(); }

    void removeLight(irr::u32 indice);

    void addLight(irr::scene::ILightSceneNode *node);
    
    void reset();
    //-----------------------------------

    //-----------------------------------
    //MATERIAL METHODS
    void build(irr::video::IVideoDriver *driver);

    irr::s32 getMaterialSolid() { return nmSolid; }
    //-----------------------------------

    //-----------------------------------
    //INHERITANCE METHODS
	virtual void OnSetConstants(irr::video::IMaterialRendererServices *services, irr::s32 userData);
    //-----------------------------------

private:

    //-----------------------------------
    //METHODS
    void rebuildCallbackParameters();
    void rebuildPositions();
    //-----------------------------------

    //-----------------------------------
    //DATAS
    irr::core::array<irr::scene::ILightSceneNode*> lights;

    irr::s32 nmSolid, nmTransAdd, nmTransAlphaRef;
    //-----------------------------------

    //-----------------------------------
    //CALLBACK PARAMETERS
    irr::core::array<irr::f32> fLightStrengthArray;
    irr::core::array<irr::f32> fvLightPositionArray;
    irr::core::array<irr::f32> fvLightColorArray;
    //-----------------------------------

};

#endif /* defined(__C_NORMAL_MAPPING_RENDERER_H_INCLUDED__) */
