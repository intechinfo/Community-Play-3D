//
//  CShaderCallback.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 26/02/13.
//
//

#ifndef __C_FILTER_CALLBACK_H_INCLUDED__
#define __C_FILTER_CALLBACK_H_INCLUDED__

#include <irrlicht.h>

class CDevices;

//---------------------------------------------------------------------------------------------
//--------------------------------------POST PROCESS METHODS-----------------------------------
//---------------------------------------------------------------------------------------------

class CFilterCallback : public IPostProcessingRenderCallback {

public:

	CFilterCallback(irr::s32 materialTypeIn, lua_State *L, irr::core::stringw callback);

	void OnPreRender(ISSWERender* effect);
    
	void OnPostRender(ISSWERender* effect);

	s32 materialType;

	irr::core::stringw *getCallbackPtr() { return &callback; }
	irr::core::stringw getCallback() { return callback; }
	void updateCallback(stringw newCallback);

private:

	lua_State *L;
	irr::core::stringw callback;

};

//---------------------------------------------------------------------------------------------
//--------------------------------------CLASS--------------------------------------------------
//---------------------------------------------------------------------------------------------

class CCoreFilterCallback {

public:

	CCoreFilterCallback(CDevices *devices);
	~CCoreFilterCallback();

	void createLuaState(lua_State *L);

private:

	CDevices *devices;

};

#endif
