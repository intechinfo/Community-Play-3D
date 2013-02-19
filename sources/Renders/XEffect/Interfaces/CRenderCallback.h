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
#include "CRenderCallbackScripting.h"

class CEffectRenderCallback : public IPostProcessingRenderCallback {
public:
    
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------
    //EFFECTS PROPERTIES
	CEffectRenderCallback(irr::s32 materialTypeIn, irr::IrrlichtDevice *Device) : materialType(materialTypeIn) {
        luaScripting = new CRenderCallbackScripting(Device);
    }
    
	void OnPreRender(EffectHandler* effect);
    
	void OnPostRender(EffectHandler* effect) {
        
    }
    
    virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData) {
        /*irr::video::IVideoDriver* driver = services->getVideoDriver();
        
        for (irr::u32 cbi=0; cbi < vertexValues.size(); cbi++) {
            services->setVertexShaderConstant(vertexValuesNames[cbi].c_str(), reinterpret_cast<irr::f32*>(&vertexValues[cbi]), 1);
        }
        
        for (irr::u32 cbi=0; cbi < pixelValues.size(); cbi++) {
            services->setPixelShaderConstant(pixelValuesNames[cbi].c_str(), reinterpret_cast<irr::f32*>(&returnedValue), 1);
        }*/
	}
    
    irr::s32 materialType;
    
    //GET METHODS
    irr::core::array<float> *getVertexValues() { return &vertexValues; }
    irr::core::array<irr::core::stringc> *getVertexValuesNames() { return &vertexValuesNames; }
    
    irr::core::array<irr::core::stringc> *getPixelValues() { return &pixelValues; }
    irr::core::array<irr::core::stringc> *getPixelValuesNames() { return &pixelValuesNames; }
    
    //CLEAR METHODS
    void clearVertexValues();
    void clearPixelValues();
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------
    
private:
    
    //ARRAYS WITH SHADER CONSTANTS VALUES
    irr::core::array<float> vertexValues;
    irr::core::array<irr::core::stringc> vertexValuesNames;
    
    irr::core::array<irr::core::stringc> pixelValues;
    irr::core::array<irr::core::stringc> pixelValuesNames;
    
    //SCRIPTING LUA
    CRenderCallbackScripting *luaScripting;
    
    
};

#endif
