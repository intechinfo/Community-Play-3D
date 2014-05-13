//
//  CCoreData.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 25/09/12.
//
//

#include "stdafx.h"
#include "CScenarioData.h"

#include "CCoreData.h"

//---------------------------------------------------------------------------------------------
//----------------------------------SCENARIO DATAS STRUCTS-------------------------------------
//---------------------------------------------------------------------------------------------

SScenarioScript::SScenarioScript(stringw _name) {
	name = _name;
	script = new SScriptFile("--Event Script\n"
							 "\n"
							 "function init()\n"
							 "    --Init your script here, variables, etc.\n"
							 "end\n"
							 "\n"
							 "function loop(timeMS)\n"
							 "    --Update here (timeMS in milliseconds)\n"
							 "end\n",
							 name);
}

void SScenarioScript::initScript() {
	destroyLuaState();
	createLuaState();
}

void SScenarioScript::executeScript(u32 time) {
	stringc cmd = "loop(";
	cmd += time;
	cmd += ")";

	luaL_dostring(L, cmd.c_str());
}

void SScenarioScript::createLuaState() {
	L = luaL_newstate();
}

void SScenarioScript::destroyLuaState() {
	lua_close(L);
}

//---------------------------------------------------------------------------------------------
//----------------------------------SCENARIO DATA CLASS----------------------------------------
//---------------------------------------------------------------------------------------------

CScenarioData::CScenarioData() {

}

CScenarioData::~CScenarioData() {

}

//---------------------------------------------------------------------------------------------
//----------------------------------SCENARIO DATA FUNCTIONS------------------------------------
//---------------------------------------------------------------------------------------------


