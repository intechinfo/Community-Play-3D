//
//  CUIWindowEditNode.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 29/09/12.
//
//

#ifndef __C_UI_WINDOW_EDIT_LIGHT_H_INCLUDED__
#define __C_UI_WINDOW_EDIT_LIGHT_H_INCLUDED__

#include "../Device/CDevices.h"

#include "CUIWindowEditNode.h"

enum CXT_EDIT_LIGHT_WINDOW_EVENTS {
    CXT_EDIT_LIGHT_WINDOW_EVENTS_GENERAL_POSITION = 0x80000,
    CXT_EDIT_LIGHT_WINDOW_EVENTS_GENERAL_TARGET,
    
    CXT_EDIT_LIGHT_WINDOW_EVENTS_ADVANCED_RESOLUTION,
    CXT_EDIT_LIGHT_WINDOW_EVENTS_ADVANCED_DIFFUSE_COLOR,
    
    CXT_EDIT_LIGHT_WINDOW_EVENTS_APPLY_BUTTON,
    CXT_EDIT_LIGHT_WINDOW_EVENTS_CLOSE_BUTTON
};

class CUIWindowEditLight : public IEventReceiver {
    
public:
    
    CUIWindowEditLight(CDevices *_devices, s32 _index);
    ~CUIWindowEditLight();
    
    void open(ISceneNode *node, stringw prefix);
    
    bool OnEvent(const SEvent &event);
    
private:
    
    CDevices *devices;
    s32 index;
    
    //-----------------------------------
    //EDIT WINDOW USER INTERFACE ELEMENTS
    IGUIWindow *editWindow;
    IGUITabControl *tabCtrl;
    
    IGUITab *generalTab, *advancedTab, *lensFlareTab, *shadowLightTab;
    
    IGUIButton *applyButton, *closeButton;
    
    IGUIEditBox *ebNodeName;
    IGUIEditBox *ebNodePositionX, *ebNodePositionY, *ebNodePositionZ;
    IGUIEditBox *ebNodeTargetX, *ebNodeTargetY, *ebNodeTargetZ;
    IGUIEditBox *ebNodeRadius;
    
    IGUIComboBox *resolutionComboBox;
    IGUIEditBox *ebNodeDiffuseColorR, *ebNodeDiffuseColorG, *ebNodeDiffuseColorB, *ebNodeDiffuseColorA;
    IGUIEditBox *ebNodeAmbiantColorR, *ebNodeAmbiantColorG, *ebNodeAmbiantColorB, *ebNodeAmbiantColorA;
    IGUIEditBox *ebNodeSpecularColorR, *ebNodeSpecularColorG, *ebNodeSpecularColorB, *ebNodeSpecularColorA;
    
    IGUICheckBox *lensFlare;
    IGUIEditBox *lfStrength;
    IGUISpinBox *sphereXYZ;
    IGUIEditBox *sphereX, *sphereY, *sphereZ;
    IGUIEditBox *sphereTexture;
	IGUIButton *editBillBoardLensFlare;
    IGUIButton *sphereTextureBrowse;
    IGUIEditBox *bbW, *bbH;
    IGUISpinBox *bbWH;
    IGUIEditBox *bbTexture;
    IGUIButton *bbTextureBrowse;
    IGUIEditBox *lfnTexture;
    IGUIButton *lfnTextureBrowse;
    IGUIButton *lfnCursorPosition, *lfn0Position;
    IGUIButton *lfnArrowMeshPosition;

	IGUIEditBox *farValueSL;
	IGUIImage *shadowMapPreview, *shadowMapPreview2;
	IGUICheckBox *autoRecalculate;
	IGUICheckBox *isTorchMode;
    //-----------------------------------
    
    //-----------------------------------
    //EDITING NODE INFORMATIONS
    ILightSceneNode *nodeToEdit;
    stringw nodeToEditPrefix;
    
    u32 currentBrowse;
    //-----------------------------------
    
};

#endif
