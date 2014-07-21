//
//  CExporter.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#ifndef __C_SSWE_UT_SOUND_VIEWER_H_INCLUDED__
#define __C_SSWE_UT_SOUND_VIEWER_H_INCLUDED__

#include <irrlicht.h>

#include <IDevices.h>
#include <DLLExport.h>

#include "CGUISpectrum.h"

class CSoundViewer : public irr::IEventReceiver, public IUpdate {
public:

	CSoundViewer(IDevices *_devices, irr::core::stringc _workingDirectory);
	~CSoundViewer();

	void update();

	bool OnEvent(const SEvent &event);

	//-----------------------------------
	//METHODS

	//-----------------------------------

private:
	//-----------------------------------
	//METHODS

	//-----------------------------------

	//-----------------------------------
	//DATAS
	IDevices *devices;

	irr::core::stringc workingDirectory;

	cp3d::audio::IAudioManager *amgr;
	cp3d::audio::IAudioElement *audioElement;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	irr::gui::IGUIWindow *window;

	irr::gui::CGUISpectrum *spectrumViewer;
	irr::gui::IGUIScrollBar *timeline;

	irr::gui::IGUIButton *loadButton, *playButton, *stopButton;
	irr::gui::IGUIButton *closeButton;

	irr::gui::IGUIFileOpenDialog *soundDialog;
	//-----------------------------------

};

#endif
