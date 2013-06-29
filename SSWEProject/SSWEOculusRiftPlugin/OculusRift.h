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
	void clear();

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

	ISceneManager *m_sceneMngr;
	ISceneManager *m_toolsSceneMng;
	IGUIEnvironment *m_guiEnv;
	EffectHandler *m_xEffect;

	float m_eyeSpace;
	ICameraSceneNode *m_headCamera;
	ICameraSceneNode *m_leftCamera;
	ICameraSceneNode *m_rightCamera;
	ICameraSceneNode *m_camera;
	rect<s32> m_leftViewport;
	rect<s32> m_rightViewport;
};

#endif
