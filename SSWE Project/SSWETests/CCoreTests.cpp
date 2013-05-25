#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SSWETests {

	TEST_CLASS(CCoreTests) {
			static CCore *core;
			static IrrlichtDevice *device;
		public:
//---------------------------------------------------------------------------------------------
//---------------------------------------INITIALIZE CLASS--------------------------------------
//---------------------------------------------------------------------------------------------
			TEST_CLASS_INITIALIZE(CCoreTestsInitialize) {
				core = new CCore();
				device = createDevice(EDT_NULL);
			}
//---------------------------------------------------------------------------------------------
//---------------------------------------NUMBERS TESTS-----------------------------------------
//---------------------------------------------------------------------------------------------
			TEST_METHOD(get__NUMBER__U32Test) {
				Assert::AreEqual((u32)1, core->getU32("1"));
				Assert::AreNotEqual((u32)1, core->getU32("2"));
			}
			TEST_METHOD(get__NUMBER__S32Test) {
				Assert::AreEqual(1, core->getS32("1"));
				Assert::AreNotEqual(1, core->getS32("2"));
			}
			TEST_METHOD(get__NUMBER__F32Test) {
				Assert::AreEqual(1.1f, core->getF32("1.1"), false, L"1", LINE_INFO());
				Assert::AreNotEqual(1.1f, core->getF32("2.1"), false, L"1", LINE_INFO());
			}
//---------------------------------------------------------------------------------------------
//------------------------------------STRING NUMBERS TESTS-------------------------------------
//---------------------------------------------------------------------------------------------
			TEST_METHOD(get__STRING__U32Test) {
				Assert::AreEqual(L"1", core->getStrNumberU32(1).c_str());
				Assert::AreNotEqual(L"1", core->getStrNumberU32(2).c_str());
			}
			TEST_METHOD(get__STRING__F32Test) {
				stringw number(1.1);
				Assert::AreEqual(number.c_str(), core->getStrNumber(1.1f).c_str());
				Assert::AreNotEqual(number.c_str(), core->getStrNumber(2.1f).c_str());
			}
			TEST_METHOD(get__STRING_ConvertToStringTest) {
				stringw value = "Test";
				Assert::AreEqual(stringw("Test").c_str(), stringw(core->convertToString(value).c_str()).c_str());
				Assert::AreNotEqual(stringw("Test_").c_str(), stringw(core->convertToString(value).c_str()).c_str());
			}
			TEST_METHOD(get__STRING_ChangeTextWithValueTest) {
				stringw value = "Test";
				Assert::AreEqual(stringw(value + "1").c_str(), core->changeTextWithValue(value.c_str(), 1).c_str());
				Assert::AreNotEqual(stringw(value + "2").c_str(), core->changeTextWithValue(value.c_str(), 1).c_str());
			}
//---------------------------------------------------------------------------------------------
//---------------------------------------TEXTURES_TESTS----------------------------------------
//---------------------------------------------------------------------------------------------
			TEST_METHOD(TEXTURES_textureAlreadyExistsTest) {
				Assert::AreEqual(-1, core->textureAlreadyExists("test_texture", device->getVideoDriver()));
				ITexture *texture_test = device->getVideoDriver()->addTexture(dimension2du(1, 1), "test_texture");
				Assert::AreNotEqual(-1, core->textureAlreadyExists("test_texture", device->getVideoDriver()));
			}
//---------------------------------------------------------------------------------------------
//---------------------------------------NODES_TESTS-------------------------------------------
//---------------------------------------------------------------------------------------------
			TEST_METHOD(NODES_getNodePrefixTest) {
				ISceneNode *node = device->getSceneManager()->addEmptySceneNode();
				node->setName("#map:test");
				Assert::AreEqual("#map", core->getNodeNamePrefix(node).c_str());
				Assert::AreNotEqual("#map:", core->getNodeNamePrefix(node).c_str());
			}
//---------------------------------------------------------------------------------------------
//---------------------------------------TEAR DOWN CLASS---------------------------------------
//---------------------------------------------------------------------------------------------
			TEST_CLASS_CLEANUP(CCoreTestsCleanUp) {
				delete core;

				device->closeDevice();
				device->drop();
			}
	};
}

CCore *SSWETests::CCoreTests::core;
IrrlichtDevice *SSWETests::CCoreTests::device;
