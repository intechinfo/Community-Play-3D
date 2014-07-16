#include "stdafx.h"
#include "CppUnitTest.h"

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
				workingPath = device->getFileSystem()->getWorkingDirectory() + "/../../../SSWE/";

				L = luaL_newstate();
				luaL_openlibs(L);
			}
//---------------------------------------------------------------------------------------------
//---------------------------------------MATH TESTS-----------------------------------------
//---------------------------------------------------------------------------------------------
			TEST_METHOD(vector3dfTest) {
				cp3d::scripting::bindVector3df(L);
				int erred = luaL_dofile(L, stringc(workingPath + "Lua_Tests/math/vector3df.lua").c_str());
				if (erred)
					Assert::Fail(stringw(luaL_checkstring(L, -1)).c_str());
			}

			TEST_METHOD(matrix4Test) {
				cp3d::scripting::bindVector3df(L);
				cp3d::scripting::bindMatrix4(L, device->getVideoDriver());
				int erred = luaL_dofile(L, stringc(workingPath + "Lua_Tests/math/matrix4.lua").c_str());
				if (erred)
					Assert::Fail(stringw(luaL_checkstring(L, -1)).c_str());
			}
//---------------------------------------------------------------------------------------------
//---------------------------------------TEAR DOWN CLASS---------------------------------------
//---------------------------------------------------------------------------------------------
			TEST_CLASS_CLEANUP(CCoreTestsCleanUp) {
				lua_close(L);

				device->closeDevice();
				device->drop();
			}
	};
}

lua_State *SSWETests::CScriptingTest::L;
IrrlichtDevice *SSWETests::CScriptingTest::device;
stringc SSWETests::CScriptingTest::workingPath;
