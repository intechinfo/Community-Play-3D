/* defined(__MacOSX__CUIWindowAddVolumeLight__) *///
//  CUIWindowAddVolumeLight.h
//  MacOSX
//
//  Created by Arthur Caron on 28/05/2013
//
//

#ifndef __C_UI_WINDOW_ADD_VOLUME_LIGHT_H_INCLUDED__
#define __C_UI_WINDOW_ADD_VOLUME_LIGHT_H_INCLUDED__

#include "Device/CDevices.h"

class CUIWindowAddVolumeLight : public IEventReceiver {
    
public:
    
    CUIWindowAddVolumeLight(CDevices *_devices, IGUIListBox *_lightsListBox);
    ~CUIWindowAddVolumeLight();
    
    void open();
	bool isValid();
    
    bool OnEvent(const SEvent &event);
    
private:
    
    CDevices *devices;
    
    IGUIListBox *volumeLightsListBox;

    IGUIWindow *addVolumeLightWindow;
    IGUIEditBox *volumeLightNameEditBox;
	IGUIComboBox *volumeLightParentComboBox;
	IGUIEditBox *volumeLightSubdivUEditBox;
	IGUIEditBox *volumeLightSubdivVEditBox;
	IGUIEditBox *volumeLightAFootColorEditBox;
	IGUIEditBox *volumeLightRFootColorEditBox;
	IGUIEditBox *volumeLightGFootColorEditBox;
	IGUIEditBox *volumeLightBFootColorEditBox;
	IGUIEditBox *volumeLightATailColorEditBox;
	IGUIEditBox *volumeLightRTailColorEditBox;
	IGUIEditBox *volumeLightGTailColorEditBox;
	IGUIEditBox *volumeLightBTailColorEditBox;
	IGUIEditBox *volumeLightXPositionEditBox;
	IGUIEditBox *volumeLightYPositionEditBox;
	IGUIEditBox *volumeLightZPositionEditBox;
	IGUIEditBox *volumeLightXRotationEditBox;
	IGUIEditBox *volumeLightYRotationEditBox;
	IGUIEditBox *volumeLightZRotationEditBox;
	IGUIEditBox *volumeLightXScaleEditBox;
	IGUIEditBox *volumeLightYScaleEditBox;
	IGUIEditBox *volumeLightZScaleEditBox;

	stringw name;
	u32 subdivU;
	u32 subdivV;
	u32 aFootColor;
	u32 rFootColor;
	u32 gFootColor;
	u32 bFootColor;
	u32 aTailColor;
	u32 rTailColor;
	u32 gTailColor;
	u32 bTailColor;
	vector3df position;
	vector3df rotation;
	vector3df scale;

	IGUIButton *acceptButton;
	IGUIButton *closeButton;
};

#endif
