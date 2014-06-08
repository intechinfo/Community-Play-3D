// stdafx.h�: fichier Include pour les fichiers Include syst�me standard,
// ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
// et sont rarement modifi�s

#pragma once

#include <irrlicht.h>

#ifndef _IRR_OSX_PLATFORM_
    #include <SSWECore.h>
#else
    #include "SSWECore.h"
#endif

#include <SSWECore.h>
#include <SSWELib.h>

#include "SSWELib/Device/CDevices.h"
#include "SSWECore/Core/CCore.h"
#include <SSWECore/Core/CCoreData.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>

#include <irrbullet.h>

#ifdef _IRR_WINDOWS_API_
    extern CRITICAL_SECTION CriticalSection;
#endif
