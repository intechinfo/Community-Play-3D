//
//  CUIWindowEditNodeAnimators.cpp
//  Windows
//
//  Created by Arthur Caron on 12/06/13.
//
//

#include "stdafx.h"
#include "CUIEditFPSCamera.h"

CUIEditFPSCamera::CUIEditFPSCamera(CDevices *_devices) {
	devices = _devices;

	//-----------------------------------
    //GUI ELEMENTS
	IGUIEnvironment *gui = devices->getGUIEnvironment();
	window = gui->addWindow(rect<s32>(140, 170, 560, 560), false, L"Configure FPS Camera", 0, -1);

	gui->addStaticText(L"Ellipsoid Radius :", rect<s32>(10, 20, 120, 40), true, false, window, -1, true);
	gui->addStaticText(L"X : ", rect<s32>(10, 40, 40, 60), true, false, window, -1, true);
	erx = gui->addEditBox(devices->getCore()->getStrNumber(devices->getCollisionManager()->getFPSCameraSettings()->getEllipsoidRadius().X).c_str(), rect<s32>(40, 40, 140, 60), true, window, -1);
	gui->addStaticText(L"Y : ", rect<s32>(140, 40, 170, 60), true, false, window, -1, true);
	ery = gui->addEditBox(devices->getCore()->getStrNumber(devices->getCollisionManager()->getFPSCameraSettings()->getEllipsoidRadius().Y).c_str(), rect<s32>(170, 40, 270, 60), true, window, -1);
	gui->addStaticText(L"Z : ", rect<s32>(270, 40, 300, 60), true, false, window, -1, true);
	erz = gui->addEditBox(devices->getCore()->getStrNumber(devices->getCollisionManager()->getFPSCameraSettings()->getEllipsoidRadius().Z).c_str(), rect<s32>(300, 40, 400, 60), true, window, -1);

	gui->addStaticText(L"Gravity Per Second :", rect<s32>(10, 80, 140, 100), true, false, window, -1, true);
	gui->addStaticText(L"X : ", rect<s32>(10, 100, 40, 120), true, false, window, -1, true);
	gpsx = gui->addEditBox(devices->getCore()->getStrNumber(devices->getCollisionManager()->getFPSCameraSettings()->getGravityPerSecond().X).c_str(), rect<s32>(40, 100, 140, 120), true, window, -1);
	gui->addStaticText(L"Y : ", rect<s32>(140, 100, 170, 120), true, false, window, -1, true);
	gpsy = gui->addEditBox(devices->getCore()->getStrNumber(devices->getCollisionManager()->getFPSCameraSettings()->getGravityPerSecond().Y).c_str(), rect<s32>(170, 100, 270, 120), true, window, -1);
	gui->addStaticText(L"Z : ", rect<s32>(270, 100, 300, 120), true, false, window, -1, true);
	gpsz = gui->addEditBox(devices->getCore()->getStrNumber(devices->getCollisionManager()->getFPSCameraSettings()->getGravityPerSecond().Z).c_str(), rect<s32>(300, 100, 400, 120), true, window, -1);

	gui->addStaticText(L"Ellipsoid Translation :", rect<s32>(10, 140, 150, 160), true, false, window, -1, true);
	gui->addStaticText(L"X : ", rect<s32>(10, 160, 40, 180), true, false, window, -1, true);
	etx = gui->addEditBox(devices->getCore()->getStrNumber(devices->getCollisionManager()->getFPSCameraSettings()->getEllipsoidTranslation().X).c_str(), rect<s32>(40, 160, 140, 180), true, window, -1);
	gui->addStaticText(L"Y : ", rect<s32>(140, 160, 170, 180), true, false, window, -1, true);
	ety = gui->addEditBox(devices->getCore()->getStrNumber(devices->getCollisionManager()->getFPSCameraSettings()->getEllipsoidTranslation().Y).c_str(), rect<s32>(170, 160, 270, 180), true, window, -1);
	gui->addStaticText(L"Z : ", rect<s32>(270, 160, 300, 180), true, false, window, -1, true);
	etz = gui->addEditBox(devices->getCore()->getStrNumber(devices->getCollisionManager()->getFPSCameraSettings()->getEllipsoidTranslation().Z).c_str(), rect<s32>(300, 160, 400, 180), true, window, -1);

	gui->addStaticText(L"Sliding Value : ", rect<s32>(10, 200, 100, 220), true, false, window, -1, true);
	sv = gui->addEditBox(devices->getCore()->getStrNumber(devices->getCollisionManager()->getFPSCameraSettings()->getSlidingValue()).c_str(), rect<s32>(100, 200, 270, 220), true, window, -1);
	
	gui->addStaticText(L"Move Speed : ", rect<s32>(10, 220, 100, 240), true, false, window, -1, true);
	ms = gui->addEditBox(devices->getCore()->getStrNumber(devices->getCollisionManager()->getFPSCameraSettings()->getCameraAnimator()->getMoveSpeed()).c_str(), rect<s32>(100, 220, 270, 240), true, window, -1);

	accept = gui->addButton(rect<s32>(10, 300, 110, 330), window, -1, L"Accept", L"Accept");
	close = gui->addButton(rect<s32>(130, 300, 230, 330), window, -1, L"Cancel", L"Cancel");
	//-----------------------------------

	devices->getEventReceiver()->AddEventReceiver(this, window);
}

CUIEditFPSCamera::~CUIEditFPSCamera() {
	
}

bool CUIEditFPSCamera::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == accept) {
				devices->getCollisionManager()->getFPSCameraSettings()->setEllipsoidRadius(devices->getCore()->getVector3df(erx->getText(), ery->getText(), erz->getText()));
				devices->getCollisionManager()->getFPSCameraSettings()->setGravityPerSecond(devices->getCore()->getVector3df(gpsx->getText(), gpsy->getText(), gpsz->getText()));
				devices->getCollisionManager()->getFPSCameraSettings()->setEllipsoidTranslation(devices->getCore()->getVector3df(etx->getText(), ety->getText(), etz->getText()));
				devices->getCollisionManager()->getFPSCameraSettings()->setSlidingValue(devices->getCore()->getF32(sv->getText()));
                
				devices->getCollisionManager()->getFPSCameraSettings()->getCameraAnimator()->setMoveSpeed(devices->getCore()->getF32(ms->getText()));
                devices->getCameraRig()->setMoveSpeed(devices->getCore()->getF32(ms->getText()));

				devices->getCollisionManager()->createAnimatorCollisionCamera(devices->getFPSCamera());
				//devices->getCollisionManager()->createAnimatorCollisionCamera(devices->getCameraRig()->getCameraSceneNode());
				devices->getSceneManager()->setActiveCamera(devices->getFPSCamera());
				//devices->getSceneManager()->setActiveCamera(devices->getCameraRig()->getCameraSceneNode());
				devices->getFPSCamera()->setPosition(devices->getMayaCamera()->getPosition());
				//devices->getCameraRig()->getCameraSceneNode()->setPosition(devices->getMayaCamera()->getPosition());
				devices->getDevice()->getCursorControl()->setVisible(false);

				devices->getEventReceiver()->RemoveEventReceiver(this);
				window->remove();

				devices->getCursor()->setVisible(false);
				delete this;
			}

			if (event.GUIEvent.Caller == close) {
				devices->getEventReceiver()->RemoveEventReceiver(this);
				window->remove();
				delete this;
			}
		}
	}

	return false;
}
