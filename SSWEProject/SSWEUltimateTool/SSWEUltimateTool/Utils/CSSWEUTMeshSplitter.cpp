//
//  CExporter.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#include "Stdafx.h"
#include "CSSWEUTMeshSplitter.h"

using namespace irr;
using namespace gui;
using namespace core;
using namespace scene;
using namespace video;

CUIMeshSplitter::CUIMeshSplitter(cp3d::core::IDevices *_devices, stringc _workingDirectory) {
	/// Set datas
	devices = _devices;
	workingDirectory = _workingDirectory;

	/// Configure events
	devices->getEventReceiver()->AddEventReceiver(this);
}

CUIMeshSplitter::~CUIMeshSplitter() {

}

bool CUIMeshSplitter::OnEvent(const SEvent &event) {

	return false;
}
