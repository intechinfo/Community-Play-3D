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

void SSWE_LIB_API bindVector3df(lua_State *L);

} // End namespace scripting
} // End namespace cp3d

#endif /* defined(__C_LUA_VECTOR_3D_H_INCLUDED__) */
