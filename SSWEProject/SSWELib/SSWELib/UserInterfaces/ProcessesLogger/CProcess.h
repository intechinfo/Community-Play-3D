//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_PROCESSES_H_INCLUDED__
#define __C_PROCESSES_H_INCLUDED__

#include "../../../../SSWECore/SSWECore/Core/CCore.h"

#include "../../GUIExtension/ProgressBar/ProgressBar.h"

class CProcess {

public:

	CProcess(IGUIEnvironment *_gui, stringw _name);
	~CProcess();

	void setName(stringw _name);
	stringw getName() { return name; }

	bool hasFinished() { return finished; }
	void setHasFinished(bool _hasFinished) { finished = _hasFinished; }

	IGUIStaticText *getNameTxt() { return nameTxt; }
	IGUIStaticText *getBackground() { return bg; }

	CProgressBar *getProgressBar() { return bar; }

private:

	IGUIEnvironment *gui;

	stringw name;
	bool finished;

	IGUIStaticText *nameTxt, *bg;
	CProgressBar *bar;

};

#endif