/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include "CUIWindowEditClouds.h"

CUIWindowEditClouds::CUIWindowEditClouds(CDevices *_devices) {
	devices = _devices;

	//GUI ELEMENTS
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	window = gui->addWindow(rect<s32>(450, 130, 1100, 557), false, L"Edit Clouds...", 0, -1);
	window->getMinimizeButton()->setVisible(true);
	devices->getCore()->centerWindow(window, devices->getVideoDriver()->getScreenSize());

	//LEFT
	gui->addStaticText(L"Clouds : ", rect<s32>(10, 30, 210, 50), true, true, window, -1, false);
	removeCloud = gui->addButton(rect<s32>(210, 30, 230, 50), window, -1, L"-", L"Remove Selected Cloud");
	addCloud = gui->addButton(rect<s32>(230, 30, 250, 50), window, -1, L"+", L"Add new cloud with default values");

	clouds = gui->addListBox(rect<s32>(10, 50, 250, 410), window, -1, true);
	for (u32 i=0; i < devices->getCoreData()->getCloudsData()->size(); i++) {
		clouds->addItem(stringw(devices->getCoreData()->getCloudsData()->operator[](i).getCloudSceneNode()->getName()).c_str());
	}
	if (devices->getCoreData()->getCloudsData()->size() > 0)
		clouds->setSelected(0);
	else
		clouds->setSelected(-1);

	gui->addStaticText(L"", rect<s32>(260, 30, 270, 410), true, true, window, -1, false);

	//RIGHT
	tabctrl = gui->addTabControl(rect<s32>(280, 30, 640, 410), window, true, true, -1);
	generalTab = tabctrl->addTab(L"General");
	colorsTab = tabctrl->addTab(L"Colors");

	gui->addStaticText(L"Translation :", rect<s32>(280, 30, 570, 50), true, true, generalTab, -1, false);
	gui->addStaticText(L"X :", rect<s32>(290, 50, 320, 70), true, true, generalTab, -1, false);
	translationX = gui->addEditBox(L"", rect<s32>(320, 50, 570, 70), true, generalTab, -1);
	gui->addStaticText(L"X :", rect<s32>(290, 70, 320, 90), true, true, generalTab, -1, false);
	translationY = gui->addEditBox(L"", rect<s32>(320, 70, 570, 90), true, generalTab, -1);

	gui->addStaticText(L"Texture Scale :", rect<s32>(280, 90, 370, 110), true, true, generalTab, -1, false);
	textureScale = gui->addEditBox(L"", rect<s32>(370, 90, 560, 110), true, generalTab, -1);

	gui->addStaticText(L"Radius :", rect<s32>(280, 120, 570, 140), true, true, generalTab, -1, false);
	gui->addStaticText(L"Inner :", rect<s32>(290, 140, 340, 160), true, true, generalTab, -1, false);
	innerRadius = gui->addEditBox(L"", rect<s32>(340, 140, 570, 160), true, generalTab, -1);
	gui->addStaticText(L"Outer :", rect<s32>(290, 160, 340, 180), true, true, generalTab, -1, false);
	outerRadius = gui->addEditBox(L"", rect<s32>(340, 160, 570, 180), true, generalTab, -1);

	gui->addStaticText(L"Heights :", rect<s32>(280, 200, 570, 220), true, true, generalTab, -1, false);
	gui->addStaticText(L"Center :", rect<s32>(290, 220, 340, 240), true, true, generalTab, -1, false);
	centerHeight = gui->addEditBox(L"", rect<s32>(340, 220, 570, 240), true, generalTab, -1);
	gui->addStaticText(L"Inner :", rect<s32>(290, 240, 340, 260), true, true, generalTab, -1, false);
	innerHeight = gui->addEditBox(L"", rect<s32>(340, 240, 570, 260), true, generalTab, -1);
	gui->addStaticText(L"Outer :", rect<s32>(290, 260, 340, 280), true, true, generalTab, -1, false);
	outerHeight = gui->addEditBox(L"", rect<s32>(340, 260, 570, 280), true, generalTab, -1);
	for (IGUIElement *it : generalTab->getChildren()) {
		it->setRelativePosition(position2di(it->getRelativePosition().UpperLeftCorner.X - 270, it->getRelativePosition().UpperLeftCorner.Y));
	}

	gui->addStaticText(L"Center Color :", rect<s32>(10, 20, 350, 40), true, true, colorsTab, -1, false);
	gui->addStaticText(L"Red :", rect<s32>(30, 40, 90, 60), true, true, colorsTab, -1, false);
	centerColorRed = gui->addEditBox(L"", rect<s32>(90, 40, 350, 60), true, colorsTab, -1);
	gui->addStaticText(L"Green :", rect<s32>(30, 60, 90, 80), true, true, colorsTab, -1, false);
	centerColorGreen = gui->addEditBox(L"", rect<s32>(90, 60, 350, 80), true, colorsTab, -1);
	gui->addStaticText(L"Blue :", rect<s32>(30, 80, 90, 100), true, true, colorsTab, -1, false);
	centerColorBlue = gui->addEditBox(L"", rect<s32>(90, 80, 350, 100), true, colorsTab, -1);
	gui->addStaticText(L"Alpha :", rect<s32>(30, 100, 90, 120), true, true, colorsTab, -1, false);
	centerColorAlpha = gui->addEditBox(L"", rect<s32>(90, 100, 350, 120), true, colorsTab, -1);

	gui->addStaticText(L"Inner Color :", rect<s32>(10, 120, 350, 140), true, true, colorsTab, -1, false);
	gui->addStaticText(L"Red :", rect<s32>(30, 140, 90, 160), true, true, colorsTab, -1, false);
	innerColorRed = gui->addEditBox(L"", rect<s32>(90, 140, 350, 160), true, colorsTab, -1);
	gui->addStaticText(L"Green :", rect<s32>(30, 160, 90, 180), true, true, colorsTab, -1, false);
	innerColorGreen = gui->addEditBox(L"", rect<s32>(90, 160, 350, 180), true, colorsTab, -1);
	gui->addStaticText(L"Blue :", rect<s32>(30, 180, 90, 200), true, true, colorsTab, -1, false);
	innerColorBlue = gui->addEditBox(L"", rect<s32>(90, 180, 350, 200), true, colorsTab, -1);
	gui->addStaticText(L"Alpha :", rect<s32>(30, 200, 90, 220), true, true, colorsTab, -1, false);
	innerColorAlpha = gui->addEditBox(L"", rect<s32>(90, 200, 350, 220), true, colorsTab, -1);

	gui->addStaticText(L"Inner Color :", rect<s32>(10, 220, 350, 240), true, true, colorsTab, -1, false);
	gui->addStaticText(L"Red :", rect<s32>(30, 240, 90, 260), true, true, colorsTab, -1, false);
	outerColorRed = gui->addEditBox(L"", rect<s32>(90, 240, 350, 260), true, colorsTab, -1);
	gui->addStaticText(L"Green :", rect<s32>(30, 260, 90, 280), true, true, colorsTab, -1, false);
	outerColorGreen = gui->addEditBox(L"", rect<s32>(90, 260, 350, 280), true, colorsTab, -1);
	gui->addStaticText(L"Blue :", rect<s32>(30, 280, 90, 300), true, true, colorsTab, -1, false);
	outerColorBlue = gui->addEditBox(L"", rect<s32>(90, 280, 350, 300), true, colorsTab, -1);
	gui->addStaticText(L"Alpha :", rect<s32>(30, 300, 90, 320), true, true, colorsTab, -1, false);
	outerColorAlpha = gui->addEditBox(L"", rect<s32>(90, 300, 350, 320), true, colorsTab, -1);

	//DATAS
	devices->getEventReceiver()->AddEventReceiver(this, window);

	if (clouds->getSelected() == 0) {
		fillContents(devices->getCoreData()->getCloudsData()->operator[](0).getCloudSceneNode());
	} else {
		tabctrl->setEnabled(false);
	}
}

