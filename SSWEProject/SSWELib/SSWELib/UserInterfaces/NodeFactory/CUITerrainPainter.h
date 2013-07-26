//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_TERRAIN_PAINTER_H_INCLUDED__
#define __C_UI_TERRAIN_PAINTER_H_INCLUDED__

#include <IGraphicTool.h>

#include "../../Device/CDevices.h"

class CUIMainWindow;

class CUITerrainPainter : public IGraphicTool, public IEventReceiver, public IUpdate {

public:

	CUITerrainPainter(CDevices *_devices, STerrainsData _tdata, CUIMainWindow *mw);
	~CUITerrainPainter();

	void update();

	bool OnEvent(const SEvent &event);

	virtual void setDecalManager(DecalManager *decalManager) { decalMgr = decalManager; }
	virtual DecalManager *getDecalManager() const { return decalMgr; }

private:

	//-----------------------------------
	//METHODS
	void RaiseTerrainVertex(s32 index, f32 step, bool up);
	//-----------------------------------

	//-----------------------------------
	//DEVICES AND DATA
	CDevices *devices;
	bool leftClickDown, rightClickDown;

	//TIMER
	ITimer* timer;
	u32 then, then30;

	//TERRAIN INFORMATIONS
	ITerrainSceneNode *node;
	STerrainsData tdata;
	ITriangleSelector* terrainSelector;
	s32 vertexIndex;
	s32 terrainSize;
	array<array<S3DVertex2TCoords *>> terrainVerticesLines;

	//TOOLS
	DecalManager *decalMgr;
	ISceneNode* arrow;
	stringc circlePath;
	ITexture *circleTex;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	//-----------------------------------

};

#endif