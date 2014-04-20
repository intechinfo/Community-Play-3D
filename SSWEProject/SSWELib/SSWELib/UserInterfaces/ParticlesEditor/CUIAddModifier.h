//
//  CUIEditParticleFlags.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/01/2014.
//
//

#ifndef __C_UI_PARTICLE_EDITOR_ADD_MODIFIER_H_INCLUDED__
#define __C_UI_PARTICLE_EDITOR_ADD_MODIFIER_H_INCLUDED__

#include "../../Device/CDevices.h"
#include "../../GUIExtension/ViewPort/CGUIViewPort.h"
#include "../../GUIExtension/NodesEditor/CGUINodesEditor.h"

#include "../../Device/Core/ParticleSystems/CParticleSystemsTypes.h"

class CUIAddModifier : public IEventReceiver {
public:
    
	CUIAddModifier(CDevices *_devices, IGUIElement *parent, SPK::Group *_group, CGUINodesEditor *_editor, CGUINode *_parentNode);
    ~CUIAddModifier();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    //-----------------------------------
	//METHODS
	void addVector3D1(stringw name);
	void addVector3D2(stringw name);
	void addTwoParameters(stringw name1, stringw name2, bool addedVector1, bool addedVector2);

	void modifyVector1(SPK::Vector3D v);
	void modifyVector2(SPK::Vector3D v);
	void modifyTwoParameters(float f1, float f2);

	void createNode();

	void fillTriggerCB(IGUIComboBox *cb);
	void fillForceFactorCB(IGUIComboBox *cb);
    //-----------------------------------
    
    //-----------------------------------
	//DEVICE && DATAS
	CDevices *devices;

	SPK::Group *group;
	SPK::Modifier *currentModifier;

	CGUINodesEditor *editor;
	CGUINode *parentNode;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	IGUIWindow *window;
	CGUIViewport *viewPort;
	IGUIComboBox *modifierTypecb;
	IGUIStaticText *editZone;

	IGUIEditBox *v1_1, *v1_2, *v1_3;
	IGUIEditBox *v2_1, *v2_2, *v2_3;
	IGUIEditBox *eb1, *eb2;

	IGUIButton *accept, *cancel;
	//-----------------------------------

	E_PS_GROUP_MODIFIER_TYPE modifierType;

};

#endif
