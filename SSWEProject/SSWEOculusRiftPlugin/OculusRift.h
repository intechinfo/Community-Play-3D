//
//  CExporter.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#ifndef __C_OCULUS_RIFT_MONITOR_H_INCLUDED__
#define __C_OCULUS_RIFT_MONITOR_H_INCLUDED__

#include <irrlicht.h>

#include <IMonitor.h>
#include <DLLExport.h>

struct HMDDescriptor 
{
  int hResolution;
  int vResolution;
  float hScreenSize;
  float vScreenSize;
  float interpupillaryDistance;
  float lensSeparationDistance;
  float eyeToScreenDistance;
  float distortionK[4];
};

class COculusRoftMonitor : public IMonitor {
public:

	COculusRoftMonitor();
	~COculusRoftMonitor();

	HMDDescriptor getHMD(); 
	void setHMD(HMDDescriptor HMD);

	irr::f32 getWorldScale(); 
	void setWorldScale(irr::f32 worldScale);

	//-----------------------------------
	//IMONITOR
	void init();
	void destroy() { }

	void setActiveCamera(irr::scene::ICameraSceneNode *camera);
	irr::scene::ICameraSceneNode* getActiveCamera() { return monitorCamera; }

	void setSceneManager(irr::scene::ISceneManager *sceneManager);
	irr::scene::ISceneManager* getSceneManager() { return smgr; }

	void setGUIEnvironment(irr::gui::IGUIEnvironment *guiEnv) { gui = guiEnv; }
	irr::gui::IGUIEnvironment* getGUIEnvironment() { return gui; }

	void drawScene();
	void drawGUI();
	//-----------------------------------

private:

	//-----------------------------------
	//RENDERS
	irr::video::IVideoDriver *driver;
	irr::scene::ISceneManager *smgr;
	irr::gui::IGUIEnvironment *gui;
	//-----------------------------------

	//-----------------------------------
	//SCENE NODES
	irr::scene::ICameraSceneNode *monitorCamera;
	//-----------------------------------

	irr::video::ITexture* _renderTexture;

	HMDDescriptor _HMD;
	irr::f32 _worldScale;
	irr::core::matrix4 _projectionLeft;
	irr::core::matrix4 _projectionRight;
	irr::f32 _eyeSeparation;
	irr::f32 _lensShift;

	irr::core::rect<irr::s32> _viewportLeft;
	irr::core::rect<irr::s32> _viewportRight;

	irr::scene::ICameraSceneNode* _pCamera;

	irr::video::SMaterial _renderMaterial;
	irr::video::S3DVertex _planeVertices[4];
	irr::u16 _planeIndices[6];
	irr::ITimer* _timer;

	class OculusDistorsionCallback: public IPostProcessingRenderCallback
	{ 
	public:
		irr::f32 scale[2];
		irr::f32 scaleIn[2];
		irr::f32 lensCenter[2];
		irr::f32 hmdWarpParam[4];
		irr::s32 materialType;

		OculusDistorsionCallback() { OculusDistorsionCallback(0); }
		OculusDistorsionCallback(irr::s32 _materialType) : materialType(_materialType) { }

		void OnPreRender(ISSWERender *effect) 
		{
			effect->setPostProcessingEffectConstant(materialType, "scale", scale, 2);
			effect->setPostProcessingEffectConstant(materialType, "scaleIn", scaleIn, 2);
			effect->setPostProcessingEffectConstant(materialType, "lensCenter", lensCenter, 2);
			effect->setPostProcessingEffectConstant(materialType, "hmdWarpParam", hmdWarpParam, 4);
		}

		void OnPostRender(ISSWERender *effect) { }
	};
	OculusDistorsionCallback _distortionCB;

};

#endif
