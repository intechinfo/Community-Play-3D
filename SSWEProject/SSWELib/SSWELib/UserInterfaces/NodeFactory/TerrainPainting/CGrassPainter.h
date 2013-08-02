//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_GRASS_PAINTER_H_INCLUDED__
#define __C_GRASS_PAINTER_H_INCLUDED__

#include "../../../Device/CDevices.h"

class CGrassPainter {

public:

	CGrassPainter(CDevices *_devices, ITerrainSceneNode * _node);
	~CGrassPainter();

	//-----------------------------------
	//METHODS
	s32 getCurrentQuality() { return currentQuality; }
	void setCurrentQuality(s32 _currentQuality) { currentQuality = _currentQuality; }

	s32 getCurrentScale() { return currentScale; }
	void setCurrentScale(s32 _currentScale) { currentScale = _currentScale; }

	s32 getCurrentDistance() { return currentDistance; }
	void setCurrentDistance(s32 _currentDistance) { currentDistance = _currentDistance; }

	array<ITexture *> *getTextures() { return &textures; }
	u32 getTextureIndex() { return textureIndex; }
	void setTextureIndex(u32 _textureIndex) { textureIndex = _textureIndex; }
	//-----------------------------------

	void paint(vector3df pos, s32 scale, s32 quality, s32 distance, bool remove);

private:

	//-----------------------------------
	//METHODS

	//-----------------------------------

	//-----------------------------------
	//STRUCTURES
	struct SGrassData {
		SGrassData(ISceneNode *_node, vector3df _pos, s32 _scale, s32 _quality) {
			pos = _pos;
			scale = _scale;
			quality = _quality;

			node = _node;
		}
		vector3df pos;
		s32 scale;
		s32 quality;
		ISceneNode *node;
		array<ISceneNode *> nodes;
	};
	//-----------------------------------

	//-----------------------------------
	//DEVICES AND DATA
	CDevices *devices;
	ITerrainSceneNode *node;

	core::list<SGrassData *> grassList;
	SMeshBuffer *buffer;
	SMesh *mesh;

	s32 material;

	//EDITING VALUES
	s32 currentScale;
	s32 currentQuality;
	s32 currentDistance;

	array<ITexture *> textures;
	u32 textureIndex;

	class CGrassShaderCallback : public IShaderConstantSetCallBack {
	public:
		CGrassShaderCallback::CGrassShaderCallback(IrrlichtDevice *_device, vector3df _lightPosv) {
			device = _device;
			lightPosv = _lightPosv;
		}

		virtual void OnSetConstants(irr::video::IMaterialRendererServices *services, irr::s32 userData) {
			IVideoDriver *driver = services->getVideoDriver();

			//VERTEX
			core::matrix4 worldViewProj;
			worldViewProj *= driver->getTransform(ETS_PROJECTION);
			worldViewProj *= driver->getTransform(ETS_VIEW);
			worldViewProj *= driver->getTransform(ETS_WORLD);
			services->setVertexShaderConstant("worldViewProj", worldViewProj.pointer(), 16);

			worldViewProj = driver->getTransform(ETS_WORLD);
			services->setVertexShaderConstant("mWorld", worldViewProj.pointer(), 16);

			irr::f32 camPos[3];
			device->getSceneManager()->getActiveCamera()->getAbsolutePosition().getAs3Values(&camPos[0]);
            services->setVertexShaderConstant("eyePositionO", &camPos[0], 3);

			irr::f32 lightPos[3];
			lightPosv.getAs3Values(&lightPos[0]);
            services->setVertexShaderConstant("LightPos", &lightPos[0], 3);

			f32 time = device->getTimer()->getTime();
			services->setVertexShaderConstant("time", &time, 1);

			//PIXEL

		}
	private:
		IrrlichtDevice *device;
		vector3df lightPosv;
	} *grassShaderCallback;
	//-----------------------------------

};

#endif