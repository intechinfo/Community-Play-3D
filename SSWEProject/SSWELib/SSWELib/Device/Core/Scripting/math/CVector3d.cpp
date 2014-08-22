//
//  CVector3d.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 23/04/2014.
//
//

#include "stdafx.h"
#include "CVector3d.h"

namespace cp3d {
namespace scripting {

using namespace irr;
using namespace core;

//---------------------------------------------------------------------------------------------
//-----------------------------------------UTILS-----------------------------------------------
//---------------------------------------------------------------------------------------------

vector3df *checkVector3df(lua_State *L, int n, bool error) {
	if (lua_type(L, n) != LUA_TUSERDATA) {
		if (error)
			luaL_error(L, "Argument must be a Vector3df");
		else
			return 0;
	}

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
	VECTOR3DF_CHECK_VECTOR(v);
	VECTOR3DF_CHECK_ARGUMENTS(2, L);
    f32 x = luaL_checknumber(L, 2);
    v->X = x;
	lua_pop(L, -2);
    return 1;
}

int vector3dfSetY(lua_State *L) {
    vector3df *v = checkVector3df(L, 1);
	VECTOR3DF_CHECK_VECTOR(v);
	VECTOR3DF_CHECK_ARGUMENTS(2, L);
    f32 y = luaL_checknumber(L, 2);
    v->Y = y;
	lua_pop(L, -2);
    return 1;
}
int vector3dfSetZ(lua_State *L) {
    vector3df *v = checkVector3df(L, 1);
	VECTOR3DF_CHECK_VECTOR(v);
	VECTOR3DF_CHECK_ARGUMENTS(2, L);
    f32 z = luaL_checknumber(L, 2);
    v->Z = z;
	lua_pop(L, -2);
    return 1;
}
int vector3dfSetVector(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
    vector3df *v2 = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
    v1->set(*v2);
    return 1;
}
int vector3dfsetXYZ(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
	VECTOR3DF_CHECK_VECTOR(v1);
	VECTOR3DF_CHECK_ARGUMENTS(4, L);
    f32 x = luaL_checknumber(L, 2);
    f32 y = luaL_checknumber(L, 3);
    f32 z = luaL_checknumber(L, 4);
    v1->set(x, y, z);
	lua_pop(L, -2);
    return 1;
}

//---------------------------------------------------------------------------------------------
//---------------------------------------GETTERS-----------------------------------------------
//---------------------------------------------------------------------------------------------
int vector3dfGetX(lua_State *L) {
    vector3df *v = checkVector3df(L, 1);
	VECTOR3DF_CHECK_VECTOR(v);
    lua_pushnumber(L, v->X);
    return 1;
}
int vector3dfGetY(lua_State *L) {
    vector3df *v = checkVector3df(L, 1);
	VECTOR3DF_CHECK_VECTOR(v);
    lua_pushnumber(L, v->Y);
    return 1;
}
int vector3dfGetZ(lua_State *L) {
    vector3df *v = checkVector3df(L, 1);
	VECTOR3DF_CHECK_VECTOR(v);
    lua_pushnumber(L, v->Z);
    return 1;
}

//---------------------------------------------------------------------------------------------
//------------------------------------OPERATORS------------------------------------------------
//---------------------------------------------------------------------------------------------

int vector3dfAdd(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
    vector3df *v2 = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
    v1->operator+=(*v2);
    lua_pop(L, -2);
    return 1;
}
int vector3dfAddScalar(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
	VECTOR3DF_CHECK_VECTOR(v1);
	VECTOR3DF_CHECK_ARGUMENTS(2, L);
    f32 s = luaL_checknumber(L, 2);
    v1->operator+=(s);
    lua_pop(L, -2);
    return 1;
}

int vector3dfMultiply(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
    vector3df *v2 = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
    v1->operator*=(*v2);
    lua_pop(L, -2);
    return 1;
}
int vector3dfMultiplyScalar(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
	VECTOR3DF_CHECK_VECTOR(v1);
	VECTOR3DF_CHECK_ARGUMENTS(2, L);
    f32 s = luaL_checknumber(L, 2);
    v1->operator*=(s);
    lua_pop(L, -2);
    return 1;
}

int vector3dfSub(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
    vector3df *v2 = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
    v1->operator-=(*v2);
    lua_pop(L, -2);
    return 1;
}

int vector3dfDivide(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
    vector3df *v2 = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
    v1->operator/=(*v2);
    lua_pop(L, -2);
    return 1;
}
int vector3dfDivideScalar(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
	VECTOR3DF_CHECK_VECTOR(v1);
	VECTOR3DF_CHECK_ARGUMENTS(2, L);
    f32 s = luaL_checknumber(L, 2);
    v1->operator/=(s);
    lua_pop(L, -2);
    return 1;
}

int vector3dfEquals(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
    vector3df *v2 = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
	bool areEquals = v1->operator==(*v2);
	lua_pushboolean(L, areEquals);
	return 1;
}

int vector3dfInferiorTo(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
    vector3df *v2 = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
	bool checkEquals = lua_toboolean(L, 3);
	bool isInferior;
	if (checkEquals)
		isInferior = v1->operator<=(*v2);
	else
		isInferior = v1->operator<(*v2);
	lua_pushboolean(L, isInferior);
	return 1;
}
int vector3dfSuperiorTo(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
    vector3df *v2 = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
	bool checkEquals = lua_toboolean(L, 3);
	bool isSuperior;
	if (checkEquals)
		isSuperior = v1->operator>=(*v2);
	else
		isSuperior = v1->operator>(*v2);
	lua_pushboolean(L, isSuperior);
	return 1;
}

//---------------------------------------------------------------------------------------------
//-----------------------------------------METHODS---------------------------------------------
//---------------------------------------------------------------------------------------------

int vector3dfDot(lua_State *L) {
    vector3df *v1 = checkVector3df(L, 1);
    vector3df *v2 = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
    f32 dot = v1->dotProduct(*v2);
    lua_pushnumber(L, dot);
    return 1;
}
int vector3dfCrossProduct(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
    vector3df *v2 = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
	v1->set(v1->crossProduct(*v2));
    return 1;
}

int vector3dfGetLength(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
	VECTOR3DF_CHECK_VECTOR(v1);
	f32 length = v1->getLength();
	lua_pushnumber(L, length);
	return 1;
}
int vector3dfGetLengthSQ(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
	VECTOR3DF_CHECK_VECTOR(v1);
	f32 lengthsq = v1->getLengthSQ();
	lua_pushnumber(L, lengthsq);
	return 1;
}
int vector3dfSetLength(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
	VECTOR3DF_CHECK_VECTOR(v1);
	f32 length = v1->getLengthSQ();
	v1->setLength(length);
	return 1;
}

int vector3dfNegate(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
	VECTOR3DF_CHECK_VECTOR(v1);
	v1->invert();
    lua_pop(L, -2);
    return 1;
}

int vector3dfNormalize(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
	VECTOR3DF_CHECK_VECTOR(v1);
	v1->normalize();
    lua_pop(L, -2);
    return 1;
}

int vector3dfDistanceTo(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
	vector3df *v2 = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
	f32 distance = v1->getDistanceFrom(*v2);
    lua_pushnumber(L, distance);
    return 1;
}
int vector3dfDistanceToSQ(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
	vector3df *v2 = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
	f32 distance = v1->getDistanceFromSQ(*v2);
    lua_pushnumber(L, distance);
    return 1;
}

int vector3dfRotationToDirection(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
	vector3df *v2 = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
	v1->set(v1->rotationToDirection(*v2));
	lua_pop(L, -2);
    return 1;
}

int vector3dfGetSphericalCoordinateAngles(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
	vector3df *v2 = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
	v2->set(v1->getSphericalCoordinateAngles());
	lua_pop(L, -1);
    return 1;
}
int vector3dfGetHorizontalAngle(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
	vector3df *v2 = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
	v2->set(v1->getHorizontalAngle());
	lua_pop(L, -1);
    return 1;
}

int vector3dfInterpolate(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
	vector3df *v2 = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
	vector3df *v3 = checkVector3df(L, 3);
	VECTOR3DF_CHECK_ARGUMENTS(5, L);
	f32 d = luaL_checknumber(L, 4);
	vector3df *v4 = checkVector3df(L, 5);
	VECTOR3DF_CHECK_VECTORS(v3, v4);
	v4->set(v1->interpolate(*v2, *v3, d));
	lua_pop(L, -2);
    return 1;
}
int vector3dfGetInterpolated_quadratic(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
	vector3df *v2 = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
	vector3df *v3 = checkVector3df(L, 3);
	VECTOR3DF_CHECK_ARGUMENTS(5, L);
	f32 d = luaL_checknumber(L, 4);
	vector3df *v4 = checkVector3df(L, 5);
	VECTOR3DF_CHECK_VECTORS(v3, v4);
	v4->set(v1->getInterpolated_quadratic(*v2, *v3, d));
	lua_pop(L, -2);
    return 1;
}
int vector3dfGetInterpolated(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
	vector3df *v2 = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
	VECTOR3DF_CHECK_ARGUMENTS(4, L);
	f32 d = luaL_checknumber(L, 3);
	vector3df *v3 = checkVector3df(L, 4);
	VECTOR3DF_CHECK_VECTOR(v3);
	v3->set(v1->getInterpolated(*v2, d));
	lua_pop(L, -2);
	return 1;
}

int vector3dfRotateYZBy(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
	VECTOR3DF_CHECK_ARGUMENTS(3, L);
	f32 d = luaL_checknumber(L, 2);
	vector3df *v2 = checkVector3df(L, 3);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
	v1->rotateYZBy(d, *v2);
	lua_pop(L, -2);
	return 1;
}
int vector3dfRotateXYBy(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
	VECTOR3DF_CHECK_ARGUMENTS(3, L);
	f32 d = luaL_checknumber(L, 2);
	vector3df *v2 = checkVector3df(L, 3);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
	v1->rotateXYBy(d, *v2);
	lua_pop(L, -2);
	return 1;
}
int vector3dfRotateXZBy(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
	VECTOR3DF_CHECK_ARGUMENTS(3, L);
	f32 d = luaL_checknumber(L, 2);
	vector3df *v2 = checkVector3df(L, 3);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
	v1->rotateXZBy(d, *v2);
	lua_pop(L, -2);
	return 1;
}

int vector3dfIsBetweenPoints(lua_State *L) {
	vector3df *v1 = checkVector3df(L, 1);
	vector3df *v2 = checkVector3df(L, 2);
	vector3df *v3 = checkVector3df(L, 3);
	VECTOR3DF_CHECK_VECTORS(v1, v2);
	VECTOR3DF_CHECK_VECTOR(v3);
	bool isBetween = v1->isBetweenPoints(*v2, *v3);
	lua_pushboolean(L, isBetween);
	return 1;
}

//---------------------------------------------------------------------------------------------
//-------------------------------------------BIND----------------------------------------------
//---------------------------------------------------------------------------------------------
int vector3dfPrint(lua_State *L) {
    vector3df *v = checkVector3df(L, 1);
	VECTOR3DF_CHECK_VECTOR(v);
    
    printf("X=%f -- Y=%f -- Z=%f\n", v->X, v->Y, v->Z);
    
    return 1;
}

void bindVector3df(lua_State *L, bool allowConstructor) {
    
    luaL_Reg sVector3dfMeta[] =
    {
        { "__gc", allowConstructor ? vector3dfDestructor : NULL },
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

		{"equals", vector3dfEquals},

		{"isInferiorTo", vector3dfInferiorTo},
		{"isSuperiorTo", vector3dfSuperiorTo},
        
        /// Setters
        {"setX", vector3dfSetX},
        {"setY", vector3dfSetY},
        {"setZ", vector3dfSetZ},
        
        {"setVector", vector3dfSetVector},
        {"setXYZ" , vector3dfsetXYZ},
        
        /// Methods
        {"dot", vector3dfDot},
		{"crossProduct", vector3dfCrossProduct},

		{"getLength", vector3dfGetLength},
		{"getLengthSQ", vector3dfGetLengthSQ},
		{"setLength", vector3dfSetLength},

		{"negate", vector3dfNegate},
		{"normalize", vector3dfNormalize},

		{"distanceTo", vector3dfDistanceTo},
		{"distanceFromSQ", vector3dfDistanceToSQ},

		{"rotationToDirection", vector3dfRotationToDirection},
		{"getSphericalCoordinateAngles", vector3dfGetSphericalCoordinateAngles},
		{"getHorizontalAngle", vector3dfGetHorizontalAngle},

		{"interpolate", vector3dfInterpolate},
		{"getInterpolated_quadratic", vector3dfGetInterpolated_quadratic},
		{"getInterpolated", vector3dfGetInterpolated},

		{"rotateYZBy", vector3dfRotateYZBy},
		{"rotateXYBy", vector3dfRotateXYBy},
		{"rotateXZBy", vector3dfRotateXZBy},

		{"isBetweenPoints", vector3dfIsBetweenPoints},
        
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

} // End namespace scripting
} // End namespace cp3d