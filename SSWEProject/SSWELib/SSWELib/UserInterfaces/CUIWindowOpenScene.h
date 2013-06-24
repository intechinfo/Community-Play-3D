//
//  CUIWindowAddOctTree.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 27/09/12.
//
//

#ifndef __C_UI_WINDOW_OPEN_SCENE_H_INCLUDED__
#define __C_UI_WINDOW_OPEN_SCENE_H_INCLUDED__

#include "../Device/CDevices.h"

#include "../Scenes/CImporter.h"

enum GUI_WINDOW_EVENTS_OPEN_SCENE {
    CXT_WINDOW_OPEN_SCENE_EVENTS_CLOSE = 0x13000,
    CXT_WINDOW_OPEN_SCENE_EVENTS_SELECT,
    CXT_WINDOW_OPEN_SCENE_EVENTS_ACCEPT
};

class CUIWindowOpenScene : public IEventReceiver {
    
public:
    
    CUIWindowOpenScene(CDevices *_devices);
    ~CUIWindowOpenScene();
    
    void open(bool concat=false);
    stringw getPathFile();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    CDevices *devices;
    IGUIElement *modalScreen;

	IGUIFileOpenDialog *openDialog;
    stringw path_file;

	bool concat;
    
};

#endif
