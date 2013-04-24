//
//  CUIMaterialEditor.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/03/13.
//
//

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

void CMaterialEditorFactory::setCreateAllTextureLayer2NormalMapped(ISceneNode *node) {
	IGUIWindow *processingWindow = devices->addInformationDialog(L"Processing", L"Creating Textures...\nMapping Normal Textures...", EMBF_CANCEL, true, guiParent);
	processingWindow->getCloseButton()->remove();

	((IGUIWindow *)guiParent)->getMinimizeButton()->setVisible(false);
	((IGUIWindow *)guiParent)->getMaximizeButton()->setVisible(false);

	for (u32 i=0; i < node->getMaterialCount(); i++) {
		processingWindow->setText(stringw(stringw("Material ") + stringw(i) + stringw(" / ") + stringw(node->getMaterialCount())).c_str());
		devices->reupdate();

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
		}
	}
	processingWindow->remove();
	((IGUIWindow *)guiParent)->getMinimizeButton()->setVisible(true);
	((IGUIWindow *)guiParent)->getMaximizeButton()->setVisible(true);
}

void CMaterialEditorFactory::setAllTextureLayer2NormalMapped(ISceneNode *node, f32 factor) {
	IGUIWindow *processingWindow = devices->addInformationDialog(L"Processing", L"Mapping Normal Textures...", EMBF_CANCEL, true, guiParent);
	processingWindow->getCloseButton()->remove();

	((IGUIWindow *)guiParent)->getMinimizeButton()->setVisible(false);
	((IGUIWindow *)guiParent)->getMaximizeButton()->setVisible(false);

	for (u32 i=0; i < node->getMaterialCount(); i++) {
		processingWindow->setText(stringw(stringw("Material ") + stringw(i) + stringw(" / ") + stringw(node->getMaterialCount())).c_str());
		devices->reupdate();

		if (node->getMaterial(i).TextureLayer[1].Texture) {
			devices->getVideoDriver()->makeNormalMapTexture(node->getMaterial(i).TextureLayer[1].Texture, factor);
		}
	}
	processingWindow->remove();
	((IGUIWindow *)guiParent)->getMinimizeButton()->setVisible(true);
	((IGUIWindow *)guiParent)->getMaximizeButton()->setVisible(true);
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
