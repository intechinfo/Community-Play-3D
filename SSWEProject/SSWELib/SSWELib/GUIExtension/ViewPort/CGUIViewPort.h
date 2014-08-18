#ifndef __C_GUI_VIEWPORT_H_INCLUDED__
#define __C_GUI_VIEWPORT_H_INCLUDED__

#include "IGUIViewPort.h"

namespace irr {
    namespace gui {

        class CGUIViewport : public IGUIViewport {

        public:

            CGUIViewport(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle);

            virtual ~CGUIViewport();

            bool OnEvent(const SEvent &event);

            void draw();

            void setSceneManager(scene::ISceneManager* scene);
			void setSecondarySceneManager(scene::ISceneManager *scene);
			void setRenderScreenQuad(bool render) { renderScreenQuad = render; }
			void setScreenQuad(CScreenQuad *quad) { ScreenQuadPtr = quad; }

			bool isRenderingScreenQuad() { return renderScreenQuad; }

            scene::ISceneManager* getSceneManager() const;

            void setOverrideColor(video::SColor color);

            void enableOverrideColor(bool override);

        private:

            scene::ISceneManager* SceneManager;
			scene::ISceneManager *SecondarySceneManager;
			CScreenQuad *ScreenQuadPtr;
			bool renderScreenQuad;

            bool OverrideColorEnabled;
            video::SColor OverrideColor;
        };


    }
}

#endif
