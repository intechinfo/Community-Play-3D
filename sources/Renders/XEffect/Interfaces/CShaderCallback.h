//
//  CShaderCallback.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 26/02/13.
//
//

#ifndef __C_SHADER_CALLBACK_H_INCLUDED__
#define __C_SHADER_CALLBACK_H_INCLUDED__

#include <irrlicht.h>

#include <iostream>
#include <sstream>
#include <vector>

enum ESHADER_TYPE {
    EST_VERTEX = 0,
    EST_PIXEL
};

class CShaderCallback : public irr::video::IShaderConstantSetCallBack {
  
public:
    
    CShaderCallback();
    ~CShaderCallback();
    
    //CONSTANTS
    irr::core::array<irr::core::vector3df> vectors3D;
    irr::core::array<irr::core::stringc> vectors3D_c;
    irr::core::array<ESHADER_TYPE> vectors3D_st;
    
    irr::core::array<float> floats;
    irr::core::array<irr::core::stringc> floats_c;
    irr::core::array<ESHADER_TYPE> floats_st;
    
    irr::core::array<int> integers;
    irr::core::array<irr::core::stringc> integers_c;
    irr::core::array<ESHADER_TYPE> integers_st;
    
    irr::core::array<irr::core::matrix4> matrixes4;
    irr::core::array<irr::core::stringc> matrixes4_c;
    irr::core::array<ESHADER_TYPE> matrixes4_st;
    
    irr::core::array<irr::video::SColorf> colors;
    irr::core::array<irr::core::stringc> colors_c;
    irr::core::array<ESHADER_TYPE> colors_st;
    
    //MATERIAL METHODS
    void buildMaterial(irr::video::IVideoDriver *driver);
    irr::s32 getMaterial() { return material; }
    
    void setVertexShader(irr::core::stringc newVertexShader) { vertexShader = newVertexShader; }
    irr::core::stringc getVertexShader() { return vertexShader; }
    void setPixelShader(irr::core::stringc newPixelShader) { pixelShader = newPixelShader; }
    irr::core::stringc getPixelShader() { return pixelShader; }
    
    //SHADER TYPE
    void setVertexShaderType(irr::video::E_VERTEX_SHADER_TYPE _vertexShaderType) { vertexShaderType = _vertexShaderType; }
    irr::video::E_VERTEX_SHADER_TYPE getVertexShaderType() { return vertexShaderType; }
    void setPixelShaderType(irr::video::E_PIXEL_SHADER_TYPE _pixelShaderType) { pixelShaderType = _pixelShaderType; }
    irr::video::E_PIXEL_SHADER_TYPE getPixelShaderType() { return pixelShaderType; }
    void setBaseMaterial(irr::video::E_MATERIAL_TYPE _baseMaterial) { baseMaterial = _baseMaterial; }
    irr::video::E_MATERIAL_TYPE getBaseMaterial() { return baseMaterial; }
    
    //NAME METHODS
    void setName(irr::core::stringc newName) { name = newName; }
    irr::core::stringc getName() { return name; }
    
    //CONSTANTS METHODS
    void setConstants(irr::core::stringc newConstants) { constants = newConstants; }
    irr::core::stringc getConstants() { return constants; }
    
    //DEVICE METHODS
    void setDevice(irr::IrrlichtDevice *_device) { device = _device; }
    irr::IrrlichtDevice *getDevice() { return device; }
    
    //INHERITANCE METHODS
    virtual void OnSetConstants(irr::video::IMaterialRendererServices *services, irr::s32 userData);
    
private:
    
    //METHODS
    void buildConstants(irr::video::IVideoDriver *_driver);
    
    //MATERIAL INFORMATIONS
    irr::s32 material;
    irr::core::stringc name;
    irr::core::stringc constants;
    
    irr::core::stringc vertexShader;
    irr::core::stringc pixelShader;
    
    irr::video::E_VERTEX_SHADER_TYPE vertexShaderType;
    irr::video::E_PIXEL_SHADER_TYPE pixelShaderType;
    irr::video::E_MATERIAL_TYPE baseMaterial;
    
    //IRRLICHT DEVICE
    irr::IrrlichtDevice *device;
    
};

#endif
