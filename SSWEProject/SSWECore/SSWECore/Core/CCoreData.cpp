//
//  CCoreData.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 25/09/12.
//
//

#include "stdafx.h"
#include "CCoreData.h"

#include "../../../SSWERenders/Renders/XEffect/EffectShaders.h"

CCoreData::CCoreData() {
    clearAllTheArrays();
}

CCoreData::~CCoreData() {

}

//---------------------------------------------------------------------------------------------
//----------------------------------CORE DATA FUNCTIONS ---------------------------------------
//---------------------------------------------------------------------------------------------

void CCoreData::clear() {
	for (u32 i=0; i < terrainsData.size(); i++) {
		terrainsData[i].getNode()->remove();
	}

    for (u32 i=0; i < treesData.size(); i++) {
        treesData[i].getNode()->remove();
    }
    for (u32 i=0; i < objectsData.size(); i++) {
		objectsData[i].getNode()->remove();
    }
    for (u32 i=0; i < lightsData.size(); i++) {
		lightsData[i].getNode()->remove();
    }
	for (u32 i=0; i < volumeLightsData.size(); i++) {
		volumeLightsData[i].getNode()->remove();
	}
	for (u32 i=0; i < waterSurfaces.size(); i++) {
		waterSurfaces[i].getWaterSurface()->remove();
	}

	for (u32 i=0; i < effectRenderCallbacks.size(); i++) {
		delete effectRenderCallbacks[i];
	}
	for (u32 i=0; i < materialRenderCallbacks.size(); i++) {
		delete materialRenderCallbacks[i];
	}
	for (u32 i=0; i < shaderCallbacks.size(); i++) {
		delete shaderCallbacks[i];
	}
}

void CCoreData::clearAllTheArrays() {
	//NODES
	terrainsData.clear();
    treesData.clear();
	objectsData.clear();
    lightsData.clear();
	volumeLightsData.clear();
	waterSurfaces.clear();

	planarTextureMappingValues.clear();

	//EFFECT SHADERS
    effectRenders.clear();
    effectRendersPaths.clear();

	//CALLBACKS
    effectRenderCallbacks.clear();
	materialRenderCallbacks.clear();

    shaderCallbacks.clear();

	//RENDERS
	materialRenders.clear();
	materialRendersPaths.clear();
}

array<stringw> CCoreData::getSceneNodeAnimatorsNames(IrrlichtDevice *_device, u32 idx) {
	ISceneManager *smgr = _device->getSceneManager();
	ISceneNodeAnimatorFactory *anf = smgr->getSceneNodeAnimatorFactory(idx);
	array<stringw> names;

	if (anf) {
		anf->createSceneNodeAnimator("test !", 0);

		for (u32 i=0; i < getAmountOfSceneNodeAnimators(); i++) {
			names.push_back(stringw(anf->getCreateableSceneNodeAnimatorTypeName((ESCENE_NODE_ANIMATOR_TYPE)i)));
		}
	}

	return names;
}

u32 CCoreData::getAmountOfSceneNodeAnimators() {
	return (u32)ESNAT_COUNT;
}

array<ISceneNode *> CCoreData::getAllSceneNodes() {
    array<ISceneNode *> nodes;
    nodes.clear();
    
	for (u32 i=0; i < terrainsData.size(); i++) {
		nodes.push_back(terrainsData[i].getNode());
	}
    for (int i=0; i < treesData.size(); i++) {
		nodes.push_back(treesData[i].getNode());
    }
    for (int i=0; i < objectsData.size(); i++) {
		nodes.push_back(objectsData[i].getNode());
    }
    for (int i=0; i < lightsData.size(); i++) {
		nodes.push_back(lightsData[i].getNode());
    }
	for (int i=0; i < volumeLightsData.size(); i++) {
		nodes.push_back(volumeLightsData[i].getNode());
	}
	for (int i=0; i < waterSurfaces.size(); i++) {
		nodes.push_back(waterSurfaces[i].getNode());
	}
    
    return nodes;
}

