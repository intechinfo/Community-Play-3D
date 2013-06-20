//
//  CLuaBinds.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 31/01/13.
//
//
#include "stdafx.h"

#include "CLuaBinds.h"
#include "../../CDevices.h"

//---------------------------------------------------------------------------------------------
//------------------------------------CORE METHODS---------------------------------------------
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
//-----------------------------------SCENE METHODS---------------------------------------------
//---------------------------------------------------------------------------------------------
CSceneScripting *sceneScripting = 0;

int scene_gc(lua_State *L);
int scene_index(lua_State *L);
int scene_newindex(lua_State *L);

int scene_new(lua_State *L);
int scene_destroy(lua_State *L);

int scene_load_mesh(lua_State *L);
int scene_add_terrain_scene_node(lua_State *L);
int scene_add_oct_tree_scene_node(lua_State *L);
int scene_add_tree_scene_node(lua_State *L);
int scene_add_object_scene_node(lua_State *L);
int scene_add_light_scene_node(lua_State *L);
int scene_add_volume_light_scene_node(lua_State *L);

int scene_add_cube_scene_node(lua_State *L);
int scene_add_sphere_scene_node(lua_State *L);
int scene_add_plane_scene_node(lua_State *L);
int scene_add_billboard_scene_node(lua_State *L);


//SET UP CLASSES
static const luaL_Reg scene_meta[] = {
	{"__gc", scene_gc},
	{"__index", scene_index},
    {"__newindex", scene_newindex},
	{ NULL, NULL }
};
static const luaL_Reg scene_methods[] = {
	{"new", scene_new},

	{"loadMesh", scene_load_mesh},
	{"addTerrainMeshSceneNode", scene_add_terrain_scene_node},
	{"addTerrainOctTreeSceneNode", scene_add_oct_tree_scene_node},
	{"addTreeMeshSceneNode", scene_add_tree_scene_node},
	{"addObjectMeshSceneNode", scene_add_object_scene_node},
	{"addLightSceneNode", scene_add_light_scene_node},
	{"addVolumeLightSceneNode", scene_add_volume_light_scene_node},

	{"addCubeSceneNode", scene_add_cube_scene_node},
	{"addSphereSceneNode", scene_add_sphere_scene_node},
	{"addPlaneSceneNode", scene_add_plane_scene_node},
	{"addBillBoardSceneNode", scene_add_billboard_scene_node},
	{ NULL, NULL }
};

//---------------------------------------------------------------------------------------------
//--------------------------------------IO METHODS---------------------------------------------
//---------------------------------------------------------------------------------------------
CFileSystemScripting *fileSystemScripting = 0;

int io_gc(lua_State *L);
int io_index(lua_State *L);
int io_newindex(lua_State *L);

int io_new(lua_State *L);
int io_destroy(lua_State *L);

int io_get_file_list(lua_State *L);
int io_get_file_count(lua_State *L);

//SET UP CLASSES
static const luaL_Reg io_meta[] = {
	{"__gc", io_gc},
	{"__index", io_index},
    {"__newindex", io_newindex},
	{ NULL, NULL }
};
static const luaL_Reg io_methods[] = {
	{"new", io_new},
	{"getFileList", io_get_file_list},
	{"getFileCount", io_get_file_count},
	{ NULL, NULL }
};

struct userData {
    int x;
};

//---------------------------------------------------------------------------------------------
//-----------------------------------CSCRIPTING CLASS------------------------------------------
//---------------------------------------------------------------------------------------------

