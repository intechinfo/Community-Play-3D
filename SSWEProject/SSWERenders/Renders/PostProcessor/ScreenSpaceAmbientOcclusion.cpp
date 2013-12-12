#include "stdafx.h"
#include "ScreenSpaceAmbientOcclusion.h"

//---------------------------------------------------------------------------------------------
//-----------------------------------SSAO------------------------------------------------------
//---------------------------------------------------------------------------------------------
SSAORenderCallback::SSAORenderCallback(irr::s32 materialTypeIn) { 
	materialType = materialTypeIn; 
}

SSAORenderCallback::~SSAORenderCallback() { 
	
}

void SSAORenderCallback::OnPostRender(EffectHandler* effect) {

}

void SSAORenderCallback::OnPreRender(EffectHandler* effect) {
	irr::video::IVideoDriver* driver = effect->getIrrlichtDevice()->getVideoDriver();
	viewProj = driver->getTransform(irr::video::ETS_PROJECTION) * driver->getTransform(irr::video::ETS_VIEW);
	effect->setPostProcessingEffectConstant(materialType, "mViewProj", viewProj.pointer(), 16);
}

//---------------------------------------------------------------------------------------------
//-----------------------------------RENDER CALLBACKS------------------------------------------
//---------------------------------------------------------------------------------------------

CRenderCallbacks::CRenderCallbacks(EffectHandler *effect, irr::core::stringc ext, irr::core::stringc workingDir) {
	reffect = effect;
	randVecTexture = 0;
	reffect->setPostProcessingUserTexture(randVecTexture);

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

	if (!randVecTexture) {
		randVecTexture = reffect->generateRandomVectorTexture(irr::core::dimension2du(512, 512));
	}

    #ifndef _IRR_OSX_PLATFORM_
	SSAO = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/HLSL/SSAO") + extention, 0, false);
	BlurH = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/HLSL/BlurHP") + extention, 0, false);
	BlurV = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/HLSL/BlurVP") + extention, 0, false);
	SSAOCombine = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/HLSL/SSAOCombine") + extention, 0, false);
    #else
    SSAO = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/GLSL/SSAO") + extention, 0, false);
	BlurH = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/GLSL/BlurHP") + extention, 0, false);
	BlurV = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/GLSL/BlurVP") + extention, 0, false);
	SSAOCombine = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/GLSL/SSAOCombine") + extention, 0, false);
    #endif

	ssaoRenderCallback = new SSAORenderCallback(SSAO);
	reffect->setPostProcessingRenderCallback(SSAO, ssaoRenderCallback);

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

	reffect->getIrrlichtDevice()->getVideoDriver()->removeTexture(randVecTexture);
	ssaoRenderCallback = 0;
	randVecTexture = 0;
}
