//
//  CUIWindowAddOctTree.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 27/09/12.
//
//
#include "stdafx.h"
#include "CUIWindowAddTerrain.h"

void createTilesForImage(c8* ImageFileName, c8 * TileName, irr::video::IVideoDriver * Driver, bool IsHeightMap) {
	/* Les tiles sont créés et nommés dans l'ordre suivant :

		1 - 2 - 3 - 4
		|   |   |   |
		5 - 6 - 7 - 8
		|   |   |   |
		9 -10 -11 -12
		|   |   |   |
		13 -14 -15 -16
	*/


	u16 TileSize = 256;	
	u16 TileSize2 = TileSize;
	core::dimension2du dim;
	
	// si c'est une heightmap on forme des tiles de 2^n+1 (257)
	if(IsHeightMap) {
		//TileSize = 256;
		//TileSize2 = TileSize;
		TileSize2++;
		dim.Height = TileSize2;
		dim.Width = TileSize2;
	}
	else {
		TileSize = 256;
		TileSize2 = TileSize;
		dim.Height = TileSize;
		dim.Width = TileSize;
	}

	video::IImage * image = Driver->createImageFromFile(ImageFileName);
	s32 * data = new s32[dim.Height * dim.Width * 3];
	video::IImage * tile  = Driver->createImageFromData(video::ECF_R8G8B8, dim, data);

	int n = 1;
	int xx = 0;
	int yy = 0;
	for(int j=0; j<4; j++) {
		for(int p=0; p<4; p++) {
			for(int y=0; y<=TileSize2; y++) {
				for(int x=0; x<=TileSize2; x++) {
					tile->setPixel(x, y, image->getPixel(x+xx,y+yy));
				}			
			}

			//core::stringc str = L"Media2/map";
			core::stringc str(TileName);
			str += n;
			str +=  ".png";
			n++;


			Driver->writeImageToFile(tile, str.c_str(), 0 ); 			
			if(IsHeightMap)
				xx+=TileSize;
			else
				xx+=TileSize-1;
		}
		xx=0;
		if(IsHeightMap)
			yy+=TileSize;
		else
			yy+=TileSize-1;
	} 
	


	image->drop();
	tile->drop();
}

