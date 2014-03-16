#ifndef __I_GUI_CODE_EDITOR_H_INCLUDED__
#define __I_GUI_CODE_EDITOR_H_INCLUDED__

#include <irrlicht.h>

#include <ITool.h>

class IGUICodeEditor : public ITool {

public:

	//CODE EDITOR CLASS
	virtual void setEnabled(bool _enabled) = 0;
	virtual void setAutoSave(bool _autoSave) = 0;
	virtual void setAlwaysBringToFront(bool _alwaysBringToFront) = 0;

	virtual void setTextData(irr::core::stringw *code) = 0;

	//ITOOL
	virtual void resize(irr::core::rect<s32> position) = 0;
	virtual void resize(irr::core::position2di position) = 0;
	virtual irr::gui::IGUIElement *getWindow() = 0;
	virtual irr::gui::IGUIElement *getCodeEditorElement() = 0;
	virtual void setVisible(bool visible) = 0;
	virtual void close() = 0;

	//EDITOR
	virtual void addLUAKeywords() = 0;
	virtual void addCppKeywords() = 0;
	virtual void addShaderKeywords() = 0;
	virtual void addConstantsShaderKeyWords() = 0;
    virtual void setBackgroundColor(irr::video::SColor color = irr::video::SColor(255, 200, 200, 200)) = 0;

	virtual void addKeyword(irr::core::stringc keyword, irr::video::SColor color, bool matchCase) = 0;
	virtual void clearKeyWords() = 0;

};

#endif