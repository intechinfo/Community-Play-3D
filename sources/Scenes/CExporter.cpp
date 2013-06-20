//
//  CExporter.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//
#include "stdafx.h"
#include "CExporter.h"

CExporter::CExporter(CDevices *_devices) {
    devices = _devices;
}

CExporter::~CExporter() {
    
}

void CExporter::exportScene(stringc file_path) {

    wd = devices->getWorkingDirectory().c_str();
    //wd+= "/";
    
	export_file = fopen(stringc(stringc(file_path.c_str()) + stringc("_temp")).c_str(), "w");
    
    //HEADER
	fprintf(export_file, "<?xml version=\"1.0\"?>\n\n");
    
    //ROOT
    fprintf(export_file, "<rootScene>\n\n");
    //---------------------------------------------------------------------------------------------
	//-----------------------------------CONFIG----------------------------------------------------
	//---------------------------------------------------------------------------------------------
	exportConfig();
    
    //SCENE
    fprintf(export_file, "\t<scene>\n\n");
    
    //---------------------------------------------------------------------------------------------
	//-----------------------------------TERRAINS--------------------------------------------------
	//---------------------------------------------------------------------------------------------
    exportTerrains();
    //---------------------------------------------------------------------------------------------
	//-----------------------------------OBJECTS---------------------------------------------------
	//---------------------------------------------------------------------------------------------
    exportObjects();
    //---------------------------------------------------------------------------------------------
	//-----------------------------------TREES-----------------------------------------------------
	//---------------------------------------------------------------------------------------------
    exportTrees();
	//---------------------------------------------------------------------------------------------
	//-----------------------------------LIGHTS--------------------------------------------------
	//---------------------------------------------------------------------------------------------
    exportLights();
	//---------------------------------------------------------------------------------------------
	//-----------------------------------VOLUME LIGHTS---------------------------------------------
	//---------------------------------------------------------------------------------------------
    exportVolumeLights();
	//---------------------------------------------------------------------------------------------
	//-----------------------------------WATER SURFACES--------------------------------------------
	//---------------------------------------------------------------------------------------------
	exporterWaterSurfaces();
    //---------------------------------------------------------------------------------------------
	//-----------------------------------FINISH EXPORTATION----------------------------------------
	//---------------------------------------------------------------------------------------------
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
		if (line != "\n")
			file_to_copy << line.c_str() << std::endl;
	}

	exported_file.close();
	file_to_copy.close();

	//REMOVE THE TEMP FILE
	if (remove(stringc(stringc(file_path.c_str()) + stringc("_temp")).c_str()) != 0) {
		devices->addInformationDialog(L"Error", L"Error when deleting the temporary file... \n Not a problem.", EMBF_OK);
	}

}

//---------------------------------------------------------------------------------------------
//-----------------------------------XML EXPORTERS---------------------------------------------
//---------------------------------------------------------------------------------------------
void CExporter::exportConfig() {
	//CONFIG
	fprintf(export_file, "\t<config>\n\n");

	//SCENE INFORMATION
	fprintf(export_file, "\t\t <numberOfObjects value=\"%u\" />\n\n", devices->getCoreData()->getAllSceneNodes().size());
    
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
					stringc(devices->getCoreData()->getEffectRenderCallbacks()->operator[](i)->getPixelValues()->operator[](ei)).c_str());
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
		fprintf(export_file, "\t\t\t\t <vertex shader=\n\"%s\" />\n", stringc(devices->getCoreData()->getShaderCallbacks()->operator[](i)->getVertexShader()).c_str());
		fprintf(export_file, "\t\t\t\t <pixel shader=\n\"%s\" />\n", stringc(devices->getCoreData()->getShaderCallbacks()->operator[](i)->getPixelShader()).c_str());
		fprintf(export_file, "\t\t\t\t <constants value=\"%s\" />\n", stringc(devices->getCoreData()->getShaderCallbacks()->operator[](i)->getConstants()).c_str());
        
		fprintf(export_file, "\t\t\t </materialType>\n\n");
	}
	fprintf(export_file, "\n\t\t </materialTypes>\n\n");
    
	//END CONFIG
	fprintf(export_file, "\t</config>\n\n\n\n");
}

