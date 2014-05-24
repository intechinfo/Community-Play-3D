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

CUIAnimatedMeshFactory::CUIAnimatedMeshFactory(CDevices *_devices) {
	devices = _devices;
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	//-----------------------------------
    //GUI ELEMENTS
	/// Main
	window = gui->addWindow(rect<s32>(30, 30, 1163, 780), false, L"Aniamted Mesh Viewer", 0, -1);
	window->getMinimizeButton()->setVisible(true);
	window->getMaximizeButton()->setVisible(false);
	devices->getCore()->maximizeWindow(window, rect<s32>(30, 30, 1163, 780));

	viewPort = new CGUIViewport(gui, window, -1, rect<s32>(0, 20, window->getRelativePosition().getWidth()-250, window->getRelativePosition().getHeight()));
	smgr = devices->getSceneManager()->createNewSceneManager(false);
	viewPort->setRenderScreenQuad(false);
	viewPort->setSceneManager(smgr);
	smgr->addCameraSceneNodeMaya();
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

	close = gui->addButton(rect<s32>(140, zone->getRelativePosition().getHeight() - 45, 240, zone->getRelativePosition().getHeight()-15),
						   zone, -1, L"Close", L"Close this window");
	//-----------------------------------

	devices->setRenderScene(false);
	devices->getEventReceiver()->AddEventReceiver(this, window);
}
CUIAnimatedMeshFactory::~CUIAnimatedMeshFactory() {

}

bool CUIAnimatedMeshFactory::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == close) {
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				devices->setRenderScene(true);
				delete this;
			}
		}

		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window) {
				devices->getEventReceiver()->RemoveEventReceiver(this);
				devices->setRenderScene(true);
				delete this;
			}
		}

	}

	return false;
}
