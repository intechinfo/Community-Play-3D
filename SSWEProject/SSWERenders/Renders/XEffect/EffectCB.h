#ifndef H_XEFFECTS_CB
#define H_XEFFECTS_CB

#include "EffectHandler.h"
#include "CPSSMUtils.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

class CPSSMCallBack : public video::IShaderConstantSetCallBack
{
public:
    CPSSMCallBack(CPSSMUtils* vPSSMUtils) : PSSMUtils(vPSSMUtils)
    {
        vBias[0] = 0.001;
        vBias[1] = 0.0008;
        vBias[2] = 0.00049;
        vBias[3] = 0.00015;
    }

    ~CPSSMCallBack(){};

	const video::SMaterial *UsedMaterial;
	virtual void OnSetMaterial(const SMaterial& material) {
		UsedMaterial = &material;
	}

    virtual void OnSetConstants(video::IMaterialRendererServices *services, s32 userData)
    {
        matrix4 Matrix;

		s32 spCount = PSSMUtils->getSplitsCount();
		services->setVertexShaderConstant("NumSplits", &spCount, 1);
		services->setPixelShaderConstant("NumSplits", &spCount, 1);

		Matrix = services->getVideoDriver()->getTransform(ETS_PROJECTION);
        Matrix *= services->getVideoDriver()->getTransform(ETS_VIEW);
        Matrix *= services->getVideoDriver()->getTransform(ETS_WORLD);
		services->setVertexShaderConstant("WVP", Matrix.pointer(), 16);

        float TextureOffset = 0.5f;

        matrix4 mTextureMatrixConst;
        mTextureMatrixConst[0] = 0.5f;
        mTextureMatrixConst[1] = 0.0f;
        mTextureMatrixConst[2] = 0.0f;
        mTextureMatrixConst[3] = 0.0f;

        mTextureMatrixConst[4] = 0.0f;
        mTextureMatrixConst[5] = -0.5f;

		if(services->getVideoDriver()->getDriverType() == EDT_OPENGL)
        mTextureMatrixConst[5] *= -1.0f;

        mTextureMatrixConst[6] = 0.0f;
        mTextureMatrixConst[7] = 0.0f;

        mTextureMatrixConst[8] = 0.0f;
        mTextureMatrixConst[9] = 0.0f;
        mTextureMatrixConst[10] = 1.0f;
        mTextureMatrixConst[11] = 0.0f;

        mTextureMatrixConst[12] = TextureOffset;
        mTextureMatrixConst[13] = TextureOffset;
        mTextureMatrixConst[14] = 0.0f;
        mTextureMatrixConst[15] = 1.0f;

        //TextureMatrix = cgGetNamedParameter(Vertex, "TextureMatrix");
        for(int i = 0; i < PSSMUtils->getSplitsCount(); i++)
        {
            TextureOffset = 0.5f;

            if(services->getVideoDriver()->getDriverType() == EDT_OPENGL)
            TextureOffset -= (0.5f / (float)UsedMaterial->TextureLayer[i].Texture->getSize().Width);
            else
            TextureOffset += (0.5f / (float)UsedMaterial->TextureLayer[i].Texture->getSize().Width);

            mTextureMatrixConst[12] = TextureOffset;
            mTextureMatrixConst[13] = TextureOffset;

            //CurrentParam = cgGetArrayParameter(TextureMatrix, i);

            Matrix = mTextureMatrixConst;
            Matrix *= PSSMUtils->getProjectionMatrix(i);
            Matrix *= PSSMUtils->getViewMatrix(i);
			Matrix *= services->getVideoDriver()->getTransform(ETS_WORLD);

			TextureMatrix[i] = Matrix;
            //Services->setMatrix( CurrentParam,ICGT_MATRIX_IDENTITY,Matrix );
			services->setVertexShaderConstant(core::stringc("TextureMatrix" + i).c_str(), TextureMatrix[i].pointer(), 16);
        }

        //SplitDistance = cgGetNamedParameter(Fragment, "SplitDistance");
        for(int i = 0; i < PSSMUtils->getSplitsCount(); i++)
        {
			SplitDistance[i] = PSSMUtils->getSplitDistance(i+1);
        }
		services->setPixelShaderConstant("SplitDistance", SplitDistance, 4);

		services->setPixelShaderConstant("Bias", vBias, 4);

        //TextureSize = cgGetNamedParameter(Fragment, "TextureSize");
        for(int i = 0; i < PSSMUtils->getSplitsCount(); i++)
        {
            float vTextureSize = (float)UsedMaterial->TextureLayer[i].Texture->getSize().Width;
            vTextureSize *= (i+1);
			TextureSize[i] = vTextureSize;

            //CurrentParam = cgGetArrayParameter(TextureSize, i);
            //Services->setParameter1f(CurrentParam, vTextureSize);
        }
		services->setPixelShaderConstant("TextureSize", TextureSize, 4);

        //DepthMap = cgGetNamedParameter(Fragment, "DepthMap");
        for(int i = 0; i < PSSMUtils->getSplitsCount(); i++)
        {
            //CurrentParam = cgGetArrayParameter(DepthMap, i);
            //Services->EnableTexture( CurrentParam,Material.TextureLayer[i].Texture);
			DepthMap[4] = i;
        }
		services->setPixelShaderConstant("DepthMap", DepthMap, 4);
	}

private:
    CPSSMUtils* PSSMUtils;
    irr::f32 vBias[4];
	core::matrix4 TextureMatrix[4];
	irr::f32 SplitDistance[4];
	irr::f32 Bias[4];
	irr::f32 TextureSize[4];
	irr::s32 DepthMap[4];
};

