//
//  CVector3d.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 23/04/2014.
//
//

#ifndef __C_LUA_MATRIX4_H_INCLUDED__
#define __C_LUA_MATRIX4_H_INCLUDED__

namespace cp3d {
namespace scripting {

#include <irrlicht.h>
#include "../Lua52/include/lua.hpp"

#include <SSWELib.h>

irr::core::matrix4 *checkMatrix4(lua_State *L, int n);
void SSWE_LIB_API bindMatrix4(lua_State *L, irr::video::IVideoDriver *driver);

#define MATRIX4_CHECK_ARGUMENTS(argc, L) \
	if (lua_gettop(L) != argc) return 0; \

#define MATRIX4_CHECK_MATRIXES(mat1, mat2) \
	if (!mat1 || !mat2) return 0; \

#define MATRIX4_CHECK_MATRIX(mat) \
	if (!mat) return 0;

} // End namespace scripting
} // End namespace cp3d

#endif /* defined(__C_LUA_VECTOR_3D_H_INCLUDED__) */
