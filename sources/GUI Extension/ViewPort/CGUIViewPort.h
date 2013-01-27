#ifndef __C_GUI_VIEWPORT_H_INCLUDED__
#define __C_GUI_VIEWPORT_H_INCLUDED__

#include "IGUIViewport.h"

namespace irr
{
    
    namespace gui
    {
        
        class CGUIViewport : public IGUIViewport
        {
        public:
            
            //! constructor
            CGUIViewport(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle, bool _drawEffects);
            
            //! destructor
            virtual ~CGUIViewport();
            
            //! called if an event happened.
            virtual bool OnEvent(SEvent event);
            
            //! draws the element and its children
            virtual void draw();
            
            //! sets the scene to be shown
            virtual void setSceneManager(scene::ISceneManager* scene);
            
            //! sets the effects manager
            void setEffects(EffectHandler *_effect);
            
            //! gets the scene to be shown
            virtual scene::ISceneManager* getSceneManager() const;
            
            //! sets the viewport fill color
            virtual void setOverrideColor(video::SColor color);
            
            //! enable or disable the viewport fill color
            virtual void enableOverrideColor(bool override);
            
        private:
            scene::ISceneManager* SceneManager;
            
            bool OverrideColorEnabled;
            video::SColor OverrideColor;
            
            EffectHandler *effect;
            bool drawEffects;
        };
        
        
    } // end namespace gui
} // end namespace irr

#endif