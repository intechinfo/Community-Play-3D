#include "stdafx.h"

#include "EffectHandler.h"
#include "EffectCB.h"
#include "EffectShaders.h"

///HDR
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
ClearColour(0x0), shadowsUnsupported(false), DepthRTT(0), DepthPass(false), depthMC(0), shadowMC(0),
AmbientColour(0x0), use32BitDepth(use32BitDepthBuffers), useVSM(useVSMShadows)
{
	bool tempTexFlagMipMaps = driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
	bool tempTexFlag32 = driver->getTextureCreationFlag(ETCF_ALWAYS_32_BIT);

	ScreenRTT = driver->addRenderTargetTexture(ScreenRTTSize, "ScreenRTT");
	ScreenQuad.rt[0] = driver->addRenderTargetTexture(ScreenRTTSize, "ColorMapSampler");
	ScreenQuad.rt[1] = driver->addRenderTargetTexture(ScreenRTTSize, "ScreenMapSampler");

	DOFMapSampler = driver->addRenderTargetTexture(ScreenRTTSize, "DOFMapSampler");

	//LIGHT SCATTERING PASS
	LightScatteringRTT = driver->addRenderTargetTexture(ScreenRTTSize, "LightScatteringRTT");
	useLightScattering = false;

	//HDR PIPELINE
	useHDR = true;

	GlobalContext::DeviceContext.SetDevice(dev);

    ppm = new PostProcessingManager();
	amp = new Amplifier();
	quad = new Graphics::CHDRScreenQuad();

	psm = new PhongShaderManager(driver, device->getFileSystem()->getWorkingDirectory());
	pp = new HDRPostProcess(ScreenRTTSize);
	ppm->AddPostProcess(pp);
	pp->GetBloomGenerator()->SetGaussianCoefficient(0.3f);

	phong.Lighting = false;
	phong.MaterialType = psm->getMaterialType();
	phong.Shininess = 1.0f;
	phong.MaterialTypeParam = 200.0f;

	HDRProcessedRT = driver->addRenderTargetTexture(ScreenRTTSize,"HDRProcessedRTT", video::ECF_A8R8G8B8);

	hdrManager = new CHDRManager(pp, &phong);

	//BACK RENDER
	backRenderRTT = driver->addRenderTargetTexture(ScreenRTTSize, "BackRenderRTT");

	//NORMAL PASS
	normalRenderRTT = driver->addRenderTargetTexture(ScreenRTTSize, "NormalPassRTT");
	renderNormalPass = false;

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
			   (driver->getDriverType() == EDT_DIRECT3D9 && driver->queryFeature(EVDF_PIXEL_SHADER_2_0))))
	{
		depthMC = new DepthShaderCB(this);
		shadowMC = new ShadowShaderCB(this);
		normalMC = new NormalShaderCB(this);

		Depth = gpu->addHighLevelShaderMaterial(
                                                sPP.ppShader(SHADOW_PASS_1V[shaderExt]).c_str(), "vertexMain", video::EVST_VS_2_0,
                                                sPP.ppShader(SHADOW_PASS_1P[shaderExt]).c_str(), "pixelMain", video::EPST_PS_2_0,
                                                depthMC, video::EMT_SOLID);

		DepthT = gpu->addHighLevelShaderMaterial(
                                                 sPP.ppShader(SHADOW_PASS_1V[shaderExt]).c_str(), "vertexMain", video::EVST_VS_2_0,
                                                 sPP.ppShader(SHADOW_PASS_1PT[shaderExt]).c_str(), "pixelMain", video::EPST_PS_2_0,
                                                 depthMC, video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);

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
			Shadow[i] = gpu->addHighLevelShaderMaterial(
                                                        sPP.ppShader(SHADOW_PASS_2V[shaderExt]).c_str(), "vertexMain", vertexProfile,
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
	if (smgr->getActiveCamera())
		lastCameraRotation = smgr->getActiveCamera()->getRotation();
	lastTimeRotationDiff = device->getTimer()->getTime();

    //DEPTH OF FIELD
	useDOF = false;

	//HDR PIPELINE
	//HDRModel = this->addPostProcessingEffectFromFile("shaders/HDR/Model.fx", 0, false);
	//this->setPostProcessingRenderCallback(HDRModel, new CHDRCallback(this, HDRModel));
}

EffectHandler::~EffectHandler()
{
	if(ScreenRTT)
		driver->removeTexture(ScreenRTT);

	if(ScreenQuad.rt[0])
		driver->removeTexture(ScreenQuad.rt[0]);

	if(ScreenQuad.rt[1])
		driver->removeTexture(ScreenQuad.rt[1]);

	if(DepthRTT)
		driver->removeTexture(DepthRTT);
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
		driver->removeTexture(DepthRTT);
		DepthRTT = driver->addRenderTargetTexture(resolution, "depthRTT", use32BitDepth ? ECF_G32R32F : ECF_G16R16F);
	}
    
    if (DOFMapSampler) {
		driver->removeTexture(DOFMapSampler);
        DOFMapSampler = driver->addRenderTargetTexture(resolution, "DOFMapSampler");
	}

	if (LightScatteringRTT) {
		driver->removeTexture(LightScatteringRTT);
		LightScatteringRTT = driver->addRenderTargetTexture(resolution, "LightScatteringRTT");
	}

	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, tempTexFlagMipMaps);
	driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, tempTexFlag32);

	ScreenRTTSize = resolution;
}

