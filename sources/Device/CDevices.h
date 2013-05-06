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

#include "Core/CCore.h"
#include "Core/CCoreData.h"

#include "Core/CCoreObjectPlacement.h"

#include "../GUIExtension/OpenFileDialog/CGUIFileSelector.h"

#ifdef _IRR_OSX_PLATFORM_
	//Plateforms compatibility
#else

#endif

enum DEVICES_FILE_OPEN_DIALOG_EVENTS {
	DEVICES_FILE_OPEN_DIALOG_EVENTS_CLOSE = 0x15000,
	DEVICES_FILE_OPEN_DIALOG_EVENTS_OK
};

class CDevices : public IEventReceiver {

public:

	CDevices();
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

	bool isRenderScene() { return renderScene; }
	void setRenderScene(bool _renderScene) { renderScene = _renderScene; }

	void setRenderGUI(bool _renderGUI) { renderGUI = _renderGUI; }
	bool isRenderingGUI() { return renderGUI; }
	//-----------------------------------

	//-----------------------------------
	//CORE
	CCore *getCore() { return wolrdCore; };
	CCoreData *getCoreData() { return worldCoreData; }

	CCollisionManager *getCollisionManager() { return collisionManager; }
	CCoreObjectPlacement *getObjectPlacement() { return objPlacement; }
	//-----------------------------------

	//-----------------------------------
	//CAMERAS
	ICameraSceneNode *getFPSCamera() { return camera_fps; }
	ICameraSceneNode *getMayaCamera() { return camera_maya; }
	//-----------------------------------

	//-----------------------------------
	//METHODS
	void createDevice(SIrrlichtCreationParameters parameters);

	vector3df getCursorPosition() { return cursorBillBoard->getPosition(); }
	ISceneNode *getCursor() { return cursorBillBoard; }

	void updateDevice();
	void reupdate();

	stringw getProjectName() { return projectName; }
	void setProjectName(stringw _projectName) { projectName = _projectName; }
	//-----------------------------------

	//-----------------------------------
	//UI ADVANCED DIALOG METHODS
	IGUIFileOpenDialog *createFileOpenDialog(stringw title, IGUIElement *parent);
	CGUIFileSelector *createFileOpenDialog(stringw title, CGUIFileSelector::E_FILESELECTOR_TYPE type);
	CGUIFileSelector *createFileOpenDialog(stringw title, CGUIFileSelector::E_FILESELECTOR_TYPE type, IGUIElement *parent);

	IGUIWindow *addInformationDialog(stringw title, stringw text, s32 flag, bool modal=true, IGUIElement *parent=0);
	IGUIWindow *addErrorDialog(stringw title, stringw text, s32 flag);
	IGUIWindow *addWarningDialog(stringw title, stringw text, s32 flag);
	//-----------------------------------

	//-----------------------------------
	//INPUT METHODS
	bool isCtrlPushed() { return ctrlWasPushed; }
	bool isShiftPushed() { return shiftWasPushed; }
	//-----------------------------------

	//-----------------------------------
	//SHARED NODES
	ISceneNode *getSkydome() { return skydome; }
	void setSkydome(ISceneNode *node) { skydome = node; }
	ISceneNode *getSkyBox() { return skybox; }
	void setSkyBox(ISceneNode *node) { skybox = node; }
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
	//-----------------------------------

	//-----------------------------------
	//RENDERS
	EffectHandler *effect;
	E_FILTER_TYPE filterType;
	stringc shaderExt;

	IPostProcessMotionBlur* motionBlur;

	bool renderScene;
	bool renderXEffect, renderGUI;
	//-----------------------------------

	//-----------------------------------
	//CAMERAS
	SKeyMap keyMap[5];
	ICameraSceneNode *camera_fps, *camera_maya;
	IBillboardSceneNode *cursorBillBoard;
	//-----------------------------------

	//-----------------------------------
	//CORE
	CCore *wolrdCore;
	CCoreData *worldCoreData;

	CCollisionManager *collisionManager;
	CCoreObjectPlacement *objPlacement;

	stringc workingDirectory;
	//-----------------------------------

	//-----------------------------------
	//UI ADVANCED OPEN FILE DIALOG
	IGUIWindow *window;
	IGUIFileOpenDialog *dialog;
	IGUIImage *dialogPreview;
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
};

#endif
