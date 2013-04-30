//
//  CAction.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 07/11/12.
//
//

#ifndef __C_MANUAL_ACTION_H_INCLUDED__
#define __C_MANUAL_ACTION_H_INCLUDED__

#include <irrlicht.h>

enum E_MANUAL_ACTION_TYPE {
	EMAT_ROTATION = 0,
	EMAT_TRANSLATION,
	EMAT_SCALE
};

enum E_MANUAL_ACTION_SENS {
	EMAS_POSITIVE = 0,
	EMAS_NEGATIVE,
	EMAS_UNKNOWN
};

class CManualAction {
    
public:

    CManualAction();
    ~CManualAction();

	//METHODS

	//SET METHODS
	void setName(irr::core::stringw name) { actionName = name; }

	void setType(E_MANUAL_ACTION_TYPE type) { actionType = type; }
	void setSens(E_MANUAL_ACTION_SENS sens) { actionSens = sens; }

	void setTimeToComplete(irr::f32 time) { timeToComplete = time; }
	void setTimesToDo(irr::u32 times) { timesToDo = times; }
	void setTimeToStart(irr::s32 time) { timeToStart = time; }

	//GET METHODS
	irr::core::stringw getName() { return actionName; }

	E_MANUAL_ACTION_TYPE getType() { return actionType; }
	E_MANUAL_ACTION_SENS getSens() { return actionSens; }

	irr::f32 setTimeToComplete() { return timeToComplete; }
	irr::u32 getTimesToDo() { return timesToDo; }
	irr::s32 getTimeToStart() { return timeToStart; }

	irr::core::array<CManualAction *> getPredecessors() { return predecessors; }
	bool isCompleted() { return completed; }

private:

	irr::core::stringw actionName;

	E_MANUAL_ACTION_TYPE actionType;
	E_MANUAL_ACTION_SENS actionSens;

	irr::f32 timeToComplete;
	irr::u32 timesToDo;
	irr::s32 timeToStart;

	irr::core::array<CManualAction *> predecessors;

	bool completed;
	bool started;
};

#endif
