//
//  CUICharacterWindow.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 10/10/12.
//
//

#ifndef __C_UI_CHARECTER_WINDOW_H_INCLUDED__
#define __C_UI_CHARECTER_WINDOW_H_INCLUDED__

enum CXT_EDIT_WINDOW_CHARACTER_EVENTS {
	CXT_EDIT_WINDOW_CHARACTER_EVENTS_OPEN = 0x12000,
	CXT_EDIT_WINDOW_CHARACTER_EVENTS_EDIT,
	CXT_EDIT_WINDOW_CHARACTER_EVENTS_SAVE,
	CXT_EDIT_WINDOW_CHARACTER_EVENTS_CLOSE,

	CXT_EDIT_WINDOW_CHARACTER_EVENTS_ENTER_BONES_EDITION,

	CXT_EDIT_WINDOW_CHARACTER_EVENTS_ADD,
	CXT_EDIT_WINDOW_CHARACTER_EVENTS_DELETE,

	CXT_EDIT_WINDOW_CHARACTER_EVENTS_PREVIEW,

	CXT_EDIT_WINDOW_CHARACTER_EVENTS_APPLY_SPEED,

	CXT_EDIT_WINDOW_CHARACTER_EVENTS_ROTATION_SB,

	CXT_EDIT_WINDOW_CHARACTER_EVENTS_CLOSE_BONES_EDITION_MODE
};

#include "../Device/CDevices.h"

#include "../UserInterfaces/CUIWindowEditNode.h"
#include "../GUIExtension/ViewPort/CGUIViewPort.h"

#include "CUIEditBones.h"

class CUICharacterWindow : public IEventReceiver {

public:

	CUICharacterWindow(CDevices *_devices);
	~CUICharacterWindow();

	void open();
	void setModel(IAnimatedMeshSceneNode *pnode, s32 index);

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
	//DEVICE ELEMENTS
	CDevices *devices;

	IGUIViewport *viewPort;

	ISceneManager *smgr;
	CGridSceneNode *grid;
	IBoneSceneNode *lastBoneSceneNode;
	ICameraSceneNode *camera, *cameraMaya;

	CUIEditBones *editBones;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	IGUIWindow *characterWindow;

	IGUIContextMenu *menu, *submenu;
	IGUIToolBar *bar, *infosBar;

	IGUIScrollBar *rotationScrollBar;

	IGUIStaticText *actionsText;
	IGUIListBox *animationList;
	IGUIButton *add, *remove;

	IGUITabControl *tabCtrl;

	IGUITab *detailsTab;
	IGUIEditBox *animationSpeed;
	IGUIButton *applyAnimationSpeed;

	IGUITab *actionsTab;
	IGUIButton *previewAction;
	IGUIEditBox *endAction, *startAction, *nameAction;
	IGUIEditBox *speedAction;

	IGUITab *framesTab;
	IGUIListBox *framesListBox;

	IGUIFileOpenDialog *saveDialog;

	ISceneNode *boneNode;
	IGUIStaticText *bgJointNodes;
	IGUIListBox *jointNodesListBox;

	//-----------------------------------

	//-----------------------------------
	//USED VALUES
	int currentLoad;
    
	array<CAction *> actions;
    
	ILightSceneNode* light;
	IAnimatedMeshSceneNode *node;
	//-----------------------------------

	//-----------------------------------
	//METHODS
	void exportAnimatedModel();
	//-----------------------------------
};

#endif
