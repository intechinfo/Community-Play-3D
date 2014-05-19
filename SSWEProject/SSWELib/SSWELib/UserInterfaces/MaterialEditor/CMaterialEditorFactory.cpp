//
//  CUIMaterialEditor.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/03/13.
//
//
#include "stdafx.h"
#include "CMaterialEditorFactory.h"

CMaterialEditorFactory::CMaterialEditorFactory(CDevices *_devices) {
	devices = _devices;

	driver = devices->getVideoDriver();
	smgr = devices->getSceneManager();

	guiParent = 0;
}

CMaterialEditorFactory::~CMaterialEditorFactory() {

}

void CMaterialEditorFactory::reupdateTreeView(ISceneNode *node, IGUITreeView *materialsList, IGUITreeViewNode *rootTreeViewNode, IGUIImageList *imageList) {
	
	//materialsList->getRoot()->clearChilds();
	materialsList->getRoot()->clearChildren();
	
	if (imageList) {
		materialsList->setImageList(imageList);
	}
    
    rootTreeViewNode = materialsList->getRoot()->addChildBack((stringw("Materials (") + stringw(node->getMaterialCount())
                                                              + stringw(")")).c_str(), 0);
    rootTreeViewNode->setExpanded(true);
    for (u32 i=0; i < node->getMaterialCount(); i++) {
        stringw name = L"Material ";
        name += i+1;
        IGUITreeViewNode *treeNode = rootTreeViewNode->addChildBack(name.c_str(), 0, 10);
        treeNode->setData(&node->getMaterial(i));
    }
    rootTreeViewNode->getFirstChild()->setSelected(true);
}

ITexture *CMaterialEditorFactory::setTextureNormalMapped(stringc nameOfTexture, ITexture *originalTexture) {
	ITexture *tex = copyTexture(nameOfTexture, originalTexture);
	devices->getVideoDriver()->makeNormalMapTexture(tex, 9.0f);
	return tex;
}

void CMaterialEditorFactory::setCreateAllTextureLayer2NormalMapped() {
	//CREATE PROCESS
	CProcess *process = new CProcess(devices->getGUIEnvironment(), "CREATE AND NORMAL MAP TEXTURES");
	devices->getProcessesLogger()->addProcess(process);
    
	E_SHADOW_MODE shadowMode = devices->getXEffect()->getNodeShadowMode(node, devices->getXEffectFilterType());
	devices->getXEffect()->removeShadowFromNode(node);
	devices->getXEffect()->addShadowToNode(node, devices->getXEffectFilterType(), ESM_EXCLUDE);

	#pragma omp for schedule(dynamic)
	for (s32 i=0; i < node->getMaterialCount(); i++) {
        #ifdef _IRR_OSX_PLATFORM_
        devices->reupdate();
        #endif
		if (node->getMaterial(i).TextureLayer[0].Texture) {

			if (node->getMaterial(i).TextureLayer[0].Texture->isRenderTarget())
				continue;

			stringc texname = node->getMaterial(i).TextureLayer[0].Texture->getName().getPath().c_str();
			stringc texmname = stringc(texname + stringc("._normal_mapped")).c_str();
            
			if (devices->getCore()->textureAlreadyExists(texmname, devices->getVideoDriver()) == -1) {
				ITexture *texm = node->getMaterial(i).TextureLayer[0].Texture;

				ITexture *tex;
				if (node->getMaterial(i).TextureLayer[1].Texture)
					tex = node->getMaterial(i).TextureLayer[1].Texture;
				else
					tex = copyTexture(texmname.c_str(), texm);

				devices->getVideoDriver()->makeNormalMapTexture(tex, 9.0f);
				node->getMaterial(i).setTexture(0, texm);
				node->getMaterial(i).setTexture(1, tex);
			} else {
				node->getMaterial(i).setTexture(1, driver->getTextureByIndex(devices->getCore()->textureAlreadyExists(texmname, devices->getVideoDriver())));
			}
			process->getProgressBar()->setPercentage((100*i)/node->getMaterialCount());
		}
	}

	//SET PROCESS HAS FINISHED
	process->setHasFinished(true);
	devices->getXEffect()->removeShadowFromNode(node);
	devices->getXEffect()->addShadowToNode(node, devices->getXEffectFilterType(), shadowMode);
}

void CMaterialEditorFactory::setAllTextureLayer2NormalMapped(f32 factor) {
	//CREATE PROCESS
	CProcess *process = new CProcess(devices->getGUIEnvironment(), "NORMAL MAPPING TEXTURES");
	devices->getProcessesLogger()->addProcess(process);

	E_SHADOW_MODE shadowMode = devices->getXEffect()->getNodeShadowMode(node, devices->getXEffectFilterType());
	devices->getXEffect()->removeShadowFromNode(node);
	devices->getXEffect()->addShadowToNode(node, devices->getXEffectFilterType(), ESM_EXCLUDE);

	#pragma omp for schedule(dynamic)
	for (s32 i=0; i < node->getMaterialCount(); i++) {
        #ifdef _IRR_OSX_PLATFORM_
        devices->reupdate();
        #endif
		if (node->getMaterial(i).TextureLayer[0].Texture) {
			while (node->getMaterial(i).TextureLayer[0].Texture->isRenderTarget());
		}

		if (node->getMaterial(i).TextureLayer[1].Texture) {
			devices->getVideoDriver()->makeNormalMapTexture(node->getMaterial(i).TextureLayer[1].Texture, factor);
		}
		process->getProgressBar()->setPercentage((100*i)/node->getMaterialCount());
	}

	//SET PROCESS HAS FINISHED
	process->setHasFinished(true);
	devices->getXEffect()->removeShadowFromNode(node);
	devices->getXEffect()->addShadowToNode(node, devices->getXEffectFilterType(), shadowMode);
}
 
ITexture *CMaterialEditorFactory::copyTexture(stringc nameOfTexture, ITexture *texture) {

	IImage *image = driver->createImage(texture, position2di(0, 0), texture->getOriginalSize());
                    
    for (u32 i=0; i < image->getDimension().Width; i++) {
        for (u32 j=0; j < image->getDimension().Height; j++ ) {
            SColor pixel = image->getPixel(i, j);
            u32 red = pixel.getRed();
            u32 green = pixel.getGreen();
            u32 blue = pixel.getBlue();
            f32 average = 0.3*red + 0.59*green + 0.11*blue;

            if (average > 100) {
                //average = 192;
            }
			if (average < 50) {
				//average = 0;
			}
                            
            image->setPixel(i, j, SColor(255, average, average, average));
        }
    }

	ITexture *tex = devices->getVideoDriver()->addTexture(nameOfTexture.c_str(), image);

	return tex;
}
