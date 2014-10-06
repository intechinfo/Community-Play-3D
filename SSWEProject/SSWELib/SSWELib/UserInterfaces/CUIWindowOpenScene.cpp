//
//  CUIWindowAddOctTree.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 27/09/12.
//
//
#include "stdafx.h"
#include "CUIWindowOpenScene.h"

#include "../../../SSWERenders/Renders/Materials/CNormalMappingMaterial.h"

CUIWindowOpenScene::CUIWindowOpenScene(CDevices *_devices) {
    devices = _devices;
    devices->getEventReceiver()->AddEventReceiver(this);
    
    path_file = L"";
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

void CUIWindowOpenScene::open(bool concat) {
    /*IGUIFileOpenDialog *dialog = devices->getGUIEnvironment()->addFileOpenDialog(L"Open Scene");
    dialog->setMinSize(dimension2d<u32>(400, 400));
    dialog->setNotClipped(false);
    dialog->setRelativePositionProportional(rect<f32>(0.3, 0.3, 0.3, 0.6));*/
    
	this->concat = concat;

    openDialog = devices->createFileOpenDialog(L"Open saved world", CGUIFileSelector::EFST_OPEN_DIALOG);
	((CGUIFileSelector *)openDialog)->addFileFilter(L"WORLD", L"world", 0);

	//modalScreen = devices->getGUIEnvironment()->addModalScreen(openDialog);
    //devices->createFileOpenDialog(L"Open Scene");
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
			if (event.GUIEvent.Caller == openDialog) {
                IGUIFileOpenDialog *dialog = (IGUIFileOpenDialog *)event.GUIEvent.Caller;
                path_file = L"";
                path_file.append(dialog->getFileName());
                if (path_file != L"") {
					if (!concat) {
						devices->getCoreData()->clear();
						devices->getCoreData()->clearAllTheArrays();
						devices->getXEffect()->clearAll();
                        devices->getNormalMappingMaterial()->reset();
					}

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
					importer->setPathOfFile_t(path_file.c_str());
					//std::thread importer_t(&CImporter::import_t, *importer);
					//importer_t.join();
                    importer->importScene(path_file.c_str());
                    delete importer;

					stringw projectName = path_file;
					projectName.remove(devices->getDevice()->getFileSystem()->getFileDir(projectName.c_str()));
					projectName.remove("/");
					stringc fileExtension;
					core::getFileNameExtension(fileExtension, projectName.c_str());
					projectName.remove(fileExtension);
					devices->setProjectName(projectName.c_str());
                    
                    window->remove();
                } else {
                    devices->addWarningDialog(L"Warning", L"Cannot load the world\n Dialog didn't give the file name", EMBF_OK);
                }
            }
        }
    }
    
    return false;
}
