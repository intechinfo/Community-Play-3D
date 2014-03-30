//
//  IRender.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 27/12/2013.
//
//

#ifndef __I_SSWE_RENDER_PLUGIN_H_INCLUDED__
#define __I_SSWE_RENDER_PLUGIN_H_INCLUDED__

#include <irrlicht.h>

enum E_SHADOW_MODE
{
	ESM_RECEIVE = 0,
	ESM_CAST,
	ESM_BOTH,
	ESM_EXCLUDE,
    ESM_NO_SHADOW,
	ESM_COUNT
};

enum E_FILTER_TYPE
{
	EFT_NONE = 0,
	EFT_4PCF,
	EFT_8PCF,
	EFT_12PCF,
	EFT_16PCF,
	EFT_COUNT
};

enum E_SHADOW_LIGHT_TYPE
{
	ESLT_SPOT = 0,
	ESLT_POINT,
	ESLT_DIRECTIONAL
};

struct ICP3DShadowLight {

};

class ISSWERender;
class IPostProcessingRenderCallback
{
public:
	virtual void OnPreRender(ISSWERender* effect) = 0;
	virtual void OnPostRender(ISSWERender* effect) = 0;
};

class ISSWERender {
    
public:
    
	virtual void setAmbientColor(irr::video::SColor ambientColour) = 0;

	/// Update the scene using update
	/// updateOcclusionQueries : to save performances you can disable the occlusion queries when rendering the scene
	/// outputTarget : the output target is a render target you can get after rendering that contains the render for post processes
    virtual void update(bool updateOcclusionQueries = false, irr::video::ITexture* outputTarget = 0) = 0;

	/// Change the active scene manager of the renderer
	/// Will draw only nodes of the new scene manager
    virtual void setActiveSceneManager(irr::scene::ISceneManager* smgrIn) = 0;
    
	/// Get the original Irrlicht device
    virtual irr::IrrlichtDevice *getIrrlichtDevice() = 0;

	/// Add a shadow to node
	/// The shadow system is based on XEffect by BlinSide : http://irrlicht.sourceforge.net/forum/viewtopic.php?t=23638
	/// Today the system is so far different and optimized by keep some same functions like addShadowToNode
	/// node : the node to apply shadows
	/// filterType : which filter you want to apply
	/// shadowMode : which shadow mode you want to apply (cast, receive, both ? etc.)
	virtual void addShadowToNode(irr::scene::ISceneNode *node, E_FILTER_TYPE filterType, E_SHADOW_MODE shadowMode) = 0;

	/// Set enable and disable Motion Blur renderer
	/// Also get is the motion blur is enabled
	virtual void setUseMotionBlur(bool use) = 0;
	virtual bool isUsingMotionBlur() = 0;

	/// Sets enable and disable DepthOfField
	/// Also get is DOF is enabled
	virtual void setUseDepthOfField(bool use) = 0;
	virtual bool isUsingDepthOfField() = 0;

	virtual void removeAllPostProcessingEffects(bool removeCallbacks = true) = 0;

	virtual irr::s32 addPostProcessingEffectFromFile(const irr::core::stringc& filename, IPostProcessingRenderCallback* callback = 0, bool pushFront=false) = 0;

	virtual irr::s32 addPostProcessingEffectFromString(const irr::core::stringc pixelShader, IPostProcessingRenderCallback *callback = 0, bool pushFront=false) = 0;

	virtual void setPostProcessingUserTexture(irr::video::ITexture* userTexture) = 0;

	virtual void setPostProcessingEffectConstant(const irr::s32 materialType, const irr::core::stringc& name, irr::f32* data, const irr::u32 count) = 0;

	virtual void setPostProcessingRenderCallback(irr::s32 MaterialType, IPostProcessingRenderCallback* callback = 0) = 0;
    
};

#endif
