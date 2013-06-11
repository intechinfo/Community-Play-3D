//
//  CImporter.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 01/10/12.
//
//

#include "stdafx.h"
#include "CImporter.h"

CImporter::CImporter(CDevices *_devices) {
    devices = _devices;

	smgr = devices->getSceneManager();
}

CImporter::~CImporter() {
    devices = 0;
}

void CImporter::readWithNextElement(std::string node, std::string nextNode) {
    xmlReader->read();
    element = xmlReader->getNodeName();
    if (element != node && element != nextNode) {
        while (xmlReader && element != node && element != nextNode && xmlReader->read()) {
            element = xmlReader->getNodeName();
            printf("current element : %s\n", element.c_str());
        }
    }
}

void CImporter::read(std::string node) {
    xmlReader->read();
    element = xmlReader->getNodeName();
    if (element != node) {
        while (xmlReader && element != node && xmlReader->read()) {
            element = xmlReader->getNodeName();
            printf("current element : %s\n", element.c_str());
        }
    }
}

void CImporter::importScene(stringc file_path) {
    stringc wd = devices->getWorkingDirectory().c_str();
    wd += "/";
    devices->getDevice()->getFileSystem()->changeWorkingDirectoryTo(wd.c_str());
    
    xmlReader = createIrrXMLReader(file_path.c_str());
	if (!xmlReader)
		return;
    
    devices->getCoreData()->clearAllTheArrays();
    
	newImportScene(file_path);
	return;

    if (xmlReader) {
        read("rootScene");
    }
    
    while (xmlReader && xmlReader->read()) {
        
        element = "";
        
        if (xmlReader->getNodeType() == EXN_ELEMENT) {
            
            element = xmlReader->getNodeName();
            
            //------------------------------------------------------------------------------------------------------------------------------
            //------------------------------------------------------------------------------------------------------------------------------
            //------------------------------------------------------------------------------------------------------------------------------
            //IF CONFIG
            if (element == "config") {
                
                //IF GRID
                read("grid");
                read("accentLineOffset");
                u32 ALO = devices->getCore()->getU32(xmlReader->getAttributeValue("ALO"));
                devices->getObjectPlacement()->getGridSceneNode()->SetAccentlineOffset(ALO);
                
                read("size");
                u32 size = devices->getCore()->getU32(xmlReader->getAttributeValue("S"));
                devices->getObjectPlacement()->getGridSceneNode()->SetSize(size);
                
                read("spacing");
                u32 spacing = devices->getCore()->getU32(xmlReader->getAttributeValue("SP"));
                devices->getObjectPlacement()->getGridSceneNode()->SetSpacing(spacing);
                
                //IF CAMERA
                read("camera");
                read("position");
                vector3df position = vector3df(devices->getCore()->getF32(xmlReader->getAttributeValue("X")),
                                               devices->getCore()->getF32(xmlReader->getAttributeValue("Y")),
                                               devices->getCore()->getF32(xmlReader->getAttributeValue("Z")));
                devices->getMayaCamera()->setPosition(position);
                
                read("rotation");
                vector3df rotation = vector3df(devices->getCore()->getF32(xmlReader->getAttributeValue("X")),
                                               devices->getCore()->getF32(xmlReader->getAttributeValue("Y")),
                                               devices->getCore()->getF32(xmlReader->getAttributeValue("Z")));
                devices->getMayaCamera()->setRotation(rotation);
                
                //IF EFFECTS
                read("effect");
                readWithNextElement("postProcessingEffect", "effect");
                while (element == "postProcessingEffect") {
                    stringw path = L"";
                    read("file_path");
                    path = xmlReader->getAttributeValue("path");
                    
                    s32 render = devices->getXEffect()->addPostProcessingEffectFromFile(path.c_str());
                    devices->getCoreData()->getEffectRenders()->push_back(render);
                    devices->getCoreData()->getEffectRendersPaths()->push_back(path);
                    
                    CEffectRenderCallback *callback = new CEffectRenderCallback(render, devices->getDevice());
                    callback->clearPixelValues();
                    callback->clearVertexValues();
                    devices->getCoreData()->getEffectRenderCallbacks()->push_back(callback);
                    devices->getXEffect()->setPostProcessingRenderCallback(render, callback);
                    
                    read("values");
                    readWithNextElement("value", "values");
                    while (element == "value") {
                        stringw name = L"";
                        stringc value = "0.0";
                        
                        name = xmlReader->getAttributeValue("name");
                        value = xmlReader->getAttributeValue("val");
                        
                        callback->getPixelValues()->push_back(value);
                        callback->getPixelValuesNames()->push_back(name);
                        
                        readWithNextElement("value", "values");
                    }
                    read("postProcessingEffect");
                    readWithNextElement("postProcessingEffect", "effect");
                }

                //IF MATERIAL TYPES
                read("materialTypes");
                readWithNextElement("materialType", "materialTypes");
                while (element == "materialType") {
                    
                    read("pixelShaderType");
                    E_PIXEL_SHADER_TYPE pixelShaderType = (E_PIXEL_SHADER_TYPE)xmlReader->getAttributeValueAsInt("type");
                    read("vertexShaderType");
                    E_VERTEX_SHADER_TYPE vertexShaderType = (E_VERTEX_SHADER_TYPE)xmlReader->getAttributeValueAsInt("type");
                    
                    stringc name = "";
                    read("name");
                    name = xmlReader->getAttributeValue("cname");
                    
                    stringc vertex = "", pixel = "", constants = "";
                    read("vertex");
                    vertex = xmlReader->getAttributeValue("shader");
                    read("pixel");
                    pixel = xmlReader->getAttributeValue("shader");
                    read("constants");
                    constants = xmlReader->getAttributeValue("value");
                    
                    CShaderCallback *callback = new CShaderCallback();
                    callback->setName(name.c_str());
                    callback->setVertexShader(vertex.c_str());
                    callback->setPixelShader(pixel.c_str());
                    callback->setConstants(constants.c_str());
                    callback->setPixelShaderType(pixelShaderType);
                    callback->setVertexShaderType(vertexShaderType);
                    callback->setDevice(devices->getDevice());
                    callback->buildMaterial(devices->getVideoDriver());
                    devices->getCoreData()->getShaderCallbacks()->push_back(callback);
                    
                    read("materialType");
                    readWithNextElement("materialType", "materialTypes");
                }
                
                //IF OTHER TO COME
                
                read("config");
            }
            //END CONFIG
            //------------------------------------------------------------------------------------------------------------------------------
            //------------------------------------------------------------------------------------------------------------------------------
            //------------------------------------------------------------------------------------------------------------------------------
            //IF TERRAIN
            if (element == "terrain") {
                //PATH
                read("path");
                std::string path = xmlReader->getAttributeValue("file");
                io::path extension;
                core::getFileNameExtension(extension, path.c_str());
                extension.make_lower();
                
                IMesh *octTreeMesh;
                ISceneNode *octTreeNode;
                
                read("type");
                stringc esnt = xmlReader->getAttributeValue("esnt");
                
                if (extension == ".jpg" || extension == ".png" || extension == ".bmp") {
                    octTreeNode = devices->getSceneManager()->addTerrainSceneNode(path.c_str(), 0, -1, vector3df(0.f, 0.f, 0.f),
                                                                                  vector3df(0.f, 0.f, 0.f), vector3df(1.f, 0.1f, 1.f),
                                                                                  SColor (255, 255, 255, 255), 5, ETPS_17, 4);
					devices->getCoreData()->getTerrainMinPolysPerNode()->push_back(0);
                } else if (esnt == "octtree") {
                    octTreeMesh = devices->getSceneManager()->getMesh(path.c_str());
					devices->getCoreData()->getTerrainMeshes()->push_back(octTreeMesh);
					octTreeNode = devices->getSceneManager()->addOctreeSceneNode(octTreeMesh, 0, -1, xmlReader->getAttributeValueAsInt("mppn"));
					devices->getCoreData()->getTerrainMinPolysPerNode()->push_back(xmlReader->getAttributeValueAsInt("mppn"));
                } else if (esnt == "mesh") {
                    octTreeMesh = devices->getSceneManager()->getMesh(path.c_str());
					if (xmlReader->getAttributeValueAsInt("tangents") == 1) {
						octTreeMesh = devices->getSceneManager()->getMeshManipulator()->createMeshWithTangents(octTreeMesh, false, true, false, true);
					}
					devices->getCoreData()->getTerrainMeshes()->push_back(octTreeMesh);
					octTreeNode = devices->getSceneManager()->addMeshSceneNode(octTreeMesh, 0, -1);
					devices->getCoreData()->getTerrainMinPolysPerNode()->push_back(0);
				}

				read("factory");
				if (element == "factory") {
					readWithNextElement("primitive", "factory");
					while (element == "primitive") {
						stringc primitiveType = xmlReader->getAttributeValue("type");
						stringc option = xmlReader->getAttributeValue("options");
						if (primitiveType == "planar") {
							read("resolutionS");
							f32 resolutionS = xmlReader->getAttributeValueAsFloat("value");
							if (option == "general") {
								devices->getSceneManager()->getMeshManipulator()->makePlanarTextureMapping(octTreeMesh, xmlReader->getAttributeValueAsFloat("value"));
								SPlanarTextureMappingData ptmd(resolutionS, 0, 0, vector3df(0), true);
								devices->getCoreData()->getPlanarTextureMappingValues()->push_back(ptmd);
							}
							devices->getCoreData()->getPlanarMeshes()->push_back(octTreeNode);
						}
						readWithNextElement("primitive", "factory");
					}
				}

                //NAME
                if (octTreeNode) {
                    read("name");
                    std::string name = xmlReader->getAttributeValue("c8name");
                    c8 *c8name = (c8*)name.c_str();
                    octTreeNode->setName(c8name);
                    
                    //MATERIALS
                    read("materials");
                    read("material");
                    while (element == "material") {
                        u32 id = devices->getCore()->getU32(xmlReader->getAttributeValue("id"));
                        stringc texture1 = xmlReader->getAttributeValue("texture1");
                        stringc texture2 = xmlReader->getAttributeValue("texture2");
                        stringc texture3 = xmlReader->getAttributeValue("texture3");
                        stringc texture4 = xmlReader->getAttributeValue("texture4");
                        
                        ITexture *textures[4];
                        textures[0] = devices->getVideoDriver()->getTexture(texture1.c_str());
                        textures[1] = devices->getVideoDriver()->getTexture(texture2.c_str());
                        textures[2] = devices->getVideoDriver()->getTexture(texture3.c_str());
                        textures[3] = devices->getVideoDriver()->getTexture(texture4.c_str());
                        
                        if (octTreeNode) {
                            for (int i=0; i < 4; i++) {
                                octTreeNode->getMaterial(id).setTexture(i, textures[i]);
                            }
                        }
                        
                        octTreeNode->getMaterial(id).DiffuseColor.set(devices->getCore()->getU32(xmlReader->getAttributeValue("dca")),
                                                                      devices->getCore()->getU32(xmlReader->getAttributeValue("dcr")),
                                                                      devices->getCore()->getU32(xmlReader->getAttributeValue("dcg")),
                                                                      devices->getCore()->getU32(xmlReader->getAttributeValue("dcb")));
                        octTreeNode->getMaterial(id).AmbientColor.set(devices->getCore()->getU32(xmlReader->getAttributeValue("aca")),
                                                                      devices->getCore()->getU32(xmlReader->getAttributeValue("acr")),
                                                                      devices->getCore()->getU32(xmlReader->getAttributeValue("acg")),
                                                                      devices->getCore()->getU32(xmlReader->getAttributeValue("acb")));
                        octTreeNode->getMaterial(id).SpecularColor.set(devices->getCore()->getU32(xmlReader->getAttributeValue("sca")),
                                                                       devices->getCore()->getU32(xmlReader->getAttributeValue("scr")),
                                                                       devices->getCore()->getU32(xmlReader->getAttributeValue("scg")),
                                                                       devices->getCore()->getU32(xmlReader->getAttributeValue("scb")));
                        octTreeNode->getMaterial(id).EmissiveColor.set(devices->getCore()->getU32(xmlReader->getAttributeValue("eca")),
                                                                       devices->getCore()->getU32(xmlReader->getAttributeValue("ecr")),
                                                                       devices->getCore()->getU32(xmlReader->getAttributeValue("ecg")),
                                                                       devices->getCore()->getU32(xmlReader->getAttributeValue("ecb")));
                        bool lighting = false;
                        int lighting_int = devices->getCore()->getU32(xmlReader->getAttributeValue("lighting"));
                        if (lighting_int == 1) {
                            lighting = true;
                        }
                        octTreeNode->getMaterial(id).Lighting = lighting;
                        
                        if (devices->getCore()->getS32(xmlReader->getAttributeValue("materialType")) < 0) {
                            s32 mts32 = (s32)devices->getCore()->getS32(xmlReader->getAttributeValue("materialType"));
                            octTreeNode->getMaterial(id).MaterialType = (E_MATERIAL_TYPE)devices->getCoreData()->getShaderCallbacks()->operator[](-(mts32+1))->getMaterial();
                        } else {
                            CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
                            E_MATERIAL_TYPE mt;
                            mt = editNode->getMaterialType((s32)devices->getCore()->getU32(xmlReader->getAttributeValue("materialType")));
                            octTreeNode->getMaterial(id).MaterialType = mt;
                            delete editNode;
                        }
                        
                        readWithNextElement("material", "materials");
                    }
                    
                    //POSITION
                    read("position");
                    if (element == "position") {
                        vector3df position = vector3df(devices->getCore()->getF32(xmlReader->getAttributeValue("X")),
                                                       devices->getCore()->getF32(xmlReader->getAttributeValue("Y")),
                                                       devices->getCore()->getF32(xmlReader->getAttributeValue("Z")));
                        octTreeNode->setPosition(position);
                    }
                    
                    //ROTATION
                    read("rotation");
                    if (element == "rotation") {
                        vector3df rotation = vector3df(devices->getCore()->getF32(xmlReader->getAttributeValue("X")),
                                                       devices->getCore()->getF32(xmlReader->getAttributeValue("Y")),
                                                       devices->getCore()->getF32(xmlReader->getAttributeValue("Z")));
                        octTreeNode->setRotation(rotation);
                    }
                    
                    //SCALE
                    read("scale");
                    if (element == "scale") {
                        vector3df scale = vector3df(devices->getCore()->getF32(xmlReader->getAttributeValue("X")),
                                                    devices->getCore()->getF32(xmlReader->getAttributeValue("Y")),
                                                    devices->getCore()->getF32(xmlReader->getAttributeValue("Z")));
                        octTreeNode->setScale(scale);
                    }

					//IS VISIBLE
					read("visible");
					if (element == "visible") {
						octTreeNode->setVisible(xmlReader->getAttributeValueAsInt("bool"));
					}

					//SHADOW MODE
					read("shadowMode");
					E_SHADOW_MODE shadowMode = ESM_EXCLUDE;
					if (element == "shadowMode") {
						shadowMode = (E_SHADOW_MODE)xmlReader->getAttributeValueAsInt("mode");
					}
                    
                    //octTreeNode->setMaterialFlag(EMF_NORMALIZE_NORMALS, false);
                    devices->getXEffect()->addShadowToNode(octTreeNode, devices->getXEffectFilterType(), shadowMode);
                    devices->getCollisionManager()->setCollisionToAnOctTreeNode(octTreeNode);
                    devices->getCoreData()->getTerrainNodes()->push_back(octTreeNode);
                    devices->getCoreData()->getTerrainPaths()->push_back(path.c_str());

					//ONLY FOR RE5 VILLAGE MESH
					for (u32 mtli=0; mtli < octTreeNode->getMaterialCount(); mtli++) {
						octTreeNode->getMaterial(mtli).EmissiveColor = SColor(255, 255, 255, 255);
					}
                }
            }
            //END IF TERRAIN
            //------------------------------------------------------------------------------------------------------------------------------
            //------------------------------------------------------------------------------------------------------------------------------
            //------------------------------------------------------------------------------------------------------------------------------
            //IF TREE
            if (element == "tree") {
                //PATH
                read("path");
                std::string path = xmlReader->getAttributeValue("file");
                io::path extension;
                core::getFileNameExtension(extension, path.c_str());
                extension.make_lower();
                
                IMesh *octTreeMesh;
                ISceneNode *octTreeNode;
                
                octTreeMesh = devices->getSceneManager()->getMesh(path.c_str());
                octTreeNode = devices->getSceneManager()->addOctreeSceneNode(octTreeMesh, 0, -1, 1024);

                //NAME
                read("name");
                std::string name = xmlReader->getAttributeValue("c8name");
                c8 *c8name = (c8*)name.c_str();
                octTreeNode->setName(c8name);
                
                //MATERIALS
                read("materials");
                read("material");
                while (element == "material") {
                    u32 id = devices->getCore()->getU32(xmlReader->getAttributeValue("id"));
                    stringc texture1 = xmlReader->getAttributeValue("texture1");
                    stringc texture2 = xmlReader->getAttributeValue("texture2");
                    stringc texture3 = xmlReader->getAttributeValue("texture3");
                    stringc texture4 = xmlReader->getAttributeValue("texture4");
                    
                    ITexture *textures[4];
                    textures[0] = devices->getVideoDriver()->getTexture(texture1.c_str());
                    textures[1] = devices->getVideoDriver()->getTexture(texture2.c_str());
                    textures[2] = devices->getVideoDriver()->getTexture(texture3.c_str());
                    textures[3] = devices->getVideoDriver()->getTexture(texture4.c_str());
                    
                    if (octTreeNode) {
                        for (int i=0; i < 4; i++) {
                            octTreeNode->getMaterial(id).setTexture(i, textures[i]);
                        }
                    }
                    
                    octTreeNode->getMaterial(id).DiffuseColor.set(devices->getCore()->getU32(xmlReader->getAttributeValue("dca")),
                                                                  devices->getCore()->getU32(xmlReader->getAttributeValue("dcr")),
                                                                  devices->getCore()->getU32(xmlReader->getAttributeValue("dcg")),
                                                                  devices->getCore()->getU32(xmlReader->getAttributeValue("dcb")));
                    octTreeNode->getMaterial(id).AmbientColor.set(devices->getCore()->getU32(xmlReader->getAttributeValue("aca")),
                                                                  devices->getCore()->getU32(xmlReader->getAttributeValue("acr")),
                                                                  devices->getCore()->getU32(xmlReader->getAttributeValue("acg")),
                                                                  devices->getCore()->getU32(xmlReader->getAttributeValue("acb")));
                    octTreeNode->getMaterial(id).SpecularColor.set(devices->getCore()->getU32(xmlReader->getAttributeValue("sca")),
                                                                   devices->getCore()->getU32(xmlReader->getAttributeValue("scr")),
                                                                   devices->getCore()->getU32(xmlReader->getAttributeValue("scg")),
                                                                   devices->getCore()->getU32(xmlReader->getAttributeValue("scb")));
                    octTreeNode->getMaterial(id).EmissiveColor.set(devices->getCore()->getU32(xmlReader->getAttributeValue("eca")),
                                                                   devices->getCore()->getU32(xmlReader->getAttributeValue("ecr")),
                                                                   devices->getCore()->getU32(xmlReader->getAttributeValue("ecg")),
                                                                   devices->getCore()->getU32(xmlReader->getAttributeValue("ecb")));
                    bool lighting = false;
                    int lighting_int = devices->getCore()->getU32(xmlReader->getAttributeValue("lighting"));
                    if (lighting_int == 1) {
                        lighting = true;
                    }
                    octTreeNode->getMaterial(id).Lighting = lighting;
                    
                    CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
                    E_MATERIAL_TYPE mt = editNode->getMaterialType((s32)devices->getCore()->getU32(xmlReader->getAttributeValue("materialType")));
                    delete editNode;
                    octTreeNode->getMaterial(id).MaterialType = mt;
                    
                    readWithNextElement("material", "materials");
                }
                
                //POSITION
                read("position");
                if (element == "position") {
                    vector3df position = vector3df(devices->getCore()->getF32(xmlReader->getAttributeValue("X")),
                                                   devices->getCore()->getF32(xmlReader->getAttributeValue("Y")),
                                                   devices->getCore()->getF32(xmlReader->getAttributeValue("Z")));
                    octTreeNode->setPosition(position);
                }
                
                //ROTATION
                read("rotation");
                if (element == "rotation") {
                    vector3df rotation = vector3df(devices->getCore()->getF32(xmlReader->getAttributeValue("X")),
                                                   devices->getCore()->getF32(xmlReader->getAttributeValue("Y")),
                                                   devices->getCore()->getF32(xmlReader->getAttributeValue("Z")));
                    octTreeNode->setRotation(rotation);
                }
                
                //SCALE
                read("scale");
                if (element == "scale") {
                    vector3df scale = vector3df(devices->getCore()->getF32(xmlReader->getAttributeValue("X")),
                                                devices->getCore()->getF32(xmlReader->getAttributeValue("Y")),
                                                devices->getCore()->getF32(xmlReader->getAttributeValue("Z")));
                    octTreeNode->setScale(scale);
                }
                
                octTreeNode->setMaterialFlag(EMF_NORMALIZE_NORMALS, false);
                devices->getXEffect()->addShadowToNode(octTreeNode, devices->getXEffectFilterType());
                devices->getCollisionManager()->setCollisionToAnOctTreeNode(octTreeNode);
				STreesData tdata(octTreeMesh, octTreeNode, path.c_str());
                devices->getCoreData()->getTreesData()->push_back(tdata);
            }
            //END IF TREE
            //------------------------------------------------------------------------------------------------------------------------------
            //------------------------------------------------------------------------------------------------------------------------------
            //------------------------------------------------------------------------------------------------------------------------------
            //IF OBJECT
            if (element == "object") {
                //PATH
                read("path");
                std::string path = xmlReader->getAttributeValue("file");
                
                IAnimatedMesh *animatedMesh;
                IAnimatedMeshSceneNode *animatedNode;
				bool isFromNodesFactory = true;
                
				if (path == "cube") {
					IMeshSceneNode *animatedNodeNode = devices->getSceneManager()->addCubeSceneNode();
					animatedNode = (IAnimatedMeshSceneNode *)animatedNodeNode;
					IMesh *animatedMeshMesh = animatedNodeNode->getMesh();
					animatedMesh = (IAnimatedMesh *)animatedMeshMesh;
				} else if (path == "sphere") {
					ISceneNode *animatedNodeNode = devices->getSceneManager()->addSphereSceneNode();
					animatedNode = (IAnimatedMeshSceneNode *)animatedNodeNode;
				} else if (path == "hillPlaneMesh") {
					IAnimatedMesh *animatedMeshMesh = devices->getSceneManager()->addHillPlaneMesh("#new_hille_plane_mesh", 
																				dimension2df(25, 25), dimension2du(25, 25),
																				0, 0, dimension2df(0.f, 0.f), dimension2df(20.f, 20.f));
					IAnimatedMeshSceneNode *animatedNodeNode = devices->getSceneManager()->addAnimatedMeshSceneNode(animatedMeshMesh);
					animatedNode = animatedNodeNode;
					animatedMesh = animatedMeshMesh;
				} else if (path == "billboard") {
					IBillboardSceneNode *animatedNodeNode = devices->getSceneManager()->addBillboardSceneNode();
					animatedNode = (IAnimatedMeshSceneNode *)animatedNodeNode;
					animatedMesh = 0;
				} else {
					animatedMesh = devices->getSceneManager()->getMesh(path.c_str());
					animatedNode = devices->getSceneManager()->addAnimatedMeshSceneNode(animatedMesh);
					animatedNode->setAnimationSpeed(0);
					animatedNode->setFrameLoop(0, 0);
					isFromNodesFactory = false;
				}
                
                //NAME
                read("name");
                std::string name = xmlReader->getAttributeValue("c8name");
                c8 *c8name = (c8*)name.c_str();
                animatedNode->setName(c8name);
                
                //MATERIALS
                read("materials");
                read("material");
                while (element == "material") {
                    u32 id = devices->getCore()->getU32(xmlReader->getAttributeValue("id"));
                    stringc texture1 = xmlReader->getAttributeValue("texture1");
                    stringc texture2 = xmlReader->getAttributeValue("texture2");
                    stringc texture3 = xmlReader->getAttributeValue("texture3");
                    stringc texture4 = xmlReader->getAttributeValue("texture4");
                    
                    ITexture *textures[4];
                    textures[0] = devices->getVideoDriver()->getTexture(texture1.c_str());
                    textures[1] = devices->getVideoDriver()->getTexture(texture2.c_str());
                    textures[2] = devices->getVideoDriver()->getTexture(texture3.c_str());
                    textures[3] = devices->getVideoDriver()->getTexture(texture4.c_str());
                    
                    if (animatedNode) {
                        for (int i=0; i < 4; i++) {
                            animatedNode->getMaterial(id).setTexture(i, textures[i]);
                        }
                    }
                    
                    animatedNode->getMaterial(id).DiffuseColor.set(devices->getCore()->getU32(xmlReader->getAttributeValue("dca")),
                                                                  devices->getCore()->getU32(xmlReader->getAttributeValue("dcr")),
                                                                  devices->getCore()->getU32(xmlReader->getAttributeValue("dcg")),
                                                                  devices->getCore()->getU32(xmlReader->getAttributeValue("dcb")));
                    animatedNode->getMaterial(id).AmbientColor.set(devices->getCore()->getU32(xmlReader->getAttributeValue("aca")),
                                                                  devices->getCore()->getU32(xmlReader->getAttributeValue("acr")),
                                                                  devices->getCore()->getU32(xmlReader->getAttributeValue("acg")),
                                                                  devices->getCore()->getU32(xmlReader->getAttributeValue("acb")));
                    animatedNode->getMaterial(id).SpecularColor.set(devices->getCore()->getU32(xmlReader->getAttributeValue("sca")),
                                                                   devices->getCore()->getU32(xmlReader->getAttributeValue("scr")),
                                                                   devices->getCore()->getU32(xmlReader->getAttributeValue("scg")),
                                                                   devices->getCore()->getU32(xmlReader->getAttributeValue("scb")));
                    animatedNode->getMaterial(id).EmissiveColor.set(devices->getCore()->getU32(xmlReader->getAttributeValue("eca")),
                                                                   devices->getCore()->getU32(xmlReader->getAttributeValue("ecr")),
                                                                   devices->getCore()->getU32(xmlReader->getAttributeValue("ecg")),
                                                                   devices->getCore()->getU32(xmlReader->getAttributeValue("ecb")));
                    bool lighting = false;
                    int lighting_int = devices->getCore()->getU32(xmlReader->getAttributeValue("lighting"));
                    if (lighting_int == 1) {
                        lighting = true;
                    }
                    animatedNode->getMaterial(id).Lighting = lighting;
                    
                    E_MATERIAL_TYPE mt;
                    
                    if (devices->getCore()->getU32(xmlReader->getAttributeValue("materialType")) > 24) {
                        s32 mts32 = (s32)devices->getCore()->getU32(xmlReader->getAttributeValue("materialType"));
                        bool founded = false;
                        int i=0;
                        while (!founded && i < devices->getCoreData()->getShaderCallbacks()->size()) {
                            if (devices->getCoreData()->getShaderCallbacks()->operator[](i)->getMaterial() == mts32) {
                                mt = (E_MATERIAL_TYPE)devices->getCoreData()->getShaderCallbacks()->operator[](i)->getMaterial();
                                founded = true;
                            }
                            i++;
                        }
                    } else {
                        CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
                        mt = editNode->getMaterialType((s32)devices->getCore()->getU32(xmlReader->getAttributeValue("materialType")));
                        delete editNode;
                    }
                    
                    animatedNode->getMaterial(id).MaterialType = mt;
                    
                    readWithNextElement("material", "materials");
                }
                
                //POSITION
                read("position");
                if (element == "position") {
                    vector3df position = vector3df(devices->getCore()->getF32(xmlReader->getAttributeValue("X")),
                                                   devices->getCore()->getF32(xmlReader->getAttributeValue("Y")),
                                                   devices->getCore()->getF32(xmlReader->getAttributeValue("Z")));
                    animatedNode->setPosition(position);
                }
                
                //ROTATION
                read("rotation");
                if (element == "rotation") {
                    vector3df rotation = vector3df(devices->getCore()->getF32(xmlReader->getAttributeValue("X")),
                                                   devices->getCore()->getF32(xmlReader->getAttributeValue("Y")),
                                                   devices->getCore()->getF32(xmlReader->getAttributeValue("Z")));
                    animatedNode->setRotation(rotation);
                }
                
                //SCALE
                read("scale");
                if (element == "scale") {
                    vector3df scale = vector3df(devices->getCore()->getF32(xmlReader->getAttributeValue("X")),
                                                devices->getCore()->getF32(xmlReader->getAttributeValue("Y")),
                                                devices->getCore()->getF32(xmlReader->getAttributeValue("Z")));
					if (animatedNode->getType() != ESNT_BILLBOARD) {
						animatedNode->setScale(scale);
					} else {
						((IBillboardSceneNode *)animatedNode)->setSize(dimension2d<f32>(scale.X, scale.Y));
						animatedNode->setScale(scale);
					}
                }
                
                animatedNode->setMaterialFlag(EMF_NORMALIZE_NORMALS, false);
                devices->getXEffect()->addShadowToNode(animatedNode, devices->getXEffectFilterType());
				if (!isFromNodesFactory) {
					devices->getCollisionManager()->setCollisionToAnAnimatedNode(animatedNode);
				} else {
					devices->getCollisionManager()->setCollisionFromBoundingBox(animatedNode);
				}
				SObjectsData odata(animatedMesh, animatedNode, path.c_str());
				devices->getCoreData()->getObjectsData()->push_back(odata);
            }
            //END IF OBJECT
            //------------------------------------------------------------------------------------------------------------------------------
            //------------------------------------------------------------------------------------------------------------------------------
            //------------------------------------------------------------------------------------------------------------------------------
            //IF LIGHT
            if (element == "light") {
                ISceneNode *light = devices->getSceneManager()->addLightSceneNode();
                SShadowLight shadowLight = SShadowLight(1024, vector3df(0,0,0), vector3df(0,0,0), 
                                                        SColor(255, 255, 255, 255), 20.0f, 5000.f, 89.99f * DEGTORAD);
                read("name");
                if (element == "name") {
                    light->setName(xmlReader->getAttributeValue("c8name"));
                }
                read("position");
                if (element == "position") {
                    vector3df position = vector3df(devices->getCore()->getF32(xmlReader->getAttributeValue("X")),
                                                   devices->getCore()->getF32(xmlReader->getAttributeValue("Y")),
                                                   devices->getCore()->getF32(xmlReader->getAttributeValue("Z")));
                    light->setPosition(position);
                }
                read("target");
                if (element == "target") {
                    vector3df target = vector3df(devices->getCore()->getF32(xmlReader->getAttributeValue("X")),
                                                   devices->getCore()->getF32(xmlReader->getAttributeValue("Y")),
                                                   devices->getCore()->getF32(xmlReader->getAttributeValue("Z")));
                    light->setRotation(target);
                }
                read("diffuseColor");
                if (element == "diffuseColor") {
                    SColorf color = SColorf(devices->getCore()->getF32(xmlReader->getAttributeValue("R")),
                                          devices->getCore()->getF32(xmlReader->getAttributeValue("G")),
                                          devices->getCore()->getF32(xmlReader->getAttributeValue("B")), 255.f);
                    ((ILightSceneNode *)light)->getLightData().DiffuseColor = color;
                }
                read("radius");
                if (element == "radius") {
                    f32 radius = devices->getCore()->getF32(xmlReader->getAttributeValue("value"));
                    ((ILightSceneNode *)light)->setRadius(radius);
                }
				read("farValue");
				if (element == "farValue") {
					f32 farValue = devices->getCore()->getF32(xmlReader->getAttributeValue("value"));
					shadowLight.setFarValue(farValue);
				}
                read("shadows");
                if (element == "shadows") {
                    u32 resol = devices->getCore()->getU32(xmlReader->getAttributeValue("resol"));
                    shadowLight.setShadowMapResolution(resol);
                }

				SLightsData ldata(light);

				read("lensFlare");
				if (element == "lensFlare") {
					readWithNextElement("mesh", "lensFlare");
					if (element == "mesh") {
						//SETUP NODES
						IMeshSceneNode* meshNode = devices->getSceneManager()->addSphereSceneNode(1, 16, devices->getSceneManager()->getRootSceneNode());
						meshNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
						meshNode->setMaterialFlag(EMF_LIGHTING, false);
						meshNode->setScale(vector3d<f32>(0, 0, 0));
						meshNode->setParent(light);
						meshNode->setName(stringc(stringc(light->getName()) + stringc("_flare_mesh")).c_str());

						IBillboardSceneNode* bill = devices->getSceneManager()->addBillboardSceneNode(meshNode);
						bill->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
						bill->setMaterialFlag(EMF_LIGHTING, false);
						bill->setSize(dimension2d<f32>(0, 0));
						bill->setParent(meshNode);
						bill->setName(stringc(stringc(light->getName()) + stringc("_flare_bill")).c_str());

						CLensFlareSceneNode* lensFlareNode = new CLensFlareSceneNode(meshNode, devices->getSceneManager());
						lensFlareNode->setFalseOcclusion(true);
						lensFlareNode->setParent(meshNode);
						lensFlareNode->setName(stringc(stringc(light->getName()) + stringc("_flare_node")).c_str());

						//CONFIGURE NODES
						//MESH
						read("scale");
						if (element == "scale") {
							vector3df scale = vector3df(devices->getCore()->getF32(xmlReader->getAttributeValue("X")),
														devices->getCore()->getF32(xmlReader->getAttributeValue("Y")),
														devices->getCore()->getF32(xmlReader->getAttributeValue("Z")));
							meshNode->setScale(scale);
						}
						read("texture");
						if (element == "texture") {
							ITexture *tex = devices->getVideoDriver()->getTexture(xmlReader->getAttributeValue("path"));
							meshNode->setMaterialTexture(0, tex);
						}
						//BILLBOARD
						read("size");
						if (element == "size") {
							dimension2df size = dimension2df(devices->getCore()->getF32(xmlReader->getAttributeValue("Width")),
															 devices->getCore()->getF32(xmlReader->getAttributeValue("Height")));
							bill->setSize(size);
						}
						read("texture");
						if (element == "texture") {
							ITexture *tex = devices->getVideoDriver()->getTexture(xmlReader->getAttributeValue("path"));
							bill->setMaterialTexture(0, tex);
						}
						//LENS FLARE SCENE NODE
						read("strength");
						if (element == "strength") {
							f32 strength = devices->getCore()->getF32(xmlReader->getAttributeValue("value"));
							lensFlareNode->setStrength(strength);
						}
						read("texture");
						if (element == "texture") {
							ITexture *tex = devices->getVideoDriver()->getTexture(xmlReader->getAttributeValue("path"));
							lensFlareNode->setMaterialTexture(0, tex);
						}
						read("falseOcclusion");
						if (element == "falseOcclusion") {
							lensFlareNode->setFalseOcclusion(devices->getCore()->getU32(xmlReader->getAttributeValue("value")));
						}

						read("position");
						if (element == "position") {
							vector3df position = vector3df(devices->getCore()->getF32(xmlReader->getAttributeValue("X")),
														   devices->getCore()->getF32(xmlReader->getAttributeValue("Y")),
														   devices->getCore()->getF32(xmlReader->getAttributeValue("Z")));
							meshNode->setPosition(position);
						}

						ldata.setLensFlareMeshSceneNode(meshNode);
						ldata.setLensFlareBillboardSceneNode(bill);
						ldata.setLensFlareSceneNode(lensFlareNode);
					}
				}

                devices->getXEffect()->addShadowLight(shadowLight);
                devices->getCoreData()->getLightsData()->push_back(ldata);
            }
            
        }
    }
    
    if (xmlReader) {
        read("rootScene");
    }
    
    delete xmlReader;

}

