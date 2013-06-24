#ifndef OCULUSRIFT_H
#define OCULUSRIFT_H

#include <OVR.h>
using namespace OVR;

#include <irrlicht.h>
#include <IMonitor.h>

class OculusRift : IMonitor
{
public:
	OculusRift();
	~OculusRift();

	void init();
	void clear();

	HMDInfo getInfo();
	bool isOculusRiftEnabled();
	void setOculusRiftEnabled(bool isEnabled);

private:
	Ptr<DeviceManager> m_deviceManager;
	Ptr<HMDDevice> m_HMD;
	Ptr<SensorDevice> m_sensorDevice;
	SensorFusion m_fusionResult;
	HMDInfo m_info;
	bool m_infoLoaded;

	bool m_isOculusRiftEnabled;
};

#endif
