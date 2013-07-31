//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_TERRAIN_PAINTER_H_INCLUDED__
#define __C_UI_TERRAIN_PAINTER_H_INCLUDED__

#include "../../Device/CDevices.h"

class CUIMainWindow;

class CUITerrainPainter : public IUpdate, public IEventReceiver {

public:

	CUITerrainPainter(CDevices *_devices, STerrainsData _tdata, CUIMainWindow *mw);
	~CUITerrainPainter();

	void update();

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
	//METHODS
	void RaiseTerrainVertex(vector3df clickPos, f32 step, bool up);
	void smoothTerrain(IMeshBuffer* mb, u32 indice, s32 smoothFactor);

	void drawBrush(vector3df pos);
	f32 getHeightAt(vector3df pos);
	//-----------------------------------

	//-----------------------------------
	//DEVICES AND DATA
	CDevices *devices;
	bool leftClickDown, rightClickDown;

	//TIMER
	ITimer* timer;

	//TERRAIN INFORMATIONS
	ITerrainSceneNode *node;
	STerrainsData tdata;
	ITriangleSelector* terrainSelector;

	vector3df oldTerrainScale, oldTerrainRotation, oldTerrainPosition;
	s32 vertexIndex;
	s32 terrainSizeWidth, terrainSizeHeight;
	f32 minHeight, maxHeight, heightInterval;

	//TOOLS
	ISceneNode* arrow;

	//TERRAIN PAINTING VALUES
	u32 currentStep;
	f32 currentRadius;

	array<array<S3DVertex2TCoords *>> terrainVerticesLines;
	array<array<bool>> againUpVertices;

	bool canPaint;

	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	IGUIWindow *window;

	IGUIToolBar *bar;
	IGUIButton *exportHeightMapbtn;

	IGUIEditBox *stepValueeb, *radiusValueeb;
	IGUIScrollBar *stepsb, *radiussb;

	IGUIEditBox *smoothFactor;
	IGUIButton *runSmoothing;

	IGUIButton *cancel, *accept;
	//-----------------------------------

};

#endif