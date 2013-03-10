/*
 *  devices.h
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __C_UI_CMAIN_WINDOW_H_INCLUDED__
#define __C_UI_CMAIN_WINDOW_H_INCLUDED__

#include "../../Device/CDevices.h"

#include "CUIWindowAddTerrain.h"
#include "CUIWindowAddTree.h"
#include "CUIWindowAddObject.h"
#include "CUIWindowAddLight.h"

#include "../CUIWindowEditNode.h"

enum GUI_MAIN_WINDOW_EVENTS {
    CXT_MAIN_WINDOW_EVENTS_ADD_OCTTREE = 0x20000,
    CXT_MAIN_WINDOW_EVENTS_DELETE_OCTTREE, CXT_MAIN_WINDOW_EVENTS_DELETE_OCTTREE_YES,
    
    CXT_MAIN_WINDOW_EVENTS_ADD_TREE,
    CXT_MAIN_WINDOW_EVENTS_DELETE_TREE, CXT_MAIN_WINDOW_EVENTS_DELETE_TREE_YES,
    
    CXT_MAIN_WINDOW_EVENTS_ADD_OBJECT,
    CXT_MAIN_WINDOW_EVENTS_DELETE_OBJECT, CXT_MAIN_WINDOW_EVENTS_DELETE_OBJECTS_YES,
    
    CXT_MAIN_WINDOW_EVENTS_ADD_LIGHT,
    CXT_MAIN_WINDOW_EVENTS_DELETE_LIGHT, CXT_MAIN_WINDOW_EVENTS_DELETE_LIGHT_LIGHT_YES,
    
    CXT_MAIN_WINDOW_EVENTS_ADD_DYNAMIC_L,
    CXT_MAIN_WINDOW_EVENTS_DELETE_DYNAMIC_L, CXT_MAIN_WINDOW_EVENTS_DELETE_DYNAMIC_L_YES,
    
    CXT_MAIN_WINDOW_EVENTS_ADD_WATER_SURFACE,
    CXT_MAIN_WINDOW_EVENTS_DELETE_WATER_SURFACE, CXT_MAIN_WINDOW_EVENTS_DELETE_WATER_SURFACE_YES
};

class CUIMainWindow : public IEventReceiver {
    
public:
    
    CUIMainWindow(CDevices *_devices);
    ~CUIMainWindow();
    
    void refresh();
    
    IGUIListBox *getActiveListBox();
    ISceneNode *getSelectedNode();
    stringc getSelectedNodePrefix(ISceneNode *node);
    void selectSelectedNode(ISceneNode *node);
    
    void cloneNode();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    CDevices *devices;
    
    IGUIWindow *mainWindow;
    bool isWindowed;
    
    //-----------------------------------
    //WINDOW GUI ELEMENTS
    IGUITabControl *tabCtrl;
    IGUITab *terrainsTab, *treesTab, *objectsTab, *lightsTab, *dynamicLightsTab, *waterSurfacesTab;
    
    IGUIListBox *terrainsListBox, *treesListBox, *objectsListBox, *lightsListBox, *dynamicListBox, *waterSurfacesListBox;
    IGUIButton *addTerrain, *removeTerrain;
    IGUIButton *addTree, *removeTree;
    IGUIButton *addObject, *removeObject;
    IGUIButton *addLight, *removeLight;
    IGUIButton *addDynamicL, *removeDynamicL;
    IGUIButton *addWaterSurface, *removeWaterSurface;
    
    CUIWindowAddOctTree *addOctTreeInstance;
    CUIWindowAddTree *addTreeInstance;
    CUIWindowAddObject *addObjectInstance;
    CUIWindowAddLight *addLightInstance;
    //-----------------------------------
    
    //-----------------------------------
    //SCENE ELEMENTS
    //LIGHTS
    IBillboardSceneNode *light_icon;
    
    //MOUSE NODE SELECTION
    ISceneCollisionManager *collisionManager;
    ISceneNode *previousNode;
    //-----------------------------------
    
};

#endif
