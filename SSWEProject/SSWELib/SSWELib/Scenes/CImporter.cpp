//
//  CImporter.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 01/10/12.
//
//

#include "stdafx.h"
#include "CImporter.h"

#include "../Renders/XEffect/Interfaces/CFilterCallback.h"

#include "irrbullet.h"

#include "../../../SSWERenders/Renders/Materials/CNormalMappingMaterial.h"

#include "../UserInterfaces/MaterialEditor/CMaterialEditorFactory.h"

CImporter::CImporter(CDevices *_devices) {
    devices = _devices;

	smgr = devices->getSceneManager();
}

CImporter::~CImporter() {
    devices = 0;
	pathOfFile = "";
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

	pathOfFile = file_path;
	newImportScene(pathOfFile);
	return;
}

//---------------------------------------------------------------------------------------------
//-----------------------------------BUILDING NODES--------------------------------------------
//---------------------------------------------------------------------------------------------

void CImporter::buildTerrain() {
	read("path");
	stringc path = xmlReader->getAttributeValue("file");

	read("type");
	stringc type = xmlReader->getAttributeValue("esnt");

	STerrainsData tdata;
	IMesh *mesh = 0;
    ISceneNode *node = 0;
	u32 minPolysPerNode = 0;
	if (type == "heightMap") {
		node = smgr->addTerrainSceneNode(path.c_str(), 0, -1, vector3df(0.f, 0.f, 0.f),
                                                vector3df(0.f, 0.f, 0.f), vector3df(1.f, 0.1f, 1.f),
												SColor (255, 255, 255, 255), 1024, ETPS_17, 50);
		tdata.setMinPolysPerNode(0);
	} else if (type == "octtree") {
		minPolysPerNode = xmlReader->getAttributeValueAsInt("mppn");
		mesh = smgr->getMesh(path.c_str());
		u32 numberOfVertices = 0;
		for (u32 i=0; i < mesh->getMeshBufferCount(); i++) {
			numberOfVertices += mesh->getMeshBuffer(i)->getVertexCount();
		}
		assert(mesh != 0);
		/*node = smgr->addOctreeSceneNode(mesh, 0, -1, (numberOfVertices % minPolysPerNode == 0) ?
													 numberOfVertices/(minPolysPerNode+1)
													 : numberOfVertices/minPolysPerNode);*/
        mesh->setHardwareMappingHint(EHM_DYNAMIC, EBT_VERTEX_AND_INDEX);
		node = smgr->addOctreeSceneNode(mesh, 0, -1, minPolysPerNode);
	} else if (type == "mesh") {
		mesh = devices->getSceneManager()->getMesh(path.c_str());

		if (mesh) {
			assert(mesh != 0);

			if (xmlReader->getAttributeValueAsInt("tangents") == 1) {
				mesh = smgr->getMeshManipulator()->createMeshWithTangents(mesh, true, true, false, true);
			}
			mesh->setHardwareMappingHint(EHM_DYNAMIC, EBT_VERTEX_AND_INDEX);
			node = smgr->addMeshSceneNode(mesh, 0, -1);
		} else {
			read("terrain");
		}
	}

	SData data = readFactory(node, mesh);

	if (node) {
		read("name");
		stringc name = xmlReader->getAttributeValue("c8name");
		node->setName(name.c_str());

		readMaterials(node);
		readTransformations(node);
		readViewModes(node);

        devices->getCollisionManager()->setCollisionToAnOctTreeNode(node);

		tdata.setMesh(mesh);
		tdata.setNode(node);
		tdata.setPath(path);
		tdata.setMinPolysPerNode(minPolysPerNode);
		tdata.setType(node->getType());

		copySDataFactoryOf(&tdata, data);
		readPhysics(&tdata);

		devices->getCoreData()->getTerrainsData()->push_back(tdata);
	}
}

