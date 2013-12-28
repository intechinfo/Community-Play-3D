//
//  CUIEditEffects.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 20/11/12.
//
//
#include "stdafx.h"
#include "CUIWindowEditFilters.h"

//---------------------------------------------------------------------------------------------
//--------------------------------------POST PROCESS METHODS-----------------------------------
//---------------------------------------------------------------------------------------------

EffectHandler *geffect;
irr::s32 gmaterialType;

class CFilterCallback : public IPostProcessingRenderCallback {

public:

	CFilterCallback(irr::s32 materialTypeIn, SFilter *_fs) : materialType(materialTypeIn) {
		fs = _fs;
    }

	void OnPreRender(EffectHandler* effect) {
		geffect = effect;
		gmaterialType = materialType;
		luaL_dostring(fs->getLuaState(), stringc(fs->getCallback()).c_str());
	}
    
	void OnPostRender(EffectHandler* effect) {
		effect->clearPostProcessEffectConstants(materialType);
	}

	s32 materialType;

private:

	SFilter *fs;

};

struct userData {
    int x;
};

//---------------------------------------------------------------------------------------------
//--------------------------------------CLASS--------------------------------------------------
//---------------------------------------------------------------------------------------------

CUIWindowEditFilters::CUIWindowEditFilters(CDevices *_devices) {
    devices = _devices;

	IGUIEnvironment *gui = devices->getGUIEnvironment();
	//GUI ELEMENTS
	//RENDERERS
	window = gui->addWindow(rect<s32>(460, 260, 1360, 680), false, L"Shaders Edition Window", 0, -1);
	window->getMaximizeButton()->setVisible(true);
	window->getMinimizeButton()->setVisible(true);
	devices->getCore()->centerWindow(window, devices->getVideoDriver()->getScreenSize());

	viewPort = new CGUIViewport(devices->getGUIEnvironment(), window, 1, rect<s32>(10, 60, 460, 350));
    if (viewPort) {
		viewPort->setScreenQuad(devices->getXEffect()->getScreenQuad());
		viewPort->setSceneManager(devices->getXEffect()->getActiveSceneManager());
		viewPort->setRenderScreenQuad(true);
        viewPort->setOverrideColor(SColor(255, 0, 0, 0)); 
    }

	useVSM = gui->addCheckBox(devices->getXEffect()->isUsingVSMShadows(), rect<s32>(10, 360, 460, 380), window, -1, L"Use VSM Shadows");
	enableDepthPass = gui->addCheckBox(devices->getXEffect()->isDepthPassEnabled(), rect<s32>(10, 380, 460, 400), window, -1, L"Enable Depth Pass");

	//FILTERS
	filters = gui->addListBox(rect<s32>(490, 80, 890, 350), window, -1, true);
	gui->addStaticText(L"Filters :", rect<s32>(490, 60, 650, 80), false, true, window, -1, false);
	editCallback = gui->addButton(rect<s32>(650, 60, 750, 80), window, -1, L"Callback", L"Edit selected filter's callback");
	editFilter = gui->addButton(rect<s32>(750, 60, 850, 80), window, -1, L"Edit", L"Edit selected filter");
	addFilter = gui->addButton(rect<s32>(870, 60, 890, 80), window, -1, L"+", L"Add a filter");
	removeFilter = gui->addButton(rect<s32>(850, 60, 870, 80), window, -1, L"-", L"Remove selected filter");

	close = gui->addButton(rect<s32>(790, 380, 890, 410), window, -1, L"Close", L"Close this window");

	//FILL FILTERS LIST
	CCoreData *cdata = devices->getCoreData();
	for (u32 i=0; i < cdata->getEffectFilters()->size(); i++) {
		filters->addItem(stringw(cdata->getEffectFilters()->operator[](i).getName()).c_str());
	}
	if (filters->getItemCount() > 0) {
		filters->setSelected(0);
	} else {
		filters->setSelected(-1);
	}

	//MENU
	menu = gui->addMenu(window);
	menu->addItem(L"File", -1, true, true);
	menu->addItem(L"Edit", -1, true, true);

	IGUIContextMenu *submenu = menu->getSubMenu(0);
	submenu->addItem(L"Load From File...", 1, true);
	submenu->addItem(L"Load From Package...", 2, true);
	submenu->addItem(L"Close", 3, true);

	submenu = menu->getSubMenu(1);
	submenu->addItem(L"Reload", 1, true);

	//OTHERS
	devices->getEventReceiver()->AddEventReceiver(this, window);
}

CUIWindowEditFilters::~CUIWindowEditFilters() {
    
}

