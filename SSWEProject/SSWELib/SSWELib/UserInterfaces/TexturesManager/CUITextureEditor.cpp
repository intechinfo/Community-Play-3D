//
//  CUIRenderingInfos.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//
#include "stdafx.h"
#include "CUITextureEditor.h"

CUITextureEditor::CUITextureEditor(CDevices *_devices, ITexture *texture) {
	devices = _devices;

	driver = devices->getVideoDriver();
	gui = devices->getGUIEnvironment();

	//SET UP TEXTURES
	textureToEdit = texture;

	stringc texturename = "texture_editor_texture_to_edit_";
	texturename += texture->getName().getPath().c_str();
	tempTexture = devices->getCore()->copyTexture(texturename, textureToEdit, driver);

	//SET UP GUI
	window = gui->addWindow(rect<s32>(630, 300, 1270, 750), false, L"Texture Editor", 0, -1);
	devices->getCore()->centerWindow(window, driver->getScreenSize());
	window->getMaximizeButton()->setVisible(true);
	window->getMinimizeButton()->setVisible(true);

	image = gui->addImage(rect<s32>(150, 30, 630, 440), window, -1, L"Texture to edit", true);
	image->setImage(tempTexture);
	image->setScaleImage(true);

	//TOOL BAR
	toolbartxt = gui->addStaticText(L"", rect<s32>(10, 30, 140, 440), true, true, window, -1, true);

	scaleImagecb = gui->addCheckBox(true, rect<s32>(0, 0, 130, 20), toolbartxt, -1, L"Scale Texture");

	gui->addStaticText(L"Selected Pixel :", rect<s32>(0, 30, 110, 50), true, true, toolbartxt, -1, true);
	selectedColorst = gui->addStaticText(L"", rect<s32>(110, 30, 130, 50), true, true, toolbartxt, -1, true);

	useAlphaChannelcb = gui->addCheckBox(true, rect<s32>(0, 60, 130, 80), toolbartxt, -1, L"Use Alpha Ch.");

	setSelectedColorAlphabtn = gui->addButton(rect<s32>(0, 90, 130, 110), toolbartxt, -1, L"Set Color Alpha", L"Set selected color as alpha for the texture");

	//EVENTS
	devices->getEventReceiver()->AddEventReceiver(this, window);
}

CUITextureEditor::~CUITextureEditor() {

}

bool CUITextureEditor::OnEvent(const SEvent &event) {

	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP) {
			if (gui->getFocus() == image) {
				position2di cursorPosition = position2di(event.MouseInput.X - window->getRelativePosition().UpperLeftCorner.X - image->getRelativePosition().UpperLeftCorner.X,
														event.MouseInput.Y - window->getRelativePosition().UpperLeftCorner.Y - image->getRelativePosition().UpperLeftCorner.Y);

				u32 pitch = tempTexture->getPitch();
				ECOLOR_FORMAT format = tempTexture->getColorFormat();
				u32 bytes = video::IImage::getBitsPerPixelFromFormat(format) / 8;

				unsigned char *buffer = (unsigned char *)tempTexture->lock();
				if (buffer) {
					if (image->isImageScaled()) {
						selectedPixelColor = SColor(*(unsigned int*)(buffer + ((cursorPosition.Y * pitch)) + ((cursorPosition.X * bytes))));
					} else {
						selectedPixelColor = SColor(*(unsigned int*)(buffer + (cursorPosition.Y * pitch) + (cursorPosition.X * bytes)));
					}
					tempTexture->unlock();
					selectedColorst->setBackgroundColor(selectedPixelColor);
				}
			}
		}
	}

	if (event.EventType == EET_GUI_EVENT) {

		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			//IF CLOSE WINDOW
			if (event.GUIEvent.Caller == window) {
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
		}

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == window->getMaximizeButton()) {
				devices->getCore()->maximizeWindow(window, rect<s32>(630, 300, 1270, 750));
				toolbartxt->setRelativePosition(rect<s32>(toolbartxt->getRelativePosition().UpperLeftCorner.X,
														  toolbartxt->getRelativePosition().UpperLeftCorner.Y,
														  toolbartxt->getRelativePosition().LowerRightCorner.X,
														  window->getRelativePosition().getHeight()-5));
				image->setRelativePosition(rect<s32>(image->getRelativePosition().UpperLeftCorner.X,
													 image->getRelativePosition().UpperLeftCorner.Y,
													 window->getRelativePosition().getWidth()-5,
													 window->getRelativePosition().getHeight()-5));
			}

			if (event.GUIEvent.Caller == setSelectedColorAlphabtn) {

			}
		}

		if (event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED) {
			if (event.GUIEvent.Caller == scaleImagecb)
				image->setScaleImage(scaleImagecb->isChecked());

			if (event.GUIEvent.Caller == useAlphaChannelcb)
				image->setUseAlphaChannel(useAlphaChannelcb->isChecked());
		}
	}
	
	return false;
}
