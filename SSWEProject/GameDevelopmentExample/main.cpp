//
//  main.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 03/10/2014.
//
//

#include "main.h"

#include <DLLExport.h>

using namespace cp3d;
using namespace cp3d::core;

cp3d::core::IDevelomentInstance *createDevelopmentInstance(cp3d::core::IDevices *devices) {
    return new CGameDevelopmentExample(devices);
}

CGameDevelopmentExample::CGameDevelopmentExample(IDevices *devices)
    : IDevelomentInstance(devices)
{
    /// Initialize your game here
    /// Scene is in the Editor
    devices->addInformationDialog("Works !", "The development plugins works !", EMBF_OK);
}

CGameDevelopmentExample::~CGameDevelopmentExample() {
    /// Nothing...
}

void CGameDevelopmentExample::run(void) {
    /// Configure assets here
}

void CGameDevelopmentExample::stop(void) {
    ///free assets here
}

void CGameDevelopmentExample::update() {
    /// Device is automatically updated here
}
