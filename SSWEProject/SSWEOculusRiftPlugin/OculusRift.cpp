#include "stdafx.h"
#include "OculusRift.h"

static const char *pixelShader =
"sampler2D RT : register(s0);"
"uniform float2 LensCenter;"
"uniform float2 ScreenCenter;"
"uniform float2 Scale;"
"uniform float2 ScaleIn;"
"uniform float4 HmdWarpParam;"
""
// Scales input texture coordinates for distortion.
"float2 HmdWarp(float2 in01)"
"{"
"   float2 theta = (in01 - LensCenter) * ScaleIn;" // Scales to [-1, 1]
"   float rSq = theta.x * theta.x + theta.y * theta.y;"
"   float2 rvector = theta * (HmdWarpParam.x + HmdWarpParam.y * rSq + HmdWarpParam.z * rSq * rSq + HmdWarpParam.w * rSq * rSq * rSq);"
"   return LensCenter + Scale * rvector;"
"}"
""
"float4 main_fp(float4 pos : POSITION, float2 iTexCoord : TEXCOORD0) : COLOR"
"{"
"   float2 tc = HmdWarp(iTexCoord);"
"   return float4(tex2D(RT, tc).rgb,1);"
"}";

void* createMonitor() {
	return static_cast< void* > (new OculusRift);
}

OculusRift::OculusRift()
{
	m_deviceName = "Oculus Rift";

	m_infoLoaded = false;
	m_deviceEnabled = true;
	m_rendersXEffect = false;
	m_rendersXEffectFullTraitement = false;
	m_device = NULL;
	m_sceneMngr = NULL;
	m_toolsSceneMng = NULL;
	m_guiEnv = NULL;
	m_xEffect = NULL;

	m_camera = NULL;
	m_headCamera = NULL;
	m_leftCamera = NULL;
	m_rightCamera = NULL;
	m_videoDriver = NULL;
	m_renderTexture = NULL;
	
	m_timer = NULL;

	m_eyeSpace = 0.64;
	m_leftViewport = rect<s32>(0, 0, 0, 0);
	m_rightViewport = rect<s32>(0, 0, 0, 0);

	
}


OculusRift::~OculusRift()
{
}

