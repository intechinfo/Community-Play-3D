//
//  CVector3d.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 23/04/2014.
//
//

#include "CVector3d.h"

using namespace irr;
using namespace core;

//---------------------------------------------------------------------------------------------
//-----------------------------------------UTILS-----------------------------------------------
//---------------------------------------------------------------------------------------------

vector3df *checkVector3df(lua_State *L, int n) {
    return *(vector3df **)luaL_checkudata(L, n, "luaL_Vector3df");
}

//---------------------------------------------------------------------------------------------
//------------------------------------CTOR & DTOR----------------------------------------------
//---------------------------------------------------------------------------------------------

int vector3dfConstructor(lua_State *L) {
    vector3df ** udata = (vector3df **)lua_newuserdata(L, sizeof(vector3df *));
    *udata = new vector3df(0);
    
    luaL_getmetatable(L, "luaL_Vector3df");
    lua_setmetatable(L, -2);
    
    return 1;
}

int vector3dfDestructor(lua_State * L) {
    vector3df *v = checkVector3df(L, 1);
    delete v;
    
    return 0;
}

//---------------------------------------------------------------------------------------------
//---------------------------------------SETTERS-----------------------------------------------
//---------------------------------------------------------------------------------------------

int vector3dfSetX(lua_State *L) {
    vector3df *v = checkVector3df(L, 1);
    f32 x = luaL_checknumber(L, 2);
    v->X = x;
    return 1;
}

int vector3dfSetY(lua_State *L) {
    vector3df *v = checkVector3df(L, 1);
    f32 y = luaL_checknumber(L, 2);
    v->Y = y;
    return 1;
}
int vector3dfSetZ(lua_State *L) {
    vector3df *v = checkVector3df(L, 1);
    f32 z = luaL_checknumber(L, 2);
    v->Z = z;
    return 1;
}
int vector3dfSetVector(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
    vector3df *v2 = checkVector3df(L, 2);
    v1->set(*v2);
    return 1;
}
int vector3dfsetXYZ(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
    f32 x = luaL_checknumber(L, 2);
    f32 y = luaL_checknumber(L, 3);
    f32 z = luaL_checknumber(L, 4);
    v1->set(x, y, z);
    return 1;
}

//---------------------------------------------------------------------------------------------
//---------------------------------------GETTERS-----------------------------------------------
//---------------------------------------------------------------------------------------------
int vector3dfGetX(lua_State *L) {
    vector3df *v = checkVector3df(L, 1);
    lua_pushnumber(L, v->X);
    return 1;
}
int vector3dfGetY(lua_State *L) {
    vector3df *v = checkVector3df(L, 1);
    lua_pushnumber(L, v->Y);
    return 1;
}
int vector3dfGetZ(lua_State *L) {
    vector3df *v = checkVector3df(L, 1);
    lua_pushnumber(L, v->Z);
    return 1;
}

//---------------------------------------------------------------------------------------------
//------------------------------------OPERATORS------------------------------------------------
//---------------------------------------------------------------------------------------------

int vector3dfAdd(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
    vector3df *v2 = checkVector3df(L, 2);
    v1->operator+=(*v2);
    lua_pop(L, -2);
    return 1;
}
int vector3dfAddScalar(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
    f32 s = luaL_checknumber(L, 2);
    v1->operator+=(s);
    lua_pop(L, -2);
    return 1;
}

int vector3dfMultiply(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
    vector3df *v2 = checkVector3df(L, 2);
    v1->operator*=(*v2);
    lua_pop(L, -2);
    return 1;
}
int vector3dfMultiplyScalar(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
    f32 s = luaL_checknumber(L, 2);
    v1->operator*=(s);
    lua_pop(L, -2);
    return 1;
}

int vector3dfSub(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
    vector3df *v2 = checkVector3df(L, 2);
    v1->operator-=(*v2);
    lua_pop(L, -2);
    return 1;
}

int vector3dfDivide(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
    vector3df *v2 = checkVector3df(L, 2);
    v1->operator/=(*v2);
    lua_pop(L, -2);
    return 1;
}
int vector3dfDivideScalar(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
    f32 s = luaL_checknumber(L, 2);
    v1->operator/=(s);
    lua_pop(L, -2);
    return 1;
}

//---------------------------------------------------------------------------------------------
//-----------------------------------------METHODS---------------------------------------------
//---------------------------------------------------------------------------------------------

int vector3dfDot(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
    vector3df *v2 = checkVector3df(L, 2);
    f32 dot = v1->dotProduct(*v2);
    lua_pushnumber(L, dot);
    return 1;
}

//---------------------------------------------------------------------------------------------
//-------------------------------------------BIND----------------------------------------------
//---------------------------------------------------------------------------------------------
int vector3dfPrint(lua_State *L) {
    vector3df *v = checkVector3df(L, 1);
    
    printf("X=%f -- Y=%f -- Z=%f\n", v->X, v->Y, v->Z);
    
    return 1;
}

void bindVector3df(lua_State *L) {
    
    luaL_Reg sVector3dfMeta[] =
    {
        { "__gc", vector3dfDestructor },
        {"__index", NULL},
        {"__newindex", NULL},
        { NULL, NULL }
    };
    
    static const luaL_Reg sVector3dfMethods[] = {
        {"new", vector3dfConstructor},
        
        /// Operators
        {"add", vector3dfAdd},
        {"addScalar", vector3dfAddScalar},
        
        {"multiply", vector3dfMultiply},
        {"multiplyScalar", vector3dfMultiplyScalar},
        
        {"sub", vector3dfSub},
        
        {"divide", vector3dfDivide},
        {"divideScalar", vector3dfDivideScalar},
        
        /// Setters
        {"setX", vector3dfSetX},
        {"setY", vector3dfSetY},
        {"setZ", vector3dfSetZ},
        
        {"setVector", vector3dfSetVector},
        {"setXYZ" , vector3dfsetXYZ},
        
        /// Methods
        {"dot", vector3dfDot},
        
        /// Getters
        {"x", vector3dfGetX},
        {"y", vector3dfGetY},
        {"z", vector3dfGetZ},
        
        /// Infos
        {"printVector",vector3dfPrint},
        { NULL, NULL }
    };
    
    int lib_id, meta_id;
    lua_createtable(L, 0, 0);
    lib_id = lua_gettop(L);
    luaL_newmetatable(L, "luaL_Vector3df");
    meta_id = lua_gettop(L);
    luaL_setfuncs(L, sVector3dfMeta, 0);
    luaL_newlib(L, sVector3dfMethods);
    lua_setfield(L, meta_id, "__index");
    luaL_newlib(L, sVector3dfMeta);
    lua_setfield(L, meta_id, "__metatable");
    lua_setmetatable(L, lib_id);
    lua_setglobal(L, "Vector3df");
}