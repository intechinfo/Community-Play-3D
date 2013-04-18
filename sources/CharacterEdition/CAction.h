//
//  CAction.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 07/11/12.
//
//

#ifndef __C_ACTION_H_INCLUDED__
#define __C_ACTION_H_INCLUDED__

#include <irrlicht.h>

using namespace irr;
using namespace video;
using namespace gui;
using namespace core;
using namespace scene;
using namespace io;

class CAction {
    
public:
    
    CAction();
    ~CAction();

	//METHODS
	stringc getXMLValues();
    
    //GET METHODS
    s32 getStart() { return start; }
    s32 getEnd() { return end; }
    stringw getName() { return name; }
    
    f32 getAnimSpeed() { return animSpeed; }
    
    //SET METHODS
    void setStart(s32 _start) { start = _start; }
    void setEnd(s32 _end) { end = _end; }
    void setName(stringw _name) { name = _name; }
    
    void setAnimSpeed(f32 _animSpeed) { animSpeed = _animSpeed; }
    
private:
    
    s32 start, end;
    stringw name;
    
    f32 animSpeed;
    
};

#endif
