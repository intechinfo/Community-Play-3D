#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace io;

//Post processes
namespace Paths {
namespace PostProcesses {
    #ifdef _IRR_WINDOWS_API_
	const path PostProcessDir("PostProcesses\\");
	#else
    const path PostProcessDir("PostProcesses/");
	#endif
	const path Invert(PostProcessDir + "Invert.fx");
	const path DownSampleX4(PostProcessDir + "DownSampleX4.fx");
	const path BrightPass(PostProcessDir + "BrightPass.fx");
	const path Bloom(PostProcessDir + "Bloom.fx");
	const path GaussianBlur(PostProcessDir + "GaussianBlur.fx");
	const path Luminance(PostProcessDir + "Luminance.fx");
	const path HDR(PostProcessDir + "HDR.fx");
	const path TextureAdder(PostProcessDir + "TextureAdder.fx");

    const path DownSampleX4GL(PostProcessDir + "DownSampleX4.gfx");
    const path BrightPassGL(PostProcessDir + "BrightPass.gfx");
	const path LuminanceGL(PostProcessDir + "Luminance.gfx");
	const path LuminanceDownSampleGL(PostProcessDir + "LuminanceDownSample.gfx");
	const path GaussianBlurHGL(PostProcessDir + "GaussianBlurH.gfx");
	const path GaussianBlurVGL(PostProcessDir + "GaussianBlurV.gfx");
	const path HDRGL(PostProcessDir + "HDR.gfx");
	const path TextureAdderGL(PostProcessDir + "TextureAdder.gfx");

}
} //end namespace Graphics
