//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_CODE_EDITOR_H_INCLUDED__
#define __C_UI_CODE_EDITOR_H_INCLUDED__

#include "../../Device/CDevices.h"

#include "../ITool.h"
#include "../../GUIExtension/CodeEditor/CGUICodeEditor.h"

class CUICodeEditor : public IEventReceiver, public ITool {

public:

	CUICodeEditor(CDevices *_devices, stringw *code, bool _single=true);
	~CUICodeEditor();

	void setEnabled(bool _enabled) { codeEditor->setEnabled(_enabled); }
	void setAutoSave(bool _autoSave);
	void setAlwaysBringToFront(bool _alwaysBringToFront) { alwaysBringToFront = _alwaysBringToFront; }

	void setTextData(stringw *code);

	//EVENT RECEIVER
	bool OnEvent(const SEvent &event);

	//ITOOL
	void resize(rect<s32> position);
	void resize(position2di position);
	void setVisible(bool visible);
	void close();

	//-----------------------------------
    //METHODS
	void maximize(rect<s32> position);
	//-----------------------------------

private:

	//-----------------------------------
    //DEVICES AND DATAS
	CDevices *devices;

	IGUIEnvironment *gui;

	bool autoSave, single, alwaysBringToFront;
	stringw *textData;
	//-----------------------------------

	//-----------------------------------
    //GUI ELEMENTS
	IGUIWindow *window;
	IGUIContextMenu *menu;

	CGUIEditBoxIRB *codeEditor;

	IGUIFileOpenDialog *openFromFileDialog;
	//-----------------------------------



};

#endif