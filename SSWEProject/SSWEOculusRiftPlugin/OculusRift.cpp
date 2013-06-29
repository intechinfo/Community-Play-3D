#include "stdafx.h"
#include "OculusRift.h"


OculusRift::OculusRift(ISceneManager *sceneManager, IGUIEnvironment *guiEnv, EffectHandler *xEffect, ICameraSceneNode *camera)
{
	m_deviceName = "Oculus Rift";

	m_infoLoaded = false;
	m_deviceEnabled = false;
	m_rendersXEffect = false;
	m_rendersXEffectFullTraitement = false;
	m_activeCamera = NULL;
	m_sceneMngr = NULL;
	m_guiEnv = NULL;
	m_xEffect = NULL;

	m_activeCamera = camera;
	m_sceneMngr = sceneManager;
	m_guiEnv = guiEnv;
	m_xEffect = xEffect;

	m_eyeSpace = 0.64;
	m_headCamera = m_sceneMngr->addCameraSceneNode(0, vector3df(0, 0, 0), vector3df(0, 0, 10000), -1, false);
	m_leftCamera = m_sceneMngr->addCameraSceneNode(m_headCamera, m_headCamera->getPosition() - vector3df(m_eyeSpace/2, 0, 0), vector3df(-m_eyeSpace/2, 0, 10000));
	m_rightCamera = m_sceneMngr->addCameraSceneNode(m_headCamera, m_headCamera->getPosition() + vector3df(m_eyeSpace/2, 0, 0), vector3df(m_eyeSpace/2, 0, 10000));
	m_leftViewport = rect<s32>(0, 0, 0, 0);
	m_rightViewport = rect<s32>(0, 0, 0, 0);

	
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

	m_sensorDevice = *m_deviceManager->EnumerateDevices<SensorDevice>().CreateDevice();

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

void OculusRift::setName(stringc name)
{
	m_deviceName = name;
}

stringc OculusRift::getName()
{
	return m_deviceName;
}



bool OculusRift::isEnabled()
{
	return m_deviceEnabled;
}

void OculusRift::setEnable(bool enable)
{
	m_deviceEnabled = enable;
}

bool OculusRift::isXEffectRendered()
{
	return m_rendersXEffect;
}

void OculusRift::setXEffectRendered(bool enable)
{
	m_rendersXEffect = enable;
}

bool OculusRift::isRenderingXEffectFullTraitement()
{
	return m_rendersXEffectFullTraitement;
}

void OculusRift::setRenderingXEffectFullTraitement(bool enable)
{
	m_rendersXEffectFullTraitement = enable;
}



void OculusRift::setActiveCamera(ICameraSceneNode *camera)
{
	m_activeCamera = camera;
}

ICameraSceneNode* OculusRift::getActiveCamera()
{
	return m_activeCamera;
}

void OculusRift::setSceneManager(ISceneManager *sceneMngr)
{
	m_sceneMngr = sceneMngr;
}

ISceneManager* OculusRift::getSceneManager()
{
	return m_sceneMngr;
}

void OculusRift::setToolsSceneManager(ISceneManager *sceneMngr)
{
	m_toolsSceneMng = sceneMngr;
}

void OculusRift::setGUIEnvironment(IGUIEnvironment *guiEnv)
{
	m_guiEnv = guiEnv;
}

IGUIEnvironment* OculusRift::getGUIEnvironment()
{
	return m_guiEnv;
}

void OculusRift::setXEffect(EffectHandler *xEffect)
{
	m_xEffect = xEffect;
}

EffectHandler* OculusRift::getXEffect()
{
	return m_xEffect;
}



void OculusRift::drawScene()
{
	IVideoDriver *videoDriver = m_sceneMngr->getVideoDriver();

	m_leftViewport = rect<s32>(m_info.DesktopX + m_info.HResolution/2, m_info.DesktopY, m_info.DesktopX + m_info.HResolution, m_info.DesktopY + m_info.VResolution);
	m_rightViewport = rect<s32>(m_info.DesktopX, m_info.DesktopY, m_info.DesktopX + m_info.HResolution/2, m_info.DesktopY + m_info.VResolution);

	m_headCamera->setPosition(m_camera->getPosition());
	m_headCamera->setTarget(m_camera->getTarget());
	m_headCamera->setRotation(vector3df(m_fusionResult.GetOrientation().x, m_fusionResult.GetOrientation().y, m_fusionResult.GetOrientation().z));

	videoDriver->beginScene(true, true, video::SColor(255, 0, 0, 0));

	videoDriver->setViewPort(m_leftViewport);
	m_sceneMngr->setActiveCamera(m_leftCamera);
	m_sceneMngr->drawAll();

	videoDriver->setViewPort(m_rightViewport);
	m_sceneMngr->setActiveCamera(m_rightCamera);
	m_sceneMngr->drawAll();

	videoDriver->endScene();
}

void OculusRift::renderXEffectFullPostTraitement(ITexture *texture)
{
	/* NOT WORKING YET. TODO: Recalculate the texture itself that it fits Oculus Rift resolution
	if (m_rendersXEffectFullTraitement && m_rendersXEffect) {
		IVideoDriver *driver = m_sceneMngr->getVideoDriver();
		rect<s32> rt1Rect = rect<s32>(m_info.DesktopX + m_info.HResolution/2, m_info.DesktopY, m_info.DesktopX + m_info.HResolution, m_info.DesktopY + m_info.VResolution);
		rect<s32> rt2Rect = rect<s32>(m_info.DesktopX, m_info.DesktopY, m_info.DesktopX + m_info.HResolution/2, m_info.DesktopY + m_info.VResolution);
		rect<s32> textureRect1 = rect<s32>();
		rect<s32> textureRect2 = rect<s32>();
		driver->draw2DImage(texture, rt1Rect, rt1Rect);
	}*/
}

void OculusRift::drawGUI()
{

}