void CExporter::exportTerrains() {
	for (int i=0; i < devices->getCoreData()->getTerrainNodes()->size(); i++) {
        ISceneNode *node = devices->getCoreData()->getTerrainNodes()->operator[](i);
        
        devices->getCoreData()->getTerrainPaths()->operator[](i).remove(wd.c_str());
        
        fprintf(export_file, "\t\t <terrain>\n\n");
        fprintf(export_file, "\t\t\t <path file=\"%ls\" />\n\n", devices->getCoreData()->getTerrainPaths()->operator[](i).c_str());
        if (node->getType() == ESNT_OCTREE) {
			fprintf(export_file, "\t\t\t <type esnt=\"octtree\" mppn=\"%u\" />\n\n", devices->getCoreData()->getTerrainMinPolysPerNode()->operator[](i));
		} else if (node->getType() == ESNT_TERRAIN) {
			fprintf(export_file, "\t\t\t <type esnt=\"heightMap\" />\n\n");
		} else {
            fprintf(export_file, "\t\t\t <type esnt=\"mesh\" />\n\n");
        }
        
		exportFactory("\t\t\t", node);

        fprintf(export_file, "\t\t\t <name c8name=\"%s\" />\n\n", node->getName());
        
        //MATERIALS
		exportMaterials("\t\t\t", node);
        
        exportTransformations("\t\t\t", node);

		fprintf(export_file, "\t\t\t <visible bool=\"%d\" />\n", node->isVisible());
		fprintf(export_file, "\t\t\t <shadowMode mode=\"%d\" />\n", getShadowMode(node));

        
        //END TERRAIN
        fprintf(export_file, "\n\t\t </terrain>\n\n");
    }
}

void CExporter::exportObjects() {
	for (int i=0; i < devices->getCoreData()->getObjectsData()->size(); i++) {
		ISceneNode *node = devices->getCoreData()->getObjectsData()->operator[](i).getNode();
        
		devices->getCoreData()->getObjectsData()->operator[](i).getPath().remove(wd.c_str());
        
        fprintf(export_file, "\t\t <object>\n\n");
        
		fprintf(export_file, "\t\t\t <path file=\"%ls\" />\n\n", devices->getCoreData()->getObjectsData()->operator[](i).getPath().c_str());
        fprintf(export_file, "\t\t\t <name c8name=\"%s\" />\n\n", node->getName());
        
		exportMaterials("\t\t\t", node);
        exportTransformations("\t\t\t", node);

		fprintf(export_file, "\t\t\t <visible bool=\"%d\" />\n", node->isVisible());
        fprintf(export_file, "\t\t\t <shadowMode mode=\"%d\" />\n", getShadowMode(node));

		fprintf(export_file, "\t\t\t <actions>\n");
		for (u32 ai=0; ai < devices->getCoreData()->getObjectsData()->operator[](i).getActions()->size(); ai++) {
			CAction *action = devices->getCoreData()->getObjectsData()->operator[](i).getActions()->operator[](ai);
			fprintf(export_file, stringc(stringc("\t\t\t\t ") + stringc(action->getXMLValues()) + "\n").c_str());
		}
		fprintf(export_file, "\t\t\t </actions>\n\n");

        fprintf(export_file, "\t\t </object>\n\n");
        
    }
}

void CExporter::exportTrees() {
	for (int i=0; i < devices->getCoreData()->getTreesData()->size(); i++) {
		ISceneNode *node = devices->getCoreData()->getTreesData()->operator[](i).getNode();
        
		devices->getCoreData()->getTreesData()->operator[](i).getPath().remove(wd.c_str());
        
        fprintf(export_file, "\t\t <tree>\n\n");
        
		fprintf(export_file, "\t\t\t <path file=\"%ls\" />\n\n", devices->getCoreData()->getTreesData()->operator[](i).getPath().c_str());
        fprintf(export_file, "\t\t\t <name c8name=\"%s\" />\n\n", node->getName());
        
		exportMaterials("\t\t\t", node);
        
        exportTransformations("\t\t\t", node);
        
		fprintf(export_file, "\t\t\t <visible bool=\"%d\" />\n", node->isVisible());
		fprintf(export_file, "\t\t\t <shadowMode mode=\"%d\" />\n", getShadowMode(node));
        
        fprintf(export_file, "\t\t </tree>\n\n");
    }
}

