#include "stdafx.h"
#include "ScreenSpaceAmbientOcclusion.h"

//---------------------------------------------------------------------------------------------
//---------------------------------------CALLBACKS---------------------------------------------
//---------------------------------------------------------------------------------------------
//SSAO
SSAORenderCallback::SSAORenderCallback(irr::s32 materialTypeIn) { 
	materialType = materialTypeIn;
	tex = 0;
}

SSAORenderCallback::~SSAORenderCallback() {

}

void SSAORenderCallback::OnPostRender(ISSWERender* effect) {

}

void SSAORenderCallback::OnPreRender(ISSWERender* effect) {
	irr::video::IVideoDriver* driver = effect->getIrrlichtDevice()->getVideoDriver();
	viewProj = driver->getTransform(irr::video::ETS_PROJECTION) * driver->getTransform(irr::video::ETS_VIEW);
	effect->setPostProcessingEffectConstant(materialType, "mViewProj", viewProj.pointer(), 16);

	effect->setPostProcessingUserTexture(tex);
}

//LIGHT SCATTERING
LightScatteringCallback::LightScatteringCallback(irr::s32 materialTypeIn, EffectHandler *effect) { 
	materialType = materialTypeIn;
	tex = 0;

	irr::video::IImage *bi = effect->getIrrlichtDevice()->getVideoDriver()->createImage(irr::video::ECF_R8G8B8, irr::core::dimension2du(8, 8));
	for (u32 i=0; i < 8; i++)
		for (u32 j=0; j < 8; j++)
			bi->setPixel(i, j, irr::video::SColor(0, 0, 0, 0));

	blackTexture = effect->getIrrlichtDevice()->getVideoDriver()->addTexture("blackTexture", bi);
	delete bi;

}

LightScatteringCallback::~LightScatteringCallback() { 

}

void LightScatteringCallback::OnPostRender(ISSWERender* effect) {

}

void LightScatteringCallback::OnPreRender(ISSWERender* effect) {

	irr::video::IVideoDriver *driver = effect->getIrrlichtDevice()->getVideoDriver();
	irr::core::array<irr::scene::ISceneNode *> ar = ((EffectHandler*)effect)->getDepthPassNodes();
	driver->setRenderTarget(tex, true, true, irr::video::SColor(255, 0, 0, 0));

	for (u32 i=0; i < ar.size(); i++) {
		core::array<irr::video::SMaterial> BufferMaterialList(ar[i]->getMaterialCount());
		BufferMaterialList.set_used(0);

		for(u32 g = 0;g < ar[i]->getMaterialCount(); ++g) {
			BufferMaterialList.push_back(ar[i]->getMaterial(g));
		}

		if (ar[i]->getType() != irr::scene::ESNT_BILLBOARD) {
			ar[i]->setMaterialType(irr::video::EMT_SOLID);
			for (u32 h=0; h < irr::video::MATERIAL_MAX_TEXTURES; h++)
				ar[i]->setMaterialTexture(h, blackTexture);
		}

		ar[i]->OnAnimate(effect->getIrrlichtDevice()->getTimer()->getTime());
        ar[i]->render();
		
		for(u32 g = 0;g < ar[i]->getMaterialCount(); ++g)
				ar[i]->getMaterial(g) = BufferMaterialList[g];

	}

	effect->setPostProcessingUserTexture(tex);

	if (((EffectHandler*)effect)->getShadowLightCount() > 0) {
		irr::core::vector3df screenPosition = ((EffectHandler*)effect)->getShadowLight(0).getPosition();

		core::vector2di scrPos = effect->getIrrlichtDevice()->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(screenPosition);
		core::vector2df screen((float)scrPos.X/(float)effect->getIrrlichtDevice()->getVideoDriver()->getScreenSize().Width,
								(float)scrPos.Y/(float)effect->getIrrlichtDevice()->getVideoDriver()->getScreenSize().Height);
		screen.Y = 1-screen.Y;
		screen.X = 1-screen.X;
		effect->setPostProcessingEffectConstant(materialType, "lightPositionOnScreen", &screen.X, 2);
	}
}

//---------------------------------------------------------------------------------------------
//-----------------------------------RENDER CALLBACKS------------------------------------------
//---------------------------------------------------------------------------------------------

CRenderCallbacks::CRenderCallbacks(EffectHandler *effect, irr::core::stringc ext, irr::core::stringc workingDir) {
	reffect = effect;

	workingDirirectory = workingDir;
	extention = ext;

	ssaoRenderCallback = 0;
	lightScatteringCallback = 0;
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
	SSAOCombine = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/HLSL/SSAOCombine") + extention, 0);
    #else
    SSAO = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/GLSL/SSAO") + extention, 0, false);
	BlurH = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/GLSL/BlurHP") + extention, 0, false);
	BlurV = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/GLSL/BlurVP") + extention, 0, false);
	SSAOCombine = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/GLSL/SSAOCombine") + extention, 0, false);
    #endif

	ssaoRenderCallback = new SSAORenderCallback(SSAO);
	reffect->setPostProcessingRenderCallback(SSAO, ssaoRenderCallback);
	ssaoRenderCallback->setTexture(reffect->generateRandomVectorTexture(irr::core::dimension2du(2048, 2048)));
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

irr::s32 CRenderCallbacks::buildLightScattering() {

	LightScattering = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/HLSL/GodRays") + extention, 0);

	lightScatteringCallback = new LightScatteringCallback(LightScattering, reffect);
	reffect->setPostProcessingRenderCallback(LightScattering, lightScatteringCallback);
	lightScatteringCallback->setTexture(reffect->getIrrlichtDevice()->getVideoDriver()->addRenderTargetTexture(reffect->getIrrlichtDevice()->getVideoDriver()->getScreenSize(), "LightScatteringRTT"));
	reffect->setPostProcessingUserTexture(lightScatteringCallback->getTexture());

	return LightScattering;
}

void CRenderCallbacks::removeLightScattering() {
	reffect->removePostProcessingEffect(LightScattering);

	reffect->setPostProcessingUserTexture(0);

	reffect->getIrrlichtDevice()->getVideoDriver()->removeTexture(lightScatteringCallback->getTexture());
	lightScatteringCallback->setTexture(0);
	delete lightScatteringCallback;
	lightScatteringCallback = 0;
}
