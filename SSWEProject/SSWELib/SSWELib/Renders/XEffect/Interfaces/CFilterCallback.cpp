//
//  CShaderCallback.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 26/02/13.
//
//
#include "stdafx.h"
#include "CFilterCallback.h"

#include "../../../Device/CDevices.h"

/// Maths
#include "../../../Device/Core/Scripting/math/CVector3d.h"
#include "../../../Device/Core/Scripting/math/CMatrix4.h"

/// Scene
//#include "../../../Device/Core/Scripting/Scene/CCamera.h"

//---------------------------------------------------------------------------------------------
//--------------------------------------POST PROCESS METHODS-----------------------------------
//---------------------------------------------------------------------------------------------

EffectHandler *geffect;
irr::s32 gmaterialType;
irr::core::stringc gWorkingPath;

CFilterCallback::CFilterCallback(irr::s32 materialTypeIn, lua_State *L, irr::core::stringw callback) : materialType(materialTypeIn) {
	this->L = L;
	this->callback = callback;
}

void CFilterCallback::OnPreRender(ISSWERender* effect) {
	geffect = (EffectHandler*)effect;
	gmaterialType = materialType;
	luaL_dostring(this->L, stringc(this->callback).c_str());
}
    
void CFilterCallback::OnPostRender(ISSWERender* effect) {
	((EffectHandler*)effect)->clearPostProcessEffectConstants(materialType);
}

void CFilterCallback::updateCallback(stringw newCallback) {
	callback = newCallback;
}

struct userData {
    int x;
};

//---------------------------------------------------------------------------------------------
//--------------------------------------CLASS--------------------------------------------------
//---------------------------------------------------------------------------------------------

CCoreFilterCallback::CCoreFilterCallback(CDevices *devices) {
	this->devices = devices;
	gWorkingPath = devices->getWorkingDirectory();
}

CCoreFilterCallback::~CCoreFilterCallback() {
    
}

//---------------------------------------------------------------------------------------------
//--------------------------------------LUA CORE + CORE----------------------------------------
//---------------------------------------------------------------------------------------------
//LUA CORE
static const luaL_Reg lualibs[] = {
    {"base", luaopen_base},
    {"math", luaopen_math},
    {NULL, NULL}
};

//CORE
int core_gc(lua_State *L);
int core_index(lua_State *L);
int core_newindex(lua_State *L);

int core_new(lua_State *L);

int core_setPixelShaderConstantInteger(lua_State *L);
int core_setPixelShaderConstantFloat(lua_State *L);
int core_setPixelShaderConstantVector2D(lua_State *L);
int core_setPixelShaderConstantVector3D(lua_State *L);
int core_setPixelShaderConstantMatrix4(lua_State *L);

int core_setTextureAtIndex(lua_State *L);
int core_setRttTextureAtIndex(lua_State *L);

static const luaL_Reg core_meta[] = {
	{"__gc", core_gc},
	{"__index", core_index},
	{"__newindex", core_newindex},
	{ NULL, NULL }
};

static const luaL_Reg core_methods[] = {
	{"new", core_new},
	{"setPixelShaderConstantInteger", core_setPixelShaderConstantInteger},
	{"setPixelShaderConstantFloat", core_setPixelShaderConstantFloat},
	{"setPixelShaderConstantVector2D", core_setPixelShaderConstantVector2D},
	{"setPixelShaderConstantVector3D", core_setPixelShaderConstantVector3D},
	{"setPixelShaderConstantMatrix4", core_setPixelShaderConstantMatrix4},

	{"setTextureAtIndex", core_setTextureAtIndex},
	{"setRttTextureAtIndex", core_setRttTextureAtIndex},
	{ NULL, NULL }
};

//CORE UTILS
int utils_gc(lua_State *L);
int utils_index(lua_State *L);
int utils_newindex(lua_State *L);

int utils_new(lua_State *L);

int utils_getCurrentTime(lua_State *L);
int utils_getScreenCoordinatesFrom3DPosition(lua_State *L);

static const luaL_Reg utils_meta[] = {
	{"__gc", utils_gc},
	{"__index", utils_index},
	{"__newindex", utils_newindex},
	{ NULL, NULL }
};

static const luaL_Reg utils_methods[] = {
	{"new", utils_new},
	{"getCurrentTime", utils_getCurrentTime},

	{"getScreenCoordinatesFrom3DPosition", utils_getScreenCoordinatesFrom3DPosition},
	{ NULL, NULL }
};

//CORE DRIVER
int driver_gc(lua_State *L);
int driver_index(lua_State *L);
int driver_newindex(lua_State *L);

int driver_new(lua_State *L);

