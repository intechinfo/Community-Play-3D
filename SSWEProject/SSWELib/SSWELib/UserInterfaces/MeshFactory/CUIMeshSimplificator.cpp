/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"

#include "CUIMeshSimplificator.h"

CUIMeshSimplificator::CUIMeshSimplificator(CDevices *_devices) {
	devices = _devices;
	mesh = 0;
	node = 0;

	//-----------------------------------
    //GUI ELEMENTS
	ui = new CUIMeshSimplificatorUI(devices->getGUIEnvironment(), devices->getDevice());

	/// Window
	devices->getCore()->centerWindow(ui->window, devices->getVideoDriver()->getScreenSize());

	/// Viewport
	smgr = devices->getSceneManager()->createNewSceneManager(false);
	smgr->addCameraSceneNodeMaya()->setPosition(vector3df(200.f));
	smgr->addLightSceneNode(0, vector3df(200.f), SColorf(1.0, 1.0, 1.0, 1.0), 20000.f, -1);
	ui->viewPort->setSceneManager(smgr);
	ui->viewPort->setRenderScreenQuad(false);
    ui->viewPort->setOverrideColor(SColor(255, 0, 0, 0));

	/// Console
	ui->console->setEnabled(false);
	ui->console->setBackgroundColor(SColor(255, 255, 255, 255));
	ui->console->setOverrideColor(SColor(255, 0, 0, 0));
	//-----------------------------------

	devices->getEventReceiver()->AddEventReceiver(this, ui->window);
}
CUIMeshSimplificator::~CUIMeshSimplificator() {

}

bool CUIMeshSimplificator::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		/// Button clicked
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == ui->addMesh) {
				openMesh = devices->createFileOpenDialog(L"Select mesh", CGUIFileSelector::EFST_OPEN_DIALOG, 0, true);
			}

			if (event.GUIEvent.Caller == ui->close) {
				if (node && mesh) {
					for (u32 i=0; i < mesh->getMeshBufferCount(); i++)
						devices->getMeshSimplificator()->removeSimplifiedMeshBuffer(mesh->getMeshBuffer(i));
				}
				delete ui;
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
				return true;
			}

			if (event.GUIEvent.Caller == ui->accept) {
				delete ui;
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
				return true;
			}

			if (event.GUIEvent.Caller == ui->compute) {
				if (node && mesh) {
					ui->compute->setEnabled(false);

					f32 percentage = devices->getCore()->getF32(ui->percentageEditBox->getText());
					consoleText = "Computing...\n";
					for (u32 i=0; i < mesh->getMeshBufferCount(); i++) {
						devices->getMeshSimplificator()->switchToOriginalMeshBuffer(mesh->getMeshBuffer(i));
						consoleText += "Computing mesh buffer " + i;
						consoleText += "\n";
						devices->getMeshSimplificator()->simplifyMeshBuffer(mesh->getMeshBuffer(i), percentage,
							[=](IMeshBuffer *buffer) {
								devices->getMeshSimplificator()->switchToSimplifiedMeshBuffer(buffer);
								mesh->setDirty();
								consoleText += "Computed mesh buffer " + i;
								ui->console->setText(consoleText.c_str());
								if (buffer == mesh->getMeshBuffer(mesh->getMeshBufferCount()-1))
									ui->compute->setEnabled(true);
							}
						);
					}
					ui->console->setText(consoleText.c_str());
				}
			}
		}

		/// Scroll bar change
		if (event.GUIEvent.EventType == EGET_SCROLL_BAR_CHANGED) {
			if (event.GUIEvent.Caller == ui->scalebar) {
				if (node && mesh) {
					node->setScale(vector3df((f32)ui->scalebar->getPos() / 10.f));
				}
			}
		}

		/// File selected
		if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
			if (event.GUIEvent.Caller == openMesh) {

				if (node) node->remove();
				if (mesh) {
					smgr->getMeshCache()->removeMesh(mesh);
				}

				mesh = smgr->getMesh(openMesh->getFileName());
				if (mesh) {
					node = smgr->addMeshSceneNode(mesh);
					node->setScale(vector3df((f32)ui->scalebar->getPos()));
					for (u32 i=0; i < mesh->getMeshBufferCount(); i++)
						devices->getMeshSimplificator()->addSimplifiedMeshBuffer(mesh->getMeshBuffer(i));
				} else {
					mesh = 0;
					node = 0;
					devices->addErrorDialog(L"Error when loading the mesh", L"Cannot load the selected mesh...", EMBF_OK);
				}

			}
		}
	}

	return false;
}
