//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_TERRAIN_PAINTING_H_INCLUDED__
#define __C_UI_TERRAIN_PAINTING_H_INCLUDED__

#include "../../../Device/CDevices.h"

#include "CTerrainPainter.h"
#include "CGrassPainter.h"

class CUIMainWindow;

class CUITerrainPainter : public cp3d::core::IUpdate, public IEventReceiver {

public:

	CUITerrainPainter(CDevices *_devices, STerrainsData _tdata, CUIMainWindow *mw);
	~CUITerrainPainter();

	void update();

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
	//METHODS

	//-----------------------------------

	//-----------------------------------
	//DEVICES AND DATA
	CDevices *devices;
	bool leftClickDown, rightClickDown;

	CTerrainPainter *terrainPainter;
	CGrassPainter *grassPainter;

	//TIMER
	ITimer* timer;

	//TERRAIN INFORMATIONS
	ITerrainSceneNode *node;
	STerrainsData tdata;
	ITriangleSelector* terrainSelector;

	vector3df oldTerrainScale, oldTerrainRotation, oldTerrainPosition;

	//TOOLS
	ISceneNode* arrow;

	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	IGUIWindow *window;

	IGUITabControl *tabctrl;
	IGUITab *painting;
	IGUITab *grassPainting;
	IGUITab *treePainting;

	//TERRAIN
	IGUIEditBox *stepValueeb, *radiusValueeb;
	IGUIScrollBar *stepsb, *radiussb;
	IGUIEditBox *smoothFactor;
	IGUIButton *runSmoothing;
	//GRASS
	IGUIEditBox *grassScaleValueeb, *grassQualityeb, *grassSpaceeb;
	IGUIScrollBar *grassScalesb, *grassQualitysb, *grassSpacesb;

	IGUIButton *grassAddTexture, *grassRemoveTexture;
	IGUIListBox *grassTextures;
	IGUIFileOpenDialog *grassChooseTexture;
	//WINDOW
	IGUIButton *cancel, *accept;
	//-----------------------------------

	enum E_PAINTING_TYPE {
		EPT_TERRAIN,
		EPT_GRASS
	} paintingType;

};

#endif