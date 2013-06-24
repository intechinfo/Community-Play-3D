#ifndef __SCREEN_SPACE_AMBIANT_OCCLUSION__
#define __SCREEN_SPACE_AMBIANT_OCCLUSION__

#include <SSWERenders.h>
#include "../XEffect/EffectHandler.h"

class SSWE_RENDERS_API SSAORenderCallback : public SSWE_RENDERS_EXPORTS IPostProcessingRenderCallback {

public:

	SSAORenderCallback(irr::s32 materialTypeIn);
	~SSAORenderCallback();

	void OnPreRender(EffectHandler* effect);
	void OnPostRender(EffectHandler* effect);

private:

	irr::core::matrix4 viewProj;
	irr::s32 materialType;

};

class SSWE_RENDERS_API CRenderCallbacks {

public:

		CRenderCallbacks(EffectHandler *effect, irr::core::stringc ext=".hlsl", irr::core::stringc workingDir="");

		~CRenderCallbacks();

		//SSAO
		irr::core::array<irr::s32> buildSSAO();
		void removeSSAO();
		SSAORenderCallback *getSSAORenderCallback() { return ssaoRenderCallback; }

		//OTHER

private:

	irr::video::ITexture *randVecTexture;
	irr::core::matrix4 viewProj;
	EffectHandler *reffect;

	irr::core::stringc workingDirirectory;
	irr::core::stringc extention;

	//SSAO
	irr::s32 SSAO;
	irr::s32 BlurH;
	irr::s32 BlurV;
	irr::s32 SSAOCombine;
	SSAORenderCallback *ssaoRenderCallback;
};

#endif
