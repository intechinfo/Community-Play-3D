//
//  CExporter.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#ifndef __C_SSWE_UT_MESH_SPLITTER_H_INCLUDED__
#define __C_SSWE_UT_MESH_SPLITTER_H_INCLUDED__

#include <irrlicht.h>

#include <IDevices.h>
#include <DLLExport.h>

class CUIMeshSplitter : public irr::IEventReceiver {
public:

	CUIMeshSplitter(cp3d::core::IDevices *_devices, irr::core::stringc _workingDirectory);
	~CUIMeshSplitter();

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
	cp3d::core::IDevices *devices;
	irr::core::stringc workingDirectory;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS

	//-----------------------------------

};

#endif
