//
//  CUIMaterialEditorGeneral.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/03/13.
//
//

#ifndef __C_UI_MATERIAL_EDITOR_TEXTURES_H_INCLUDED__
#define __C_UI_MATERIAL_EDITOR_TEXTURES_H_INCLUDED__

#include "../../../Device/CDevices.h"

class CUIMaterialEditorTextures : public IEventReceiver {
    
public:
    
    CUIMaterialEditorTextures(CDevices *_devices, IGUIElement *_parent, ISceneNode *node);
    ~CUIMaterialEditorTextures();
    
    void update(SMaterial *material);
    
    bool OnEvent(const SEvent &event);
    
private:
    
    //-----------------------------------
	//METHODS
    void setTexture(u32 id, stringw file_path, IGUIImage *image, IGUIEditBox *editBox);
    u32 getSelectedLayer();
    //-----------------------------------
    
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
    IGUIEditBox *gtex1, *gtex2, *gtex3, *gtex4;
    IGUIButton *gltex1, *gltex2, *gltex3, *gltex4;
    IGUIButton *gdtex1, *gdtex2, *gdtex3, *gdtex4;
    IGUIFileOpenDialog *gbtex1, *gbtex2, *gbtex3, *gbtex4;
    IGUIImage *gitex1, *gitex2, *gitex3, *gitex4;
    
    //ADVANCED
    IGUIButton *layer1, *layer2, *layer3, *layer4;
    IGUIComboBox *textureWrapUChoice, *textureWrapVChoice;
    IGUIComboBox *anisotropicFilterChoice, *lodBiasChoice;
    IGUIImage *layerPreview;
    IGUIButton *regenerateBipMapsLevels;
    
    IGUIEditBox *loadTemplatePath;
    IGUIButton *browseLoadTemplate, *saveTemplate;
    IGUIFileOpenDialog *loadTemplateFileOpenDialog, *saveTemplateFileOpenDialog;
    
    //DIALOGS
    IGUIFileOpenDialog *gfodtex1, *gfodtex2, *gfodtex3, *gfodtex4;
    //-----------------------------------
    
};

#endif