//---------------------------------------------------------------------------------------------
//-----------------------------------BUILDING NODES--------------------------------------------
//---------------------------------------------------------------------------------------------

void CImporter::buildTerrain() {
	read("path");
	stringc path = xmlReader->getAttributeValue("file");

	read("type");
	stringc type = xmlReader->getAttributeValue("esnt");

	IMesh *mesh = 0;
    ISceneNode *node = 0;
	u32 minPolysPerNode = 0;
	if (type == "heightMap") {
		node = smgr->addTerrainSceneNode(path.c_str(), 0, -1, vector3df(0.f, 0.f, 0.f),
                                                vector3df(0.f, 0.f, 0.f), vector3df(1.f, 0.1f, 1.f),
                                                SColor (255, 255, 255, 255), 5, ETPS_17, 4);
		devices->getCoreData()->getTerrainMinPolysPerNode()->push_back(0);
	} else if (type == "octtree") {
		minPolysPerNode = xmlReader->getAttributeValueAsInt("mppn");
		mesh = smgr->getMesh(path.c_str());
		node = smgr->addOctreeSceneNode(mesh, 0, -1, minPolysPerNode);
	} else if (type == "mesh") {
		mesh = devices->getSceneManager()->getMesh(path.c_str());
		if (xmlReader->getAttributeValueAsInt("tangents") == 1) {
			mesh = smgr->getMeshManipulator()->createMeshWithTangents(mesh, false, true, false, true);
		}
		node = smgr->addMeshSceneNode(mesh, 0, -1);
	}

	readFactory(node);

	if (node) {
		read("name");
		stringc name = xmlReader->getAttributeValue("c8name");
		node->setName(name.c_str());

		readMaterials(node);
		readTransformations(node);
		readViewModes(node);

        devices->getCollisionManager()->setCollisionToAnOctTreeNode(node);
		devices->getCoreData()->getTerrainMeshes()->push_back(mesh);
        devices->getCoreData()->getTerrainNodes()->push_back(node);
        devices->getCoreData()->getTerrainPaths()->push_back(path.c_str());
		devices->getCoreData()->getTerrainMinPolysPerNode()->push_back(minPolysPerNode);
	}
}

