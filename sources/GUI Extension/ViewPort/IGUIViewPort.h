#ifndef __I_GUI_VIEWPORT_H_INCLUDED__
#define __I_GUI_VIEWPORT_H_INCLUDED__

#include <IGUIElement.h>
#include <SColor.h>

#include "../../Device/CDevices.h"

namespace irr
{
    
    namespace scene
    {
        class ISceneManager;
    }
    
    namespace gui
    {
        
        //! 3d scene GUI element.
        class IGUIViewport : public IGUIElement
        {
        public:
            
            //! constructor
            IGUIViewport(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle)
            : IGUIElement(EGUIET_MESH_VIEWER, environment, parent, id, rectangle)
            {
            }
            
            //! sets the scene to be shown
            virtual void setSceneManager(scene::ISceneManager* scene) = 0;
            
            virtual void setEffects(EffectHandler *_effect) = 0;
            
            //! gets the scene to be shown
            virtual scene::ISceneManager* getSceneManager() const = 0;
            
            //! sets the viewport fill color
            virtual void setOverrideColor(video::SColor color) = 0;
            
            //! enable or disable the viewport fill color
            virtual void enableOverrideColor(bool override) = 0;
        };
        
        
    } // end namespace gui
} // end namespace irr

#endif