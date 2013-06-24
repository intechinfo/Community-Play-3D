//
//  CUIWindowAddOctTree.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 27/09/12.
//
//

#ifndef __C_UI_WINDOW_ADD_TREE_H_INCLUDED__
#define __C_UI_WINDOW_ADD_TREE_H_INCLUDED__

#include "../../Device/CDevices.h"

enum GUI_WINDOW_EVENTS_ADD_TREE {
    CXT_WINDOW_ADD_TREE_EVENTS_CLOSE = 0x40000,
    CXT_WINDOW_ADD_TREE_EVENTS_SELECT,
    CXT_WINDOW_ADD_TREE_EVENTS_ACCEPT
};

class CUIWindowAddTree : public IEventReceiver {
    
public:
    
    CUIWindowAddTree(CDevices *_devices, IGUIListBox *_treesListBox);
    ~CUIWindowAddTree();
    
    void open();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    CDevices *devices;
    
    IGUIListBox *treesListBox;
    
    IGUIWindow *addTreeWindow;
    IGUIEditBox *addTreeEditBox;
    
    stringw path_file;
    bool isOpenFileDialogOpened;
    
};

#endif
