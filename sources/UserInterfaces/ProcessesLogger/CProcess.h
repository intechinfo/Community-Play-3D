//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_PROCESSES_H_INCLUDED__
#define __C_PROCESSES_H_INCLUDED__

#include "../../Device/Core/CCore.h"

#include "../../GUIExtension/ProgressBar/ProgressBar.h"

class CProcess {

public:

	CProcess(stringw _name);
	~CProcess();

private:

	stringw name;
	bool finished;

};

#endif