//
//  CUIWindowEditMaterials.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 23/02/13.
//
//
#include "stdafx.h"
#include "CUIWindowEditDOF.h"

#ifdef __COMPILE_WITH_DOF_SUPPORT_

CUIWindowEditDOF::CUIWindowEditDOF(CDevices *_devices) {
    devices = _devices;
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	//GUI ELEMENTS
	window = gui->addWindow(rect<s32>(490, 220, 1249, 460), false, L"Edit Depth Of Field", 0, -1);
	devices->getCore()->centerWindow(window, devices->getVideoDriver()->getScreenSize());
	window->getMinimizeButton()->setVisible(true);
	

	values[0] =	SValue(L"Range : ", devices->getDOF()->range);
	values[1] =	SValue(L"Focus : ", devices->getDOF()->focus);
	values[2] =	SValue(L"Sample Distance : ", devices->getDOF()->sampleDist0);
	values[3] =	SValue(L"Distance Scale : ", devices->getDOF()->distanceScale);

	ranges[0] = SRange(0, 99, 10);
	ranges[1] = SRange(0, 40, 100);
	ranges[2] = SRange(0, devices->getMayaCamera()->getFarValue(), 10000);
	ranges[3] = SRange(0, 1570, 10000);

	for (u32 i=0; i < 4; i++) {
		IGUIStaticText *tempText = gui->addStaticText(values[i].txt.c_str(), rect<s32>(10, 30, 750, 50), true, true, window, -1, false);
		tempText->setRelativePosition(position2di(10, 30+50*i));

		bars[i] = gui->addScrollBar(true, rect<s32>(10, 50, 750, 70), window, -1);
		bars[i]->setRelativePosition(position2di(10, 50+50*i));
		bars[i]->setMin(ranges[i].min);
		bars[i]->setMax(ranges[i].max);
		bars[i]->setPos((f32)(values[i].value*ranges[i].divide));
		bars[i]->setSmallStep(1);
		bars[i]->setLargeStep(1);
	}

	devices->getEventReceiver()->AddEventReceiver(this, window);
}

CUIWindowEditDOF::~CUIWindowEditDOF() {

}

bool CUIWindowEditDOF::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window) {
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
		}

		if (event.GUIEvent.EventType == EGET_SCROLL_BAR_CHANGED) {
			if (event.GUIEvent.Caller == bars[0]) {
				devices->getDOF()->range = (f32)((f32)bars[0]->getPos()/ranges[0].divide);
            }

			if (event.GUIEvent.Caller == bars[1]) {
				devices->getDOF()->focus = (f32)((f32)bars[1]->getPos()/ranges[1].divide);
            }

			if (event.GUIEvent.Caller == bars[2]) {
				devices->getDOF()->sampleDist1 = (f32)((f32)bars[2]->getPos()/ranges[2].divide);
				devices->getDOF()->sampleDist0 = devices->getDOF()->sampleDist1+0.02f;
			}

			if (event.GUIEvent.Caller == bars[3]) {
				devices->getDOF()->distanceScale = (f32)((f32)bars[3]->getPos()/ranges[3].divide);
            }
            
            stringw text = L"Range :";
            text += devices->getDOF()->range;
            devices->getDevice()->getLogger()->log(text.c_str());
            text = L"Focus :";
            text += devices->getDOF()->focus;
            devices->getDevice()->getLogger()->log(text.c_str());
            text = L"sampleDist1 :";
            text += devices->getDOF()->sampleDist1;
            devices->getDevice()->getLogger()->log(text.c_str());
            text = L"sampleDist0 :";
            text += devices->getDOF()->sampleDist0;
            devices->getDevice()->getLogger()->log(text.c_str());
            text = L"distanceScale :";
            text += devices->getDOF()->distanceScale;
            devices->getDevice()->getLogger()->log(text.c_str());
		}
	}

	return false;
}

#endif
