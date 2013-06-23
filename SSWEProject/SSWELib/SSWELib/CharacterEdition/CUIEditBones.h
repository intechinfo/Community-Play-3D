//
//  CUICharacterWindow.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 10/10/12.
//
//

#ifndef __C_UI_EDIT_BONES_H_INCLUDED__
#define __C_UI_EDIT_BONES_H_INCLUDED__

#include "../Device/CDevices.h"
#include "../UserInterfaces/CUIWindowEditNode.h"

#include "ManualAnimation/CFrame.h"

class CUIEditBones : public IEventReceiver {

public:

	CUIEditBones(CDevices *_devices, IAnimatedMeshSceneNode *_node);
	~CUIEditBones();

	void open();
	void close();

	bool OnEvent(const SEvent &event);

	IGUIButton *getCloseButton() { return bonesCloseButton; }

private:

	//-----------------------------------
	//DEVICE ELEMENTS
	CDevices *devices;
	EffectHandler *effect;

	IVideoDriver *driver;
	ISceneManager *smgr;
	IGUIEnvironment *gui;

	IBoneSceneNode *lastBoneSceneNode;

	IAnimatedMeshSceneNode *node;
	//-----------------------------------

	//-----------------------------------
	//METHODS
	void resetBonesWindow();

	void previewManualAction();
	void previewEntireFrame();

	void update();
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	//CONTEXT MENU
	IGUIContextMenu *menu;
	IGUIContextMenu *filem, *editm, *bonesm, *helpm;

	//MAIN WINDOW
	IGUIWindow *mainWindow;
	IGUIButton *addFrame, *removeFrame;
	IGUIListBox *frames;
	IGUIButton *previewFrame;
	IGUITreeView *animations;
	IGUITreeViewNode *rootAnimations;
	IGUIButton *addAnimation, *removeAnimation;

	//ANIMATIONS PARAMS WINDOW
	IGUIWindow *bonesWindow;

	IGUIListBox *bonesListBox;

	IGUIListBox *manualActionslb;
	IGUIButton *addManualAction, *removeManualActions;

	IGUIEditBox *nameeb;
	IGUIComboBox *typecb;
	IGUIComboBox *senscb;
	IGUIEditBox *timeToStartcb, *timeToCompletecb;

	IGUIButton *addPredecessor, *removePredecessor, *editBehavior;
	IGUIListBox *predecessors;

	IGUIStaticText *startInfo, *endInfo;

	IGUIButton *preview;

	IGUIButton *bonesCloseButton;

	//PREDECESSORS WINDOW
	IGUIWindow *predecessorsWindow;
	IGUITreeView *predecessorstv;
	IGUITreeViewNode *rootPredecessors;
	IGUIButton *cancelPredecessors, *acceptPredecessors;
	//-----------------------------------

	//-----------------------------------
	//DATAS
	array<CFrame *> arrayFrames;

	CFrame *selectedFrame;
	CAnimation *selectedAnimation;
	CManualAction *selectedManualAction;

	bool updateView;
	//-----------------------------------
};

#endif
