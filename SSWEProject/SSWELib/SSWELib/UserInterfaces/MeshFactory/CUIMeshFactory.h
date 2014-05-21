//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_MESH_FACTORY_H_INCLUDED__
#define __C_UI_MESH_FACTORY_H_INCLUDED__

#include "../../Device/CDevices.h"

class CMeshFactory;

class CUIMeshFactory : public IEventReceiver {

public:

	CUIMeshFactory(CDevices *_devices);
	~CUIMeshFactory();

	bool OnEvent(const SEvent &event);

private:

	CDevices *devices;
	CMeshFactory *factory;

	void reloadListBox();

	//-----------------------------------
    //GUI ELEMENTS
	IGUIWindow *window;

	IGUIListBox *meshes;

	IGUIButton *reload, *remove, *clear;
	IGUIButton *close;

	IGUIWindow *removeInformationWindow;
	//-----------------------------------

};

#endif