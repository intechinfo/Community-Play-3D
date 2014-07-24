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

class CUIAddAudioElement : public IEventReceiver {

public:

	CUIAddAudioElement(CDevices *_devices);
	~CUIAddAudioElement();

	void open();

	//-----------------------------------
    /// Event receiver
	bool OnEvent(const SEvent &event);
	//-----------------------------------

private:

	//-----------------------------------
    //DEVICES AND DATAS
	CDevices *devices;

	IGUIEnvironment *gui;
	//-----------------------------------

	//-----------------------------------
    //GUI ELEMENTS
	IGUIFileOpenDialog *openFromFileDialog;
	//-----------------------------------



};

#endif