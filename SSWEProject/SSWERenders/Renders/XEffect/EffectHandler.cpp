#include "stdafx.h"

#include "EffectHandler.h"
#include "EffectCB.h"
#include "EffectShaders.h"

///HDR

#include "../HDR/CHDRPostProcess.h"
#include "../HDR/CHDRScreenQuad.h"
#include "../HDR/CPhongShaderManager.h"
#include "../HDR/CGlobalContext.h"
#include "../HDR/CPostProcessingManager.h"
#include "../HDR/CDSBloomGenerator.h"
#include "../HDR/CLuminanceGenerator.h"
#include "../HDR/CAmplifier.h"

#include <string>
#include <iostream>
#include <fstream>

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

using namespace Graphics;

EffectHandler::EffectHandler(IrrlichtDevice* dev, const irr::core::dimension2du& screenRTTSize,
                             const bool useVSMShadows, const bool useRoundSpotLights, const bool use32BitDepthBuffers)
: device(dev), smgr(dev->getSceneManager()), driver(dev->getVideoDriver()),
ScreenRTTSize(screenRTTSize.getArea() == 0 ? dev->getVideoDriver()->getScreenSize() : screenRTTSize),
ClearColour(0x0), shadowsUnsupported(false), DepthRTT(0), SSAORTT(0), NormalRenderRTT(0), LightScatteringRTT(0),
DepthPass(false), depthMC(0), shadowMC(0),
AmbientColour(0x0), use32BitDepth(use32BitDepthBuffers), useVSM(useVSMShadows)
{
	bool tempTexFlagMipMaps = driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
	bool tempTexFlag32 = driver->getTextureCreationFlag(ETCF_ALWAYS_32_BIT);

	ScreenRTT = driver->addRenderTargetTexture(ScreenRTTSize, "ScreenRTT");
	ScreenQuad.rt[0] = driver->addRenderTargetTexture(ScreenRTTSize, "ColorMapSampler");
	ScreenQuad.rt[1] = driver->addRenderTargetTexture(ScreenRTTSize, "ScreenMapSampler");

	DOFMapSampler = driver->addRenderTargetTexture(ScreenRTTSize, "DOFMapSampler");

	//LIGHT SCATTERING PASS
	LightScatteringRTT.RenderTexture = driver->addRenderTargetTexture(ScreenRTTSize, "LightScatteringRTT");
	useLightScattering = false;

	//HDR PIPELINE
	useHDR = true;

	GlobalContext::DeviceContext.SetDevice(dev);

    ppm = new PostProcessingManager();
	amp = new Amplifier();
	quad = new Graphics::CHDRScreenQuad();

	//psm = new PhongShaderManager(driver, device->getFileSystem()->getWorkingDirectory());
	pp = new HDRPostProcess(ScreenRTTSize);
	ppm->AddPostProcess(pp);
	pp->GetBloomGenerator()->SetGaussianCoefficient(0.3f);

	phong.Lighting = false;
	//phong.MaterialType = psm->getMaterialType();
	phong.Shininess = 1.0f;
	phong.MaterialTypeParam = 200.0f;

	HDRProcessedRT = driver->addRenderTargetTexture(ScreenRTTSize,"HDRProcessedRTT", video::ECF_A8R8G8B8);

	hdrManager = new CHDRManager(pp, &phong);

	//BACK RENDER
	backRenderRTT = driver->addRenderTargetTexture(dimension2du(512, 512), "BackRenderRTT");

	//NORMAL PASS
	NormalRenderRTT.RenderTexture = driver->addRenderTargetTexture(ScreenRTTSize, "NormalPassRTT");
	renderNormalPass = false;

	//MULTI RENDER TARGETS
	DepthRTT.RenderTexture = driver->addRenderTargetTexture(ScreenRTTSize, "DepthRTT", use32BitDepth ? ECF_G32R32F : ECF_G16R16F);
	SSAORTT.RenderTexture = driver->addRenderTargetTexture(ScreenRTTSize, "SSAODepthRTT", use32BitDepth ? ECF_G32R32F : ECF_G16R16F);

	CP3DRenderTargets.clear();
	/*CP3DRenderTargets.push_back(DepthRTT);
	CP3DRenderTargets.push_back(NormalRenderRTT);
	CP3DRenderTargets.push_back(LightScatteringRTT);*/

	//REFLECTION PASS
	useReflectionPass = false;
	ReflectionRTT = driver->addRenderTargetTexture(dimension2du(512, 512), "ReflectionPassRTT");
	cameraForPasses = smgr->addCameraSceneNode(0, core::vector3df(0, 0, 0), core::vector3df(0, 0, 0), -1, false);

	//OTHERS
	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, tempTexFlagMipMaps);
	driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, tempTexFlag32);

	CShaderPreprocessor sPP(driver);

	E_SHADER_EXTENSION shaderExt = (driver->getDriverType() == EDT_DIRECT3D9) ? ESE_HLSL : ESE_GLSL;

	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

	if(gpu && ((driver->getDriverType() == EDT_OPENGL && driver->queryFeature(EVDF_ARB_GLSL)) ||
			   (driver->getDriverType() == EDT_DIRECT3D9 && driver->queryFeature(EVDF_PIXEL_SHADER_2_0)
			   && driver->queryFeature(EVDF_PIXEL_SHADER_3_0))))
	{
		depthMC = new DepthShaderCB(this);
		shadowMC = new ShadowShaderCB(this);
		normalMC = new NormalShaderCB(this);

		Depth = gpu->addHighLevelShaderMaterial(sPP.ppShader(SHADOW_PASS_1V[shaderExt]).c_str(), "vertexMain", video::EVST_VS_2_0,
                                                sPP.ppShader(SHADOW_PASS_1P[shaderExt]).c_str(), "pixelMain", video::EPST_PS_2_0,
                                                depthMC, video::EMT_SOLID);
		DepthT = gpu->addHighLevelShaderMaterial(
                                                 sPP.ppShader(SHADOW_PASS_1V[shaderExt]).c_str(), "vertexMain", video::EVST_VS_2_0,
                                                 sPP.ppShader(SHADOW_PASS_1PT[shaderExt]).c_str(), "pixelMain", video::EPST_PS_2_0,
                                                 depthMC, video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
		Depth2T = gpu->addHighLevelShaderMaterial(sPP.ppShader(SHADOW_PASS_1V[shaderExt]).c_str(), "vertexMain", video::EVST_VS_2_0,
												  sPP.ppShader(SHADOW_PASS_1P_2T[shaderExt]).c_str(), "pixelMain", video::EPST_PS_2_0,
                                                  depthMC, video::EMT_SOLID);

		WhiteWash = gpu->addHighLevelShaderMaterial(
                                                    sPP.ppShader(SHADOW_PASS_1V[shaderExt]).c_str(), "vertexMain", video::EVST_VS_2_0,
                                                    sPP.ppShader(WHITE_WASH_P[shaderExt]).c_str(), "pixelMain", video::EPST_PS_2_0,
                                                    depthMC, video::EMT_SOLID);

		WhiteWashTRef = gpu->addHighLevelShaderMaterial(
                                                        sPP.ppShader(SHADOW_PASS_1V[shaderExt]).c_str(), "vertexMain", video::EVST_VS_2_0,
                                                        sPP.ppShader(WHITE_WASH_P[shaderExt]).c_str(), "pixelMain", video::EPST_PS_2_0,
                                                        depthMC, video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);

		WhiteWashTAdd = gpu->addHighLevelShaderMaterial(
                                                        sPP.ppShader(SHADOW_PASS_1V[shaderExt]).c_str(), "vertexMain", video::EVST_VS_2_0,
                                                        sPP.ppShader(WHITE_WASH_P_ADD[shaderExt]).c_str(), "pixelMain", video::EPST_PS_2_0,
                                                        depthMC, video::EMT_TRANSPARENT_ALPHA_CHANNEL);

		WhiteWashTAlpha = gpu->addHighLevelShaderMaterial(
                                                          sPP.ppShader(SHADOW_PASS_1V[shaderExt]).c_str(), "vertexMain", video::EVST_VS_2_0,
                                                          sPP.ppShader(WHITE_WASH_P[shaderExt]).c_str(), "pixelMain", video::EPST_PS_2_0,
                                                          depthMC, video::EMT_TRANSPARENT_ALPHA_CHANNEL);

		#ifdef SSWE_EDITOR
		SelectionMaterial = gpu->addHighLevelShaderMaterial(
														   sPP.ppShader(SELECTION_PASS_V[shaderExt]).c_str(), "vertexMain", video::EVST_VS_2_0,
														   sPP.ppShader(SELECTION_PASS_P[shaderExt]).c_str(), "pixelMain", video::EPST_PS_2_0,
														   new SelectionPassCB(this), video::EMT_SOLID);
		#endif

		dnlcMC = new DepthNormalLightCustomCB(this);

		const char *RenderPassesDefines[3] = {
			"_CP3D_DEPTH_PASS_",
			"_CP3D_NORMAL_PASS_",
			"_CP3D_SCATTERING_PASS_"
		};
        const char *RenderPassesColors[3] = {
            driver->getDriverType() == EDT_OPENGL ? "gl_FragData[0]" : "COLOR0",
            driver->getDriverType() == EDT_OPENGL ? "gl_FragData[1]" : "COLOR1",
            driver->getDriverType() == EDT_OPENGL ? "gl_FragData[2]" : "COLOR2"
        };
		RenderPasses.clear();
		for (u32 i=0; i < 7; i++) {
			for (u32 j=0; j < 3; j++)
				sPP.removeShaderDefine(stringc(RenderPassesDefines[j]).c_str());

			core::stringc currentRenderPassP = DEPTH_NORMAL_SCATTERING_P[shaderExt];

			if (i == 0) {
				sPP.addShaderDefine("_CP3D_DEPTH_PASS_");
				currentRenderPassP = sPP.ppShader(currentRenderPassP).replace("CP3DCOLOR0", RenderPassesColors[0]);
			} else if (i == 1) {
				sPP.addShaderDefine("_CP3D_NORMAL_PASS_");
				currentRenderPassP = sPP.ppShader(currentRenderPassP).replace("CP3DCOLOR1", RenderPassesColors[0]);
			} else if (i == 2) {
				sPP.addShaderDefine("_CP3D_SCATTERING_PASS_");
				currentRenderPassP = sPP.ppShader(currentRenderPassP).replace("CP3DCOLOR2", RenderPassesColors[0]);
			}else if (i == 3) {
				sPP.addShaderDefine("_CP3D_DEPTH_PASS_");
				sPP.addShaderDefine("_CP3D_NORMAL_PASS_");
				currentRenderPassP = sPP.ppShader(currentRenderPassP).replace("CP3DCOLOR0", RenderPassesColors[0]);
				currentRenderPassP = sPP.ppShader(currentRenderPassP).replace("CP3DCOLOR1", RenderPassesColors[1]);
			} else if (i == 4) {
				sPP.addShaderDefine("_CP3D_DEPTH_PASS_");
				sPP.addShaderDefine("_CP3D_SCATTERING_PASS_");
				currentRenderPassP = sPP.ppShader(currentRenderPassP).replace("CP3DCOLOR0", RenderPassesColors[0]);
				currentRenderPassP = sPP.ppShader(currentRenderPassP).replace("CP3DCOLOR2", RenderPassesColors[1]);
			} else if (i == 5) {
				sPP.addShaderDefine("_CP3D_NORMAL_PASS_");
				sPP.addShaderDefine("_CP3D_SCATTERING_PASS_");
				currentRenderPassP = sPP.ppShader(currentRenderPassP).replace("CP3DCOLOR1", RenderPassesColors[0]);
				currentRenderPassP = sPP.ppShader(currentRenderPassP).replace("CP3DCOLOR2", RenderPassesColors[1]);
			} else if (i == 6) {
				sPP.addShaderDefine("_CP3D_DEPTH_PASS_");
				sPP.addShaderDefine("_CP3D_NORMAL_PASS_");
				sPP.addShaderDefine("_CP3D_SCATTERING_PASS_");
				currentRenderPassP = sPP.ppShader(currentRenderPassP).replace("CP3DCOLOR0", RenderPassesColors[0]);
				currentRenderPassP = sPP.ppShader(currentRenderPassP).replace("CP3DCOLOR1", RenderPassesColors[1]);
				currentRenderPassP = sPP.ppShader(currentRenderPassP).replace("CP3DCOLOR2", RenderPassesColors[2]);
			}
            
			irr::s32 currentPass = gpu->addHighLevelShaderMaterial(sPP.ppShader(DEPTH_NORMAL_SCATTERING_V[shaderExt]).c_str(), "vertexMain", video::EVST_VS_3_0,
																   currentRenderPassP.c_str(), "pixelMain", video::EPST_PS_3_0,
																   dnlcMC, video::EMT_SOLID);
			RenderPasses.push_back(currentPass);
		}

		if(useRoundSpotLights)
			sPP.addShaderDefine("ROUND_SPOTLIGHTS");

		if(useVSMShadows)
			sPP.addShaderDefine("VSM");

		const u32 sampleCounts[EFT_COUNT] = {1, 4, 8, 12, 16};

		const E_VERTEX_SHADER_TYPE vertexProfile =
        driver->queryFeature(video::EVDF_VERTEX_SHADER_3_0) ? EVST_VS_3_0 : EVST_VS_2_0;

		const E_PIXEL_SHADER_TYPE pixelProfile =
        driver->queryFeature(video::EVDF_PIXEL_SHADER_3_0) ? EPST_PS_3_0 : EPST_PS_2_0;

		for(u32 i = 0;i < EFT_COUNT;i++)
		{
			sPP.addShaderDefine("SAMPLE_AMOUNT", core::stringc(sampleCounts[i]));
			Shadow[i] = gpu->addHighLevelShaderMaterial(sPP.ppShader(SHADOW_PASS_2V[shaderExt]).c_str(), "vertexMain", vertexProfile,
                                                        sPP.ppShader(SHADOW_PASS_2P[shaderExt]).c_str(), "pixelMain", pixelProfile,
														shadowMC, video::EMT_SOLID);
		}

		sPP.addShaderDefine("_CP3D_COMPUTE_FOG_");
		for(u32 i = 0;i < EFT_COUNT;i++)
		{
			sPP.addShaderDefine("SAMPLE_AMOUNT", core::stringc(sampleCounts[i]));
			ShadowFog[i] = gpu->addHighLevelShaderMaterial(sPP.ppShader(SHADOW_PASS_2V[shaderExt]).c_str(), "vertexMain", vertexProfile,
														   sPP.ppShader(SHADOW_PASS_2P[shaderExt]).c_str(), "pixelMain", pixelProfile,
													 	   shadowMC, video::EMT_SOLID);
		}

		// Set resolution preprocessor defines.
		sPP.addShaderDefine("SCREENX", core::stringc(ScreenRTTSize.Width));
		sPP.addShaderDefine("SCREENY", core::stringc(ScreenRTTSize.Height));

		// Create screen quad shader callback.
		ScreenQuadCB* SQCB = new ScreenQuadCB(this, true);

		// Light modulate.
		LightModulate = gpu->addHighLevelShaderMaterial(sPP.ppShader(SCREEN_QUAD_V[shaderExt]).c_str(), "vertexMain", vertexProfile,
                                                        sPP.ppShader(LIGHT_MODULATE_P[shaderExt]).c_str(), "pixelMain", pixelProfile, SQCB);

		// Simple present.
		Simple = gpu->addHighLevelShaderMaterial(sPP.ppShader(SCREEN_QUAD_V[shaderExt]).c_str(), "vertexMain", vertexProfile,
                                                 sPP.ppShader(SIMPLE_P[shaderExt]).c_str(), "pixelMain", pixelProfile, SQCB,
                                                 video::EMT_TRANSPARENT_ADD_COLOR);

		// VSM blur.
		VSMBlurH = gpu->addHighLevelShaderMaterial(sPP.ppShader(SCREEN_QUAD_V[shaderExt]).c_str(), "vertexMain", vertexProfile,
                                                   sPP.ppShader(VSM_BLUR_P[shaderExt]).c_str(), "pixelMain", pixelProfile, SQCB);

		sPP.addShaderDefine("VERTICAL_VSM_BLUR");

		VSMBlurV = gpu->addHighLevelShaderMaterial(sPP.ppShader(SCREEN_QUAD_V[shaderExt]).c_str(), "vertexMain", vertexProfile,
                                                   sPP.ppShader(VSM_BLUR_P[shaderExt]).c_str(), "pixelMain", pixelProfile, SQCB);

		DepthOfField = gpu->addHighLevelShaderMaterial(sPP.ppShader(SCREEN_QUAD_V[shaderExt]).c_str(), "vertexMain", vertexProfile,
													   sPP.ppShader(DEPTH_OF_FIELD_P[shaderExt]).c_str(), "pixelMain", pixelProfile, SQCB);

		if (driver->getDriverType() == EDT_OPENGL)
			NormalPass = EMT_SOLID;
		else
			NormalPass = gpu->addHighLevelShaderMaterial(sPP.ppShader(NORMAL_PASS_V[shaderExt]).c_str(), "vertexMain", vertexProfile,
														 sPP.ppShader(NORMAL_PASS_P[shaderExt]).c_str(), "pixelMain", pixelProfile, normalMC);

		// Drop the screen quad callback.
		SQCB->drop();
	}
	else
	{
		Depth = EMT_SOLID;
		DepthT = EMT_SOLID;
		WhiteWash = EMT_SOLID;
		WhiteWashTRef = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
		WhiteWashTAdd = EMT_TRANSPARENT_ADD_COLOR;
		WhiteWashTAlpha = EMT_TRANSPARENT_ALPHA_CHANNEL;
		Simple = EMT_SOLID;

		for(u32 i = 0;i < EFT_COUNT;++i)
			Shadow[i] = EMT_SOLID;

		device->getLogger()->log("XEffects: Shader effects not supported on this system.");
		shadowsUnsupported = true;
	}

    //MOTION BLUR
	motionBlur = new IPostProcessMotionBlur(smgr->getActiveCamera(), smgr, -2);
	motionBlur->initiate(screenRTTSize.Width, screenRTTSize.Height, 0.6);
	useMotionBlur = false;
	motionBlur->getCallback()->m_time = device->getTimer()->getTime();
	motionBlur->getCallback()->m_Strength = 0.f;

    //DEPTH OF FIELD
	useDOF = false;

	RandomVecTexture = this->generateRandomVectorTexture(dimension2du(256, 256), "RandomVecTexture");
}

