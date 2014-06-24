#include "stdafx.h"

#include "CGaussianBlurVPostProcess.h"
#include "CGlobalContext.h"
#include "CResourceManager.h"
#include "CPaths.h"

namespace Graphics {

GaussianBlurVPostProcess::GaussianBlurVPostProcess() {
	IReadFile* fh = Resources::ResourceManager::OpenResource(Paths::PostProcesses::GaussianBlur);
	if(fh == NULL)
		throw new Exception("Vertical Gaussian Blur shader file couldn't be opened", __FUNCTION__);

	mt = (E_MATERIAL_TYPE)GlobalContext::DeviceContext.GetVideoDriver()->getGPUProgrammingServices()->
		addHighLevelShaderMaterialFromFiles(nullptr,
		nullptr, video::EVST_VS_1_1,
		fh, "PSGaussianBlurV", video::EPST_PS_2_0,
		this);

	if(mt < 0) {
		fh->drop();
		throw new Exception("Vertical Gaussian Blur shader couldn't be loaded", __FUNCTION__);
	}
		
	quad.SetMaterialType(mt);

	fh->drop();
}

void GaussianBlurVPostProcess::OnSetConstants(IMaterialRendererServices* services, s32 userData) {
	services->setPixelShaderConstant("blurOffsets", blurOffsets, 9);
	services->setPixelShaderConstant("blurWeights", blurWeights, 9);
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