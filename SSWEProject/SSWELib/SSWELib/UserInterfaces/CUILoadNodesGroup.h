//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_LOAD_NODES_GROUP_H_INCLUDED__
#define __C_UI_LOAD_NODES_GROUP_H_INCLUDED__

#include "../Device/CDevices.h"

#include "MaterialEditor/CMaterialEditorFactory.h"

class CUILoadNodesGroup : public IEventReceiver {

public:

	CUILoadNodesGroup(CDevices *_devices);
	~CUILoadNodesGroup();

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
    //DEVICE AND DATAS
	CDevices *devices;
	CMaterialEditorFactory *mtFactory;
	//-----------------------------------

	//-----------------------------------
    //METHODS
	void loadGroup();
	//-----------------------------------

	//-----------------------------------
    //GUI ELEMENTS
	IGUIWindow *window;
	IGUIEditBox *extensioneb;
	IGUIButton *chooseDirectory, *accept, *cancel;
	IGUIStaticText *directoryStaticText;

	CGUIFileSelector *selector;
	//-----------------------------------

};

#endif
