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
	IGUIEnvironment *getGUIEnvironment() { return gui; }
    
    EventReceiver *getEventReceiver() { return &receiver; }
    bool isEditBoxEntered() { return editBoxEntered; }
    void setEditBoxEntered(bool _editBoxEntered) { editBoxEntered = _editBoxEntered; }
    
    stringc getWorkingDirectory() { return workingDirectory; }
    //-----------------------------------
    
    //-----------------------------------
    //RENDERS
    ShaderGroup *getHDR() { return hdr; }
    void setHDRDrawable(bool draw) { renderHDR = draw; }
    bool isHDRDrawable() { return renderHDR; }
    
    EffectHandler *getXEffect() { return effect; }
    E_FILTER_TYPE getXEffectFilterType() { return filterType; }
    void setXEffectDrawable(bool draw) { renderXEffect = draw; }
    bool isXEffectDrawable() { return renderXEffect; }
    stringc getShaderExt() { return shaderExt; }
    
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
    //-----------------------------------
    
    //-----------------------------------
    //UI ADVANCED DIALOG METHODS
	void createFileOpenDialog(stringw title);
    
    void addInformationDialog(stringw title, stringw text, EMESSAGE_BOX_FLAG flag);
    void addErrorDialog(stringw title, stringw text, EMESSAGE_BOX_FLAG flag);
    void addWarningDialog(stringw title, stringw text, EMESSAGE_BOX_FLAG flag);
    //-----------------------------------
	
private:
    
    //-----------------------------------
    //DEVICE
	IrrlichtDevice *Device;
    IVideoDriver *driver;
    ISceneManager *smgr, *effectSmgr;
    IGUIEnvironment *gui;
    
    EventReceiver receiver;
    bool editBoxEntered;
    //-----------------------------------
    
    //-----------------------------------
    //RENDERS
    ShaderGroup* hdr;
    
    EffectHandler *effect;
	E_FILTER_TYPE filterType;
    stringc shaderExt;
    
    bool renderHDR, renderXEffect, renderGUI;
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
    //-----------------------------------
    
    //-----------------------------------
    //CORE
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
};

#endif
