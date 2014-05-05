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

class CObjectsUtils {
	
public:

	CObjectsUtils(CDevices *_devices, IGUITreeView *_treeView);
	~CObjectsUtils();

	void setTreeView(IGUITreeView *_treeView) { treeView = _treeView; }

	//-----------------------------------
	//METHODS
	void fill();

	void fillTerrains();
	void fillObjects();
	void filleLights();
	void fillVolumeLights();
	void fillWaterSurfaces();

	void createNodes();
	//-----------------------------------

private:

	//-----------------------------------
	//GUI ASSETS
	IGUITreeView *treeView;

	IGUITreeViewNode *rootNode;
	IGUITreeViewNode *terrainsNode;
	IGUITreeViewNode *objectsNode;
	IGUITreeViewNode *lightsNode;
	IGUITreeViewNode *volumeLightsNode;
	IGUITreeViewNode *waterSurfacesNode;
	//-----------------------------------

	//-----------------------------------
	//DATAS
	CDevices *devices;
	IGUIEnvironment *gui;

	ISceneNode *whoIstreeNodeSelected;
	//-----------------------------------

	//-----------------------------------
	//METHODS
	void addChildrenBackWithSDataArray(IGUITreeViewNode *treeNode, array<SData> *nodes);
	void addChildrenBackSDataRecursively(IGUITreeViewNode *treeNode, SData node);
	void addChildrenBackRecursively(IGUITreeViewNode *treeNode, ISceneNode *node);
	s32 getImageListIndexForNodeType(ESCENE_NODE_TYPE type);
	//-----------------------------------

};

#endif

