// stdafx.h�: fichier Include pour les fichiers Include syst�me standard,
// ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
// et sont rarement modifi�s

#pragma once

#ifndef _IRR_OSX_PLATFORM_
    #include <SSWECore.h>
#else
    #include "SSWECore.h"
#endif

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <assert.h>

#include <stdlib.h>
#include <stdio.h>

#include <irrlicht.h>

#include <memory>
#include <thread>
#include <mutex>

#ifndef _IRR_OSX_PLATFORM_
    #ifndef _SSWE_LINUX_
        #include <Windows.h>
	#endif
#endif

#ifndef _SSWE_LINUX_
    #include "src/Bullet-C-Api.h"
    #include "src/btBulletDynamicsCommon.h"
    #include "src/btBulletCollisionCommon.h"
#else
    #include <src/Bullet-C-Api.h>
    #include <src/btBulletDynamicsCommon.h>
    #include <src/btBulletCollisionCommon.h>
#endif