void OculusRift::init()
{
	System::Init();

	isOculusConnected();

	if(m_infoLoaded)
	{
		m_device = createDevice(EDT_DIRECT3D9, dimension2d<u32>(m_info.HResolution, m_info.VResolution), 32, false, true, true);
		m_device->setWindowCaption(L"SSWE Oculus Rift Plugin");
		m_sceneMngr = m_device->getSceneManager();
		m_videoDriver = m_device->getVideoDriver();
		m_timer = m_device->getTimer();

		// Init shader parameters
		m_distortionCallback.scale[0] = 0.3f;
		m_distortionCallback.scale[1] = 0.35f;
		m_distortionCallback.scaleIn[0] = 2.0f; 
		m_distortionCallback.scaleIn[1] = 2.0f;
		m_distortionCallback.lensCenter[0] = 0.5f;
		m_distortionCallback.lensCenter[1] = 0.5f;
		m_distortionCallback.screenCenter[0] = 0.0f;
		m_distortionCallback.screenCenter[1] = 0.0f;
		m_distortionCallback.hmdWarpParam[0] = 1.0f;
		m_distortionCallback.hmdWarpParam[1] = 0.22f;
		m_distortionCallback.hmdWarpParam[2] = 0.24f;
		m_distortionCallback.hmdWarpParam[3] = 0.0f;

		// Plane
		m_planeVertices[0] = S3DVertex(-1.0f, -1.0f, 1.0f,1,1,0, SColor(255,0,255,255), 0.0f, 0.0f);
		m_planeVertices[1] = S3DVertex(-1.0f,  1.0f, 1.0f,1,1,0, SColor(255,0,255,255), 0.0f, 1.0f);
		m_planeVertices[2] = S3DVertex( 1.0f,  1.0f, 1.0f,1,1,0, SColor(255,0,255,255), 1.0f, 1.0f);
		m_planeVertices[3] = S3DVertex( 1.0f, -1.0f, 1.0f,1,1,0, SColor(255,0,255,255), 1.0f, 0.0f);
		m_planeIndices[0] = 0; 
		m_planeIndices[1] = 1; 
		m_planeIndices[2] = 2; 
		m_planeIndices[3] = 0; 
		m_planeIndices[4] = 2; 
		m_planeIndices[5] = 3;

		// Create shader material
		m_renderMaterial.Wireframe = false;
		m_renderMaterial.Lighting = false;
		m_renderMaterial.TextureLayer[0].TextureWrapU = ETC_CLAMP;
		m_renderMaterial.TextureLayer[0].TextureWrapV = ETC_CLAMP;

		IGPUProgrammingServices* gpu = m_videoDriver->getGPUProgrammingServices(); 
		m_renderMaterial.MaterialType=(E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterial(0, "main", EVST_VS_3_0, pixelShader, "main_fp", EPST_PS_3_0, &m_distortionCallback);

		// Setting up the HMD
		float aspect = ((float)m_info.HResolution/2) / (float)m_info.VResolution;

		// Fov is normally computed with:
		//   2*atan2(HMD.vScreenSize,2*HMD.eyeToScreenDistance)
		// But with lens distortion it is increased (see Oculus SDK Documentation)
		float r = -1.0f - (4.0f * (m_info.HScreenSize/4.0f - m_info.LensSeparationDistance/2.0f) / m_info.HScreenSize);
		float distScale = (m_info.DistortionK[0] + m_info.DistortionK[1] * pow(r,2) + m_info.DistortionK[2] * pow(r,4) + m_info.DistortionK[3] * pow(r,6));
		float fov = 2.0f*atan2(m_info.VScreenSize*distScale, 2.0f*m_info.EyeToScreenDistance);

		// Compute camera offset
		m_eyeSpace = m_worldScale * m_info.InterpupillaryDistance/2.0f;

		// Compute Viewport
		m_leftViewport = rect<s32>(0, 0, m_info.HResolution/2, m_info.VResolution);
		m_rightViewport = rect<s32>(m_info.HResolution/2, 0, m_info.HResolution, m_info.VResolution);

		// Distortion shader parameters

		m_distortionCallback.scale[0] = 1.0f/distScale;
		m_distortionCallback.scale[1] = 1.0f*aspect/distScale;
  
		m_distortionCallback.scaleIn[0] = 1.0f;
		m_distortionCallback.scaleIn[1] = 1.0f/aspect;
  
		m_distortionCallback.hmdWarpParam[0] = m_info.DistortionK[0];
		m_distortionCallback.hmdWarpParam[1] = m_info.DistortionK[1];
		m_distortionCallback.hmdWarpParam[2] = m_info.DistortionK[2];
		m_distortionCallback.hmdWarpParam[3] = m_info.DistortionK[3];
	}
}

void OculusRift::destroy()
{
	m_sensorDevice.Clear();
	m_HMD.Clear();
	m_deviceManager.Clear();

	if(m_device)
		m_device->drop();
	System::Destroy();
}

HMDInfo OculusRift::getInfo()
{
	return m_info;
}

double OculusRift::degToRad(double deg)
{
    return deg * 0.314 / 180.0;
}

bool OculusRift::isOculusConnected()
{
	if(!m_deviceManager)
		m_deviceManager = *DeviceManager::Create();

	if(!m_HMD)
		m_HMD = *m_deviceManager->EnumerateDevices<HMDDevice>().CreateDevice();

	if(m_HMD)
	{
		m_infoLoaded = m_HMD->GetDeviceInfo(&m_info);

		m_sensorDevice = m_HMD->GetSensor();
	}

	m_sensorDevice = *m_deviceManager->EnumerateDevices<SensorDevice>().CreateDevice();

	if(m_sensorDevice)
		m_fusionResult.AttachToSensor(m_sensorDevice);

	if(m_infoLoaded)
		return true;
	else
		return false;
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
	m_camera = camera;
}

ICameraSceneNode* OculusRift::getActiveCamera()
{
	return m_camera;
}

void OculusRift::setSceneManager(ISceneManager *sceneMngr)
{
	m_sceneMngr = sceneMngr;
	m_device->setInputReceivingSceneManager(sceneMngr);
	IGPUProgrammingServices* gpu = m_sceneMngr->getVideoDriver()->getGPUProgrammingServices(); 
	m_renderMaterial.MaterialType = (E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterial(0, "main", EVST_VS_3_0, pixelShader, "main_fp", EPST_PS_3_0, &m_distortionCallback);
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
	if(m_videoDriver)
	{
		if(this->isOculusConnected())
		{
			IVideoDriver *videoDriver = m_sceneMngr->getVideoDriver();

			videoDriver->setMaterial(m_renderMaterial);

			ICameraSceneNode *camera = m_sceneMngr->addCameraSceneNode();
			camera->setPosition(m_camera->getPosition());

			m_sceneMngr->setActiveCamera(camera);
			videoDriver->setViewPort(m_leftViewport);
			m_sceneMngr->drawAll();

			camera->setPosition(vector3df(m_camera->getPosition().X , m_camera->getPosition().Y , m_camera->getPosition().Z));

			videoDriver->setViewPort(m_rightViewport);
			m_sceneMngr->drawAll();

			m_sceneMngr->setActiveCamera(m_camera);
		}
	}
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
	if(m_videoDriver)
	{
		IVideoDriver *videoDriver = m_sceneMngr->getVideoDriver();

		videoDriver->setMaterial(m_renderMaterial);

		videoDriver->setViewPort(m_leftViewport);
		m_guiEnv->getRootGUIElement()->move(vector2d<s32>(100, 0));
		m_guiEnv->drawAll();
		videoDriver->setViewPort(m_rightViewport);
		m_guiEnv->getRootGUIElement()->move(vector2d<s32>(-200,0));
		m_guiEnv->drawAll();

		m_guiEnv->getRootGUIElement()->move(vector2d<s32>(100,0));
	}
}