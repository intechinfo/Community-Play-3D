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

class CAction {
    
public:
    
    CAction();
    ~CAction();

	//METHODS
	irr::core::stringc getXMLValues();
    
    //GET METHODS
    irr::s32 getStart() { return start; }
    irr::s32 getEnd() { return end; }
    irr::core::stringw getName() { return name; }
    
    irr::f32 getAnimSpeed() { return animSpeed; }
    
    //SET METHODS
    void setStart(irr::s32 _start) { start = _start; }
    void setEnd(irr::s32 _end) { end = _end; }
    void setName(irr::core::stringw _name) { name = _name; }
    
    void setAnimSpeed(irr::f32 _animSpeed) { animSpeed = _animSpeed; }
    
private:
    
    irr::s32 start, end;
    irr::core::stringw name;
    
    irr::f32 animSpeed;
    
};

#endif