CScripting::CScripting(CDevices *_devices) {
	devices = _devices;
	scriptFile = "";
	scriptName = "";

	 //LUA PART
    L = luaL_newstate();
    //LOAD LUA AND LUA LIBS
    static const luaL_Reg lualibs[] = {
        {"base", luaopen_base},
        {"io", luaopen_io},
        {"math", luaopen_math},
        {NULL, NULL}
    };

    const luaL_Reg *lib = lualibs;
    for(; lib->func != NULL; lib++) {
        luaL_requiref(L, lib->name, lib->func, 1);
        lua_settop(L, 0);
    }

	//LOAD SCENE
	int lib_id, meta_id;
    lua_createtable(L, 0, 0);
    lib_id = lua_gettop(L);
    luaL_newmetatable(L, "Scene");
    meta_id = lua_gettop(L);
    luaL_setfuncs(L, scene_meta, 0);
    luaL_newlib(L, scene_methods);
    lua_setfield(L, meta_id, "__index");    
    luaL_newlib(L, scene_meta);
    lua_setfield(L, meta_id, "__metatable");
    lua_setmetatable(L, lib_id);
    lua_setglobal(L, "Scene");

	//LOAD IO
    lua_createtable(L, 0, 0);
    lib_id = lua_gettop(L);
    luaL_newmetatable(L, "FileSystem");
    meta_id = lua_gettop(L);
    luaL_setfuncs(L, io_meta, 0);
    luaL_newlib(L, io_methods);
    lua_setfield(L, meta_id, "__index");    
    luaL_newlib(L, io_meta);
    lua_setfield(L, meta_id, "__metatable");
    lua_setmetatable(L, lib_id);
    lua_setglobal(L, "FileSystem");
}

CScripting::~CScripting() {

}

void CScripting::initializeSceneScripting() {
	sceneScripting = new CSceneScripting(devices);
}
void CScripting::initializeFileSystemScripting() {
	fileSystemScripting = new CFileSystemScripting(devices);
}

void CScripting::runScript(irr::core::stringc script, irr::core::stringc name) {
	CProcess *process = new CProcess(devices->getGUIEnvironment(), name.c_str());
	devices->getProcessesLogger()->addProcess(process);
	luaL_dostring(L, script.c_str());
	process->setHasFinished(true);
}

//---------------------------------------------------------------------------------------------
//-----------------------------------IMPLEMENT SCENE METHODS-----------------------------------
//---------------------------------------------------------------------------------------------

int scene_gc(lua_State *L) {
	printf("## __scene__gc\n");
	return 0;
}
int scene_index(lua_State *L) {
	printf("## __index\n");
    return 0;
}
int scene_newindex(lua_State *L) {
	printf("## __newindex\n");
    return 0;
}

int scene_new(lua_State *L) {
	printf("## __scene__new\n");

	lua_newuserdata(L,sizeof(userData));
    luaL_getmetatable(L, "Scene");
    lua_setmetatable(L, -2); 

    return 1;
}
int scene_destroy(lua_State *L) {
	printf("## __scene__destroy\n");

	lua_newuserdata(L,sizeof(userData));
    luaL_getmetatable(L, "nil");
    lua_setmetatable(L, -2); 

    return 1;
}

int scene_load_mesh(lua_State *L) {
	printf("## loadMesh\n");
	int argc = lua_gettop(L);

	irr::core::stringc path = lua_tostring(L, 2);
	sceneScripting->loadMesh(path.c_str());

	const char *s = path.c_str();
	lua_pushstring(L, s);

	return 1;
}

