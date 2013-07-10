//
//  CUIWindowEditVolumeLight.h
//  Windows
//
//  Created by Arthur Caron on 10/06/13.
//
//

#ifndef __C_UI_WINDOW_EDIT_VOLUME_LIGHT_H_INCLUDED__
#define __C_UI_WINDOW_EDIT_VOLUME_LIGHT_H_INCLUDED__

#include "../Device/CDevices.h"

#include "CUIWindowEditNode.h"

class CUIWindowEditVolumeLight : public IEventReceiver {
    
public:
    
    CUIWindowEditVolumeLight(CDevices *_devices);
    ~CUIWindowEditVolumeLight();
    
    void open(ISceneNode *node, stringw prefix);
    
    bool OnEvent(const SEvent &event);
    
private:
    
    CDevices *devices;

	bool isValid();
    
    //-----------------------------------
    //EDIT WINDOW USER INTERFACE ELEMENTS
    IGUIWindow *editWindow;
    
    IGUIEditBox *volumeLightNameEditBox;
	IGUIComboBox *volumeLightParentComboBox;
	IGUIEditBox *volumeLightSubdivUEditBox;
	IGUIEditBox *volumeLightSubdivVEditBox;
	IGUIEditBox *volumeLightAFootColorEditBox;
	IGUIEditBox *volumeLightRFootColorEditBox;
	IGUIEditBox *volumeLightGFootColorEditBox;
	IGUIEditBox *volumeLightBFootColorEditBox;
	IGUIEditBox *volumeLightATailColorEditBox;
	IGUIEditBox *volumeLightRTailColorEditBox;
	IGUIEditBox *volumeLightGTailColorEditBox;
	IGUIEditBox *volumeLightBTailColorEditBox;

    IGUIButton *applyButton, *closeButton;

	stringw name;
	u32 subdivU;
	u32 subdivV;
	u32 aFootColor;
	u32 rFootColor;
	u32 gFootColor;
	u32 bFootColor;
	u32 aTailColor;
	u32 rTailColor;
	u32 gTailColor;
	u32 bTailColor;
    //-----------------------------------
    
    //-----------------------------------
    //EDITING NODE INFORMATIONS
    IVolumeLightSceneNode *nodeToEdit;
    stringw nodeToEditPrefix;
    
    u32 currentBrowse;
    //-----------------------------------
    
};

#endif