int driver_setTexture(lua_State *L);
int driver_setRttTexture(lua_State *L);

int driver_getLightPosition(lua_State *L);

static const luaL_Reg driver_meta[] = {
	{"__gc", driver_gc},
	{"__index", driver_index},
	{"__newindex", driver_newindex},
	{ NULL, NULL }
};

static const luaL_Reg driver_methods[] = {
	{"new", driver_new},
	{"setTexture", driver_setTexture},
	{"setRttTexture", driver_setRttTexture},

	{"getLightPosition", driver_getLightPosition},
	{ NULL, NULL }
};

//---------------------------------------------------------------------------------------------
//--------------------------------------MATRIXES-----------------------------------------------
//---------------------------------------------------------------------------------------------
//MATRIX CLASSES
int mat4_gc(lua_State *L);
int mat4_index(lua_State *L);
int mat4_newindex(lua_State *L);

int mat4_new(lua_State *L);

int mat4_multiplyMatrixes(lua_State *L);
int mat4_getTransform(lua_State *L);

static const luaL_Reg mat4_meta[] = {
	{"__gc", mat4_gc},
	{"__index", mat4_index},
	{"__newindex", mat4_newindex},
	{ NULL, NULL }
};

static const luaL_Reg mat4_methods[] = {
	{"new", mat4_new},
	{"multiplyMatrixes", mat4_multiplyMatrixes},
	{"getTransform", mat4_getTransform},
	{ NULL, NULL }
};

//---------------------------------------------------------------------------------------------
//--------------------------------------REST OF CLASS------------------------------------------
//---------------------------------------------------------------------------------------------

void CCoreFilterCallback::createLuaState(lua_State *L) {
	//LUA CORE
	const luaL_Reg *lib = lualibs;
	for(; lib->func != NULL; lib++) {
		luaL_requiref(L, lib->name, lib->func, 1);
		lua_settop(L, 0);
	}

	int lib_id, meta_id;

	//CORE
	lua_createtable(L, 0, 0);
    lib_id = lua_gettop(L);
    luaL_newmetatable(L, "Core");
    meta_id = lua_gettop(L);
    luaL_setfuncs(L, core_meta, 0);
    luaL_newlib(L, core_methods);
    lua_setfield(L, meta_id, "__index");    
    luaL_newlib(L, core_meta);
    lua_setfield(L, meta_id, "__metatable");
    lua_setmetatable(L, lib_id);
    lua_setglobal(L, "Core");

	//MATRIXES
	lua_createtable(L, 0, 0);
    lib_id = lua_gettop(L);
    luaL_newmetatable(L, "Matrix4");
    meta_id = lua_gettop(L);
    luaL_setfuncs(L, mat4_meta, 0);
    luaL_newlib(L, mat4_methods);
    lua_setfield(L, meta_id, "__index");    
    luaL_newlib(L, mat4_meta);
    lua_setfield(L, meta_id, "__metatable");
    lua_setmetatable(L, lib_id);
    lua_setglobal(L, "Matrix4");

	//UTILS
	lua_createtable(L, 0, 0);
    lib_id = lua_gettop(L);
    luaL_newmetatable(L, "Utils");
    meta_id = lua_gettop(L);
    luaL_setfuncs(L, utils_meta, 0);
    luaL_newlib(L, utils_methods);
    lua_setfield(L, meta_id, "__index");    
    luaL_newlib(L, utils_meta);
    lua_setfield(L, meta_id, "__metatable");
    lua_setmetatable(L, lib_id);
    lua_setglobal(L, "Utils");

	//DRIVER
	lua_createtable(L, 0, 0);
    lib_id = lua_gettop(L);
    luaL_newmetatable(L, "Driver");
    meta_id = lua_gettop(L);
    luaL_setfuncs(L, driver_meta, 0);
    luaL_newlib(L, driver_methods);
    lua_setfield(L, meta_id, "__index");    
    luaL_newlib(L, driver_meta);
    lua_setfield(L, meta_id, "__metatable");
    lua_setmetatable(L, lib_id);
    lua_setglobal(L, "Driver");

	//OTHERS
	//cp3d::scripting::bindVector3df(L, false);
	//cp3d::scripting::bindMatrix4(L, devices->getVideoDriver(), false);
	//cp3d::scripting::bindCamera(L, devices->getSceneManager());

	luaL_dostring(L, "filter = Core.new()\n"
					 "utils = Utils.new()\n"
					 "driver = Driver.new()\n");
}

//---------------------------------------------------------------------------------------------
//--------------------------------------LUA METHODS--------------------------------------------
//---------------------------------------------------------------------------------------------

