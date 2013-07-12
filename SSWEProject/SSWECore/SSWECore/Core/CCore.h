//
//  enums.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 04/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef __C_CORE_H_INCLUDED__
#define __C_CORE_H_INCLUDED__

#include <SSWECore.h>

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <assert.h>

#include <stdlib.h>
#include <stdio.h>

#include <irrlicht.h>

#ifndef _IRR_OSX_PLATFORM_
	#include <memory>
	#include <thread>
	#include <Windows.h>
	#include <mutex>
#endif

//RENDERS
#include "../../../SSWERenders/Renders/XEffect/XEffects.h"

//SCENE NODES
#include "../../../SSWELib/SSWELib/SceneNodes/LensFlareSceneNode.h"
#include "../../../SSWELib/SSWELib/SceneNodes/SceneNodeAnimatorFollowCamera.h"

//BULLET
#include "src/Bullet-C-Api.h"
#include "src/btBulletDynamicsCommon.h"
#include "src/btBulletCollisionCommon.h"

using namespace irr;
using namespace video;
using namespace gui;
using namespace core;
using namespace scene;
using namespace io;

enum E_CORE_USER_EVENTS {
	ECUE_REACTIVE_MINIMIZED_WINDOW = 0,

	ECUE_TEXTURE_REMOVED
};

//--------------------------
//CORE CLASS

class SSWE_CORE_API CCore {
    
public:
	
	CCore();
	~CCore();

	void setDevice(IrrlichtDevice *_device) { device = _device; }
    
    //CORE METHODS AND FUNCTIONS
	std::string convertToString(stringw _textToConvert);
    
	vector3df getVector3df(std::string X, std::string Y, std::string Z);
    vector3df getVector3df(stringw X, stringw Y, stringw Z);
	
	dimension2d<u32> getDimensionU32(std::string sizeW, std::string sizeH);
	dimension2d<f32> getDimensionF32(std::string sizeW, std::string sizeH);
	
	f32 getF32(stringc valuef32);
	u32 getU32(stringc valueu32);
    s32 getS32(stringc values32);

	stringw getStrNumber(f32 value);
    stringw getStrNumberU32(u32 value);
	stringw getStrVector3df(vector3df v);

	stringc getStringcFromFile(stringc pathFile);
	stringc getStringcFromIReadFile(stringc pathFile);
	
	stringw changeTextWithValue(const wchar_t *text, s32 value);

    stringw getTexturePath(ITexture *texture);

	stringc getNodeNamePrefix(ISceneNode *node);

	//DRIVER METHODS
	s32 textureAlreadyExists(stringc name, IVideoDriver *driver);

	//SCENE METHODS
	ISceneNode *clone(ISceneNode *node, stringc meshPath, ISceneManager *smgr);
	s32 nodeExistsInArray(array<ISceneNode *> *nodes, ISceneNode *node);
	array<ISceneNode *> *getArrayOfAListOfNodeChildren(ISceneNode *node);

	//GUI METHODS
	bool elementIsInArrayOfElements(IGUIElement *element, array<IGUIElement *> elements);
	array<IGUIElement *> getArrayOfAListOfGUIElementChildren(IGUIElement *element);
	void deactiveChildrenOfGUIElement(IGUIElement *element, bool visible);
	void fillArrayOfGUIElementsFromArrayOfGUIElements(array<IGUIElement *> *toFill, array<IGUIElement *> source);
	void maximizeWindow(IGUIWindow *window, rect<s32> minRelativePosition);
    
	//VIDEO METHODS
	u32 getNumberOfBuildInMaterialTypes();
	array<stringc> getArrayOfBuildInMaterialTypes();

	//BULLET METHODS
	btVector3 getBtVector3(vector3df vector) { return btVector3(vector.X, vector.Y, vector.Z); }
    
private:
	//--------------------------
    //DATAS
	IrrlichtDevice *device;
	//--------------------------

	//--------------------------
    //CORE PRIVATE METHODS AND FUNCTIONS
	bool readtoken(IReadFile* f, stringc* str);
	bool readline(IReadFile* f, stringc* str);
	//--------------------------

};

//--------------------------

//--------------------------
//MULTIPLE EVENT RECEIVERS CLASS

class EventReceiver : public IEventReceiver {

public:
    
	//--------------------------
	//MULTIPLE EVENTS MANAGER
    virtual bool OnEvent(const SEvent& mainEvent) {
        for (u32 i=0; i < mEventReceivers.size(); ++i) {
            mEventReceivers[i]->OnEvent(mainEvent);
			if (i < mGuiWindows.size()) {
				if (mGuiWindows[i] && mainEvent.EventType == EET_GUI_EVENT) {
					if (mainEvent.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
						if (mainEvent.GUIEvent.Caller == ((IGUIWindow *)mGuiWindows[i])->getMinimizeButton()) {
							mGuiWindows[i]->setVisible(false);
						}
					}
				}
			}
        }
        return false;
    }
    
    void AddEventReceiver(IEventReceiver * receiver, IGUIWindow *window=0) {
        mEventReceivers.push_back(receiver);
		if (window) {
			IGUIStaticText *txt = gui->addStaticText(window->getText(), rect<s32>(0, 0, 0, 0), false, false, 0, -1, false);
 			txt->setToolTipText(window->getText());
 			txt->setOverrideColor(SColor(255, 255, 255, 255));
 			txt->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
			txt->setID(mGuiWindows.size());
			txt->setDrawBorder(false);
			mNames.push_back(txt);
			mGuiWindows.push_back(window);
			if (window)
				gui->setFocus(window);
		} else {
			mNames.push_back(0);
			mGuiWindows.push_back(0);
		}
    }
    
    bool RemoveEventReceiver(IEventReceiver * receiver) {
        for (unsigned int i=0; i < mEventReceivers.size(); ++i) {
            if (mEventReceivers[i] == receiver) {
                mEventReceivers.erase(i);
				mGuiWindows.erase(i);
				if (mNames[i])
					mNames[i]->remove();
				mNames.erase(i);
            }
        }
        return false;
    }

	//--------------------------
	//MINIMIZED WINDOWS MANAGER
	void ReactiveWindow(s32 ID) {
		if (ID < 0 || ID >= mGuiWindows.size())
			return;

		mGuiWindows[ID]->setVisible(true);
		gui->getRootGUIElement()->bringToFront(mGuiWindows[ID]);
	}

	u32 GetMinimizedWindowsCount() { return mEventReceivers.size(); }

	IGUIElement *getWindowName(u32 i) { return mNames[i]; }

	void setDriver(IVideoDriver *_driver) { driver = _driver; }
	void setGUI(IGUIEnvironment *_gui) { gui = _gui; }
    
private:

    array<IEventReceiver *> mEventReceivers;
	array<IGUIWindow *> mGuiWindows;
	array<IGUIStaticText *> mNames;

	IVideoDriver *driver;
	IGUIEnvironment *gui;

};
//--------------------------

#endif
