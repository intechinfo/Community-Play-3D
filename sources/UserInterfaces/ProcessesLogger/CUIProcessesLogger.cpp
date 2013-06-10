//
//  CUIRenderingInfos.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//
#include "stdafx.h"
#include "CUIProcessesLogger.h"

CUIProcessesLogger::CUIProcessesLogger(IGUIEnvironment *_gui) {
	//-----------------------------------
	//INITIALIZATION
	gui = _gui;
	processes.clear();
	//-----------------------------------

	//-----------------------------------
	//GUI ASSETS
	window = gui->addWindow(rect<s32>(970, 120, 1250, 500), false, L"Processes", 0, -1);
	window->setDraggable(false);
	window->getCloseButton()->remove();
	//-----------------------------------
}

CUIProcessesLogger::~CUIProcessesLogger() {

}

void CUIProcessesLogger::update() {
	window->setRelativePosition(position2di(gui->getVideoDriver()->getScreenSize().Width-window->getRelativePosition().getWidth(), 75));
}

bool CUIProcessesLogger::OnEvent(const SEvent &event) {

	return false;
}
