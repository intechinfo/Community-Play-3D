/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include "CUILightMapsCreator.h"

//LightMaps
#include "CRadiosityGenerator.h"

CUILightsMapCreator::CUILightsMapCreator(CDevices *_devices, SData *data) {
	devices = _devices;
	this->data = data;

	IGUIEnvironment *gui = devices->getGUIEnvironment();

	//GUI ELEMENTS
	window = gui->addWindow(rect<s32>(510, 160, 1006, 660), false, L"Lights Map Creator", 0, -1);
	window->getCloseButton()->setVisible(false);
	window->getMinimizeButton()->setVisible(true);

	stringw nodeName = "Node : ";
	nodeName += data->getNode()->getName();
	gui->addStaticText(nodeName.c_str(), rect<s32>(10, 30, 490, 50), true, true, window, -1, true);

	gui->addStaticText(L"Sampling Factor :", rect<s32>(10, 70, 140, 90), true, true, window, -1, true);
	samplingFactoreb = gui->addEditBox(L"3.0", rect<s32>(140, 70, 240, 90), true, window, -1);

	gui->addStaticText(L"Occlusion Accuracy :", rect<s32>(10, 90, 140, 110), true, true, window, -1, true);
	occlusionAccuracyeb = gui->addEditBox(L"4096", rect<s32>(140, 90, 240, 110), true, window, -1);

	gui->addStaticText(L"Diffuse Smooth :", rect<s32>(10, 110, 140, 130), true, true, window, -1, true);
	diffuseSmootheb = gui->addEditBox(L"8", rect<s32>(140, 110, 240, 130), true, window, -1);

	gui->addStaticText(L"Occlusion Angle :", rect<s32>(10, 130, 140, 150), true, true, window, -1, true);
	occlusionAngleeb = gui->addEditBox(L"100.0", rect<s32>(140, 130, 240, 150), true, window, -1);

	gui->addStaticText(L"Bounces :", rect<s32>(10, 170, 140, 190), true, true, window, -1, true);
	bounceseb = gui->addEditBox(L"3", rect<s32>(140, 170, 240, 190), true, window, -1);

	gui->addStaticText(L"Accuracy :", rect<s32>(10, 190, 140, 210), true, true, window, -1, true);
	accuracyeb = gui->addEditBox(L"85", rect<s32>(140, 190, 240, 210), true, window, -1);

	closeb = gui->addButton(rect<s32>(380, 460, 480, 490), window, -1, L"Close", L"Close this window");
	calculateb = gui->addButton(rect<s32>(390, 210, 490, 240), window, -1, L"Calculate !", L"Calculate lightmaps and replace...");

	logeb = new CGUIEditBoxIRB(L"Ready.\n", true, true, devices->getGUIEnvironment(), window, -1, rect<s32>(10, 240, 490, 440), devices->getDevice());
	logeb->setMultiLine(true);
	logeb->clearKeywords();
    logeb->setOverrideColor(SColor(180, 32, 32, 32));
    logeb->setBackgroundColor(SColor(255, 200, 200, 200));
    logeb->setLineCountColors(SColor(255, 32, 32, 32), SColor(200, 64, 120, 180), SColor(255, 200, 200, 224));
    logeb->setSelectionColors(SColor(180, 0, 96, 64), SColor(255, 255, 255, 255), SColor(180, 0, 128, 96));
	logeb->addKeyword("Ready.", SColor(255, 0, 255, 150), true);
	logeb->addKeyword("Bounce...", SColor(255, 255, 0, 150), true);
	logeb->addKeyword("Success.", SColor(0, 255, 0, 150), true);

	//END GUI ELEMENTS

	devices->getEventReceiver()->AddEventReceiver(this, window);
}

CUILightsMapCreator::~CUILightsMapCreator() {

}

bool CUILightsMapCreator::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == closeb) {
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}

			if (event.GUIEvent.Caller == calculateb) {
				//LOG LEVEL
				ELOG_LEVEL level = devices->getDevice()->getLogger()->getLogLevel();
				devices->getDevice()->getLogger()->setLogLevel(ELL_INFORMATION);

				//SHADOW LIGHTS
				u32 occlusionAccuracy2 = devices->getCore()->getU32(occlusionAccuracyeb->getText());
				occlusionAccuracy = occlusionAccuracy2;
				array<SShadowLight> lights;
				for (u32 i=0; i < devices->getXEffect()->getShadowLightCount(); i++) {
					devices->getXEffect()->getShadowLight(i).setShadowMapResolution(occlusionAccuracy2);
					lights.push_back(devices->getXEffect()->getShadowLight(i));
				}

				//CREATE WHITE TEXTURE
				IImage *blankTexture = devices->getVideoDriver()->createImage(ECF_A8R8G8B8, dimension2du(256, 256));
				for (u32 i=0; i < blankTexture->getDimension().Width; i++) {
					for (u32 j=0; j < blankTexture->getDimension().Height; j++) {
						blankTexture->setPixel(i, j, SColor(255, 255, 255, 255));
					}
				}
				ITexture *tex = devices->getVideoDriver()->addTexture("blank_rad_gpu_tex", blankTexture);
				for (u32 i=0; i < this->data->getMesh()->getMeshBufferCount(); i++) {
					this->data->getMesh()->getMeshBuffer(i)->getMaterial().setTexture(1, tex);
				}

				//CALCULATE
				myNode = new CRadiosityGenerator(devices->getSceneManager(), devices->getXEffect());
				myNode->setFilterType(devices->getXEffectFilterType());
				myNode->addRadiosityNode((IMeshSceneNode*)this->data->getNode());

				myNode->calculateDirectLighting(devices->getDevice()->getTimer()->getTime());
				myNode->calculateInDirectLighting(0,devices->getDevice()->getTimer());
                for(int i=0; i<bounces-1; i++)
					myNode->calculateInDirectLighting(bounces,devices->getDevice()->getTimer());

                myNode->saveLightMaps();

				//delete myNode;
				//RESTORE
				devices->getDevice()->getLogger()->setLogLevel(level);

				for (u32 i=0; i < lights.size(); i++) {
					devices->getXEffect()->addShadowLight(lights[i]);
				}
			}
		}
	}

	return false;
}

void CUILightsMapCreator::log(stringw text) {
	stringw etext = logeb->getText();
	etext += "\n";
	etext += text;
	logeb->setText(etext.c_str());
}

void CUILightsMapCreator::buildLightMaps() {

}
