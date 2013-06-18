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

	IGUIStaticText *mainArea;

	IGUIButton *addButton;
	IGUIButton *removeButton;
    IGUIButton *applyButton;
	IGUIButton *closeButton;

    //-----------------------------------
    //EDITING NODE INFORMATIONS
    ISceneNode *nodeToEdit;
    stringw nodeToEditPrefix;
    
    u32 currentBrowse;
    //-----------------------------------
    
};

#endif
