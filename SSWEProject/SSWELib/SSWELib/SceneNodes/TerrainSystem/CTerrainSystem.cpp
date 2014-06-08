#include "stdafx.h"

#include "CTerrainSystem.h"

using namespace irr;
using namespace video;
using namespace core;
using namespace scene;

CTerrainSystem::CTerrainSystem(ISceneManager *smgr, stringc hgtmap, u32 divFactor)
	: ISceneNode(0, smgr, -1)
{
	/*
	METHOD:
		- CREATE AN IMAGE FROM FILE hgtmap
		- CREATE AN ARRAY OF IMAGES THAT WILL CONTAIN THE SEPARATED HEIGHT MAPS (divFactor IMAGES)
			-DRAW A BLACK RECTANGLE FOR CREATING SKIRTS
		- ADD A NEW TERRAIN FOR EACH SPLITTED IMAGES
		- ADD EACH TERRAIN TO THE TERRAIN LIST
	*/


	//CREATE THE HEIGHT MAP IMAGE
	io::IReadFile *file = smgr->getFileSystem()->createAndOpenFile(hgtmap);
	IImage *hgtmapImage = smgr->getVideoDriver()->createImageFromFile(file);
	dimension2du hgtmapImageDimensions = hgtmapImage->getDimension();
	u32 pixelsPerTerrain = (hgtmapImageDimensions.Width * hgtmapImageDimensions.Height)/divFactor;
	dimension2du hgtmapsDimensions = dimension2du(pixelsPerTerrain+2, pixelsPerTerrain+2);

	//CREATE divFactor IMAGES
	array<IImage *> images;
	images.set_used(0);

	for (u32 i=0; i < divFactor; i++) {
		IImage* tmpImage = smgr->getVideoDriver()->createImage(ECF_A8R8G8B8, hgtmapsDimensions);
		tmpImage->fill(SColor(255, 0, 0, 0));
		images.push_back(tmpImage);
	}

	//FILL THE SEPARATED IMAGES
	for (u32 i=0; i < images.size(); i++) {
		for (u32 u=1; u < images[i]->getDimension().Width-1; u++) {
			for (u32 v=1; v < images[i]->getDimension().Height-1; v++) {
				images[i]->setPixel(u, v, hgtmapImage->getPixel((hgtmapsDimensions.Width*i)+u, (hgtmapsDimensions.Height*i)+v), false);
			}
		}
	}

	for (u32 i=0; i < images.size(); i++) {
		stringc imageName = "Big Height Maps/Test/image_";
		imageName += i;
		imageName += ".png";

		printf("%s\n", imageName.c_str());
		smgr->getVideoDriver()->writeImageToFile(images[i], imageName);
	}
}

CTerrainSystem::~CTerrainSystem() {

    #ifndef _IRR_LINUX_PLATFORM_
	ISceneNode::~ISceneNode();
	#endif
}

void CTerrainSystem::OnRegisterSceneNode() {

	ISceneNode::OnRegisterSceneNode();
}
void CTerrainSystem::render() {

}
