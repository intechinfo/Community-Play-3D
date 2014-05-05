/*
 *  devices.h
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __C_DEVICES_H_INCLUDED__
#define __C_DEVICES_H_INCLUDED__

#include <IDevices.h>
#include <ISSWELibPlugin.h>
#include <IGUICodeEditor.h>

#include "../../../SSWECore/SSWECore/Core/CCore.h"
#include "../../../SSWECore/SSWECore/Core/CCoreData.h"
#include "Core/CRenderCore.h"

#include "Core/Scripting/CLuaBinds.h"

#include "../../../SSWECore/SSWECore/Core/CCoreObjectPlacement.h"

#include "../UserInterfaces/ProcessesLogger/CUIProcessesLogger.h"

#include "../../../SSWERenders/Renders/PostProcessor/ScreenSpaceAmbientOcclusion.h"

#include "Monitor/MonitorRegister.h"

#include "../SceneNodes/Camera/CRiggedCamera.h"

#ifndef _IRR_OSX_PLATFORM_
    #include <omp.h>
#endif

#ifdef _IRR_OSX_PLATFORM_
	//Plateforms compatibility
#else

#endif

class CCoreUserInterface;
class irrBulletWorld;

enum DEVICES_FILE_OPEN_DIALOG_EVENTS {
	DEVICES_FILE_OPEN_DIALOG_EVENTS_CLOSE = 0x15000,
	DEVICES_FILE_OPEN_DIALOG_EVENTS_OK
};

class CDevices : public IDevices, public IEventReceiver {

public:

	CDevices(CCoreUserInterface *_coreUserInterface, bool playOnly = false);
	~CDevices();

	bool OnEvent(const SEvent &event);

	//-----------------------------------
	//DEVICE
	IrrlichtDevice *getDevice() { return Device; }
	IVideoDriver *getVideoDriver() { return driver; }

	ISceneManager *getSceneManager() { return smgr; }
	ISceneManager *getSecondSceneManager() { return effectSmgr; }

	array<ISceneManager *> *getSceneManagers() { return &smgrs; }
	void removeSceneManager(ISceneManager *smgrToDelete);
	void setSceneManagerToDraw(ISceneManager *smgrToDraw);
	ISceneManager *createNewSceneManager() { return effectSmgr->createNewSceneManager(); }
	ISceneManager *getRenderingSceneManager() { return smgrs[sceneManagerToDrawIndice]; }

	IGUIEnvironment *getGUIEnvironment() { return gui; }

	EventReceiver *getEventReceiver() { return &receiver; }
	bool isEditBoxEntered() { return editBoxEntered; }
	void setEditBoxEntered(bool _editBoxEntered) { editBoxEntered = _editBoxEntered; }

	stringc getWorkingDirectory() { return workingDirectory; }

	stringw getContextName() { return contextName; }
	void setContextName(stringw _contextName) { contextName = _contextName; }
	//-----------------------------------

	//-----------------------------------
	//RENDERS
	EffectHandler *getXEffect() { return effect; }
	E_FILTER_TYPE getXEffectFilterType() { return filterType; }
	void setXEffectDrawable(bool draw) { renderXEffect = draw; }
	bool isXEffectDrawable() { return renderXEffect; }
	stringc getShaderExt() { return shaderExt; }
	void rebuildXEffect();

	CRenderCallbacks *getRenderCallbacks() { return renderCallbacks; }

	bool isRenderScene() { return renderScene; }
	void setRenderScene(bool _renderScene) { renderScene = _renderScene; }

	void setRenderGUI(bool _renderGUI) { renderGUI = _renderGUI; }
	bool isRenderingGUI() { return renderGUI; }

	void setRenderFullPostTraitements(bool _render) { renderFullPostTraitements = _render; }
	bool isRenderingFullPostTraitements() { return renderFullPostTraitements; }
	//-----------------------------------

	//-----------------------------------
	//CORE
    CCoreUserInterface *getCoreUserInterface() { return coreUserInterface; }
    
	CCore *getCore() { return wolrdCore; };
	CCoreData *getCoreData() { return worldCoreData; }

	CCollisionManager *getCollisionManager() { return collisionManager; }
	CCoreObjectPlacement *getObjectPlacement() { return objPlacement; }

	CUIProcessesLogger *getProcessesLogger() { return processesLogger; }

	CScripting *getScripting() { return scripting; }

	irrBulletWorld *getBulletWorld() { return bulletWorld; }
	//-----------------------------------

	//-----------------------------------
	//CAMERAS
	ICameraSceneNode *getFPSCamera() { return camera_fps; }
	ICameraSceneNode *getMayaCamera() { return camera_maya; }
	CCameraRig *getCameraRig() { return camera_rig; }
	ICameraSceneNode *getScenarioCamera() { return scenarioCamera; }
	//-----------------------------------

	//-----------------------------------
	//METHODS
	void createDevice(SIrrlichtCreationParameters parameters);

	vector3df getCursorPosition() { return cursorBillBoard->getPosition(); }
	ISceneNode *getCursor() { return cursorBillBoard; }

	void updateDevice();
	void updateEntities();

	void reupdate(EffectHandler *_effect = 0);

	stringw getProjectName() { return projectName; }
	void setProjectName(stringw _projectName) { projectName = _projectName; }

	bool isOnlyForPlaying() { return isOnlyForPlay; }

	void applyAnimationToModel(irr::scene::ISceneNode *node, irr::u32 animationNumber);
	void applyAnimationFromNameToModel(irr::scene::ISceneNode *node, irr::core::stringc name);
	irr::s32 getCurrentAnimationIndiceOf(irr::scene::ISceneNode *node);
	//-----------------------------------

	//-----------------------------------
	//UI ADVANCED DIALOG METHODS
	IGUIFileOpenDialog *createFileOpenDialog(stringw title, IGUIElement *parent);
	CGUIFileSelector *createFileOpenDialog(stringw title, CGUIFileSelector::E_FILESELECTOR_TYPE type, bool modal=false);
	CGUIFileSelector *createFileOpenDialog(stringw title, CGUIFileSelector::E_FILESELECTOR_TYPE type, IGUIElement *parent, bool modal=false);

	IGUIWindow *addInformationDialog(stringw title, stringw text, s32 flag, bool modal=true, IGUIElement *parent=0);
	IGUIWindow *addErrorDialog(stringw title, stringw text, s32 flag);
	IGUIWindow *addWarningDialog(stringw title, stringw text, s32 flag);
	//-----------------------------------

	//-----------------------------------
	//UI ADVANCED DIALOG METHODS
	IGUICodeEditor *createGUICodeEditor();
	ISData *getSelectedData();
	//-----------------------------------

	//-----------------------------------
	//INPUT METHODS
	bool isCtrlPushed() { return ctrlWasPushed; }
	bool isShiftPushed() { return shiftWasPushed; }

	SKeyMap getKeyMap(int i) { return keyMap[i]; }
	void setKeyMap(SKeyMap _keyMap, int i) { keyMap[i] = _keyMap; }
	void applyKeyMapOnFPSCamera() { animatorFPS->setKeyMap(keyMap, 5); }
	//-----------------------------------

	//-----------------------------------
	//SHARED NODES
	ISceneNode *getSkydome() { return skydome; }
	void setSkydome(ISceneNode *node) { skydome = node; }
	ISceneNode *getSkyBox() { return skybox; }
	void setSkyBox(ISceneNode *node) { skybox = node; }
	//-----------------------------------

	//-----------------------------------
	//MONITOR
	MonitorRegister *getMonitorRegister() { return monitorRegister; }
	//-----------------------------------

private:

	//-----------------------------------
	//PRIVATE METHODS
	void drawGUI();
	void drawScene();
	//-----------------------------------

	//-----------------------------------
	//DEVICE
	IrrlichtDevice *Device;
	IVideoDriver *driver;

	ISceneManager *smgr, *effectSmgr;
	array<ISceneManager *> smgrs;
	u32 sceneManagerToDrawIndice;

	IGUIEnvironment *gui;

	EventReceiver receiver;
	bool editBoxEntered;

	stringw projectName, contextName;

	bool isOnlyForPlay;
	//-----------------------------------

	//-----------------------------------
	//RENDERS
	EffectHandler *effect;
	E_FILTER_TYPE filterType;
	stringc shaderExt;

	IPostProcessMotionBlur* motionBlur;
	CRenderCallbacks *renderCallbacks;

	bool renderScene;
	bool renderXEffect, renderGUI;
	bool renderFullPostTraitements;

	CRenderCore *renderCore;
	//-----------------------------------

	//-----------------------------------
	//CAMERAS
	SKeyMap keyMap[9];
	ICameraSceneNode *camera_fps, *camera_maya, *scenarioCamera;
	CCameraRig *camera_rig;
	ISceneNodeAnimatorCameraFPS *animatorFPS;

	IBillboardSceneNode *cursorBillBoard;
	//-----------------------------------

	//-----------------------------------
	//CORE
    CCoreUserInterface *coreUserInterface;
    
	CCore *wolrdCore;
	CCoreData *worldCoreData;

	CCollisionManager *collisionManager;
	CCoreObjectPlacement *objPlacement;

	CUIProcessesLogger *processesLogger;

	stringc workingDirectory;

	CScripting *scripting;

	irrBulletWorld *bulletWorld;
	//-----------------------------------

	//-----------------------------------
	//INPUT ELEMENTS
	bool ctrlWasPushed;
	bool shiftWasPushed;
	//-----------------------------------

	//-----------------------------------
	//SHARED NODES
	ISceneNode *skydome, *skybox;
	//-----------------------------------

	//-----------------------------------
	//MONITOR
	MonitorRegister *monitorRegister;
	//-----------------------------------
};

#endif
