//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_PROCESSES_LOGGER_H_INCLUDED__
#define __C_UI_PROCESSES_LOGGER_H_INCLUDED__

#include "../../Device/Core/CCore.h"

#include "CProcess.h"

struct SProcess {
	
public:

	SProcess(stringw _name, std::thread _t) {
		process = new CProcess(_name);
	}

private:

	CProcess *process;

};

class CUIProcessesLogger : public IEventReceiver {

public:

	CUIProcessesLogger(IGUIEnvironment *_gui);
	~CUIProcessesLogger();

	void update();

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
	//GUI ASSETS
	IGUIEnvironment *gui;

	IGUIWindow *window;
	//-----------------------------------

	//-----------------------------------
	//PROCESSES
	array<SProcess> processes;
	//-----------------------------------

};

#endif

