//
//  CNormalMappingMaterial.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 21/05/2014.
//
//

#include "stdafx.h"
#include "CNormalMappingMaterial.h"

#ifdef _IRR_OSX_PLATFORM_
#include "../XEffect/EffectShaders.h"

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
    
    s32 baseMap = 0;
    s32 bumpMap = 1;
    services->setPixelShaderConstant(services->getPixelShaderConstantID("baseMap"), &baseMap, 1);
    services->setPixelShaderConstant(services->getPixelShaderConstantID("bumpMap"), &bumpMap, 1);
    
    s32 lightsCount = lights.size();
    services->setPixelShaderConstant(services->getPixelShaderConstantID("lightsCount"), &lightsCount, 1);
    services->setVertexShaderConstant(services->getVertexShaderConstantID("lightsCount"), &lightsCount, 1);
    
    f32 fSpecularPower = 20.f;
    f32 fSpecularStrength = 0.9f;
    f32 fBumpStrength = 5.f;
    services->setPixelShaderConstant(services->getPixelShaderConstantID("fSpecularPower"), &fSpecularPower, 1);
    services->setPixelShaderConstant(services->getPixelShaderConstantID("fSpecularStrength"), &fSpecularStrength, 1);
    services->setPixelShaderConstant(services->getPixelShaderConstantID("fBumpStrength"), &fBumpStrength, 1);
    
    matrix4 matWorldInverse;
    matWorldInverse *= services->getVideoDriver()->getTransform(ETS_WORLD);
    matWorldInverse.makeInverse();
    services->setVertexShaderConstant(services->getVertexShaderConstantID("matWorldInverse"), matWorldInverse.pointer(), 16);
    
    services->setVertexShaderConstant(services->getVertexShaderConstantID("fvLightPosition"), fvLightPositionArray.pointer(), fvLightPositionArray.size());
    services->setVertexShaderConstant(services->getVertexShaderConstantID("fLightStrength"), fLightStrengthArray.pointer(), fLightStrengthArray.size());
    
    SColorf fvAmbient(0.7f, 0.7f, 0.7f, 1.f);
    services->setPixelShaderConstant(services->getPixelShaderConstantID("fvAmbient"), (float*)(&fvAmbient), 4);
    services->setPixelShaderConstant(services->getPixelShaderConstantID("fvLightColor"), fvLightColorArray.pointer(), fvLightColorArray.size());

}

