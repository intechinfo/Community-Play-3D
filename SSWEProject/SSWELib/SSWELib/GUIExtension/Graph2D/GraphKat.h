#ifndef __C_GRAPH_H_INCLUDED__
#define __C_GRAPH_H_INCLUDED__

#include <Irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

class CGraphGUI {
public:

	CGraphGUI(wchar_t* _caption, IrrlichtDevice* device, core::rect<s32> rect);
	~CGraphGUI();

	void setText(wchar_t* text);

	void addValue(u8 val);

	void draw();
	void drawOrange();
	void drawGreen();
	void drawBlue();
	void drawCrazy();
	void drawStrange();

	u8 getMax();

private:

	IVideoDriver* driver;
	IGUIEnvironment* guienv;
	IGUIFont* font;

	wchar_t* caption;
	array<u8> t;
	u16 L,T,W,H;
	u8 Max;
	SColor white;
	SColor start, end;

	void drawBorder();
};

#endif
