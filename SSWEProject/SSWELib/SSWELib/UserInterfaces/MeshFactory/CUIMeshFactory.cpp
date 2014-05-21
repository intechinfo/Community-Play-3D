/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"

#include "CUIMeshFactory.h"
#include "CMeshFactory.h"

CUIMeshFactory::CUIMeshFactory(CDevices *_devices) {
	devices = _devices;
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	//-----------------------------------
    //GUI ELEMENTS
	window = gui->addWindow(rect<s32>(180, 150, 860, 550), false, L"Mesh Factory", 0, -1);
	window->getMinimizeButton()->setVisible(true);
	window->getMaximizeButton()->setVisible(false);
	devices->getCore()->centerWindow(window, devices->getVideoDriver()->getScreenSize());

	meshes = gui->addListBox(rect<s32>(10, 30, 520, 390), window, -1, true);
	reloadListBox();

	reload = gui->addButton(rect<s32>(530, 30, 670, 60), window, -1, L"Reload", L"Reload the current mesh");
	remove = gui->addButton(rect<s32>(530, 70, 670, 100), window, -1, L"Remove", L"Remove the current mesh");
	clear = gui->addButton(rect<s32>(530, 110, 670, 140), window, -1, L"Clear", L"Free memory for unused meshes");

	close = gui->addButton(rect<s32>(560, 360, 670, 390), window, -1, L"Close", L"Close this window");
	//-----------------------------------

	factory = new CMeshFactory(devices);

	devices->getEventReceiver()->AddEventReceiver(this, window);
}
CUIMeshFactory::~CUIMeshFactory() {

}

void CUIMeshFactory::reloadListBox() {
	meshes->clear();
	for (u32 i=0; i < devices->getSceneManager()->getMeshCache()->getMeshCount(); i++) {
		IMesh *mesh = devices->getSceneManager()->getMeshCache()->getMeshByIndex(i);

		if (mesh == devices->getObjectPlacement()->getArrowXLine()) continue;
		if (mesh == devices->getObjectPlacement()->getArrowYLine()) continue;
		if (mesh == devices->getObjectPlacement()->getArrowZLine()) continue;
		if (mesh == devices->getObjectPlacement()->getArrowYXZLine()) continue;

		stringw meshName = devices->getSceneManager()->getMeshCache()->getMeshName(mesh);
		meshName.remove(devices->getWorkingDirectory());

		meshes->addItem(meshName.c_str());
	}
}

bool CUIMeshFactory::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == close) {
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete factory;
				delete this;
			} else
			if (event.GUIEvent.Caller == reload) {
				if (meshes->getSelected() != -1) {
					IMesh *mesh = 0;
					mesh = devices->getSceneManager()->getMesh(devices->getWorkingDirectory() + meshes->getListItem(meshes->getSelected()));
					if (!mesh)
						mesh = devices->getSceneManager()->getMesh(meshes->getListItem(meshes->getSelected()));

					factory->reloadMesh(mesh, meshes->getListItem(meshes->getSelected()));
				}
			} else
			if (event.GUIEvent.Caller == remove) {
				IMesh *mesh = 0;
				mesh = devices->getSceneManager()->getMesh(devices->getWorkingDirectory() + meshes->getListItem(meshes->getSelected()));
				if (!mesh)
					mesh = devices->getSceneManager()->getMesh(meshes->getListItem(meshes->getSelected()));

				array<ISceneNode *> nodes = factory->getNodesThatUse(mesh);
				if (nodes.size() == 0) {
					devices->getSceneManager()->getMeshCache()->removeMesh(mesh);
				} else {
					stringw text = "The selected mesh : \"";
					text += meshes->getListItem(meshes->getSelected());
					text += "\"\nis used by : \n\n";
					for (u32 i=0; i < nodes.size(); i++) {
						text += "\t\t - ";
						text += nodes[i]->getName();
						text += "\n";
					}
					text += "\nIf you remove the current mesh, these nodes will also be removed.\n";
					text += "Do you want to remove the selected mesh ?";

					removeInformationWindow = devices->addInformationDialog("Informations", text.c_str(), EMBF_YES | EMBF_NO, true);
				}

			} else
			if (event.GUIEvent.Caller == clear) {
				devices->getSceneManager()->getMeshCache()->clearUnusedMeshes();
				reloadListBox();
			}
		}

		if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED || event.GUIEvent.EventType == EGET_LISTBOX_SELECTED_AGAIN) {
			if (event.GUIEvent.Caller == meshes) {
				if (meshes->getSelected() != -1) {
					meshes->setToolTipText(meshes->getListItem(meshes->getSelected()));
					devices->getGUIEnvironment()->setFocus(0);
					devices->getGUIEnvironment()->setFocus(meshes);
				}
			}
		}

		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window) {
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete factory;
				delete this;
			}
		}

		if (event.GUIEvent.EventType == EGET_MESSAGEBOX_YES) {
			if (event.GUIEvent.Caller == removeInformationWindow) {
				IMesh *mesh = 0;
				mesh = devices->getSceneManager()->getMesh(devices->getWorkingDirectory() + meshes->getListItem(meshes->getSelected()));
				if (!mesh)
					mesh = devices->getSceneManager()->getMesh(meshes->getListItem(meshes->getSelected()));

				array<ISceneNode *> nodes = factory->getNodesThatUse(mesh);
				for (u32 i=0; i < nodes.size(); i++) {
					devices->getCoreData()->removeSceneNode(nodes[i], devices->getXEffect());
				}

				devices->getSceneManager()->getMeshCache()->removeMesh(mesh);

				reloadListBox();
			}
		}

	}

	return false;
}
