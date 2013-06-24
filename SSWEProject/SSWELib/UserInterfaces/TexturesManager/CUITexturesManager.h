//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_TEXTURES_MANAGER_H_INCLUDED__
#define __C_UI_TEXTURES_MANAGER_H_INCLUDED__

#include "Device/CDevices.h"

class CUITexturesManager : public IEventReceiver {

public:

	CUITexturesManager(CDevices *_devices);
	~CUITexturesManager();

	bool OnEvent(const SEvent &event);

	//GET ARRAY OF NODES THAT USE THE TEXTURE
	array<ISceneNode *> whoUseThisTexture(ITexture *tex);
	void removeTexture(ITexture *tex);

private:

	//-----------------------------------
	//DEVICE
	CDevices *devices;
	IVideoDriver *driver;
	IGUIEnvironment *gui;
	//-----------------------------------

	//-----------------------------------
	//METHODS
	SEvent getUpdateEvent();
	//-----------------------------------

	//-----------------------------------
	//USER INTERFACE ELEMENTS
	IGUIWindow *window;

	IGUIListBox *textures;
	IGUIImage *texturePreview;
	IGUIButton *edit, *whoUse, *remove, *refresh;
	//-----------------------------------

};

#endif