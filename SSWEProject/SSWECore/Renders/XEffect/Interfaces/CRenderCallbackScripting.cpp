//
//  CRenderCallbackScripting.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 03/02/13.
//
//
#include "stdafx.h"
#include "CRenderCallbackScripting.h"

irr::f32 currentTime;

irr::video::IVideoDriver *driverScripting;
irr::scene::ISceneManager *smgrScripting;

irr::f32 dimension_f[2];
irr::f32 vector2d_f[2];
irr::f32 vector3d_f[3];
irr::core::matrix4 worldViewProj;

irr::core::stringc returnedValueType;
irr::f32 returnedValue;
irr::f32 returnedValueCount;

CRenderCallbackScripting::CRenderCallbackScripting(irr::IrrlichtDevice *_Device) {
    device = _Device;
    
    //LUA PART
    luaState = luaL_newstate();
    //LOAD LUA AND LUA LIBS
    static const luaL_Reg lualibs[] = {
        {"base", luaopen_base},
        {"io", luaopen_io},
        {"math", luaopen_math},
        {NULL, NULL}
    };
    
    const luaL_Reg *lib = lualibs;
    for(; lib->func != NULL; lib++) {
        luaL_requiref(luaState, lib->name, lib->func, 1);
        lua_settop(luaState, 0);
    }
    
    //REGISTER FUNCTIONS
    //RETURN VALUE
    lua_pushcfunction(luaState, returned_value);
    lua_setglobal(luaState, "returnValue");
    
    //SET RETURNED VALUE TYPE
    lua_pushcfunction(luaState, setReturnedValueAsInteger);
    lua_setglobal(luaState, "setReturnedValueAsFloat");
    
    lua_pushcfunction(luaState, setReturnedValueAsDimension2D);
    lua_setglobal(luaState, "setReturnedValueAsDimension2D");
    
    lua_pushcfunction(luaState, setReturnedValueAsVector2D);
    lua_setglobal(luaState, "setReturnedValueAsVector2D");
    
    lua_pushcfunction(luaState, setReturnedValueAsVector3D);
    lua_setglobal(luaState, "setReturnedValueAsVector3D");
    
    lua_pushcfunction(luaState, setReturnedValueAsMatrix4);
    lua_setglobal(luaState, "setReturnedValueAsMatrix4");
    
    //SOME FUNCTIONS
    lua_pushcfunction(luaState, current_time);
    lua_setglobal(luaState, "current_time");
    
    //MATRIX4 FUNCTIONS
    lua_pushcfunction(luaState, getTransformEtsProjection);
    lua_setglobal(luaState, "getTransformEtsProjection");
    
    lua_pushcfunction(luaState, getTransformEtsView);
    lua_setglobal(luaState, "getTransformEtsView");
    
    lua_pushcfunction(luaState, getTransformEtsWorld);
    lua_setglobal(luaState, "getTransformEtsWorld");
    
    //PIXELS FUNCTIONS
    lua_pushcfunction(luaState, getColorPixel);
    lua_setglobal(luaState, "getColorPixel");
    
    lua_pushcfunction(luaState, getCurrentRenderTargetSizeX);
    lua_setglobal(luaState, "getCurrentRenderTargetSizeX");
    
    lua_pushcfunction(luaState, getCurrentRenderTargetSizeY);
    lua_setglobal(luaState, "getCurrentRenderTargetSizeY");
    
    //TESTING PART ------------------------------------------------------------------------------------------
    /*int status = luaL_loadfile(luaState, "/test.lua");
    int result = 0;
    if(status == LUA_OK) {
        result = lua_pcall(luaState, 0, LUA_MULTRET, 0);
    } else {
        exit(EXIT_FAILURE);
    }
    luaL_dostring(luaState, "io.write(\"J'exécute la commande qui est de type const char* :-) \\n\")");*/
    //TESTING PART ------------------------------------------------------------------------------------------
    driverScripting = device->getVideoDriver();
    smgrScripting = device->getSceneManager();
}

CRenderCallbackScripting::~CRenderCallbackScripting() {
    device = NULL;
}

void CRenderCallbackScripting::update() {
    currentTime = device->getTimer()->getRealTime();
    worldViewProj = 0;
    returnedValue = 0;
    returnedValueCount = 0;
    returnedValueType = "";
}

void CRenderCallbackScripting::runStringScript(irr::core::stringc script) {
    luaL_dostring(luaState, script.c_str());
    lua_pcall(luaState, 0, LUA_MULTRET, 0);
}

int setReturnedValueAsInteger(lua_State *L) {
    returnedValueType = "integer";
    return 0;
}

int setReturnedValueAsMatrix4(lua_State *L) {
    returnedValueType = "mat4";
    return 0;
}

int setReturnedValueAsDimension2D(lua_State *L) {
    returnedValueType = "sampler2D";
    return 0;
}

int setReturnedValueAsVector2D(lua_State *L) {
    returnedValueType = "vec2";
    return 0;
}

int setReturnedValueAsVector3D(lua_State *L) {
    returnedValueType = "vec3";
    return 0;
}

int returned_value(lua_State *L) {
    int argc = lua_gettop(L);
    
    //FIRST ARGUMENT : VALUE
    irr::core::stringc returnedValueString = lua_tostring(L, 1);
    irr::f32 value_f32;
    irr::core::stringc value_f32_c = returnedValueString.c_str();
    value_f32_c.replace(',', '.');
    std::istringstream issX(value_f32_c.c_str());
    issX >> value_f32;
    
    //SECOND ARGUMENT : COUNT
    irr::core::stringc returnedValueCountString = lua_tostring(L, 2);
    irr::f32 value_count_f32;
    irr::core::stringc value_count_f32_c = returnedValueCountString.c_str();
    value_count_f32_c.replace(',', '.');
    std::istringstream issXCount(value_count_f32_c.c_str());
    issXCount >> value_count_f32;
    
    returnedValue = value_f32;
    returnedValueCount = value_count_f32;
    
    return 0;
}

int current_time(lua_State *L) {
    
    lua_pushnumber(L, currentTime);
    
    return 1;
}

int getCurrentRenderTargetSizeX(lua_State *L) {
    
    lua_pushnumber(L, driverScripting->getCurrentRenderTargetSize().Width);
    
    return 1;
}

int getCurrentRenderTargetSizeY(lua_State *L) {
    
    lua_pushnumber(L, driverScripting->getCurrentRenderTargetSize().Height);
    
    return 1;
}

int getColorPixel(lua_State *L) {
    
    lua_pushnumber(L, 1);
    
    return 1;
}

int getGreenColorPixel(lua_State *L) {
    
    return 1;
}

int getBlueColorPixel(lua_State *L) {
    
    return 1;
}

int getTransformEtsWorld(lua_State *L) {
    worldViewProj *= driverScripting->getTransform(irr::video::ETS_VIEW);
    return 0;
}

int getTransformEtsView(lua_State *L) {
    worldViewProj *= driverScripting->getTransform(irr::video::ETS_WORLD);
    return 0;
}

int getTransformEtsProjection(lua_State *L) {
    worldViewProj *= driverScripting->getTransform(irr::video::ETS_PROJECTION);
    return 0;
}
