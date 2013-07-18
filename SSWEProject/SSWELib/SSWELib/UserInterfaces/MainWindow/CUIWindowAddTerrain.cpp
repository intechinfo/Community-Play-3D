//
//  CUIWindowAddOctTree.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 27/09/12.
//
//
#include "stdafx.h"
#include "CUIWindowAddTerrain.h"

CUIWindowAddOctTree::CUIWindowAddOctTree(CDevices *_devices, IGUIListBox *_terrainsListBox) {
    devices = _devices;

    terrainsListBox = _terrainsListBox;

    path_file = L"";
	addOctTreeWindow = 0;

	terrainType = ELTT_MESH;
}

CUIWindowAddOctTree::~CUIWindowAddOctTree() {

}

void CUIWindowAddOctTree::open() {

	devices->getEventReceiver()->RemoveEventReceiver(this);
	devices->getEventReceiver()->AddEventReceiver(this);

	if (addOctTreeWindow) {
		addOctTreeWindow->remove();
	}

	addOctTreeWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(440, 130, 740, 320), true, L"Add a terrain mesh", 0, -1);
	devices->getGUIEnvironment()->addStaticText(L"Type : ", rect<s32>(10, 30, 60, 50), false, false, addOctTreeWindow, -1, false);
	terrainTypecb = devices->getGUIEnvironment()->addComboBox(rect<s32>(60, 30, 290, 50), addOctTreeWindow, -1);
	terrainTypecb->addItem(L"MESH");
	terrainTypecb->addItem(L"TERRAIN");
	terrainTypecb->addItem(L"TERRAIN_MESH");
	devices->getGUIEnvironment()->addStaticText(L"Name : (#map: will be added automatically)", rect<s32>(10, 60, 290, 80), false, false, addOctTreeWindow, -1, false);

	if (terrainType == ELTT_MESH) {

		devices->getGUIEnvironment()->addStaticText(L"Name : (#map: will be added automatically)", rect<s32>(10, 60, 290, 80), false, false, addOctTreeWindow, -1, false);
		addOctTreeEditBox = devices->getGUIEnvironment()->addEditBox(L"myName", rect<s32>(10, 80, 190, 100), true, addOctTreeWindow, -1);
		selectMesh =devices->getGUIEnvironment()->addButton(rect<s32>(190, 80, 290, 100), addOctTreeWindow, -1, L"Select Mesh", L"Select the mesh");

		asMeshSceneNode = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(10, 110, 290, 130), addOctTreeWindow, -1, L"Load as Mesh instead of OctTree Mesh");

		minPolysPerNodeTxt = devices->getGUIEnvironment()->addStaticText(L"Minimal Polys Per Node :", rect<s32>(10, 130, 180, 150), true, false, addOctTreeWindow, -1, false);
		minPolysPerNode = devices->getGUIEnvironment()->addEditBox(L"256", rect<s32>(180, 130, 290, 150), true, addOctTreeWindow, -1);

		accept = devices->getGUIEnvironment()->addButton(rect<s32>(10, 160, 110, 180), addOctTreeWindow, -1, L"Accept", 
														 L"Accept and add the selected mesh");
		close = devices->getGUIEnvironment()->addButton(rect<s32>(120, 160, 220, 180), addOctTreeWindow, -1, L"Cancel", 
														L"Close this window");
	}
	if (terrainType == ELTT_TERRAIN) {
		addOctTreeWindow->setRelativePosition(rect<s32>(440, 130, 740, 340));
		addOctTreeEditBox = devices->getGUIEnvironment()->addEditBox(L"myName", rect<s32>(10, 80, 190, 100), true, addOctTreeWindow, -1);
		selectMesh =devices->getGUIEnvironment()->addButton(rect<s32>(190, 80, 290, 100), addOctTreeWindow, -1, L"Select Hgtmap", L"Select the Height Map");

		devices->getGUIEnvironment()->addStaticText(L"Patch Size :", rect<s32>(10, 110, 120, 130), true, false, addOctTreeWindow, -1, false);
		terrainPatchSizecb = devices->getGUIEnvironment()->addComboBox(rect<s32>(120, 110, 290, 130), addOctTreeWindow, -1);
		terrainPatchSizecb->addItem(L"9");
		terrainPatchSizecb->addItem(L"17");
		terrainPatchSizecb->addItem(L"33");
		terrainPatchSizecb->addItem(L"65");
		terrainPatchSizecb->addItem(L"129");

		devices->getGUIEnvironment()->addStaticText(L"Max Level Of Details :", rect<s32>(10, 130, 160, 150), true, false, addOctTreeWindow, -1, false);
		terrainMaxLODeb = devices->getGUIEnvironment()->addEditBox(L"5", rect<s32>(160, 130, 290, 150), true, addOctTreeWindow, -1);

		devices->getGUIEnvironment()->addStaticText(L"Smooth Factor :", rect<s32>(10, 150, 120, 170), true, false, addOctTreeWindow, -1, false);
		terrainSmoothFactoreb = devices->getGUIEnvironment()->addEditBox(L"0", rect<s32>(120, 150, 290, 170), true, addOctTreeWindow, -1);

		accept = devices->getGUIEnvironment()->addButton(rect<s32>(10, 180, 110, 200), addOctTreeWindow, -1, L"Accept", 
														 L"Accept and add the selected mesh");
		close = devices->getGUIEnvironment()->addButton(rect<s32>(120, 180, 220, 200), addOctTreeWindow, -1, L"Cancel", 
														L"Close this window");
	}

}

