#include "stdafx.h"

#include "CLDRBloomPostProcess.h"
#include "CGlobalContext.h"
#include "CResourceManager.h"
#include "CPaths.h"

namespace Graphics {

LDRBloomPostProcess::LDRBloomPostProcess(const dimension2d<u32>& sourceSize)
	: bloomLimit(0.3f)
{
	IReadFile* fh = Resources::ResourceManager::OpenResource(Paths::PostProcesses::BloomGL);
	if(fh == NULL)
		throw new Exception("Bloom shader file couldn't be opened", __FUNCTION__);
    IVideoDriver* vd = GlobalContext::DeviceContext.GetVideoDriver();
    IReadFile *fh2 = 0;
    if (vd->getDriverType() == video::EDT_OPENGL)
        fh2 = Resources::ResourceManager::OpenResource(Paths::PostProcesses::HDRVertex);

	mt = (E_MATERIAL_TYPE)GlobalContext::DeviceContext.GetVideoDriver()->getGPUProgrammingServices()->
		addHighLevelShaderMaterialFromFiles(
        vd->getDriverType() == EDT_OPENGL ? fh2 : nullptr,
        vd->getDriverType() == EDT_OPENGL ? "main" : nullptr, video::EVST_VS_1_1,
		fh, "PSBloom", video::EPST_PS_2_0,
		this);

	if(mt < 0) {
		fh->drop();
		throw new Exception("Bloom shader couldn't be loaded", __FUNCTION__);
	}

	quad.SetMaterialType(mt);

	fh->drop();
    if (fh2) fh2->drop();

	bloomGen = new DSBloomGenerator(sourceSize);
	quad.SetTexture(bloomGen->GetOutput(), 1);
}

LDRBloomPostProcess::~LDRBloomPostProcess() {
	delete bloomGen;
}

void LDRBloomPostProcess::OnSetConstants(IMaterialRendererServices* services, s32 userData) {
	services->setPixelShaderConstant("bloomLimit", &bloomLimit, 1);
}

void LDRBloomPostProcess::Render(ITexture* __restrict source, ITexture* __restrict output) {
	bloomGen->Render(source);
	quad.SetTexture(source, 0);
	quad.Render(output);
}

} //end namespace Graphics