void CImporter::buildTree() {
	read("path");
	stringc path = xmlReader->getAttributeValue("file");

	IMesh *mesh = 0;
    ISceneNode *node = 0;

	mesh = smgr->getMesh(path.c_str());
	node = smgr->addOctreeSceneNode(mesh, 0, -1, 4096*16);

	if (node) {
		read("name");
		stringc name = xmlReader->getAttributeValue("c8name");
		node->setName(name.c_str());

		readMaterials(node);
		readTransformations(node);
		readViewModes(node);

		devices->getCollisionManager()->setCollisionFromBoundingBox(node);
        STreesData tdata(mesh, node, path, ESNT_OCTREE, 1024);
		devices->getCoreData()->getTreesData()->push_back(tdata);
	}
}

void CImporter::buildObject() {
	read("path");
	stringc path = xmlReader->getAttributeValue("file");

	IAnimatedMesh *mesh = 0;
	IAnimatedMeshSceneNode *node = 0;

	if (path == "cube") {
		node = (IAnimatedMeshSceneNode *)smgr->addCubeSceneNode();
	} else if (path == "sphere") {
		node = (IAnimatedMeshSceneNode *)smgr->addSphereSceneNode();
	} else if (path == "hillPlaneMesh") {
		mesh = smgr->addHillPlaneMesh("#new_hille_plane_mesh", 
									  dimension2df(25, 25), dimension2du(25, 25),
									  0, 0, dimension2df(0.f, 0.f), dimension2df(20.f, 20.f));
		node = smgr->addAnimatedMeshSceneNode(mesh);
	} else if (path == "billboard") {
		node = (IAnimatedMeshSceneNode *)smgr->addBillboardSceneNode();
	} else {
		mesh = smgr->getMesh(path.c_str());
		node = smgr->addAnimatedMeshSceneNode(mesh);
		if (node) {
			node->setAnimationSpeed(0);
			node->setFrameLoop(0, 0);
		}
	}

	if (node) {
		read("name");
		stringc name = xmlReader->getAttributeValue("c8name");
		node->setName(name.c_str());

		readMaterials(node);
		readTransformations(node);
		readViewModes(node);

		array<CAction *> actions;
		read("actions");
		readWithNextElement("action", "actions");
		while (element == "action") {
			CAction *action = new CAction();
			action->setXMLValues(xmlReader);
			actions.push_back(action);
			readWithNextElement("action", "actions");
		}

		if (path == "sphere") {
			devices->getCollisionManager()->setCollisionFromBoundingBox(node);
		} else if (node->getType() != ESNT_BILLBOARD) {
			devices->getCollisionManager()->setCollisionToAnAnimatedNode(node);
		}
        SObjectsData odata(mesh, node, path);
		odata.setActions(&actions);
		devices->getCoreData()->getObjectsData()->push_back(odata);
	}
}

