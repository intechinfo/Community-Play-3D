#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

namespace SSWETests
{
	TEST_CLASS(CCoreDataTests) {
		static IrrlichtDevice *device;
		static CCoreData *coreData;
	public:
//---------------------------------------------------------------------------------------------
//---------------------------------------INITIALIZE CLASS--------------------------------------
//---------------------------------------------------------------------------------------------
		TEST_CLASS_INITIALIZE(CCoreDataTestsInitialize) {
			device = createDevice(EDT_NULL);

			coreData = new CCoreData();
		}
//---------------------------------------------------------------------------------------------
//---------------------------------------CCORE DATA TESTS--------------------------------------
//---------------------------------------------------------------------------------------------
		TEST_METHOD(getAllSceneNodesTest) {
			//ADD A TERRAIN
			ISceneNode *emptyTerrain = device->getSceneManager()->addEmptySceneNode();
			STerrainsData tdata(0, emptyTerrain, "", 0, ESNT_EMPTY);
			coreData->getTerrainsData()->push_back(tdata);

			//ADD AN OBJECT
			ISceneNode *emptyObject = device->getSceneManager()->addEmptySceneNode();
			SObjectsData odata(0, emptyObject, "");
			coreData->getObjectsData()->push_back(odata);

			array<ISceneNode *> nodes = coreData->getAllSceneNodes();
			if (nodes.size() != 2) {
				Assert::Fail(L"2 nodes were added but the array doesn't have two elements");
			}
			if (nodes[0] != emptyTerrain) {
				Assert::Fail(L"Node 1 is not the first node added");
			}
			if (nodes[1] != emptyObject) {
				Assert::Fail(L"Node 2 is not the second node added");
			}

			//CLEAN UP CORE DATA
			device->getSceneManager()->clear();
			coreData->getTerrainsData()->erase(0);
			coreData->getObjectsData()->erase(0);
		}
		TEST_METHOD(clearAllTheArraysTest) {
			//ADD A TERRAIN
			ISceneNode *emptyTerrain = device->getSceneManager()->addEmptySceneNode();
			STerrainsData tdata(0, emptyTerrain, "", 0, ESNT_EMPTY);
			coreData->getTerrainsData()->push_back(tdata);

			//ADD AN OBJECT
			ISceneNode *emptyObject = device->getSceneManager()->addEmptySceneNode();
			SObjectsData odata(0, emptyObject, "");
			coreData->getObjectsData()->push_back(odata);

			Assert::AreEqual((u32)1, coreData->getTerrainsData()->size());
			Assert::AreEqual((u32)1, coreData->getObjectsData()->size());

			coreData->clearAllTheArrays();

			Assert::AreEqual((u32)0, coreData->getTerrainsData()->size());
			Assert::AreEqual((u32)0, coreData->getObjectsData()->size());

			device->getSceneManager()->clear();
		}
		TEST_METHOD(getArrayOfLightNodesTest) {
			ILightSceneNode *l1 = device->getSceneManager()->addLightSceneNode();
			ILightSceneNode *l2 = device->getSceneManager()->addLightSceneNode();

			SLightsData l1data(l1);
			SLightsData l2data(l2);
			coreData->getLightsData()->push_back(l1data);
			coreData->getLightsData()->push_back(l2data);

			array<ISceneNode *> nodes = coreData->getArrayOfLightNodes();

			if (nodes.size() != 2) {
				Assert::Fail(L"Number of lights in the array is not equal to number of lights in the scene");
			}
			if (nodes[0] != l1) {
				Assert::Fail(L"Light 1 in the array is not the first light added");
			}
			if (nodes[1] != l2) {
				Assert::Fail(L"Light 2 in the array is not the second light added");
			}

			coreData->clearAllTheArrays();
			device->getSceneManager()->clear();

			//BECAUSE getArrayOfObjectNodes ETC. FOLLOW THE SAME METHOD, WE WILL NOT TEST THEM
		}
		TEST_METHOD(getISDataOfSceneNode) {
			/// Create
			ISceneNode *cube1 = device->getSceneManager()->addCubeSceneNode(50.f);
			ILightSceneNode *l1 = device->getSceneManager()->addLightSceneNode();
			ISceneNode *emptyTerrain = device->getSceneManager()->addEmptySceneNode();

			/// Configure
			STerrainsData tdata(0, emptyTerrain, "", 0, ESNT_EMPTY);
			coreData->getTerrainsData()->push_back(tdata);

			SObjectsData odata1(0, cube1, "cube1");
			coreData->getObjectsData()->push_back(odata1);

			SLightsData l1data(l1);
			coreData->getLightsData()->push_back(l1data);

			/// Act
			ISData *cube1Data = coreData->getISDataOfSceneNode(odata1.getNode());
			ISData *terrainData = coreData->getISDataOfSceneNode(tdata.getNode());
			ISData *lightData = coreData->getISDataOfSceneNode(l1data.getNode());

			/// Test
			if (cube1Data->getNode() != cube1) {
				Assert::Fail(L"Cube1 != data1");
			}
			if (terrainData->getNode() != tdata.getNode()) {
				Assert::Fail(L"terrainData != emptyTerrain");
			}
			if (lightData->getNode() != l1data.getNode()) {
				Assert::Fail(L"lightData != l1data");
			}
		}
//---------------------------------------------------------------------------------------------
//---------------------------------------TEAR DOWN CLASS---------------------------------------
//---------------------------------------------------------------------------------------------
		TEST_CLASS_CLEANUP(CCoreDataTestsCleanUp) {
			delete coreData;

			device->closeDevice();
			device->drop();
		}
	};
}

CCoreData *SSWETests::CCoreDataTests::coreData;
IrrlichtDevice *SSWETests::CCoreDataTests::device;
