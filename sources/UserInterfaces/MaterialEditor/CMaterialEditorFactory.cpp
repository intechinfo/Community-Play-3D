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
	
	materialsList->getRoot()->clearChilds();
	
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

void CMaterialEditorFactory::setCreateAllTextureLayer2NormalMapped() {
	//CREATE PROCESS
	CProcess *process = new CProcess(devices->getGUIEnvironment(), "CREATE AND NORMAL MAP TEXTURES");
	devices->getProcessesLogger()->addProcess(process);

	E_SHADOW_MODE shadowMode = devices->getXEffect()->getNodeShadowMode(node, devices->getXEffectFilterType());
	devices->getXEffect()->removeShadowFromNode(node);

	for (u32 i=0; i < node->getMaterialCount(); i++) {
		if (node->getMaterial(i).TextureLayer[0].Texture) {
			stringc texname = node->getMaterial(i).TextureLayer[0].Texture->getName().getPath().c_str();
			stringc texmname = stringc(texname + stringc("_copy")).c_str();

			if (devices->getCore()->textureAlreadyExists(texmname, devices->getVideoDriver()) == -1) {
				ITexture *texm = node->getMaterial(i).TextureLayer[0].Texture;
				ITexture *tex = copyTexture(texmname.c_str(), texm);
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
	devices->getXEffect()->addShadowToNode(node, devices->getXEffectFilterType(), shadowMode);
}

void CMaterialEditorFactory::setAllTextureLayer2NormalMapped(f32 factor) {
	//CREATE PROCESS
	CProcess *process = new CProcess(devices->getGUIEnvironment(), "NORMAL MAPPING TEXTURES");
	devices->getProcessesLogger()->addProcess(process);

	E_SHADOW_MODE shadowMode = devices->getXEffect()->getNodeShadowMode(node, devices->getXEffectFilterType());
	devices->getXEffect()->removeShadowFromNode(node);

	for (u32 i=0; i < node->getMaterialCount(); i++) {
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
	devices->getXEffect()->addShadowToNode(node, devices->getXEffectFilterType(), shadowMode);
}

ITexture *CMaterialEditorFactory::copyTexture(stringc nameOfTexture, ITexture *texture) {
	ITexture *tex = devices->getVideoDriver()->addTexture(dimension2du(texture->getOriginalSize().Width, texture->getOriginalSize().Height), 
														  nameOfTexture.c_str());
	u8* pixelsTex = (u8 *)tex->lock();
	u8 *pixelsTexm = (u8 *)texture->lock();
	for(u32 ui = 0; ui < tex->getOriginalSize().Width * tex->getOriginalSize().Height; ui++) {
		for (u32 uj=0; uj < 3; uj++) {
			*pixelsTex = *pixelsTexm;
			pixelsTex++; pixelsTexm++;
		}
		pixelsTex++; pixelsTexm++;
	}
	tex->unlock();
	texture->unlock();

	return tex;
}
