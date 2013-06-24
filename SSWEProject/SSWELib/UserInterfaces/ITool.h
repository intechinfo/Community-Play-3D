#ifndef ITOOL_H
#define ITOOL_H

class ITool
{
public: 
	virtual void resize(rect<s32> position) = 0;
	virtual void setVisible(bool visible) = 0;
	virtual void close() = 0;
};

#endif