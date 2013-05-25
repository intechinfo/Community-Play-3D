//
//  CAction.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 07/11/12.
//
//

#include "stdafx.h"
#include "CManualAction.h"

CManualAction::CManualAction() {
	completed = false;
	started = false;

	actionName = "myName";
	timesToDo = 1;
	timeToComplete = 0;
	timeToStart = 0;
	actionType = EMAT_ROTATION;
	actionSens = EMAS_NEGATIVE;

	predecessors.clear();
}

CManualAction::~CManualAction() {

}
