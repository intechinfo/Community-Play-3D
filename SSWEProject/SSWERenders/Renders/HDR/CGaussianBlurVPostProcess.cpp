#include "stdafx.h"

#include "CGaussianBlurVPostProcess.h"
#include "CGlobalContext.h"
#include "CResourceManager.h"
#include "CPaths.h"

namespace Graphics {

GaussianBlurVPostProcess::GaussianBlurVPostProcess() : GaussianBlurBase() {
	IReadFile* fh;
	if (GlobalContext::DeviceContext.GetVideoDriver()->getDriverType() == video::EDT_OPENGL)
        fh = Resources::ResourceManager::OpenResource(Paths::PostProcesses::GaussianBlurVGL);
	else
        fh = Resources::ResourceManager::OpenResource(Paths::PostProcesses::GaussianBlur);
    
    IVideoDriver* vd = GlobalContext::DeviceContext.GetVideoDriver();
    IReadFile *fh2 = 0;
    if (vd->getDriverType() == video::EDT_OPENGL)
        fh2 = Resources::ResourceManager::OpenResource(Paths::PostProcesses::HDRVertex);

	if(fh == NULL)
		throw new Exception("Vertical Gaussian Blur shader file couldn't be opened", __FUNCTION__);

	mt = (E_MATERIAL_TYPE)GlobalContext::DeviceContext.GetVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
        vd->getDriverType() == EDT_OPENGL ? fh2 : nullptr,
        vd->getDriverType() == EDT_OPENGL ? "main" : nullptr, video::EVST_VS_2_0,
		fh, "PSGaussianBlurV", video::EPST_PS_2_0,
		this);

	if(mt < 0) {
		fh->drop();
		throw new Exception("Vertical Gaussian Blur shader couldn't be loaded", __FUNCTION__);
	}

	quad.SetMaterialType(mt);

	fh->drop();
    if (fh2) fh2->drop();
}

void GaussianBlurVPostProcess::OnSetConstants(IMaterialRendererServices* services, s32 userData) {
    irr::s32 texVar = 0;
    
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

void GaussianBlurVPostProcess::Render(ITexture* __restrict source, ITexture* __restrict output) {
	if(output->getSize() != lastOutputDims) {
		//we need to update the shader offsets since the output size
		//has changed
		lastOutputDims = output->getSize();

		for(u32 c = 0; c < 9; c++)
			blurOffsets[c] = (static_cast<f32>(c) - 4.0f) * (1.0f / static_cast<f32>(lastOutputDims.Height));
	}

	quad.SetTexture(source);
	quad.Render(output);
}

} //end namespace Graphics