void EffectHandler::enableDepthPass(bool enableDepthPass)
{
	DepthPass = enableDepthPass;

	if(DepthPass && DepthRTT == 0)
		DepthRTT = driver->addRenderTargetTexture(ScreenRTTSize, "depthRTT", use32BitDepth ? ECF_G32R32F : ECF_G16R16F);
}

void EffectHandler::addPostProcessingEffect(irr::s32 MaterialType, IPostProcessingRenderCallback* callback)
{
	SPostProcessingPair pPair(MaterialType, 0);
	pPair.renderCallback = callback;
	PostProcessingRoutines.push_back(pPair);
}

void EffectHandler::addShadowToNode(irr::scene::ISceneNode *node, E_FILTER_TYPE filterType, E_SHADOW_MODE shadowMode)
{
	bool founded = false;
	for (u32 i=0; i < ShadowNodeArray.size(); i++) {
		if (ShadowNodeArray[i].node == node) {
			founded = true;
			break;
		}
	}
	if (!founded) {
		SShadowNode snode = {node, shadowMode, filterType};
		ShadowNodeArray.push_back(snode);
	}
}


void EffectHandler::addNodeToDepthPass(irr::scene::ISceneNode *node)
{
	bool founded = false;
    irr::s32 i = 0;

    while (!founded && i < DepthPassArray.size()) {
        if (DepthPassArray[i] == node) {
            founded = true;
        }
        i++;
    }

    if (!founded) {
        DepthPassArray.push_back(node);
    } else {
        printf("Node already depth passed\n");
    }
}

