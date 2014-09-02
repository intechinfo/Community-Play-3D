#include "stdafx.h"

#include "CDownSampleX4PostProcess.h"
#include "CGlobalContext.h"
#include "CResourceManager.h"
#include "CPaths.h"

namespace Graphics {

DownSampleX4PostProcess::DownSampleX4PostProcess() {
	IReadFile* fh;
	if (GlobalContext::DeviceContext.GetVideoDriver()->getDriverType() == video::EDT_OPENGL)
        fh = Resources::ResourceManager::OpenResource(Paths::PostProcesses::DownSampleX4GL);
	else
        fh = Resources::ResourceManager::OpenResource(Paths::PostProcesses::DownSampleX4);

    IVideoDriver* vd = GlobalContext::DeviceContext.GetVideoDriver();
    IReadFile *fh2 = 0;
    if (vd->getDriverType() == video::EDT_OPENGL)
        fh2 = Resources::ResourceManager::OpenResource(Paths::PostProcesses::HDRVertex);

	if(fh == NULL)
		throw new Exception("Down-Sample shader file couldn't be opened", __FUNCTION__);

	mt = (E_MATERIAL_TYPE)GlobalContext::DeviceContext.GetVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
        vd->getDriverType() == EDT_OPENGL ? fh2 : nullptr,
        vd->getDriverType() == EDT_OPENGL ? "main" : nullptr, video::EVST_VS_1_1,
		fh, "PSDownSample", video::EPST_PS_2_0,
		this);

	if(mt < 0) {
		fh->drop();
		throw new Exception("Down-Sample shader couldn't be loaded", __FUNCTION__);
	}

	quad.SetMaterialType(mt);

	fh->drop();
    if (fh2) fh2->drop();
}

void DownSampleX4PostProcess::OnSetConstants(IMaterialRendererServices* services, s32 userData) {
    irr::s32 texVar = 0;
    #ifndef _IRR_OSX_PLATFORM_
    services->setPixelShaderConstant("tex0", &texVar, 1);
	services->setPixelShaderConstant("dsOffsets", reinterpret_cast<f32*>(&dsOffsets), 32);
    #else
    services->setPixelShaderConstant(services->getPixelShaderConstantID("tex0"), &texVar, 1);
	services->setPixelShaderConstant(services->getPixelShaderConstantID("dsOffsets[0]"), reinterpret_cast<f32*>(&dsOffsets), 32);
    #endif
}

void DownSampleX4PostProcess::Render(ITexture* __restrict source, ITexture* __restrict output) {
	if(output->getSize() != lastOutputDims) {
		//we need to update the shader offsets since the output size
		//has changed
		lastOutputDims = output->getSize();

		s32 idx = 0;
		for( s32 i = -2; i < 2; i++ ) {
			for( s32 j = -2; j < 2; j++ ) {
				dsOffsets[idx][0] = (static_cast<f32>( i ) + 0.5f) * (1.0f / static_cast<f32>(lastOutputDims.Width));
				dsOffsets[idx][1] = (static_cast<f32>( j ) + 0.5f) * (1.0f / static_cast<f32>(lastOutputDims.Height));
				idx++;
			}
		}
	}

	quad.SetTexture(source);
	quad.Render(output);
}

} //end namespace Graphics
