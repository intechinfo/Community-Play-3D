//
//  CVector3d.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 23/04/2014.
//
//

#ifndef __C_LUA_VECTOR_3D_H_INCLUDED__
#define __C_LUA_VECTOR_3D_H_INCLUDED__

namespace cp3d {
namespace scripting {

#include <irrlicht.h>
#include "../Lua52/include/lua.hpp"

#include <SSWELib.h>

irr::core::vector3df *checkVector3df(lua_State *L, int n, bool error = true);
void SSWE_LIB_API bindVector3df(lua_State *L, bool allowConstructor = true);

#define VECTOR3DF_CHECK_ARGUMENTS(argc, L) \
	if (lua_gettop(L) != argc) { \
		luaL_error(L, "cannot check the arguments"); \
		return 0; \
	}

#define VECTOR3DF_CHECK_VECTORS(v1, v2) \
	if (!v1 || !v2) { \
		luaL_error(L, "cannot check the vectors"); \
		return 0; \
	}

#define VECTOR3DF_CHECK_VECTOR(v) \
	if (!v) {\
		luaL_error(L, "cannot check the vector"); \
		return 0; \
	}

} // End namespace scripting
} // End namespace cp3d

#endif /* defined(__C_LUA_VECTOR_3D_H_INCLUDED__) */
