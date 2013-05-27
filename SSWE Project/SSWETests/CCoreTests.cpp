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
				device = createDevice(EDT_NULL);

				core = new CCore();
				core->setDevice(device);
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
			TEST_METHOD(get__NUMBER__getVector3df) {
				//THIS TESTS core->getVector3df() USING std::string AND irr::core::stringw
				vector3df vec = vector3df(1.1f, 2.2f, 3.3f);
				vector3df vecGetted = core->getVector3df(std::string("1.1"), std::string("2.2"), std::string("3.3"));
				Assert::AreEqual(vec.X, vecGetted.X);
				Assert::AreEqual(vec.Y, vecGetted.Y);
				Assert::AreEqual(vec.Z, vecGetted.Z);
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
			TEST_METHOD(get__STRING__vector3dfTest) {
				stringw vecx = stringw("X :") + stringw(1.1f);
				stringw vecy = stringw(" Y : ") + stringw(2.2f);
				stringw vecz = stringw(" Z : ") + stringw(3.3f);
				stringw vec = vecx + vecy + vecz;
				Assert::AreEqual(vec.c_str(), core->getStrVector3df(vector3df(1.1, 2.2, 3.3)).c_str());
				Assert::AreNotEqual(vec.c_str(), core->getStrVector3df(vector3df(1.2, 2.3, 3.4)).c_str());
			}
//---------------------------------------------------------------------------------------------
//---------------------------------------TEXTURES_TESTS----------------------------------------
//---------------------------------------------------------------------------------------------
			TEST_METHOD(TEXTURES_textureAlreadyExistsTest) {
				Assert::AreEqual(-1, core->textureAlreadyExists("test_texture", device->getVideoDriver()));
				ITexture *texture_test = device->getVideoDriver()->addTexture(dimension2du(1, 1), "test_texture");
				Assert::AreNotEqual(-1, core->textureAlreadyExists("test_texture", device->getVideoDriver()));
			}
			TEST_METHOD(TEXTURES_getTexturePathTest) {
				ITexture *tex = device->getVideoDriver()->addTexture(dimension2du(100, 100), "test");
				Assert::AreEqual(L"test", core->getTexturePath(tex).c_str());
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
//---------------------------------------GUI_TESTS---------------------------------------------
//---------------------------------------------------------------------------------------------
			TEST_METHOD(GUI_getArrayOfAListOfGUIElementChildren) {
				IGUIButton *btn1 = device->getGUIEnvironment()->addButton(rect<s32>(0, 0, 1, 1), 0);
				IGUIButton *btn2 = device->getGUIEnvironment()->addButton(rect<s32>(0, 0, 1, 1), 0);

				array<IGUIElement *> elements = core->getArrayOfAListOfGUIElementChildren(device->getGUIEnvironment()->getRootGUIElement());
				if (elements.size() != 2) {
					Assert::Fail(L"Number of gui elements in array is different of gui elements created before");
				}
				if (elements[0] != btn1) {
					Assert::Fail(L"gui element 1 is different of first element created before");
				}
				if (elements[1] != btn2) {
					Assert::Fail(L"gui element 2 is different of second element created before");
				}

				device->getGUIEnvironment()->clear();
			}
			TEST_METHOD(GUI_elementIsInArrayOfElements) {
				IGUIButton *btn1 = device->getGUIEnvironment()->addButton(rect<s32>(0, 0, 1, 1), 0);
				IGUIButton *btn2 = device->getGUIEnvironment()->addButton(rect<s32>(0, 0, 1, 1), 0);
				IGUIButton *btn3 = device->getGUIEnvironment()->addButton(rect<s32>(0, 0, 1, 1), 0);
				IGUIElement *root = device->getGUIEnvironment()->getRootGUIElement();

				//GET ARRAY OF CHILDREN OF THE ROOT GUI ELEMENT (CONTAINING btn1, btn2 and btn3)
				array<IGUIElement *> elements = core->getArrayOfAListOfGUIElementChildren(root);

				Assert::AreEqual(true, core->elementIsInArrayOfElements(btn3, elements));

				device->getGUIEnvironment()->clear();
			}
			TEST_METHOD(GUI_fillArrayOfGUIElementsFromArrayOfGUIElements) {
				IGUIButton *btn1 = device->getGUIEnvironment()->addButton(rect<s32>(0, 0, 1, 1), 0);
				IGUIButton *btn2 = device->getGUIEnvironment()->addButton(rect<s32>(0, 0, 1, 1), 0);
				IGUIButton *btn3 = device->getGUIEnvironment()->addButton(rect<s32>(0, 0, 1, 1), 0);

				array<IGUIElement *> elements;
				array<IGUIElement *> elementsToFill;

				elements = core->getArrayOfAListOfGUIElementChildren(device->getGUIEnvironment()->getRootGUIElement());
				core->fillArrayOfGUIElementsFromArrayOfGUIElements(elementsToFill, elements);

				if (elementsToFill.size() != elements.size()) {
					stringw fail = L"\nFail : sizes are not equal";
					fail += stringw("\n\t elementsToFill : ") + stringw(elementsToFill.size());
					fail += stringw("\n\t elements : ") + stringw(elements.size());
					Assert::Fail(fail.c_str());
				}
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
