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
#include "CMeshFactory.h"

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
    ui->console->setOverrideColor(SColor(255, 32, 32, 32));
    ui->console->setBackgroundColor(SColor(255, 200, 200, 200));
    ui->console->setLineCountColors(SColor(255, 32, 32, 32), SColor(200, 64, 120, 180), SColor(255, 200, 200, 224));
    ui->console->setSelectionColors(SColor(180, 0, 96, 64), SColor(255, 255, 255, 255), SColor(180, 0, 128, 96));
    ui->console->setWordWrap(false);
	ui->console->setEnabled(true);
    ui->console->setOverrideColor(SColor(255, 0, 0, 0));
    ui->console->setAutoScroll(true);
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
                    ui->console->setText(L"");

					f32 percentage = devices->getCore()->getF32(ui->percentageEditBox->getText());
                    
                    CMeshFactory *meshFactory = new CMeshFactory(devices);
                    ISceneManager *oldSmgr = devices->getSceneManager();
                    devices->setSceneManagerToDraw(smgr);
                    meshFactory->reloadMesh(mesh, smgr->getMeshCache()->getMeshName(mesh));
                    devices->setSceneManagerToDraw(oldSmgr);
                    delete meshFactory;
                    
                    devices->getMeshSimplificator()->simplifyMesh(mesh, percentage,
                        [=](IMeshBuffer *buffer, s32 index) {
                            stringw consoleText = ui->console->getText();
                            consoleText += "Finished mesh buffer n ";
                            consoleText += index;
                            consoleText += "\n";
                            ui->console->setText(consoleText.c_str());
                        },
                        [=](IMesh *computedMesh) {
                            for (u32 i=0; i < computedMesh->getMeshBufferCount(); i++) {
                                devices->getMeshSimplificator()->switchToSimplifiedMeshBuffer(computedMesh->getMeshBuffer(i));
                            }
                            ui->compute->setEnabled(true);
                        }
                    );
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
