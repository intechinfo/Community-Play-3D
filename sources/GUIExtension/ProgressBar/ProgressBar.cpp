#include "stdafx.h"
#include "ProgressBar.h"

using namespace irr;

CProgressBar::CProgressBar(gui::IGUIEnvironment* environment, s32 id, gui::IGUIElement *parent) :
		gui::IGUIElement(gui::EGUIET_ELEMENT, environment, parent, id, core::rect<s32>(10, 10, 110, 30)) {
	percentage = 0;
	direction = HORIZONTAL;

	borderSize = 1;

	fillColor = video::SColor(255, 0, 255, 255);
	backgroundColor = video::SColor(255, 255, 255, 255);
	borderColor = video::SColor(255, 0, 0, 0);
	textColor = video::SColor(255, 255, 255, 0);

	fillTexture = 0;
	backgroundTexture = 0;

	font = environment->getSkin()->getFont();
	showText = true;
	texture = 0;
	update = true;
}

CProgressBar::~CProgressBar() {
	if (fillTexture) {
		fillTexture->drop();
	}
	if (backgroundTexture) {
		backgroundTexture->drop();
	}
	if (texture) {
		texture->drop();
	}
}

u32 CProgressBar::getPercentage() const {
	return percentage;
}

void CProgressBar::setPercentage(const u32 percentage) {
	if (this->percentage != percentage) {
		this->percentage = core::clamp(percentage, (u32) 0, (u32) 100);
		update = true;
	}
}

CProgressBar::Direction CProgressBar::getDirection() {
	return direction;
}

void CProgressBar::setDirection(Direction direction) {
	this->direction = direction;
	update = true;
}

void CProgressBar::setWidth(const u32 width) {
	setDimension(width, RelativeRect.getHeight());
}

void CProgressBar::setHeight(const u32 height) {
	setDimension(RelativeRect.getWidth(), height);
}

core::recti CProgressBar::getDimension() {
	return RelativeRect;
}

void CProgressBar::setDimension(const u32 width, const u32 height) {
	setRelativePosition(core::recti(RelativeRect.UpperLeftCorner.X, RelativeRect.UpperLeftCorner.Y, RelativeRect.UpperLeftCorner.X + width, RelativeRect.UpperLeftCorner.Y + height));
	update = true;
}

void CProgressBar::setDimension(core::recti dimension) {
	setRelativePosition(dimension);
	update = true;
}

core::vector2di CProgressBar::getPosition() const {
	return RelativeRect.UpperLeftCorner;
}

void CProgressBar::setPosition(core::vector2di position) {
	setRelativePosition(position);
	update = true;
}

void CProgressBar::setBorderSize(u32 size) {
	if (this->borderSize != size) {
		borderSize = size;
		update = true;
	}
}

video::SColor CProgressBar::getFillColor() {
	return fillColor;
}

void CProgressBar::setFillColor(video::SColor fillColor) {
	if (this->fillColor != fillColor) {
		this->fillColor = fillColor;
		update = true;
	}
}

void CProgressBar::setBackgroundColor(video::SColor backgroundColor) {
	if (this->backgroundColor != backgroundColor) {
		this->backgroundColor = backgroundColor;
		update = true;
	}
}

void CProgressBar::setBorderColor(video::SColor borderColor) {
	if (this->borderColor != borderColor) {
		this->borderColor = borderColor;
		update = true;
	}
}

void CProgressBar::setTextColor(video::SColor textColor) {
	if (this->textColor != textColor) {
		this->textColor = textColor;
		update = true;
	}
}

void CProgressBar::setColors(video::SColor fillColor, video::SColor backgroundColor, video::SColor borderColor, video::SColor textColor) {
	this->fillColor = fillColor;
	this->backgroundColor = backgroundColor;
	this->borderColor = borderColor;
	this->textColor = textColor;
	update = true;
}

void CProgressBar::setFillTexture(video::ITexture* texture) {
	if (this->fillTexture) {
		this->fillTexture->drop();
	}
	this->fillTexture = texture;

	if (this->fillTexture) {
		this->fillTexture->grab();
	}
	update = true;
}

void CProgressBar::setBackgroundTexture(video::ITexture* texture) {
	if (this->backgroundTexture) {
		this->backgroundTexture->drop();
	}
	this->backgroundTexture = texture;

	if (this->backgroundTexture) {
		this->backgroundTexture->grab();
	}
	update = true;
}

bool CProgressBar::isShowText() {
	return showText;
}