void CImporter::buildTree() {
	read("path");
	stringc path = xmlReader->getAttributeValue("file");

	IMesh *mesh = 0;
    ISceneNode *node = 0;

	mesh = smgr->getMesh(path.c_str());
	node = smgr->addOctreeSceneNode(mesh, 0, -1, 4096*16);
	//node = smgr->addMeshSceneNode(mesh, 0, -1);

	if (node) {
		read("name");
		stringc name = xmlReader->getAttributeValue("c8name");
		node->setName(name.c_str());

		readMaterials(node);
		readTransformations(node);
		readViewModes(node);

		devices->getCollisionManager()->setCollisionToAnOctTreeNode(node);
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
		node = (IAnimatedMeshSceneNode *)smgr->addCubeSceneNode(50.f);
		devices->getSceneManager()->getMeshManipulator()->recalculateNormals(((IMeshSceneNode*)node)->getMesh(), true);
	} else if (path == "sphere") {
		node = (IAnimatedMeshSceneNode *)smgr->addSphereSceneNode();
	} else if (path == "hillPlaneMesh") {
		mesh = smgr->addHillPlaneMesh("#new_hille_plane_mesh",
									  dimension2df(25, 25), dimension2du(25, 25),
									  0, 0, dimension2df(0.f, 0.f), dimension2df(1.f, 1.f));
		node = smgr->addAnimatedMeshSceneNode(mesh);
	} else if (path == "billboard") {
		node = (IAnimatedMeshSceneNode *)smgr->addBillboardSceneNode();
	} else {
		mesh = smgr->getMesh(path.c_str());
		if (mesh->getMeshType() == EAMT_SKINNED) {
			((ISkinnedMesh*)mesh)->convertMeshToTangents();
		}
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

		SData data = readFactory(node, mesh);

		readMaterials(node);
		readTransformations(node);
		readViewModes(node);

		//if (node->getType() == ESNT_BILLBOARD) {
		//	((IBillboardSceneNode*)node)->setSize(dimension2df(node->getScale().X, node->getScale().Y));
		//}

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
			devices->getXEffect()->addNodeToDepthPass(node);
		} else if (path == "cube") {
			devices->getCollisionManager()->setCollisionFromBoundingBox(node);
			devices->getXEffect()->addNodeToDepthPass(node);
		} else if (path == "billboard") {
			devices->getCollisionManager()->setCollisionFromBoundingBox(node);
		} else if (path == "hillPlaneMesh") {
			devices->getCollisionManager()->setCollisionFromBoundingBox(node);
		} else {
			devices->getCollisionManager()->setCollisionToAnAnimatedNode(node);
		}

        SObjectsData odata(mesh, node, path);
		copySDataFactoryOf(&odata, data);

		readPhysics(&odata);

		odata.getAnimationMeshes()->clear();
		odata.setActions(&actions);
		for (u32 i=0; i < actions.size(); i++) {
			IAnimatedMesh *smesh = devices->getSceneManager()->getMesh(actions[i]->getAnimationPath());
			odata.getAnimationMeshes()->push_back(smesh);
		}

		devices->getCoreData()->getObjectsData()->push_back(odata);
	}
}

