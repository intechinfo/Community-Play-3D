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

#include "../../../SSWERenders/Renders/XEffect/XEffects.h"

#include "../../../SSWELib/SSWELib/SceneNodes/LensFlareSceneNode.h"
#include "../../../SSWELib/SSWELib/SceneNodes/SceneNodeAnimatorFollowCamera.h"

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
    
	//VIDEO METHODS
	u32 getNumberOfBuildInMaterialTypes();
	array<stringc> getArrayOfBuildInMaterialTypes();
    
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
        }
        return false;
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

	//--------------------------
	//MINIMIZED WINDOWS MANAGER
	void ReactiveWindow(s32 referenceCounted) {
		u32 rf = referenceCounted;
		for (u32 i=0; i < smw.GUIImages.size(); i++) {
			if (smw.GUIImages[i]->getReferenceCount() == referenceCounted) {
				rf = smw.minimizedGUIWindows[i]->getReferenceCount();
				break;
			}
		}
		SEvent rcEvent;
		rcEvent.EventType = EET_USER_EVENT;
		rcEvent.UserEvent.UserData1 = ECUE_REACTIVE_MINIMIZED_WINDOW;
		rcEvent.UserEvent.UserData2 = rf;
		for (u32 i=0; i < smw.minimizedWindows.size(); ++i) {
            smw.minimizedWindows[i]->OnEvent(rcEvent);
        }
	}

	void AddMinimizedWindow(IEventReceiver *rcv, IGUIWindow *window) {
		IImage *scs = driver->createScreenShot();
		IGUIImage *image = gui->addImage(rect<s32>(0, 0, scs->getDimension().Width, scs->getDimension().Height), 0, -1, window->getText());
		ITexture *texture = driver->addTexture(stringc(stringc(window->getText()) + stringc(window->getReferenceCount())).c_str(), scs);

		IGUIStaticText *txt = gui->addStaticText(window->getText(), rect<s32>(0, 0, 0, 0), false, false, 0, -1, false);
		txt->setToolTipText(window->getText());
		txt->setOverrideColor(SColor(255, 255, 255, 255));
		txt->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
		scs->grab();

		image->setText(window->getText());
		image->setToolTipText(window->getText());
		image->setImage(texture);
		image->setScaleImage(true);

		window->setVisible(false);

		smw.minimizedWindows.push_back(rcv);
		smw.minimizedGUIWindows.push_back(window); 
		smw.parent.push_back(window->getParent()); 
		smw.relativePositions.push_back(window->getRelativePosition());
		smw.images.push_back(texture);
		smw.GUIImages.push_back(image);
		smw.names.push_back(txt);
	}

	void RemoveMinimizedWindow(IEventReceiver *rcv) {
		for (u32 i=0; i < smw.minimizedWindows.size(); ++i) {
            if (smw.minimizedWindows[i] == rcv) {
				smw.minimizedGUIWindows[i]->setVisible(true);
				smw.parent[i]->addChild(smw.minimizedGUIWindows[i]);
				smw.minimizedGUIWindows[i]->setRelativePosition(smw.relativePositions[i]);
				
				smw.GUIImages[i]->remove();
				driver->removeTexture(smw.images[i]);
				smw.names[i]->remove();

				smw.minimizedWindows.erase(i);
				smw.minimizedGUIWindows.erase(i);
				smw.parent.erase(i);
				smw.relativePositions.erase(i);
				smw.images.erase(i);
				smw.GUIImages.erase(i);
				smw.names.erase(i);
            }
        }
	}

	u32 GetMinimizedWindowsCount() { return smw.minimizedWindows.size(); }

	IEventReceiver *getErcv(u32 i) { return smw.minimizedWindows[i]; }
	IGUIElement *getWindow(u32 i) { return smw.GUIImages[i]; }
	IGUIElement *getWindowName(u32 i) { return smw.names[i]; }
	IGUIWindow *getGUIWindow(u32 i) { return smw.minimizedGUIWindows[i]; }
	IGUIElement *getParent(u32 i) { return smw.parent[i]; }
	ITexture *getWindowTexture(u32 i) { return smw.images[i]; }

	void setDriver(IVideoDriver *_driver) { driver = _driver; }
	void setGUI(IGUIEnvironment *_gui) { gui = _gui; }
    
private:
    array<IEventReceiver *> mEventReceivers;

	struct SMinimizedWindows {
		array<IEventReceiver *> minimizedWindows;
		array<IGUIWindow *> minimizedGUIWindows;
		array<IGUIElement *> parent;
		array<rect<s32>> relativePositions;
		array<ITexture *> images;
		array<IGUIImage *> GUIImages;
		array<IGUIStaticText *> names;
	} smw;

	IVideoDriver *driver;
	IGUIEnvironment *gui;

};
//--------------------------

#endif
