#include "stdafx.h"
#include "CScreenSpaceAmbientOcclusion.h"

#include "../XEffect/EffectHandler.h"

//---------------------------------------------------------------------------------------------
//---------------------------------------CALLBACKS---------------------------------------------
//---------------------------------------------------------------------------------------------
//SSAO
SSAORenderCallback::SSAORenderCallback(irr::s32 materialTypeIn, irr::video::ITexture *depthTex) {
	materialType = materialTypeIn;
	noiseTex = 0;
	this->depthTex = depthTex;
}

SSAORenderCallback::~SSAORenderCallback() {

}

void SSAORenderCallback::OnPostRender(ISSWERender* effect) {

}

void SSAORenderCallback::OnPreRender(ISSWERender* effect) {
	irr::video::IVideoDriver* driver = effect->getIrrlichtDevice()->getVideoDriver();
	viewProj = driver->getTransform(irr::video::ETS_PROJECTION) * driver->getTransform(irr::video::ETS_VIEW);
	effect->setPostProcessingEffectConstant(materialType, "mViewProj", viewProj.pointer(), 16);

	effect->setPostProcessingUserTexture(noiseTex);

	((EffectHandler *)effect)->getScreenQuadPtr()->getMaterial().setTexture(2, depthTex);
}

//---------------------------------------------------------------------------------------------
//-----------------------------------RENDER CALLBACKS------------------------------------------
//---------------------------------------------------------------------------------------------

CRenderCallbacks::CRenderCallbacks(EffectHandler *effect, irr::core::stringc ext, irr::core::stringc workingDir) {
	reffect = effect;

	workingDirirectory = workingDir;
	extention = ext;

	ssaoRenderCallback = 0;
}

CRenderCallbacks::~CRenderCallbacks() {
	reffect->setPostProcessingUserTexture(0);
	reffect->getIrrlichtDevice()->getVideoDriver()->removeTexture(0);

	removeSSAO();
}

//SSAO
irr::core::array<irr::s32> CRenderCallbacks::buildSSAO() {

    #ifndef _IRR_OSX_PLATFORM_
	SSAO = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/HLSL/SSAO") + extention, 0);
	BlurH = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/HLSL/BlurHP") + extention, 0);
	BlurV = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/HLSL/BlurVP") + extention, 0);
	//SSAOCombine = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/HLSL/SSAOCombine") + extention, 0);
    #else
    SSAO = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/GLSL/SSAO") + extention, 0, false);
	BlurH = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/GLSL/BlurHP") + extention, 0, false);
	BlurV = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/GLSL/BlurVP") + extention, 0, false);
	SSAOCombine = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/GLSL/SSAOCombine") + extention, 0, false);
    #endif

	ssaoRenderCallback = new SSAORenderCallback(SSAO, reffect->getIrrlichtDevice()->getVideoDriver()->getTexture("SSAODepthRTT"));
	reffect->setPostProcessingRenderCallback(SSAO, ssaoRenderCallback);
	ssaoRenderCallback->setTexture(reffect->generateRandomVectorTexture(irr::core::dimension2du(512, 512)));
	reffect->setPostProcessingUserTexture(ssaoRenderCallback->getTexture());

	irr::core::array<irr::s32> materialTypes;
	materialTypes.push_back(SSAO);
	materialTypes.push_back(BlurH);
	materialTypes.push_back(BlurV);
	materialTypes.push_back(SSAOCombine);

	return materialTypes;
}

void CRenderCallbacks::removeSSAO() {
	reffect->removePostProcessingEffect(SSAO);
	reffect->removePostProcessingEffect(BlurH);
	reffect->removePostProcessingEffect(BlurV);
	reffect->removePostProcessingEffect(SSAOCombine);

	reffect->setPostProcessingUserTexture(0);

	reffect->getIrrlichtDevice()->getVideoDriver()->removeTexture(ssaoRenderCallback->getTexture());
	ssaoRenderCallback->setTexture(0);
	ssaoRenderCallback = 0;
}
