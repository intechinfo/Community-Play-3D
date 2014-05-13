//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_SCENARIO_MAKER_MAIN_H_INCLUDED__
#define __C_UI_SCENARIO_MAKER_MAIN_H_INCLUDED__

#include "../../Device/CDevices.h"

#include "../../GUIExtension/ViewPort/CGUIViewPort.h"
#include "../../GUIExtension/TimeLine/CGUITimeLine.h"

class CUIScenarioObjects;

class CUIScenarioMakerMain : public IEventReceiver, public IUpdate {
	
	friend class CUIScenarioObjects;
	
public:

	CUIScenarioMakerMain(CDevices *_devices);
	~CUIScenarioMakerMain();

	void update();
	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
	//GUI ASSETS
	IGUIWindow *window;
	IGUIContextMenu *menu;
	IGUIToolBar *bar;

	IGUIWindow *objectsZone, *viewPortZone, *inspectZone;
	IGUIWindow *timeLineZone;

	/// Objects zone
	IGUITreeView *objectsView;

	/// Viewport zone
	IGUIViewport *viewPort;

	/// Timeline zone
	CGUITimeLine *timeline;
	//-----------------------------------

	//-----------------------------------
	//DATAS
	CDevices *devices;

	IGUIEnvironment *gui;

	CUIScenarioObjects *objectsUtils;
	//-----------------------------------

	//-----------------------------------
	//METHODS
	void createObjectsView(rect<s32> size);

	void maximize();
	//-----------------------------------

};

#endif

