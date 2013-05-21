#include "CGUIViewport.h"
#include <IGUIEnvironment.h>
#include <ISceneManager.h>
#include <ICameraSceneNode.h>
#include <IVideoDriver.h>
#include <IGUISkin.h>

namespace irr {
    namespace gui {
        
        
        //! constructor
        CGUIViewport::CGUIViewport(irr::gui::IGUIEnvironment* environment, irr::gui::IGUIElement* parent, s32 id, irr::core::rect<s32> rectangle)
        : IGUIViewport(environment, parent, id, rectangle)
        , SceneManager(0)
        , OverrideColorEnabled(false)
        , OverrideColor(255, 0, 0, 0) {
#ifdef _DEBUG
            setDebugName("CGUIViewport");
#endif
        }
        
        CGUIViewport::~CGUIViewport() {
            if (SceneManager) {
                SceneManager->drop();
            }
        }
        
        bool CGUIViewport::OnEvent(SEvent event) {
            bool absorbed = false;
            
            if (SceneManager)
                absorbed = SceneManager->postEventFromUser(event);
            
            if (!absorbed && Parent)
                absorbed = Parent->OnEvent(event);
            
            return absorbed;
        }
        
        void CGUIViewport::draw() {
            if (!IsVisible)
                return;
            
            video::IVideoDriver* driver = Environment->getVideoDriver();
            
            core::rect<s32> frameRect(AbsoluteRect);
            if (OverrideColorEnabled)
                driver->draw2DRectangle(OverrideColor, frameRect, &AbsoluteClippingRect);
            
            IGUISkin* skin = Environment->getSkin();
            
            frameRect.LowerRightCorner.Y = frameRect.UpperLeftCorner.Y + 1;
            driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), frameRect, &AbsoluteClippingRect);
            
            frameRect.LowerRightCorner.Y = AbsoluteRect.LowerRightCorner.Y;
            frameRect.LowerRightCorner.X = frameRect.UpperLeftCorner.X + 1;
            driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), frameRect, &AbsoluteClippingRect);
            
            frameRect = AbsoluteRect;
            frameRect.UpperLeftCorner.X = frameRect.LowerRightCorner.X - 1;
            driver->draw2DRectangle(skin->getColor(EGDC_3D_HIGH_LIGHT), frameRect, &AbsoluteClippingRect);
            
            frameRect = AbsoluteRect;
            frameRect.UpperLeftCorner.Y = AbsoluteRect.LowerRightCorner.Y - 1;
            frameRect.LowerRightCorner.Y = AbsoluteRect.LowerRightCorner.Y;
            driver->draw2DRectangle(skin->getColor(EGDC_3D_HIGH_LIGHT), frameRect, &AbsoluteClippingRect);
            
            if (SceneManager) {
                core::rect<s32> viewPort = AbsoluteRect;
                viewPort.LowerRightCorner.X -= 1;
                viewPort.LowerRightCorner.Y -= 1;
                viewPort.UpperLeftCorner.X += 1;
                viewPort.UpperLeftCorner.Y += 1;
                
                viewPort.clipAgainst(AbsoluteClippingRect);
                
                core::rect<s32> screenRect;
                screenRect.UpperLeftCorner.X = 0;
                screenRect.UpperLeftCorner.Y = 0;
                screenRect.LowerRightCorner.X = driver->getScreenSize().Width;
                screenRect.LowerRightCorner.Y = driver->getScreenSize().Height;
                
                viewPort.clipAgainst(screenRect);
                
                if ( !(viewPort.getWidth() < 1 || viewPort.getHeight() < 1)) {
                    scene::ICameraSceneNode* cam = SceneManager->getActiveCamera();
                    
                    if (cam) {
                        
                        core::rect<s32> oldViewPort = driver->getViewPort();
                        driver->setViewPort(viewPort);

                        driver->clearZBuffer();

                        f32 oldAspectRatio = cam->getAspectRatio();
                        cam->setAspectRatio(1.f * viewPort.getWidth() / viewPort.getHeight());

                        f32 oldFieldOfView = cam->getFOV();
                        cam->setFOV(oldFieldOfView * viewPort.getHeight() / (AbsoluteRect.getHeight() - 2));

                        SceneManager->drawAll();

                        cam->setFOV(oldFieldOfView);
                        
                        cam->setAspectRatio(oldAspectRatio);
                        
                        driver->setViewPort(oldViewPort);
                    }
                }
            }
            
            IGUIElement::draw();
        }
        
        void CGUIViewport::setSceneManager(scene::ISceneManager* scene) {
            if (SceneManager)
                SceneManager->drop();
            
            SceneManager = scene;
            
            if (SceneManager)
                SceneManager->grab();
        }
        
        scene::ISceneManager* CGUIViewport::getSceneManager() const {
            return SceneManager;
        }
        
        void CGUIViewport::setOverrideColor(video::SColor color) {
            OverrideColor = color;
            enableOverrideColor(true);
        }
        
        void CGUIViewport::enableOverrideColor(bool override) {
            OverrideColorEnabled = override;
        }
        
        
        
    }
}
