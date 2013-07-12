// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés

#pragma once

#include <SSWECore.h>

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

#ifndef _IRR_OSX_PLATFORM_
	#include <memory>
	#include <thread>
	#include <Windows.h>
	#include <mutex>
#endif

#include "src/Bullet-C-Api.h"
#include "src/btBulletDynamicsCommon.h"
#include "src/btBulletCollisionCommon.h"
