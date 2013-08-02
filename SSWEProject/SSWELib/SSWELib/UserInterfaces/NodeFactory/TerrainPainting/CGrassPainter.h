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

	//EDITING VALUES
	s32 currentScale;
	s32 currentQuality;
	s32 currentDistance;
	//-----------------------------------

};

#endif