void CImporter::buildLight() {
	ILightSceneNode *node = smgr->addLightSceneNode();
	SShadowLight shadowLight(1024, vector3df(0,0,0), vector3df(0,0,0), SColor(255, 255, 255, 255), 20.0f, 1000.f, 89.99f * DEGTORAD, false);

	read("name");
	node->setName(xmlReader->getAttributeValue("c8name"));

	//TRANSFORMATIONS
	read("position");
	vector3df position = buildVector3df();
    node->setPosition(position);
	shadowLight.setPosition(position);
    read("target");
    vector3df target = buildVector3df();
    node->setRotation(target);
	shadowLight.setTarget(target);

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
	read("autoRecalculate");
	shadowLight.setAutoRecalculate(xmlReader->getAttributeValueAsInt("value"));

	read("shadows");
	shadowLight.setShadowMapResolution(xmlReader->getAttributeValueAsFloat("resol"));

	SLightsData ldata(node);

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
		devices->getXEffect()->addNodeToLightScatteringPass(lfBillBoard);

		lfNode = new CLensFlareSceneNode(lfMeshNode, devices->getSceneManager());
		lfNode->setParent(lfMeshNode);
		lfNode->setName(stringc(stringc(node->getName()) + stringc("_flare_node")).c_str());
		devices->getVideoDriver()->addOcclusionQuery(lfMeshNode, lfMeshNode->getMesh());
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
		ldata.setLensFlareStrengthFactor(xmlReader->getAttributeValueAsFloat("value"));
		read("texture");
		lfNode->setMaterialTexture(0, devices->getVideoDriver()->getTexture(xmlReader->getAttributeValue("path")));
		read("falseOcclusion");
		read("position");
		lfNode->setPosition(buildVector3df());

		//devices->getXEffect()->addShadowToNode(lfMeshNode, devices->getXEffectFilterType(), ESM_EXCLUDE);
		devices->getXEffect()->addShadowToNode(lfBillBoard, devices->getXEffectFilterType(), ESM_EXCLUDE);
		devices->getXEffect()->addShadowToNode(lfNode, devices->getXEffectFilterType(), ESM_EXCLUDE);

		ldata.setLensFlareMeshSceneNode(lfMeshNode);
		ldata.setLensFlareBillboardSceneNode(lfBillBoard);
		ldata.setLensFlareSceneNode(lfNode);
	}

	if (devices->getVideoDriver()->getDriverType() == EDT_OPENGL)
        devices->getNormalMappingMaterial()->addLight(node);

	devices->getXEffect()->addShadowLight(shadowLight);
	devices->getCoreData()->getLightsData()->push_back(ldata);

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
	stringc meshPath = xmlReader->getAttributeValue("path");
	RealisticWaterSceneNode *waterSurface = new RealisticWaterSceneNode(smgr, 512, 512, devices->getWorkingDirectory() + "shaders/Materials/Water/",
                                                                        dimension2du(512, 512), devices->getSceneManager()->getRootSceneNode());
	ISceneNode *node = waterSurface;

	if (node) {
		read("name");
        waterSurface->setName(xmlReader->getAttributeValue("c8name"));
		waterSurface->getWaterSceneNode()->setName(xmlReader->getAttributeValue("c8name"));

		readTransformations(waterSurface);
		readMaterials(node);
		readViewModes(waterSurface->getWaterSceneNode());

		read("shaderPackagePath");
		stringc shaderPackagePath = xmlReader->getAttributeValue("value");
		CShaderCallback *callback = new CShaderCallback(devices->getVideoDriver()->getDriverType());
		IFileSystem *fileSystem = devices->getDevice()->getFileSystem();
		if (fileSystem->addZipFileArchive(shaderPackagePath.c_str())) {
			stringc vertexLines = devices->getCore()->getStringcFromIReadFile("vertex.vbs");
			stringc pixelLines = devices->getCore()->getStringcFromIReadFile("pixel.fbs");
			stringc constantsLines = devices->getCore()->getStringcFromIReadFile("constants.cbs");
			callback->setDevice(devices->getDevice());
			callback->setVertexShader(vertexLines.c_str());
			callback->setVertexShaderType(EVST_VS_3_0);
			callback->setPixelShader(pixelLines.c_str());
			callback->setPixelShaderType(EPST_PS_3_0);
			callback->setConstants(constantsLines.c_str());
			callback->buildMaterial(devices->getVideoDriver());
			fileSystem->removeFileArchive(fileSystem->getFileArchive(fileSystem->getFileArchiveCount()-1));
		}
		node->setMaterialType((E_MATERIAL_TYPE)callback->getMaterial());

		devices->getCollisionManager()->setCollisionFromBoundingBox(waterSurface->getWaterSceneNode());
		SWaterSurfacesData wsdata(waterSurface, callback, shaderPackagePath, meshPath);

		readPhysics(&wsdata);

		devices->getCoreData()->getWaterSurfaces()->push_back(wsdata);
	}

}

