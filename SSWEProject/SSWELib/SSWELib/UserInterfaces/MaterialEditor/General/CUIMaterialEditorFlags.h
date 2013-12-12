//
//  CUIMaterialEditorColors.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/03/13.
//
//

#ifndef __C_UI_MATERIAL_EDITOR_FLAGS_H_INCLUDED__
#define __C_UI_MATERIAL_EDITOR_FLAGS_H_INCLUDED__

#include "../../../Device/CDevices.h"

class CUIMaterialEditorFlags : public IEventReceiver {
    
public:
    
    CUIMaterialEditorFlags(CDevices *_devices, IGUIElement *_parent, ISceneNode *node);
    ~CUIMaterialEditorFlags();
    
    void update(SMaterial *material);
    
    bool OnEvent(const SEvent &event);
    
private:
    
    //-----------------------------------
	//DEVICES ELEMENTS AND DATAS
    CDevices *devices;
    
    ISceneManager *smgr;
    IVideoDriver *driver;
    IGUIEnvironment *gui;
    
    ISceneNode *nodeToEdit;
    SMaterial *materialToEdit;
    //-----------------------------------
    
    //-----------------------------------
	//GUI ELEMENTS
	IGUIElement *parent;

	//GENERAL
	IGUICheckBox *anisotropicFilter, *bilinearFilter, *trilinearFilter, *antialiasingFilter;

	IGUICheckBox *backFaceCulling, *frontFaceCulling;

	IGUICheckBox *colorMask, *colorMaterial, *textureWrap;
	IGUICheckBox *zbuffer, *zwriteEnable, *enableFog;

	IGUIEditBox *materialTypeParam1, *materialTypeParam2;
	IGUIEditBox *shininess, *thickness;
    //-----------------------------------
    
};

#endif