EffectHandler::~EffectHandler()
{
	if(ScreenRTT)
		driver->removeTexture(ScreenRTT);

	if(ScreenQuad.rt[0])
		driver->removeTexture(ScreenQuad.rt[0]);

	if(ScreenQuad.rt[1])
		driver->removeTexture(ScreenQuad.rt[1]);

	if(DepthRTT.RenderTexture)
		driver->removeTexture(DepthRTT.RenderTexture);
}

void EffectHandler::setScreenRenderTargetResolution(const irr::core::dimension2du& resolution)
{
    //driver->setRenderTarget(0);

	bool tempTexFlagMipMaps = driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
	bool tempTexFlag32 = driver->getTextureCreationFlag(ETCF_ALWAYS_32_BIT);

	//driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);
	//driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, false);

	if(ScreenRTT)
		driver->removeTexture(ScreenRTT);

    ScreenRTT = driver->addRenderTargetTexture(resolution, "ScreenRTT");

	if(ScreenQuad.rt[0])
		driver->removeTexture(ScreenQuad.rt[0]);

	ScreenQuad.rt[0] = driver->addRenderTargetTexture(resolution, "ColorMapSampler");

	if(ScreenQuad.rt[1])
		driver->removeTexture(ScreenQuad.rt[1]);

	ScreenQuad.rt[1] = driver->addRenderTargetTexture(resolution, "ScreenMapSampler");

	if(DepthRTT != 0)
	{
		driver->removeTexture(DepthRTT.RenderTexture);
		DepthRTT = driver->addRenderTargetTexture(resolution, "depthRTT", use32BitDepth ? ECF_G32R32F : ECF_G16R16F);
	}
    
    if (DOFMapSampler) {
		driver->removeTexture(DOFMapSampler);
        DOFMapSampler = driver->addRenderTargetTexture(resolution, "DOFMapSampler");
	}

	if (LightScatteringRTT.RenderTexture) {
		driver->removeTexture(LightScatteringRTT.RenderTexture);
		LightScatteringRTT = driver->addRenderTargetTexture(resolution, "LightScatteringRTT");
	}

	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, tempTexFlagMipMaps);
	driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, tempTexFlag32);

	ScreenRTTSize = resolution;
}

