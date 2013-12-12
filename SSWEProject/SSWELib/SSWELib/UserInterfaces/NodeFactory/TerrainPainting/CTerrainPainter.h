//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_TERRAIN_PAINTER_H_INCLUDED__
#define __C_TERRAIN_PAINTER_H_INCLUDED__

#include "../../../Device/CDevices.h"

class CTerrainPainter {

public:

	CTerrainPainter(CDevices *_devices, ITerrainSceneNode *node, dimension2di terrainSize);
	~CTerrainPainter();

	f32 getMinHeight() { return minHeight; }
	f32 getMaxHeight() { return maxHeight; }

	s32 getVertexIndex() { return vertexIndex; }
	void setVertexIndex(s32 _vertexIndex) { vertexIndex = _vertexIndex; }

	u32 getCurrentStep() { return currentStep; }
	void setCurrentStep(u32 _currentStep) { currentStep = _currentStep; }

	f32 getCurrentRadius() { return currentRadius; }
	void setCurrentRadius(f32 _currentRadius) { currentRadius = _currentRadius; }

	dimension2di getTerrainSize() { return dimension2di(terrainSizeWidth, terrainSizeHeight); }

	//-----------------------------------
	//METHODS
	void applyTransform(s32 smoothFactor);

	void RaiseTerrainVertex(vector3df clickPos, f32 step, bool up);
	void smoothTerrain(IMeshBuffer* mb, u32 indice, s32 smoothFactor);

	void drawBrush(vector3df pos);
	//-----------------------------------

private:

	//-----------------------------------
	//METHODS
	f32 getHeightAt(vector3df pos);
	//-----------------------------------

	//-----------------------------------
	//DEVICES AND DATA
	CDevices *devices;
	ITerrainSceneNode *node;

	//VALUES
	s32 vertexIndex;
	s32 terrainSizeWidth, terrainSizeHeight;
	u32 currentStep;
	f32 currentRadius;

	f32 minHeight;
	f32 maxHeight;
	f32 heightInterval;

	array<array<S3DVertex2TCoords *> > terrainVerticesLines;
	array<array<bool> > againUpVertices;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS

	//-----------------------------------

};

#endif