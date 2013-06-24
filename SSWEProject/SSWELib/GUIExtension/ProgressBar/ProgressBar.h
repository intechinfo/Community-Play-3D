/**
 * CProgressBar.h
 *
 * Tank @ War Project
 * April 2011
 */

#ifndef __C_PROGRESS_BAR_H_INCLUDED__
#define __C_PROGRESS_BAR_H_INCLUDED__

#include <irrlicht.h>

using namespace irr;

class CProgressBar: public irr::gui::IGUIElement {

public:

	enum Direction {
		HORIZONTAL,
		VERTICAL
	};

private:

	gui::IGUIFont* font;
	bool showText;
	u32 percentage;
	Direction direction;
	u32 borderSize;

	video::SColor fillColor;
	video::SColor backgroundColor;
	video::SColor borderColor;
	video::SColor textColor;

	video::ITexture* fillTexture;
	video::ITexture* backgroundTexture;

	video::ITexture* texture;
	bool update;
public:

	CProgressBar(gui::IGUIEnvironment* environment, s32 id = -1, gui::IGUIElement *parent=0);
	virtual ~CProgressBar();

	u32 getPercentage() const;
	void setPercentage(const u32 percentage);

	CProgressBar::Direction getDirection();
	void setDirection(Direction direction);

	void setWidth(const u32 width);
	void setHeight(const u32 height);

	core::recti getDimension();
	void setDimension(const u32 width, const u32 height);
	void setDimension(core::recti dimension);

	core::vector2di getPosition() const;
	void setPosition(core::vector2di position);

	void setBorderSize(u32 size);

	video::SColor getFillColor();
	void setFillColor(video::SColor fillColor);

	void setBackgroundColor(video::SColor backgroundColor);

	void setBorderColor(video::SColor borderColor);

	void setTextColor(video::SColor textColor);
	void setColors(video::SColor fillColor, video::SColor backgroundColor, video::SColor borderColor, video::SColor textColor);

	void setFillTexture(video::ITexture* texture);

	void setBackgroundTexture(video::ITexture* texture);

	bool isShowText();
	void setShowText(bool show);

	void setFont(gui::IGUIFont* font);

	virtual void draw();

private:

	void drawProgressBar();
	void updateProgressBar();

	void updateRenderTargetTexture();
};

#endif
