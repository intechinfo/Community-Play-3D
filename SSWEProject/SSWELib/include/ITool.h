#ifndef ITOOL_H
#define ITOOL_H

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