#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace io;

//Post processes
namespace Paths {
namespace PostProcesses {

	const path PostProcessDir("PostProcesses\\");
	const path Invert(PostProcessDir + "Invert.fx");
	const path DownSampleX4(PostProcessDir + "DownSampleX4.fx");
	const path BrightPass(PostProcessDir + "BrightPass.fx");
	const path Bloom(PostProcessDir + "Bloom.fx");
	const path GaussianBlur(PostProcessDir + "GaussianBlur.fx");
	const path Luminance(PostProcessDir + "Luminance.fx");
	const path HDR(PostProcessDir + "HDR.fx");
	const path TextureAdder(PostProcessDir + "TextureAdder.fx");

}
} //end namespace Graphics
