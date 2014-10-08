//
//  CNormalMappingMaterial.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 21/05/2014.
//
//

#include "stdafx.h"
#include "CNormalMappingMaterial.h"

#define __CP3D__MAX_LIGHTS__ 8

enum E_SHADER_EXTENSION
{
	ESE_GLSL=0,
	ESE_HLSL,

	ESE_COUNT
};

using namespace irr;
using namespace core;
using namespace video;

CNormalMappingMaterial::CNormalMappingMaterial() {

}

CNormalMappingMaterial::~CNormalMappingMaterial() {

}

void CNormalMappingMaterial::removeLight(irr::u32 indice) {
    lights.erase(indice);
    rebuildCallbackParameters();
}

void CNormalMappingMaterial::addLight(irr::scene::ILightSceneNode *node) {
    if (node) {
        lights.push_back(node);
        rebuildCallbackParameters();
    }
}

void CNormalMappingMaterial::reset() {
    lights.clear();
    rebuildCallbackParameters();
}

void CNormalMappingMaterial::rebuildCallbackParameters() {
    fvLightColorArray.clear();
    fLightStrengthArray.clear();

    for (u32 i=0; i < lights.size(); i++) {
        fvLightColorArray.push_back(lights[i]->getLightData().DiffuseColor.a);
        fvLightColorArray.push_back(lights[i]->getLightData().DiffuseColor.r);
        fvLightColorArray.push_back(lights[i]->getLightData().DiffuseColor.g);
        fvLightColorArray.push_back(lights[i]->getLightData().DiffuseColor.b);

        fLightStrengthArray.push_back(lights[i]->getLightData().Radius);
    }

    rebuildPositions();
}

void CNormalMappingMaterial::rebuildPositions() {
    fvLightPositionArray.clear();
    for (u32 i=0; i < lights.size(); i++) {
        fvLightPositionArray.push_back(lights[i]->getPosition().X);
        fvLightPositionArray.push_back(lights[i]->getPosition().Y);
        fvLightPositionArray.push_back(lights[i]->getPosition().Z);
    }
}