void CImporter::buildLight() {
	ILightSceneNode *node = smgr->addLightSceneNode();
	SShadowLight shadowLight(1024, vector3df(0,0,0), vector3df(0,0,0), SColor(255, 255, 255, 255), 20.0f, 5000.f, 89.99f * DEGTORAD, false);

	read("name");
	node->setName(xmlReader->getAttributeValue("c8name"));

	//TRANSFORMATIONS
	read("position");
	vector3df position = buildVector3df();
    node->setPosition(position);
    read("target");
    vector3df rotation = buildVector3df();
    node->setRotation(rotation);

	//PARAMETERS
	read("diffuseColor");
	node->getLightData().DiffuseColor = buildSColorf();
	read("ambiantColor");
	node->getLightData().AmbientColor = buildSColorf();
	read("specularColor");
	node->getLightData().SpecularColor = buildSColorf();

	read("radius");
	node->setRadius(xmlReader->getAttributeValueAsFloat("value"));
	read("farValue");
	shadowLight.setFarValue(xmlReader->getAttributeValueAsFloat("value"));

	read("shadows");
	shadowLight.setShadowMapResolution(xmlReader->getAttributeValueAsFloat("resol"));

	IMeshSceneNode* lfMeshNode = 0;
	IBillboardSceneNode *lfBillBoard = 0;
	CLensFlareSceneNode *lfNode = 0;
	read("lensFlare");
	readWithNextElement("mesh", "lensFlare");
	if (element == "mesh") {
		lfMeshNode = devices->getSceneManager()->addSphereSceneNode(1, 16, devices->getSceneManager()->getRootSceneNode());
		lfMeshNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
		lfMeshNode->setMaterialFlag(EMF_LIGHTING, false);
		lfMeshNode->setScale(vector3d<f32>(0, 0, 0));
		lfMeshNode->setParent(node);
		lfMeshNode->setName(stringc(stringc(node->getName()) + stringc("_flare_mesh")).c_str());
		lfBillBoard = devices->getSceneManager()->addBillboardSceneNode(lfMeshNode);
		lfBillBoard->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
		lfBillBoard->setMaterialFlag(EMF_LIGHTING, false);
		lfBillBoard->setSize(dimension2d<f32>(0, 0));
		lfBillBoard->setParent(lfMeshNode);
		lfBillBoard->setName(stringc(stringc(node->getName()) + stringc("_flare_bill")).c_str());
		lfNode = new CLensFlareSceneNode(lfMeshNode, devices->getSceneManager());
		lfNode->setFalseOcclusion(true);
		lfNode->setParent(lfMeshNode);
		lfNode->setName(stringc(stringc(node->getName()) + stringc("_flare_node")).c_str());
		if (IRRLICHT_SDK_VERSION != "1.7.3") {
			devices->getVideoDriver()->addOcclusionQuery(lfMeshNode, lfMeshNode->getMesh());
		}
		//MESH
		read("scale");
		lfMeshNode->setScale(buildVector3df());
		read("texture");
		lfMeshNode->setMaterialTexture(0, devices->getVideoDriver()->getTexture(xmlReader->getAttributeValue("path")));
		//BILLBOARD
		read("size");
		lfBillBoard->setSize(buildDimension2df());
		read("texture");
		lfBillBoard->setMaterialTexture(0, devices->getVideoDriver()->getTexture(xmlReader->getAttributeValue("path")));
		//LENS FLARE
		read("strength");
		lfNode->setStrength(xmlReader->getAttributeValueAsFloat("value"));
		read("texture");
		lfNode->setMaterialTexture(0, devices->getVideoDriver()->getTexture(xmlReader->getAttributeValue("path")));
		read("falseOcclusion");
		lfNode->setFalseOcclusion(xmlReader->getAttributeValueAsInt("value"));
		read("position");
		lfNode->setPosition(buildVector3df());
	}

	devices->getXEffect()->addShadowLight(shadowLight);
	devices->getCoreData()->getLightsData()->push_back(SLightsData(node, lfMeshNode, lfBillBoard, lfNode));
}

