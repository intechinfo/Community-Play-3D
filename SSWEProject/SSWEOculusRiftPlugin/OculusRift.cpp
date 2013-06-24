#include "stdafx.h"
#include "OculusRift.h"


OculusRift::OculusRift()
{
	m_infoLoaded = false;
	m_isOculusRiftEnabled = false;
}


OculusRift::~OculusRift()
{
}

void OculusRift::init()
{
	System::Init();

	m_deviceManager = *DeviceManager::Create();

	m_HMD = *m_deviceManager->EnumerateDevices<HMDDevice>().CreateDevice();

	if(m_HMD)
	{
		m_infoLoaded = m_HMD->GetDeviceInfo(&m_info);

		m_sensorDevice = m_HMD->GetSensor();
	}
	else
	{
		m_sensorDevice = *m_deviceManager->EnumerateDevices<SensorDevice>().CreateDevice();
	}

	if(m_sensorDevice)
		m_fusionResult.AttachToSensor(m_sensorDevice);
}

void OculusRift::clear()
{
	m_sensorDevice.Clear();
	m_HMD.Clear();
	m_deviceManager.Clear();

	System::Destroy();
}

HMDInfo OculusRift::getInfo()
{
	return m_info;
}

bool OculusRift::isOculusRiftEnabled()
{
	return m_isOculusRiftEnabled;
}

void OculusRift::setOculusRiftEnabled(bool isEnabled)
{
	m_isOculusRiftEnabled = isEnabled;
}