void EffectHandler::enableDepthPass(bool enableDepthPass, bool SSAODepthPass)
{
	DepthPass = enableDepthPass;
}

void EffectHandler::addPostProcessingEffect(irr::s32 MaterialType, IPostProcessingRenderCallback* callback)
{
	SPostProcessingPair pPair(MaterialType, 0);
	pPair.renderCallback = callback;
	PostProcessingRoutines.push_back(pPair);
}

void EffectHandler::addShadowToNode(irr::scene::ISceneNode *node, E_FILTER_TYPE filterType, E_SHADOW_MODE shadowMode)
{
	for (u32 i=0; i < ShadowNodeArray.size(); i++) {
		if (ShadowNodeArray[i].node == node) {
			ShadowNodeArray[i].filterType = filterType;
			ShadowNodeArray[i].shadowMode = shadowMode;
			if (ShadowNodeArray[i].node->getType() == ESNT_SKY_BOX) {
				SShadowNode n = ShadowNodeArray[i];
				ShadowNodeArray.erase(i);
				ShadowNodeArray.push_front(n);
			}
			return;
		}
	}

	SShadowNode snode = {node, shadowMode, filterType, false, false, false};
	if (node->getType() == ESNT_SKY_BOX)
		ShadowNodeArray.push_front(snode);
	else
		ShadowNodeArray.push_back(snode);
}


