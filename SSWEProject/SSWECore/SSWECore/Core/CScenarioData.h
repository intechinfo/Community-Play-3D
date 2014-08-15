//
//  CCoreData.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 25/09/12.
//
//

#ifndef __C_SCENARIO_DATA_H_INCLUDED__
#define __C_SCENARIO_DATA_H_INCLUDED__

#include "stdafx.h"
#include "CCore.h"

//LUA
#include "../Lua52/include/lua.hpp"

//INTERFACES AND PLUGINS
#include <IMonitor.h>
#include <ISSWECoreData.h>

//---------------------------------------------------------------------------------------------
//----------------------------------SCENARIO DATAS STRUCTS-------------------------------------
//---------------------------------------------------------------------------------------------

namespace cp3d {
	namespace scenario {
		enum E_SCENARIO_EVENT_TYPE {
			ESET_EXECUTE_SCRIPT = 0,
			ESET_PLAY_SOUND,
			ESET_EXECUTE_ANIMATOR,

			ESET_COUNT
		};
	}
}

//SCRIPTS
struct SScriptFile;
struct SScenarioScript {
public:
	SScenarioScript(stringw _name);

	stringw getName() { return name; }
	void setName(stringw _name) { name = _name; }

	SScriptFile *getScriptFile() { return script; }

	void createLuaState();
	void executeScript(u32 time);
	void destroyLuaState();
	void initScript();
	lua_State *getLuaState() { return L; }

private:
	stringw name;
	SScriptFile *script;
	lua_State *L;
};

//SOUNDS
struct SScenarioSound {

};

//EVENTS
struct SScenarioEvent {
public:
	SScenarioEvent(stringw _name, cp3d::scenario::E_SCENARIO_EVENT_TYPE _type, void *_data = 0) {
		name = _name;
		eventType = _type;
		data = _data;
	}

	stringw getName() { return name; }
	void setName(stringw _name) { name = _name; }

	void *getData() { return data; }
	void setData(void *_data) { data = _data; }

	cp3d::scenario::E_SCENARIO_EVENT_TYPE getType() { return eventType; }

private:
	stringw name;
	cp3d::scenario::E_SCENARIO_EVENT_TYPE eventType;
	void *data;
};

//---------------------------------------------------------------------------------------------
//----------------------------------SCENARIO DATA CLASS----------------------------------------
//---------------------------------------------------------------------------------------------

class SSWE_CORE_API CScenarioData {

public:

	CScenarioData();
	~CScenarioData();

	//-----------------------------------
	//METHODS

	//-----------------------------------

private:

	//-----------------------------------
	//DATAS
	array<ISceneNodeAnimator *> internalAnimators;
	array<SScenarioSound> sounds;
	array<SScenarioEvent> events;
	//-----------------------------------

};

#endif
