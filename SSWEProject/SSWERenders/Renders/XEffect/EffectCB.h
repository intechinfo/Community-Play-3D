#ifndef H_XEFFECTS_CB
#define H_XEFFECTS_CB

#include "EffectHandler.h"
#include "CPSSMUtils.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

class SelectionPassCB : public video::IShaderConstantSetCallBack {
public:
	SelectionPassCB(EffectHandler* effectIn) : effect(effectIn) {};

	virtual void OnSetConstants(video::IMaterialRendererServices *services, s32 userData) {
		IVideoDriver* driver = services->getVideoDriver();

		if (driver->getDriverType() == video::EDT_OPENGL) {
            irr::s32 texVar = 0;
            services->setVertexShaderConstant("diffuse", &texVar, 1);
		} else {
            worldViewProj = driver->getTransform(video::ETS_PROJECTION);
            worldViewProj *= driver->getTransform(video::ETS_VIEW);
            worldViewProj *= driver->getTransform(video::ETS_WORLD);

            services->setVertexShaderConstant("worldViewProj", worldViewProj.pointer(), 16);
		}
	}

private:
	EffectHandler *effect;
	core::matrix4 worldViewProj;
};

class SSWE_RENDERS_API NormalShaderCB : public video::IShaderConstantSetCallBack {
public:
	NormalShaderCB(EffectHandler* effectIn) : effect(effectIn) {};

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData) {
		IVideoDriver* driver = services->getVideoDriver();

		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("cWorldViewProj", worldViewProj.pointer(), 16);

		worldView = driver->getTransform(video::ETS_WORLD);
		worldView *= driver->getTransform(video::ETS_VIEW);
		services->setVertexShaderConstant("cWorldView", worldView.pointer(), 16);

		services->setPixelShaderConstant("cFarDistance", &FarLink, 1);
	}

	EffectHandler* effect;
	f32 FarLink;
	core::matrix4 worldViewProj, worldView;
};

class SSWE_RENDERS_API DepthNormalLightCustomCB : public video::IShaderConstantSetCallBack {
public:
	DepthNormalLightCustomCB(EffectHandler* effectIn) : effect(effectIn)
	{
		RenderNormal = 1;
	}

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData) {
		IVideoDriver* driver = services->getVideoDriver();

		worldView = driver->getTransform(video::ETS_WORLD);
		worldView *= driver->getTransform(video::ETS_VIEW);

        if (driver->getDriverType() != EDT_OPENGL) {
            worldViewProj = driver->getTransform(video::ETS_PROJECTION);
            worldViewProj *= driver->getTransform(video::ETS_VIEW);
            worldViewProj *= driver->getTransform(video::ETS_WORLD);
            services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
        } else {
            irr::s32 DiffuseTextureVar = 0;
            services->setPixelShaderConstant("DiffuseMapSampler", &DiffuseTextureVar, 1);
        }

		if (RenderDepthPass) {
			services->setVertexShaderConstant("CP3DDepthPassMaxD", &FarLink, 1);
		}
		if (RenderNormalPass) {
			services->setPixelShaderConstant("CP3DNormalPassFarDistance", &FarLink, 1);
			services->setVertexShaderConstant("mWorldView", worldView.pointer(), 16);
            if (driver->getDriverType() == EDT_OPENGL) {
                irr::s32 NormalMapTextureVar = 1;
                services->setPixelShaderConstant("NormalMapSampler", &NormalMapTextureVar, 1);
            }
		}

		if (RenderScatteringPass) {
			services->setPixelShaderConstant("CP3DScatteringPassRenderNormal", &RenderNormal, 1);
		}

	}

	EffectHandler* effect;

	f32 FarLink;
	s32 RenderNormal;

	bool RenderScatteringPass;
	bool RenderNormalPass;
	bool RenderDepthPass;

	core::matrix4 worldViewProj;
	core::matrix4 worldView;
};

class SSWE_RENDERS_API DepthShaderCB : public video::IShaderConstantSetCallBack {
public:
	DepthShaderCB(EffectHandler* effectIn) : effect(effectIn)
	{
		SSAOLink = 100.f;
	}

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData) {
		IVideoDriver* driver = services->getVideoDriver();

		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);

		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
		services->setVertexShaderConstant("MaxD", &FarLink, 1);

		if (MultiRenderTarget) {
			services->setPixelShaderConstant("maxSSAODepthPass", &SSAOLink, 1);
		}
	}

	EffectHandler* effect;
	f32 FarLink, SSAOLink;
	core::matrix4 worldViewProj;
	bool MultiRenderTarget;
};


class CCustomFilterCallback : public IPostProcessingRenderCallback {

public:

	CCustomFilterCallback(std::function<void(ISSWERender *render, irr::s32 materialType)> preRenderCallback, 
						  std::function<void(ISSWERender *render, irr::s32 materialType)> postRenderCallback,
						  irr::s32 materialType=-1)
	{
		PreRenderCallback = preRenderCallback;
		PostRenderCallback = postRenderCallback;
		MaterialType = materialType;
	}

	void OnPostRender(ISSWERender* effect) {
		PostRenderCallback(effect, MaterialType);
	}

	void OnPreRender(ISSWERender* effect) {
		PreRenderCallback(effect, MaterialType);
	}

	irr::s32 MaterialType;

private:

	std::function<void(ISSWERender *render, irr::s32 materialType)> PreRenderCallback;
	std::function<void(ISSWERender *render, irr::s32 materialType)> PostRenderCallback;

};

class SSWE_RENDERS_API ShadowShaderCB : public video::IShaderConstantSetCallBack {
public:
	ShadowShaderCB(EffectHandler* effectIn) : effect(effectIn) {};

