//
//  CUIEditParticleFlags.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/01/2014.
//
//

#ifndef __C_UI_PARTICLE_EDITOR_INTINTERPOLATORS_H_INCLUDED__
#define __C_UI_PARTICLE_EDITOR_INTINTERPOLATORS_H_INCLUDED__

#include "../../Device/CDevices.h"

class CUIParticleEditorInterpolators : public IEventReceiver {
public:
    
    CUIParticleEditorInterpolators(CDevices *_devices, SPK::Model *_model, IGUIElement *parent);
    ~CUIParticleEditorInterpolators();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    //-----------------------------------
	//METHODS
	void openAddEditWindow(f32 x, f32 y, f32 y1);
	void removeEditWindow();

	void fillTable();
    //-----------------------------------
    
    //-----------------------------------
	//DEVICE && DATAS
    CDevices *devices;

	SPK::Model *model;
    //-----------------------------------
    
    //-----------------------------------
	//GUI ELEMENTS
    IGUIWindow *window;

	IGUIListBox *paramsList; array<SPK::Interpolator*> interpolators;
	IGUICheckBox *enableLooping;
	IGUIEditBox *scaleXVariationeb, *offsetXVariationeb;

	IGUIButton *editEntry, *addEntryb, *removeEntry;
	IGUITable *entriesTable;

	IGUIButton *close;

	//EDIT ENTRY
	enum E_ENTRY_EDIT_TYPE {
		EEDT_UNKNOWN = 0,
		EEDT_ADD,
		EEDT_EDIT
	};
	E_ENTRY_EDIT_TYPE entryEditType;
	IGUIWindow *editEntryWindow;
	IGUIEditBox *xeditentry, *yeditentry, *y1editentry;
	IGUIButton *editEntryOk, *editEntryCancel;
    //-----------------------------------
    
};

#endif
