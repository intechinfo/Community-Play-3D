//
//  CUIWindowExportFile.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 07/11/12.
//
//

#include "CUIWindowExportScene.h"

CUIWindowExportScene::CUIWindowExportScene(CDevices *_devices) {
    devices = _devices;
    devices->getEventReceiver()->AddEventReceiver(this);
    
    path_file = L"";
}

CUIWindowExportScene::~CUIWindowExportScene() {
    
}

void CUIWindowExportScene::open() {
    exportSceneWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(450, 100, 750, 240), 
                                                                true, L"Export the scene", 0, -1);
    
    devices->getGUIEnvironment()->addStaticText(L"Name : (#scene exportation)", 
                                                rect<s32>(5, 25, 250, 85), false, false, exportSceneWindow, -1, false);
    exportSceneEditBox = devices->getGUIEnvironment()->addEditBox(path_file.c_str(), rect<s32>(5, 50, 210, 80), true, 
                                                                  exportSceneWindow, -1);
    devices->getGUIEnvironment()->addButton(rect<s32>(5, 110, 70, 135), exportSceneWindow, 
                                            CXT_WINDOW_EXPORT_SCENE_EVENTS_ACCEPT, L"Export", 
                                            L"Accept and export the scene");
    devices->getGUIEnvironment()->addButton(rect<s32>(80, 110, 145, 135), exportSceneWindow, 
                                            CXT_WINDOW_EXPORT_SCENE_EVENTS_CLOSE, L"Cancel", L"Close this window");
}

void CUIWindowExportScene::exportScene(stringw filename) {
    IGUIWindow *window = devices->getGUIEnvironment()->addMessageBox(L"Exporting", 
                                                                     L"Please wait while exporting \n\n"
                                                                     L"This action can take a while",
                                                                     true, 0x0, 0);
    window->getCloseButton()->remove();
    window->getMinimizeButton()->setVisible(true);
    
    devices->getVideoDriver()->beginScene(true, true, SColor(0x0));
    devices->updateDevice();
    devices->getVideoDriver()->endScene();
    
    setPathFile(exportSceneEditBox->getText());
    CExporter *exporter = new CExporter(devices);
    stringc pathExport = devices->getWorkingDirectory().c_str();
    pathExport += "/";
    pathExport += path_file.c_str();
    pathExport += ".world";
    exporter->exportScene(pathExport.c_str());
    delete exporter;
    
    window->remove();
    
    exportSceneWindow->remove();
}

bool CUIWindowExportScene::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_GUI_EVENT) {
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            s32 id = event.GUIEvent.Caller->getID();
            switch (id) {
                    
                case CXT_WINDOW_EXPORT_SCENE_EVENTS_ACCEPT: {
                    IGUIWindow *window = devices->getGUIEnvironment()->addMessageBox(L"Exporting", 
                                                                                     L"Please wait while exporting \n\n"
                                                                                     L"This action can take a while",
                                                                                     true, 0x0, 0);
                    window->getCloseButton()->remove();
                    window->getMinimizeButton()->setVisible(true);
                    
                    devices->getVideoDriver()->beginScene(true, true, SColor(0x0));
                    devices->updateDevice();
                    devices->getVideoDriver()->endScene();

                    setPathFile(exportSceneEditBox->getText());
                    CExporter *exporter = new CExporter(devices);
                    stringc pathExport = devices->getWorkingDirectory().c_str();
                    pathExport += "/";
                    pathExport += path_file.c_str();
                    pathExport += ".world";
                    exporter->exportScene(pathExport.c_str());
                    delete exporter;
                    
                    window->remove();
                    
                    exportSceneWindow->remove();
                }
                    break;
                    
                case CXT_WINDOW_EXPORT_SCENE_EVENTS_CLOSE:
                    exportSceneWindow->remove();
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    return false;
}