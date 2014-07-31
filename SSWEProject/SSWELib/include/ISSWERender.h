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
#include <ISSWEHDRManager.h>

#include <functional>

namespace cp3d {
namespace video {

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

class ISSWERender;

/*
IPostProcessingRenderCallback pure virtual class
Allows you to create custom callbacks when calling:
	virtual irr::s32 addPostProcessingEffectFromFile(const irr::core::stringc& filename,
													 IPostProcessingRenderCallback* callback = 0,
													 bool pushFront=false) = 0;
Example usage :

class CCallbackExample : IPostProcessingRenderCallback {
public:

	CCallbackExample(irr::s32 materialType) : matType(materialType) { }
	~CCallbackExample() { }

	void OnPreRender(ISSWERender* effect) {

		irr::core::matrix4 worldViewProj;
		worldViewProj *= effect->getIrrlichtDevice()->getVideoDriver()->getTransform(irr::video::ETS_PROJECTION);
		worldViewProj *= effect->getIrrlichtDevice()->getVideoDriver()->getTransform(irr::video::ETS_VIEW);
		worldViewProj *= effect->getIrrlichtDevice()->getVideoDriver()->getTransform(irr::video::ETS_WORLD);

		effect->setPostProcessingEffectConstant(matType, "worldViewProj", worldViewProj.pointer(), 16);
	}

	void OnPostRender(ISSWERender* effect) {
		/// Here some treatments I want to perform after rendering the filter
		/// It follows the same method of OnPreRender()
	}

private:
	irr::s32 matType;

};

Create a callback using lambdas:

renderer->addPostProcessingEffectFromFile("shaders/HLSL/my_shader.hlsl",
	[](cp3d::video::ISSWERender *render, irr::s32 materialType) {
		/// PreRender
		render->setPostProcessingEffectConstant(materialType, "my_constant", &my_variable, count);
	},
	[](cp3d::video::ISSWERender *render, irr::s32 materialType) {
		/// PostRender, same method as PreRender
	}
);

*/
class IPostProcessingRenderCallback {

public:

	virtual void OnPreRender(ISSWERender* effect) = 0;
	virtual void OnPostRender(ISSWERender* effect) = 0;

};

struct IShadowLight {
	
public:

	/// Returns the far value of the light
	virtual irr::f32 getFarValue() const = 0;
	/// Sets a new far value for the light
    virtual void setFarValue(const irr::f32 farPlane) = 0;

	/// Returns the near value of the light
	virtual irr::f32 getNearValue() const = 0;
	/// Sets the new near value for the light
	virtual void setNearValue(const irr::f32 nearValue) = 0;

	/// Returns the FOV of the light
	virtual irr::f32 getFOV() const = 0;
	/// Sets the new FOV of the light
	virtual void setFOV(const irr::f32 fov) = 0;

	/// Sets the new shadow map resolution of the light
	virtual void setShadowMapResolution(irr::u32 shadowMapResolution) = 0;
	/// Returns the shadow map resolution of the light
	virtual irr::u32 &getShadowMapResolution() = 0;

	/// Sets the new shadow light type
	virtual void setLightType(E_SHADOW_LIGHT_TYPE type) = 0;
	/// Returns the shadow light type
	virtual E_SHADOW_LIGHT_TYPE getLightType() = 0;

	/// Returns if the shadow light is a torch
	virtual bool isTorchMode() = 0;
	/// Shortcut to set the shadow light as a torch
	virtual void setTorchMode(bool use) = 0;

	/// Returns if the light must recalculate the shadow map
	virtual bool mustRecalculate() = 0;
	/// Sets if the light must recalculate the shadow map
	virtual void setRecalculate(bool recalculate) = 0;

	/// Returns if the shadow map is automatically recalculated by the light
	virtual bool isAutoRecalculate() = 0;
	/// Sets if the shadow map is automatically recalculated by the light
	virtual void setAutoRecalculate(bool autoRecalculate) = 0;
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
    
	/// Get the original Irrlicht device and get access to the driver, scene manager etc.
    virtual irr::IrrlichtDevice *getIrrlichtDevice() = 0;

	/// Add a shadow to node
	/// The shadow system is based on XEffect by BlinSide : http://irrlicht.sourceforge.net/forum/viewtopic.php?t=23638
	/// Today the system is so far different and optimized by keep some same functions like addShadowToNode
	/// node : the node to apply shadows
	/// filterType : which filter you want to apply
	/// shadowMode : which shadow mode you want to apply (cast, receive, both ? etc.)
	virtual void addShadowToNode(irr::scene::ISceneNode *node, E_FILTER_TYPE filterType, E_SHADOW_MODE shadowMode) = 0;

	/// Removes the shadows from a node. If the node doesn't exists then nothing happen.
	/// node : the node you want to remove the shadows
	virtual void removeShadowFromNode(irr::scene::ISceneNode* node) = 0;