//CORE
int core_gc(lua_State *L) {
	//printf("## __core__gc\n");
	return 1;
}
int core_index(lua_State *L) {
	//printf("## __index\n");
    return 1;
}
int core_newindex(lua_State *L) {
	//printf("## __newindex\n");
    return 1;
}

int core_new(lua_State *L) {
	//printf("## __core__new\n");

	lua_newuserdata(L,sizeof(userData));
    luaL_getmetatable(L, "Core");
    lua_setmetatable(L, -2);

    return 1;
}

int core_setPixelShaderConstantInteger(lua_State *L) {
	int argc = lua_gettop(L);
	if (argc < 3)
		return 0;

	stringc name = lua_tostring(L, 2);
	s32 *value = new s32(lua_tointeger(L, 3));

	geffect->setPostProcessingEffectConstant(gmaterialType, name.c_str(), reinterpret_cast<f32*>(&value), 1);

	return 1;
}

int core_setPixelShaderConstantFloat(lua_State *L) {
	int argc = lua_gettop(L);
	if (argc < 3)
		return 0;

	stringc name = lua_tostring(L, 2);
	luaL_checktype(L, 3, LUA_TNUMBER);

	f32 *value = new f32(lua_tonumber(L, 3));

	geffect->setPostProcessingEffectConstant(gmaterialType, name.c_str(), value, 1);

	return 1;
}

int core_setPixelShaderConstantVector2D(lua_State *L) {
	int argc = lua_gettop(L);
	if (argc < 3)
		return 0;

	stringc name = lua_tostring(L, 2);

	luaL_checktype(L, 3, LUA_TTABLE);
	lua_getfield(L, 3, "x");
	lua_getfield(L, 3, "y");
	f32 x = luaL_checknumber(L, -2);
	f32 y = luaL_checknumber(L, -1);

	lua_pop(L, 1);

	f32 *const v = new f32[2];
	v[0] = x;
	v[1] = y;

	geffect->setPostProcessingEffectConstant(gmaterialType, name.c_str(), v, 2);

	return 1;
}

int core_setPixelShaderConstantVector3D(lua_State *L) {
	int argc = lua_gettop(L);
	if (argc < 3)
		return 0;

	VECTOR3DF_CHECK_ARGUMENTS(3, L);
	stringc name = lua_tostring(L, 2);
	vector3df *v = cp3d::scripting::checkVector3df(L, 3);
	VECTOR3DF_CHECK_VECTOR(v);

	geffect->setPostProcessingEffectConstant(gmaterialType, name.c_str(), (f32*)v, 3);

	/*luaL_checktype(L, 3, LUA_TTABLE);
	lua_getfield(L, 3, "x");
	lua_getfield(L, 3, "y");
	lua_getfield(L, 3, "z");
	f32 x = luaL_checknumber(L, -3);
	f32 y = luaL_checknumber(L, -2);
	f32 z = luaL_checknumber(L, -1);

	lua_pop(L, 1);

	f32 *const v = new f32[3];
	v[0] = x;
	v[1] = y;
	v[2] = z;

	geffect->setPostProcessingEffectConstant(gmaterialType, name.c_str(), v, 3);*/

	return 1;
}

int core_setPixelShaderConstantMatrix4(lua_State *L) {
	int argc = lua_gettop(L);
	MATRIX4_CHECK_ARGUMENTS(3, L);
	stringc name = lua_tostring(L, 2);
	matrix4 *mat = cp3d::scripting::checkMatrix4(L, 3);
	MATRIX4_CHECK_MATRIX(mat);

	geffect->setPostProcessingEffectConstant(gmaterialType, name.c_str(), mat->pointer(), 16);

	return 1;
}

int core_setTextureAtIndex(lua_State *L) {
	int argc = lua_gettop(L);
	if (argc < 3)
		return 0;

	s32 index = lua_tointeger(L, 2);
	stringc path = gWorkingPath + lua_tostring(L, 3);

	if (index >= 0) {
		geffect->getScreenQuadPtr()->getMaterial().setTexture(index, geffect->getIrrlichtDevice()->getVideoDriver()->getTexture(path.c_str()));
	}

	return 1;
}

int core_setRttTextureAtIndex(lua_State *L) {
	int argc = lua_gettop(L);
	if (argc < 3)
		return 0;

	s32 index = lua_tointeger(L, 2);
	stringc path = lua_tostring(L, 3);

	if (index >= 0) {
		geffect->getScreenQuadPtr()->getMaterial().setTexture(index, geffect->getIrrlichtDevice()->getVideoDriver()->getTexture(path.c_str()));
	}

	return 1;
}

//MATRIXES