void CImporter::buildVolumeLight() {

	read("name");
	stringc name = xmlReader->getAttributeValue("c8name");

	read("footColor");
	SColor footColor = buildSColor();
	read("tailColor");
	SColor tailColor = buildSColor();
	read("subDivideU");
	u32 subDivideU = xmlReader->getAttributeValueAsInt("value");
	read("subDivideV");
	u32 subDivideV = xmlReader->getAttributeValueAsInt("value");

	IVolumeLightSceneNode *node = smgr->addVolumeLightSceneNode(0, -1, subDivideU, subDivideV, footColor, tailColor);
	if (node) {
		node->setName(name.c_str());

		readMaterials(node);
		readTransformations(node);

		readViewModes(node);

		devices->getCoreData()->getVolumeLightsData()->push_back(SVolumeLightsData(node));
	}
}

void CImporter::buildWaterSurface() {
	read("mesh");
	CWaterSurface *waterSurface = new CWaterSurface(smgr, 0, smgr->getMesh(xmlReader->getAttributeValue("path")), 
													true, true, devices->getWorkingDirectory());
	IAnimatedMeshSceneNode *node = waterSurface->getWaterNode();

	if (node) {
		read("name");
		node->setName(xmlReader->getAttributeValue("c8name"));

		readTransformations(node);
		readMaterials(node);
		readViewModes(node);

		devices->getCollisionManager()->setCollisionToAnAnimatedNode(node);
		SWaterSurfacesData wsdata(waterSurface, 0, "");
		devices->getCoreData()->getWaterSurfaces()->push_back(wsdata);
	}

}

