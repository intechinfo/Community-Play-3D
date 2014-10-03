#include "stdafx.h"

#include "CGaussianBlurHPostProcess.h"
#include "CGlobalContext.h"
#include "CResourceManager.h"
#include "CPaths.h"


namespace Graphics {

GaussianBlurHPostProcess::GaussianBlurHPostProcess() : GaussianBlurBase() {
	IReadFile* fh;
	if (GlobalContext::DeviceContext.GetVideoDriver()->getDriverType() == video::EDT_OPENGL)
        fh = Resources::ResourceManager::OpenResource(Paths::PostProcesses::GaussianBlurHGL);
    else
        fh = Resources::ResourceManager::OpenResource(Paths::PostProcesses::GaussianBlur);
    
    IVideoDriver* vd = GlobalContext::DeviceContext.GetVideoDriver();
    IReadFile *fh2 = 0;
    if (vd->getDriverType() == video::EDT_OPENGL)
        fh2 = Resources::ResourceManager::OpenResource(Paths::PostProcesses::HDRVertex);
	else
		fh2 = Resources::ResourceManager::OpenResource(Paths::PostProcesses::HDRVertexHLSL);

	if(fh == NULL)
		throw new Exception("Horizontal Gaussian Blur shader file couldn't be opened", __FUNCTION__);

	mt = (E_MATERIAL_TYPE)GlobalContext::DeviceContext.GetVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
        fh2,
        vd->getDriverType() == EDT_OPENGL ? "main" : "vertexMain", video::EVST_VS_2_0,
		fh, "PSGaussianBlurH", video::EPST_PS_2_0,
		this);

	if(mt < 0) {
		fh->drop();
		throw new Exception("Horizontal Gaussian Blur shader couldn't be loaded", __FUNCTION__);
	}

	quad.SetMaterialType(mt);

	fh->drop();
    if (fh2) fh2->drop();
}

void GaussianBlurHPostProcess::OnSetConstants(IMaterialRendererServices* services, s32 userData) {
    irr::s32 texVar = 0;

	const irr::core::dimension2du currentRTTSize = services->getVideoDriver()->getCurrentRenderTargetSize();
	const irr::f32 screenX = (irr::f32)currentRTTSize.Width, screenY = (irr::f32)currentRTTSize.Height;
	services->setVertexShaderConstant("screenX", &screenX, 1);
	services->setVertexShaderConstant("screenY", &screenY, 1);
    
    #ifndef _IRR_OSX_PLATFORM_
    services->setPixelShaderConstant("tex0", &texVar, 1);
	services->setPixelShaderConstant("blurOffsets", blurOffsets, 9);
	services->setPixelShaderConstant("blurWeights", blurWeights, 9);
    #else
    services->setPixelShaderConstant(services->getPixelShaderConstantID("tex0"), &texVar, 1);
    services->setPixelShaderConstant(services->getPixelShaderConstantID("blurOffsets[0]"), blurOffsets, 9);
    services->setPixelShaderConstant(services->getPixelShaderConstantID("blurWeights[0]"), blurWeights, 9);
    #endif
    
}

void GaussianBlurHPostProcess::Render(ITexture* __restrict source, ITexture* __restrict output) {
	if(output->getSize() != lastOutputDims) {
		//we need to update the shader offsets since the output size
		//has changed
		lastOutputDims = output->getSize();

		for(u32 c = 0; c < 9; c++)
			blurOffsets[c] = (static_cast<f32>(c) - 4.0f) * (1.0f / static_cast<f32>(lastOutputDims.Width));
	}

	quad.SetTexture(source);
	quad.Render(output);
}

} //end namespace Graphics
