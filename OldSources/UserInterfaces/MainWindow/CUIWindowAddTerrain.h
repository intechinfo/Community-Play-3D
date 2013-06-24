//
//  CUIWindowAddOctTree.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 27/09/12.
//
//

#ifndef __C_UI_WINDOW_ADD_OCT_TREE_H_INCLUDED__
#define __C_UI_WINDOW_ADD_OCT_TREE_H_INCLUDED__

#include "../../Device/CDevices.h"

enum GUI_WINDOW_EVENTS_ADD_OCT_TREE {
    CXT_WINDOW_ADD_OCT_TREE_EVENTS_CLOSE = 0x30000,
    CXT_WINDOW_ADD_OCT_TREE_EVENTS_SELECT,
    CXT_WINDOW_ADD_OCT_TREE_EVENTS_ACCEPT
};

class CUIWindowAddOctTree : public IEventReceiver {
  
public:
    
    CUIWindowAddOctTree(CDevices *_devices, IGUIListBox *_terrainsListBox);
    ~CUIWindowAddOctTree();
    
    void open();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    CDevices *devices;
    
    IGUIListBox *terrainsListBox;
    
    IGUIWindow *addOctTreeWindow;
    IGUIEditBox *addOctTreeEditBox;
    
    IGUICheckBox *asMeshSceneNode;
    IGUIEditBox *minPolysPerNode;
    
    stringw path_file;
    bool isOpenFileDialogOpened;
    
};

#endif
