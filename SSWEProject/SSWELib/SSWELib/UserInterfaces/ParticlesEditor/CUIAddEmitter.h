//
//  CUIEditParticleFlags.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/01/2014.
//
//

#ifndef __C_UI_PARTICLE_EDITOR_ADD_EMITTER_H_INCLUDED__
#define __C_UI_PARTICLE_EDITOR_ADD_EMITTER_H_INCLUDED__

#include "../../Device/CDevices.h"
#include "../../GUIExtension/ViewPort/CGUIViewPort.h"
#include "../../GUIExtension/NodesEditor/CGUINodesEditor.h"

class CUIAddEmitter : public IEventReceiver {
public:
    
	CUIAddEmitter(CDevices *_devices, IGUIElement *parent, SPK::Group *_group, CGUINodesEditor *_editor, CGUINode *_parentNode);
    ~CUIAddEmitter();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    //-----------------------------------
	//METHODS
	void createDirectionGUI();
	void createAngleGUI();

	void clearEditionZone();
    //-----------------------------------
    
    //-----------------------------------
	//DEVICE && DATAS
	CDevices *devices;

	SPK::Group *group;
	SPK::Emitter *currentEmitter;

	CGUINodesEditor *editor;
	CGUINode *parentNode;
    //-----------------------------------
    
    //-----------------------------------
	//GUI ELEMENTS
	IGUIWindow *window;
	CGUIViewport *viewPort;
	IGUIComboBox *emitterTypecb;
	IGUIStaticText *editionZonet;

	//DIRECTION
	IGUIEditBox *dirx, *diry, *dirz;
	IGUIEditBox *angleA, *angleB;
	IGUICheckBox *invertedcb;

	//BUTTON
	IGUIButton *close, *accept;
    //-----------------------------------

	enum E_CURRENT_EMITTER_TYPE {
		ECET_STRAIGHT = 0,
		ECET_STATIC,
		ECET_SPHERIC,
		ECET_RANDOM,
		ECET_NORMAL
	} currentEmitterType;
    
};

#endif
