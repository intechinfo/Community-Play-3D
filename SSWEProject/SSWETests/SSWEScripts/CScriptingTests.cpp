#include "stdafx.h"
#include "CppUnitTest.h"

#include "../../Lua52/include/lua.hpp"
#include "../../SSWELib/SSWELib/Device/Core/Scripting/math/CMatrix4.h"
#include "../../SSWELib/SSWELib/Device/Core/Scripting/math/CVector3d.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

namespace SSWETests {

	TEST_CLASS(CScriptingTest) {
			static lua_State *L;
			static IrrlichtDevice *device;
			static stringc workingPath;
		public:
//---------------------------------------------------------------------------------------------
//---------------------------------------INITIALIZE CLASS--------------------------------------
//---------------------------------------------------------------------------------------------
			TEST_CLASS_INITIALIZE(CScriptingTestInitialize) {
				device = createDevice(EDT_NULL);
				device->getFileSystem()->changeWorkingDirectoryTo(device->getFileSystem()->getWorkingDirectory() + "/../../../SSWE/");
				workingPath = device->getFileSystem()->getWorkingDirectory();

				L = luaL_newstate();
				luaL_openlibs(L);

				cp3d::scripting::bindVector3df(L, true);
				cp3d::scripting::bindMatrix4(L, device->getVideoDriver(), true);
			}
//---------------------------------------------------------------------------------------------
//---------------------------------------MATH TESTS-----------------------------------------
//---------------------------------------------------------------------------------------------
			TEST_METHOD(vector3dfTest) {
				int erred = luaL_dofile(L, stringc(workingPath + "/Lua_Tests/math/vector3df.lua").c_str());
				if (erred)
					Assert::Fail(stringw(luaL_checkstring(L, -1)).c_str());
			}

			TEST_METHOD(matrix4Test) {
				int erred = luaL_dofile(L, stringc(workingPath + "/Lua_Tests/math/matrix4.lua").c_str());
				if (erred)
					Assert::Fail(stringw(luaL_checkstring(L, -1)).c_str());
			}
//---------------------------------------------------------------------------------------------
//---------------------------------------TEAR DOWN CLASS---------------------------------------
//---------------------------------------------------------------------------------------------
			TEST_CLASS_CLEANUP(CCoreTestsCleanUp) {
				lua_close(L);

				device->closeDevice();
			}
	};
}

lua_State *SSWETests::CScriptingTest::L;
IrrlichtDevice *SSWETests::CScriptingTest::device;
stringc SSWETests::CScriptingTest::workingPath;
