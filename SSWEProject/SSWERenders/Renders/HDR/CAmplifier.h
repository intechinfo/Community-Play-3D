#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace video;
using namespace scene;
using namespace io;

#include "CGlobalContext.h"
#include "CResourceManager.h"
#include "CPaths.h"

namespace Graphics {

class Amplifier : public irr::video::IShaderConstantSetCallBack {

public:

	Amplifier() {
        IVideoDriver* vd = GlobalContext::DeviceContext.GetVideoDriver();

		IReadFile* fh;
		if (vd->getDriverType() == video::EDT_OPENGL)
            fh = GlobalContext::DeviceContext.GetFileSystem()->createAndOpenFile("Amplifier.gfx");
        else
            fh = GlobalContext::DeviceContext.GetFileSystem()->createAndOpenFile("Amplifier.fx");
        
        IReadFile *fh2 = 0;
        if (vd->getDriverType() == video::EDT_OPENGL)
            fh2 = Resources::ResourceManager::OpenResource(Paths::PostProcesses::HDRVertex);

		if(fh == NULL)
			throw new Exception("Amplifier shader file couldn't be opened", __FUNCTION__);

		mt = (E_MATERIAL_TYPE)vd->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(vd->getDriverType() == EDT_OPENGL ? fh2 : nullptr,
                                                                                                   vd->getDriverType() == EDT_OPENGL ? "main" : nullptr,
                                                                                                   video::EVST_VS_1_1, fh,
																								   "PSAmplifier", video::EPST_PS_2_0);

		if(mt < 0) {
			fh->drop();
			throw new Exception("Amplifier shader couldn't be loaded", __FUNCTION__);
		}

		fh->drop();
        if (fh2) fh2->drop();
	}

	void OnSetConstants(IMaterialRendererServices* services, s32 userData) {
		irr::s32 tex0 = 0;
		services->setPixelShaderConstant("tex0", &tex0, 1);
	}

	E_MATERIAL_TYPE getMaterialType() { return mt; }

protected:

	E_MATERIAL_TYPE mt;
};

} //end namespace Graphics
