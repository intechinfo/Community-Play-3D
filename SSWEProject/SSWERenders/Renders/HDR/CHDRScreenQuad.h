#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace video;

#include "CGlobalContext.h"
#include "CErrorHandling.h"

namespace Graphics {

//class for rendering a texture to the screen (assumed to be an RTT)
//Particular to the video driver given to it on initialization
class CHDRScreenQuad {

public:

	CHDRScreenQuad() {
		vd = GlobalContext::DeviceContext.GetVideoDriver();

		const SColor white(255, 255, 255, 255);
		/*vertices[0].Pos = vector3df(-1.f, 1.f, 0.f);
		vertices[0].TCoords = vector2df(0.f, 0.f);
		vertices[0].Color = white;
		vertices[1].Pos = vector3df(1.f, 1.f, 0.f);
		vertices[1].TCoords = vector2df(1.f, 0.f);
		vertices[1].Color = white;
		vertices[2].Pos = vector3df(1.f, -1.f, 0.f);
		vertices[2].TCoords = vector2df(1.f, 1.f);
		vertices[2].Color = white;
		vertices[3].Pos = vector3df(-1.f, -1.f, 0.f);
		vertices[3].TCoords = vector2df(0.f, 1.f);
		vertices[3].Color = white;*/
        vertices[0] = irr::video::S3DVertex(-1.0f,-1.0f,0.0f,0,0,1,irr::video::SColor(0x0),0.0f,1.0f);
        vertices[1] = irr::video::S3DVertex(-1.0f, 1.0f,0.0f,0,0,1,irr::video::SColor(0x0),0.0f,0.0f);
        vertices[2] = irr::video::S3DVertex( 1.0f, 1.0f,0.0f,0,0,1,irr::video::SColor(0x0),1.0f,0.0f);
        vertices[3] = irr::video::S3DVertex( 1.0f,-1.0f,0.0f,0,0,1,irr::video::SColor(0x0),1.0f,1.0f);
        for (u32 i=0; i < 4; i++)
            vertices[i].Color = white;
        
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 0;
		indices[4] = 2;
		indices[5] = 3;

		//mat.BackfaceCulling = false;
		mat.Lighting = false;
		//mat.ZBuffer = video::ECFN_ALWAYS;
		mat.ZWriteEnable = false;
		mat.Wireframe = false;
		mat.AntiAliasing = true;
		mat.setFlag(video::EMF_BILINEAR_FILTER, true);
		mat.setFlag(video::EMF_TRILINEAR_FILTER, true);
		mat.setFlag(video::EMF_ANISOTROPIC_FILTER, true);
		mat.UseMipMaps=false;
		for(u32 c = 0; c < MATERIAL_MAX_TEXTURES; c++) {
			mat.TextureLayer[c].TextureWrapU = video::ETC_CLAMP;
			mat.TextureLayer[c].TextureWrapV = video::ETC_CLAMP;
		}
	}

	SMaterial& GetMaterial() { return mat; }

	//Set the texture to render with the quad
	void SetTexture(ITexture* __restrict tex, u32 layer = 0) {
		mat.TextureLayer[layer].Texture = tex;
	}

	ITexture* GetTexture(u32 layer = 0) { return mat.TextureLayer[layer].Texture; }

	void SetMaterialType(E_MATERIAL_TYPE mt) { mat.MaterialType = mt; }

	void Render(bool setRTToFrameBuff = true) {

		if(setRTToFrameBuff)
			vd->setRenderTarget(video::ERT_FRAME_BUFFER);
		vd->setMaterial(mat);

        vd->setTransform(ETS_WORLD, core::IdentityMatrix);
        vd->setTransform(ETS_VIEW, core::IdentityMatrix);
        vd->setTransform(ETS_PROJECTION, core::IdentityMatrix);
        
		vd->drawIndexedTriangleList(&vertices[0], 4, &indices[0], 2);
	}

	void Render(ITexture* rt) {

		vd->setRenderTarget(rt);
		vd->setMaterial(mat);

        vd->setTransform(ETS_WORLD, core::IdentityMatrix);
        vd->setTransform(ETS_VIEW, core::IdentityMatrix);
        vd->setTransform(ETS_PROJECTION, core::IdentityMatrix);
        
		vd->drawIndexedTriangleList(&vertices[0], 4, &indices[0], 2);
	}

protected:

	S3DVertex vertices[4];
	u16 indices[6];
	SMaterial mat;

	IVideoDriver* __restrict vd;

};

} //end namespace graphics
