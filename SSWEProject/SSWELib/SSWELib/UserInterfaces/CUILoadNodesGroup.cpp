/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include "CUILoadNodesGroup.h"

CUILoadNodesGroup::CUILoadNodesGroup(CDevices *_devices) {
	devices = _devices;

	//-----------------------------------
    //GUI ELEMENTS
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	window = gui->addWindow(rect<s32>(460, 100, 790, 250), false, L"Load Groupe", 0, -1);
	window->getCloseButton()->remove();

	gui->addStaticText(L"Extension : ", rect<s32>(10, 20, 100, 40), true, true, window, -1, true);
	extensioneb = gui->addEditBox(L".obj", rect<s32>(100, 20, 220, 40), true, window, -1);

	chooseDirectory = gui->addButton(rect<s32>(10, 50, 130, 70), window, -1, L"Choose Directory", L"Choose Directory");
	directoryStaticText = gui->addStaticText(stringw(devices->getDevice()->getFileSystem()->getWorkingDirectory()).c_str(), 
											 rect<s32>(130, 50, 310, 70), true, true, window, -1, true);

	accept = gui->addButton(rect<s32>(210, 110, 310, 140), window, -1, L"Accept", L"Accept");
	cancel = gui->addButton(rect<s32>(100, 110, 200, 140), window, -1, L"Cancel", L"Cancel");
	//-----------------------------------

	devices->getEventReceiver()->AddEventReceiver(this, window);
}

CUILoadNodesGroup::~CUILoadNodesGroup() {

}

void CUILoadNodesGroup::loadGroup() {
	CProcess *process = new CProcess(devices->getGUIEnvironment(), "");
	devices->getProcessesLogger()->addProcess(process);

	ISceneManager *smgr = devices->getSceneManager();

	IFileSystem *fs = devices->getDevice()->getFileSystem();
	stringc previousDirectory = fs->getWorkingDirectory();

	fs->changeWorkingDirectoryTo(directoryStaticText->getText());
	IFileList *fl = fs->createFileList();
	fs->changeWorkingDirectoryTo(previousDirectory.c_str());

	for (u32 i=0; i < fl->getFileCount(); i++) {
		io::path ext;
		core::getFileNameExtension(ext, fl->getFullFileName(i));
		if (ext == ".obj") {
			IMesh *mesh = smgr->getMesh(fl->getFullFileName(i));
			IMeshSceneNode *node = smgr->addMeshSceneNode(mesh, 0, -1);

			if (node) {
				devices->getXEffect()->addShadowToNode(node, devices->getXEffectFilterType(), ESM_BOTH);
				devices->getCollisionManager()->setCollisionToAnOctTreeNode(node);

				node->setMaterialFlag(EMF_LIGHTING, false);
				node->setMaterialFlag(EMF_NORMALIZE_NORMALS, false);

				stringw octTreeNodeName = L"#map:";
				octTreeNodeName += i;
				node->setName(octTreeNodeName.c_str());

				STerrainsData tdata(mesh, node, fl->getFullFileName(i), 0, ESNT_MESH);
				devices->getCoreData()->getTerrainsData()->push_back(tdata);

				process->setName(stringw(stringc("Added node :") + fl->getFileName(i)).c_str());

			} else {
				process->setName(stringw(stringc("Failed added node :") + fl->getFileName(i)).c_str());
			}
		}
		process->getProgressBar()->setPercentage((100*i)/fl->getFileCount());
	}

	process->setHasFinished(true);

	window->setVisible(true);
}

bool CUILoadNodesGroup::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == chooseDirectory) {
				selector = devices->createFileOpenDialog(L"Choose directory", CGUIFileSelector::EFST_OPEN_DIALOG);
				selector->setDirectoryChoosable(true);
			}

			if (event.GUIEvent.Caller == accept) {
				window->setVisible(false);
				std::thread loadGroup_t(&CUILoadNodesGroup::loadGroup, *this);
				loadGroup_t.detach();
			}

			if (event.GUIEvent.Caller == cancel) {
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
		}

		if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
			if (event.GUIEvent.Caller == selector) {
				directoryStaticText->setText(selector->getFileName());
				directoryStaticText->setToolTipText(selector->getFileName());
			}
		}
	}

	return false;
}
