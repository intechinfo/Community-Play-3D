//
//  CUIRenderingInfos.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//
#include "stdafx.h"
#include "CUIScenarioMakerMain.h"

#include "ScenarioObjects/CUIScenarioObjects.h"

CUIScenarioMakerMain::CUIScenarioMakerMain(CDevices *_devices) {
	devices = _devices;

	gui = devices->getGUIEnvironment();

	//---------------------------------------------------------------------------------------------
	//------------------------------------------GUI ELEMENTS---------------------------------------
	//---------------------------------------------------------------------------------------------
	
	/// Window
	window = gui->addWindow(rect<s32>(0, 0, 1190, 660), false, L"Scenario Maker", 0, -1);
	window->getMaximizeButton()->setVisible(true);
	window->getMinimizeButton()->setVisible(true);
	devices->getCore()->centerWindow(window, devices->getVideoDriver()->getScreenSize());

	menu = gui->addMenu(window);
    menu->addItem(L"File", -1, true, true);
	menu->addItem(L"Edit", -1, true, true);
	menu->addItem(L"Factory", -1, true, true);

	bar = devices->getGUIEnvironment()->addToolBar(window, -1);
    bar->setRelativePosition(position2di(0, 40));
	ITexture* image = devices->getVideoDriver()->getTexture(devices->getWorkingDirectory() + stringc("/GUI/folder.png"));
	bar->addButton(-1, 0, L"Open Saved Animation Configuration", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture(devices->getWorkingDirectory() + stringc("/GUI/edit.png"));
	bar->addButton(-1, 0, L"Edit The Node", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture(devices->getWorkingDirectory() + stringc("/GUI/save.png"));
	bar->addButton(0, 0, L"Export this scene", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture(devices->getWorkingDirectory() + stringc("/GUI/help.png"));
	bar->addButton(0, 0, L"Open Help", image, 0, false, true);

	/// Zones
	(objectsZone = gui->addWindow(rect<s32>(10, 70, 400, 380), false, L"Scene Objects", window, -1))->setDraggable(false);
	(viewPortZone = gui->addWindow(rect<s32>(400, 70, 790, 380), false, L"Scene View", window, -1))->setDraggable(false);
	(inspectZone = gui->addWindow(rect<s32>(790, 70, 1180, 380), false, L"Inspector", window, -1))->setDraggable(false);
	(timeLineZone = gui->addWindow(rect<s32>(170, 390, 1180, 650), false, L"Timeline", window, -1))->setDraggable(false);

	objectsZone->getCloseButton()->setVisible(false);
	viewPortZone->getCloseButton()->setVisible(false);
	inspectZone->getCloseButton()->setVisible(false);
	timeLineZone->getCloseButton()->setVisible(false);

	/// Objects Zone
	objectsView = gui->addTreeView(rect<s32>(15, 25, 375, 300), objectsZone, -1, true, true, true);

	/// Viewport Zone
	viewPort = new CGUIViewport(gui, viewPortZone, 1, rect<s32>(15, 25, 375, 250)); 
    if (viewPort) {
		viewPort->setScreenQuad(devices->getXEffect()->getScreenQuadPtr());
		viewPort->setSceneManager(devices->getSceneManager());
		viewPort->setRenderScreenQuad(true);
        viewPort->setOverrideColor(SColor(255, 0, 0, 0)); 
    }

	/// Timeline Zone
	timeline = new CGUITimeLine(devices->getDevice(), timeLineZone, -1, rect<s32>(5, 25, 1005, 200), false);

	/// End
	objectsUtils = new CUIScenarioObjects(devices, objectsView, this);

	//maximize();
	devices->getEventReceiver()->AddEventReceiver(this, window, this);
}

CUIScenarioMakerMain::~CUIScenarioMakerMain() {

}

void CUIScenarioMakerMain::update() {
	objectsView->setRelativePosition(rect<s32>(15, 25, objectsZone->getRelativePosition().getWidth()-15, objectsZone->getRelativePosition().getHeight()-15));
	viewPort->setRelativePosition(rect<s32>(15, 25, viewPortZone->getRelativePosition().getWidth()-15, viewPortZone->getRelativePosition().getHeight()-15));
	timeline->setRelativePosition(rect<s32>(5, 25, timeLineZone->getRelativePosition().getWidth()-5, timeLineZone->getRelativePosition().getHeight()-10));
}

bool CUIScenarioMakerMain::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window) {
				delete objectsUtils;
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
		}

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == window->getMaximizeButton()) {
				maximize();
			}
		}
	}

	return false;
}

void CUIScenarioMakerMain::createObjectsView(rect<s32> size) {
	if (objectsView)
		objectsView->remove();
	objectsView = gui->addTreeView(size, objectsZone, -1, true, true, true);
	objectsUtils->setTreeView(objectsView);
	objectsUtils->createNodes();
}

void CUIScenarioMakerMain::maximize() {

	devices->getCore()->maximizeWindow(window, rect<s32>(0, 0, 1190, 660));

	s32 zonesWidths = window->getRelativePosition().getWidth() / 3;
	objectsZone->setRelativePosition(rect<s32>(10, 70, zonesWidths, (window->getRelativePosition().getHeight() / 3) * 2));

	if (window->getRelativePosition().getWidth() == devices->getVideoDriver()->getScreenSize().Width + 2) {
		viewPortZone->setRelativePosition(rect<s32>(zonesWidths, 70, zonesWidths*2+zonesWidths/2, (window->getRelativePosition().getHeight() / 3) * 2));
		inspectZone->setRelativePosition(rect<s32>(zonesWidths*2+zonesWidths/2, 70, zonesWidths*3, (window->getRelativePosition().getHeight() / 3) * 2));
	} else {
		viewPortZone->setRelativePosition(rect<s32>(zonesWidths, 70, zonesWidths*2, (window->getRelativePosition().getHeight() / 3) * 2));
		inspectZone->setRelativePosition(rect<s32>(zonesWidths*2, 70, zonesWidths*3, (window->getRelativePosition().getHeight() / 3) * 2));
	}

	timeLineZone->setRelativePosition(rect<s32>(window->getRelativePosition().getWidth() / 6,
												(window->getRelativePosition().getHeight() / 3) * 2 + 15,
												window->getRelativePosition().getWidth()-15,
												window->getRelativePosition().getHeight()-15));

	if (window->getRelativePosition().getWidth() == rect<s32>(0, 0, 1190, 660).getWidth())
		devices->getCore()->centerWindow(window, devices->getVideoDriver()->getScreenSize());

	createObjectsView(rect<s32>(15, 25, objectsZone->getRelativePosition().getWidth()-15, objectsZone->getRelativePosition().getHeight()-15));
}