int scene_add_terrain_scene_node(lua_State *L) {
	printf("## addTerrainMeshSceneNode\n");
	int argc = lua_gettop(L);

	irr::core::stringc path = lua_tostring(L, 2);
	sceneScripting->addTerrainMeshSceneNode(path.c_str());

	return 0;
}
int scene_add_oct_tree_scene_node(lua_State *L) {
	printf("## addTerrainOctTreeSceneNode\n");
	int argc = lua_gettop(L);

	irr::core::stringc path = lua_tostring(L, 2);
	irr::u32 minPolys = lua_tointeger(L, 3);
	sceneScripting->addTerrainOctTreeSceneNode(path.c_str(), minPolys);

	return 0;
}
int scene_add_tree_scene_node(lua_State *L) {
	printf("## addTreeMeshSceneNode\n");
	int argc = lua_gettop(L);

	irr::core::stringc path = lua_tostring(L, 2);
	irr::u32 minPolys = lua_tointeger(L, 3);
	sceneScripting->addTreeMeshSceneNode(path.c_str(), minPolys);

	return 0;
}
int scene_add_object_scene_node(lua_State *L) {
	printf("## addObjectMeshSceneNode\n");
	int argc = lua_gettop(L);

	irr::core::stringc path = lua_tostring(L, 2);
	sceneScripting->addObjectMeshSceneNode(path.c_str());

	return 0;
}
int scene_add_light_scene_node(lua_State *L) {
	printf("## addLightSceneNode\n");
	int argc = lua_gettop(L);

	sceneScripting->addLightSceneNode();

	return 0;
}
int scene_add_volume_light_scene_node(lua_State *L) {
	printf("## addVolumeLightSceneNode\n");
	int argc = lua_gettop(L);

	sceneScripting->addVolumeLightSceneNode();

	return 0;
}

int scene_add_cube_scene_node(lua_State *L) {
	printf("## addCubeSceneNode\n");
	int argc = lua_gettop(L);
	sceneScripting->addCubeSceneNode();
	return 0;
}
int scene_add_sphere_scene_node(lua_State *L) {
	printf("## addSphereSceneNode\n");
	int argc = lua_gettop(L);
	sceneScripting->addSphereSceneNode();
	return 0;
}
int scene_add_plane_scene_node(lua_State *L) {
	printf("## addPlaneSceneNode\n");
	int argc = lua_gettop(L);
	sceneScripting->addHillPlaneMesh();
	return 0;
}
int scene_add_billboard_scene_node(lua_State *L) {
	printf("## addBillBoardSceneNode\n");
	int argc = lua_gettop(L);
	sceneScripting->addBillBoardSceneNode();
	return 0;
}

//---------------------------------------------------------------------------------------------
//--------------------------------------IO METHODS---------------------------------------------
//---------------------------------------------------------------------------------------------
int io_gc(lua_State *L) {
	printf("## __filesystem__gc\n");
	return 0;
}
int io_index(lua_State *L) {
	printf("## __index\n");
    return 0;
}
int io_newindex(lua_State *L) {
	printf("## __newindex\n");
    return 0;
}

int io_new(lua_State *L) {
	printf("## __io__new\n");

	lua_newuserdata(L,sizeof(userData));
    luaL_getmetatable(L, "FileSystem");
    lua_setmetatable(L, -2); 

    return 1;
}
int io_destroy(lua_State *L) {
	printf("## __io__destroy\n");

	lua_newuserdata(L,sizeof(userData));
    luaL_getmetatable(L, "nil");
    lua_setmetatable(L, -2); 

    return 1;
}

int io_get_file_list(lua_State *L) {
	printf("## getFileList\n");
	int argc = lua_gettop(L);
	stringc path = "";
	bool resetDirectory=true;
	if (argc >= 2) {
		path = lua_tostring(L, 2);
	}
	if (argc >= 3) {
		resetDirectory = lua_toboolean(L, 3);
	}
	IFileList *fl = fileSystemScripting->getFileList(path.c_str(), resetDirectory);

	lua_createtable(L, fl->getFileCount(), 0);
    int newTable = lua_gettop(L);
    int index = 1;
	while(index != fl->getFileCount()) {
		lua_pushstring(L, fl->getFullFileName(index).c_str());
        lua_rawseti(L, newTable, index);
        ++index;
    }
    return 1;
}
int io_get_file_count(lua_State *L) {
	printf("## getFileCount\n");
	int argc = lua_gettop(L);
	stringc path = "";
	bool resetDirectory=true;
	if (argc >= 2) {
		path = lua_tostring(L, 2);
	}
	if (argc >= 3) {
		resetDirectory = lua_toboolean(L, 3);
	}

	IFileList *fl = fileSystemScripting->getFileList(path.c_str(), resetDirectory);

	lua_pushinteger(L, fl->getFileCount());

	return 1;
}
