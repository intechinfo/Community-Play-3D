//
//  CVector3d.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 23/04/2014.
//
//

#include "stdafx.h"
#include "CMatrix4.h"

#include "CVector3d.h"

namespace cp3d {
namespace scripting {

using namespace irr;
using namespace core;

//---------------------------------------------------------------------------------------------
//-----------------------------------------UTILS-----------------------------------------------
//---------------------------------------------------------------------------------------------

irr::video::IVideoDriver *_driver;

matrix4 *checkMatrix4(lua_State *L, int n) {
	if (lua_type(L, n) != LUA_TUSERDATA) return 0;

    return *(matrix4 **)luaL_checkudata(L, n, "luaL_Matrix4");
}

//---------------------------------------------------------------------------------------------
//------------------------------------CTOR & DTOR----------------------------------------------
//---------------------------------------------------------------------------------------------

int matrix4Constructor(lua_State *L) {
    matrix4 ** udata = (matrix4 **)lua_newuserdata(L, sizeof(matrix4 *));
    *udata = new matrix4();
    
    luaL_getmetatable(L, "luaL_Matrix4");
    lua_setmetatable(L, -2);
    
    return 1;
}

int matrix4Destructor(lua_State * L) {
    matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);
    delete mat;
    
    return 1;
}

//---------------------------------------------------------------------------------------------
//---------------------------------------SETTERS-----------------------------------------------
//---------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------
//---------------------------------------GETTERS-----------------------------------------------
//---------------------------------------------------------------------------------------------

int matrix4Pointer(lua_State *L) {
	matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);

	lua_newtable(L);
	for(u32 i = 0; i < 16; i++) {
		lua_pushnumber(L, i);
		f32 value = mat->operator[](i);
		lua_pushnumber(L, value);
		lua_settable(L, -3);
	}

	return 1;
}

//---------------------------------------------------------------------------------------------
//------------------------------------OPERATORS------------------------------------------------
//---------------------------------------------------------------------------------------------
int matrix4Set(lua_State *L) {
	matrix4 *mat1 = checkMatrix4(L, 1);
	matrix4 *mat2 = checkMatrix4(L, 2);
	MATRIX4_CHECK_MATRIXES(mat1, mat2);

	mat1->operator=(*mat2);

	lua_pop(L, -2);

	return 1;
}

int matrix4Equals(lua_State *L) {
	matrix4 *mat1 = checkMatrix4(L, 1);
	matrix4 *mat2 = checkMatrix4(L, 2);
	MATRIX4_CHECK_MATRIXES(mat1, mat2);

	lua_pushboolean(L, mat1->operator==(*mat2));

	return 1;
}

int matrix4Add(lua_State *L) {
	matrix4 *mat1 = checkMatrix4(L, 1);
	matrix4 *mat2 = checkMatrix4(L, 2);

	MATRIX4_CHECK_MATRIXES(mat1, mat2);

	mat1->operator+=(*mat2);

	lua_pop(L, -2);

	return 1;
}

int matrix4Minus(lua_State *L) {
	matrix4 *mat1 = checkMatrix4(L, 1);
	matrix4 *mat2 = checkMatrix4(L, 2);
	MATRIX4_CHECK_MATRIXES(mat1, mat2);

	mat1->operator-=(*mat2);

	lua_pop(L, -2);

	return 1;
}

int matrix4Multiply(lua_State *L) {
	matrix4 *mat1 = checkMatrix4(L, 1);
	matrix4 *mat2 = checkMatrix4(L, 2);
	MATRIX4_CHECK_MATRIXES(mat1, mat2);

	mat1->operator*=(*mat2);

	lua_pop(L, -2);

	return 1;
}

//---------------------------------------------------------------------------------------------
//-----------------------------------------METHODS---------------------------------------------
//---------------------------------------------------------------------------------------------
int matrix4MakeIdentity(lua_State *L) {
	matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);

	mat->makeIdentity();

	lua_pop(L, -2);

	return 1;
}

int matrix4IsIdentity(lua_State *L) {
	matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);

	lua_pushboolean(L, mat->isIdentity());

	return 1;
}

int matrix4IsOrthogonal(lua_State *L) {
	matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);

	lua_pushboolean(L, mat->isOrthogonal());

	return 1;
}

int matrix4IsIdentity_integer_base(lua_State *L) {
	matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);

	lua_pushboolean(L, mat->isIdentity_integer_base());

	return 1;
}

int matrix4SetTranslation(lua_State *L) {
	matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);

	vector3df *v = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTOR(v);

	mat->setTranslation(*v);

	lua_pop(L, -2);

	return 1;
}

int matrix4GetTranslation(lua_State *L) {
	matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);

	vector3df *v = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTOR(v);

	v->set(mat->getTranslation());

	return 1;
}

int matrix4SetRotationRadians(lua_State *L) {
	matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);

	vector3df *v = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTOR(v);

	mat->setRotationRadians(*v);

	lua_pop(L, -2);

	return 1;
}

int matrix4SetRotationDegrees(lua_State *L) {
	matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);

	vector3df *v = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTOR(v);

	mat->setRotationDegrees(*v);

	lua_pop(L, -2);

	return 1;
}

int matrix4GetRotationDegrees(lua_State *L) {
	matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);

	vector3df *v = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTOR(v);

	v->set(mat->getRotationDegrees());

	return 1;
}

