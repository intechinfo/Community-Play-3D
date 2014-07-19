//
//  CExporter.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#ifndef __C_UI_EXPORTER_H_INCLUDED__
#define __C_UI_EXPORTER_H_INCLUDED__

#include "../Device/CDevices.h"

class CExporter {
    
public:
    
    CExporter(CDevices *_devices);
    ~CExporter();
    
    void exportScene(stringc file_path);
	void exportCamerasConfig();
    
private:
    
	//-----------------------------------
	//DEVICES AND DATAS
    CDevices *devices;

	FILE *export_file;
	stringc wd;
	//-----------------------------------

	//-----------------------------------
	//METHODS
	//XML EXPORTERS
	void exportConfig();
	void exportTerrains();
	void exportObjects();
	void exportTrees();
	void exportLights();
	void exportVolumeLights();
	void exporterWaterSurfaces();

	void exportSkybox();

	//INFORMATIONS EXPORTERS
	void exportMaterials(stringc tabs, ISceneNode *node);
	void exportTransformations(stringc tabs, ISceneNode *node);
	void exportFactory(stringc tabs, ISceneNode *node);
	void exportPhysics(stringc tabs, ISceneNode *node);
	
	//GET PARAMETERS
	E_SHADOW_MODE getShadowMode(ISceneNode *node);
	//-----------------------------------
    
};

#endif