void CImporter::buildSkybox() {
	if (!devices->getSkyBox()) {
		devices->getVideoDriver()->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);
		ISceneNode* skyboxNode = smgr->addSkyBoxSceneNode(
			devices->getVideoDriver()->getTexture("data/Lights/glacier_up.png"),
			devices->getVideoDriver()->getTexture("data/Lights/glacier_dn.png"),
			devices->getVideoDriver()->getTexture("data/Lights/glacier_lf.png"),
			devices->getVideoDriver()->getTexture("data/Lights/glacier_rt.png"),
			devices->getVideoDriver()->getTexture("data/Lights/glacier_ft.png"),
			devices->getVideoDriver()->getTexture("data/Lights/glacier_bk.png"));
		devices->getVideoDriver()->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, true);
		skyboxNode->setName("editor:skydome");
		devices->setSkyBox(skyboxNode);
		skyboxNode->setVisible(true);
	}

	u32 i=0;
	read("texture");
	while (element == "texture") {
		stringc path = xmlReader->getAttributeValue("path");
		devices->getSkyBox()->getMaterial(i).setTexture(0, devices->getVideoDriver()->getTexture(path.c_str()));
		readWithNextElement("texture", "skybox");
		i++;
	}
}

//---------------------------------------------------------------------------------------------
//-----------------------------------BUILDING CONFIGURATIONS-----------------------------------
//---------------------------------------------------------------------------------------------

void CImporter::readConfig() {
	read("config");

	//CONFIG SCENE
	read("numberOfObjects");
	numberOfObjects = xmlReader->getAttributeValueAsInt("value");

	//IF GRID
	if (!devices->isOnlyForPlaying()) {
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
	}

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

	//FPS CAMERA SETTINGS
	read("fpsCameraSettings");
	read("ellipsoidRadius");
	devices->getCollisionManager()->getFPSCameraSettings()->setEllipsoidRadius(buildVector3df());
	read("gravityPerSecond");
	devices->getCollisionManager()->getFPSCameraSettings()->setGravityPerSecond(buildVector3df());
	read("ellipsoidTranslation");
	devices->getCollisionManager()->getFPSCameraSettings()->setEllipsoidTranslation(buildVector3df());
	read("slidingValue");
	devices->getCollisionManager()->getFPSCameraSettings()->setSlidingValue(xmlReader->getAttributeValueAsFloat("value"));

	//EFFECTS
	readEffects();
	//CALLBACKS
	readMaterialShaderCallbacks();
	//SCRIPTS
	readScripts();

	//PHYSICS
	read("physics");
	read("gravity");
	devices->getBulletWorld()->setGravity(buildVector3df());
	read("physics");

	read("config");
}

