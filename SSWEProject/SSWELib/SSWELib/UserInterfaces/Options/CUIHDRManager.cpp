/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include "CUIHDRManager.h"

struct SHDRDataStruct {
public:

	SHDRDataStruct(stringw name, f32 divider, rect<s32> position, IGUIEnvironment *gui, IGUIElement *parent, u32 index) {
		gui->addStaticText(name.c_str(), rect<s32>(position.UpperLeftCorner.X, position.UpperLeftCorner.Y,
												   position.LowerRightCorner.X, position.LowerRightCorner.Y-10),
						   true, true, parent, -1, true);
		valuebox = gui->addEditBox(L"0", rect<s32>(position.LowerRightCorner.X, position.UpperLeftCorner.Y,
												   parent->getRelativePosition().getWidth(),
												   position.LowerRightCorner.Y-10),
								  true, parent, -1);
		valuebar = gui->addScrollBar(true, rect<s32>(position.UpperLeftCorner.X, position.LowerRightCorner.Y-5,
													 parent->getRelativePosition().getWidth(),
													 position.LowerRightCorner.Y+10),
									 parent, index);
		valuebar->setMin(0);
		valuebar->setMax(1 * divider);

		this->divider = divider;
	}

	void setDivider(f32 divider) {
		this->divider = divider;
		valuebar->setMax(1 * divider);
	}

	IGUIEditBox *valuebox;
	IGUIScrollBar *valuebar;
	f32 divider;

};

CUIHDRManager::CUIHDRManager(CDevices *_devices) {
	devices = _devices;
	IGUIEnvironment *gui = devices->getGUIEnvironment();
	IVideoDriver *driver = devices->getVideoDriver();

	//-----------------------------------
    //GUI ELEMENTS
	window = gui->addWindow(rect<s32>(340, 140, 810, 420), false, L"HDR Manager", 0, -1);
	devices->getCore()->maximizeWindow(window, rect<s32>(340, 140, 810, 420));
	window->setRelativePosition(rect<s32>(driver->getScreenSize().Width-350, window->getRelativePosition().UpperLeftCorner.Y,
										  driver->getScreenSize().Width, window->getRelativePosition().LowerRightCorner.Y));

	const char *names[9] = {
		"Brightness Threshold",
		"Gaussian Coefficient",
		"Gaussian Mean",
		"Gaussian Standard Deviation",
		"Exposure",
		"Minimum Luminance",
		"Maximum Luminance",
		"Luminance IncreaseRate",
		"Luminance DecreaseRate"
	};

	for (u32 i=0; i < 9; i++) {
		SHDRDataStruct t(names[i], 1000.f, rect<s32>(10, 30+70*i, 160, 30+70*i+30), gui, window, i);
		hdrStructs.push_back(t);
	}

	fillField(0, devices->getXEffect()->getHDRManager()->getBrightnessThreshold());
	fillField(1, devices->getXEffect()->getHDRManager()->getGaussianCoefficient());
	fillField(2, devices->getXEffect()->getHDRManager()->getGaussianMean());
	fillField(3, devices->getXEffect()->getHDRManager()->getGaussianStandardDeviation());
	fillField(4, devices->getXEffect()->getHDRManager()->getExprosure());
	fillField(5, devices->getXEffect()->getHDRManager()->getMinimumLuminance());

	hdrStructs[6].valuebar->setMax(1e20f);
	hdrStructs[6].divider = 1;
	fillField(6, devices->getXEffect()->getHDRManager()->getMaximumLuminance());

	hdrStructs[7].valuebar->setMax(100);
	hdrStructs[7].divider = 1;
	fillField(7, devices->getXEffect()->getHDRManager()->getLuminanceIncreaseRate());
	
	hdrStructs[8].valuebar->setMax(100);
	hdrStructs[8].divider = 1;
	fillField(8, devices->getXEffect()->getHDRManager()->getLumnanceDecreaseRate());

	//-----------------------------------

	devices->getEventReceiver()->AddEventReceiver(this, window);
}

CUIHDRManager::~CUIHDRManager() {

}

void CUIHDRManager::fillField(u32 index, f32 value) {
	hdrStructs[index].valuebar->setPos(value * hdrStructs[index].divider);
	hdrStructs[index].valuebox->setText(devices->getCore()->getStrNumber(value).c_str());
}

bool CUIHDRManager::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			hdrStructs.clear();
			devices->getEventReceiver()->RemoveEventReceiver(this);
			delete this;
		}

		if (event.GUIEvent.EventType == EGET_SCROLL_BAR_CHANGED) {
			/// Check if the scrool bar is in a parent context
			if (event.GUIEvent.Caller && event.GUIEvent.Caller->getParent() && event.GUIEvent.Caller->getParent() == window) {
				
				///Just switch ... -_-
				IGUIScrollBar *bar = (IGUIScrollBar*)event.GUIEvent.Caller;

				hdrStructs[bar->getID()].valuebox->setText(devices->getCore()->getStrNumber(f32((f32)bar->getPos() / hdrStructs[bar->getID()].divider)).c_str());
				f32 value = f32((f32)bar->getPos() / hdrStructs[bar->getID()].divider);
				switch (bar->getID()) {
					case 0:
						devices->getXEffect()->getHDRManager()->setBrightnessThreshold(value);
					break;
					case 1:
						devices->getXEffect()->getHDRManager()->setGaussianCoefficient(value);
					break;
					case 2:
						devices->getXEffect()->getHDRManager()->setGaussianMean(value);
					break;
					case 3:
						devices->getXEffect()->getHDRManager()->setGaussianStandardDeviation(value);
					break;
					case 4:
						devices->getXEffect()->getHDRManager()->setExposure(value);
					break;
					case 5:
						devices->getXEffect()->getHDRManager()->setMinimumLuminance(value);
					break;
					case 6:
						devices->getXEffect()->getHDRManager()->setMaximumLuminance(value);
					break;
					case 7:
						devices->getXEffect()->getHDRManager()->setLuminanceIncreaseRate(value);
					break;
					case 8:
						devices->getXEffect()->getHDRManager()->setLuminanceDecreaseRate(value);
					break;

					default:break;
				}

			}
		}
	}

	return false;
}
