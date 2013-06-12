#ifndef OCULUSRIFT_H
#define OCULUSRIFT_H

#include <OVR.h>
using namespace OVR;

class OculusRift
{
public:
	OculusRift();
	~OculusRift();

	void init();
	void clear();

	HMDInfo getInfo();

private:
	Ptr<DeviceManager> m_deviceManager;
	Ptr<HMDDevice> m_HMD;
	Ptr<SensorDevice> m_sensorDevice;
	SensorFusion m_fusionResult;
	HMDInfo m_info;
	bool m_infoLoaded;
};

#endif
