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
irr::core::stringc gWorkingPath;

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
	gWorkingPath = devices->getWorkingDirectory();

	IGUIEnvironment *gui = devices->getGUIEnvironment();
	//GUI ELEMENTS
	//RENDERERS
	window = gui->addWindow(rect<s32>(460, 260, 1360, 680), false, L"Shaders Edition Window", 0, -1);
	window->getMaximizeButton()->setVisible(true);
	window->getMinimizeButton()->setVisible(true);
	devices->getCore()->centerWindow(window, devices->getVideoDriver()->getScreenSize());

	viewPort = new CGUIViewport(devices->getGUIEnvironment(), window, 1, rect<s32>(10, 60, 460, 350));
    if (viewPort) {
		//viewPort->setScreenQuad(devices->getXEffect()->getScreenQuad());
		viewPort->setScreenQuad(devices->getXEffect()->getScreenQuadPtr());
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

	openLogger = gui->addButton(rect<s32>(490, 350, 590, 370), window, -1, L"Open Logs", L"Open Logs Window");

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
	submenu->addItem(L"Load From File... (CTRL+O)", 1, true);
	submenu->addItem(L"Load From Package...", 2, true);
	submenu->addItem(L"Close", 3, true);

	submenu = menu->getSubMenu(1);
	submenu->addItem(L"Reload (CTRL+RETURN)", 1, true);

	//OTHERS
	devices->getEventReceiver()->AddEventReceiver(this, window);

	this->createLogWindow();
}

CUIWindowEditFilters::~CUIWindowEditFilters() {
	loggerWindow->remove();
}

void CUIWindowEditFilters::createLogWindow() {
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	loggerWindow = gui->addWindow(rect<s32>(380, 110, 1370, 480), false, L"Filters Log", 0, -1);
	loggerWindow->getCloseButton()->setVisible(false);

	logEditBox = gui->addEditBox(L"", rect<s32>(10, 30, 980, 330), true, loggerWindow, -1);
	logEditBox->setMultiLine(true);
	logEditBox->setWordWrap(true);
	logEditBox->setAutoScroll(true);

	closeLogWindow = gui->addButton(rect<s32>(880, 340, 980, 360), loggerWindow, -1, L"Close", L"Close Logs Window");
	devices->getCore()->centerWindow(loggerWindow, devices->getVideoDriver()->getScreenSize());

	loggerWindow->setVisible(false);
}

void CUIWindowEditFilters::reloadFilters() {
	ELOG_LEVEL logLevel = devices->getDevice()->getLogger()->getLogLevel();
	devices->getDevice()->getLogger()->setLogLevel(ELL_INFORMATION);
	stringw logText = L"--------------------------------------------\n\n";
	logText += L"Compiling Filters\n\n";
	logEditBox->setText(L"");

	devices->getXEffect()->removeAllPostProcessingEffects();

	array<SFilter> *arrayf = devices->getCoreData()->getEffectFilters();
	array<stringc> newCodes;

	for (u32 i=0; i < arrayf->size(); i++) {
		logText += arrayf->operator[](i).getName().c_str();
		logText += "\n";

		if (arrayf->operator[](i).getPixelShader() == "") {
			newCodes.push_back("wanted_error();");
		} else {
			newCodes.push_back(arrayf->operator[](i).getPixelShader());
		}
	}

	logText += L"\n--------------------------------------------\n\n";

	logEditBox->setText(logText.c_str());
	bool success = true;

	irr::core::array<s32> materialTypes = devices->getXEffect()->reloadPostProcessingEffects(newCodes);
	stringc errors = "";
	for (u32 i=0; i < materialTypes.size(); i++) {
		if (materialTypes[i] == -1) {
			errors += "Error when loading ";
			errors += arrayf->operator[](i).getName();
			errors += "\n";
		} else {
			//devices->getXEffect()->setPostProcessingRenderCallback(materialTypes[i], arrayf->operator[](i).getPostProcessingCallback());
			CFilterCallback *cb = new CFilterCallback(materialTypes[i], &arrayf->operator[](i));
			devices->getXEffect()->setPostProcessingRenderCallback(materialTypes[i], cb);
			arrayf->operator[](i).setPostProcessingCallback(cb);
		}
		arrayf->operator[](i).setMaterial(materialTypes[i]);
	}

	if (errors != "")
		devices->addInformationDialog("Errors", errors, EMBF_OK, true);

	stringw text = logEditBox->getText();
	if (success) {
		text += "Success";
	} else {
		text += "Failure";
	}
	logEditBox->setText(text.c_str());

	devices->getDevice()->getLogger()->setLogLevel(logLevel);

}