CUIWindowEditClouds::~CUIWindowEditClouds() {

}

void CUIWindowEditClouds::fillContents(CCloudSceneNode *node) {
	translationX->setText(stringw(node->getTranslation().X).c_str());
	translationY->setText(stringw(node->getTranslation().Y).c_str());

	textureScale->setText(stringw(node->getTextureScale()).c_str());

	innerRadius->setText(stringw(node->getInnerRadius()).c_str());
	outerRadius->setText(stringw(node->getOuterRadius()).c_str());

	centerHeight->setText(stringw(node->getCenterHeight()).c_str());
	innerHeight->setText(stringw(node->getInnerHeight()).c_str());
	outerHeight->setText(stringw(node->getOuterHeight()).c_str());

	centerColorRed->setText(stringw(node->getCenterColor().getRed()).c_str());
	centerColorGreen->setText(stringw(node->getCenterColor().getGreen()).c_str());
	centerColorBlue->setText(stringw(node->getCenterColor().getBlue()).c_str());
	centerColorAlpha->setText(stringw(node->getCenterColor().getAlpha()).c_str());

	innerColorRed->setText(stringw(node->getInnerColor().getRed()).c_str());
	innerColorGreen->setText(stringw(node->getInnerColor().getGreen()).c_str());
	innerColorBlue->setText(stringw(node->getInnerColor().getBlue()).c_str());
	innerColorAlpha->setText(stringw(node->getInnerColor().getAlpha()).c_str());

	outerColorRed->setText(stringw(node->getOuterColor().getRed()).c_str());
	outerColorGreen->setText(stringw(node->getOuterColor().getGreen()).c_str());
	outerColorBlue->setText(stringw(node->getOuterColor().getBlue()).c_str());
	outerColorAlpha->setText(stringw(node->getOuterColor().getAlpha()).c_str());
}

