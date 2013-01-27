//
//  CRenderCallback.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 28/11/12.
//
//

#ifndef __C_RENDER_CALLBACK_H_INCLUDED__
#define __C_RENDER_CALLBACK_H_INCLUDED__

#include "../XEffects.h"

class CEffectRenderCallback : public IPostProcessingRenderCallback {
public:
    
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------
    //EFFECTS PROPERTIES
	CEffectRenderCallback(irr::s32 materialTypeIn) : materialType(materialTypeIn) {}
    
	void OnPreRender(EffectHandler* effect) {
        //irr::video::IVideoDriver* driver = effect->getIrrlichtDevice()->getVideoDriver();
		//viewProj = driver->getTransform(irr::video::ETS_PROJECTION) * driver->getTransform(irr::video::ETS_VIEW);
		//effect->setPostProcessingEffectConstant(materialType, "mViewProj", viewProj.pointer(), 16);
        for (irr::u32 i=0; i < pixelValues.size(); i++) {
            effect->setPostProcessingEffectConstant(materialType, pixelValuesNames[i], reinterpret_cast<irr::f32*>(&pixelValues[i]), 1);
        }
	}
    
	void OnPostRender(EffectHandler* effect) {}
    
    /*virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData) {
        irr::video::IVideoDriver* driver = services->getVideoDriver();
        
        for (irr::u32 cbi=0; cbi < vertexValues.size(); cbi++) {
            services->setVertexShaderConstant(vertexValuesNames[cbi].c_str(), reinterpret_cast<irr::f32*>(&vertexValues[cbi]), 1);
        }
        
        for (irr::u32 cbi=0; cbi < pixelValues.size(); cbi++) {
            services->setPixelShaderConstant(pixelValuesNames[cbi].c_str(), reinterpret_cast<irr::f32*>(&pixelValues[cbi]), 1);
        }
	}*/
    
    irr::core::matrix4 viewProj;
    irr::s32 materialType;
    
    //GET METHODS
    irr::core::array<float> *getVertexValues() { return &vertexValues; }
    irr::core::array<irr::core::stringc> *getVertexValuesNames() { return &vertexValuesNames; }
    
    irr::core::array<float> *getPixelValues() { return &pixelValues; }
    irr::core::array<irr::core::stringc> *getPixelValuesNames() { return &pixelValuesNames; }
    
    //CLEAR METHODS
    void clearVertexValues();
    void clearPixelValues();
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------
    
private:
    
    //ARRAYS WITH SHADER CONSTANTS VALUES
    irr::core::array<float> vertexValues;
    irr::core::array<irr::core::stringc> vertexValuesNames;
    
    irr::core::array<float> pixelValues;
    irr::core::array<irr::core::stringc> pixelValuesNames;
    
    
};

#endif