class CPSSMDepthCallBack: public video::IShaderConstantSetCallBack
{
public:
    CPSSMDepthCallBack(CPSSMUtils* vPSSMUtils) : PSSMUtils(vPSSMUtils)
    {
    }

    ~CPSSMDepthCallBack(){};

    virtual void OnSetConstants(video::IMaterialRendererServices *services, s32 userData)
    {
        matrix4 Matrix;

		Matrix = PSSMUtils->getProjectionMatrix(PSSMUtils->getCurrentPass());
		//Matrix = services->getVideoDriver()->getTransform(ETS_PROJECTION);
		Matrix *= PSSMUtils->getViewMatrix(PSSMUtils->getCurrentPass());
		//Matrix *= services->getVideoDriver()->getTransform(ETS_VIEW);
		Matrix *= services->getVideoDriver()->getTransform(ETS_WORLD);
		services->setVertexShaderConstant("WVP", Matrix.pointer(), 16);

		f32 Near = 0.1f;
		services->setPixelShaderConstant("Near", &Near, 1);
		f32 Far = 2000.f;
		services->setPixelShaderConstant("Far", &Far, 1);
    }

private:
    CPSSMUtils* PSSMUtils;
};

class SelectionPassCB : public video::IShaderConstantSetCallBack {
public:
	SelectionPassCB(EffectHandler* effectIn) : effect(effectIn) {};

	virtual void OnSetConstants(video::IMaterialRendererServices *services, s32 userData) {
		IVideoDriver* driver = services->getVideoDriver();

		worldViewProj = driver->getTransform(video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);

		services->setVertexShaderConstant("worldViewProj", worldViewProj.pointer(), 16);
	}

private:
	EffectHandler *effect;
	core::matrix4 worldViewProj;
};

class SSWE_RENDERS_API DepthShaderCB : public video::IShaderConstantSetCallBack {
public:
	DepthShaderCB(EffectHandler* effectIn) : effect(effectIn) {};

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData) {
		IVideoDriver* driver = services->getVideoDriver();

		worldViewProj = driver->getTransform(video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);

		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		services->setVertexShaderConstant("MaxD", &FarLink, 1);
	}

	EffectHandler* effect;
	f32 FarLink;
	core::matrix4 worldViewProj;
};

class SSWE_RENDERS_API GodRaysCB : public IPostProcessingRenderCallback {
    
public:
	GodRaysCB(irr::s32 materialTypeIn) : materialType(materialTypeIn) {}
    
	void OnPreRender(EffectHandler* effect) {
        core::vector2di scrPos = effect->getIrrlichtDevice()->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(screenPosition);
        core::vector2df screen((float)scrPos.X/(float)effect->getIrrlichtDevice()->getVideoDriver()->getScreenSize().Width,
                               (float)scrPos.Y/(float)effect->getIrrlichtDevice()->getVideoDriver()->getScreenSize().Height);
        screen.Y = 1-screen.Y;
        effect->setPostProcessingEffectConstant(materialType, "lightPositionOnScreen", reinterpret_cast<f32*>(&screen), 2);
    }
    
	void OnPostRender(EffectHandler* effect) {}
    
    //virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData) {}
    
    core::vector3df screenPosition;
    irr::s32 materialType;
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
