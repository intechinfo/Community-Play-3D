//
//  CUIWindowAddOctTree.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 27/09/12.
//
//

#include "CUIWindowOpenScene.h"

CUIWindowOpenScene::CUIWindowOpenScene(CDevices *_devices) {
    devices = _devices;
    devices->getEventReceiver()->AddEventReceiver(this);
    
    path_file = L"";
    isOpenFileDialogOpened = false;
}

CUIWindowOpenScene::~CUIWindowOpenScene() {
    
}

stringw CUIWindowOpenScene::getPathFile() {
    stringw returnedPathFile = path_file;
    
    returnedPathFile.remove(devices->getWorkingDirectory());
    returnedPathFile.remove('/');
    returnedPathFile.remove(".world");
    
    return returnedPathFile;
}

void CUIWindowOpenScene::open() {
    /*IGUIFileOpenDialog *dialog = devices->getGUIEnvironment()->addFileOpenDialog(L"Open Scene");
    dialog->setMinSize(dimension2d<u32>(400, 400));
    dialog->setNotClipped(false);
    dialog->setRelativePositionProportional(rect<f32>(0.3, 0.3, 0.3, 0.6));*/
    
    devices->createFileOpenDialog(L"Open saved world", CGUIFileSelector::EFST_OPEN_DIALOG)->addFileFilter(L"WORLD", L"world", 0);
    //devices->createFileOpenDialog(L"Open Scene");
    
    isOpenFileDialogOpened = true;
}

bool CUIWindowOpenScene::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_GUI_EVENT) {
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            s32 id = event.GUIEvent.Caller->getID();
            switch (id) {
                    
                default:
                    break;
            }
        }
        
        if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
            if (isOpenFileDialogOpened == true) {
                IGUIFileOpenDialog *dialog = (IGUIFileOpenDialog *)event.GUIEvent.Caller;
                path_file = L"";
                path_file.append(dialog->getFileName());
                isOpenFileDialogOpened = false;
                if (path_file != L"") {
                    devices->getCoreData()->clear();
                    devices->getCoreData()->clearAllTheArrays();
                    
                    IGUIWindow *window = devices->getGUIEnvironment()->addMessageBox(L"Loading", 
                                                                                     L"Please wait while importing the world \n\n"
                                                                                     L"This action can take a while",
                                                                                     true, 0x0, 0);
                    
                    devices->getVideoDriver()->beginScene(true, true, SColor(0x0));
                    devices->updateDevice();
                    devices->getVideoDriver()->endScene();
                    
                    devices->getDevice()->getLogger()->log(path_file.c_str());
                    if (devices->getDevice()->getFileSystem()->existFile(path_file.c_str())) {
                        devices->getDevice()->getLogger()->log("The file exists !");
                    } else {
                        devices->getDevice()->getLogger()->log("The file Doesn't exist !");
                    }
                    
                    CImporter *importer = new CImporter(devices);
                    importer->importScene(path_file.c_str());
                    delete importer;
                    
                    array<ISceneNode *> nodes = devices->getCoreData()->getAllSceneNodes();
                    for (u32 i=0; i < nodes.size(); i++) {
                        devices->getPostProcessManager()->addNodeToDepthPass(nodes[i]);
                    }
                    
                    window->remove();
                } else {
                    devices->addWarningDialog(L"Warning", L"Cannot load the world\n Dialog didn't give the file name", EMBF_OK);
                }
            }
        }
    }
    
    return false;
}
