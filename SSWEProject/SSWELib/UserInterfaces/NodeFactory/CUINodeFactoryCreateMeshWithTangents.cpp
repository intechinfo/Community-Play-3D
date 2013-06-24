#include "stdafx.h"
#include "CUINodeFactoryCreateMeshWithTangents.h"

CUINodeFactoryCreateMeshWithTangents::CUINodeFactoryCreateMeshWithTangents(CDevices *_devices) {
	devices = _devices;
	devices->getEventReceiver()->AddEventReceiver(this);
}
CUINodeFactoryCreateMeshWithTangents::~CUINodeFactoryCreateMeshWithTangents() {

}

void CUINodeFactoryCreateMeshWithTangents::open(ISceneNode *node, IMesh *mesh) {
	nodeToEdit = node;
	meshToEdit = mesh;

	IGUIEnvironment *gui = devices->getDevice()->getGUIEnvironment();
	//WINDOW
	window = gui->addWindow(rect<s32>(260, 140, 490, 360), false, L"Create Mesh With Tangents", 0, -1);
	//WINDOW ASSETS
	stringw meshName = L"Mesh : ";
	meshName += node->getName();
	gui->addStaticText(meshName.c_str(), rect<s32>(10, 30, 200, 50), true, true, window, -1, false);

	recalculateNormals = gui->addCheckBox(false, rect<s32>(10, 60, 200, 80), window, -1, L"Recalculate Normals");
	smooth = gui->addCheckBox(false, rect<s32>(10, 90, 200, 110), window, -1, L"Smooth");
	angleWeighted = gui->addCheckBox(false, rect<s32>(10, 120, 200, 140), window, -1, L"Angle Weigthed");
	recalculateTangents = gui->addCheckBox(false, rect<s32>(10, 150, 200, 170), window, -1, L"Recalculate Tangents");

	//BUTTONS
	accept = gui->addButton(rect<s32>(10, 180, 110, 210), window, -1, L"Ok", L"Apply to the mesh");
	cancel = gui->addButton(rect<s32>(120, 180, 220, 210), window, -1, L"Close", L"Close The Window");
}

void CUINodeFactoryCreateMeshWithTangents::create() {
	CProcess *process = new CProcess(devices->getGUIEnvironment(), "CREATE MESH WITH TANGENTS");
	devices->getProcessesLogger()->addProcess(process);

	IGUIWindow *windowPrecessing = devices->addInformationDialog(L"Informations", L"Are you sure ?\nProcessing...", 0x0, true, window);
	sure->setText(L"Processing...");
	window->addChild(windowPrecessing);

    devices->getVideoDriver()->beginScene(true, true, SColor(0x0));
    devices->updateDevice();
    devices->getVideoDriver()->endScene();

	IMeshManipulator *mm = devices->getSceneManager()->getMeshManipulator();
	devices->getVideoDriver()->beginScene(true, true, SColor(0x0));
    devices->updateDevice();
    devices->getVideoDriver()->endScene();
	IMesh *tangentsMesh = mm->createMeshWithTangents(meshToEdit, recalculateNormals->isChecked(), smooth->isChecked(), angleWeighted->isChecked(), recalculateTangents->isChecked());
	ISceneNode *newNode;
	if (nodeType == ESNT_OCTREE) {
		newNode = devices->getSceneManager()->addOctreeSceneNode(tangentsMesh);
	} else {
		newNode = devices->getSceneManager()->addMeshSceneNode(tangentsMesh);
	}

	newNode->setName(stringc(stringc(nodeToEdit->getName()) + stringc("_with_tangents")).c_str());
	newNode->setPosition(nodeToEdit->getPosition());
	newNode->setRotation(nodeToEdit->getRotation());
	newNode->setParent(nodeToEdit->getParent());
	newNode->setScale(nodeToEdit->getScale());
	for (u32 i=0; i < nodeToEdit->getMaterialCount(); i++) {
		newNode->getMaterial(i) = nodeToEdit->getMaterial(i);
	}

	stringc prefix = devices->getCore()->getNodeNamePrefix(newNode);
	if (prefix == "#map") {
		devices->getCoreData()->getTerrainMeshes()->push_back(tangentsMesh);
		devices->getCoreData()->getTerrainNodes()->push_back(newNode);
		devices->getCoreData()->getTerrainMinPolysPerNode()->push_back(nodeMinPolysPerNode);
		devices->getCoreData()->getTerrainPaths()->push_back(meshPath.c_str());
	} else if (prefix == "#tree") {
		STreesData tdata(tangentsMesh, newNode, meshPath.c_str(), ESNT_OCTREE, nodeMinPolysPerNode);
		devices->getCoreData()->getTreesData()->push_back(tdata);
	} else if (prefix == "#object") {
		SObjectsData odata(tangentsMesh, newNode, meshPath.c_str());
		devices->getCoreData()->getObjectsData()->push_back(odata);
	} else if (prefix == "#water") {
		CWaterSurface *ws = new CWaterSurface(devices->getSceneManager(), 0, 0);
		SWaterSurfacesData wsdata(ws, 0);
		devices->getCoreData()->getWaterSurfaces()->push_back(wsdata);
	}

	devices->getXEffect()->addShadowToNode(newNode, devices->getXEffectFilterType(), 
											devices->getXEffect()->getNodeShadowMode(nodeToEdit, devices->getXEffectFilterType()));
	devices->getCollisionManager()->setCollisionToAnOctTreeNode(newNode);

	windowPrecessing->remove();
	window->remove();

	process->setHasFinished(true);
}

bool CUINodeFactoryCreateMeshWithTangents::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == cancel) {
				devices->getEventReceiver()->RemoveEventReceiver(this);
				window->remove();
				delete this;
			}
			if (event.GUIEvent.Caller == accept) {
				sure = devices->addInformationDialog(L"Informations", L"Are you sure ?", 0x3, true, window);
			}
		}
		if (event.GUIEvent.EventType == EGET_MESSAGEBOX_OK) {
			if (event.GUIEvent.Caller == sure) {
				std::thread create_t(&CUINodeFactoryCreateMeshWithTangents::create, *this);
				create_t.detach();

				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
		}
	}

	return false;
}
