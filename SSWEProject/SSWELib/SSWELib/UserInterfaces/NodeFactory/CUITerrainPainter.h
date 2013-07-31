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
	void smoothTerrain(IMeshBuffer* mb, s32 smoothFactor);
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

	vector3df oldTerrainScale, oldTerrainRotation, oldTerrainPosition;
	s32 vertexIndex;
	s32 terrainSizeWidth, terrainSizeHeight;
	f32 minHeight, maxHeight, heightInterval;

	//TOOLS
	DecalManager *decalMgr;
	ISceneNode* arrow;
	stringc circlePath;
	ITexture *circleTex;

	//TERRAIN PAINTING VALUES
	u32 currentStep;
	f32 currentRadius;

	struct SVerticesBeginInformations {
		SVerticesBeginInformations() {
			againUp = true;
		}

		S3DVertex2TCoords *vertice;
		bool againUp;
	};
	struct SVerticesInformations {
		SVerticesInformations(S3DVertex2TCoords *_vertice, vector2di _position, bool _againUp) {
			vertice = _vertice;
			position = _position;
			againUp = _againUp;
		}

		S3DVertex2TCoords *vertice;
		vector2di position;
		bool againUp;
	};
	array<array<S3DVertex2TCoords *>> terrainVerticesLines;
	array<array<SVerticesBeginInformations>> terrainVerticesBeginLines;

	bool canPaint;
	f32 positionYTo;

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