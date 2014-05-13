//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_OBJECTS_UTILS_H_INCLUDED__
#define __C_OBJECTS_UTILS_H_INCLUDED__

#include "../../../Device/CDevices.h"

class CUIScenarioMakerMain;

class CUIScenarioObjects {
	
public:

	CUIScenarioObjects(CDevices *_devices, IGUITreeView *_treeView, CUIScenarioMakerMain *_scenarioMain);
	~CUIScenarioObjects();

	void setTreeView(IGUITreeView *_treeView) { treeView = _treeView; }

	//-----------------------------------
	//METHODS
	void fill();

	void fillAnimators();
	void fillSounds();
	void fillScripts();
	void fillEvents();
	void fillScene();

	void createNodes();
	//-----------------------------------

private:

	//-----------------------------------
	//GUI ASSETS
	IGUITreeView *treeView;

	IGUITreeViewNode *rootNode;
	IGUITreeViewNode *animatorsNode;
	IGUITreeViewNode *soundsNode;
	IGUITreeViewNode *scriptsNode;
	IGUITreeViewNode *eventsNode;
	IGUITreeViewNode *sceneNode;
	//-----------------------------------

	//-----------------------------------
	//DATAS
	CDevices *devices;
	IGUIEnvironment *gui;

	ISceneNode *whoIstreeNodeSelected;

	CUIScenarioMakerMain *scenarioMain;
	//-----------------------------------

	//-----------------------------------
	//METHODS
	void addChildrenBackWithSDataArray(IGUITreeViewNode *treeNode, array<SData> *nodes);
	void addChildrenBackSDataRecursively(IGUITreeViewNode *treeNode, SData node);
	void addChildrenBackRecursively(IGUITreeViewNode *treeNode, ISceneNode *node);
	s32 getImageListIndexForNodeType(ESCENE_NODE_TYPE type);
	//-----------------------------------

	//-----------------------------------
	//ENUMS
	enum E_SCENARIO_OBJECTS_RC {
		ESORC_CREATE_NEW = 0
	};
	//-----------------------------------

};

#endif

