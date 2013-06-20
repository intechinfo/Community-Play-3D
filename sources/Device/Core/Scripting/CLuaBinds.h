//
//  CLuaBinds.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 31/01/13.
//
//

#ifndef __C_LUA_BINDS_H_INCLUDED__
#define __C_LUA_BINDS_H_INCLUDED__

class CDevices;

#include <irrlicht.h>
#include "../Lua52/include/lua.hpp"

#include "CSceneScripting.h"
#include "CFileSystemScripting.h"

//---------------------------------------------------------------------------------------------
//-----------------------------------CSCRIPTING------------------------------------------------
//---------------------------------------------------------------------------------------------

class CScripting {

public:

	CScripting(CDevices *_devices);
	~CScripting();

	void initializeSceneScripting();
	void initializeFileSystemScripting();

	void runScript(irr::core::stringc script, irr::core::stringc name);
	void runScript_t() { runScript(scriptFile, scriptName); }

	void setScriptFile(irr::core::stringc _scriptFile) { scriptFile = _scriptFile; }
	void setScriptName(irr::core::stringc _scriptName) { scriptName = _scriptName; }

private:

	//-----------------------------------
	//DATAS
	CDevices *devices;
	lua_State *L;

	irr::core::stringc scriptFile;
	irr::core::stringc scriptName;
	//-----------------------------------

};

#endif
