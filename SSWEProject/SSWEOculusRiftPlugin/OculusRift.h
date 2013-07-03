#ifndef OCULUSRIFT_H
#define OCULUSRIFT_H

#include <OVR.h>
using namespace OVR;

#include <irrlicht.h>
using namespace irr;
using namespace video;
using namespace gui;
using namespace core;
using namespace scene;

#include <IMonitor.h>
#include <DLLExport.h>

class OculusRift : public IMonitor
{
public:
	OculusRift();
	~OculusRift();

	void init();
	void destroy();

	HMDInfo getInfo();

	bool isOculusConnected();

	void setName(stringc name);
	stringc getName();

	bool isEnabled();
	void setEnable(bool enable);
	bool isXEffectRendered();
	void setXEffectRendered(bool enable);
	bool isRenderingXEffectFullTraitement();
	void setRenderingXEffectFullTraitement(bool enable);

	void setActiveCamera(ICameraSceneNode *camera);
	ICameraSceneNode* getActiveCamera();
	void setSceneManager(ISceneManager *sceneMngr);
	ISceneManager* getSceneManager();
	void setToolsSceneManager(ISceneManager *sceneMngr);
	void setGUIEnvironment(IGUIEnvironment *guiEnv);
	IGUIEnvironment* getGUIEnvironment();
	void setXEffect(EffectHandler *xEffect);
	EffectHandler* getXEffect();

	void drawScene();
	void renderXEffectFullPostTraitement(ITexture *texture);
	void drawGUI();

private:
	Ptr<DeviceManager> m_deviceManager;
	stringc m_deviceName;
	Ptr<HMDDevice> m_HMD;
	Ptr<SensorDevice> m_sensorDevice;
	SensorFusion m_fusionResult;
	HMDInfo m_info;
	bool m_infoLoaded;
	bool m_deviceEnabled;
	bool m_rendersXEffect;
	bool m_rendersXEffectFullTraitement;

	IrrlichtDevice *m_device;
	ISceneManager *m_sceneMngr;
	ISceneManager *m_toolsSceneMng;
	IGUIEnvironment *m_guiEnv;
	EffectHandler *m_xEffect;

	float m_eyeSpace;
	ICameraSceneNode *m_camera;
	ICameraSceneNode *m_projectionCamera;
	rect<s32> m_leftViewport;
	rect<s32> m_rightViewport;

	IVideoDriver* m_videoDriver;
	ITexture* m_renderTexture;

	f32 m_worldScale;
	matrix4 m_projectionLeft;
	matrix4 m_projectionRight;
	f32 m_lensShift;

	SMaterial m_renderMaterial;
	S3DVertex m_planeVertices[4];
	u16 m_planeIndices[6];
	ITimer *m_timer;

	class OculusDistorsionCallback: public irr::video::IShaderConstantSetCallBack 
	{ 
	public:
		f32 scale[2];
		f32 scaleIn[2];
		f32 lensCenter[2];
		f32 screenCenter[2];
		f32 hmdWarpParam[4];
		virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData) 
		{ 
			irr::video::IVideoDriver* driver = services->getVideoDriver();
			services->setPixelShaderConstant("Scale", scale, 2);
			services->setPixelShaderConstant("ScaleIn", scaleIn ,2);
			services->setPixelShaderConstant("LensCenter", lensCenter ,2);
			//services->setPixelShaderConstant("ScreenCenter", screenCenter, 2);
			services->setPixelShaderConstant("HmdWarpParam", hmdWarpParam ,4);
		}
	};

	OculusDistorsionCallback m_distortionCallback;
};

#endif
