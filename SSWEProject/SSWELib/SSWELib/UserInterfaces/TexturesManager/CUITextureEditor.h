//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_TEXTURES_EDITOR_H_INCLUDED__
#define __C_UI_TEXTURES_EDITOR_H_INCLUDED__

#include "../../Device/CDevices.h"

class CUITextureEditor: public IEventReceiver {

public:

	CUITextureEditor(CDevices *_devices, ITexture *texture);
	~CUITextureEditor();

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
	//DEVICE
	CDevices *devices;
	IVideoDriver *driver;
	IGUIEnvironment *gui;

	ITexture *textureToEdit, *tempTexture;

	SColor selectedPixelColor;
	//-----------------------------------

	//-----------------------------------
	//METHODS

	//-----------------------------------

	//-----------------------------------
	//USER INTERFACE ELEMENTS
	IGUIWindow *window;
	IGUIImage *image;

	//TOOL BAR
	IGUIStaticText *toolbartxt;

	IGUICheckBox *scaleImagecb;
	IGUIStaticText *selectedColorst;
	IGUICheckBox *useAlphaChannelcb;
	IGUIButton *setSelectedColorAlphabtn;
	//-----------------------------------

};

#endif