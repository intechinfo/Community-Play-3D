#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace video;

#include "CHDRScreenQuad.h"

namespace Graphics {

class IPostProcess {

public:

	virtual void Render(ITexture* __restrict source, ITexture* __restrict output) = 0;

protected:

	CHDRScreenQuad quad;

};

} //end namespace graphics
