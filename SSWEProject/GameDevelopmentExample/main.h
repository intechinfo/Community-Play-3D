//
//  main.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 03/10/2014.
//
//

#ifndef __C_MAIN_H_INCLUDED__
#define __C_MAIN_H_INCLUDED__

#include <ISSWELibPlugin.h>

class CGameDevelopmentExample : public cp3d::core::IDevelomentInstance {
  
public:
    CGameDevelopmentExample(cp3d::core::IDevices *devices);
    ~CGameDevelopmentExample();
    
    /// Run the current plugin
    void run(void);
    /// Stop the current plugin
    void stop(void);
    
    /// Update the development plugin
    void update();
    
private:
        /// Nothing...
};

#endif
