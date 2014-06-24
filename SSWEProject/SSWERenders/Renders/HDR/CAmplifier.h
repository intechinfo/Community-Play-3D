#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace video;
using namespace scene;
using namespace io;

#include "CGlobalContext.h"
#include "CResourceManager.h"

namespace Graphics {

class Amplifier {

public:

	Amplifier() {
		IReadFile* fh = GlobalContext::DeviceContext.GetFileSystem()->createAndOpenFile("Amplifier.fx");
		if(fh == NULL)
			throw new Exception("Amplifier shader file couldn't be opened", __FUNCTION__);

		IVideoDriver* vd = GlobalContext::DeviceContext.GetVideoDriver();

		mt = (E_MATERIAL_TYPE)vd->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(nullptr, nullptr, video::EVST_VS_1_1, fh,
																								   "PSAmplifier", video::EPST_PS_2_0);

		if(mt < 0) {
			fh->drop();
			throw new Exception("Amplifier shader couldn't be loaded", __FUNCTION__);
		}

		fh->drop();
	}

	E_MATERIAL_TYPE getMaterialType() { return mt; }

protected:

	E_MATERIAL_TYPE mt;
};

} //end namespace Graphics