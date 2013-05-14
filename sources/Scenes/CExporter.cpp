//
//  CExporter.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#include "CExporter.h"

CExporter::CExporter(CDevices *_devices) {
    devices = _devices;
}

CExporter::~CExporter() {
    
}

void CExporter::exportScene(stringc file_path) {
    
    stringc wd = devices->getWorkingDirectory().c_str();
    //wd+= "/";
    
    FILE *export_file;
	export_file = fopen(stringc(stringc(file_path.c_str()) + stringc("_temp")).c_str(), "w");
    
    //HEADER
	fprintf(export_file, "<?xml version=\"1.0\"?>\n\n");
    
    //ROOT
    fprintf(export_file, "<rootScene>\n\n");
    
    //CONFIG
    fprintf(export_file, "\t<config>\n\n");
    
    //GRID
    fprintf(export_file, "\t\t <grid>\n\n");
    fprintf(export_file, "\t\t\t <accentLineOffset ALO=\"%d\" />\n", devices->getObjectPlacement()->getGridSceneNode()->GetAccentlineOffset());
    fprintf(export_file, "\t\t\t <size S=\"%d\" />\n", devices->getObjectPlacement()->getGridSceneNode()->GetSize());
    fprintf(export_file, "\t\t\t <spacing SP=\"%d\" />\n\n", devices->getObjectPlacement()->getGridSceneNode()->GetSpacing());
    fprintf(export_file, "\t\t </grid>\n\n");
    
    //CAMERA
    fprintf(export_file, "\t\t <camera>\n\n");
    fprintf(export_file, "\t\t\t <position X=\"%f\" Y=\"%f\" Z=\"%f\" />\n", devices->getMayaCamera()->getPosition().X,
            devices->getMayaCamera()->getPosition().Y, devices->getMayaCamera()->getPosition().Z);
    fprintf(export_file, "\t\t\t <rotation X=\"%f\" Y=\"%f\" Z=\"%f\" />\n\n", devices->getMayaCamera()->getRotation().X,
            devices->getMayaCamera()->getRotation().Y, devices->getMayaCamera()->getRotation().Z);
    fprintf(export_file, "\t\t </camera>\n\n");
    
    //EFFECTS
    fprintf(export_file, "\t\t <effect>\n\n");
    for (int i=0; i < devices->getCoreData()->getEffectRenders()->size(); i++) {
        stringc path = "";
        path = devices->getCoreData()->getEffectRendersPaths()->operator[](i).c_str();
        //fprintf(export_file, "\t\t\t <postProcessingEffect path=\"%s\" />\n", path.c_str());
        fprintf(export_file, "\t\t\t <postProcessingEffect>\n");
        fprintf(export_file, "\t\t\t\t <file_path path=\"%s\" />\n", path.c_str());
        fprintf(export_file, "\t\t\t\t <values>\n");
        for (u32 ei=0; ei < devices->getCoreData()->getEffectRenderCallbacks()->operator[](i)->getPixelValues()->size(); ei++) {
            fprintf(export_file, "\t\t\t\t\t <value name=\"%s\" val=\"%s\" />\n",
                    devices->getCoreData()->getEffectRenderCallbacks()->operator[](i)->getPixelValuesNames()->operator[](ei).c_str(),
                    devices->getCoreData()->getEffectRenderCallbacks()->operator[](i)->getPixelValues()->operator[](ei).c_str());
        }
        fprintf(export_file, "\t\t\t\t </values>\n");
        fprintf(export_file, "\t\t\t </postProcessingEffect>\n\n");
    }
    fprintf(export_file, "\n\t\t </effect>\n\n");
    
    //MATERIAL TYPES
    fprintf(export_file, "\t\t <materialTypes>\n\n");
    for (int i=0; i < devices->getCoreData()->getShaderCallbacks()->size(); i++) {
        fprintf(export_file, "\t\t\t <materialType>\n");
        
        fprintf(export_file, "\t\t\t\t <pixelShaderType type=\"%d\" /> \n", devices->getCoreData()->getShaderCallbacks()->operator[](i)->getPixelShaderType());
        fprintf(export_file, "\t\t\t\t <vertexShaderType type=\"%d\" /> \n", devices->getCoreData()->getShaderCallbacks()->operator[](i)->getVertexShaderType());
		fprintf(export_file, "\t\t\t\t <baseMaterial type=\"%d\" /> \n", devices->getCoreData()->getShaderCallbacks()->operator[](i)->getBaseMaterial());
        fprintf(export_file, "\t\t\t\t <name cname=\"%s\" />\n", devices->getCoreData()->getShaderCallbacks()->operator[](i)->getName().c_str());
        fprintf(export_file, "\t\t\t\t <vertex shader=\n\"%s\" />\n", devices->getCoreData()->getShaderCallbacks()->operator[](i)->getVertexShader().c_str());
        fprintf(export_file, "\t\t\t\t <pixel shader=\n\"%s\" />\n", devices->getCoreData()->getShaderCallbacks()->operator[](i)->getPixelShader().c_str());
        fprintf(export_file, "\t\t\t\t <constants value=\"%s\" />\n", devices->getCoreData()->getShaderCallbacks()->operator[](i)->getConstants().c_str());
        
        fprintf(export_file, "\t\t\t </materialType>\n\n");
    }
    fprintf(export_file, "\n\t\t </materialTypes>\n\n");
    
    //END CONFIG
    fprintf(export_file, "\t</config>\n\n\n\n");
    
    //SCENE
    fprintf(export_file, "\t<scene>\n\n");
    
    //TERRAINS
    for (int i=0; i < devices->getCoreData()->getTerrainNodes()->size(); i++) {
        ISceneNode *node = devices->getCoreData()->getTerrainNodes()->operator[](i);
        
        devices->getCoreData()->getTerrainPaths()->operator[](i).remove(wd.c_str());
        
        fprintf(export_file, "\t\t <terrain>\n\n");
        fprintf(export_file, "\t\t\t <path file=\"%ls\" />\n\n", devices->getCoreData()->getTerrainPaths()->operator[](i).c_str());
        if (node->getType() == ESNT_OCTREE) {
			fprintf(export_file, "\t\t\t <type esnt=\"octtree\" mppn=\"%u\" />\n\n", devices->getCoreData()->getTerrainMinPolysPerNode()->operator[](i));
        } else {
            fprintf(export_file, "\t\t\t <type esnt=\"mesh\" />\n\n");
        }
        
		fprintf(export_file, "\t\t\t <factory> \n\n");
		s32 planaredIndice = devices->getCoreData()->isMeshPlanared(node);
		if (planaredIndice != -1) {
			SPlanarTextureMappingData sptm = devices->getCoreData()->getPlanarTextureMappingValues()->operator[](planaredIndice);
			if (sptm.isGeneralPlanarTextureMapping()) {
				fprintf(export_file, "\t\t\t\t <primitive type=\"planar\" options=\"general\" /> \n");
				fprintf(export_file, "\t\t\t\t\t <resolutionS value=\"%f\" /> \n", sptm.getResolutionS());
				fprintf(export_file, "\t\t\t\t </primitive> \n");
			} else {
				fprintf(export_file, "\t\t\t\t <primitive type=\"planar\" /> \n");
				fprintf(export_file, "\t\t\t\t\t <resolutionS value=\"%f\" /> \n", sptm.getResolutionS());
				fprintf(export_file, "\t\t\t\t\t <resolutionT value=\"%f\" /> \n", sptm.getResolutionT());
				fprintf(export_file, "\t\t\t\t\t <axis value=\"%u\" /> \n", sptm.getAxis());
				fprintf(export_file, "\t\t\t\t\t <offset X=\"%f\" Y=\"%f\" Z=\"%f\" /> \n", sptm.getOffset().X, sptm.getOffset().Y, sptm.getOffset().Z);
				fprintf(export_file, "\t\t\t\t\t <general value=\"%u\" />\n", sptm.isGeneralPlanarTextureMapping());
				fprintf(export_file, "\t\t\t\t </primitive> \n");
			}
		}
		fprintf(export_file, "\n\t\t\t </factory> \n\n");

        fprintf(export_file, "\t\t\t <name c8name=\"%s\" />\n\n", node->getName());
        
        //MATERIALS
        fprintf(export_file, "\t\t\t <materials> \n\n");
        for (int mi=0; mi < node->getMaterialCount(); mi++) {
            stringc internalNames[4];
            
            for (int in=0; in < 4; in++) {
                if (node->getMaterial(mi).TextureLayer[in].Texture) {
                    internalNames[in] = node->getMaterial(mi).TextureLayer[in].Texture->getName().getPath().c_str();
                    internalNames[in].remove(wd.c_str());
                } else {
                    internalNames[in] = "";
                }
            }
            
            int lighting = 0;
            if (node->getMaterial(mi).Lighting == true) {
                lighting = 1;
            }
            
            int matTypeInt = node->getMaterial(mi).MaterialType;
            if (node->getMaterial(mi).MaterialType > 24) {
                bool founded = false;
                int dmti = 1;
                while (!founded) {
                    if (node->getMaterial(mi).MaterialType == devices->getCoreData()->getShaderCallbacks()->operator[](dmti-1)->getMaterial()) {
                        matTypeInt = -dmti;
                        founded = true;
                    }
                    dmti ++;
                }
            }
            
            fprintf(export_file, "\t\t\t\t <material id=\"%d\" texture1=\"%s\" texture2=\"%s\" texture3=\"%s\" texture4=\"%s\" "
                    "dca=\"%u\" dcr=\"%u\" dcg=\"%u\" dcb=\"%u\" "
                    "aca=\"%u\" acr=\"%u\" acg=\"%u\" acb=\"%u\" "
                    "sca=\"%u\" scr=\"%u\" scg=\"%u\" scb=\"%u\" "
                    "eca=\"%u\" ecr=\"%u\" ecg=\"%u\" ecb=\"%u\" "
                    "lighting=\"%u\" materialType=\"%d\" /> \n",
                    mi, internalNames[0].c_str(), internalNames[1].c_str(), internalNames[2].c_str(), internalNames[3].c_str(),
                    node->getMaterial(mi).DiffuseColor.getAlpha(), node->getMaterial(mi).DiffuseColor.getRed(), node->getMaterial(mi).DiffuseColor.getGreen(), node->getMaterial(mi).DiffuseColor.getBlue(),
                    node->getMaterial(mi).AmbientColor.getAlpha(), node->getMaterial(mi).AmbientColor.getRed(), node->getMaterial(mi).AmbientColor.getGreen(), node->getMaterial(mi).AmbientColor.getBlue(),
                    node->getMaterial(mi).SpecularColor.getAlpha(), node->getMaterial(mi).SpecularColor.getRed(), node->getMaterial(mi).SpecularColor.getGreen(), node->getMaterial(mi).SpecularColor.getBlue(),
                    node->getMaterial(mi).EmissiveColor.getAlpha(), node->getMaterial(mi).EmissiveColor.getRed(), node->getMaterial(mi).EmissiveColor.getGreen(), node->getMaterial(mi).EmissiveColor.getBlue(),
                    lighting, matTypeInt);
        }
        fprintf(export_file, "\n\t\t\t </materials> \n\n");
        
        fprintf(export_file, "\t\t\t <position X=\"%f\" Y=\"%f\" Z=\"%f\" />\n", node->getPosition().X, node->getPosition().Y,
                node->getPosition().Z);
        fprintf(export_file, "\t\t\t <rotation X=\"%f\" Y=\"%f\" Z=\"%f\" />\n", node->getRotation().X, node->getRotation().Y,
                node->getRotation().Z);
        fprintf(export_file, "\t\t\t <scale X=\"%f\" Y=\"%f\" Z=\"%f\" />\n", node->getScale().X, node->getScale().Y,
                node->getScale().Z);

		E_SHADOW_MODE shadowMode;
		if (devices->getXEffect()->isNodeShadowed(node, devices->getXEffectFilterType(), ESM_BOTH)) {
			shadowMode = ESM_BOTH;
		} else if (devices->getXEffect()->isNodeShadowed(node, devices->getXEffectFilterType(), ESM_EXCLUDE)) {
			shadowMode = ESM_EXCLUDE;
		} else if (devices->getXEffect()->isNodeShadowed(node, devices->getXEffectFilterType(), ESM_CAST)) {
			shadowMode = ESM_CAST;
		} else if (devices->getXEffect()->isNodeShadowed(node, devices->getXEffectFilterType(), ESM_RECEIVE)) {
			shadowMode = ESM_RECEIVE;
		}

		fprintf(export_file, "\t\t\t <visible bool=\"%d\" />\n", node->isVisible());
		fprintf(export_file, "\t\t\t <shadowMode mode=\"%d\" />\n", shadowMode);

        
        //END TERRAIN
        fprintf(export_file, "\n\t\t </terrain>\n\n");
    }
    
    //OBJECTS
    for (int i=0; i < devices->getCoreData()->getObjectsData()->size(); i++) {
		ISceneNode *node = devices->getCoreData()->getObjectsData()->operator[](i).getNode();
        
		devices->getCoreData()->getObjectsData()->operator[](i).getPath().remove(wd.c_str());
        
        fprintf(export_file, "\t\t <object>\n\n");
        
		fprintf(export_file, "\t\t\t <path file=\"%ls\" />\n\n", devices->getCoreData()->getObjectsData()->operator[](i).getPath().c_str());
        fprintf(export_file, "\t\t\t <name c8name=\"%s\" />\n\n", node->getName());
        
        //MATERIALS
        fprintf(export_file, "\t\t\t <materials> \n\n");
        for (int mi=0; mi < node->getMaterialCount(); mi++) {
            stringc internalNames[4];
            
            for (int in=0; in < 4; in++) {
                if (node->getMaterial(mi).TextureLayer[in].Texture) {
                    internalNames[in] = node->getMaterial(mi).TextureLayer[in].Texture->getName().getPath().c_str();
                    internalNames[in].remove(wd.c_str());
                } else {
                    internalNames[in] = "";
                }
            }
            
            int lighting = 0;
            if (node->getMaterial(mi).Lighting == true) {
                lighting = 1;
            }
            
            fprintf(export_file, "\t\t\t\t <material id=\"%d\" texture1=\"%s\" texture2=\"%s\" texture3=\"%s\" texture4=\"%s\" "
                    "dca=\"%u\" dcr=\"%u\" dcg=\"%u\" dcb=\"%u\" "
                    "aca=\"%u\" acr=\"%u\" acg=\"%u\" acb=\"%u\" "
                    "sca=\"%u\" scr=\"%u\" scg=\"%u\" scb=\"%u\" "
                    "eca=\"%u\" ecr=\"%u\" ecg=\"%u\" ecb=\"%u\" "
                    "lighting=\"%u\" materialType=\"%u\" /> \n",
                    mi, internalNames[0].c_str(), internalNames[1].c_str(), internalNames[2].c_str(), internalNames[3].c_str(),
                    node->getMaterial(mi).DiffuseColor.getAlpha(), node->getMaterial(mi).DiffuseColor.getRed(), node->getMaterial(mi).DiffuseColor.getGreen(), node->getMaterial(mi).DiffuseColor.getBlue(),
                    node->getMaterial(mi).AmbientColor.getAlpha(), node->getMaterial(mi).AmbientColor.getRed(), node->getMaterial(mi).AmbientColor.getGreen(), node->getMaterial(mi).AmbientColor.getBlue(),
                    node->getMaterial(mi).SpecularColor.getAlpha(), node->getMaterial(mi).SpecularColor.getRed(), node->getMaterial(mi).SpecularColor.getGreen(), node->getMaterial(mi).SpecularColor.getBlue(),
                    node->getMaterial(mi).EmissiveColor.getAlpha(), node->getMaterial(mi).EmissiveColor.getRed(), node->getMaterial(mi).EmissiveColor.getGreen(), node->getMaterial(mi).EmissiveColor.getBlue(),
                    lighting, node->getMaterial(mi).MaterialType);
        }
        fprintf(export_file, "\n\t\t\t </materials> \n\n");
        
        fprintf(export_file, "\t\t\t <position X=\"%f\" Y=\"%f\" Z=\"%f\" />\n", node->getPosition().X, node->getPosition().Y,
                node->getPosition().Z);
        fprintf(export_file, "\t\t\t <rotation X=\"%f\" Y=\"%f\" Z=\"%f\" />\n", node->getRotation().X, node->getRotation().Y,
                node->getRotation().Z);
        fprintf(export_file, "\t\t\t <scale X=\"%f\" Y=\"%f\" Z=\"%f\" />\n", node->getScale().X, node->getScale().Y,
                node->getScale().Z);

        
        fprintf(export_file, "\t\t </object>\n\n");
        
    }
    
    //TREES
    for (int i=0; i < devices->getCoreData()->getTreesData()->size(); i++) {
		ISceneNode *node = devices->getCoreData()->getTreesData()->operator[](i).getNode();
        
		devices->getCoreData()->getTreesData()->operator[](i).getPath().remove(wd.c_str());
        
        fprintf(export_file, "\t\t <tree>\n\n");
        
		fprintf(export_file, "\t\t\t <path file=\"%ls\" />\n\n", devices->getCoreData()->getTreesData()->operator[](i).getPath().c_str());
        fprintf(export_file, "\t\t\t <name c8name=\"%s\" />\n\n", node->getName());
        
        //MATERIALS
        fprintf(export_file, "\t\t\t <materials> \n\n");
        for (int mi=0; mi < node->getMaterialCount(); mi++) {
            stringc internalNames[4];
            
            for (int in=0; in < 4; in++) {
                if (node->getMaterial(mi).TextureLayer[in].Texture) {
                    internalNames[in] = node->getMaterial(mi).TextureLayer[in].Texture->getName().getPath().c_str();
                } else {
                    internalNames[in] = "";
                }
            }
            
            int lighting = 0;
            if (node->getMaterial(mi).Lighting == true) {
                lighting = 1;
            }
            
            fprintf(export_file, "\t\t\t\t <material id=\"%d\" texture1=\"%s\" texture2=\"%s\" texture3=\"%s\" texture4=\"%s\" "
                    "dca=\"%u\" dcr=\"%u\" dcg=\"%u\" dcb=\"%u\" "
                    "aca=\"%u\" acr=\"%u\" acg=\"%u\" acb=\"%u\" "
                    "sca=\"%u\" scr=\"%u\" scg=\"%u\" scb=\"%u\" "
                    "eca=\"%u\" ecr=\"%u\" ecg=\"%u\" ecb=\"%u\" "
                    "lighting=\"%u\" materialType=\"%u\" /> \n",
                    mi, internalNames[0].c_str(), internalNames[1].c_str(), internalNames[2].c_str(), internalNames[3].c_str(),
                    node->getMaterial(mi).DiffuseColor.getAlpha(), node->getMaterial(mi).DiffuseColor.getRed(), node->getMaterial(mi).DiffuseColor.getGreen(), node->getMaterial(mi).DiffuseColor.getBlue(),
                    node->getMaterial(mi).AmbientColor.getAlpha(), node->getMaterial(mi).AmbientColor.getRed(), node->getMaterial(mi).AmbientColor.getGreen(), node->getMaterial(mi).AmbientColor.getBlue(),
                    node->getMaterial(mi).SpecularColor.getAlpha(), node->getMaterial(mi).SpecularColor.getRed(), node->getMaterial(mi).SpecularColor.getGreen(), node->getMaterial(mi).SpecularColor.getBlue(),
                    node->getMaterial(mi).EmissiveColor.getAlpha(), node->getMaterial(mi).EmissiveColor.getRed(), node->getMaterial(mi).EmissiveColor.getGreen(), node->getMaterial(mi).EmissiveColor.getBlue(),
                    lighting, node->getMaterial(mi).MaterialType);
        }
        fprintf(export_file, "\n\t\t\t </materials> \n\n");
        
        fprintf(export_file, "\t\t\t <position X=\"%f\" Y=\"%f\" Z=\"%f\" />\n", node->getPosition().X, node->getPosition().Y,
                node->getPosition().Z);
        fprintf(export_file, "\t\t\t <rotation X=\"%f\" Y=\"%f\" Z=\"%f\" />\n", node->getRotation().X, node->getRotation().Y,
                node->getRotation().Z);
        fprintf(export_file, "\t\t\t <scale X=\"%f\" Y=\"%f\" Z=\"%f\" />\n", node->getScale().X, node->getScale().Y,
                node->getScale().Z);
        
        
        fprintf(export_file, "\t\t </tree>\n\n");
    }
    
    for (int i=0; i < devices->getCoreData()->getLightsData()->size(); i++) {
		ISceneNode *node = devices->getCoreData()->getLightsData()->operator[](i).getNode();
        
        fprintf(export_file, "\t\t <light>\n\n");
        fprintf(export_file, "\t\t\t <name c8name=\"%s\" />\n\n", node->getName());
        
        fprintf(export_file, "\t\t\t <position X=\"%f\" Y=\"%f\" Z=\"%f\" />\n", node->getPosition().X,
                node->getPosition().Y, node->getPosition().Z);
        fprintf(export_file, "\t\t\t <target X=\"%f\" Y=\"%f\" Z=\"%f\" />\n\n", node->getRotation().X,
                node->getRotation().Y, node->getRotation().Z);
        
        fprintf(export_file, "\t\t\t <diffuseColor R=\"%f\" G=\"%f\" B=\"%f\" />\n", ((ILightSceneNode *)node)->getLightData().DiffuseColor.r, 
                ((ILightSceneNode *)node)->getLightData().DiffuseColor.g,
                ((ILightSceneNode *)node)->getLightData().DiffuseColor.b);
        
        fprintf(export_file, "\t\t\t <ambiantColor R=\"%f\" G=\"%f\" B=\"%f\" />\n", ((ILightSceneNode *)node)->getLightData().AmbientColor.r, 
                ((ILightSceneNode *)node)->getLightData().AmbientColor.g,
                ((ILightSceneNode *)node)->getLightData().AmbientColor.b);
        
        fprintf(export_file, "\t\t\t <specularColor R=\"%f\" G=\"%f\" B=\"%f\" />\n\n", ((ILightSceneNode *)node)->getLightData().SpecularColor.r, 
                ((ILightSceneNode *)node)->getLightData().SpecularColor.g,
                ((ILightSceneNode *)node)->getLightData().SpecularColor.b);
        
        fprintf(export_file, "\t\t\t <radius value=\"%f\" />\n", ((ILightSceneNode *)node)->getRadius());
		fprintf(export_file, "\t\t\t <farValue value=\"%f\" />\n", devices->getXEffect()->getShadowLight(i).getFarValue());
        
        fprintf(export_file, "\n\t\t\t <shadows resol=\"%u\" />\n\n", devices->getXEffect()->getShadowLight(i).getShadowMapResolution());

		//WRITE LENS FLARE NODE INFORMATIONS
		fprintf(export_file, "\t\t\t <lensFlare>\n");
		if (devices->getCoreData()->getLightsData()->operator[](i).getLensFlareSceneNode()) {
			SLightsData ldata = devices->getCoreData()->getLightsData()->operator[](i);

			fprintf(export_file, "\t\t\t\t <mesh> \n"); //MESH
			fprintf(export_file, "\t\t\t\t\t <scale X=\"%f\" Y=\"%f\" Z=\"%f\" />\n", ldata.getLensFlareMeshSceneNode()->getScale().X,
				    ldata.getLensFlareMeshSceneNode()->getScale().Y, ldata.getLensFlareMeshSceneNode()->getScale().Z);
			stringc ldataTexturePath = "";
			if (ldata.getLensFlareMeshSceneNode()->getMaterial(0).TextureLayer[0].Texture) {
				ldataTexturePath = ldata.getLensFlareMeshSceneNode()->getMaterial(0).TextureLayer[0].Texture->getName().getPath().c_str();
			}
			ldataTexturePath.remove(devices->getWorkingDirectory().c_str());
			fprintf(export_file, "\t\t\t\t\t <texture path=\"%s\" />\n",ldataTexturePath.c_str());
			fprintf(export_file, "\t\t\t\t </mesh> \n");

			fprintf(export_file, "\t\t\t\t <bill> \n"); //BILLBOARD
			fprintf(export_file, "\t\t\t\t\t <size Width=\"%f\" Height=\"%f\" />\n", ldata.getLensFlareBillBoardSceneNode()->getSize().Width,
				    ldata.getLensFlareBillBoardSceneNode()->getSize().Height);
			ldataTexturePath = "";
			if (ldata.getLensFlareBillBoardSceneNode()->getMaterial(0).TextureLayer[0].Texture) {
				ldataTexturePath = ldata.getLensFlareBillBoardSceneNode()->getMaterial(0).TextureLayer[0].Texture->getName().getPath().c_str();
			}
			ldataTexturePath.remove(devices->getWorkingDirectory().c_str());
			fprintf(export_file, "\t\t\t\t\t <texture path=\"%s\" />\n", ldataTexturePath.c_str());
			fprintf(export_file, "\t\t\t\t </bill> \n");

			fprintf(export_file, "\t\t\t\t <lfsn> \n"); //LENS FLARE SCENE NODES
			fprintf(export_file, "\t\t\t\t\t <strength value=\"%f\" />\n", ldata.getLensFlareSceneNode()->getStrength());
			ldataTexturePath = "";
			if (ldata.getLensFlareSceneNode()->getMaterial(0).TextureLayer[0].Texture) {
				ldataTexturePath = ldata.getLensFlareSceneNode()->getMaterial(0).TextureLayer[0].Texture->getName().getPath().c_str();
			}
			ldataTexturePath.remove(devices->getWorkingDirectory().c_str());
			fprintf(export_file, "\t\t\t\t\t <texture path=\"%s\" />\n", ldataTexturePath.c_str());
			fprintf(export_file, "\t\t\t\t\t <falseOcclusion value=\"%i\" />\n", ldata.getLensFlareSceneNode()->getFalseOcclusion());
			fprintf(export_file, "\t\t\t\t </lfsn> \n");

			fprintf(export_file, "\t\t\t\t <position X=\"%f\" Y=\"%f\" Z=\"%f\" />\n ",
					ldata.getLensFlareMeshSceneNode()->getPosition().X, ldata.getLensFlareMeshSceneNode()->getPosition().Y,
					ldata.getLensFlareMeshSceneNode()->getPosition().Z);
		}
		fprintf(export_file, "\t\t\t </lensFlare>\n\n");
        
        fprintf(export_file, "\t\t </light>\n\n");
    }
    
    //END SCENE
    fprintf(export_file, "\n\t</scene>\n\n");
    
    //END ROOT
    fprintf(export_file, "</rootScene>");
    
    fclose(export_file);

	//COPY FILES FOR SECURITY
	std::ifstream exported_file(stringc(stringc(file_path.c_str()) + stringc("_temp")).c_str(), std::ios::in);
	std::ofstream file_to_copy(file_path.c_str(), std::ios::out | std::ios::trunc);

	std::string line;
	while (std::getline(exported_file, line)) {
		//std::cout << line;
		if (line.c_str() != "\n")
			file_to_copy << line.c_str() << std::endl;
	}

	exported_file.close();
	file_to_copy.close();

	//REMOVE THE TEMP FILE
	if (remove(stringc(stringc(file_path.c_str()) + stringc("_temp")).c_str()) != 0) {
		devices->addInformationDialog(L"Error", L"Error when deleting the temp file... \n Not a problem.", EMBF_OK);
	}

}