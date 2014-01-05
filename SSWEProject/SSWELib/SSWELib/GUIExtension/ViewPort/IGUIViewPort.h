#ifndef __I_GUI_VIEWPORT_H_INCLUDED__
#define __I_GUI_VIEWPORT_H_INCLUDED__

#include <IGUIElement.h>
#include <SColor.h>

#include "../../../SSWERenders/Renders/XEffect/CScreenQuad.h"

namespace irr {
    namespace scene {
        class ISceneManager;
    }
    
    namespace gui {

        class IGUIViewport : public IGUIElement {
            
        public:
            
            IGUIViewport(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle)
            : IGUIElement(EGUIET_MESH_VIEWER, environment, parent, id, rectangle)
            {
            }
            
            virtual void setSceneManager(scene::ISceneManager* scene) = 0;
			virtual void setScreenQuad(CScreenQuad *quad) = 0;
			virtual void setRenderScreenQuad(bool render) = 0;
            
            virtual scene::ISceneManager* getSceneManager() const = 0;
            
            virtual void setOverrideColor(video::SColor color) = 0;
            
            virtual void enableOverrideColor(bool override) = 0;
        };
        
    }
}

#endif