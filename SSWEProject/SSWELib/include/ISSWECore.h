#ifndef __I_SSWE_CORE_H_INCLUDED__
#define __I_SSWE_CORE_H_INCLUDED__

#include <irrlicht.h>

#include <ITool.h>

#include "src/Bullet-C-Api.h"
#include "src/btBulletDynamicsCommon.h"
#include "src/btBulletCollisionCommon.h"

class IUpdate {
public:
	virtual void update() = 0;
};

class ISSWECore {
    
public: 
    
    virtual void setDevice(irr::IrrlichtDevice *_device) = 0;
    virtual irr::IrrlichtDevice *getDevice() = 0;
    
    //CORE METHODS AND FUNCTIONS
	virtual std::string convertToString(irr::core::stringw _textToConvert) = 0;
    
    virtual irr::core::vector3df getVector3df(std::string X, std::string Y, std::string Z) = 0; //VECTORS
    virtual irr::core::vector3df getVector3df(irr::core::stringw X, irr::core::stringw Y, irr::core::stringw Z) = 0;
	
    virtual irr::core::dimension2d<irr::u32> getDimensionU32(std::string sizeW, std::string sizeH) = 0; //DIMENSIONS
    virtual irr::core::dimension2d<irr::f32> getDimensionF32(std::string sizeW, std::string sizeH) = 0;
	
    virtual irr::f32 getF32(irr::core::stringc valuef32) = 0; //NUMBERS
    virtual irr::u32 getU32(irr::core::stringc valueu32) = 0;
    virtual irr::s32 getS32(irr::core::stringc values32) = 0;
    
	virtual irr::core::stringw getStrNumber(irr::f32 value) = 0; //STRINGS
    virtual irr::core::stringw getStrNumberU32(irr::u32 value) = 0;
	virtual irr::core::stringw getStrVector3df(irr::core::vector3df v) = 0;
    
	virtual irr::core::stringc getStringcFromFile(irr::core::stringc pathFile) = 0; //FILES
	virtual irr::core::stringc getStringcFromIReadFile(irr::core::stringc pathFile) = 0;
	
	virtual irr::core::stringw changeTextWithValue(const wchar_t *text, irr::s32 value) = 0;
    
	//DRIVER METHODS
	virtual irr::core::stringw getTexturePath(irr::video::ITexture *texture) = 0;
    virtual irr::s32 textureAlreadyExists(irr::core::stringc name, irr::video::IVideoDriver *driver) = 0;
    
	//SCENE METHODS
	virtual irr::core::stringc getNodeNamePrefix(irr::scene::ISceneNode *node) = 0;
    virtual irr::scene::ISceneNode *clone(irr::scene::ISceneNode *node, irr::core::stringc meshPath, irr::scene::ISceneManager *smgr) = 0;
    #ifndef _IRR_OSX_PLATFORM_
    virtual irr::s32 nodeExistsInArray(irr::core::array<irr::scene::ISceneNode *> *nodes, irr::scene::ISceneNode *node) = 0;
    #else
    virtual irr::s32 nodeExistsInArray(irr::core::array<irr::scene::ISceneNode *>& nodes, irr::scene::ISceneNode *node) = 0;
    #endif
    virtual irr::core::array<irr::scene::ISceneNode *> *getArrayOfAListOfNodeChildren(irr::scene::ISceneNode *node) = 0;
    
	//GUI METHODS
	virtual bool elementIsInArrayOfElements(irr::gui::IGUIElement *element, irr::core::array<irr::gui::IGUIElement *> elements) = 0;
    virtual irr::core::array<irr::gui::IGUIElement *> getArrayOfAListOfGUIElementChildren(irr::gui::IGUIElement *element) = 0;
	virtual void deactiveChildrenOfGUIElement(irr::gui::IGUIElement *element, bool visible) = 0;
	virtual void fillArrayOfGUIElementsFromArrayOfGUIElements(irr::core::array<irr::gui::IGUIElement *> *toFill, irr::core::array<irr::gui::IGUIElement *> source) = 0;
	virtual void maximizeWindow(irr::gui::IGUIWindow *window, irr::core::rect<irr::s32> minRelativePosition) = 0;
	virtual void centerWindow(irr::gui::IGUIWindow *window, irr::core::dimension2du screenSize) = 0;
    virtual irr::core::rect<irr::s32> getScreenCenterRectFromRect(irr::core::rect<irr::s32> elementRect) = 0;
    
	//VIDEO METHODS
    virtual irr::u32 getNumberOfBuildInMaterialTypes() = 0;
    virtual irr::core::array<irr::core::stringc> getArrayOfBuildInMaterialTypes() = 0;
    virtual irr::video::ITexture *copyTexture(irr::core::stringc nameOfTexture, irr::video::ITexture *texture, irr::video::IVideoDriver *driver) = 0;
    
	//BULLET METHODS
	virtual btVector3 getBtVector3(irr::core::vector3df vector) = 0;
    
};

class ISSWEEventsManager : public irr::IEventReceiver, public IUpdate {
    
public:
    
    virtual void update() = 0;
    
    virtual bool OnEvent(const irr::SEvent& mainEvent) = 0;
    
    virtual void AddEventReceiver(irr::IEventReceiver * receiver, irr::gui::IGUIWindow *window=0, IUpdate *autoUpdate=0) = 0;
    virtual bool RemoveEventReceiver(irr::IEventReceiver * receiver) = 0;
    
};

#endif