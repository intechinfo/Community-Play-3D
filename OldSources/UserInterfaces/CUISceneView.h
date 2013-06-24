//
//  CUIWindowEditNode.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 29/09/12.
//
//

#ifndef __C_UI_SCENE_VIEW_H_INCLUDED__
#define __C_UI_SCENE_VIEW_H_INCLUDED__

#include "../Device/CDevices.h"

enum CXT_VIEW_SCENE_VIEW_WINDOW_EVENTS {
    CXT_VIEW_SCENE_VIEW_CLOSE = 0x90000,
};

class CUISceneView : public IEventReceiver {
    
public:
    
    CUISceneView(CDevices *_devices);
    ~CUISceneView();
    
    void open();
    void refresh();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    void addSceneTreeItem(ISceneNode *parent, IGUITreeViewNode *nodeParent);
    
    CDevices *devices;
    
    //-----------------------------------
    //GUI ELEMENTS
    IGUITreeView *nodeTreeView;
	IGUIWindow *windowSceneTreeNode;
    IGUIButton *closeButton;
    //-----------------------------------
    
    bool windowed;
};

#endif