	/// Removes a node from the light scattering pass. If the node doesn't exists then nothing happen
	/// node : the node you want to remove from the pass
	virtual void removeNodeFromLightScatteringPass(irr::scene::ISceneNode *node) = 0;

	/// Removes a node from the depth pass (used by SSAO, deffered shading etc). If the node doesn't exists then nothing happen.
	/// node : the node you want to remove from the pass
	virtual void removeNodeFromDepthPass(irr::scene::ISceneNode *node) = 0;

	/// Recalculate all the lights in the scene. Useful when you want to re-compute shadows for all lights or unordered lights.
	virtual void setAllShadowLightsRecalculate() = 0;
	/// Returns the IShadowLight at index "index"
	virtual IShadowLight &getShadowLight(irr::u32 index) = 0;
	/// Returns the shadow light count
	virtual const irr::u32 getShadowLightCount() const = 0;

	/// Set enable and disable Motion Blur renderer
	/// use : set true if you want to use
	virtual void setUseMotionBlur(bool use) = 0;
	/// Get is the motion blur is enabled
	virtual bool isUsingMotionBlur() = 0;

	/// Sets enable and disable DepthOfField
	virtual void setUseDepthOfField(bool use) = 0;
	/// Get is DOF is enabled
	virtual bool isUsingDepthOfField() = 0;

	/// Returns if depth pass is enabled
    virtual bool isDepthPassEnabled() = 0;
	/// Sets depth pass enabled or disabled
	virtual void enableDepthPass(bool enableDepthPass) = 0;

	/// Sets if use the VSM shadows
	virtual void setUseVSMShadows(bool use) = 0;
	/// Returns if the renderer is using VSM shadows
	virtual bool isUsingVSMShadows() = 0;

	/// Removes of the post processing effects added by the user
	/// removeCallbacks : set true if you want to remove the LUA callback programs too.
	virtual void removeAllPostProcessingEffects(bool removeCallbacks = true) = 0;

	/// Add a post-processing effect (filter) from a file
	/// filename : the file containing the shader program
	/// callback : the LUA callback program or a custom callback implementation that heritates IPostProcessingRenderCallback
	/// pushfront : set true if you want to push the filter in front of all other filters
	virtual irr::s32 addPostProcessingEffectFromFile(const irr::core::stringc& filename, IPostProcessingRenderCallback* callback = 0, bool pushFront=false) = 0;
	virtual irr::s32 addPostProcessingEffectFromFile(const irr::core::stringc& filename, 
													 std::function<void(ISSWERender *render, irr::s32 materialType)> preRenderCallback,
													 std::function<void(ISSWERender *render, irr::s32 materialType)> postRenderCallback,
													 bool pushFront=false) = 0;

	/// Add a post-processing effect (filter) from a string
	/// pixelShader : the string containing the shader program
	/// callback : the LUA callback program or a custom callback implementation that heritates IPostProcessingRenderCallback
	/// pushfront : set true if you want to push the filter in front of all other filters
	virtual irr::s32 addPostProcessingEffectFromString(const irr::core::stringc pixelShader, IPostProcessingRenderCallback *callback = 0, bool pushFront=false) = 0;

	/// Sets the user post-processing texture at index 3 of the screenquad
	/// userTexture : the texture you want to use ("uniform sampler2D UserMapSampler;" or "sampler2D UserMapSampler : register(s3);" in the shader program)
	virtual void setPostProcessingUserTexture(irr::video::ITexture* userTexture) = 0;

	/// Sends a constant to the post-process
	/// materialType : the material type returned by addPostProcessingEffectFromFile or addPostProcessingEffectFromString
	/// name : the name of the constant
	/// data : the data you want to send to the shader program
	/// count : the amount of data in the pointer "*data"
	virtual void setPostProcessingEffectConstant(const irr::s32 materialType, const irr::core::stringc& name, irr::f32* data, const irr::u32 count) = 0;
	/// Sets a callback the specified post-process filter
	/// MaterialType : the material type (the post-process filter)
	/// callback : the callback implementation that heritates IPostProcessingRenderCallback
	virtual void setPostProcessingRenderCallback(irr::s32 MaterialType, IPostProcessingRenderCallback* callback = 0) = 0;

	/// Get the HDR manager performed by the CP3D rendering pipeline
	virtual IHDRManager *getHDRManager() = 0;

	/// Sets if use normal pass
	virtual void setUseNormalPass(bool use) = 0;

	/// Retunrs if the normal pass is used
	virtual bool isUsingNormalPass() = 0;

	/// Set if use the HDR pass
	virtual void setUseHDRPass(bool use) = 0;

	/// Returns if the HDR pass is used
	virtual bool isUsingHDRPass() = 0;
    
};

} /// End namespace video
} /// End namespace cp3d

#endif