void CImporter::readEffects() {
	read("depthPassEnabled");
	devices->getXEffect()->enableDepthPass(devices->getCore()->getU32(xmlReader->getAttributeValue("enabled")));
	read("lightScaterringPassEnabled");
	devices->getXEffect()->enableLightScatteringPass(devices->getCore()->getU32(xmlReader->getAttributeValue("enabled")));
	read("reflectionPassEnabled");
	devices->getXEffect()->setReflectionPassEnabled(devices->getCore()->getU32(xmlReader->getAttributeValue("enabled")));

	read("effect");
	readWithNextElement("postProcessingEffect", "effect");

	CCoreFilterCallback *cCoreCallback = new CCoreFilterCallback(devices);

	while (element == "postProcessingEffect") {
		/// Read
		read("ppeName");
		stringc name = xmlReader->getAttributeValue("name");
		read("ppeShader");
        #ifndef _IRR_OSX_PLATFORM_
		stringc shader = stringc(xmlReader->getAttributeValue("shader")).replace("&quot", "\"").replace("\n", "");
        #else
        stringc shader = stringc(xmlReader->getAttributeValue("shader")).replace("&quot", "\"").replace("\r", "");
        #endif
		read("ppeCallback");
        #ifndef _IRR_OSX_PLATFORM_
		stringc callback = stringc(xmlReader->getAttributeValue("callback")).replace("&quot", "\"").replace("\n", "");
        #else
        stringc callback = stringc(xmlReader->getAttributeValue("callback")).replace("&quot", "\"").replace("\r", "");
        #endif

		/// Create
		SFilter f;
		f.createLuaState();
		f.setPixelShader(shader.replace("\r", "\n"));
		f.setMaterial(devices->getXEffect()->addPostProcessingEffectFromString(f.getPixelShader().c_str()));

		f.setCallback(callback.replace("\r", "\n"));

		/// Complete
		name.remove(devices->getDevice()->getFileSystem()->getFileDir(name));
		f.setName(name);

		devices->getCoreData()->getEffectFilters()->push_back(f);
		u32 count = devices->getCoreData()->getEffectFilters()->size();
		cCoreCallback->createLuaState(devices->getCoreData()->getEffectFilters()->operator[](count-1).getLuaState());
		CFilterCallback *cb = new CFilterCallback(devices->getCoreData()->getEffectFilters()->operator[](count-1).getMaterial(), f.getLuaState(), f.getCallback());
		if (f.getMaterial() != -1) {
			devices->getXEffect()->setPostProcessingRenderCallback(devices->getCoreData()->getEffectFilters()->operator[](count-1).getMaterial(), cb);
		}
		devices->getCoreData()->getEffectFilters()->operator[](count-1).setPostProcessingCallback(cb);

		read("postProcessingEffect");
		readWithNextElement("postProcessingEffect", "effect");
	}

	delete cCoreCallback;
}

