//
//  CRenderCallback.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 28/11/12.
//
//

#ifndef __C_UI_SCRIPT_EDITOR_H_INCLUDED__
#define __C_UI_SCRIPT_EDITOR_H_INCLUDED__

#include "../../CDevices.h"

class CUIScriptEditor : public IEventReceiver {

public:

	CUIScriptEditor(CDevices *_devices);
	~CUIScriptEditor();

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
	//DATAS
	CDevices *devices;

	bool resizeCodeEditor;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	IGUIWindow *window;
	IGUIButton *runScript;

	IGUIEditBox *fileName;
	IGUIButton *addFile, *removeFile;
	IGUIListBox *files;

	CUICodeEditor *codeEditor;
	//-----------------------------------

};

#endif