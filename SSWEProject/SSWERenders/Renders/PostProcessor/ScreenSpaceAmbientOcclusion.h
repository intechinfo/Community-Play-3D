#ifndef __SCREEN_SPACE_AMBIANT_OCCLUSION__
#define __SCREEN_SPACE_AMBIANT_OCCLUSION__

#include <SSWERenders.h>
#include "../XEffect/EffectHandler.h"

//---------------------------------------------------------------------------------------------
//------------------------------------------CALLBACKS------------------------------------------
//---------------------------------------------------------------------------------------------

#ifndef _SSWE_LINUX_
class SSWE_RENDERS_API SSAORenderCallback : public SSWE_RENDERS_EXPORTS IPostProcessingRenderCallback {
#else
class SSAORenderCallback : public IPostProcessingRenderCallback {
#endif

public:

	SSAORenderCallback(irr::s32 materialTypeIn);
	~SSAORenderCallback();

	void OnPreRender(EffectHandler* effect);
	void OnPostRender(EffectHandler* effect);

	void setTexture(irr::video::ITexture *tex) { this->tex = tex; }
	irr::video::ITexture *getTexture() { return this->tex; }

private:

	irr::core::matrix4 viewProj;
	irr::s32 materialType;
	irr::video::ITexture *tex;

};

#ifndef _SSWE_LINUX_
class SSWE_RENDERS_API LightScatteringCallback : public SSWE_RENDERS_EXPORTS IPostProcessingRenderCallback {
#else
class LightScatteringCallback : public IPostProcessingRenderCallback {
#endif

public:

	LightScatteringCallback(irr::s32 materialTypeIn, EffectHandler *effect);
	~LightScatteringCallback();

	void OnPreRender(EffectHandler* effect);
	void OnPostRender(EffectHandler* effect);

	void setTexture(irr::video::ITexture *tex) { this->tex = tex; }
	irr::video::ITexture *getTexture() { return this->tex; }

private:

	irr::s32 materialType;
	irr::video::ITexture *tex;

	irr::video::ITexture *blackTexture;

};

//---------------------------------------------------------------------------------------------
//-----------------------------------RENDER CALLBACKS------------------------------------------
//---------------------------------------------------------------------------------------------

#ifndef _SSWE_LINUX_
class SSWE_RENDERS_API CRenderCallbacks {
#else
class CRenderCallbacks {
#endif

public:

		CRenderCallbacks(EffectHandler *effect, irr::core::stringc ext=".hlsl", irr::core::stringc workingDir="");

		~CRenderCallbacks();

		//SSAO
		irr::core::array<irr::s32> buildSSAO();
		void removeSSAO();
		SSAORenderCallback *getSSAORenderCallback() { return ssaoRenderCallback; }

		//LIGHT SCATTERING
		irr::s32 buildLightScattering();
		void removeLightScattering();
		LightScatteringCallback *getLightScatteringCallback() { return lightScatteringCallback; }


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

	//LIGHT SCATTERING
	irr::s32 LightScattering;
	LightScatteringCallback *lightScatteringCallback;
};

#endif
