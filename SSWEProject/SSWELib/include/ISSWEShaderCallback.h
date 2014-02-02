#ifndef __I_SSWE_SHADER_CALLBACK_H_INCLUDED__
#define __I_SSWE_SHADER_CALLBACK_H_INCLUDED__

#include <irrlicht.h>

enum ESHADER_TYPE {
	EST_VERTEX = 0,
	EST_PIXEL
};

enum ECOLOR_TYPE {
	ECT_SCOLORF = 0,
	ECT_SCOLOR
};

class IShaderCallback : public irr::video::IShaderConstantSetCallBack {

public:

	//METHODS

	//MATERIAL METHODS
	virtual void buildMaterial(irr::video::IVideoDriver *driver) = 0;
	virtual irr::s32 getMaterial() = 0;

	virtual void setVertexShader(irr::core::stringc newVertexShader) = 0;
	virtual irr::core::stringw getVertexShader() = 0;
	virtual void setPixelShader(irr::core::stringc newPixelShader) = 0;
	virtual irr::core::stringw getPixelShader() = 0;

	//SHADER TYPE
	virtual void setVertexShaderType(irr::video::E_VERTEX_SHADER_TYPE _vertexShaderType) = 0;
	virtual irr::video::E_VERTEX_SHADER_TYPE getVertexShaderType() = 0;
	virtual void setPixelShaderType(irr::video::E_PIXEL_SHADER_TYPE _pixelShaderType) = 0;
	virtual irr::video::E_PIXEL_SHADER_TYPE getPixelShaderType()= 0;
	virtual void setBaseMaterial(irr::video::E_MATERIAL_TYPE _baseMaterial) = 0;
	virtual irr::video::E_MATERIAL_TYPE getBaseMaterial() = 0;

	//NAME METHODS
	virtual void setName(irr::core::stringc newName) = 0;
	virtual irr::core::stringc getName() = 0;

	//CONSTANTS METHODS
	virtual void setConstants(irr::core::stringc newConstants) = 0;
	virtual irr::core::stringc getConstants() = 0;

	//DEVICE METHODS
	virtual irr::IrrlichtDevice *getDevice() = 0;

	//INHERITANCE METHODS
	//virtual void OnSetConstants(irr::video::IMaterialRendererServices *services, irr::s32 userData) = 0;

};


#endif