	virtual void OnSetMaterial(const SMaterial& material) {}

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData) {
		IVideoDriver* driver = services->getVideoDriver();

		matrix4 worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		worldViewProj = ProjLink;
		worldViewProj *= ViewLink;
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj2", worldViewProj.pointer(), 16);

		driver->getTransform(video::ETS_WORLD).getInverse(invWorld);
		vector3df lightPosOS = LightLink;
		invWorld.transformVect(lightPosOS);
		services->setVertexShaderConstant("LightPos", reinterpret_cast<f32*>(&lightPosOS.X), 4);

		services->setVertexShaderConstant("MaxD", reinterpret_cast<f32*>(&FarLink), 1);
		services->setVertexShaderConstant("MAPRES", &MapRes, 1);

		services->setPixelShaderConstant("LightColour", reinterpret_cast<f32*>(&LightColour.r), 4);
	}

	EffectHandler* effect;
	core::matrix4 invWorld;

	video::SColorf LightColour;
	core::matrix4 ProjLink;
	core::matrix4 ViewLink;
	core::vector3df LightLink;
	f32 FarLink, MapRes;
};


class SSWE_RENDERS_API ScreenQuadCB : public irr::video::IShaderConstantSetCallBack {
public:
	ScreenQuadCB(EffectHandler* effectIn, bool defaultV = true)
		: effect(effectIn), defaultVertexShader(defaultV)
	{
		uniformDescriptors.clear();
	};

	EffectHandler* effect;
	bool defaultVertexShader;

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData) {
		if(services->getVideoDriver()->getDriverType() == irr::video::EDT_OPENGL) {
			irr::s32 TexVar = 0;
			services->setPixelShaderConstant("ColorMapSampler", (irr::s32*)(&TexVar), 1);

			TexVar = 1;
			services->setPixelShaderConstant("ScreenMapSampler", (irr::s32*)(&TexVar), 1);

			TexVar = 2;
			services->setPixelShaderConstant("DepthMapSampler", (irr::s32*)(&TexVar), 1);

			TexVar = 3;
			services->setPixelShaderConstant("UserMapSampler", (irr::s32*)(&TexVar), 1);
		}

		if(defaultVertexShader) {
			const irr::core::dimension2du currentRTTSize = services->getVideoDriver()->getCurrentRenderTargetSize();
			const irr::f32 screenX = (irr::f32)currentRTTSize.Width, screenY = (irr::f32)currentRTTSize.Height;

			services->setVertexShaderConstant("screenX", &screenX, 1);
			services->setVertexShaderConstant("screenY", &screenY, 1);

			irr::scene::ISceneManager* smgr = effect->getActiveSceneManager();
			irr::scene::ICameraSceneNode* cam = smgr->getActiveCamera();

			const irr::core::position2di tLeft = services->getVideoDriver()->getViewPort().UpperLeftCorner;
			const irr::core::position2di bRight = services->getVideoDriver()->getViewPort().LowerRightCorner;

			const irr::core::line3df sLines[4] = {
				smgr->getSceneCollisionManager()->getRayFromScreenCoordinates
				(irr::core::position2di(tLeft.X, tLeft.Y), cam),
				smgr->getSceneCollisionManager()->getRayFromScreenCoordinates
				(irr::core::position2di(bRight.X, tLeft.Y), cam),
				smgr->getSceneCollisionManager()->getRayFromScreenCoordinates
				(irr::core::position2di(tLeft.X, bRight.Y), cam),
				smgr->getSceneCollisionManager()->getRayFromScreenCoordinates
				(irr::core::position2di(bRight.X, bRight.Y), cam)
			};

			services->setVertexShaderConstant("LineStarts0", &sLines[0].start.X, 3);
			services->setVertexShaderConstant("LineStarts1", &sLines[1].start.X, 3);
			services->setVertexShaderConstant("LineStarts2", &sLines[2].start.X, 3);
			services->setVertexShaderConstant("LineStarts3", &sLines[3].start.X, 3);

			services->setVertexShaderConstant("LineEnds0", &sLines[0].end.X, 3);
			services->setVertexShaderConstant("LineEnds1", &sLines[1].end.X, 3);
			services->setVertexShaderConstant("LineEnds2", &sLines[2].end.X, 3);
			services->setVertexShaderConstant("LineEnds3", &sLines[3].end.X, 3);
		}

		if(uniformDescriptors.size()) {
			irr::core::map<irr::core::stringc, SUniformDescriptor>::Iterator mapIter = uniformDescriptors.getIterator();

			for(;!mapIter.atEnd();mapIter++) {
				if(mapIter.getNode()->getValue().fPointer == 0)
					continue;

				services->setPixelShaderConstant(mapIter.getNode()->getKey().c_str(), mapIter.getNode()->getValue().fPointer,
					mapIter.getNode()->getValue().paramCount);
			}
		}
	}

	struct SUniformDescriptor {
		SUniformDescriptor() : fPointer(0), paramCount(0) {}

		SUniformDescriptor(irr::f32* fPointerIn, irr::u32 paramCountIn)
			: fPointer(fPointerIn), paramCount(paramCountIn) {}

		irr::f32* fPointer;
		irr::u32 paramCount;
	};

	void cleanUniformDescriptors() {
		if (uniformDescriptors.size() > 0) {
			irr::core::map<irr::core::stringc, SUniformDescriptor>::Iterator mapIter = uniformDescriptors.getIterator();
			for(;!mapIter.atEnd();mapIter++) {
				if(mapIter.getNode()->getValue().fPointer == 0)
					continue;

				delete mapIter.getNode()->getValue().fPointer;
			}

			uniformDescriptors.clear();
		}
	}

	irr::core::map<irr::core::stringc, SUniformDescriptor> uniformDescriptors;
};



#endif
