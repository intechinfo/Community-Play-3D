#ifndef __SCREEN_SPACE_AMBIANT_OCCLUSION__
#define __SCREEN_SPACE_AMBIANT_OCCLUSION__

#include <SSWERenders.h>
#include "../XEffect/EffectHandler.h"

//---------------------------------------------------------------------------------------------
//------------------------------------------CALLBACKS------------------------------------------
//---------------------------------------------------------------------------------------------

class SSWE_RENDERS_API SSAORenderCallback : public SSWE_RENDERS_EXPORTS IPostProcessingRenderCallback {

public:

	SSAORenderCallback(irr::s32 materialTypeIn);
	~SSAORenderCallback();

	void OnPreRender(ISSWERender* effect);
	void OnPostRender(ISSWERender* effect);

	void setTexture(irr::video::ITexture *tex) { this->tex = tex; }
	irr::video::ITexture *getTexture() { return this->tex; }

private:

	irr::core::matrix4 viewProj;
	irr::s32 materialType;
	irr::video::ITexture *tex;

};


//---------------------------------------------------------------------------------------------
//-----------------------------------RENDER CALLBACKS------------------------------------------
//---------------------------------------------------------------------------------------------

class SSWE_RENDERS_API CRenderCallbacks {

public:

		CRenderCallbacks(EffectHandler *effect, irr::core::stringc ext=".hlsl", irr::core::stringc workingDir="");

		~CRenderCallbacks();

		//SSAO
		irr::core::array<irr::s32> buildSSAO();
		void removeSSAO();
		SSAORenderCallback *getSSAORenderCallback() { return ssaoRenderCallback; }

private:

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
