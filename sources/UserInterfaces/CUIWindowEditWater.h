#ifndef CUIWINDOWEDITWATER_H
#define CUIWINDOWEDITWATER_H

#include "../Device/CDevices.h"

class CUIWindowEditWater : public IEventReceiver
{
	public:
		CUIWindowEditWater(CWaterSurface *waterSurface, CDevices *devices, rect<s32> parentSize);
		~CUIWindowEditWater();

		bool OnEvent(const SEvent &event);

	private:
		CDevices *m_devices;
		IGUIEnvironment *m_guiEnv;
		CWaterSurface *m_waterSurface;
		rect<s32> m_parentSize;
		IGUIWindow *m_window;
		IGUICheckBox *m_sinWaveCheckBox;
		IGUIStaticText *m_sinWaveText;
		IGUICheckBox *m_refractionCheckBox;
		IGUIStaticText *m_refractionText;
		IGUICheckBox *m_drawSceneCheckBox;
		IGUIStaticText *m_drawSceneText;
};

#endif // CUIWINDOWEDITWATER_H
