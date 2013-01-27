//
//  CUIEditEffects.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 20/11/12.
//
//

#ifndef __C_UI_WINDOW_EDIT_EFFECTS_H_INCLUDED__
#define __C_UI_WINDOW_EDIT_EFFECTS_H_INCLUDED__

enum CXT_EDIT_WINDOW_EFFECTS_EVENTS {
    CXT_EDIT_WINDOW_EFFECTS_EVENTS_OPEN = 0x17000,
    CXT_EDIT_WINDOW_EFFECTS_EVENTS_CLOSE,
};

#include "../../../Device/CDevices.h"
#include "../../../GUI Extension/ViewPort/CGUIViewPort.h"

class CUIWindowEditEffects : public IEventReceiver {
   
public:
    
    CUIWindowEditEffects(CDevices *_devices);
    ~CUIWindowEditEffects();
    
    void open();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    CDevices *devices;
    
    //-----------------------------------
    //MAIN WINDOW
    IGUIViewport *viewPort;
    ISceneManager *smgr;
    
    IGUIWindow *effectsWindow;
    
    IGUIContextMenu *menu, *submenu;
    
    IGUIListBox *shadersList;
    IGUICheckBox *active;
    IGUIButton *add, *remove, *editCallBack, *close;
    //-----------------------------------
    
    //-----------------------------------
    //CALLBACKS WINDOW
    IGUIWindow *editionWindow;
    
    IGUIButton *pApplyButton;
    IGUIEditBox *pvalue, *pname;
    IGUIButton *pAdd, *pRemove;
    IGUIListBox *pList;
    
    IGUIButton *vApplyButton;
    IGUIEditBox *vvalue, *vname;
    IGUIButton *vAdd, *vRemove;
    IGUIListBox *vList;
    
    IGUIButton *closeButton;
    //-----------------------------------
    
    bool openingShader;
    
};

#endif