CUIWindowAddOctTree::CUIWindowAddOctTree(CDevices *_devices, IGUIListBox *_terrainsListBox) {
    devices = _devices;

    terrainsListBox = _terrainsListBox;

    path_file = L"";
	addOctTreeWindow = 0;

	path_terrains_hgtmap = L"";
	path_terrains_texture = L"";

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
	terrainTypecb->setSelected(terrainType);
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

	if (terrainType == ELTT_TERRAIN_SPLATTING) {
		addOctTreeWindow->setRelativePosition(rect<s32>(440, 130, 990, 500));
		addOctTreeEditBox = devices->getGUIEnvironment()->addEditBox(L"myName", rect<s32>(10, 80, 190, 100), true, addOctTreeWindow, -1);
		terrainsOpenHeightMap =devices->getGUIEnvironment()->addButton(rect<s32>(190, 80, 290, 100), addOctTreeWindow, -1, L"Select Hgtmap", L"Select the Height Map");

		devices->getGUIEnvironment()->addStaticText(L"Map Size :", rect<s32>(10, 110, 90, 130), true, false, addOctTreeWindow, -1, false);
		terrainsMapSizeeb = devices->getGUIEnvironment()->addEditBox(L"256", rect<s32>(90, 110, 290, 130), true, addOctTreeWindow, -1);

		devices->getGUIEnvironment()->addStaticText(L"Dimension :", rect<s32>(10, 130, 160, 150), true, false, addOctTreeWindow, -1, false);
		terrainsDimensioneb = devices->getGUIEnvironment()->addEditBox(L"4", rect<s32>(160, 130, 290, 150), true, addOctTreeWindow, -1);

		devices->getGUIEnvironment()->addStaticText(L"LOD Texture Scale :", rect<s32>(10, 150, 160, 170), true, false, addOctTreeWindow, -1, false);
		terrainsLODTextureScaleeb = devices->getGUIEnvironment()->addEditBox(L"100.0", rect<s32>(160, 150, 290, 170), true, addOctTreeWindow, -1);

		devices->getGUIEnvironment()->addStaticText(L"Terrain Scale :", rect<s32>(10, 170, 140, 190), true, false, addOctTreeWindow, -1, false);
		terrainsScale = devices->getGUIEnvironment()->addEditBox(L"10", rect<s32>(140, 170, 290, 190), true, addOctTreeWindow, -1);

		devices->getGUIEnvironment()->addStaticText(L"Quality :", rect<s32>(10, 190, 90, 210), true, false, addOctTreeWindow, -1, false);
		terrainsQualitycb = devices->getGUIEnvironment()->addComboBox(rect<s32>(90, 190, 290, 210), addOctTreeWindow, -1);

		devices->getGUIEnvironment()->addStaticText(L"Distance Max Render :", rect<s32>(10, 210, 150, 230), true, false, addOctTreeWindow, -1, false);
		terrainsDistanceMaxRendereb = devices->getGUIEnvironment()->addEditBox(L"180.0", rect<s32>(150, 210, 290, 230), true, addOctTreeWindow, -1);

		devices->getGUIEnvironment()->addStaticText(L"Distance Load :", rect<s32>(10, 230, 150, 250), true, false, addOctTreeWindow, -1, false);
		terrainsDistanceLoadeb = devices->getGUIEnvironment()->addEditBox(L"1.38", rect<s32>(150, 230, 290, 250), true, addOctTreeWindow, -1);

		devices->getGUIEnvironment()->addStaticText(L"Distance Load :", rect<s32>(10, 250, 150, 270), true, false, addOctTreeWindow, -1, false);
		terrainsDistanceUnloadeb = devices->getGUIEnvironment()->addEditBox(L"1.05", rect<s32>(150, 250, 290, 270), true, addOctTreeWindow, -1);

		terrainsEnableFog = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(10, 270, 290, 290), addOctTreeWindow, -1, L"Enable Fog");

		terrainsTexturePath = devices->getGUIEnvironment()->addEditBox(L"myName", rect<s32>(10, 300, 190, 320), true, addOctTreeWindow, -1);
		terrainsSelectTexture =devices->getGUIEnvironment()->addButton(rect<s32>(190, 300, 290, 320), addOctTreeWindow, -1, L"Select Texture", L"Select the Height Map");

		//PLEASE ADD THE BIG TEXTURE

		accept = devices->getGUIEnvironment()->addButton(rect<s32>(10, 340, 110, 360), addOctTreeWindow, -1, L"Accept", 
														 L"Accept and add the selected mesh");
		close = devices->getGUIEnvironment()->addButton(rect<s32>(120, 340, 220, 360), addOctTreeWindow, -1, L"Cancel", 
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

			if (event.GUIEvent.Caller == terrainsOpenHeightMap) {
				openTerrainsHeightMap = devices->createFileOpenDialog(L"Select the height map", 0);
				openTerrainsHeightMap->setMaxSize(dimension2du(1280, 800));
			}

			if (event.GUIEvent.Caller == terrainsSelectTexture) {
				openTerrainsTexture = devices->createFileOpenDialog(L"Select the height map", 0);
				openTerrainsTexture->setMaxSize(dimension2du(1280, 800));
			}

			if (event.GUIEvent.Caller == accept) {
                    IMesh *octTreeMesh = 0;
                    ISceneNode *octTreeNode = 0;
					STerrainsData tdata;

					//IF MESH
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
						if (octTreeNode)
							devices->getCollisionManager()->setCollisionToAnOctTreeNode(octTreeNode);
					}
                    
					//IF TERRAIN
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

						if (octTreeNode) {
							devices->getCollisionManager()->setCollisionToAnOctTreeNode(octTreeNode);
							octTreeNode->setPosition(vector3df(0) - ((ITerrainSceneNode *)octTreeNode)->getTerrainCenter());
						}
                    }

					//IF TERRAIN SPLATTING
					if (terrainType == ELTT_TERRAIN_SPLATTING) {
						if (path_terrains_hgtmap != L"" && path_terrains_texture != L"") {
							stringc hgtmapDir = devices->getDevice()->getFileSystem()->getFileDir(path_terrains_hgtmap.c_str());
							hgtmapDir += "/";
							devices->getDevice()->getFileSystem()->changeWorkingDirectoryTo(hgtmapDir);
							printf("%s\n", hgtmapDir.c_str());
							#ifndef _IRR_OSX_PLATFORM
								system("mkdir heightmaps");
								system("mkdir textures");
							#else

							#endif
							c8 *terrain_height_map_c8 = strdup(stringc(path_terrains_hgtmap).c_str());
							c8 *terrain_texture_c8 = strdup(stringc(path_terrains_texture).c_str());
							createTilesForImage(terrain_height_map_c8, "heightmaps/map", devices->getVideoDriver(), true);
							createTilesForImage(terrain_texture_c8, "textures/color", devices->getVideoDriver(), false);

							array<stringc> listeMap;
							listeMap.push_back("heightmaps/map13.png");
							listeMap.push_back("heightmaps/map14.png"); 
							listeMap.push_back("heightmaps/map15.png");
							listeMap.push_back("heightmaps/map16.png");
							listeMap.push_back("heightmaps/map9.png");
							listeMap.push_back("heightmaps/map10.png");
							listeMap.push_back("heightmaps/map11.png");
							listeMap.push_back("heightmaps/map12.png");
							listeMap.push_back("heightmaps/map5.png");
							listeMap.push_back("heightmaps/map6.png");
							listeMap.push_back("heightmaps/map7.png");
							listeMap.push_back("heightmaps/map8.png");
							listeMap.push_back("heightmaps/map1.png");
							listeMap.push_back("heightmaps/map2.png");
							listeMap.push_back("heightmaps/map3.png");
							listeMap.push_back("heightmaps/map4.png");
							array<stringc> colorMap;
							colorMap.push_back("textures/color13.png");
							colorMap.push_back("textures/color14.png");
							colorMap.push_back("textures/color15.png");
							colorMap.push_back("textures/color16.png");
							colorMap.push_back("textures/color9.png");
							colorMap.push_back("textures/color10.png");
							colorMap.push_back("textures/color11.png");
							colorMap.push_back("textures/color12.png");
							colorMap.push_back("textures/color5.png");
							colorMap.push_back("textures/color6.png");
							colorMap.push_back("textures/color7.png");
							colorMap.push_back("textures/color8.png");
							colorMap.push_back("textures/color1.png");
							colorMap.push_back("textures/color2.png");
							colorMap.push_back("textures/color3.png");
							colorMap.push_back("textures/color4.png");
							STerrainParameters param2;
							param2.MapSize = devices->getCore()->getF32(terrainsMapSizeeb->getText());
							param2.Dimension = dimension2di(devices->getCore()->getS32(terrainsDimensioneb->getText()),
															devices->getCore()->getS32(terrainsDimensioneb->getText()));
							param2.LodTextureScale = devices->getCore()->getF32(terrainsLODTextureScaleeb->getText());
							param2.MapListFileName = listeMap;
							param2.ColorMapListFileName = colorMap;
							param2.Position = core::vector3df(0,0,0);
							param2.Scale = core::vector3df(devices->getCore()->getF32(terrainsScale->getText()),
														   devices->getCore()->getF32(terrainsScale->getText()),
														   devices->getCore()->getF32(terrainsScale->getText()));
							param2.Quality = CTerrain::ETQ_MEDIUM;
							param2.DistanceMaxRender = devices->getCore()->getF32(terrainsDistanceMaxRendereb->getText());
							param2.DistanceLoad = param2.MapSize * devices->getCore()->getF32(terrainsDistanceLoadeb->getText()) * param2.Scale.X;
							param2.DistanceUnload = param2.DistanceLoad * devices->getCore()->getF32(terrainsDistanceUnloadeb->getText());
							param2.Fog = terrainsEnableFog->isChecked();	
							param2.Debug = true;
							param2.detailTexturePath = "detail.jpg";
							octTreeNode = new  CTerrainPager(param2, devices->getSceneManager()->getRootSceneNode(), devices->getSceneManager(), -1, 
																		 devices->getCollisionManager()->getMetaTriangleSelectors());

							f32 minFog = 400;
							f32 maxFog = param2.DistanceMaxRender * param2.Scale.X * 0.85f;
							video::SColor fogColor = video::SColor(0,150,150,255);
							devices->getVideoDriver()->setFog(fogColor, EFT_FOG_EXP2, minFog, maxFog, 0.00999999999, false, true);
						} else {
							devices->addWarningDialog(L"Informations", 
                                                  L"Please select an Height Map \n\n"
                                                  L"Please select a Texture \n", EMBF_OK);
						}
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
			if (event.GUIEvent.Caller == openTerrainsHeightMap) {
				path_terrains_hgtmap = L"";
                path_terrains_hgtmap.append(openTerrainsHeightMap->getFileName());
			}
			if (event.GUIEvent.Caller == openTerrainsTexture) {
				path_terrains_texture = L"";
                path_terrains_texture.append(openTerrainsTexture->getFileName());
			}
        }
    }
    
    return false;
}
