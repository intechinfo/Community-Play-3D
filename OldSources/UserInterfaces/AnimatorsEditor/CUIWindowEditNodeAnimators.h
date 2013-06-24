//
//  CUIWindowEditNodeAnimators.h
//  Windows
//
//  Created by Arthur Caron on 12/06/13.
//
//

#ifndef __C_UI_WINDOW_EDIT_NODE_ANIMATORS_H_INCLUDED__
#define __C_UI_WINDOW_EDIT_NODE_ANIMATORS_H_INCLUDED__

#include "../../Device/CDevices.h"

#include "../CUIWindowEditNode.h"

class CUIWindowEditNodeAnimators : public IEventReceiver {
    
public:
    
    CUIWindowEditNodeAnimators(CDevices *_devices);
    ~CUIWindowEditNodeAnimators();
    
	stringw getAnimatorNameById(u32 enumIndex);
	u32 getAnimatorIdByName(stringw name);
	void fillMainArea();
	void saveChanges();
    void open(ISceneNode *node, stringw prefix);
	bool isValid();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    CDevices *devices;

    //-----------------------------------
    //EDIT WINDOW USER INTERFACE ELEMENTS
    IGUIWindow *editWindow;
    
	IGUIComboBox *nodeSelectedComboBox;
	IGUIComboBox *animatorSelectedComboBox;

	IGUIListBox *nodeAnimatorsListBox;

	IGUIStaticText *mainArea;

	IGUIButton *addButton;
	IGUIButton *removeButton;
    IGUIButton *applyButton;
	IGUIButton *closeButton;

	IGUIEditBox *intEditBox;
	IGUIEditBox *floatEditBox;
	IGUIEditBox *stringEditBox;
	IGUIEditBox *enumEditBox;
	IGUIEditBox *vector3dXEditBox;
	IGUIEditBox *vector3dYEditBox;
	IGUIEditBox *vector3dZEditBox;
	IGUIEditBox *position2dXEditBox;
	IGUIEditBox *position2dYEditBox;
	IGUIEditBox *vector2dXEditBox;
	IGUIEditBox *vector2dYEditBox;

    //-----------------------------------
    //EDITING NODE INFORMATIONS
    ISceneNode *nodeToEdit;
    stringw nodeToEditPrefix;
	ISceneNodeAnimator *animatorSelected;
	array<ISceneNodeAnimator*> arrayAnimators;
	array<ISceneNodeAnimator*> nodeAnimators;
	array<ISceneNode*> sceneNodeArray;
	ISceneNodeAnimatorFactory *factory;
    
    u32 currentBrowse;
    //-----------------------------------
    
};

#endif
