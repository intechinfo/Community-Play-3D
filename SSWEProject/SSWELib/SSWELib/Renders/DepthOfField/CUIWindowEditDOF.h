//
//  CUIEditEffects.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 20/11/12.
//
//

#ifndef __C_UI_WINDOW_EDIT_DOF_H_INCLUDED__
#define __C_UI_WINDOW_EDIT_DOF_H_INCLUDED__

#ifdef __COMPILE_WITH_DOF_SUPPORT_

#include "../../Device/CDevices.h"

class CUIWindowEditDOF : public IEventReceiver {

public:

	CUIWindowEditDOF(CDevices *_devices);
	~CUIWindowEditDOF();

	bool OnEvent(const SEvent &event);

private:

	CDevices *devices;

	struct SRange {
		SRange() {
			SRange(0, 0, 0);
		}
		SRange(s32 _min, s32 _max, s32 _divide) {
			min = _min;
			max = _max;
			divide = _divide;
		}
		s32 min;
		s32 max;
		u32 divide;
	} ranges[4];

	struct SValue {
		SValue() {
			SValue(L"", 0);
		}

		SValue(stringw _txt, f32 _value) {
			txt = _txt;
			value = _value;
		}
		stringw txt;
		f32 value;
	} values[4];

	//-----------------------------------
	//MAIN WINDOW
	IGUIWindow *window;

	IGUIScrollBar *bars[4];
	//-----------------------------------

};

#endif

#endif
