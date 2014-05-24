/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"

#include "CUIAnimatedMeshViewer.h"
#include "CMeshFactory.h"

CUIAnimatedMeshViewer::CUIAnimatedMeshViewer(CDevices *_devices) {
	devices = _devices;
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	//-----------------------------------
    //GUI ELEMENTS
	/// Main
	window = gui->addWindow(rect<s32>(30, 30, 1163, 780), false, L"Aniamted Mesh Viewer", 0, -1);
	window->getMinimizeButton()->setVisible(false);
	window->getMaximizeButton()->setVisible(false);
	devices->getCore()->maximizeWindow(window, rect<s32>(30, 30, 1163, 780));

	viewPort = new CGUIViewport(gui, window, -1, rect<s32>(0, 20, window->getRelativePosition().getWidth()-250, window->getRelativePosition().getHeight()));
	smgr = devices->getSceneManager()->createNewSceneManager(false);
	viewPort->setRenderScreenQuad(false);
	viewPort->setSceneManager(smgr);
	smgr->addCameraSceneNodeMaya();
	smgr->getActiveCamera()->setTarget(vector3df(0.f));
	smgr->setAmbientLight(SColorf(0.3f, 0.3f, 0.3f));
    viewPort->setOverrideColor(SColor(255, 0, 0, 0));
	CGridSceneNode *grid = new CGridSceneNode(smgr->getRootSceneNode(), smgr);

	/// Zone
	zone = gui->addStaticText(L"", rect<s32>(window->getRelativePosition().getWidth()-250, 20,
											window->getRelativePosition().getWidth(), window->getRelativePosition().getHeight()),
							 true, true, window, -1, true);

	gui->addStaticText(L"Scale : ", rect<s32>(0, 20, 250, 40), true, true, zone, -1, true);
	scaleeb = gui->addEditBox(L"", rect<s32>(0, 40, 150, 60), true, zone, -1);
	scaleLess = gui->addButton(rect<s32>(150, 40, 200, 60), zone, -1, L"*0.1", L"Set current scale * 0.1");
	scalePlus = gui->addButton(rect<s32>(200, 40, 250, 60), zone, -1, L"*10", L"Set current scale * 10");

	frameTxt = gui->addStaticText(L"Frames : ", rect<s32>(0, 90, 250, 110), true, true, zone, -1, true);
	gui->addStaticText(L"Begin : ", rect<s32>(20, 110, 80, 130), false, true, zone, -1, false);
	frameBegin = gui->addEditBox(L"", rect<s32>(80, 110, 250, 130), true, zone, -1);
	gui->addStaticText(L"End : ", rect<s32>(20, 130, 80, 150), false, true, zone, -1, false);
	frameEnd = gui->addEditBox(L"", rect<s32>(80, 130, 250, 150), true, zone, -1);
	previewFrame = close = gui->addButton(rect<s32>(150, 150, 250, 170), zone, -1, L"Preview", L"Preview current frame");

	gui->addStaticText(L"Data : ", rect<s32>(0, 190, 250, 210), true, true, zone, -1, true);
	open = close = gui->addButton(rect<s32>(20, 210, 120, 230), zone, -1, L"Open...", L"Open a new mesh");
	reload = close = gui->addButton(rect<s32>(120, 210, 220, 230), zone, -1, L"Reload", L"Reload the current mesh if changed");

	close = gui->addButton(rect<s32>(140, zone->getRelativePosition().getHeight() - 45, 240, zone->getRelativePosition().getHeight()-15),
						   zone, -1, L"Close", L"Close this window");
	//-----------------------------------

	factory = new CMeshFactory(devices);

	devices->setRenderScene(false);
	devices->getEventReceiver()->AddEventReceiver(this, window);

	meshAlreadyExists = false;
	meshPath = "";
	mesh = 0;
	node = 0;
}
CUIAnimatedMeshViewer::~CUIAnimatedMeshViewer() {
	if (node)
		node->remove();

	if (mesh && meshAlreadyExists)
		smgr->getMeshCache()->removeMesh(mesh);

	delete smgr;
	delete factory;
}

bool CUIAnimatedMeshViewer::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == close) {
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				devices->setRenderScene(true);
				delete this;
			} else

			if (event.GUIEvent.Caller == open) {
				openDialog = devices->createFileOpenDialog(L"Open an animated mesh", CGUIFileSelector::EFST_OPEN_DIALOG);
			} else

			if (event.GUIEvent.Caller == scaleLess) {
				if (mesh != 0 && node != 0)
					node->setScale(vector3df(node->getScale() * 0.1f));
			} else
			if (event.GUIEvent.Caller == scalePlus) {
				if (mesh != 0 && node != 0)
					node->setScale(vector3df(node->getScale() * 10.f));
			} else

			if (event.GUIEvent.Caller == previewFrame) {
				if (mesh != 0 && node != 0) {
					s32 startFrame = devices->getCore()->getS32(frameBegin->getText());
					s32 endFrame = devices->getCore()->getS32(frameEnd->getText());
					node->setFrameLoop(startFrame, endFrame);
				}
			} else

			if (event.GUIEvent.Caller == reload) {
				if (mesh != 0 && node != 0) {
					factory->reloadMesh(mesh, meshPath);
					mesh = smgr->getMesh(meshPath);
					node->setMesh(mesh);
					node->setMaterialFlag(EMF_LIGHTING, false);
				}
			}
		}

		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window) {
				devices->getEventReceiver()->RemoveEventReceiver(this);
				devices->setRenderScene(true);
				delete this;
			}
		}

		if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
			if (event.GUIEvent.Caller == openDialog) {
				stringc filename = openDialog->getFileName();
				if (!meshAlreadyExists) {
					smgr->getMeshCache()->removeMesh(mesh);
				}

				if (node) {
					node->remove();
					node = 0;
				}

				if (devices->getSceneManager()->getMeshCache()->isMeshLoaded(io::path(filename.c_str()))) {
					meshAlreadyExists = true;
				} else {
					meshAlreadyExists = false;
				}

				mesh = smgr->getMesh(filename.c_str());
				if (mesh) {
					node = smgr->addAnimatedMeshSceneNode(mesh);
					node->setMaterialFlag(EMF_LIGHTING, false);
					node->setFrameLoop(0, node->getEndFrame());
					node->setLoopMode(true);
					frameTxt->setText(stringw(stringw("Frames : (") + stringw(node->getStartFrame()) + stringw(" - ") + stringw(node->getEndFrame())).c_str());
					frameBegin->setText(stringw(node->getStartFrame()).c_str());
					frameEnd->setText(stringw(node->getEndFrame()).c_str());
					scaleeb->setText(stringw(node->getScale().X).c_str());
					meshPath = filename;
				} else {
					meshPath = "";
					meshAlreadyExists = false;
					mesh = 0;
					node = 0;
				}
			}
		}
	}

	return false;
}
