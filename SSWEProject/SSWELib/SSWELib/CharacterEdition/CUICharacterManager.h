//
//  CUICharacterWindow.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 10/10/12.
//
//

#ifndef __C_UI_CHARECTER_MANAGER_H_INCLUDED__
#define __C_UI_CHARECTER_MANAGER_H_INCLUDED__

#include "../Device/CDevices.h"

#include "../GUIExtension/ViewPort/CGUIViewPort.h"

enum CXT_CHARACTER_MANAGER_MENU_ITEM_ID {
	CXT_CHARACTER_MANAGER_MENU_SAVE = 0x12000,
	CXT_CHARACTER_MANAGER_MENU_CLOSE
};

class CUICharacterManager : public IEventReceiver {

public:

	CUICharacterManager(CDevices *_devices, IAnimatedMesh *mesh);
	~CUICharacterManager();

	void open();
	void setModel(IAnimatedMeshSceneNode *pnode);

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
	//DEVICE ELEMENTS
	CDevices *devices;

	IGUIViewport *viewPort;

	ISceneManager *smgr;
	CGridSceneNode *grid;
	ICameraSceneNode *camera;

	IAnimatedMesh *mesh;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	IGUIWindow *characterWindow;
	IGUIContextMenu *menu, *submenu;
	IGUIToolBar *bar, *infosBar;

	//ACTION LIST
	IGUIStaticText *actionsText;
	IGUIListBox *animationList;
	IGUIButton *add, *remove;

	IGUITabControl *tabCtrl;
	// DETAILS
	IGUITab *detailsTab;
	IGUIEditBox *animationSpeed;
	IGUIButton *applyAnimationSpeed;

	//ACTION
	IGUITab *actionsTab;
	IGUIButton *previewAction;
	IGUIEditBox *endAction, *startAction, *nameAction;
	IGUIEditBox *speedAction;
	IGUIComboBox *selectedAnimationcb;

	//ANIMATIONS
	IGUITab *animationsTab;
	IGUIListBox *animationslb;
	IGUIButton *addAnimationb, *deleteAnimationb;

	//DIALOGS
	IGUIFileOpenDialog *saveDialog;
	IGUIFileOpenDialog *openMeshSkinned;
	IGUIFileOpenDialog *addAnimationDialog;
	//-----------------------------------

	//-----------------------------------
	//USED VALUES
	array<CAction *> actions;
	array<IAnimatedMesh *> meshes;
    
	ILightSceneNode* light;
	IAnimatedMeshSceneNode *node;
	//-----------------------------------

	//-----------------------------------
	//METHODS
	void exportAnimatedModel();

	void setFullScreen();

	void updateDetails();
	//-----------------------------------
};

#endif
