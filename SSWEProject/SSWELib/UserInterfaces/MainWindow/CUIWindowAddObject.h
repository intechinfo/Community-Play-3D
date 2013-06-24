/* defined(__MacOSX__CUIWindowAddObject__) *///
//  CUIWindowAddObject.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 27/09/12.
//
//

#ifndef __C_UI_WINDOW_ADD_OBJECT_H_INCLUDED__
#define __C_UI_WINDOW_ADD_OBJECT_H_INCLUDED__

#include "Device/CDevices.h"

enum GUI_WINDOW_EVENTS_ADD_OBJECT {
    CXT_WINDOW_ADD_OBJECT_EVENTS_CLOSE = 0x60000,
    CXT_WINDOW_ADD_OBJECT_EVENTS_SELECT,
    CXT_WINDOW_ADD_OBJECT_EVENTS_ACCEPT
};

class CUIWindowAddObject : public IEventReceiver {
    
public:
    
    CUIWindowAddObject(CDevices *_devices, IGUIListBox *_objectsListBox);
    ~CUIWindowAddObject();
    
    void open();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    CDevices *devices;
    
    IGUIListBox *objectsListBox;
    
    IGUIWindow *addObjectWindow;
    IGUIEditBox *addObjectEditBox;
    
    stringw path_file;
    bool isOpenFileDialogOpened;
    
};

#endif
