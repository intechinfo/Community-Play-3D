//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_ADD_AUDIO_ELEMENT_H_INCLUDED__
#define __C_UI_ADD_AUDIO_ELEMENT_H_INCLUDED__

#include "../../Device/CDevices.h"

namespace irr {
	namespace gui {
		class CGUISoundViewer;
	}
}

class CUIAddAudioElement : public IEventReceiver {

public:

	CUIAddAudioElement(CDevices *_devices);
	~CUIAddAudioElement();

	//-----------------------------------
    /// Event receiver
	bool OnEvent(const SEvent &event);
	//-----------------------------------

private:

	void closeWindow();
	void removeAudioElement();

	//-----------------------------------
    //DEVICES AND DATAS
	CDevices *devices;

	cp3d::audio::IAudioElement *audioElement;

	IGUIEnvironment *gui;
	//-----------------------------------

	//-----------------------------------
    //GUI ELEMENTS
	IGUIWindow *window;
	CGUISoundViewer *soundViewer;

	IGUIButton *open;

	IGUICheckBox *loopMode, *loadAs3D;

	IGUIStaticText *informations;

	IGUIButton *accept, *close;

	IGUIFileOpenDialog *openFromFileDialog;
	//-----------------------------------



};

#endif