#ifndef __C_FACTORY_TEST_H_INCLUDED__
#define __C_FACTORY_TEST_H_INCLUDED__

#include "stdafx.h"

class CGUIInformations : public irr::IEventReceiver, public cp3d::core::IUpdate {

public:

	CGUIInformations(cp3d::core::IDevices *devices) {
		this->devices = devices;
        
        drawInformations = true;
        drawFPS = true;
        primitiveCountDrawn = true;
        
		devices->getEventReceiver()->AddEventReceiver(this, 0, this);
	}
    
	~CGUIInformations() { }

	void update() {
        if (drawInformations) {
            irr::core::stringw textToDraw = L"Informations :\n";
            
            /// All here
            if (drawFPS) {
                textToDraw += L"FPS : ";
                textToDraw += devices->getVideoDriver()->getFPS();
                textToDraw += L"\n";
            }
            
            if (primitiveCountDrawn) {
                textToDraw += L"Primitive Count Drawn : ";
                textToDraw += devices->getVideoDriver()->getPrimitiveCountDrawn();
                textToDraw += L"\n";
            }
            
            IGUIFont *font = devices->getGUIEnvironment()->getSkin()->getFont();
            font->draw(textToDraw, irr::core::rect<s32>(0, 0,
                                                        devices->getVideoDriver()->getScreenSize().Width,
                                                        devices->getVideoDriver()->getScreenSize().Height),
                       irr::video::SColor(255, 255, 0, 0));
        }
	}

	bool OnEvent(const SEvent &event) {
        
        return false;
    }

private:

	cp3d::core::IDevices *devices;
    
    bool drawInformations;
    bool drawFPS;
    bool primitiveCountDrawn;
};

#endif
