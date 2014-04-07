/*
 *  UIContextMenu.h
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __C_UI_PARTICLE_EDITOR_H_INCLUDED__
#define __C_UI_PARTICLE_EDITOR_H_INCLUDED__

#include "../../Device/CDevices.h"

#include "../../GUIExtension/ViewPort/CGUIViewPort.h"
#include "../../GUIExtension/NodesEditor/CGUINodesEditor.h"

class CUIParticleEditor : public IEventReceiver {

public:

	CUIParticleEditor(CDevices *_devices, SParticleSystem *_ps);
	~CUIParticleEditor();

	bool OnEvent(const SEvent &event);

	enum E_PS_DATA_TYPE {
        EPSDT_SYSTEM = 0,
        EPSDT_GROUP,
        EPSDT_EMITTER,
        EPSDT_RENDERER,
        EPSDT_MODEL,
		EPSDT_MODIFIER
    };

private:

	//-----------------------------------
	//DEVICE
	CDevices *devices;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
    //GENERAL
    IGUIWindow *window;
    
    IGUIContextMenu *menu;
    
    CGUINodesEditor *nodesEditor;
    IGUIViewport *viewPort;
    
    IGUICheckBox *enableClampStep;
    IGUIEditBox *clampStepeb;
    
    //ADD MODEL
    IGUIWindow *addModelWindow;
    IGUICheckBox *addModelFlagsEnabled[10];
    IGUICheckBox *addModelFlagsMutable[10];
    IGUICheckBox *addModelFlagsrandom[10];
    IGUICheckBox *addModelFlagsInterpolated[10];
    IGUIButton *addModelClose, *addModelOK;
    IGUIEditBox *addModelCapacity;
    
    //EDIT MODEL
    
    //EDIT RENDERER
    IGUIFileOpenDialog *openRendererTexture;
	//-----------------------------------

	//-----------------------------------
	//DATAS
    SParticleSystem *ps;
    
    CGUINode *selectedNode;
    
    array<stringw> paramNames;
	//-----------------------------------
    
    //-----------------------------------
	//METHODS
    void openAddModel();
    void addGroup();
    
    void createGroup(CGUINode *node, SPK::Model *model, SPK::Group *group, SPK::System *system);
    //-----------------------------------

};

#endif