bool CUIWindowEditFilters::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window) {
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
		}

		//MENU
		if (event.GUIEvent.EventType == EGET_MENU_ITEM_SELECTED) {
			if (event.GUIEvent.Caller == menu->getSubMenu(0)) {
				if (menu->getSubMenu(0)->getSelectedItem() == 0) {
					openShader = devices->createFileOpenDialog(L"Load From File...", CGUIFileSelector::EFST_OPEN_DIALOG,
						devices->getGUIEnvironment()->getRootGUIElement(), false);
				}
				if (menu->getSubMenu(0)->getSelectedItem() == 1) {
					//LOAD FROM PACKAGE
				}
				if (menu->getSubMenu(0)->getSelectedItem() == 2) {
					devices->getEventReceiver()->RemoveEventReceiver(this);
					window->remove();
					delete this;
				}
			}
		}

		//FILE SELECTION
		if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
			if (event.GUIEvent.Caller == openShader) {
				SFilter f;
				f.createLuaState();
				f.setPixelShader(devices->getCore()->getStringcFromFile(openShader->getFileName()));
				f.setCallback("filter:setPixelShaderConstantFloat(\"multiplier\", 1)");
				f.setMaterial(devices->getXEffect()->addPostProcessingEffectFromString(f.getPixelShader().c_str()));
				devices->getCoreData()->getEffectFilters()->push_back(f);
				u32 count = devices->getCoreData()->getEffectFilters()->size();
				this->createLuaState(devices->getCoreData()->getEffectFilters()->operator[](count-1).getLuaState());
				CFilterCallback *cb = new CFilterCallback(devices->getCoreData()->getEffectFilters()->operator[](count-1).getMaterial(),
														  &devices->getCoreData()->getEffectFilters()->operator[](count-1));
				devices->getXEffect()->setPostProcessingRenderCallback(devices->getCoreData()->getEffectFilters()->operator[](count-1).getMaterial(), cb);

				filters->addItem(stringw(f.getName()).c_str());
				filters->setSelected(filters->getItemCount()-1);
			}
		}

		//CHECKBOXES
		if (event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED) {
			if (event.GUIEvent.Caller == useVSM) {
				devices->getXEffect()->setUseVSMShadows(useVSM->isChecked());
			}
			if (event.GUIEvent.Caller == enableDepthPass) {
				devices->getXEffect()->enableDepthPass(enableDepthPass->isChecked());
			}
		}

		//BUTTONS
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == close) {
				devices->getEventReceiver()->RemoveEventReceiver(this);
				window->remove();
				delete this;
			}

			if (event.GUIEvent.Caller == addFilter) {
				SFilter f;
				f.createLuaState();
				devices->getCoreData()->getEffectFilters()->push_back(f);
				filters->addItem(stringw(f.getName()).c_str());
				filters->setSelected(filters->getItemCount()-1);
			}
			if (event.GUIEvent.Caller == removeFilter) {
				if (filters->getSelected() != -1) {
					s32 selected = filters->getSelected();
					filters->removeItem(selected);
					devices->getXEffect()->removePostProcessingEffect(devices->getCoreData()->getEffectFilters()->operator[](selected).getMaterial());
					devices->getCoreData()->getEffectFilters()->operator[](selected).destroyLuaState();
					devices->getCoreData()->getEffectFilters()->erase(selected);
				}
			}
			if (event.GUIEvent.Caller == editFilter) {
				s32 selected = filters->getSelected();
				if (selected != -1) {
					CUICodeEditor *codeEditor = new CUICodeEditor(devices, devices->getCoreData()->getEffectFilters()->operator[](selected).getPixelShaderPtr(), true);
					codeEditor->setAutoSave(true);
				}
			}
			if (event.GUIEvent.Caller == editCallback) {
				s32 selected = filters->getSelected();
				if (selected != -1) {
					CUICodeEditor *codeEditor = new CUICodeEditor(devices, devices->getCoreData()->getEffectFilters()->operator[](selected).getCallbackPtr(), true);
					codeEditor->setAutoSave(true);
				}
			}
		}
	}

	return false;
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

void CUIWindowEditFilters::createLuaState(lua_State *L) {
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

	luaL_dostring(L, "filter = Core.new()\n");
}

//---------------------------------------------------------------------------------------------
//--------------------------------------LUA METHODS--------------------------------------------
//---------------------------------------------------------------------------------------------

//CORE
int core_gc(lua_State *L) {
	//printf("## __core__gc\n");
	return 0;
}
int core_index(lua_State *L) {
	//printf("## __index\n");
    return 0;
}
int core_newindex(lua_State *L) {
	//printf("## __newindex\n");
    return 0;
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

	geffect->setPostProcessingEffectConstant(gmaterialType, name.c_str(), (f32*)&value, 1);

	return 0;
}

int core_setPixelShaderConstantFloat(lua_State *L) {
	int argc = lua_gettop(L);
	if (argc < 3)
		return 0;

	stringc name = lua_tostring(L, 2);
	f32 *value = new f32(lua_tonumber(L, 3));

	geffect->setPostProcessingEffectConstant(gmaterialType, name.c_str(), value, 1);

	return 0;
}

int core_setPixelShaderConstantVector2D(lua_State *L) {
	return 0;
}

int core_setPixelShaderConstantVector3D(lua_State *L) {
	return 0;
}

int core_setPixelShaderConstantMatrix4(lua_State *L) {
	return 0;
}

//MATRIXES

int mat4_gc(lua_State *L) {
	//printf("## __core__gc\n");
	return 0;
}
int mat4_index(lua_State *L) {
	//printf("## __index\n");
    return 0;
}
int mat4_newindex(lua_State *L) {
	//printf("## __newindex\n");
    return 0;
}

int mat4_new(lua_State *L) {
	//printf("## __scene__new\n");

	lua_newuserdata(L,sizeof(userData));
    luaL_getmetatable(L, "Matrix4");
    lua_setmetatable(L, -2); 

    return 1;
}

int mat4_multiplyMatrixes(lua_State *L) {


	return 0;
}

int mat4_getTransform(lua_State *L) {
	return 0;
}