void EffectHandler::addNodeToDepthPass(irr::scene::ISceneNode *node)
{
	for (u32 i=0; i < ShadowNodeArray.size(); i++) {
		if (ShadowNodeArray[i].node == node) {
			ShadowNodeArray[i].renderDepth = true;
			ShadowNodeArray[i].renderNormal = true;
			break;
		}
	}
}

void EffectHandler::removeNodeFromDepthPass(irr::scene::ISceneNode *node)
{
	for (u32 i=0; i < ShadowNodeArray.size(); i++) {
		if (ShadowNodeArray[i].node == node) {
			ShadowNodeArray[i].renderDepth = false;
			ShadowNodeArray[i].renderNormal = false;
			break;
		}
	}
}

void EffectHandler::updateEffect() {
	update();
}

void EffectHandler::update(bool updateOcclusionQueries, irr::video::ITexture* outputTarget)
{
	if(shadowsUnsupported || smgr->getActiveCamera() == 0)
		return;

	irr::core::vector3df optCameraPos = smgr->getActiveCamera()->getPosition();
	f32 nearValue = smgr->getActiveCamera()->getNearValue();
	smgr->getActiveCamera()->setNearValue(0.1f);

	if(!ShadowNodeArray.empty() && !LightList.empty())
	{
		depthMC->MultiRenderTarget = false;

		driver->setRenderTarget(ScreenQuad.rt[0], true, true, AmbientColour);

		ICameraSceneNode* activeCam = smgr->getActiveCamera();
		activeCam->OnAnimate(device->getTimer()->getTime());
		activeCam->OnRegisterSceneNode();
		activeCam->render();

		const u32 ShadowNodeArraySize = ShadowNodeArray.size();
		const u32 LightListSize = LightList.size();
		for(u32 l = 0;l < LightListSize;++l) {
			for (u32 ll=0; ll < LightList[l].getShadowLightCount(); ll++) {

				if (LightList[l].getShadowLight(ll).getFarValue() == 0
					|| LightList[l].getShadowLight(ll).getPosition().getDistanceFrom(optCameraPos) > LightList[l].getShadowLight(ll).getFarValue())
					continue;

				currentShadowMapTexture = getShadowMapTexture(LightList[l].getShadowLight(ll).getShadowMapResolution(), false, l, ll);

				bool mustRecalculate = LightList[l].getShadowLight(ll).mustRecalculate() || LightList[l].getShadowLight(ll).isAutoRecalculate();
				E_SHADOW_LIGHT_TYPE lightType = LightList[l].getLightType();
				bool computeLight=mustRecalculate && LightList[l].getShadowLight(ll).getShadowMapResolution() > 1;

				if (computeLight) {
					depthMC->FarLink = LightList[l].getShadowLight(ll).getFarValue();

					driver->setTransform(ETS_VIEW, LightList[l].getShadowLight(ll).getViewMatrix());
					driver->setTransform(ETS_PROJECTION, LightList[l].getShadowLight(ll).getProjectionMatrix());

					//currentShadowMapTexture = getShadowMapTexture(LightList[l].getShadowMapResolution(), false, l);
					driver->setRenderTarget(currentShadowMapTexture, true, true, SColor(0xffffffff));

					for(u32 i = 0;i < ShadowNodeArraySize;++i) {
						if(ShadowNodeArray[i].shadowMode == ESM_RECEIVE || ShadowNodeArray[i].shadowMode == ESM_EXCLUDE)
							continue;

						if (!ShadowNodeArray[i].node->isVisible() || ShadowNodeArray[i].shadowMode == ESM_NO_SHADOW)
							continue;

						const u32 CurrentMaterialCount = ShadowNodeArray[i].node->getMaterialCount();
						core::array<irr::s32> BufferMaterialList(CurrentMaterialCount);
						BufferMaterialList.set_used(0);

						for(u32 m = 0;m < CurrentMaterialCount;++m) {
							BufferMaterialList.push_back(ShadowNodeArray[i].node->getMaterial(m).MaterialType);
							ShadowNodeArray[i].node->getMaterial(m).MaterialType =
								(E_MATERIAL_TYPE) (BufferMaterialList[m] == video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF ? DepthT : Depth);
						}

						if (ShadowNodeArray[i].node->getParent() == activeCam)
							activeCam->OnAnimate(device->getTimer()->getTime());

						ShadowNodeArray[i].node->OnAnimate(device->getTimer()->getTime());
						ShadowNodeArray[i].node->render();

						const u32 BufferMaterialListSize = BufferMaterialList.size();
						for(u32 m = 0;m < BufferMaterialListSize;++m)
							ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[m];
					}
				}

				// Blur the shadow map texture if we're using VSM filtering.
				if (LightList[l].getShadowLight(ll).mustRecalculate() || LightList[l].getShadowLight(ll).isAutoRecalculate()) {
					if(useVSM) {
						ITexture *currentSecondaryShadowMap = getShadowMapTexture(LightList[l].getShadowLight(ll).getShadowMapResolution(), true, l);

						driver->setRenderTarget(currentSecondaryShadowMap, true, false, SColor(0xffffffff));
						ScreenQuad.getMaterial().setTexture(0, currentShadowMapTexture);
						ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)VSMBlurH;

						ScreenQuad.render(driver);

						driver->setRenderTarget(currentShadowMapTexture, true, true, SColor(0xffffffff));
						ScreenQuad.getMaterial().setTexture(0, currentSecondaryShadowMap);
						ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)VSMBlurV;

						ScreenQuad.render(driver);
					}
				}

				LightList[l].getShadowLight(ll).setRecalculate(false);

				driver->setRenderTarget(ScreenQuad.rt[1], true, true, SColor(0xffffffff));

				driver->setTransform(ETS_VIEW, activeCam->getViewMatrix());
				driver->setTransform(ETS_PROJECTION, activeCam->getProjectionMatrix());

				shadowMC->LightColour = LightList[l].getShadowLight(ll).getLightColor();
				shadowMC->LightLink = LightList[l].getShadowLight(ll).getPosition();
				shadowMC->FarLink = LightList[l].getShadowLight(ll).getFarValue();
				shadowMC->ViewLink = LightList[l].getShadowLight(ll).getViewMatrix();
				shadowMC->ProjLink = LightList[l].getShadowLight(ll).getProjectionMatrix();
				shadowMC->MapRes = (f32)LightList[l].getShadowLight(ll).getShadowMapResolution();

				// Render all the Receive nodes
				for(u32 i = 0;i < ShadowNodeArraySize;++i)
				{
					if(ShadowNodeArray[i].shadowMode == ESM_CAST || ShadowNodeArray[i].shadowMode == ESM_EXCLUDE)
						continue;

					if (!ShadowNodeArray[i].node->isVisible() || ShadowNodeArray[i].shadowMode == ESM_NO_SHADOW) {
						continue;
					}

					const u32 CurrentMaterialCount = ShadowNodeArray[i].node->getMaterialCount();
					core::array<irr::s32> BufferMaterialList(CurrentMaterialCount);
					core::array<irr::video::ITexture*> BufferTextureList(CurrentMaterialCount);
					core::array<irr::video::ITexture*> BufferTextureList2(CurrentMaterialCount);

					for(u32 m = 0;m < CurrentMaterialCount;++m)
					{
						BufferMaterialList.push_back(ShadowNodeArray[i].node->getMaterial(m).MaterialType);
						BufferTextureList.push_back(ShadowNodeArray[i].node->getMaterial(m).getTexture(0));

						if (LightList[l].getShadowLight(ll).isComputeVolumetricLight()) {
							BufferTextureList2.push_back(ShadowNodeArray[i].node->getMaterial(m).getTexture(1));
							ShadowNodeArray[i].node->getMaterial(m).setTexture(0, RandomVecTexture);
							ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)ShadowFog[ShadowNodeArray[i].filterType];
						} else {
							ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)Shadow[ShadowNodeArray[i].filterType];
						}
						ShadowNodeArray[i].node->getMaterial(m).setTexture(0, currentShadowMapTexture);
					}

					ShadowNodeArray[i].node->OnAnimate(device->getTimer()->getTime());
					ShadowNodeArray[i].node->render();

					for(u32 m = 0;m < CurrentMaterialCount;++m)
					{
						ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[m];
						ShadowNodeArray[i].node->getMaterial(m).setTexture(0, BufferTextureList[m]);
						if (LightList[l].getShadowLight(ll).isComputeVolumetricLight())
							ShadowNodeArray[i].node->getMaterial(m).setTexture(1, BufferTextureList2[m]);
					}
				}

				driver->setRenderTarget(ScreenQuad.rt[0], false, false, SColor(0x0));
				ScreenQuad.getMaterial().setTexture(0, ScreenQuad.rt[1]);
				ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)Simple;

				ScreenQuad.render(driver);
			}

			LightList[l].getNextPass();
		}

		// Render all the excluded and casting-only nodes.
		for(u32 i = 0;i < ShadowNodeArraySize;++i)
		{
			if(ShadowNodeArray[i].shadowMode != ESM_CAST && ShadowNodeArray[i].shadowMode != ESM_EXCLUDE)
					continue;

			if (!ShadowNodeArray[i].node->isVisible()) {
				continue;
			}

			const u32 CurrentMaterialCount = ShadowNodeArray[i].node->getMaterialCount();
			core::array<irr::s32> BufferMaterialList(CurrentMaterialCount);
			BufferMaterialList.set_used(0);

			for(u32 m = 0;m < CurrentMaterialCount;++m)
			{
				BufferMaterialList.push_back(ShadowNodeArray[i].node->getMaterial(m).MaterialType);

				switch(BufferMaterialList[m])
				{
					case EMT_TRANSPARENT_ALPHA_CHANNEL_REF:
						ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)WhiteWashTRef;
						break;
					case EMT_TRANSPARENT_ADD_COLOR:
						ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)WhiteWashTAdd;
						break;
					case EMT_TRANSPARENT_ALPHA_CHANNEL:
						ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)WhiteWashTAlpha;
						break;
					default:
						ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)WhiteWash;
						break;
				}
			}

			ShadowNodeArray[i].node->OnAnimate(device->getTimer()->getTime());
			ShadowNodeArray[i].node->render();

			for(u32 m = 0;m < CurrentMaterialCount;++m)
				ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[m];
		}

		/*driver->setRenderTarget(ScreenQuad.rt[0], false, false, SColor(0x0));
		ScreenQuad.getMaterial().setTexture(0, ScreenQuad.rt[1]);
		ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)Simple;
			
		ScreenQuad.render(driver);*/
	}
	else
	{
		driver->setRenderTarget(ScreenQuad.rt[0], true, true, SColor(0xffffffff));
	}

	smgr->getActiveCamera()->setNearValue(nearValue);

	//driver->setRenderTarget(ScreenQuad.rt[1], true, true, ClearColour);
	//smgr->drawAll();
    if (updateOcclusionQueries) {
        driver->runAllOcclusionQueries(true);
        driver->updateAllOcclusionQueries(true);
	}

	if (useMotionBlur) {
		motionBlur->getCallback()->m_Strength = 0.2f;

		motionBlur->render();
		driver->setRenderTarget(ScreenQuad.rt[1], true, true, ClearColour);
		motionBlur->renderFinal();

	} else {
		driver->setRenderTarget(ScreenQuad.rt[1], true, true, ClearColour);
		smgr->drawAll();
	}


	if (useReflectionPass) {
		const f32 CLIP_PLANE_OFFSET_Y = 5.0f;
		irr::scene::ICameraSceneNode *currentCamera = smgr->getActiveCamera();

		driver->setRenderTarget(ReflectionRTT, true, true, ClearColour);

		cameraForPasses->setFarValue(currentCamera->getFarValue());
		cameraForPasses->setFOV(currentCamera->getFOV());

		//Set position
		core::vector3df position = currentCamera->getAbsolutePosition();
		position.Y = -position.Y + 2 * 1.0; //position of the plan
		cameraForPasses->setPosition(position);
		
		//Set target
		core::vector3df target = currentCamera->getTarget();
		target.Y = -target.Y + 2 * 1.0;
		cameraForPasses->setTarget(target);

		smgr->setActiveCamera(cameraForPasses);

		//reflection clipping plane
		core::plane3d<f32> reflectionClipPlane(0, 0 - CLIP_PLANE_OFFSET_Y, 0, 0, 1, 0);
		driver->setClipPlane(0, reflectionClipPlane, true);

		smgr->drawAll();

		driver->enableClipPlane(0, false);

		smgr->setActiveCamera(currentCamera);
		currentCamera->OnAnimate(device->getTimer()->getTime());
		currentCamera->OnRegisterSceneNode();
		currentCamera->render();

		driver->setRenderTarget(0, false, false);
	}

	// Perform reflection pass to look back of the current render (will be used for reflection)
	/*if (true) {
		irr::scene::ICameraSceneNode *currentCamera = smgr->getActiveCamera();

		driver->setRenderTarget(backRenderRTT, true, true, ClearColour);

		smgr->setActiveCamera(cameraForPasses);
		cameraForPasses->setPosition(currentCamera->getPosition());

		line3d<f32> ray;
		ray.start = currentCamera->getPosition();
		ray.end = ray.start + (currentCamera->getTarget() - ray.start).normalize() * 1000.0f;
		vector3df toTarget = ray.getVector().invert();
		cameraForPasses->setTarget(toTarget);

		smgr->drawAll();

		smgr->setActiveCamera(currentCamera);
		currentCamera->OnAnimate(device->getTimer()->getTime());
		currentCamera->OnRegisterSceneNode();
		currentCamera->render();

		driver->setRenderTarget(0, false, false);
	}*/

	// Perform depth pass after rendering, to ensure animations stay up to date.
	if (DepthPass || useLightScattering || renderNormalPass) {

		/// Select render targets and appropriate shader program
		irr::s32 matType = -1;
		CP3DRenderTargets.clear();
		if (DepthPass) {
			CP3DRenderTargets.push_back(DepthRTT);
			matType = RenderPasses[0];
			if (renderNormalPass) {
				matType = RenderPasses[3];
				CP3DRenderTargets.push_back(NormalRenderRTT);
			}
			if (useLightScattering) {
				if (renderNormalPass)
					matType = RenderPasses[6];
				else
					matType = RenderPasses[4];
				CP3DRenderTargets.push_back(LightScatteringRTT);
			}
		} else {
			if (renderNormalPass) {
				matType = RenderPasses[1];
				CP3DRenderTargets.push_back(NormalRenderRTT);
			}
			if (useLightScattering) {
				if (renderNormalPass)
					matType = RenderPasses[5];
				else
					matType = RenderPasses[2];
				CP3DRenderTargets.push_back(LightScatteringRTT);
			}
		}

		/// Configure Callback
		dnlcMC->RenderNormalPass = renderNormalPass;
		dnlcMC->RenderDepthPass = DepthPass;
		dnlcMC->RenderScatteringPass = useLightScattering;
		if (DepthPass || renderNormalPass)
			dnlcMC->FarLink = smgr->getActiveCamera()->getFarValue();

		driver->setRenderTarget(CP3DRenderTargets, true, true, SColor(0xffffffff));

		for(u32 i = 0;i < ShadowNodeArray.size();++i)
		{
			if (!ShadowNodeArray[i].node->isVisible())
				continue;

			core::array<irr::s32> BufferMaterialList(ShadowNodeArray[i].node->getMaterialCount());
			BufferMaterialList.set_used(0);

			for(u32 g = 0;g < ShadowNodeArray[i].node->getMaterialCount();++g)
				BufferMaterialList.push_back(ShadowNodeArray[i].node->getMaterial(g).MaterialType);

			if (ShadowNodeArray[i].renderScattering) {
				if (ShadowNodeArray[i].node->getType() == ESNT_BILLBOARD)
					dnlcMC->RenderNormal = 1;
				else
					dnlcMC->RenderNormal = 0;
			}

			ShadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)matType);

			ShadowNodeArray[i].node->OnAnimate(device->getTimer()->getTime());
			ShadowNodeArray[i].node->render();

			for(u32 g = 0;g < ShadowNodeArray[i].node->getMaterialCount();++g)
				ShadowNodeArray[i].node->getMaterial(g).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[g];
		}

		//driver->setRenderTarget(0, false, false);
	}

	const u32 PostProcessingRoutinesSize = PostProcessingRoutines.size();
	bool rtTest = (useDOF);

	driver->setRenderTarget(PostProcessingRoutinesSize || rtTest || useHDR
		? ScreenRTT : outputTarget, true, true, SColor(0x0));

	ScreenQuad.getMaterial().setTexture(0, ScreenQuad.rt[1]);
	ScreenQuad.getMaterial().setTexture(1, ScreenQuad.rt[0]);

	ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)LightModulate;
	ScreenQuad.render(driver);

	ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)Simple;

	//RENDER OTHER POST PROCESSES
	bool Alter = false;
	ScreenQuad.getMaterial().setTexture(1, ScreenRTT);
	ScreenQuad.getMaterial().setTexture(2, DepthRTT.RenderTexture);
	if(PostProcessingRoutinesSize)
	{

		for(u32 i = 0;i < PostProcessingRoutinesSize;++i)
		{
			ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)PostProcessingRoutines[i].materialType;

			if(PostProcessingRoutines[i].renderCallback) PostProcessingRoutines[i].renderCallback->OnPreRender(this);

			Alter = !Alter;
			ScreenQuad.getMaterial().setTexture(0, i == 0 ? ScreenRTT : ScreenQuad.rt[int(!Alter)]);
			driver->setRenderTarget(i >= PostProcessingRoutinesSize - 1 && !rtTest && !useHDR ? outputTarget : ScreenQuad.rt[int(Alter)], true, true, ClearColour);

			ScreenQuad.render(driver);

			if(PostProcessingRoutines[i].renderCallback) PostProcessingRoutines[i].renderCallback->OnPostRender(this);

			ScreenQuad.getMaterial().setTexture(1, ScreenRTT);
			ScreenQuad.getMaterial().setTexture(2, DepthRTT.RenderTexture);
		}
	}

	//---------------------------------------------------------------------------------------------------------------
	//RENDER DEPTH OF FIELD
	if (useDOF) {
        /// Configure screen quad
		driver->setRenderTarget(DOFMapSampler, true, true, ClearColour);
        driver->draw2DImage(PostProcessingRoutinesSize == 0 ? ScreenRTT : ScreenQuad.rt[int(Alter)], core::rect<s32>(0 ,0 , ScreenRTTSize.Width, ScreenRTTSize.Height),
							core::rect<s32>(0,0,ScreenRTTSize.Width,ScreenRTTSize.Height), 0);
		ScreenQuad.getMaterial().setTexture(1, DOFMapSampler);

		/// Horizontal Blur
		ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)VSMBlurH;
		Alter = !Alter;
		ScreenQuad.getMaterial().setTexture(0, PostProcessingRoutinesSize == 0 ? ScreenRTT : ScreenQuad.rt[int(!Alter)]);
		driver->setRenderTarget(ScreenQuad.rt[int(Alter)], true, true, ClearColour);
		ScreenQuad.render(driver);

		/// Vertical Blur
		ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)VSMBlurV;
		Alter = !Alter;
		ScreenQuad.getMaterial().setTexture(0, ScreenQuad.rt[int(!Alter)]);
		driver->setRenderTarget(ScreenQuad.rt[int(Alter)], true, true, ClearColour);
		ScreenQuad.render(driver);

		/// Depth Of Field
		ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)DepthOfField;
		Alter = !Alter;
		ScreenQuad.getMaterial().setTexture(0, ScreenQuad.rt[int(!Alter)]);
		driver->setRenderTarget(useHDR ? ScreenQuad.rt[int(Alter)] : outputTarget, true, true, ClearColour);
		ScreenQuad.render(driver);
	}

	//---------------------------------------------------------------------------------------------------------------

	//HDR PIPELINE
	if (useHDR) {

		ppm->Render(PostProcessingRoutinesSize == 0 && !rtTest ? ScreenRTT : ScreenQuad.rt[int(Alter)], HDRProcessedRT);

		driver->setRenderTarget(outputTarget, true, true, ClearColour);

		ScreenQuad.getMaterial().setTexture(0, HDRProcessedRT);
		ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)Simple;
		ScreenQuad.render(driver);

		//driver->setRenderTarget(outputTarget, true, true, ClearColour);
		//ppm->Render(PostProcessingRoutinesSize == 0 && !rtTest ? ScreenRTT : ScreenQuad.rt[int(Alter)]);

	}

}

