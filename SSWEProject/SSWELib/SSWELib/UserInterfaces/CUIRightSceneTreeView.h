//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_RIGHT_SCENE_TREE_VIEW_H_INCLUDED__
#define __C_UI_RIGHT_SCENE_TREE_VIEW_H_INCLUDED__

#include "UIContextMenu.h"

class CUIRightSceneTreeView : public IEventReceiver {
    
public:
    
    CUIRightSceneTreeView(CDevices *_devices, CUIContextMenu *_cxtMenu);
    ~CUIRightSceneTreeView();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    CDevices *devices;
	CCoreData *worldCore;
	CCore *core;
	CUIContextMenu *cxtMenu;

	IVideoDriver *driver;
    
    //-----------------------------------
    //GUI ELEMENTS
	IGUIWindow *window;

	IGUITreeView *sceneView;
	IGUITreeViewNode *rootNode;
	IGUITreeViewNode *terrainsNode, *treesNode;
	IGUITreeViewNode *objectsNode, *lightsNode;
	IGUITreeViewNode *volumeLightsNode, *waterSurfacesNode;

	IGUIContextMenu *rightClickCxtMenu;
    //-----------------------------------

	//-----------------------------------
    //DATAS
	ISceneNode *whoIstreeNodeSelected;

	bool alwaysVisible;
	//-----------------------------------
    
	//-----------------------------------
    //METHODS
	s32 getImageListIndexForNodeType(ESCENE_NODE_TYPE type);

	void addChildrenBackWithArray(IGUITreeViewNode *treeNode, array<ISceneNode *> *nodes);
	void addChildrenBackWithSDataArray(IGUITreeViewNode *treeNode, array<SData> *nodes);
	void addChildrenBackRecursively(IGUITreeViewNode *treeNode, ISceneNode *node);
	void addChildrenBackSDataRecursively(IGUITreeViewNode *treeNode, SData node);
	//-----------------------------------
};

#endif
