// stdafx.cpp : fichier source incluant simplement les fichiers Include standard
// SSWELib.pch représente l'en-tête précompilé
// stdafx.obj contient les informations de type précompilées

#include "stdafx.h"

#ifndef _IRR_OSX_PLATFORM_
    #ifndef _SSWE_LINUX_
    CRITICAL_SECTION CriticalSection;
    #endif
#endif