irr::video::ITexture* EffectHandler::getShadowMapTexture(const irr::u32 resolution, const bool secondary, const irr::u32 id, const irr::u32 id2)
{
	// Using Irrlicht cache now.
	core::stringc shadowMapName = core::stringc("SSWE_SM_") + core::stringc(resolution);
	shadowMapName += "_";
	shadowMapName += id;
	shadowMapName += "_";
	shadowMapName += id2;
	shadowMapName += "_";

	if(secondary)
		shadowMapName += "_2";

	ITexture* shadowMapTexture = driver->getTexture(shadowMapName);

	if(shadowMapTexture == 0)
	{
		//device->getLogger()->log("XEffects: Please ignore previous warning, it is harmless.");

		shadowMapTexture = driver->addRenderTargetTexture(dimension2du(resolution, resolution),
                                                          shadowMapName, use32BitDepth ? ECF_G32R32F : ECF_G16R16F);
	}

	return shadowMapTexture;
}

irr::video::ITexture* EffectHandler::generateRandomVectorTexture(const irr::core::dimension2du& dimensions,
                                                                 const irr::core::stringc& name)
{
	IImage* tmpImage = driver->createImage(irr::video::ECF_A8R8G8B8, dimensions);

	srand(device->getTimer()->getRealTime());

	for(u32 x = 0;x < dimensions.Width;++x)
	{
		for(u32 y = 0;y < dimensions.Height;++y)
		{
			vector3df randVec;

			// Reject vectors outside the unit sphere to get a uniform distribution.
			do {randVec = vector3df((f32)rand() / (f32)RAND_MAX, (f32)rand() / (f32)RAND_MAX, (f32)rand() / (f32)RAND_MAX);}
			while(randVec.getLengthSQ() > 1.0f);

			const SColorf randCol(randVec.X, randVec.Y, randVec.Z);
			tmpImage->setPixel(x, y, randCol.toSColor());
		}
	}

	ITexture* randTexture = driver->addTexture(name, tmpImage);

	tmpImage->drop();

	return randTexture;
}

