/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include "CUIWindowAddNewTerrain.h"

CUIWindowAddNewTerrain::CUIWindowAddNewTerrain(CDevices *_devices) {
	devices = _devices;

	//GUI ELEMENTS
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	window = gui->addWindow(rect<s32>(670, 320, 900, 460), true, L"Add New Terrain...", 0, -1);
	devices->getCore()->centerWindow(window, devices->getVideoDriver()->getScreenSize());

	gui->addStaticText(L"Size : ", rect<s32>(10, 30, 60, 50), true, true, window, -1, false);
	sizeeb = gui->addEditBox(L"256", rect<s32>(60, 30, 160, 50), true, window, -1);

	randomHillscb = gui->addCheckBox(false, rect<s32>(10, 60, 160, 80), window, -1, L"Random Hills");

	accept = gui->addButton(rect<s32>(10, 90, 110, 120), window, -1, L"Accept", L"Accept and create the terrain");
	close = gui->addButton(rect<s32>(120, 90, 220, 120), window, -1, L"Close", L"Close");

	//DATAS
	devices->getEventReceiver()->AddEventReceiver(this);
}

CUIWindowAddNewTerrain::~CUIWindowAddNewTerrain() {

}

bool CUIWindowAddNewTerrain::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window) {
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
		}

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == close) {
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
			if (event.GUIEvent.Caller == accept) {
				s32 size = devices->getCore()->getS32(sizeeb->getText());
				if (size <= 10) {
					devices->addWarningDialog(L"Error", L"Please select a size > 10", EMBF_OK);
				} else {
					u32 goodSize = size;
					IImage* tmpImage = devices->getVideoDriver()->createImage(irr::video::ECF_A8R8G8B8, dimension2du(goodSize, goodSize));

					if (randomHillscb->isChecked()) {
						//noise(true, goodSize, goodSize, 1.0, 100, 100, 255, 255, 255);
					} else {
						for (u32 i=0; i < goodSize; i++)
							for (u32 j=0; j < goodSize; j++)
								tmpImage->setPixel(i, j, SColor(0, 0, 0, 0), true);
					}

					devices->getVideoDriver()->writeImageToFile(tmpImage, stringc(devices->getWorkingDirectory() + "newterrain.bmp"));

					ITerrainSceneNode *node = devices->getSceneManager()->addTerrainSceneNode(stringc(devices->getWorkingDirectory() + "newterrain.bmp").c_str(),
																							  0, -1, vector3df(0), vector3df(0), vector3df(1.f, 0.1f, 1.f), 
																							  SColor(255, 255, 255, 255), 1024, ETPS_17, 50, true);

					if (remove(stringc(stringc(devices->getWorkingDirectory()) + stringc("newterrain.bmp")).c_str()) != 0)
						devices->addInformationDialog(L"Error", L"Error when deleting the temporary file... \n Not a problem.", EMBF_OK);

					if (node) {
						devices->getCollisionManager()->setCollisionToAnOctTreeNode(node);

						STerrainHMData tdata(node->getMesh(), node, "custom", ETPS_65);
						devices->getCoreData()->getTerrainsData()->push_back(tdata);

						devices->getXEffect()->addShadowToNode(node, devices->getXEffectFilterType(), ESM_BOTH);

						node->setMaterialFlag(EMF_LIGHTING, false);
						node->setMaterialFlag(EMF_NORMALIZE_NORMALS, false);

						node->setName("#map:new_terrain");

						ITexture *randTexture = devices->getVideoDriver()->addTexture("custom", tmpImage);

						window->remove();
						devices->getEventReceiver()->RemoveEventReceiver(this);
						delete this;
					} else {
						devices->addWarningDialog(L"Error", L"Error when creating the terrain", EMBF_OK);
					}
				}
			}
		}
	}

	return false;
}
