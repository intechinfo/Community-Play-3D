// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés

#pragma once

#include <irrlicht.h>

#ifndef _IRR_OSX_PLATFORM_
    #include <SSWECore.h>
#else
    #include "SSWECore.h"
#endif

#include "SSWELib/Device/CDevices.h"
#include "SSWECore/Core/CCore.h"
#include "SSWECore/Core/CCoreData.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>

#ifndef _IRR_OSX_PLATFORM_
    extern CRITICAL_SECTION CriticalSection;
#endif
