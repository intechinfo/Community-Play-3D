#include "stdafx.h"

#include "CTextureAdder.h"
#include "CGlobalContext.h"
#include "CResourceManager.h"
#include "CPaths.h"

namespace Graphics {

TextureAdder::TextureAdder(const dimension2d<u32>& outputSize, ECOLOR_FORMAT bufferType) {
	IReadFile* fh;
	if (GlobalContext::DeviceContext.GetVideoDriver()->getDriverType() == video::EDT_OPENGL)
        fh = Resources::ResourceManager::OpenResource(Paths::PostProcesses::TextureAdderGL);
	else
        fh = Resources::ResourceManager::OpenResource(Paths::PostProcesses::TextureAdder);

	if(fh == NULL)
		throw new Exception("Texture Adder shader file couldn't be opened", __FUNCTION__);
    
    IVideoDriver* vd = GlobalContext::DeviceContext.GetVideoDriver();
    IReadFile *fh2 = 0;
    if (vd->getDriverType() == video::EDT_OPENGL)
        fh2 = Resources::ResourceManager::OpenResource(Paths::PostProcesses::HDRVertex);

	mt = (E_MATERIAL_TYPE)vd->getGPUProgrammingServices()->
		addHighLevelShaderMaterialFromFiles(
        vd->getDriverType() == EDT_OPENGL ? fh2 : nullptr,
        vd->getDriverType() == EDT_OPENGL ? "main" : nullptr, video::EVST_VS_1_1,
		fh, "PSTextureAdder", video::EPST_PS_2_0, this);

	if(mt < 0) {
		fh->drop();
		throw new Exception("Texture Adder shader couldn't be loaded", __FUNCTION__);
	}

	out = vd->addRenderTargetTexture(outputSize, "textureAdderRT", bufferType);

	quad.SetMaterialType(mt);

	fh->drop();
    if (fh2) fh2->drop();
}

void TextureAdder::OnSetConstants(IMaterialRendererServices* services, s32 userData) {
    irr::s32 texVar = 0;
    services->setPixelShaderConstant("tex0", &texVar, 1);
    irr::s32 texVar1 = 1;
    services->setPixelShaderConstant("tex1", &texVar1, 1);
}

void TextureAdder::Render(ITexture* __restrict source) {
	quad.SetTexture(source);
	quad.Render(out);
}

void TextureAdder::Render(ITexture* __restrict tex0, ITexture* __restrict tex1) {
	quad.SetTexture(tex0, 0);
	quad.SetTexture(tex1, 1);
	quad.Render(out);
}

} //end namespace Graphics
