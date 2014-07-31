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

/*

The IShaderCallback class allows you to create custom materials
Simply set the pixel, the vertex and the constants codes (handled by strings) and build the material
Example:



*/

class IShaderCallback : public irr::video::IShaderConstantSetCallBack {

public:

	/// Builds the material (creates the shader program)
	/// Once you have set the pixel, vertex and constants codes
	virtual void buildMaterial(irr::video::IVideoDriver *driver) = 0;
	/// Returns the material created by IShaderCallback::buildMaterial.
	/// Returns -1 if the program contains errors
	virtual irr::s32 getMaterial() = 0;

	/// Sets the vertex shader code
	virtual void setVertexShader(irr::core::stringc newVertexShader) = 0;
	/// Returns the vertex shader code
	virtual irr::core::stringw getVertexShader() = 0;
	/// Sets the pixel shader code
	virtual void setPixelShader(irr::core::stringc newPixelShader) = 0;
	/// Returns the pixel shader code
	virtual irr::core::stringw getPixelShader() = 0;

	/// Sets the vertex shader type (for D3D devices)
	virtual void setVertexShaderType(irr::video::E_VERTEX_SHADER_TYPE _vertexShaderType) = 0;
	/// Returns the vertex shader type (for D3D devices)
	virtual irr::video::E_VERTEX_SHADER_TYPE getVertexShaderType() = 0;
	/// Sets the pixel shader type (for D3D devices)
	virtual void setPixelShaderType(irr::video::E_PIXEL_SHADER_TYPE _pixelShaderType) = 0;
	/// Returns the pixel shader type (for D3D devices)
	virtual irr::video::E_PIXEL_SHADER_TYPE getPixelShaderType()= 0;
	/// Sets the base material used by the shader program
	virtual void setBaseMaterial(irr::video::E_MATERIAL_TYPE _baseMaterial) = 0;
	/// Returns the base material used by the shader program
	virtual irr::video::E_MATERIAL_TYPE getBaseMaterial() = 0;


	/// Sets the name of the callback containing the shader program
	virtual void setName(irr::core::stringc newName) = 0;
	/// Returns the name of the callback containing the shader program
	virtual irr::core::stringc getName() = 0;

	/// Sets the constants script
	/// Constants are [v-p]constant_type constant_name [values] [options]
	virtual void setConstants(irr::core::stringc newConstants) = 0;
	///Returns the constants text (string)
	virtual irr::core::stringc getConstants() = 0;

	/// Returns the irrlicht device
	virtual irr::IrrlichtDevice *getDevice() = 0;

};


#endif