void CExporter::exportLights() {
	for (int i=0; i < devices->getCoreData()->getLightsData()->size(); i++) {
		ISceneNode *node = devices->getCoreData()->getLightsData()->operator[](i).getNode();
        
        fprintf(export_file, "\t\t <light>\n\n");
        fprintf(export_file, "\t\t\t <name c8name=\"%s\" />\n\n", node->getName());
        
        fprintf(export_file, "\t\t\t <position X=\"%f\" Y=\"%f\" Z=\"%f\" />\n", node->getPosition().X,
                node->getPosition().Y, node->getPosition().Z);
        fprintf(export_file, "\t\t\t <target X=\"%f\" Y=\"%f\" Z=\"%f\" />\n\n", node->getRotation().X,
                node->getRotation().Y, node->getRotation().Z);
        
        fprintf(export_file, "\t\t\t <diffuseColor r=\"%f\" g=\"%f\" b=\"%f\" a=\"%f\" />\n", 
				((ILightSceneNode *)node)->getLightData().DiffuseColor.r, 
                ((ILightSceneNode *)node)->getLightData().DiffuseColor.g,
                ((ILightSceneNode *)node)->getLightData().DiffuseColor.b,
				((ILightSceneNode *)node)->getLightData().DiffuseColor.a);
        
        fprintf(export_file, "\t\t\t <ambiantColor r=\"%f\" g=\"%f\" b=\"%f\" a=\"%f\" />\n", 
				((ILightSceneNode *)node)->getLightData().AmbientColor.r, 
                ((ILightSceneNode *)node)->getLightData().AmbientColor.g,
                ((ILightSceneNode *)node)->getLightData().AmbientColor.b,
				((ILightSceneNode *)node)->getLightData().AmbientColor.a);
        
        fprintf(export_file, "\t\t\t <specularColor r=\"%f\" g=\"%f\" b=\"%f\" a=\"%f\" />\n\n", 
				((ILightSceneNode *)node)->getLightData().SpecularColor.r, 
                ((ILightSceneNode *)node)->getLightData().SpecularColor.g,
                ((ILightSceneNode *)node)->getLightData().SpecularColor.b,
				((ILightSceneNode *)node)->getLightData().SpecularColor.a);
        
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
			fprintf(export_file, "\t\t\t\t\t <strength value=\"%f\" />\n", ldata.getLensFlareStrengthFactor());
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
}

void CExporter::exportVolumeLights() {
	for (int i=0; i < devices->getCoreData()->getVolumeLightsData()->size(); i++) {
		IVolumeLightSceneNode *node = (IVolumeLightSceneNode *)devices->getCoreData()->getVolumeLightsData()->operator[](i).getNode();
        
        fprintf(export_file, "\t\t <volumeLight>\n\n");
        
        fprintf(export_file, "\t\t\t <name c8name=\"%s\" />\n\n", node->getName());
        
		fprintf(export_file, "\t\t\t <footColor r=\"%u\" g=\"%u\" b=\"%u\" a=\"%u\" />\n",
				node->getFootColor().getRed(), node->getFootColor().getGreen(), node->getFootColor().getBlue(), 
				node->getFootColor().getAlpha());
		fprintf(export_file, "\t\t\t <tailColor r=\"%u\" g=\"%u\" b=\"%u\" a=\"%u\" />\n",
				node->getTailColor().getRed(), node->getTailColor().getGreen(), node->getTailColor().getBlue(), 
				node->getTailColor().getAlpha());
		fprintf(export_file, "\t\t\t <subDivideU value=\"%u\" />\n", node->getSubDivideU());
		fprintf(export_file, "\t\t\t <subDivideV value=\"%u\" />\n\n", node->getSubDivideV());

		exportMaterials("\t\t\t", node);
        exportTransformations("\t\t\t", node);

		fprintf(export_file, "\t\t\t <visible bool=\"%d\" />\n", node->isVisible());
        fprintf(export_file, "\t\t\t <shadowMode mode=\"%d\" />\n", getShadowMode(node));

        fprintf(export_file, "\t\t </volumeLight>\n\n");
        
    }
}

void CExporter::exporterWaterSurfaces() {
	for (u32 i=0; i < devices->getCoreData()->getWaterSurfaces()->size(); i++) {
		CWaterSurface *surf = devices->getCoreData()->getWaterSurfaces()->operator[](i).getWaterSurface();
		ISceneNode *node = devices->getCoreData()->getWaterSurfaces()->operator[](i).getNode();
		stringw surfpath = devices->getCoreData()->getWaterSurfaces()->operator[](i).getMeshPath();

		fprintf(export_file, "\t\t <waterSurface>\n\n");

		fprintf(export_file, "\t\t\t <mesh path=\"%s\" />\n", surfpath.c_str());
		fprintf(export_file, "\t\t\t <name c8name=\"%s\" />\n\n", node->getName());

		exportTransformations("\t\t\t", node);
		exportMaterials("\t\t\t", node);

		fprintf(export_file, "\t\t\t <visible bool=\"%d\" />\n", node->isVisible());
		fprintf(export_file, "\t\t\t <shadowMode mode=\"%d\" />\n", getShadowMode(node));

		stringc shaderPackagePath = devices->getCoreData()->getWaterSurfaces()->operator[](i).getPackagePath();
		shaderPackagePath.remove(devices->getWorkingDirectory().c_str());
		fprintf(export_file, "\t\t\t <shaderPackagePath value=\"%s\" />\n", shaderPackagePath.c_str());

		fprintf(export_file, "\t\t </waterSurface>\n\n");
	}
}
//---------------------------------------------------------------------------------------------
//-----------------------------------INFORMATIONS EXPORTERS------------------------------------
//---------------------------------------------------------------------------------------------
void CExporter::exportFactory(stringc tabs, ISceneNode *node) {
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
}

void CExporter::exportMaterials(stringc tabs, ISceneNode *node) {
	fprintf(export_file, stringc(tabs + " <materials>\n").c_str());
	for (u32 mi=0; mi < node->getMaterialCount(); mi++) {
		stringc _tabs = tabs + "\t\t";
		fprintf(export_file, stringc(tabs + " \t <material id=\"%u\">\n").c_str(), mi);


		//TEXTURES
		fprintf(export_file, stringc(_tabs + " <textures>\n").c_str());
		for (u32 i=0; i < irr::video::MATERIAL_MAX_TEXTURES; i++) {
			stringc texturePath = "";
			if (node->getMaterial(mi).TextureLayer[i].Texture) {
				texturePath = node->getMaterial(mi).TextureLayer[i].Texture->getName().getPath();
				texturePath.remove(devices->getWorkingDirectory().c_str());
			}
			fprintf(export_file, _tabs.c_str());
			fprintf(export_file, "\t <texture id=\"%u\" path=\"%s\" />\n", i, texturePath.c_str());
		}
		fprintf(export_file, stringc(_tabs + " <textures>\n\n").c_str());

		//COLORS
		fprintf(export_file, stringc(_tabs + " <diffuseColor r=\"%u\" g=\"%u\" b=\"%u\" a=\"%u\" />\n").c_str(),
				node->getMaterial(mi).DiffuseColor.getRed(),
				node->getMaterial(mi).DiffuseColor.getGreen(),
				node->getMaterial(mi).DiffuseColor.getBlue(),
				node->getMaterial(mi).DiffuseColor.getAlpha());
		fprintf(export_file, stringc(_tabs + " <ambiantColor r=\"%u\" g=\"%u\" b=\"%u\" a=\"%u\" />\n").c_str(),
				node->getMaterial(mi).AmbientColor.getRed(),
				node->getMaterial(mi).AmbientColor.getGreen(),
				node->getMaterial(mi).AmbientColor.getBlue(),
				node->getMaterial(mi).AmbientColor.getAlpha());
		fprintf(export_file, stringc(_tabs + " <specularColor r=\"%u\" g=\"%u\" b=\"%u\" a=\"%u\" />\n").c_str(),
				node->getMaterial(mi).SpecularColor.getRed(),
				node->getMaterial(mi).SpecularColor.getGreen(),
				node->getMaterial(mi).SpecularColor.getBlue(),
				node->getMaterial(mi).SpecularColor.getAlpha());
		fprintf(export_file, stringc(_tabs + " <emissiveColor r=\"%u\" g=\"%u\" b=\"%u\" a=\"%u\" />\n\n").c_str(),
				node->getMaterial(mi).EmissiveColor.getRed(),
				node->getMaterial(mi).EmissiveColor.getGreen(),
				node->getMaterial(mi).EmissiveColor.getBlue(),
				node->getMaterial(mi).EmissiveColor.getAlpha());

		//LIGHTING
		fprintf(export_file, stringc(_tabs + " <lighting value=\"%u\" />\n\n").c_str(), node->getMaterial(mi).Lighting);

		//MISC PARAMS
		fprintf(export_file, stringc(_tabs + " <materialTypeParam1 value=\"%f\" />\n").c_str(), node->getMaterial(mi).MaterialTypeParam);
		fprintf(export_file, stringc(_tabs + " <materialTypeParam2 value=\"%f\" />\n\n").c_str(), node->getMaterial(mi).MaterialTypeParam2);

		fprintf(export_file, stringc(_tabs + " <shininess value=\"%f\" />\n").c_str(), node->getMaterial(mi).Shininess);
		fprintf(export_file, stringc(_tabs + " <thickness value=\"%f\" />\n\n").c_str(), node->getMaterial(mi).Thickness);

		//FLAGS
		fprintf(export_file, stringc(_tabs + " <antiAliasing value=\"%u\" />\n").c_str(), node->getMaterial(mi).AntiAliasing);
		fprintf(export_file, stringc(_tabs + " <backfaceCulling value=\"%u\" />\n").c_str(), node->getMaterial(mi).BackfaceCulling);
		fprintf(export_file, stringc(_tabs + " <colorMask value=\"%u\" />\n").c_str(), node->getMaterial(mi).ColorMask);
		fprintf(export_file, stringc(_tabs + " <colorMaterial value=\"%u\" />\n").c_str(), node->getMaterial(mi).ColorMaterial);
		fprintf(export_file, stringc(_tabs + " <fogEnable value=\"%u\" />\n").c_str(), node->getMaterial(mi).FogEnable);
		fprintf(export_file, stringc(_tabs + " <frontfaceCulling value=\"%u\" />\n").c_str(), node->getMaterial(mi).FrontfaceCulling);
		fprintf(export_file, stringc(_tabs + " <gouraudShading value=\"%u\" />\n").c_str(), node->getMaterial(mi).GouraudShading);
		fprintf(export_file, stringc(_tabs + " <normalizeNormals value=\"%u\" />\n").c_str(), node->getMaterial(mi).NormalizeNormals);
		fprintf(export_file, stringc(_tabs + " <zBuffer value=\"%u\" />\n").c_str(), node->getMaterial(mi).ZBuffer);
		fprintf(export_file, stringc(_tabs + " <zWriteEnable value=\"%u\" />\n\n").c_str(), node->getMaterial(mi).ZWriteEnable);

		//MATERIAL TYPE
		s32 matTypeNumber = node->getMaterial(mi).MaterialType;
		if (matTypeNumber > devices->getCore()->getNumberOfBuildInMaterialTypes()) {
			for (u32 i=0; i < devices->getCoreData()->getShaderCallbacks()->size(); i++) {
				CShaderCallback *callback = devices->getCoreData()->getShaderCallbacks()->operator[](i);
				if (node->getMaterial(mi).MaterialType == callback->getMaterial()) {
					matTypeNumber = -i-1;
					break;
				}
			}
		}
		fprintf(export_file, stringc(_tabs + " <materianType value=\"%d\" />\n\n").c_str(), matTypeNumber);

		fprintf(export_file, stringc(tabs + " \t </material>\n").c_str());
	}
	fprintf(export_file, stringc(tabs + " </materials>\n").c_str());
}

void CExporter::exportTransformations(stringc tabs, ISceneNode *node) {
	fprintf(export_file, stringc(tabs + " <position X=\"%f\" Y=\"%f\" Z=\"%f\" />\n").c_str(), node->getPosition().X, node->getPosition().Y,
            node->getPosition().Z);
	fprintf(export_file, stringc(tabs + " <rotation X=\"%f\" Y=\"%f\" Z=\"%f\" />\n").c_str(), node->getRotation().X, node->getRotation().Y,
            node->getRotation().Z);
	fprintf(export_file, stringc(tabs + " <scale X=\"%f\" Y=\"%f\" Z=\"%f\" />\n\n").c_str(), node->getScale().X, node->getScale().Y,
            node->getScale().Z);
}
//---------------------------------------------------------------------------------------------
//-----------------------------------GET PARAMETERS--------------------------------------------
//---------------------------------------------------------------------------------------------
E_SHADOW_MODE CExporter::getShadowMode(ISceneNode *node) {
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

	if (!devices->getXEffect()->isNodeShadowed(node)) {
		shadowMode = ESM_NO_SHADOW;
	}

	return shadowMode;
}
