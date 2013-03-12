//
//  CUIWindowExportFile.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 07/11/12.
//
//

#ifndef __C_UI_WINDOW_EXPORT_SCENE_H_INCLUDED__
#define __C_UI_WINDOW_EXPORT_SCENE_H_INCLUDED__

#include "../Device/CDevices.h"

#include "../Scenes/CExporter.h"

enum GUI_WINDOW_EVENTS_EXPORT_SCENE {
    CXT_WINDOW_EXPORT_SCENE_EVENTS_CLOSE = 0x14000,
    CXT_WINDOW_EXPORT_SCENE_EVENTS_ACCEPT
};

class CUIWindowExportScene : public IEventReceiver {
    
public:
    
    CUIWindowExportScene(CDevices *_devices);
    ~CUIWindowExportScene();
    
    void open();
    void setPathFile(stringw _path_file) { path_file = _path_file.c_str(); }
    
    bool OnEvent(const SEvent &event);
    
private:
    
    CDevices *devices;
    
    IGUIWindow *exportSceneWindow;
    IGUIEditBox *exportSceneEditBox;
    
    stringw path_file;
    bool isOpenFileDialogOpened;
    
};

#endif