EffectHandler::SPostProcessingPair EffectHandler::obtainScreenQuadMaterialFromFile(const irr::core::stringc& filename,
                                                                                   irr::video::E_MATERIAL_TYPE baseMaterial)
{
	CShaderPreprocessor sPP(driver);

	sPP.addShaderDefine("SCREENX", core::stringc(ScreenRTTSize.Width));
	sPP.addShaderDefine("SCREENY", core::stringc(ScreenRTTSize.Height));

	video::E_VERTEX_SHADER_TYPE VertexLevel = driver->queryFeature(video::EVDF_VERTEX_SHADER_3_0) ? EVST_VS_3_0 : EVST_VS_2_0;
	video::E_PIXEL_SHADER_TYPE PixelLevel = driver->queryFeature(video::EVDF_PIXEL_SHADER_3_0) ? EPST_PS_3_0 : EPST_PS_2_0;

	E_SHADER_EXTENSION shaderExt = (driver->getDriverType() == EDT_DIRECT3D9) ? ESE_HLSL : ESE_GLSL;

	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

	const stringc shaderString = sPP.ppShaderFF(filename.c_str());

	ScreenQuadCB* SQCB = new ScreenQuadCB(this, true);

	s32 PostMat = gpu->addHighLevelShaderMaterial(
                                                  sPP.ppShader(SCREEN_QUAD_V[shaderExt]).c_str(), "vertexMain", VertexLevel,
                                                  shaderString.c_str(), "pixelMain", PixelLevel,
                                                  SQCB, baseMaterial);

	SPostProcessingPair pPair(PostMat, SQCB);

	SQCB->drop();

	return pPair;
}

