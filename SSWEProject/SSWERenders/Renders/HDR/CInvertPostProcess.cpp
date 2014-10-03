#include "stdafx.h"

#include "CInvertPostProcess.h"
#include "CGlobalContext.h"
#include "CResourceManager.h"
#include "CPaths.h"

namespace Graphics {

InvertPostProcess::InvertPostProcess() {
	IReadFile* fh = Resources::ResourceManager::OpenResource(Paths::PostProcesses::Invert);
	if(fh == NULL)
		throw new Exception("Invert shader file couldn't be opened", __FUNCTION__);
    
    IVideoDriver* vd = GlobalContext::DeviceContext.GetVideoDriver();
    IReadFile *fh2 = 0;
    if (vd->getDriverType() == video::EDT_OPENGL)
        fh2 = Resources::ResourceManager::OpenResource(Paths::PostProcesses::HDRVertex);
	else
		fh2 = Resources::ResourceManager::OpenResource(Paths::PostProcesses::HDRVertexHLSL);

	mt = (E_MATERIAL_TYPE)GlobalContext::DeviceContext.GetVideoDriver()->getGPUProgrammingServices()->
		addHighLevelShaderMaterialFromFiles(
        fh2,
        vd->getDriverType() == EDT_OPENGL ? "main" : "vertexMain", video::EVST_VS_1_1,
		fh, "PSInvert", video::EPST_PS_1_1);

	if(mt < 0) {
		fh->drop();
		throw new Exception("Invert shader couldn't be loaded", __FUNCTION__);
	}
		
	quad.SetMaterialType(mt);

	fh->drop();
    if (fh2) fh2->drop();
}

void InvertPostProcess::Render(ITexture* __restrict source, ITexture* __restrict output) {
	quad.SetTexture(source);
	quad.Render(output);
}

} //end namespace Graphics