void CProgressBar::setShowText(bool show) {
	this->showText = show;
	update = true;
}

void CProgressBar::setFont(gui::IGUIFont* font) {
	this->font = font;
	update = true;
}

void CProgressBar::draw() {
	if (!isVisible()) {
		return;
	}

	if (update) {
		updateProgressBar();
		update = false;
	}

	video::IVideoDriver* driver = Environment->getVideoDriver();
	if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET)) {
		const video::SColor color = video::SColor(255, 255, 255, 255);
		const core::vector2di upperLeftCorner = RelativeRect.UpperLeftCorner - borderSize;

		driver->draw2DImage(texture, upperLeftCorner, core::recti(0, 0, texture->getOriginalSize().Width, texture->getOriginalSize().Height), 0, color, true);
	} else {
		drawProgressBar();
	}
}

void CProgressBar::drawProgressBar() {
	video::IVideoDriver* driver = Environment->getVideoDriver();
	core::recti background = RelativeRect;
	core::recti fill = RelativeRect;

	if (direction == HORIZONTAL) {
		const s32 fillX = ((RelativeRect.getWidth() * percentage) / 100) + RelativeRect.UpperLeftCorner.X;
		fill.LowerRightCorner.X = fillX;
	} else {
		const s32 fillY = (RelativeRect.getHeight() - ((RelativeRect.getHeight() * percentage) / 100)) + RelativeRect.UpperLeftCorner.Y;
		fill.UpperLeftCorner.Y = fillY;
	}

	if (driver->getDriverType() == video::EDT_DIRECT3D9 || driver->getDriverType() == video::EDT_DIRECT3D8) {
		background.UpperLeftCorner -= 1;
		fill.UpperLeftCorner -= 1;
	}

	const video::SColor color = video::SColor(255, 255, 255, 255);
	const video::SColor colors[] = { color, color, color, color };

	if (!backgroundTexture) {
		driver->draw2DRectangle(backgroundColor, background);
	} else {
		driver->draw2DImage(backgroundTexture, background, RelativeRect - RelativeRect.UpperLeftCorner, 0, colors, true);
	}
	if (!fillTexture) {
		driver->draw2DRectangle(fillColor, fill);
	} else {
		driver->draw2DImage(fillTexture, fill, RelativeRect - RelativeRect.UpperLeftCorner, 0, colors, true);
	}

	for (u32 i = 0; i < borderSize; i++) {
		core::recti border = RelativeRect;
		border.UpperLeftCorner -= i + 1;
		border.LowerRightCorner += i;

		if (driver->getDriverType() == video::EDT_DIRECT3D9 || driver->getDriverType() == video::EDT_DIRECT3D8) {
			border.UpperLeftCorner += 1;
		}

		driver->draw2DRectangleOutline(border, borderColor);

		if (driver->getDriverType() == video::EDT_DIRECT3D9 || driver->getDriverType() == video::EDT_DIRECT3D8) {
			border.LowerRightCorner = border.UpperLeftCorner;
			border.UpperLeftCorner.X -= 1;
			driver->draw2DLine(border.UpperLeftCorner, border.LowerRightCorner, borderColor);
		}
	}

	if (showText) {
		core::stringw text = L"";
		text += percentage;
		text += " %";
		font->draw(text.c_str(), RelativeRect, textColor, true, true);
	}
}

void CProgressBar::updateProgressBar() {
	video::IVideoDriver* driver = Environment->getVideoDriver();
	core::position2di position = RelativeRect.UpperLeftCorner;

	if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET)) {
		setRelativePosition(core::position2di(borderSize, borderSize));
	}

	updateRenderTargetTexture();

	if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET)) {
		setRelativePosition(position);
	}
}

void CProgressBar::updateRenderTargetTexture() {
	video::IVideoDriver* driver = Environment->getVideoDriver();
	if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET)) {
		core::dimension2du dimension = core::dimension2du(RelativeRect.getWidth() + (borderSize * 2), RelativeRect.getHeight() + (borderSize * 2));
		if (!texture || (texture && texture->getOriginalSize() != dimension)) {
			texture = driver->addRenderTargetTexture(dimension, "RTT", video::ECF_A8R8G8B8);
		}
	}

	if (texture) {
		driver->setRenderTarget(texture, true, true, video::SColor(0, 0, 0, 0));
		drawProgressBar();
		driver->setRenderTarget(0, false, false, 0);
	}
}