//---------------------------------------------------------------------------------------------
//-----------------------------------BUILDING CONFIGURATIONS-----------------------------------
//---------------------------------------------------------------------------------------------

void CImporter::readConfig() {
	read("config");
	//IF GRID
	read("grid");
	read("accentLineOffset");
	u32 ALO = devices->getCore()->getU32(xmlReader->getAttributeValue("ALO"));
	devices->getObjectPlacement()->getGridSceneNode()->SetAccentlineOffset(ALO);
                
	read("size");
	u32 size = devices->getCore()->getU32(xmlReader->getAttributeValue("S"));
	devices->getObjectPlacement()->getGridSceneNode()->SetSize(size);
                
	read("spacing");
	u32 spacing = devices->getCore()->getU32(xmlReader->getAttributeValue("SP"));
	devices->getObjectPlacement()->getGridSceneNode()->SetSpacing(spacing);
                
	//IF CAMERA
	read("camera");
	read("position");
	vector3df position = vector3df(devices->getCore()->getF32(xmlReader->getAttributeValue("X")),
									devices->getCore()->getF32(xmlReader->getAttributeValue("Y")),
									devices->getCore()->getF32(xmlReader->getAttributeValue("Z")));
	devices->getMayaCamera()->setPosition(position);
                
	read("rotation");
	vector3df rotation = vector3df(devices->getCore()->getF32(xmlReader->getAttributeValue("X")),
									devices->getCore()->getF32(xmlReader->getAttributeValue("Y")),
									devices->getCore()->getF32(xmlReader->getAttributeValue("Z")));
	devices->getMayaCamera()->setRotation(rotation);

	//EFFECTS
	readEffects();
	//CALLBACKS
	readMaterialShaderCallbacks();
	read("config");
}

