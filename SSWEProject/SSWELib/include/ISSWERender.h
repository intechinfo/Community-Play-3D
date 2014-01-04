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

class ISSWERender {
    
public:
    
    virtual void update(bool updateOcclusionQueries = false, irr::video::ITexture* outputTarget = 0) = 0;
    virtual void setActiveSceneManager(irr::scene::ISceneManager* smgrIn) = 0;
    
};

#endif