int mat4_gc(lua_State *L) {
	//printf("## __mat4__gc\n");
	return 1;
}
int mat4_index(lua_State *L) {
	//printf("## __index\n");
    return 1;
}
int mat4_newindex(lua_State *L) {
	//printf("## __newindex\n");
    return 1;
}

int mat4_new(lua_State *L) {
	//printf("## __mat4__new\n");

	lua_newuserdata(L,sizeof(userData));
    luaL_getmetatable(L, "Matrix4");
    lua_setmetatable(L, -2); 

	//matrix4 *mat = new matrix4();
	//lua_pushlightuserdata(L, mat);

    return 1;
}

int mat4_multiplyMatrixes(lua_State *L) {


	return 1;
}

int mat4_getTransform(lua_State *L) {
	return 1;
}

//UTILS
int utils_gc(lua_State *L) {
	//printf("## __utils__gc\n");
	return 1;
}
int utils_index(lua_State *L) {
	//printf("## __index\n");
    return 1;
}
int utils_newindex(lua_State *L) {
	//printf("## __newindex\n");
    return 1;
}

int utils_new(lua_State *L) {
	//printf("## __utils__new\n");

	lua_newuserdata(L,sizeof(userData));
    luaL_getmetatable(L, "Utils");
    lua_setmetatable(L, -2); 

    return 1;
}

int utils_getCurrentTime(lua_State *L) {

	u32 currentTime = geffect->getIrrlichtDevice()->getTimer()->getRealTime();
	lua_pushinteger(L, currentTime);

	return 1;
}

int utils_getScreenCoordinatesFrom3DPosition(lua_State *L) {
	int argc = lua_gettop(L);
	if (argc < 2)
		return 0;

	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "x");
	lua_getfield(L, 2, "y");
	lua_getfield(L, 2, "z");
	f32 x = luaL_checknumber(L, -3);
	f32 y = luaL_checknumber(L, -2);
	f32 z = luaL_checknumber(L, -1);

	core::vector2di scrPos = geffect->getIrrlichtDevice()->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(vector3df(x, y, z));
    core::vector2df screen((float)scrPos.X/(float)geffect->getIrrlichtDevice()->getVideoDriver()->getScreenSize().Width,
                           (float)scrPos.Y/(float)geffect->getIrrlichtDevice()->getVideoDriver()->getScreenSize().Height);
	screen.Y = 1-screen.Y;

	lua_newtable(L);
	lua_pushnumber(L, screen.X);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, screen.Y);
	lua_setfield(L, -2, "y");

	return 1;
}

//DRIVER
int driver_gc(lua_State *L) {
	//printf("## __driver__gc\n");
	return 1;
}
int driver_index(lua_State *L) {
	//printf("## __index\n");
    return 1;
}
int driver_newindex(lua_State *L) {
	//printf("## __newindex\n");
    return 1;
}

int driver_new(lua_State *L) {
	//printf("## __driver__new\n");

	lua_newuserdata(L,sizeof(userData));
    luaL_getmetatable(L, "Driver");
    lua_setmetatable(L, -2); 

    return 1;
}

int driver_setTexture(lua_State *L) {
	int argc = lua_gettop(L);
	if (argc < 2)
		return 0;

	stringc path = gWorkingPath + lua_tostring(L, 2);
	geffect->setPostProcessingUserTexture(geffect->getIrrlichtDevice()->getVideoDriver()->getTexture(path));

	return 1;
}

int driver_setRttTexture(lua_State *L) {
	int argc = lua_gettop(L);
	if (argc < 2)
		return 0;

	stringc path = lua_tostring(L, 2);
	geffect->setPostProcessingUserTexture(geffect->getIrrlichtDevice()->getVideoDriver()->getTexture(path));

	return 1;
}

int driver_getLightPosition(lua_State *L) {
	int argc = lua_gettop(L);
	if (argc < 2)
		return 0;

	f32 x = 0;
	f32 y = 0;
	f32 z = 0;

	int index = lua_tointeger(L, 2);
	if (index < geffect->getShadowLightCount() && index >= 0) {
		vector3df v = geffect->getShadowLight(index).getPosition();
		x = v.X;
		y = v.Y;
		z = v.Z;
	}

	lua_newtable(L);
	lua_pushnumber(L, x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, y);
	lua_setfield(L, -2, "y");
	lua_pushnumber(L, z);
	lua_setfield(L, -2, "z");

	return 1;
}

/*

driver:setRttTexture("ColorMapSampler")

position = utils:getScreenCoordinatesFrom3DPosition(driver:getLightPosition(0))

filter:setPixelShaderConstantVector2D("lightPositionOnScreen", position)

*/