void CNormalMappingMaterial::OnSetConstants(irr::video::IMaterialRendererServices *services, irr::s32 userData) {
    rebuildCallbackParameters();
	s32 lightsCount = lights.size();

	/// Vertex
	matrix4 matWorldInverse;
    matWorldInverse = services->getVideoDriver()->getTransform(ETS_WORLD);
    matWorldInverse.makeInverse();
	if (services->getVideoDriver()->getDriverType() == EDT_OPENGL)
		services->setVertexShaderConstant("matWorldInverse", matWorldInverse.pointer(), 16);
	else
		services->setPixelShaderConstant("matWorldInverse", matWorldInverse.pointer(), 16);

	if (services->getVideoDriver()->getDriverType() == EDT_DIRECT3D9) {
		matrix4 worldView;
		worldView *= services->getVideoDriver()->getTransform(ETS_VIEW);
		worldView *= services->getVideoDriver()->getTransform(ETS_WORLD);
		if (services->getVideoDriver()->getDriverType() == EDT_OPENGL)
			services->setVertexShaderConstant("ModelViewMatrix", worldView.pointer(), 16);
		else
			services->setPixelShaderConstant("ModelViewMatrix", worldView.pointer(), 16);

		matrix4 worldViewProj;
		worldViewProj *= services->getVideoDriver()->getTransform(ETS_PROJECTION);
		worldViewProj *= services->getVideoDriver()->getTransform(ETS_VIEW);
		worldViewProj *= services->getVideoDriver()->getTransform(ETS_WORLD);
		services->setVertexShaderConstant("ModelViewProjectionMatrix", worldViewProj.pointer(), 16);
	} else {
        matrix4 worldView;
		worldView *= services->getVideoDriver()->getTransform(ETS_VIEW);
		worldView *= services->getVideoDriver()->getTransform(ETS_WORLD);
        services->setVertexShaderConstant("ModelViewMatrix", worldView.pointer(), 16);
	}

	if (services->getVideoDriver()->getDriverType() == EDT_OPENGL) {
        #ifdef _IRR_OSX_PLATFORM_
		services->setVertexShaderConstant("fvLightPosition[0]", fvLightPositionArray.pointer(), fvLightPositionArray.size());
		services->setVertexShaderConstant("fLightStrength[0]", fLightStrengthArray.pointer(), fLightStrengthArray.size());
		#else
		services->setVertexShaderConstant("fvLightPosition", fvLightPositionArray.pointer(), fvLightPositionArray.size());
		services->setVertexShaderConstant("fLightStrength", fLightStrengthArray.pointer(), fLightStrengthArray.size());
		#endif
	} else {
		services->setPixelShaderConstant("fvLightPosition", fvLightPositionArray.pointer(), fvLightPositionArray.size());
		services->setPixelShaderConstant("fLightStrength", fLightStrengthArray.pointer(), fLightStrengthArray.size());
	}

	if (services->getVideoDriver()->getDriverType() == EDT_OPENGL)
		services->setVertexShaderConstant("numLights", (int *)&lightsCount, 1);

	/// Pixel
	if (services->getVideoDriver()->getDriverType() == EDT_OPENGL) {
		s32 baseMap = 0;
		s32 bumpMap = 1;
		services->setPixelShaderConstant("baseMap", &baseMap, 1);
		services->setPixelShaderConstant("bumpMap", &bumpMap, 1);
	}

	f32 fvAmbiant[4] = { 1.f, 1.f, 1.f, 1.f };
    services->setPixelShaderConstant(("fvAmbient"), fvAmbiant, 4);

	f32 fvLightColor[__CP3D__MAX_LIGHTS__ * 4];
    for (u32 i=0; i < lights.size(); i++) {
        fvLightColor[i + i*3] = lights[i]->getLightData().DiffuseColor.getRed();
        fvLightColor[i+1 + i*3] = lights[i]->getLightData().DiffuseColor.getGreen();
        fvLightColor[i+2 + i*3] = lights[i]->getLightData().DiffuseColor.getBlue();
        fvLightColor[i+3 + i*3] = lights[i]->getLightData().DiffuseColor.getAlpha();
    }

    #ifdef _IRR_OSX_PLATFORM_
    services->setPixelShaderConstant(services->getVideoDriver()->getDriverType() == EDT_OPENGL ? "fvLightColor[0]"
																							   : "fvLightColor",
									 fvLightColor, fvLightColorArray.size() * 4);
    #else
    services->setPixelShaderConstant(services->getVideoDriver()->getDriverType() == EDT_OPENGL ? "fvLightColor"
																							   : "fvLightColor",
									 fvLightColor, fvLightColorArray.size() * 4);
    #endif

	f32 fSpecularPower = 20.f;
    f32 fSpecularStrength = 0.9f;
    f32 fBumpStrength = 5.f;
    services->setPixelShaderConstant(("fSpecularPower"), &fSpecularPower, 1);
    services->setPixelShaderConstant(("fSpecularStrength"), &fSpecularStrength, 1);
    services->setPixelShaderConstant(("fBumpStrength"), &fBumpStrength, 1);

	services->setPixelShaderConstant("numLights", &lightsCount, 1);
}

void CNormalMappingMaterial::build(irr::video::IVideoDriver *driver) {

    E_SHADER_EXTENSION ext = (driver->getDriverType() == EDT_OPENGL) ? ESE_GLSL : ESE_HLSL;

    IGPUProgrammingServices *gpu = driver->getGPUProgrammingServices();

	const io::path vertexPath = driver->getDriverType() == EDT_OPENGL ? "shaders/Materials/NormalMappingMultiLights/NormalMapping.vert"
																	  : "shaders/Materials/NormalMappingMultiLights/NormalMapping.vsh";
	const io::path pixelPath  = driver->getDriverType() == EDT_OPENGL ? "shaders/Materials/NormalMappingMultiLights/NormalMapping.frag"
																	  : "shaders/Materials/NormalMappingMultiLights/NormalMapping.psh";

	nmSolid = gpu->addHighLevelShaderMaterialFromFiles(vertexPath, "vertexMain", EVST_VS_3_0,
                                                       pixelPath, "pixelMain", EPST_PS_3_0,
                                                       this, EMT_SOLID);

    printf("\n\n %d \n\n", nmSolid);
    printf("\n\n %d \n\n", nmTransAdd);
    printf("\n\n %d \n\n", nmTransAlphaRef);

}