void CImporter::readMaterialShaderCallbacks() {
	if (element == "rootScene") {
		delete xmlReader;
		xmlReader = createIrrXMLReader(pathOfFile.c_str());
	}
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
        #ifndef _IRR_OSX_PLATFORM_
		read("vertex");
		vertex = stringc(xmlReader->getAttributeValue("shader")).replace("\n", "");
		read("pixel");
		pixel = stringc(xmlReader->getAttributeValue("shader")).replace("\n", "");
		read("constants");
		constants = stringc(xmlReader->getAttributeValue("value")).replace("\n", "");
        #else
		read("vertex");
		vertex = stringc(xmlReader->getAttributeValue("shader")).replace("\r", "");
		read("pixel");
		pixel = stringc(xmlReader->getAttributeValue("shader")).replace("\r", "");
		read("constants");
		constants = stringc(xmlReader->getAttributeValue("value")).replace("\r", "");
        #endif

		CShaderCallback *callback = new CShaderCallback(devices->getVideoDriver()->getDriverType());
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

void CImporter::readScripts() {
	read("scripts");
	readWithNextElement("script", "scripts");
	while (element == "script") {
		SScriptFile sf("", "");

		read("name");
		sf.setName(xmlReader->getAttributeValue("value"));
		read("file");
		stringw file = xmlReader->getAttributeValue("value");
		file.replace("(%quot)", "\"");
		sf.setFile(file.c_str());

		devices->getCoreData()->getScriptFiles()->push_back(sf);

		read("script");
		readWithNextElement("script", "scripts");
	}
}

SData CImporter::readFactory(ISceneNode *_node, IMesh *_mesh) {
	SData data;
	bool isTangents, isNormal, isAngleWeighted, isSmooth;

	/// Read
	read("factory");

	read("meshFactory");
	read("tangents");
	isTangents = devices->getCore()->getU32(xmlReader->getAttributeValue("value"));
	read("normals");
	isNormal = devices->getCore()->getU32(xmlReader->getAttributeValue("value"));
	read("angleWeighted");
	isAngleWeighted = devices->getCore()->getU32(xmlReader->getAttributeValue("value"));
	read("smooth");
	isSmooth = devices->getCore()->getU32(xmlReader->getAttributeValue("value"));

	/// Act
	if (isTangents && (isNormal || isAngleWeighted || isSmooth)) {
		devices->getSceneManager()->getMeshManipulator()->recalculateTangents(_mesh, isNormal, isSmooth, isAngleWeighted);
	}

	/// Read
	read("planarMapping");
	read("planarMapped");
	bool isPlanarMapped = devices->getCore()->getU32(xmlReader->getAttributeValue("value"));
	read("planarMappedValue");
	f32 isPlanarMappedValue = devices->getCore()->getF32(xmlReader->getAttributeValue("value"));

	/// Acts
	if (isPlanarMapped)
		devices->getSceneManager()->getMeshManipulator()->makePlanarTextureMapping(_mesh, isPlanarMappedValue);

	if (_mesh)
		_mesh->setDirty();

    if (_mesh) {
        for (u32 i=0; i < _mesh->getMeshBufferCount(); i++)
            _mesh->getMeshBuffer(i)->setDirty();
    }

	/// Configure
	data.setTangentRecalculated(isTangents);
	data.setNormalRecalculated(isNormal);
	data.setSmoothedRecalculated(isSmooth);
	data.setAngleWeightedRecalculated(isAngleWeighted);

	data.setPlanarTextureMapped(isPlanarMapped);
	data.setPlanarTextureMappedValue(isPlanarMappedValue);

	return data;
}

void CImporter::copySDataFactoryOf(SData *data, SData toCopy) {
	data->setTangentRecalculated(toCopy.wasTangentRecalculated());
	data->setNormalRecalculated(toCopy.wasNormalRecalculated());
	data->setSmoothedRecalculated(toCopy.wasSmoothed());
	data->setAngleWeightedRecalculated(toCopy.wasAngleWeighted());

	data->setPlanarTextureMapped(toCopy.wasPlanarTextureMapped());
	data->setPlanarTextureMappedValue(toCopy.wasPlanarTextureMappedValue());
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
			if (stringc(xmlReader->getAttributeValue("path")) != "") {

				stringc texturePath = stringc(xmlReader->getAttributeValue("path")).c_str();
				io::path ioTexturePath;
				core::getFileNameExtension(ioTexturePath, texturePath);

				if (ioTexturePath == "._normal_mapped") {
					stringc originalTexture = stringc(xmlReader->getAttributeValue("path")).remove("._normal_mapped");
					CMaterialEditorFactory *materialEditor = new CMaterialEditorFactory(devices);
					ITexture *tex = materialEditor->setTextureNormalMapped(texturePath, devices->getVideoDriver()->getTexture(originalTexture));
					_node->getMaterial(id).TextureLayer[i].Texture = tex;
					delete materialEditor;
				} else {
					ITexture *tex = devices->getVideoDriver()->getTexture(stringc(xmlReader->getAttributeValue("path")).c_str());
					_node->getMaterial(id).TextureLayer[i].Texture = tex;
				}
			}
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
		if (matS32 == (s32)EMT_NORMAL_MAP_SOLID && devices->getVideoDriver()->getDriverType() == EDT_OPENGL)
            _node->getMaterial(id).MaterialType = (E_MATERIAL_TYPE)devices->getNormalMappingMaterial()->getMaterialSolid();
		else
            _node->getMaterial(id).MaterialType = (E_MATERIAL_TYPE)matS32;
        
		if (matS32 < 0 && -matS32 <= devices->getCoreData()->getShaderCallbacks()->size()) {
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

	//RENDERS
	read("depthPassed");
	if (devices->getCore()->getU32(xmlReader->getAttributeValue("value")) == 1)
		devices->getXEffect()->addNodeToDepthPass(_node);

	read("lightScatteringPassed");
	if (devices->getCore()->getU32(xmlReader->getAttributeValue("value")) == 1)
		devices->getXEffect()->addNodeToLightScatteringPass(_node);
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

void CImporter::readPhysics(SData *data) {
	ISceneNode *_node = data->getNode();
	IMesh *_mesh = data->getMesh();

	read("physics");
	read("body");
	u32 ptype = devices->getCore()->getU32(xmlReader->getAttributeValue("type"));

	if (ptype == cp3d::core::ISData::EIPT_RIGID_BODY) {
		read("mass");
		f32 mass = devices->getCore()->getF32(xmlReader->getAttributeValue("value"));

		ICollisionShape *shape = 0;

		if (_node->getType() == ESNT_MESH || _node->getType() == ESNT_OCTREE) {
			shape = new IBvhTriangleMeshShape(_node, _mesh, mass);
		} else if (_node->getType() == ESNT_CUBE) {
			shape = new IBoxShape(_node, mass, false);
		} else if (_node->getType() == ESNT_SPHERE) {
			shape = new ISphereShape(_node, mass, false);
		} else if (_node->getType() == ESNT_TERRAIN) {
			//NEEDS FIXES
			/*IMesh *mesh = ((ITerrainSceneNode*)_node)->getMesh();
			CDynamicMeshBuffer *mb = new CDynamicMeshBuffer(EVT_2TCOORDS, EIT_32BIT);
			((ITerrainSceneNode*)_node)->getMeshBufferForLOD(*mb);
			shape = new IBvhTriangleMeshShape((ITerrainSceneNode*)_node, ((ITerrainSceneNode*)_node)->getMesh(), mass);*/
		}

		if (shape != 0) {
			IRigidBody *rbody = devices->getBulletWorld()->addRigidBody(shape);

			irr::f32 t = 0.5f;
            irr::f32 buoyancy = 10.8f;

            irr::core::array<SBuoyancyPoint> points;

            points.push_back(SBuoyancyPoint(irr::core::vector3df(t,t,t), buoyancy));
            points.push_back(SBuoyancyPoint(irr::core::vector3df(-t,t,t), buoyancy));
            points.push_back(SBuoyancyPoint(irr::core::vector3df(-t,t,-t), buoyancy));
            points.push_back(SBuoyancyPoint(irr::core::vector3df(t,t,-t), buoyancy));

            points.push_back(SBuoyancyPoint(irr::core::vector3df(-t,-t,t), buoyancy));
            points.push_back(SBuoyancyPoint(irr::core::vector3df(t,-t,t), buoyancy));
            points.push_back(SBuoyancyPoint(irr::core::vector3df(-t,-t,-t), buoyancy));
            points.push_back(SBuoyancyPoint(irr::core::vector3df(t,-t,-t), buoyancy));
			rbody->setBuoyancyPoints(points);

			if (!devices->isOnlyForPlaying()) {
				rbody->setActivationState(EAS_DISABLE_DEACTIVATION);
				rbody->forceActivationState(EAS_DISABLE_DEACTIVATION);
			}

			data->setEnablePhysics(true);
			data->setBodyType(cp3d::core::ISData::EIPT_RIGID_BODY);
			data->setPBodyPtr(rbody);
		}
	}
	else if (ptype == cp3d::core::ISData::EIPT_SOFT_BODY) {

	}
	else if (ptype == cp3d::core::ISData::EIPT_LIQUID_BODY) {
		ILiquidBody *lbody = devices->getBulletWorld()->addLiquidBody(vector3df(-5000,0,5000), 
																	  aabbox3df(0, -10000, 0, 10000, 0, 10000),
																	  2000.0f, 200.0f);

		lbody->setCurrentDirection(vector3df(0.f, 0.f, 0.f));
		lbody->setGlobalWaveChangeIncrement(0.1f);
		lbody->setGlobalWaveUpdateFrequency(1.0f);
		lbody->setMaxGlobalWaveHeight(4.0f);
		lbody->setMinGlobalWaveHeight(-1.f);
		lbody->setLocalWaveValues(10, 1, 0.5f);
		lbody->setInfinite(true);
		lbody->setInfiniteDepth(true);
		lbody->setLiquidDensity(0.1f);

		data->setBodyType(cp3d::core::ISData::EIPT_LIQUID_BODY);
		data->setPBodyPtr(lbody);
	}

	read("body");

	read("physics");
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
	vector3d<T> vec = vector3d<T>(xmlReader->getAttributeValueAsFloat("X"),
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

void CImporter::import_t() {
	importScene(pathOfFile);
}

void CImporter::newImportScene(stringc file_path) {
	CProcess *process;
	if (!devices->isOnlyForPlaying()) {
		process = new CProcess(devices->getGUIEnvironment(), "Importing Scene");
		devices->getProcessesLogger()->addProcess(process);
	}
	u32 currentElementNumber = 0;

	std::mutex mutex;
	mutex.lock();
	bool isXeffectDrawable = devices->isXEffectDrawable();
	devices->setXEffectDrawable(false);
	read("rootScene");
	readConfig();
	devices->setXEffectDrawable(isXeffectDrawable);
	mutex.unlock();

	while (xmlReader->read()) {
		element = "";
		if (xmlReader->getNodeType() == EXN_ELEMENT)
			element = xmlReader->getNodeName();

		if (!devices->isOnlyForPlaying())
			process->setName(stringc("Importing ") + stringc(element.c_str()));

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
		else if (element == "skybox")
			buildSkybox();


		currentElementNumber++;

		if (numberOfObjects > 0 && !devices->isOnlyForPlaying())
			process->getProgressBar()->setPercentage((currentElementNumber*100)/numberOfObjects);

	}

    read("rootScene");
    delete xmlReader;

	if (!devices->isOnlyForPlaying())
		process->setHasFinished(true);

	devices->getEventReceiver()->sendUserEvent(ECUE_NODE_ADDED, 0);
}

//---------------------------------------------------------------------------------------------
//-----------------------------------LOAD CONFIGS----------------------------------------------
//---------------------------------------------------------------------------------------------

void CImporter::importCamerasConfig() {
	stringc wd = devices->getWorkingDirectory().c_str();
    wd += "/";
    devices->getDevice()->getFileSystem()->changeWorkingDirectoryTo(wd.c_str());

    xmlReader = createIrrXMLReader("Config/cameras.scfg");
	if (!xmlReader)
		return;

	read("root");

	//FPS CAMERA
	read("fpsCamera");
	SKeyMap keyMap;
	read("moveForward");
	keyMap.Action = EKA_MOVE_FORWARD;
	keyMap.KeyCode = (EKEY_CODE)xmlReader->getAttributeValueAsInt("value");
	devices->setKeyMap(keyMap, 0);
	read("moveBackward");
	keyMap.Action = EKA_MOVE_BACKWARD;
	keyMap.KeyCode = (EKEY_CODE)xmlReader->getAttributeValueAsInt("value");
	devices->setKeyMap(keyMap, 1);
	read("strafeLeft");
	keyMap.Action = EKA_STRAFE_LEFT;
	keyMap.KeyCode = (EKEY_CODE)xmlReader->getAttributeValueAsInt("value");
	devices->setKeyMap(keyMap, 2);
	read("strageRight");
	keyMap.Action = EKA_STRAFE_RIGHT;
	keyMap.KeyCode = (EKEY_CODE)xmlReader->getAttributeValueAsInt("value");
	devices->setKeyMap(keyMap, 3);
	read("jump");
	keyMap.Action = EKA_JUMP_UP;
	keyMap.KeyCode = (EKEY_CODE)xmlReader->getAttributeValueAsInt("value");
	devices->setKeyMap(keyMap, 4);
	read("fpsCamera");

	read("root");

	devices->applyKeyMapOnFPSCamera();

	delete xmlReader;
}