bool CUIWindowEditClouds::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			//CLOSE WINDOW
			if (event.GUIEvent.Caller == window) {
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
		}

		if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED && clouds->getSelected() != -1) {
			IGUIEditBox *caller = (IGUIEditBox*)event.GUIEvent.Caller;
			if (caller == translationX ||caller == translationY) {
				f32 valuex = devices->getCore()->getF32(translationX->getText());
				f32 valuey = devices->getCore()->getF32(translationY->getText());
				devices->getCoreData()->getCloudsData()->operator[](clouds->getSelected()).getCloudSceneNode()->setTranslation(vector2df(valuex, valuey));
			}

			if (caller == textureScale) {
				f32 value = devices->getCore()->getF32(textureScale->getText());
				devices->getCoreData()->getCloudsData()->operator[](clouds->getSelected()).getCloudSceneNode()->setTextureScale(value);
			}

			if (caller == innerRadius || caller == outerRadius) {
				f32 valuex = devices->getCore()->getF32(innerRadius->getText());
				f32 valuey = devices->getCore()->getF32(outerRadius->getText());
				devices->getCoreData()->getCloudsData()->operator[](clouds->getSelected()).getCloudSceneNode()->setCloudRadius(valuex, valuey);
			}

			if (caller == centerHeight || caller == innerHeight || caller == outerHeight) {
				f32 valuex = devices->getCore()->getF32(centerHeight->getText());
				f32 valuey = devices->getCore()->getF32(innerHeight->getText());
				f32 valuez = devices->getCore()->getF32(outerHeight->getText());
				devices->getCoreData()->getCloudsData()->operator[](clouds->getSelected()).getCloudSceneNode()->setCloudHeight(valuex, valuey, valuez);
			}

			if (caller == centerColorRed || caller == centerColorGreen || caller == centerColorBlue || caller == centerColorAlpha
				|| caller == innerColorRed || caller == innerColorGreen || caller == innerColorBlue ||caller == innerColorAlpha
				|| caller == outerColorRed || caller == outerColorGreen || caller == outerColorBlue || caller == outerColorAlpha) {
				f32 cr = devices->getCore()->getF32(centerColorRed->getText());
				f32 cg = devices->getCore()->getF32(centerColorGreen->getText());
				f32 cb = devices->getCore()->getF32(centerColorBlue->getText());
				f32 ca = devices->getCore()->getF32(centerColorAlpha->getText());

				f32 ir = devices->getCore()->getF32(innerColorRed->getText());
				f32 ig = devices->getCore()->getF32(innerColorGreen->getText());
				f32 ib = devices->getCore()->getF32(innerColorBlue->getText());
				f32 ia = devices->getCore()->getF32(innerColorAlpha->getText());

				f32 o_r = devices->getCore()->getF32(outerColorRed->getText());
				f32 og = devices->getCore()->getF32(outerColorGreen->getText());
				f32 ob = devices->getCore()->getF32(outerColorBlue->getText());
				f32 oa = devices->getCore()->getF32(outerColorAlpha->getText());

				devices->getCoreData()->getCloudsData()->operator[](clouds->getSelected()).getCloudSceneNode()->setCloudColor(SColor(ca, cr, cg, cb),
																															  SColor(ia, ir, ig, ib),
																															  SColor(oa, o_r, og, ob));
			}
		}

		if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
			if (event.GUIEvent.Caller == clouds) {
				if (clouds->getSelected() != -1) {
					fillContents(devices->getCoreData()->getCloudsData()->operator[](clouds->getSelected()).getCloudSceneNode());
				}
			}
		}

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			//ADD / REMOVE
			if (event.GUIEvent.Caller == removeCloud) {
				if (clouds->getSelected() != -1) {
					devices->getCoreData()->getCloudsData()->operator[](clouds->getSelected()).getCloudSceneNode()->remove();
					devices->getCoreData()->getCloudsData()->erase(clouds->getSelected());
					clouds->removeItem(clouds->getSelected());
					clouds->setSelected(0);
					if (clouds->getItemCount() == 0)
						tabctrl->setEnabled(false);
				}
			}

			if (event.GUIEvent.Caller == addCloud) {
				CCloudSceneNode *cloudLayer = new scene::CCloudSceneNode(devices->getRenderingSceneManager()->getRootSceneNode(),
																			devices->getRenderingSceneManager());
				cloudLayer->setName("New Cloud Layer");
				devices->getCoreData()->getCloudsData()->push_back(SCloudData(cloudLayer));
				clouds->addItem(L"New Cloud Layer");
				clouds->setSelected(clouds->getItemCount()-1);
				tabctrl->setEnabled(true);
			}
		}
	}

	return false;
}
