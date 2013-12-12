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

#include "../../GUIExtension/ViewPort/IGUIViewPort.h"
#include "../../GUIExtension/ViewPort/CGUIViewPort.h"

#include "CMaterialEditorFactory.h"

#include "General/CUIMaterialEditorTextures.h"
#include "General/CUIMaterialEditorColors.h"
#include "General/CUIMaterialEditorFlags.h"

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
    
	CMaterialEditorFactory *mtFactory;

    CUIMaterialEditorTextures *matEditorTextures;
	CUIMaterialEditorColors *matEditorColors;
	CUIMaterialEditorFlags *matEditorFlags;
    
    array<IGUIScrollBar *> scbars;
	array< array<position2di> > initialElementsPosition;
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
	IGUIImageList* imageList;
    IGUITreeView *materialsView;
    IGUITreeViewNode *rootTreeViewNode;
    
    //VIEW PORT
    IGUIViewport *viewPort;
	//IGUIImage *viewPort;

	//SEPARATOR
	IGUIStaticText *separatorText;
    
    //TABS
    IGUITabControl *tabctrl;
    IGUITab *texturesTab, *colorsTab, *flagsTab;
    
    //BUTTONS
    IGUIButton *close;
    //-----------------------------------
    
	//-----------------------------------
	//METHODS
	void updateElementsPositionsI(IGUIScrollBar *scroolbar);

	void update();

	void maximize();
	void minimize();
	//-----------------------------------
};

#endif
