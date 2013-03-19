//
//  CUIMaterialEditor.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/03/13.
//
//

#ifndef __C_UI_MATERIAL_EDITOR_H_INCLUDED__
#define __C_UI_MATERIAL_EDITOR_H_INCLUDED__

#include "../../Device/CDevices.h"
#include "General/CUIMaterialEditorTextures.h"

#include "../../GUIExtension/ViewPort/CGUIViewPort.h"

class CUIMaterialEditor : public IEventReceiver {
  
public:
    
    CUIMaterialEditor(CDevices *_devices);
    ~CUIMaterialEditor();
    
    void open(ISceneNode *node);
    
    bool OnEvent(const SEvent &event);
    
private:
    
    //-----------------------------------
	//DEVICES ELEMENTS AND DATAS
    CDevices *devices;
    
    ISceneManager *smgr;
    IVideoDriver *driver;
    IGUIEnvironment *gui;
    
    ISceneNode *nodeToEdit;
    
    SMaterial *selectedMaterial, *restoreMaterial;
    
    CUIMaterialEditorTextures *matEditorTextures;
    
    array<IGUIScrollBar *> scbars;
    s32 lastScrollBarPos;
    //-----------------------------------
    
    //-----------------------------------
	//GUI ELEMENTS
    IGUIWindow *meWindow;
    SColor lastTabTextColor;
    
    //CONTEXT MENU
	IGUIContextMenu *menu;
    
    //TOOL BAR
	IGUIToolBar *bar;
    IGUIButton *toolBarOpen, *toolBarSave, *toolBarRestore;
    
    //TREE VIEW
    IGUITreeView *materialsView;
    IGUITreeViewNode *rootTreeViewNode;
    
    //VIEW PORT
    IGUIViewport *viewPort;
    
    //TABS
    IGUITabControl *tabctrl;
    IGUITab *texturesTab;
    
    //BUTTONS
    IGUIButton *close;
    //-----------------------------------
    
};

#endif
