//
//  CExporter.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#ifndef __C_SSWE_UT_OPENSCAD_DOCUMENT_H_INCLUDED__
#define __C_SSWE_UT_OPENSCAD_DOCUMENT_H_INCLUDED__

#include <irrlicht.h>

#include <IDevices.h>
#include <DLLExport.h>

class COpenSCADDocument : public irr::IEventReceiver {
public:

	COpenSCADDocument(IDevices *_devices, irr::core::stringc _workingDirectory);
	~COpenSCADDocument();

	bool OnEvent(const SEvent &event);

	//-----------------------------------
	//METHODS

	//-----------------------------------

private:
	//-----------------------------------
	//METHODS
	void buildObject();
	//-----------------------------------

	//-----------------------------------
	//DATAS
	IDevices *devices;

	irr::core::stringw openscadCode;
	irr::core::stringc workingDirectory;

	irr::u32 currentObjectVersion;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	IGUICodeEditor *codeEditor;

	irr::gui::IGUIButton *buildButton;
	irr::gui::IGUIButton *closeButton;

	irr::gui::IGUIStaticText *stateTextElement;
	//-----------------------------------

};

#endif
