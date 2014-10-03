//
//  CDevelopmentInstance.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 04/10/2014.
//
//

#ifndef __C_DEVELOPMENT_INSTANCE_H_INCLUDED__
#define __C_DEVELOPMENT_INSTANCE_H_INCLUDED__

#include "../../Device/CDevices.h"

class CDevelopmentPluginManager : public cp3d::core::IDevelomentInstance {
    
public:
    
    CDevelopmentPluginManager(CDevices *devices);
    ~CDevelopmentPluginManager();
    
    void run(void);
    void stop(void);
    void update(void);
    
    bool isPlayingForDevelopment() { return playingForDevelopment; }
    
private:
    
    CDevices *devices;
    
    bool playingForDevelopment;
    EffectHandler *effectFormDevelopment;
    ISceneManager *smgrForDevelopment;
    irrBulletWorld *bworldForDevelopment;
};

#endif
