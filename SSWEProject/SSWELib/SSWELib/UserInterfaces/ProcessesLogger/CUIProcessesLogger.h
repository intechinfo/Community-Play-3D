//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_PROCESSES_LOGGER_H_INCLUDED__
#define __C_UI_PROCESSES_LOGGER_H_INCLUDED__

#include "../../../../SSWECore/SSWECore/Core/CCore.h"

#include "CProcess.h"

//---------------------------------------------------------------------------------------------
//-----------------------------------PROCESSES LOGGER STRUCT-----------------------------------
//---------------------------------------------------------------------------------------------

struct SProcess {
	
public:

	SProcess(CProcess *_process) {
		process = _process;
	}

	CProcess *getProcess() { return process; }
	bool hasFinished() { return process->hasFinished(); }
	CProgressBar *getProgressBar() { return process->getProgressBar(); }

private:

	CProcess *process;

};

//---------------------------------------------------------------------------------------------
//------------------------------------PROCESSES LOGGER CLASS-----------------------------------
//---------------------------------------------------------------------------------------------

class CUIProcessesLogger : public IEventReceiver {

public:

	CUIProcessesLogger(IGUIEnvironment *_gui);
	~CUIProcessesLogger();

	void update();

	void addProcess(CProcess *process);

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

