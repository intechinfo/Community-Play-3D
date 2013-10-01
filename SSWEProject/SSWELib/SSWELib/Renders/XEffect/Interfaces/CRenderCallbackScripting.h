//
//  CRenderCallbackScripting.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 03/02/13.
//
//

#ifndef __C_RENDER_CALLBACK_SCRIPTING_H_INCLUDED__
#define __C_RENDER_CALLBACK_SCRIPTING_H_INCLUDED__

#include <irrlicht.h>

#include <iostream>
#include <sstream>

#ifdef _IRR_OSX_PLATFORM_
	#include <lua.hpp>
#elif defined(IS_ERIO_AND_RELOU)
    #include "../../../Lua/lua.hpp"
#else
	#include "../Lua52/include/lua.hpp"
#endif

class CRenderCallbackScripting {

public:

	CRenderCallbackScripting(irr::IrrlichtDevice *_Device);
	~CRenderCallbackScripting();

	void update();
	void runStringScript(irr::core::stringc script);

	static float value;

private:

	//IRRLICHT DEVICE
	irr::IrrlichtDevice *device;

	//LUA SCRIPTING
	lua_State *luaState;

};

typedef int (*lua_CFunction) (lua_State *L);

int setReturnedValueAsInteger(lua_State *L);
int setReturnedValueAsDimension2D(lua_State *L);
int setReturnedValueAsVector2D(lua_State *L);
int setReturnedValueAsVector3D(lua_State *L);
int setReturnedValueAsMatrix4(lua_State *L);

//CURRENT TIME (OS OR DEVICE TIMER ?)
int current_time(lua_State *L);

//WORLD VIEW PROJ VALUE
int getTransformEtsWorld(lua_State *L);
int getTransformEtsView(lua_State *L);
int getTransformEtsProjection(lua_State *L);

//SCREEN
int getCurrentRenderTargetSizeX(lua_State *L);
int getCurrentRenderTargetSizeY(lua_State *L);
int getColorPixel(lua_State *L);

//FINAL RETURNED VALUE BY SCRIPT
int returned_value(lua_State *L);
extern irr::core::stringc returnedValueType;

//EXTERN VALUES
extern irr::f32 currentTime;

extern irr::video::IVideoDriver *driverScripting;
extern irr::scene::ISceneManager *smgrScripting;

extern irr::f32 dimension_f[2];
extern irr::f32 vector2d_f[2];
extern irr::f32 vector3d_f[3];
extern irr::core::matrix4 worldViewProj;

extern irr::f32 returnedValue;
extern irr::f32 returnedValueCount;

#endif