EffectHandler::SPostProcessingPair EffectHandler::obtainScreenQuadMaterialFromStrings(const irr::core::stringc& pixelShader,
																					  irr::video::E_MATERIAL_TYPE baseMaterial)
{
	CShaderPreprocessor sPP(driver);

	sPP.addShaderDefine("SCREENX", core::stringc(ScreenRTTSize.Width));
	sPP.addShaderDefine("SCREENY", core::stringc(ScreenRTTSize.Height));

	video::E_VERTEX_SHADER_TYPE VertexLevel = driver->queryFeature(video::EVDF_VERTEX_SHADER_3_0) ? EVST_VS_3_0 : EVST_VS_2_0;
	video::E_PIXEL_SHADER_TYPE PixelLevel = driver->queryFeature(video::EVDF_PIXEL_SHADER_3_0) ? EPST_PS_3_0 : EPST_PS_2_0;

	E_SHADER_EXTENSION shaderExt = (driver->getDriverType() == EDT_DIRECT3D9) ? ESE_HLSL : ESE_GLSL;

	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

	const stringc& shaderProgram = pixelShader;
	const stringc shaderString = sPP.ppShaderFFS(shaderProgram.c_str());

	ScreenQuadCB* SQCB = new ScreenQuadCB(this, true);

	s32 PostMat = gpu->addHighLevelShaderMaterial(
                                                  sPP.ppShader(SCREEN_QUAD_V[shaderExt]).c_str(), "vertexMain", VertexLevel,
                                                  shaderString.c_str(), "pixelMain", PixelLevel,
                                                  SQCB, baseMaterial);

	SPostProcessingPair pPair(PostMat, SQCB);

	SQCB->drop();

	return pPair;
}

