#include "stdafx.h"

#include "CBrightPassPostProcess.h"
#include "CGlobalContext.h"
#include "CPaths.h"
#include "CResourceManager.h"

using namespace GlobalContext;
using namespace Resources;

namespace Graphics {

BrightPassPostProcess::BrightPassPostProcess() : brightThreshold(0.8f)
{
	IReadFile* fh;
	if (DeviceContext.GetVideoDriver()->getDriverType() == video::EDT_OPENGL)
        fh = ResourceManager::OpenResource(Paths::PostProcesses::BrightPassGL);
    else
        fh = ResourceManager::OpenResource(Paths::PostProcesses::BrightPass);

	if(fh == NULL)
		throw new Exception("Bright Pass shader file couldn't be opened", __FUNCTION__);

	mt = (E_MATERIAL_TYPE)DeviceContext.GetVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(nullptr,
		nullptr, video::EVST_VS_1_1,
		fh, "PSBrightPass", video::EPST_PS_2_0,
		this);

	if(mt < 0) {
		fh->drop();
		throw new Exception("Bright Pass shader couldn't be loaded", __FUNCTION__);
	}

	quad.SetMaterialType(mt);

	fh->drop();
}

void BrightPassPostProcess::OnSetConstants(IMaterialRendererServices* services, s32 userData) {
    f32 dsOffsets2[8];
    for (u32 i=0; i < 4; i++)
        for (u32 j=0; j < 2; j++)
            dsOffsets2[i * 2 + j] = dsOffsets[i][j];
    
    irr::s32 texVar = 0;
    services->setPixelShaderConstant("tex0", &texVar, 1);
	services->setPixelShaderConstant("brightThreshold", &brightThreshold, 1);
	services->setPixelShaderConstant("dsOffsets", reinterpret_cast<f32*>(&dsOffsets2), 8);
}

void BrightPassPostProcess::Render(ITexture* __restrict source, ITexture* __restrict output) {
	if(source->getSize() != lastSourceDims) {
		//we need to update the shader offsets since the output size
		//has changed
		lastSourceDims = source->getSize();

		// Because the source and destination are NOT the same sizes, we
		// need to provide offsets to correctly map between them.
		f32 sU = (1.0f / static_cast<f32>( lastSourceDims.Width));
		f32 sV = (1.0f / static_cast<f32>( lastSourceDims.Height));

		dsOffsets[0][0] = -0.5f * sU;
		dsOffsets[0][1] = 0.5f * sV;
		dsOffsets[1][0] = 0.5f * sU;
		dsOffsets[1][1] = 0.5f * sV;
		dsOffsets[2][0] = -0.5f * sU;
		dsOffsets[2][1] = -0.5f * sV;
		dsOffsets[3][0] =  0.5f * sU;
		dsOffsets[3][1] = -0.5f * sV;
	}

	quad.SetTexture(source);
	quad.Render(output);
}

} //end namespace Graphics