array<IMesh *> CCoreData::getAllMeshes() {
    array<IMesh *> meshes;
    meshes.clear();
    
    for (int i=0; i < terrainsData.size(); i++) {
		meshes.push_back(terrainsData[i].getMesh());
    }
    for (int i=0; i < treesData.size(); i++) {
		meshes.push_back(treesData[i].getMesh());
    }
    for (int i=0; i < objectsData.size(); i++) {
		meshes.push_back(objectsData[i].getMesh());
    }
	for (u32 i=0; i < waterSurfaces.size(); i++) {
		meshes.push_back(waterSurfaces[i].getMesh());
	}

    return meshes;
}

s32 CCoreData::isMeshPlanared(ISceneNode *node) {
	s32 planared = -1;

	for (u32 i=0; i < planarTextureMappingValues.size(); i++) {
		if (planarTextureMappingValues[i].getNode() == node) {
			planared = i;
			break;
		}
	}

	return planared;
}

array<ISceneNode *> CCoreData::getArrayOfTerrainNodes() {
	array<ISceneNode *> nodes;

	for (u32 i=0; i < terrainsData.size(); i++) {
		nodes.push_back(terrainsData[i].getNode());
	}

	return nodes;
}

array<ISceneNode *> CCoreData::getArrayOfTreeNodes() {
	array<ISceneNode *> nodes;

	for (u32 i=0 ; i < treesData.size(); i++) {
		nodes.push_back(treesData[i].getNode());
	}

	return nodes;
}

array<ISceneNode *> CCoreData::getArrayOfObjectNodes() {
	array<ISceneNode *> nodes;

	for (u32 i=0; i < objectsData.size(); i++) {
		nodes.push_back(objectsData[i].getNode());
	}

	return nodes;
}

array<ISceneNode *> CCoreData::getArrayOfLightNodes() {
	array<ISceneNode *> nodes;

	for (u32 i=0; i < lightsData.size(); i++) {
		nodes.push_back(lightsData[i].getNode());
	}

	return nodes;
}

array<ISceneNode *> CCoreData::getArrayOfVolumeLightNodes() {
	array<ISceneNode *> nodes;
	
	for (u32 i=0; i < volumeLightsData.size(); i++) {
		nodes.push_back(volumeLightsData[i].getNode());
	}

	return nodes;
}

array<ISceneNode *> CCoreData::getArrayOfWaterSurfaceNodes() {
	array<ISceneNode *> nodes;

	for (u32 i=0; i < waterSurfaces.size(); i++) {
		nodes.push_back(waterSurfaces[i].getNode());
	}

	return nodes;
}

//---------------------------------------------------------------------------------------------
//----------------------------------IRRLICHT NODES---------------------------------------------
//---------------------------------------------------------------------------------------------
u32 CCoreData::getObjectNodeIndice(ISceneNode *node) {
	u32 ni=-1;

	for (u32 i=0; i < objectsData.size(); i++) {
		if (objectsData[i].getNode() == node) {
			ni = i;
			break;
		}
	}

	return ni;
}

//---------------------------------------------------------------------------------------------
//----------------------------------SHADERS-- -------------------------------------------------
//---------------------------------------------------------------------------------------------

void SLightsData::createLightShafts(EffectHandler *effect, const u32 numberOfPlanes) {
	ICameraSceneNode *initialCam = effect->getActiveSceneManager()->getActiveCamera();
	ICameraSceneNode *lightCamera = effect->getActiveSceneManager()->addCameraSceneNode();
	lightCamera->setPosition(node->getPosition());
	lightCamera->setTarget(node->getRotation());

	lightCamera->OnAnimate(effect->getIrrlichtDevice()->getTimer()->getTime());
	lightCamera->OnRegisterSceneNode();
	lightCamera->render();

	lsdata.create(lightCamera, numberOfPlanes, LIGHT_SHAFTS_V[ESE_HLSL], LIGHT_SHAFTS_P[ESE_HLSL]);

	//lightCamera->remove();
	effect->getActiveSceneManager()->setActiveCamera(initialCam);
}

//---------------------------------------------------------------------------------------------
//----------------------------------CALLBACKS -------------------------------------------------
//---------------------------------------------------------------------------------------------
