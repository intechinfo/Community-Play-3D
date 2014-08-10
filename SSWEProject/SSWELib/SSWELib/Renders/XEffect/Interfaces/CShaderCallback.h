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

#include <ISSWEShaderCallback.h>

class CDevices;
class CUICodeEditor;

class CShaderCallback : public IShaderCallback {

public:

	CShaderCallback(E_DRIVER_TYPE driverType);
	~CShaderCallback();

	//METHODS
	CUICodeEditor *modifyVertexShader(CDevices *devices);
	CUICodeEditor *modifyPixelShader(CDevices *devices);
	CUICodeEditor *modifyConstants(CDevices *devices);

	//CONSTANTS
	irr::core::array<irr::core::vector2df> vectors2D;
	irr::core::array<irr::core::stringc> vectors2D_c;
	irr::core::array<ESHADER_TYPE> vectors2D_st;

	irr::core::array<irr::core::vector3df> vectors3D;
	irr::core::array<irr::core::stringc> vectors3D_c;
	irr::core::array<ESHADER_TYPE> vectors3D_st;
    irr::core::array<bool> vectors3DAs4Values;

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
	irr::core::array<ECOLOR_TYPE> colors_t;

	//MATERIAL METHODS
	void buildMaterial(irr::video::IVideoDriver *driver);
	irr::s32 getMaterial() { return material; }

	void setVertexShader(irr::core::stringc newVertexShader) { vertexShader = newVertexShader; }
	irr::core::stringw getVertexShader() { return vertexShader; }
	void setPixelShader(irr::core::stringc newPixelShader) { pixelShader = newPixelShader; }
	irr::core::stringw getPixelShader() { return pixelShader; }

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
	void setConstants(irr::core::stringc newConstants);
	irr::core::stringc getConstants() { return constants; }

	//DEVICE METHODS
	void setDevice(irr::IrrlichtDevice *_device) { device = _device; }
	irr::IrrlichtDevice *getDevice() { return device; }

	//INHERITANCE METHODS
	virtual void OnSetConstants(irr::video::IMaterialRendererServices *services, irr::s32 userData);
	virtual void OnSetMaterial(const SMaterial& material);

private:

	//METHODS
	void buildConstants(irr::video::IVideoDriver *_driver);

	//MATERIAL INFORMATIONS
	irr::s32 material;
	const irr::video::SMaterial *smaterial;
	irr::core::stringc name;

	irr::core::stringw constants;
	irr::core::stringw vertexShader;
	irr::core::stringw pixelShader;

	irr::video::E_VERTEX_SHADER_TYPE vertexShaderType;
	irr::video::E_PIXEL_SHADER_TYPE pixelShaderType;
	irr::video::E_MATERIAL_TYPE baseMaterial;

	//IRRLICHT DEVICE AND DATAS
	irr::IrrlichtDevice *device;
    irr::core::matrix4 nullMatrix;

};

#endif