void CImporter::readEffects() {
	read("effect");
	readWithNextElement("postProcessingEffect", "effect");
	while (element == "postProcessingEffect") {
		stringw path = L"";
		read("file_path");
		path = xmlReader->getAttributeValue("path");
                    
		s32 render = devices->getXEffect()->addPostProcessingEffectFromFile(path.c_str());
		devices->getCoreData()->getEffectRenders()->push_back(render);
		devices->getCoreData()->getEffectRendersPaths()->push_back(path);
                    
		CEffectRenderCallback *callback = new CEffectRenderCallback(render, devices->getDevice());
		callback->clearPixelValues();
		callback->clearVertexValues();
		devices->getCoreData()->getEffectRenderCallbacks()->push_back(callback);
		devices->getXEffect()->setPostProcessingRenderCallback(render, callback);
                    
		read("values");
		readWithNextElement("value", "values");
		while (element == "value") {
			stringw name = L"";
			stringc value = "0.0";
                        
			name = xmlReader->getAttributeValue("name");
			value = xmlReader->getAttributeValue("val");
                        
			callback->getPixelValues()->push_back(value);
			callback->getPixelValuesNames()->push_back(name);
                        
			readWithNextElement("value", "values");
		}
		read("postProcessingEffect");
		readWithNextElement("postProcessingEffect", "effect");
	}
}

void CImporter::readMaterialShaderCallbacks() {
	read("materialTypes");
	readWithNextElement("materialType", "materialTypes");
	while (element == "materialType") {
		read("pixelShaderType");
		E_PIXEL_SHADER_TYPE pixelShaderType = (E_PIXEL_SHADER_TYPE)xmlReader->getAttributeValueAsInt("type");
		read("vertexShaderType");
		E_VERTEX_SHADER_TYPE vertexShaderType = (E_VERTEX_SHADER_TYPE)xmlReader->getAttributeValueAsInt("type");
		read("baseMaterial");
		E_MATERIAL_TYPE baseMaterial = (E_MATERIAL_TYPE)xmlReader->getAttributeValueAsInt("type");

		stringc name = "";
		read("name");
		name = xmlReader->getAttributeValue("cname");

		stringc vertex = "", pixel = "", constants = "";
		read("vertex");
		vertex = xmlReader->getAttributeValue("shader");
		read("pixel");
		pixel = xmlReader->getAttributeValue("shader");
		read("constants");
		constants = xmlReader->getAttributeValue("value");

		CShaderCallback *callback = new CShaderCallback();
		callback->setName(name.c_str());
		callback->setVertexShader(vertex.c_str());
		callback->setPixelShader(pixel.c_str());
		callback->setConstants(constants.c_str());
		callback->setPixelShaderType(pixelShaderType);
		callback->setVertexShaderType(vertexShaderType);
		callback->setBaseMaterial(baseMaterial);
		callback->setDevice(devices->getDevice());
		callback->buildMaterial(devices->getVideoDriver());
		devices->getCoreData()->getShaderCallbacks()->push_back(callback);

		read("materialType");
		readWithNextElement("materialType", "materialTypes");
	}
}

