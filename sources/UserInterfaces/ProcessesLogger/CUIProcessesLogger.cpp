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
	if (processes.size() > 0) {
		window->setVisible(true);
		gui->getRootGUIElement()->bringToFront(window);
		window->setRelativePosition(position2di(gui->getVideoDriver()->getScreenSize().Width-window->getRelativePosition().getWidth(), 75));
		for (u32 i=0; i < processes.size(); i++) {
			if (processes[i].hasFinished()) {
				delete processes[i].getProcess();
				processes.erase(i);
				break;
			}
			processes[i].getProcess()->getBackground()->setRelativePosition(position2di(5, 30+i*60+10));
			processes[i].getProcess()->getNameTxt()->setRelativePosition(position2di(5, 3));
			processes[i].getProgressBar()->setPosition(vector2di(window->getRelativePosition().UpperLeftCorner.X+10,
																 window->getRelativePosition().UpperLeftCorner.Y+30+i*60+10
																 //+processes[i].getProcess()->getNameTxt()->getRelativePosition().getHeight()
																 +processes[i].getProcess()->getNameTxt()->getRelativePosition().getHeight()));
			gui->getRootGUIElement()->bringToFront(processes[i].getProgressBar());
		}
	} else {
		window->setVisible(false);
	}
}

void CUIProcessesLogger::addProcess(CProcess *process) {
	SProcess sp(process);

	window->addChild(process->getBackground());

	processes.push_back(sp);
}

bool CUIProcessesLogger::OnEvent(const SEvent &event) {

	return false;
}
