//
//  enums.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 04/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef __C_CORE_H_INCLUDED__
#define __C_CORE_H_INCLUDED__

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
	#include <thread>
#endif

#include "../../Renders/XEffect/XEffects.h"

#include "../../SceneNodes/LensFlareSceneNode.h"
#include "../../SceneNodes/SceneNodeAnimatorFollowCamera.h"

#include "CLuaBinds.h"

using namespace irr;
using namespace video;
using namespace gui;
using namespace core;
using namespace scene;
using namespace io;

enum E_CORE_USER_EVENTS {
	ECUE_REACTIVE_MINIMIZED_WINDOW = 0
};

//--------------------------
//CORE CLASS

class CCore {
    
public:
	
	CCore();
	~CCore();

	void setDevice(IrrlichtDevice *_device) { device = _device; }

	array<IEventReceiver *> getWindows() { return windows; }
    
    //--------------------------
    //CORE METHODS AND FUNCTIONS
	std::string convertToString(stringw _textToConvert);
    
	vector3df getVector3df(std::string X, std::string Y, std::string Z);
    vector3df getVector3df(stringw X, stringw Y, stringw Z);
	
	dimension2d<u32> getDimensionU32(std::string sizeW, std::string sizeH);
	dimension2d<f32> getDimensionF32(std::string sizeW, std::string sizeH);
	
	f32 getF32(std::string valuef32);
	u32 getU32(std::string valueu32);
    s32 getS32(std::string values32);
	
	stringw getStrNumber(f32 value);
    stringw getStrNumberU32(u32 value);

	stringc getStringcFromFile(stringc pathFile);
	stringc getStringcFromIReadFile(stringc pathFile);
	
	stringw changeTextWithValue(const wchar_t *text, s32 value);
    
    stringw getTexturePath(ITexture *texture);

    ISceneNode *clone(ISceneNode *node, stringc meshPath, ISceneManager *smgr);

	s32 textureAlreadyExists(stringc name, IVideoDriver *driver);
	s32 nodeExistsInArray(array<ISceneNode *> *nodes, ISceneNode *node);
	array<ISceneNode *> *getArrayOfAListOfNodeChildren(ISceneNode *node);

	bool elementIsInArrayOfElements(IGUIElement *element, array<IGUIElement *> elements);
	array<IGUIElement *> getArrayOfAListOfGUIElementChildren(IGUIElement *element);
    //--------------------------
    
    
private:
	//--------------------------
    //DATAS
	IrrlichtDevice *device;
	array<IEventReceiver *> windows;
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
    
    virtual bool OnEvent(const SEvent& mainEvent) {
        for (unsigned int i=0; i < mEventReceivers.size(); ++i) {
            mEventReceivers[i]->OnEvent(mainEvent);
        }
        return false;
    }

	void reactiveWindow(s32 referenceCounted) {
		SEvent rcEvent;
		rcEvent.EventType = EET_USER_EVENT;
		rcEvent.UserEvent.UserData1 = ECUE_REACTIVE_MINIMIZED_WINDOW;
		rcEvent.UserEvent.UserData2 = referenceCounted;
		for (unsigned int i=0; i < mEventReceivers.size(); ++i) {
            mEventReceivers[i]->OnEvent(rcEvent);
        }
	}
    
    void AddEventReceiver(IEventReceiver * receiver) {
        mEventReceivers.push_back(receiver);
    }
    
    bool RemoveEventReceiver(IEventReceiver * receiver) {
        for (unsigned int i=0; i < mEventReceivers.size(); ++i) {
            if (mEventReceivers[i] == receiver) {
                mEventReceivers.erase(i);
            }
        }
        return false;
    }
    
private:
    array<IEventReceiver *> mEventReceivers;
};
//--------------------------

#endif
