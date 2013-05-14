//
//  CImporter.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 01/10/12.
//
//

#include "CImporter.h"

CImporter::CImporter(CDevices *_devices) {
    devices = _devices;
}

CImporter::~CImporter() {
    devices = NULL;
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
    
    devices->getCoreData()->clearAllTheArrays();
    
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
