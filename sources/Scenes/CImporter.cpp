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
                devices->getSceneManager()->getActiveCamera()->setPosition(position);
                
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
                devices->getDevice()->getLogger()->setLogLevel(ELL_INFORMATION);
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
                devices->getDevice()->getLogger()->setLogLevel(ELL_NONE);
                
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
                } else if (esnt == "octtree") {
                    octTreeMesh = devices->getSceneManager()->getMesh(path.c_str());
                    devices->getSceneManager()->getMeshManipulator()->makePlanarTextureMapping(octTreeMesh, 0.02f);
                    octTreeNode = devices->getSceneManager()->addOctreeSceneNode(octTreeMesh, 0, -1, 1024);
                } else if (esnt == "mesh") {
                    octTreeMesh = devices->getSceneManager()->getMesh(path.c_str());
                    octTreeNode = devices->getSceneManager()->addMeshSceneNode(octTreeMesh, 0, -1);
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
                    
                    octTreeNode->setMaterialFlag(EMF_NORMALIZE_NORMALS, false);
                    devices->getXEffect()->addShadowToNode(octTreeNode, devices->getXEffectFilterType());
                    devices->getCollisionManager()->setCollisionToAnOctTreeNode(octTreeNode);
                    devices->getCoreData()->getTerrainNodes()->push_back(octTreeNode);
                    devices->getCoreData()->getTerrainPaths()->push_back(path.c_str());
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
                devices->getCoreData()->getTreeNodes()->push_back(octTreeNode);
                devices->getCoreData()->getTreePaths()->push_back(path.c_str());
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
                
                animatedMesh = devices->getSceneManager()->getMesh(path.c_str());
                animatedNode = devices->getSceneManager()->addAnimatedMeshSceneNode(animatedMesh);
                animatedNode->setAnimationSpeed(0);
                animatedNode->setFrameLoop(0, 0);
                
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
                    animatedNode->setScale(scale);
                }
                
                animatedNode->setMaterialFlag(EMF_NORMALIZE_NORMALS, false);
                devices->getXEffect()->addShadowToNode(animatedNode, devices->getXEffectFilterType());
                devices->getCollisionManager()->setCollisionToAnAnimatedNode(animatedNode);
                devices->getCoreData()->getObjectNodes()->push_back(animatedNode);
                devices->getCoreData()->getObjectPaths()->push_back(path.c_str());
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
                read("shadows");
                if (element == "shadows") {
                    u32 resol = devices->getCore()->getU32(xmlReader->getAttributeValue("resol"));
                    shadowLight.setShadowMapResolution(resol);
                }
                
                devices->getXEffect()->addShadowLight(shadowLight);
                devices->getCoreData()->getLightsNodes()->push_back(light);
                
                devices->getCoreData()->getLfMeshNodes()->push_back(0);
                devices->getCoreData()->getLfBillBoardSceneNodes()->push_back(0);
                devices->getCoreData()->getLensFlareSceneNodes()->push_back(0);
            }
            
        }
    }
    
    if (xmlReader) {
        read("rootScene");
    }
    
    delete xmlReader;

}
