// Created by Etienne Rocipon on 09/04/13
//
//

#ifndef CUIWINDOWADDWATERSSURFACE_H
#define CUIWINDOWADDWATERSSURFACE_H

#include "../../Device/CDevices.h"

class CUIWindowAddWaterSurface : public IEventReceiver
{
public:
    CUIWindowAddWaterSurface(CDevices *device, IGUIListBox *waterSurfacesListBox);
    ~CUIWindowAddWaterSurface();

    void openWindow();

    bool OnEvent(const SEvent &event);

private:
    CDevices *m_devices;
    IGUIListBox *m_waterSurfacesListBox;

    IGUIWindow *m_window;
    IGUIStaticText *m_nameLabel;
    IGUIEditBox *m_nameEditBox;
    IGUIButton *m_selectButton;
    IGUIButton *m_acceptButton;
    IGUIButton *m_cancelButton;

	stringw m_filePath;
	bool m_isFileDialogOpen;
};

#endif // CUIWINDOWADDWATERSSURFACE_H