int matrix4SetInverseRotationRadians(lua_State *L) {
	matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);

	vector3df *v = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTOR(v);

	mat->setInverseRotationRadians(*v);

	lua_pop(L, -2);

	return 1;
}


int matrix4SetInverseRotationDegrees(lua_State *L) {
	matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);

	vector3df *v = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTOR(v);

	mat->setInverseRotationDegrees(*v);

	lua_pop(L, -2);

	return 1;
}

int matrix4SetRotationAxisRadians(lua_State *L) {
	MATRIX4_CHECK_ARGUMENTS(3, L);

	matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);

	f32 angle = lua_tonumber(L, 2);

	vector3df *v = checkVector3df(L, 3);
	VECTOR3DF_CHECK_VECTOR(v);

	mat->setRotationAxisRadians(angle, *v);

	lua_pop(L, -2);

	return 1;
}

int matrix4SetScale(lua_State *L) {
	MATRIX4_CHECK_ARGUMENTS(2, L);
	matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);

	vector3df *v = checkVector3df(L, 2, false);
	if (v) {
		mat->setScale(*v);
	} else {
		f32 value = lua_tonumber(L, 2);
		mat->setScale(value);
	}

	lua_pop(L, -2);

	return 1;
}

int matrix4GetScale(lua_State *L) {
	MATRIX4_CHECK_ARGUMENTS(2, L);
	matrix4 *mat = checkMatrix4(L, 1);

	vector3df *v = checkVector3df(L, 2);
	VECTOR3DF_CHECK_VECTOR(v);
	v->set(mat->getScale());

	return 1;
}

int matrix4SetWorld(lua_State *L) {
	matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);

	mat->operator=(_driver->getTransform(irr::video::ETS_WORLD));

	return 1;
}

int matrix4SetView(lua_State *L) {
	matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);

	mat->operator=(_driver->getTransform(irr::video::ETS_VIEW));

	return 1;
}

int matrix4SetProj(lua_State *L) {
	matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);

	mat->operator=(_driver->getTransform(irr::video::ETS_PROJECTION));

	return 1;
}

int matrix4SetWorldViewProj(lua_State *L) {
	matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);

	mat->makeIdentity();
	mat->operator*=(_driver->getTransform(irr::video::ETS_PROJECTION));
	mat->operator*=(_driver->getTransform(irr::video::ETS_VIEW));
	mat->operator*=(_driver->getTransform(irr::video::ETS_WORLD));

	return 1;
}

//---------------------------------------------------------------------------------------------
//-------------------------------------------BIND----------------------------------------------
//---------------------------------------------------------------------------------------------

int printMatrix4(lua_State *L) {
	matrix4 *mat = checkMatrix4(L, 1);
	MATRIX4_CHECK_MATRIX(mat);

	for (u32 i=0; i < 16; i++) {
		printf("%f, ", mat->operator[](i));
	}
	printf("\n");

	return 1;
}

void bindMatrix4(lua_State *L, irr::video::IVideoDriver *driver, bool allowConstructor) {

	_driver = driver;
    
    luaL_Reg sVector3dfMeta[] =
    {
		//{ "__gc", matrix4Destructor },
        {"__index", NULL},
        {"__newindex", NULL},
        { NULL, NULL }
    };

    static const luaL_Reg sVector3dfMethods[] = {
		{"new", matrix4Constructor},
        
        /// Operators
		{"set", matrix4Set},
		{"equals", matrix4Equals},
		{"add", matrix4Add},
		{"minus", matrix4Minus},
		{"multiply", matrix4Multiply},
        
        /// Setters
		{"setTranslation", matrix4SetTranslation},
		{"getTranslation", matrix4GetTranslation},

		{"setRotationRadians", matrix4SetRotationRadians},
		{"setRotationDegrees", matrix4SetRotationDegrees},
		{"getRotationDegrees", matrix4GetRotationDegrees},

		{"setInverseRotationRadians", matrix4SetInverseRotationRadians},
		{"setInverseRotationDegrees", matrix4SetInverseRotationDegrees},
		{"setRotationAxisRadians", matrix4SetRotationAxisRadians},

		{"setScale", matrix4SetScale},
		{"getScale", matrix4GetScale},
        
        /// Methods
		{"makeIdentity", matrix4MakeIdentity},
		{"isIdentity", matrix4IsIdentity},

		{"isOrthogonal", matrix4IsOrthogonal},
		{"isIdentity_integer_base", matrix4IsIdentity_integer_base},

		{"setWorldViewProj", matrix4SetWorldViewProj},
		{"setWorld", matrix4SetWorld},
		{"setView", matrix4SetView},
		{"setProj", matrix4SetProj},
        
        /// Getters
		{"pointer", matrix4Pointer},
        
        /// Infos
        {"printMatrix",printMatrix4},
        { NULL, NULL }
    };
    
    int lib_id, meta_id;
    lua_createtable(L, 0, 0);
    lib_id = lua_gettop(L);
    luaL_newmetatable(L, "luaL_Matrix4");
    meta_id = lua_gettop(L);
    luaL_setfuncs(L, sVector3dfMeta, 0);
    luaL_newlib(L, sVector3dfMethods);
    lua_setfield(L, meta_id, "__index");
    luaL_newlib(L, sVector3dfMeta);
    lua_setfield(L, meta_id, "__metatable");
    lua_setmetatable(L, lib_id);
    lua_setglobal(L, "Matrix4");
}

} // End namespace scripting
} // End namespace cp3d