//
//  CVector3d.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 23/04/2014.
//
//

#include "stdafx.h"
#include "CCamera.h"

#include "../math/CVector3d.h"

namespace cp3d {
namespace scripting {

using namespace irr;
using namespace core;

irr::scene::ISceneManager *gSceneManager;

//---------------------------------------------------------------------------------------------
//------------------------------------CTOR & DTOR----------------------------------------------
//---------------------------------------------------------------------------------------------

int cameraConstructor(lua_State *L) {
    
    luaL_getmetatable(L, "luaL_Camera");
    lua_setmetatable(L, -2);
    
    return 1;
}

//---------------------------------------------------------------------------------------------
//---------------------------------------GETTERS-----------------------------------------------
//---------------------------------------------------------------------------------------------

int cameraGetPosition(lua_State *L) {
	VECTOR3DF_CHECK_ARGUMENTS(2, L);

    vector3df *v = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTOR(v);

	v->set(gSceneManager->getActiveCamera()->getPosition());
    return 1;
}

int cameraGetFarValue(lua_State *L) {
	lua_pushnumber(L, gSceneManager->getActiveCamera()->getFarValue());
    return 1;
}

int cameraGetNearValue(lua_State *L) {
	lua_pushnumber(L, gSceneManager->getActiveCamera()->getNearValue());
    return 1;
}

//---------------------------------------------------------------------------------------------
//-------------------------------------------BIND----------------------------------------------
//---------------------------------------------------------------------------------------------

void bindCamera(lua_State *L, irr::scene::ISceneManager *smgr) {
    
	gSceneManager = smgr;

    luaL_Reg sVector3dfMeta[] =
    {
        { "__gc", NULL },
        {"__index", NULL},
        {"__newindex", NULL},
        { NULL, NULL }
    };
    
    static const luaL_Reg sVector3dfMethods[] = {
        {"new", cameraConstructor},
        
        /// Getters
		{"getPosition", cameraGetPosition},
		{"getFarValue", cameraGetFarValue},
		{"getNearValue", cameraGetNearValue},

        { NULL, NULL }
    };
    
    int lib_id, meta_id;
    lua_createtable(L, 0, 0);
    lib_id = lua_gettop(L);
    luaL_newmetatable(L, "luaL_Camera");
    meta_id = lua_gettop(L);
    luaL_setfuncs(L, sVector3dfMeta, 0);
    luaL_newlib(L, sVector3dfMethods);
    lua_setfield(L, meta_id, "__index");
    luaL_newlib(L, sVector3dfMeta);
    lua_setfield(L, meta_id, "__metatable");
    lua_setmetatable(L, lib_id);
    lua_setglobal(L, "Camera");
}

} // End namespace scripting
} // End namespace cp3d