#ifndef CUIWINDOWEDITWATER_H
#define CUIWINDOWEDITWATER_H

#include "../Device/CDevices.h"
#include "ITool.h"

class CUIWindowEditWater : public IEventReceiver, public ITool
{
	public:
		CUIWindowEditWater(SWaterSurfacesData *waterSurfaceData, CDevices *devices, rect<s32> position);
		~CUIWindowEditWater();

		bool OnEvent(const SEvent &event);

		void close();

	private:
		CDevices *m_devices;
		IGUIEnvironment *m_guiEnv;
		SWaterSurfacesData *m_waterSurfaceData;
		CWaterSurface *m_waterSurface;
		rect<s32> m_position;
		IGUIWindow *m_window;
		IGUICheckBox *m_sinWaveCheckBox;
		IGUIStaticText *m_sinWaveText;
		IGUICheckBox *m_refractionCheckBox;
		IGUIStaticText *m_refractionText;
};

#endif // CUIWINDOWEDITWATER_H
