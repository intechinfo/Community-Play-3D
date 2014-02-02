//
//  IRender.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 27/12/2013.
//
//

#ifndef __I_SSWE_RENDER_PLUGIN_H_INCLUDED__
#define __I_SSWE_RENDER_PLUGIN_H_INCLUDED__

#include <irrlicht.h>

enum E_SHADOW_MODE
{
	ESM_RECEIVE = 0,
	ESM_CAST,
	ESM_BOTH,
	ESM_EXCLUDE,
    ESM_NO_SHADOW,
	ESM_COUNT
};

enum E_FILTER_TYPE
{
	EFT_NONE = 0,
	EFT_4PCF,
	EFT_8PCF,
	EFT_12PCF,
	EFT_16PCF,
	EFT_COUNT
};

enum E_SHADOW_LIGHT_TYPE
{
	ESLT_SPOT = 0,
	ESLT_DIRECTIONAL
};

class ISSWERender {
    
public:
    
    virtual void update(bool updateOcclusionQueries = false, irr::video::ITexture* outputTarget = 0) = 0;
    virtual void setActiveSceneManager(irr::scene::ISceneManager* smgrIn) = 0;
    
};

#endif
