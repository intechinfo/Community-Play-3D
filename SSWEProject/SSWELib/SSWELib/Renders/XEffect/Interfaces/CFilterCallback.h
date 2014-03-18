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

	CFilterCallback(irr::s32 materialTypeIn, SFilter *_fs);

	void OnPreRender(EffectHandler* effect);
    
	void OnPostRender(EffectHandler* effect);

	s32 materialType;

private:

	SFilter *fs;

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
