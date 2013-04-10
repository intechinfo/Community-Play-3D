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
    
    CUIRightSceneTreeView(CDevices *_devices);
    ~CUIRightSceneTreeView();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    CDevices *devices;
	CCoreData *worldCore;

	IVideoDriver *driver;
    
    //-----------------------------------
    //GUI ELEMENTS
	IGUIWindow *window;

	IGUITreeView *sceneView;
	IGUITreeViewNode *rootNode;
	IGUITreeViewNode *terrainsNode, *treesNode;
	IGUITreeViewNode *objectsNode, *lightsNode;
    //-----------------------------------
    
	//-----------------------------------
    //METHODS
	s32 getImageListIndexForNodeType(ESCENE_NODE_TYPE type);

	void addChildrenBackWithArray(IGUITreeViewNode *treeNode, array<ISceneNode *> *nodes);
	void addChildrenBackRecursively(IGUITreeViewNode *treeNode, ISceneNode *node);
	//-----------------------------------
};

#endif