void CNormalMappingMaterial::build(irr::video::IVideoDriver *driver) {
    const char *NORMAL_MAPPING_V[ESE_COUNT] = {
        "#define __CP3D__MAX_LIGHTS 32\n"
        "\n"
        "uniform vec3 fvLightPosition[__CP3D__MAX_LIGHTS];\n"
        "uniform float fLightStrength[__CP3D__MAX_LIGHTS];\n"
        "uniform int lightsCount;\n"
        "uniform mat4 matWorldInverse;\n"
        "\n"
        "varying vec2 Texcoord;\n"
        "varying vec3 ViewDirection;\n"
        "varying vec3 LightDirection[__CP3D__MAX_LIGHTS];\n"
        "varying vec4 LightDistMultiplier[__CP3D__MAX_LIGHTS];\n"
        "\n"
        "float getLengthSQR (vec3 vec)\n"
        "{\n"
        "    return(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);\n"
        "}\n"
        "\n"
        "void main( void ){\n"
        "    mat4 LightTransform= gl_ModelViewMatrix;\n"
        "    LightTransform=LightTransform*matWorldInverse;\n"
        "\n"
        "    gl_Position = ftransform();\n"
        "    Texcoord = gl_MultiTexCoord0.xy;\n"
        "\n"
        "    vec4 fvObjectPosition = gl_ModelViewMatrix * gl_Vertex;\n"
        "\n"
        "    // Calculate Light Pos\n"
        "    vec4 fvLightPos[__CP3D__MAX_LIGHTS];\n"
        "    for (int i=0; i < lightsCount; i++) {\n"
        "        fvLightPos[i] = LightTransform * vec4(fvLightPosition[i],1.0);\n"
        "    }\n"
        "\n"
        "    vec3 fvViewDirection  =  - fvObjectPosition.xyz;\n"
        "\n"
        "    // Calculate Light Direction\n"
        "    vec3 fvLightDirection[__CP3D__MAX_LIGHTS];\n"
        "    for (int i=0; i < lightsCount; i++) {\n"
        "        fvLightDirection[i] = (fvLightPos[i].xyz - fvObjectPosition.xyz);\n"
        "    }\n"
        "\n"
        "    for (int i=0; i < lightsCount; i++) {\n"
        "        LightDistMultiplier[i]=1.0/(getLengthSQR(fvLightDirection[i])/(fLightStrength[i]*10000.0));\n"
        "    }\n"
        "\n"
        "    vec3 fvNormal         = gl_NormalMatrix * gl_Normal;\n"
        "\n"
        "    vec3 fvTangent   = -vec3(abs(gl_Normal.y) + abs(gl_Normal.z), abs(gl_Normal.x), 0);\n"
        "    vec3 fvBinormal =cross(fvTangent,gl_Normal);\n"
        "    fvTangent=gl_NormalMatrix*cross(fvBinormal,gl_Normal);\n"
        "    fvBinormal=gl_NormalMatrix*fvBinormal;\n"
        "\n"
        "\n"
        "    ViewDirection.x  = dot( fvTangent, fvViewDirection );\n"
        "    ViewDirection.y  = dot( fvBinormal, fvViewDirection );\n"
        "    ViewDirection.z  = dot( fvNormal, fvViewDirection );\n"
        "\n"
        "    for (int i=0; i < lightsCount; i++) {\n"
        "        LightDirection[i].x  = dot( fvTangent, fvLightDirection[i].xyz );\n"
        "        LightDirection[i].y  = dot( fvBinormal, fvLightDirection[i].xyz );\n"
        "        LightDirection[i].z  = dot( fvNormal, fvLightDirection[i].xyz );\n"
        "    }\n"
        "}\n"
        "\n"
    ,
    ""
    };
    
    const char *NORMAL_MAPPING_P[ESE_COUNT] = {
        "#define __CP3D__MAX_LIGHTS 32\n"
        "\n"
        "uniform int lightsCount;\n"
        "\n"
        "uniform vec4 fvAmbient;\n"
        "uniform vec4 fvLightColor[__CP3D__MAX_LIGHTS];\n"
        "\n"
        "uniform float fSpecularPower;\n"
        "uniform float fSpecularStrength;\n"
        "uniform float fBumpStrength;\n"
        "\n"
        "uniform sampler2D baseMap;\n"
        "uniform sampler2D bumpMap;\n"
        "\n"
        "varying vec2 Texcoord;\n"
        "varying vec3 ViewDirection;\n"
        "varying vec3 LightDirection[__CP3D__MAX_LIGHTS];\n"
        "varying vec4 LightDistMultiplier[__CP3D_MAX_LIGHTS];\n"
        "\n"
        "void main( void ){\n"
        "    if (lightsCount == 0) {\n"
        "        gl_FragColor = texture2D(baseMap, Texcoord.xy);\n"
        "        return;\n"
        "    }\n"
        "    vec3 fvLightDirection[__CP3D__MAX_LIGHTS];\n"
        "    for (int i=0; i < lightsCount; i++) {\n"
        "        fvLightDirection[i] = normalize( LightDirection[i] );\n"
        "    }\n"
        "\n"
        "    vec3  fvNormal = texture2D( bumpMap, Texcoord ).yxz;\n"
        "    fvNormal.xy*=2.0;\n"
        "    fvNormal.xy-=1.0;\n"
        "\n"
        "    fvNormal=(vec3(0.0,0.0,1.0)-fvNormal)*fBumpStrength+fvNormal;\n"
        "\n"
        "    fvNormal=normalize(fvNormal);\n"
        "\n"
        "    float fNDotL[__CP3D__MAX_LIGHTS];\n"
        "    for (int i=0; i < lightsCount; i++) {\n"
        "        fNDotL[i] = max(dot(fvNormal, fvLightDirection[i]),0.0)-0.1;\n"
        "    }\n"
        "\n"
        "    vec3 fvReflection[__CP3D__MAX_LIGHTS];\n"
        "    for (int i=0; i < lightsCount; i++) {\n"
        "        fvReflection[i] = normalize( ( ( 2.0 * fvNormal )  ) - fvLightDirection[i] );\n"
        "    }\n"
        "\n"
        "    vec3  fvViewDirection  = normalize( ViewDirection );\n"
        "\n"
        "    float fRDotV[__CP3D__MAX_LIGHTS];\n"
        "    for (int i=0; i < lightsCount; i++) {\n"
        "        fRDotV[i] = max( 0.0, dot( fvReflection[i], fvViewDirection ) );\n"
        "    }\n"
        "\n"
        "    vec4  fvBaseColor = texture2D( baseMap, Texcoord );\n"
        "\n"
        "    vec4  fvTotalAmbient   = fvAmbient * fvBaseColor;\n"
        "\n"
        "    vec4  fvTotalDiffuse   = fvLightColor[0] * fNDotL[0]* fvBaseColor*LightDistMultiplier[0];\n"
        "    vec4  fvTotalSpecular  = fNDotL[0]*fvLightColor[0] * ( pow( fRDotV[0], fSpecularPower ) )*LightDistMultiplier[0];\n"
        "\n"
        "\n"
        "\n"
        "    vec4 color=( fvTotalAmbient + fvTotalDiffuse+ (fvTotalSpecular*fSpecularStrength));\n"
        "    if(color.r>1.0){color.gb+=color.r-1.0;}\n"
        "    if(color.g>1.0){color.rb+=color.g-1.0;}\n"
        "    if(color.b>1.0){color.rg+=color.b-1.0;}\n"
        "\n"
        "    gl_FragColor = color;\n"
        "\n"
        "}\n"
        ,
        ""
    };
    
    E_SHADER_EXTENSION ext = (driver->getDriverType() == EDT_OPENGL) ? ESE_GLSL : ESE_HLSL;
    
    IGPUProgrammingServices *gpu = driver->getGPUProgrammingServices();
    
    nmSolid = gpu->addHighLevelShaderMaterial(stringc(NORMAL_MAPPING_V[ext]).c_str(), "main", EVST_VS_1_1,
                                              stringc(NORMAL_MAPPING_P[ext]).c_str(), "main", EPST_PS_1_1,
                                              this, EMT_SOLID);
    nmTransAdd = gpu->addHighLevelShaderMaterial(stringc(NORMAL_MAPPING_V[ext]).c_str(), "main", EVST_VS_1_1,
                                                 stringc(NORMAL_MAPPING_P[ext]).c_str(), "main", EPST_PS_1_1,
                                                 this, EMT_TRANSPARENT_ADD_COLOR);
    nmTransAlphaRef = gpu->addHighLevelShaderMaterial(stringc(NORMAL_MAPPING_V[ext]).c_str(), "main", EVST_VS_1_1,
                                                      stringc(NORMAL_MAPPING_P[ext]).c_str(), "main", EPST_PS_1_1,
                                                      this, EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
    
    printf("\n\n %d \n\n", nmSolid);
    printf("\n\n %d \n\n", nmTransAdd);
    printf("\n\n %d \n\n", nmTransAlphaRef);
    
}

#endif
