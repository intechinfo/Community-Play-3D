//
//  CUIMaterialEditor.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/03/13.
//
//

#ifndef __C_UI_MATERIAL_EDITOR_FACTORY_H_INCLUDED__
#define __C_UI_MATERIAL_EDITOR_FACTORY_H_INCLUDED__

#include "../../Device/CDevices.h"

class CMaterialEditorFactory {
  
public:
    
    CMaterialEditorFactory(CDevices *_devices);
    ~CMaterialEditorFactory();

	void setParentGUIElement(IGUIElement *parent) { guiParent = parent; }

	//-----------------------------------
	//METHODS
	void reupdateTreeView(ISceneNode *node, IGUITreeView *materialsList, IGUITreeViewNode *rootTreeViewNode, IGUIImageList *imageList);

	void setCreateAllTextureLayer2NormalMapped(ISceneNode *node);
	void setAllTextureLayer2NormalMapped(ISceneNode *node, f32 factor);

	ITexture *copyTexture(stringc nameOfTexture, ITexture *texture);
	//-----------------------------------

    
private:
    
    //-----------------------------------
	//DEVICES ELEMENTS AND DATAS
    CDevices *devices;
	IGUIElement *guiParent;
    
    ISceneManager *smgr;
    IVideoDriver *driver;
    IGUIEnvironment *gui;
    //-----------------------------------
};

#endif
