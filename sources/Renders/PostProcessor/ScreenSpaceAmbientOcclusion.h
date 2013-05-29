#ifndef __SCREEN_SPACE_AMBIANT_OCCLUSION__
#define __SCREEN_SPACE_AMBIANT_OCCLUSION__

#include <SSWERenders.h>
#include "../XEffect/EffectHandler.h"

class SSAORenderCallback : public IPostProcessingRenderCallback {

public:

	SSAORenderCallback(irr::s32 materialTypeIn) { materialType = materialTypeIn; }

	void OnPreRender(EffectHandler* effect) {
		irr::video::IVideoDriver* driver = effect->getIrrlichtDevice()->getVideoDriver();
		viewProj = driver->getTransform(irr::video::ETS_PROJECTION) * driver->getTransform(irr::video::ETS_VIEW);
		effect->setPostProcessingEffectConstant(materialType, "mViewProj", viewProj.pointer(), 16);
	}

	void OnPostRender(EffectHandler* effect) { }

private:

	core::matrix4 viewProj;
	s32 materialType;

};

class CRenderCallbacks {

public:

		CRenderCallbacks(EffectHandler *effect, irr::core::stringc workingDir = "") {
			reffect = effect;
			randVecTexture = reffect->generateRandomVectorTexture(irr::core::dimension2du(4096, 4096));
			reffect->setPostProcessingUserTexture(randVecTexture);

			workingDirirectory = workingDir;

			ssaoRenderCallback = 0;
		}

		~CRenderCallbacks() {
			reffect->setPostProcessingUserTexture(0);
			reffect->getIrrlichtDevice()->getVideoDriver()->removeTexture(0);

			removeSSAO();
		}

		//SSAO
		irr::core::array<irr::s32> buildSSAO(irr::core::stringc ext) {

			SSAOCombine = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/HLSL/SSAOCombine") + ext, 0, true);
			BlurV = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/HLSL/BlurVP") + ext, 0, true);
			BlurH = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/HLSL/BlurHP") + ext, 0, true);
			SSAO = reffect->addPostProcessingEffectFromFile(workingDirirectory + irr::core::stringc("shaders/HLSL/SSAO") + ext, 0, true);

			ssaoRenderCallback = new SSAORenderCallback(SSAO);
			reffect->setPostProcessingRenderCallback(SSAO, ssaoRenderCallback);

			irr::core::array<irr::s32> materialTypes;
			materialTypes.push_back(SSAO);
			materialTypes.push_back(BlurH);
			materialTypes.push_back(BlurV);
			materialTypes.push_back(SSAOCombine);

			return materialTypes;
		}

		void removeSSAO() {
			reffect->removePostProcessingEffect(SSAO);
			reffect->removePostProcessingEffect(BlurH);
			reffect->removePostProcessingEffect(BlurV);
			reffect->removePostProcessingEffect(SSAOCombine);

			reffect->setPostProcessingUserTexture(0);

			ssaoRenderCallback = 0;
		}

		SSAORenderCallback *getSSAORenderCallback() { return ssaoRenderCallback; }

private:

	irr::video::ITexture *randVecTexture;
	irr::core::matrix4 viewProj;
	EffectHandler *reffect;
	irr::core::stringc workingDirirectory;

	//SSAO
	irr::s32 SSAO;
	irr::s32 BlurH;
	irr::s32 BlurV;
	irr::s32 SSAOCombine;
	SSAORenderCallback *ssaoRenderCallback;
};

#endif