void EffectHandler::setPostProcessingEffectConstant(const irr::s32 materialType, const irr::core::stringc& name,
                                                    f32* data, const irr::u32 count)
{
	SPostProcessingPair tempPair(materialType, 0);
	s32 matIndex = PostProcessingRoutines.binary_search(tempPair);

	//printf("%s    %f\n", name.c_str(), data);

	if(matIndex != -1)
		PostProcessingRoutines[matIndex].callback->uniformDescriptors[name] = ScreenQuadCB::SUniformDescriptor(data, count);
}

s32 EffectHandler::addPostProcessingEffectFromFile(const irr::core::stringc& filename,
												   std::function<void(ISSWERender *render, irr::s32 materialType)> preRenderCallback,
												   std::function<void(ISSWERender *render, irr::s32 materialType)> postRenderCallback,
												   bool pushFront)
{
	IPostProcessingRenderCallback *renderCallback = new CCustomFilterCallback(preRenderCallback, postRenderCallback);
	s32 mat = addPostProcessingEffectFromFile(filename, renderCallback, pushFront);
	((CCustomFilterCallback*)renderCallback)->MaterialType = mat;

	return mat;
}

s32 EffectHandler::addPostProcessingEffectFromFile(const irr::core::stringc& filename,
                                                   IPostProcessingRenderCallback* callback,
												   bool pushFront)
{
	SPostProcessingPair pPair = obtainScreenQuadMaterialFromFile(filename);
	pPair.renderCallback = callback;

	if (pushFront) {
		PostProcessingRoutines.push_front(pPair);
	} else {
		PostProcessingRoutines.push_front(pPair);
	}

	return pPair.materialType;
}

irr::core::array<s32> EffectHandler::reloadPostProcessingEffects(irr::core::array<irr::core::stringc> &newCodes) {
	irr::core::array<s32> materialTypes;
	materialTypes.clear();

	for (irr::u32 i=0; i < newCodes.size(); i++) {
		SPostProcessingPair pPair = obtainScreenQuadMaterialFromStrings(newCodes[i]);
		pPair.renderCallback = 0;

		if (pPair.materialType != -1) {
			PostProcessingRoutines.push_back(pPair);
		}

		materialTypes.push_back(pPair.materialType);
	}

	return materialTypes;
}

s32 EffectHandler::addPostProcessingEffectFromString(const irr::core::stringc pixelShader,
										IPostProcessingRenderCallback *callback,
										bool pushFront)
{
	SPostProcessingPair pPair = obtainScreenQuadMaterialFromStrings(pixelShader);
	pPair.renderCallback = callback;

	if (pushFront) {
		PostProcessingRoutines.push_front(pPair);
	} else {
		PostProcessingRoutines.push_front(pPair);
	}

	return pPair.materialType;
}

void EffectHandler::clearPostProcessEffectConstants(irr::s32 MaterialType) {
	SPostProcessingPair tempPair(MaterialType, 0);
	irr::s32 i = PostProcessingRoutines.binary_search(tempPair);

	if (i != -1)
	{
		PostProcessingRoutines[i].callback->cleanUniformDescriptors();
	}
}
