//
//  CUIMaterialEditorColors.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/03/13.
//
//

#ifndef __C_UI_MATERIAL_EDITOR_COLORS_H_INCLUDED__
#define __C_UI_MATERIAL_EDITOR_COLORS_H_INCLUDED__

#include "../../../Device/CDevices.h"

class CUIMaterialEditorColors : public IEventReceiver {
    
public:
    
    CUIMaterialEditorColors(CDevices *_devices, IGUIElement *_parent, ISceneNode *node);
    ~CUIMaterialEditorColors();
    
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
	IGUIStaticText *diffuseColorPreview;
	IGUIEditBox *diffuseRed, *diffuseGreen, *diffuseBlue, *diffuseAlpha;
	IGUIScrollBar *diffuseRedS, *diffuseGreenS, *diffuseBlueS, *diffuseAlphaS;

	IGUIStaticText *emissiveColorPreview;
	IGUIEditBox *emissiveRed, *emissiveGreen, *emissiveBlue, *emissiveAlpha;
	IGUIScrollBar *emissiveRedS, *emissiveGreenS, *emissiveBlueS, *emissiveAlphaS;

	IGUIStaticText *specularColorPreview;
	IGUIEditBox *specularRed, *specularGreen, *specularBlue, *specularAlpha;
	IGUIScrollBar *specularRedS, *specularGreenS, *specularBlueS, *specularAlphaS;

	IGUIStaticText *ambiantColorPreview;
	IGUIEditBox *ambiantRed, *ambiantGreen, *ambiantBlue, *ambiantAlpha;
	IGUIScrollBar *ambiantRedS, *ambiantGreenS, *ambiantBlueS, *ambiantAlphaS;
    //-----------------------------------
    
};

#endif
