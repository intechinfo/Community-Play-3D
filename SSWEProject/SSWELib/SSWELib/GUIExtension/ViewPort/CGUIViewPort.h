#ifndef __C_GUI_VIEWPORT_H_INCLUDED__
#define __C_GUI_VIEWPORT_H_INCLUDED__

#include "IGUIViewport.h"

namespace irr {
    namespace gui {
        
        class CGUIViewport : public IGUIViewport {
            
        public:
            
            CGUIViewport(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle);
            
            virtual ~CGUIViewport();
            
            virtual bool OnEvent(SEvent event);
            
            virtual void draw();
            
            virtual void setSceneManager(scene::ISceneManager* scene);
			virtual void setRenderScreenQuad(bool render) { renderScreenQuad = render; }
			void setScreenQuad(CScreenQuad *quad) { ScreenQuadPtr = quad; }
            
            virtual scene::ISceneManager* getSceneManager() const;
            
            virtual void setOverrideColor(video::SColor color);
            
            virtual void enableOverrideColor(bool override);
            
        private:
            
            scene::ISceneManager* SceneManager;
			CScreenQuad *ScreenQuadPtr;
			bool renderScreenQuad;
            
            bool OverrideColorEnabled;
            video::SColor OverrideColor;
        };
        
        
    }
}

#endif