void EffectHandler::removeNodeFromDepthPass(irr::scene::ISceneNode *node)
{
	s32 i = DepthPassArray.binary_search(node);

	if(i != -1)
		DepthPassArray.erase(i);
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
		driver->setRenderTarget(ScreenQuad.rt[0], true, true, AmbientColour);

		ICameraSceneNode* activeCam = smgr->getActiveCamera();
		activeCam->OnAnimate(device->getTimer()->getTime());
		//activeCam->OnRegisterSceneNode();
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

				/*if (!computeLight) {
					if (lightType == ESLT_SPOT || lightType == ESLT_DIRECTIONAL) {
						if (mustRecalculate && LightList[l].getShadowLight(ll).getShadowMapResolution() > 1)
							computeLight = true;
						else
							computeLight = false;
					} else {
						if (ll == LightList[l].getCurrentPass() && mustRecalculate && LightList[l].getShadowLight(ll).getShadowMapResolution() > 1)
							computeLight = true;
						else
							computeLight = false;
					}
				}*/

				if (computeLight) {
					depthMC->FarLink = LightList[l].getShadowLight(ll).getFarValue();

					driver->setTransform(ETS_VIEW, LightList[l].getShadowLight(ll).getViewMatrix());
					driver->setTransform(ETS_PROJECTION, LightList[l].getShadowLight(ll).getProjectionMatrix());

					//currentShadowMapTexture = getShadowMapTexture(LightList[l].getShadowMapResolution(), false, l);
					driver->setRenderTarget(currentShadowMapTexture, true, true, SColor(0xffffffff));

					for(u32 i = 0;i < ShadowNodeArraySize;++i) {
						if(ShadowNodeArray[i].shadowMode == ESM_RECEIVE || ShadowNodeArray[i].shadowMode == ESM_EXCLUDE)
							continue;

						if (!ShadowNodeArray[i].node->isVisible())
							continue;

						const u32 CurrentMaterialCount = ShadowNodeArray[i].node->getMaterialCount();
						core::array<irr::s32> BufferMaterialList(CurrentMaterialCount);
						BufferMaterialList.set_used(0);

						for(u32 m = 0;m < CurrentMaterialCount;++m) {
							BufferMaterialList.push_back(ShadowNodeArray[i].node->getMaterial(m).MaterialType);
							ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)
								(BufferMaterialList[m] == video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF
															? DepthT : Depth);
						}

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

						driver->setRenderTarget(currentSecondaryShadowMap, true, true, SColor(0xffffffff));
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

					if (!ShadowNodeArray[i].node->isVisible()) {
						continue;
					}

					const u32 CurrentMaterialCount = ShadowNodeArray[i].node->getMaterialCount();
					core::array<irr::s32> BufferMaterialList(CurrentMaterialCount);
					core::array<irr::video::ITexture*> BufferTextureList(CurrentMaterialCount);

					for(u32 m = 0;m < CurrentMaterialCount;++m)
					{
						BufferMaterialList.push_back(ShadowNodeArray[i].node->getMaterial(m).MaterialType);
						BufferTextureList.push_back(ShadowNodeArray[i].node->getMaterial(m).getTexture(0));

						ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)Shadow[ShadowNodeArray[i].filterType];
						ShadowNodeArray[i].node->getMaterial(m).setTexture(0, currentShadowMapTexture);
					}

					ShadowNodeArray[i].node->OnAnimate(device->getTimer()->getTime());
					ShadowNodeArray[i].node->render();

					for(u32 m = 0;m < CurrentMaterialCount;++m)
					{
						ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[m];
						ShadowNodeArray[i].node->getMaterial(m).setTexture(0, BufferTextureList[m]);
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
		/*skyNode->setMaterialType((E_MATERIAL_TYPE)WhiteWash);
		skyNode->OnAnimate(device->getTimer()->getTime());
		skyNode->render();
		skyNode->setMaterialType(EMT_SOLID);*/
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

	if (useMotionBlur) { //&& smgr->getActiveCamera() == FPSCamera) {

		core::vector3df camRotationDiff = smgr->getActiveCamera()->getRotation() - lastCameraRotation;
		lastTimeRotationDiff = device->getTimer()->getTime() - lastTimeRotationDiff;
		if (abs(camRotationDiff.X) > 50 || abs(camRotationDiff.Y) > 50 || abs(camRotationDiff.Z) > 50) {
			motionBlur->getCallback()->m_Strength = (0.8f * (camRotationDiff.Y / (lastTimeRotationDiff / 10.f))) / (360.f / (lastTimeRotationDiff / 10.f));
		} else {
			motionBlur->getCallback()->m_Strength = 0.2f;
		}
		lastCameraRotation = smgr->getActiveCamera()->getRotation();
		lastTimeRotationDiff = device->getTimer()->getTime();

		motionBlur->render();
		driver->setRenderTarget(ScreenQuad.rt[1], true, true, ClearColour);
		motionBlur->renderFinal();

	} else {
		driver->setRenderTarget(ScreenQuad.rt[1], true, true, ClearColour);
		smgr->drawAll();
	}

	if (useLightScattering) {
        /// LightScatteringRTT is the RTT we created for the pass
		driver->setRenderTarget(LightScatteringRTT, true, true, SColor(255, 0, 0, 0));

		for (u32 g=0; g < driver->getDynamicLightCount(); g++)
			driver->turnLightOn(g, false);

        /// LightScatteringPass is the scene nodes array
		for(u32 i = 0;i < LightScatteringPass.size();++i)
		{
			if (!LightScatteringPass[i]->isVisible())
				continue;
            /// Create configurations array
			core::array<irr::video::SMaterial> BufferMaterialList(LightScatteringPass[i]->getMaterialCount());
			BufferMaterialList.set_used(0);

            /// Save configurations, here we save all SMateiral structures of the scene nodes
			for(u32 g = 0;g < LightScatteringPass[i]->getMaterialCount();++g)
				BufferMaterialList.push_back(LightScatteringPass[i]->getMaterial(g));

			ESCENE_NODE_TYPE type = LightScatteringPass[i]->getType();

            /// Because we chose to render "Bill Boards" normally, we configure all other scene nodes to be
            /// completely black
			if (type != ESNT_BILLBOARD) {
				for(u32 g = 0;g < LightScatteringPass[i]->getMaterialCount();++g) {
					E_MATERIAL_TYPE mattype = LightScatteringPass[i]->getMaterial(g).MaterialType;
					bool lsptest = (mattype != EMT_SOLID && mattype != EMT_TRANSPARENT_ADD_COLOR
									&& mattype != EMT_TRANSPARENT_ALPHA_CHANNEL
									&& mattype != EMT_TRANSPARENT_ALPHA_CHANNEL_REF
									&& mattype != EMT_TRANSPARENT_VERTEX_ALPHA);
                    /// If current material type is a custom material type you created (like Normal Mapping), draw it SOLID.
                    /// If not, exceptionally, we don't touch the material type to keep transparent materials
					if (lsptest) {
                        //LightScatteringPass[i]->getMaterial(g).MaterialType = EMT_SOLID;
                        LightScatteringPass[i]->setMaterialType(EMT_SOLID);
                        break;
					}
				}

                /// Set all materials of the current scene node lighting
				LightScatteringPass[i]->setMaterialFlag(EMF_LIGHTING, true);
                /// Configure the emissive color to black
				for (u32 g=0; g < LightScatteringPass[i]->getMaterialCount(); g++) {
					LightScatteringPass[i]->getMaterial(g).EmissiveColor = SColor(255, 0, 0, 0);
				}
			}

            /// Animate and render the scene node
            LightScatteringPass[i]->OnAnimate(device->getTimer()->getTime());
            LightScatteringPass[i]->render();

            /// Reset current scene node configuration
			for(u32 g = 0;g < LightScatteringPass[i]->getMaterialCount();++g)
				LightScatteringPass[i]->getMaterial(g) = BufferMaterialList[g];
		}

        /// Reset current RTT to the previous RTT
        driver->setRenderTarget(0, false, false);
	}

	if (useReflectionPass) {
		const f32 CLIP_PLANE_OFFSET_Y = 5.0f;
		irr::scene::ICameraSceneNode *currentCamera = smgr->getActiveCamera();

		driver->setRenderTarget(ReflectionRTT, true, true, ClearColour);

		cameraForPasses->setFarValue(currentCamera->getFarValue());
		cameraForPasses->setFOV(currentCamera->getFOV());

		//Set position
		core::vector3df position = currentCamera->getAbsolutePosition();
		position.Y = -position.Y + 2 * 0; //position of the water
		cameraForPasses->setPosition(position);

		//Set target
		core::vector3df target = currentCamera->getTarget();
		target.Y = -target.Y + 2 * 0;
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

	// Perform normal pass after rendering, to ensure animations stay up to date.
	if(renderNormalPass)
	{
		driver->setRenderTarget(normalRenderRTT, true, true, SColor(0x0));

		normalMC->FarLink = smgr->getActiveCamera()->getFarValue();

		for(u32 i = 0;i < DepthPassArray.size();++i)
		{
			if (!DepthPassArray[i]->isVisible())
				continue;

			core::array<irr::s32> BufferMaterialList(DepthPassArray[i]->getMaterialCount());
			BufferMaterialList.set_used(0);

			for(u32 g = 0;g < DepthPassArray[i]->getMaterialCount();++g)
				BufferMaterialList.push_back(DepthPassArray[i]->getMaterial(g).MaterialType);

            DepthPassArray[i]->setMaterialType((E_MATERIAL_TYPE)NormalPass);
            DepthPassArray[i]->OnAnimate(device->getTimer()->getTime());
            DepthPassArray[i]->render();

			for(u32 g = 0;g < DepthPassArray[i]->getMaterialCount();++g)
				DepthPassArray[i]->getMaterial(g).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[g];
		}

        driver->setRenderTarget(0, false, false);
	}

	// Perform depth pass after rendering, to ensure animations stay up to date.
	if(DepthPass)
	{
		driver->setRenderTarget(DepthRTT, true, true, SColor(0xffffffff));
        //driver->setRenderTarget(DepthRTT, true, true, SColor(255, 0, 0, 0));

		// Set max distance constant for depth shader.
		depthMC->FarLink = smgr->getActiveCamera()->getFarValue();

		for(u32 i = 0;i < DepthPassArray.size();++i)
		{
			if (!DepthPassArray[i]->isVisible())
				continue;

			core::array<irr::s32> BufferMaterialList(DepthPassArray[i]->getMaterialCount());
			BufferMaterialList.set_used(0);

			for(u32 g = 0;g < DepthPassArray[i]->getMaterialCount();++g)
				BufferMaterialList.push_back(DepthPassArray[i]->getMaterial(g).MaterialType);

            DepthPassArray[i]->setMaterialType((E_MATERIAL_TYPE)Depth);
            DepthPassArray[i]->OnAnimate(device->getTimer()->getTime());
            DepthPassArray[i]->render();

			for(u32 g = 0;g < DepthPassArray[i]->getMaterialCount();++g)
				DepthPassArray[i]->getMaterial(g).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[g];
		}

        driver->setRenderTarget(0, false, false);
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
	ScreenQuad.getMaterial().setTexture(2, DepthRTT);
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
			ScreenQuad.getMaterial().setTexture(2, DepthRTT);
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
		/*ppm->Render(PostProcessingRoutinesSize == 0 && !rtTest ? ScreenRTT : ScreenQuad.rt[int(Alter)],
					HDRProcessedRT);
		driver->setRenderTarget(outputTarget, true, true, ClearColour);
		quad->SetTexture(HDRProcessedRT);
		quad->Render(false);*/

		//driver->setRenderTarget(outputTarget, true, true, ClearColour);
		//ppm->Render(PostProcessingRoutinesSize == 0 && !rtTest ? ScreenRTT : ScreenQuad.rt[int(Alter)]);

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

//---------------------------------------------------------------------------------------------
//------------------------------------RADIOSITY------------------------------------------------
//---------------------------------------------------------------------------------------------

void EffectHandler::updateRadiosity(const irr::u32 time, const bool screenSpaceOnly,irr::video::ITexture* outputTarget,irr::scene::ISceneNode* node,irr::core::array<scene::IMeshBuffer*>* buffers)
{
    if(outputTarget)
    {
        if(outputTarget->getSize()!= ScreenRTT->getSize())
        {
            setScreenRenderTargetResolution(outputTarget->getSize());
        }
    }
    if (shadowsUnsupported || smgr->getActiveCamera() == 0)
        return;

    if (!ShadowNodeArray.empty() && !LightList.empty())
    {
        driver->setRenderTarget(ScreenQuad.rt[0], true, true, AmbientColour);

        ICameraSceneNode* activeCam = smgr->getActiveCamera();
        activeCam->OnAnimate(device->getTimer()->getTime());
        activeCam->OnRegisterSceneNode();
        activeCam->render();

        const u32 ShadowNodeArraySize = ShadowNodeArray.size();
        const u32 LightListSize = LightList.size();
        for (u32 l = 0; l < LightListSize; ++l)
        {
            // Set max distance constant for depth shader.
            depthMC->FarLink = LightList[l].getFarValue();

            driver->setTransform(ETS_VIEW, LightList[l].getViewMatrix());
            driver->setTransform(ETS_PROJECTION, LightList[l].getProjectionMatrix());

            ITexture* currentShadowMapTexture = getShadowMapTexture(LightList[l].getShadowMapResolution());
            driver->setRenderTarget(currentShadowMapTexture, true, true, SColor(0xffffffff));

            for (u32 i = 0; i < ShadowNodeArraySize; ++i)
            {
                if (ShadowNodeArray[i].shadowMode == ESM_RECEIVE || ShadowNodeArray[i].shadowMode == ESM_EXCLUDE)
                    continue;

                const u32 CurrentMaterialCount = ShadowNodeArray[i].node->getMaterialCount();
                core::array<irr::s32> BufferMaterialList(CurrentMaterialCount);
                BufferMaterialList.set_used(0);

                for (u32 m = 0; m < CurrentMaterialCount; ++m)
                {
                    BufferMaterialList.push_back(ShadowNodeArray[i].node->getMaterial(m).MaterialType);
                    ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)
                            (BufferMaterialList[m] == video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF ? DepthT : Depth);
                }

                ShadowNodeArray[i].node->OnAnimate(device->getTimer()->getTime());
                ShadowNodeArray[i].node->render();

                const u32 BufferMaterialListSize = BufferMaterialList.size();
                for (u32 m = 0; m < BufferMaterialListSize; ++m)
                    ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[m];
            }

            // Blur the shadow map texture if we're using VSM filtering.
            if (useVSM)
            {
                ITexture* currentSecondaryShadowMap = getShadowMapTexture(LightList[l].getShadowMapResolution(), true);

                driver->setRenderTarget(currentSecondaryShadowMap, true, true, SColor(0xffffffff));
                ScreenQuad.getMaterial().setTexture(0, currentShadowMapTexture);
                ScreenQuad.getMaterial().setTexture(0, currentShadowMapTexture);
                ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)VSMBlurH;
                ScreenQuad.getMaterial().TextureLayer[0].TextureWrapU=1;
                ScreenQuad.getMaterial().TextureLayer[0].BilinearFilter=0;
                //ScreenQuad.getMaterial().TextureLayer[1].BilinearFilter=false;

                ScreenQuad.render(driver);

                driver->setRenderTarget(currentShadowMapTexture, true, true, SColor(0xffffffff));
                ScreenQuad.getMaterial().setTexture(0, currentSecondaryShadowMap);
                ScreenQuad.getMaterial().setTexture(0, currentSecondaryShadowMap);
                ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)VSMBlurV;
                ScreenQuad.getMaterial().TextureLayer[0].BilinearFilter=0;
                //ScreenQuad.getMaterial().TextureLayer[1].BilinearFilter=false;

                ScreenQuad.render(driver);
            }

            driver->setRenderTarget(ScreenQuad.rt[1], true, true, SColor(0,0,0,0));

            driver->setTransform(ETS_VIEW, activeCam->getViewMatrix());
            driver->setTransform(ETS_PROJECTION, activeCam->getProjectionMatrix());

            shadowMC->LightColour = LightList[l].getLightColor();
            shadowMC->LightLink = LightList[l].getPosition();
            shadowMC->FarLink = LightList[l].getFarValue();
            shadowMC->ViewLink = LightList[l].getViewMatrix();
            shadowMC->ProjLink = LightList[l].getProjectionMatrix();
            shadowMC->MapRes = (f32)LightList[l].getShadowMapResolution();

            for (u32 i = 0; i < ShadowNodeArraySize; ++i)
            {
                if (ShadowNodeArray[i].shadowMode == ESM_CAST || ShadowNodeArray[i].shadowMode == ESM_EXCLUDE)
                    continue;

                const u32 CurrentMaterialCount = ShadowNodeArray[i].node->getMaterialCount();
                core::array<irr::s32> BufferMaterialList(CurrentMaterialCount);
                core::array<irr::video::ITexture*> BufferTextureList(CurrentMaterialCount);

                core::array<u8> clampModesUList(CurrentMaterialCount);
                core::array<u8> clampModesVList(CurrentMaterialCount);

                for (u32 m = 0; m < CurrentMaterialCount; ++m)
                {
                    BufferMaterialList.push_back(ShadowNodeArray[i].node->getMaterial(m).MaterialType);
                    BufferTextureList.push_back(ShadowNodeArray[i].node->getMaterial(m).getTexture(0));

                    if (screenSpaceOnly)
                    {
						ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)Shadow[ShadowNodeArray[i].filterType];
                    }

                    else
                        ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)Shadow[ShadowNodeArray[i].filterType];
                    ShadowNodeArray[i].node->getMaterial(m).setTexture(0, currentShadowMapTexture);

                    clampModesUList.push_back(ShadowNodeArray[i].node->getMaterial(m).TextureLayer[0].TextureWrapU);
                    clampModesVList.push_back(ShadowNodeArray[i].node->getMaterial(m).TextureLayer[0].TextureWrapV);
                    //ShadowNodeArray[i].node->getMaterial(m).TextureLayer[0].TextureWrapU=video::ETC_CLAMP;
                    //ShadowNodeArray[i].node->getMaterial(m).TextureLayer[0].TextureWrapV=video::ETC_CLAMP;
                }

                //Render all project shadow maps
                if (!node)
                {
                    ShadowNodeArray[i].node->OnAnimate(device->getTimer()->getTime());
                    ShadowNodeArray[i].node->render();
                }
                else
                {
                    //render projected shadows on specific buffers of a single shAdow map
                    if (ShadowNodeArray[i].node==node)
                    {
                        ShadowNodeArray[i].node->OnAnimate(device->getTimer()->getTime());

                        driver->setTransform(video::ETS_WORLD, node->getAbsoluteTransformation());
                        irr::core::array<scene::IMeshBuffer*>bufferlist=*buffers;

                        for (int buf=0; buf<bufferlist.size(); buf++)
                        {
                            driver->setMaterial(ShadowNodeArray[i].node->getMaterial(buf));
                            driver->drawMeshBuffer(bufferlist[buf]);
                        }
                    }
                }

                for (u32 m = 0; m < CurrentMaterialCount; ++m)
                {
                    ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[m];
                    ShadowNodeArray[i].node->getMaterial(m).setTexture(0, BufferTextureList[m]);

                    ShadowNodeArray[i].node->getMaterial(m).TextureLayer[0].TextureWrapU=clampModesUList[m];
                    ShadowNodeArray[i].node->getMaterial(m).TextureLayer[0].TextureWrapV=clampModesVList[m];
                }
            }

            driver->setRenderTarget(ScreenQuad.rt[0], false, false, SColor(0,0,0,0));
            ScreenQuad.getMaterial().setTexture(0, ScreenQuad.rt[1]);
            ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)Simple;
            ScreenQuad.getMaterial().ColorMask=0xffffffff;
            ScreenQuad.render(driver);
        }
        /*
                // Render all the excluded and casting-only nodes.
                for (u32 i = 0;i < ShadowNodeArraySize;++i)
                {
                    if (ShadowNodeArray[i].shadowMode != ESM_CAST && ShadowNodeArray[i].shadowMode != ESM_EXCLUDE)
                        continue;

                    const u32 CurrentMaterialCount = ShadowNodeArray[i].node->getMaterialCount();
                    core::array<irr::s32> BufferMaterialList(CurrentMaterialCount);
                    BufferMaterialList.set_used(0);

                    for (u32 m = 0;m < CurrentMaterialCount;++m)
                    {
                        BufferMaterialList.push_back(ShadowNodeArray[i].node->getMaterial(m).MaterialType);

                        switch (BufferMaterialList[m])
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

                    for (u32 m = 0;m < CurrentMaterialCount;++m)
                        ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[m];
                }*/
    }
    else
    {
        driver->setRenderTarget(ScreenQuad.rt[0], true, true, SColor(0xffffffff));
    }
    if (!screenSpaceOnly)
    {
        driver->setRenderTarget(ScreenQuad.rt[1], true, true, SColor(0xffffffff));

        core::list<scene::ISceneNode*>::ConstIterator it = smgr->getRootSceneNode()->getChildren().begin();
        while (it!=smgr->getRootSceneNode()->getChildren().end())
        {
            scene::ISceneNode* node = *it;
            node->OnAnimate(time);
            node->render();
            it++;

        }
    }

    const u32 PostProcessingRoutinesSize = PostProcessingRoutines.size();

    if (!screenSpaceOnly)
    {
        driver->setRenderTarget(outputTarget, true, true, SColor(0x0));


        ScreenQuad.getMaterial().setTexture(0, ScreenQuad.rt[1]);
        ScreenQuad.getMaterial().setTexture(1, ScreenQuad.rt[0]);

        ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)LightModulate;

        ScreenQuad.render(driver);
    }

    if (outputTarget)
    {

        driver->setRenderTarget(ScreenRTT , true, true, SColor(0,255,0,255));
        ScreenQuad.getMaterial().setTexture(0, ScreenQuad.rt[0]);
        ScreenQuad.getMaterial().setTexture(1, ScreenQuad.rt[0]);
        ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)VSMBlurH;
        ScreenQuad.getMaterial().TextureLayer[0].TextureWrapU=1;
        ScreenQuad.getMaterial().TextureLayer[0].TextureWrapV=1;
        ScreenQuad.getMaterial().TextureLayer[0].BilinearFilter=true;
        ScreenQuad.getMaterial().TextureLayer[1].BilinearFilter=false;


        //ScreenQuad.getMaterial().setFlag(video::EMF_BILINEAR_FILTER,false);
        ScreenQuad.render(driver);

        driver->setRenderTarget(ScreenQuad.rt[0], true, true, SColor(0,255,0,255))  ;
        ScreenQuad.getMaterial().setTexture(0, ScreenRTT );
        ScreenQuad.getMaterial().setTexture(1, ScreenRTT );
        ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)VSMBlurV;
        ScreenQuad.getMaterial().TextureLayer[0].TextureWrapU=1;
        ScreenQuad.getMaterial().TextureLayer[0].TextureWrapV=1;
        ScreenQuad.getMaterial().TextureLayer[0].BilinearFilter=true;
        ScreenQuad.getMaterial().TextureLayer[1].BilinearFilter=false;

        ScreenQuad.render(driver);


        //ScreenQuad.getMaterial().setFlag(video::EMF_BILINEAR_FILTER,false);
        ScreenQuad.render(driver);

        driver->setRenderTarget(outputTarget, true, true, SColor(0,255,0,255))  ;
        ScreenQuad.getMaterial().setTexture(0, ScreenQuad.rt[0] );
        ScreenQuad.getMaterial().setTexture(1, ScreenQuad.rt[0] );
        ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)VSMBlurH;
        ScreenQuad.getMaterial().TextureLayer[0].TextureWrapU=1;
        ScreenQuad.getMaterial().TextureLayer[0].TextureWrapV=1;
        ScreenQuad.getMaterial().TextureLayer[0].BilinearFilter=true;
        ScreenQuad.getMaterial().TextureLayer[1].BilinearFilter=true;

        ScreenQuad.render(driver);

    }

}

