#ifndef __I_TOOLS_H_INCLUDED__
#define __I_TOOLS_H_INCLUDED__

#include <irrlicht.h>

class ITool
{
public: 
	virtual void resize(irr::core::rect<s32> position) = 0;
	virtual void resize(irr::core::position2di position) = 0;

	virtual irr::gui::IGUIElement *getWindow() = 0;

	virtual void setVisible(bool visible) = 0;

	virtual void close() = 0;
};

#endif