bool CUIWindowEditFilters::OnEvent(const SEvent &event) {

	if (event.EventType == EET_LOG_TEXT_EVENT) {
		stringw textLogEvent = logEditBox->getText();
		textLogEvent += event.LogEvent.Text;
		textLogEvent += L"\n";
		logEditBox->setText(textLogEvent.c_str());
	}

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
			if (event.GUIEvent.Caller == menu->getSubMenu(1)) {
				if (menu->getSubMenu(1)->getSelectedItem() == 0) {
					this->reloadFilters();
				}
			}
		}

		//FILE SELECTION
		if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
			if (event.GUIEvent.Caller == openShader) {
				SFilter f;
				f.createLuaState();
				f.setPixelShader(devices->getCore()->getStringcFromFile(openShader->getFileName()));
				//f.setCallback("filter:setPixelShaderConstantVector2D(\"multiplier\", {x=2, y=3})");
				f.setMaterial(devices->getXEffect()->addPostProcessingEffectFromString(f.getPixelShader().c_str()));
				
				stringw name = openShader->getFileName();
				name.remove(devices->getDevice()->getFileSystem()->getFileDir(name));
				f.setName(name);

				devices->getCoreData()->getEffectFilters()->push_back(f);
				u32 count = devices->getCoreData()->getEffectFilters()->size();
				this->createLuaState(devices->getCoreData()->getEffectFilters()->operator[](count-1).getLuaState());
				CFilterCallback *cb = new CFilterCallback(devices->getCoreData()->getEffectFilters()->operator[](count-1).getMaterial(),
														  &devices->getCoreData()->getEffectFilters()->operator[](count-1));
				devices->getXEffect()->setPostProcessingRenderCallback(devices->getCoreData()->getEffectFilters()->operator[](count-1).getMaterial(), cb);
				devices->getCoreData()->getEffectFilters()->operator[](count-1).setPostProcessingCallback(cb);

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
			if (event.GUIEvent.Caller == openLogger) {
				loggerWindow->setVisible(true);
				devices->getGUIEnvironment()->getRootGUIElement()->bringToFront(loggerWindow);
			}
			if (event.GUIEvent.Caller == closeLogWindow) {
				loggerWindow->setVisible(false);
			}
		}
	}

	if (event.EventType == EET_KEY_INPUT_EVENT) {
		if (!event.KeyInput.PressedDown) {
			if (devices->isCtrlPushed()) {
				if (event.KeyInput.Key == KEY_RETURN) {
					if (filters->getSelected() != -1) {
						this->reloadFilters();
						//std::thread t(&CUIWindowEditFilters::reloadFilters, *this);
						//t.detach();
					}
				}
				if (event.KeyInput.Key == KEY_KEY_O) {
					openShader = devices->createFileOpenDialog(L"Load From File...", CGUIFileSelector::EFST_OPEN_DIALOG,
						devices->getGUIEnvironment()->getRootGUIElement(), false);
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

	geffect->setPostProcessingEffectConstant(gmaterialType, name.c_str(), reinterpret_cast<f32*>(&value), 1);

	return 0;
}

int core_setPixelShaderConstantFloat(lua_State *L) {
	int argc = lua_gettop(L);
	if (argc < 3)
		return 0;

	stringc name = lua_tostring(L, 2);
	luaL_checktype(L, 3, LUA_TNUMBER);

	f32 *value = new f32(lua_tonumber(L, 3));

	geffect->setPostProcessingEffectConstant(gmaterialType, name.c_str(), value, 1);

	return 0;
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

	return 0;
}

int core_setPixelShaderConstantVector3D(lua_State *L) {
	int argc = lua_gettop(L);
	if (argc < 3)
		return 0;

	stringc name = lua_tostring(L, 2);

	luaL_checktype(L, 3, LUA_TTABLE);
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

	geffect->setPostProcessingEffectConstant(gmaterialType, name.c_str(), v, 3);

	return 0;
}

int core_setPixelShaderConstantMatrix4(lua_State *L) {
	return 0;
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

	return 0;
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

	return 0;
}

//MATRIXES

int mat4_gc(lua_State *L) {
	//printf("## __mat4__gc\n");
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
	//printf("## __mat4__new\n");

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

//UTILS
int utils_gc(lua_State *L) {
	//printf("## __utils__gc\n");
	return 0;
}
int utils_index(lua_State *L) {
	//printf("## __index\n");
    return 0;
}
int utils_newindex(lua_State *L) {
	//printf("## __newindex\n");
    return 0;
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
	return 0;
}
int driver_index(lua_State *L) {
	//printf("## __index\n");
    return 0;
}
int driver_newindex(lua_State *L) {
	//printf("## __newindex\n");
    return 0;
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

	return 0;
}

int driver_setRttTexture(lua_State *L) {
	int argc = lua_gettop(L);
	if (argc < 2)
		return 0;

	stringc path = lua_tostring(L, 2);
	geffect->setPostProcessingUserTexture(geffect->getIrrlichtDevice()->getVideoDriver()->getTexture(path));

	return 0;
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