void CImporter::readFactory(ISceneNode *_node) {

}

void CImporter::readMaterials(ISceneNode *_node) {
	read("materials");
	readWithNextElement("material", "materials");
	while (element == "material") {
		u32 id = xmlReader->getAttributeValueAsInt("id");
		//TEXTURES
		read("textures");
		for (u32 i=0; i < irr::video::MATERIAL_MAX_TEXTURES; i++) {
			read("texture");
			ITexture *tex = devices->getVideoDriver()->getTexture(xmlReader->getAttributeValue("path"));
			_node->getMaterial(id).TextureLayer[i].Texture = tex;
		}

		//COLORS
		read("diffuseColor");
		_node->getMaterial(id).DiffuseColor = buildSColor();
		read("ambiantColor");
		_node->getMaterial(id).AmbientColor = buildSColor();
		read("specularColor");
		_node->getMaterial(id).SpecularColor = buildSColor();
		read("emissiveColor");
		_node->getMaterial(id).EmissiveColor = buildSColor();

		//LIGHTING
		read("lighting");
		_node->getMaterial(id).Lighting = xmlReader->getAttributeValueAsInt("value");

		//MATERIAL TYPE PARAMETERS
		read("materialTypeParam1");
		_node->getMaterial(id).MaterialTypeParam = xmlReader->getAttributeValueAsFloat("value");
		read("materialTypeParam2");
		_node->getMaterial(id).MaterialTypeParam2 = xmlReader->getAttributeValueAsFloat("value");

		//MISC PARAMETERS
		read("shininess");
		_node->getMaterial(id).Shininess = xmlReader->getAttributeValueAsFloat("value");
		read("thickness");
		_node->getMaterial(id).Thickness = xmlReader->getAttributeValueAsFloat("value");

		//FLAGS
		read("antiAliasing");
		_node->getMaterial(id).AntiAliasing = xmlReader->getAttributeValueAsInt("value");
		read("backfaceCulling");
		_node->getMaterial(id).BackfaceCulling = xmlReader->getAttributeValueAsInt("value");
		read("colorMask");
		_node->getMaterial(id).ColorMask = xmlReader->getAttributeValueAsInt("value");
		read("colorMaterial");
		_node->getMaterial(id).ColorMaterial = xmlReader->getAttributeValueAsInt("value");
		read("fogEnable");
		_node->getMaterial(id).FogEnable = xmlReader->getAttributeValueAsInt("value");
		read("frontfaceCulling");
		_node->getMaterial(id).FrontfaceCulling = xmlReader->getAttributeValueAsInt("value");
		read("gouraudShading");
		_node->getMaterial(id).GouraudShading = xmlReader->getAttributeValueAsInt("value");
		read("normalizeNormals");
		_node->getMaterial(id).NormalizeNormals = xmlReader->getAttributeValueAsInt("value");
		read("zBuffer");
		_node->getMaterial(id).ZBuffer = xmlReader->getAttributeValueAsInt("value");
		read("zWriteEnable");
		_node->getMaterial(id).ZWriteEnable = xmlReader->getAttributeValueAsInt("value");

		//MATERIAL TYPE
		read("materianType");
		s32 matS32 = xmlReader->getAttributeValueAsInt("value");
		_node->getMaterial(id).MaterialType = (E_MATERIAL_TYPE)matS32;
		if (matS32 < 0) {
			_node->getMaterial(id).MaterialType = (E_MATERIAL_TYPE)devices->getCoreData()->getShaderCallbacks()->operator[](-matS32-1)->getMaterial();
		}

		read("material");
		readWithNextElement("material", "materials");
	}
}

void CImporter::readTransformations(ISceneNode *_node) {
	 //POSITION
    read("position");
    vector3df position = buildVector3df();
    _node->setPosition(position);
                    
    //ROTATION
    read("rotation");
    vector3df rotation = buildVector3df();
    _node->setRotation(rotation);
                    
    //SCALE
    read("scale");
    vector3df scale = buildVector3df();
	if (_node->getType() == ESNT_BILLBOARD) {
		((IBillboardSceneNode *)_node)->setSize(dimension2d<f32>(scale.X, scale.Y));
	} else {
		_node->setScale(scale);
	}
}

void CImporter::readViewModes(ISceneNode *_node) {
	read("visible");
	_node->setVisible(xmlReader->getAttributeValueAsInt("bool"));

	read("shadowMode");
	E_SHADOW_MODE shadowMode = ESM_EXCLUDE;
	shadowMode = (E_SHADOW_MODE)xmlReader->getAttributeValueAsInt("mode");
	if (shadowMode != ESM_NO_SHADOW)
		devices->getXEffect()->addShadowToNode(_node, devices->getXEffectFilterType(), shadowMode);
}

//---------------------------------------------------------------------------------------------
//-----------------------------------BUILDING PARAMETERS---------------------------------------
//---------------------------------------------------------------------------------------------

SColor CImporter::buildSColor() {
	SColor color(xmlReader->getAttributeValueAsInt("a"), xmlReader->getAttributeValueAsInt("r"),
				 xmlReader->getAttributeValueAsInt("g"), xmlReader->getAttributeValueAsInt("b"));
	return color;
}

SColorf CImporter::buildSColorf() {
	SColorf color(xmlReader->getAttributeValueAsInt("r"), xmlReader->getAttributeValueAsInt("g"), 
				  xmlReader->getAttributeValueAsInt("b"), xmlReader->getAttributeValueAsInt("a"));
	return color;
}

vector3df CImporter::buildVector3df() {
	vector3df vec = buildVector3d<f32>();
    return vec;
}

dimension2df CImporter::buildDimension2df() {
	dimension2df dim = buildDimension2d<f32>();
	return dim;
}

template<class T>
vector3d<T> CImporter::buildVector3d() {
	vector3df vec = vector3df(xmlReader->getAttributeValueAsFloat("X"),
							  xmlReader->getAttributeValueAsFloat("Y"),
							  xmlReader->getAttributeValueAsFloat("Z"));
    return vec;
}

template<class T>
dimension2d<T> CImporter::buildDimension2d() {
	dimension2d<T> dim = dimension2d<T>(devices->getCore()->getF32(xmlReader->getAttributeValue("Width")),
										devices->getCore()->getF32(xmlReader->getAttributeValue("Height")));
	return dim;
}

//---------------------------------------------------------------------------------------------
//-----------------------------------LOAD SCENE------------------------------------------------
//---------------------------------------------------------------------------------------------

void CImporter::newImportScene(stringc file_path) {
	read("rootScene");

	readConfig();

	while (xmlReader->read()) {

		element = "";
		if (xmlReader->getNodeType() == EXN_ELEMENT)
			element = xmlReader->getNodeName();

		if (element == "terrain")
			buildTerrain();
		else if (element == "tree")
			buildTree();
		else if (element == "object")
			buildObject();
		else if (element == "light")
			buildLight();
		else if (element == "waterSurface")
			buildWaterSurface();
		else if (element == "volumeLight")
			buildVolumeLight();

	}

    read("rootScene");

    delete xmlReader;

}