bool CUIWindowAddOctTree::OnEvent(const SEvent &event) {

    if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == addOctTreeWindow) {
				devices->getEventReceiver()->RemoveEventReceiver(this);
				addOctTreeWindow = 0;
			}
		}

		if (event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED) {
			if (event.GUIEvent.Caller == asMeshSceneNode) {
				//UPDATE WINDOW AS MESH SCENE NODE
				if (asMeshSceneNode->isChecked()) {
					minPolysPerNodeTxt->setEnabled(false);
					minPolysPerNode->setEnabled(false);
				} else {
					minPolysPerNodeTxt->setEnabled(true);
					minPolysPerNode->setEnabled(true);
				}
			}
		}

		if (event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED) {
			if (event.GUIEvent.Caller == terrainTypecb) {
				terrainType = (ELOAD_TERRAIN_TYPE)terrainTypecb->getSelected();
				open();
			}
		}

        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == selectMesh) {
				openMesh = devices->createFileOpenDialog(L"Select the mesh", 0);
				openMesh->setMaxSize(dimension2du(1280, 800));
			}

			if (event.GUIEvent.Caller == accept) {
                    IMesh *octTreeMesh = 0;
                    ISceneNode *octTreeNode = 0;
					STerrainsData tdata;

					u32 minppn = -1;
					if (terrainType == ELTT_MESH) {
						if (!asMeshSceneNode->isChecked()) {
							stringc minppn_w = minPolysPerNode->getText();
							minppn = devices->getCore()->getU32(minppn_w.c_str());
							if (minppn < 0)
								minppn = 256;
						}
						octTreeMesh = devices->getSceneManager()->getMesh(path_file.c_str());
                        
                        if (asMeshSceneNode->isChecked()) {
                            octTreeNode = devices->getSceneManager()->addMeshSceneNode(octTreeMesh, 0, -1);
                        } else {
                            octTreeNode = devices->getSceneManager()->addOctreeSceneNode(octTreeMesh, 0, -1, minppn);
                        }
					}
                    
					//LOAD MESH AND ADD NODE
                    if (terrainType == ELTT_TERRAIN) {
						u32 terrainPatchSize = devices->getCore()->getU32(terrainPatchSizecb->getItem(terrainPatchSizecb->getSelected()));

						s32 smoothFactor = devices->getCore()->getS32(terrainSmoothFactoreb->getText());
						if (smoothFactor < 0)
							smoothFactor = 0;

						s32 terrainMaxLOD = devices->getCore()->getS32(terrainMaxLODeb->getText());
						if (terrainMaxLOD < 0)
							terrainMaxLOD = 5;

                        octTreeNode = devices->getSceneManager()->addTerrainSceneNode(path_file, 0, -1, vector3df(0.f, 0.f, 0.f),
                                                                                      vector3df(0.f, 0.f, 0.f), vector3df(1.f, 0.1f, 1.f),
                                                                                      SColor (255, 255, 255, 255),
																					  terrainMaxLOD,
																					  (E_TERRAIN_PATCH_SIZE)terrainPatchSize,
																					  smoothFactor);
						/*octTreeNode = new CTerrainSceneNode(0, devices->getDevice(), devices->getDevice()->getFileSystem(), -1, terrainMaxLOD, 
															(E_TERRAIN_PATCH_SIZE)terrainPatchSize);
						io::IReadFile* file = devices->getDevice()->getFileSystem()->createAndOpenFile(path_file.c_str());
						((CTerrainSceneNode *)octTreeNode)->loadHeightMap(file, SColor (255, 255, 255, 255), smoothFactor);*/
                    }
					//IF NODE SET VALUES
					if (octTreeNode) {
						tdata.setMesh(octTreeMesh);

						if (octTreeNode->getType() == ESNT_OCTREE)
							tdata.setMinPolysPerNode(minppn);

						tdata.setType(octTreeNode->getType());
					}
					//INITIALIZE NODE
                    if (octTreeNode) {
                        octTreeNode->setMaterialFlag(EMF_LIGHTING, false);
                        octTreeNode->setMaterialFlag(EMF_NORMALIZE_NORMALS, false);
                        
                        stringw octTreeNodeName = L"#map:";
                        octTreeNodeName += addOctTreeEditBox->getText();
                        octTreeNode->setName(octTreeNodeName.c_str());
                        
                        devices->getXEffect()->addShadowToNode(octTreeNode, devices->getXEffectFilterType(), ESM_BOTH);
                        devices->getCollisionManager()->setCollisionToAnOctTreeNode(octTreeNode);
                        
						tdata.setNode(octTreeNode);
						tdata.setPath(path_file.c_str());
						devices->getCoreData()->getTerrainsData()->push_back(tdata);
                        
                        terrainsListBox->addItem(octTreeNodeName.c_str());
                        
                        addOctTreeWindow->remove();
						addOctTreeWindow = 0;
						devices->getEventReceiver()->RemoveEventReceiver(this);
                    } else {
                        devices->addWarningDialog(L"Warning", 
                                                  L"Error when loading the selected mesh \n\n"
                                                  L"Please verify you 3D model : \n", EMBF_OK);
                    }
                }
                    
			if (event.GUIEvent.Caller == close) {
                    addOctTreeWindow->remove();
					devices->getEventReceiver()->RemoveEventReceiver(this);
					addOctTreeWindow = 0;
            }
        }
        
        if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
			if (event.GUIEvent.Caller == openMesh) {
                path_file = L"";
                path_file.append(openMesh->getFileName());
            }
        }
    }
    
    return false;
}
