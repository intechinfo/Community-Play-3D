//
//  CImporter.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 01/10/12.
//
//

#ifndef __C_UI_IMPORTER_H_INCLUDED__
#define __C_UI_IMPORTER_H_INCLUDED__

#include <ISSWEImporter.h>

#include "../Device/CDevices.h"

#include "../UserInterfaces/CUIWindowEditNode.h"

class CImporter : public ISSWEImporter {

public:

	CImporter(CDevices *_devices);
	~CImporter();

	void import_t();
	void setPathOfFile_t(stringc path) { pathOfFile = path; }

	void importScene(stringc file_path);
	void newImportScene(stringc file_path);

	void importCamerasConfig();

private:

	//--------------------------
    //DEVICES AND DATAS
	CDevices *devices;

	ISceneManager *smgr;

	IrrXMLReader *xmlReader;
	std::string element;
	u32 numberOfObjects;

	stringc pathOfFile;
	//--------------------------

	//--------------------------
    //METHODS
	//READING XML FILES
	void readNextElemement();
	void read(std::string node);
	void readWithNextElement(std::string node, std::string nextNode);

	//BUILDING NODES
	void buildTerrain();
	void buildTree();
	void buildObject();
	void buildLight();
	void buildVolumeLight();
	void buildWaterSurface();

	void buildSkybox();

	//BUILDING CONFIGURATIONS
	void readConfig();
	void readEffects();
	void readMaterialShaderCallbacks();
	void readScripts();
	SData readFactory(ISceneNode *_node, IMesh *_mesh);
	void copySDataFactoryOf(SData *data, SData toCopy);
	void readMaterials(ISceneNode *_node);
	void readTransformations(ISceneNode *_node);
	void readViewModes(ISceneNode *_node);
	void readPhysics(SData *data);

	//BUILDING PARAMETERS
	SColor buildSColor();
	SColorf buildSColorf();

	vector3df buildVector3df();
	dimension2df buildDimension2df();

	template<class T>
	vector3d<T> buildVector3d();
	template<class T>
	dimension2d<T> buildDimension2d();

	//--------------------------
};

#endif
