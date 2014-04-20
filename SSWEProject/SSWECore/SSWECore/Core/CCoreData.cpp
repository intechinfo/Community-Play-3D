//
//  CCoreData.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 25/09/12.
//
//

#include "stdafx.h"
#include "CCoreData.h"

#include <ISSWELibPlugin.h>

CCoreData::CCoreData() {
    clearAllTheArrays();
}

CCoreData::~CCoreData() {

}

//---------------------------------------------------------------------------------------------
//----------------------------------CORE DATA FUNCTIONS----------------------------------------
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

	for (u32 i=0; i < effectFilters.size(); i++) {
		//delete effectFilters[i];
		effectFilters[i].destroyLuaState();
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
	effectFilters.clear();

	//CALLBACKS
    shaderCallbacks.clear();

	//SCRIPTS
	scriptFiles.clear();
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

void CCoreData::removeSceneNode(ISceneNode *node, ISSWERender *_effect) {
	EffectHandler *effect = (EffectHandler *)_effect;
    //SEARCH TERRAINS
    s32 indice = getArrayOfTerrainNodes().binary_search(node);
    if (indice != -1) {
        effect->removeNodeFromDepthPass(terrainsData[indice].getNode());
        effect->removeShadowFromNode(terrainsData[indice].getNode());
        effect->removeNodeFromLightScatteringPass(terrainsData[indice].getNode());
        terrainsData[indice].getNode()->remove();
        terrainsData[indice].removeClonedNodes();
        terrainsData.erase(indice);
        effect->setAllShadowLightsRecalculate();
        return;
    }
    
    //SEARCH OBJECTS
    indice = getArrayOfObjectNodes().binary_search(node);
    if (indice != -1) {
        effect->removeNodeFromDepthPass(objectsData[indice].getNode());
        effect->removeShadowFromNode(objectsData[indice].getNode());
        effect->removeNodeFromLightScatteringPass(objectsData[indice].getNode());
        objectsData[indice].getNode()->remove();
        objectsData[indice].removeClonedNodes();
        objectsData.erase(indice);
        effect->setAllShadowLightsRecalculate();
        return;
    }
    
    //SEARCH TREES
    indice = getArrayOfTreeNodes().binary_search(node);
    if (indice != -1) {
        effect->removeNodeFromDepthPass(treesData[indice].getNode());
        effect->removeShadowFromNode(treesData[indice].getNode());
        effect->removeNodeFromLightScatteringPass(treesData[indice].getNode());
        treesData[indice].getNode()->remove();
        treesData[indice].removeClonedNodes();
        treesData.erase(indice);
        effect->setAllShadowLightsRecalculate();
        return;
    }
    
    //SEARCH LIGHTS
    indice = getArrayOfLightNodes().binary_search(node);
    if (indice != -1) {
        SLightsData ldata = lightsData[indice];
        if (ldata.getLensFlareSceneNode()) {
            effect->removeNodeFromDepthPass(ldata.getLensFlareSceneNode());
            effect->removeShadowFromNode(ldata.getLensFlareSceneNode());
            effect->removeNodeFromLightScatteringPass(ldata.getLensFlareSceneNode());
            ldata.getLensFlareSceneNode()->remove();
        }
        if (ldata.getLensFlareBillBoardSceneNode()) {
            effect->removeNodeFromDepthPass(ldata.getLensFlareBillBoardSceneNode());
            effect->removeShadowFromNode(ldata.getLensFlareBillBoardSceneNode());
            effect->removeNodeFromLightScatteringPass(ldata.getLensFlareBillBoardSceneNode());
            ldata.getLensFlareBillBoardSceneNode()->remove();
        }
        if (ldata.getLensFlareMeshSceneNode()) {
            effect->removeNodeFromDepthPass(ldata.getLensFlareMeshSceneNode());
            effect->removeShadowFromNode(ldata.getLensFlareMeshSceneNode());
            effect->removeNodeFromLightScatteringPass(ldata.getLensFlareMeshSceneNode());
            effect->getIrrlichtDevice()->getVideoDriver()->removeOcclusionQuery(ldata.getLensFlareMeshSceneNode());
            ldata.getLensFlareMeshSceneNode()->remove();
        }
        ldata.getNode()->remove();
        ldata.removeClonedNodes();
        lightsData.erase(indice);
        effect->setAllShadowLightsRecalculate();
        return;
    }
    
    //SEARCH VOLUME LIGHTS
    indice = getArrayOfVolumeLightNodes().binary_search(node);
    if (indice != -1) {
        effect->removeNodeFromDepthPass(volumeLightsData[indice].getNode());
        effect->removeShadowFromNode(volumeLightsData[indice].getNode());
        effect->removeNodeFromLightScatteringPass(volumeLightsData[indice].getNode());
        volumeLightsData[indice].getNode()->remove();
        volumeLightsData[indice].removeClonedNodes();
        volumeLightsData.erase(indice);
        effect->setAllShadowLightsRecalculate();
        return;
    }
    
    //SEARCH WATERS
    indice = getArrayOfWaterSurfaceNodes().binary_search(node);
    if (indice != -1) {
        effect->removeNodeFromDepthPass(waterSurfaces[indice].getNode());
        //effect->removeNodeFromDepthPass(waterSurfaces[indice].getWaterSurface()->getWaterNode());
        effect->removeNodeFromDepthPass(waterSurfaces[indice].getWaterSurface()->getWaterSceneNode());
        //effect->removeShadowFromNode(waterSurfaces[indice].getWaterSurface()->getWaterNode());
        effect->removeShadowFromNode(waterSurfaces[indice].getWaterSurface()->getWaterSceneNode());
        effect->removeNodeFromLightScatteringPass(waterSurfaces[indice].getWaterSurface()->getWaterSceneNode());
        waterSurfaces[indice].getWaterSurface()->remove();
        waterSurfaces[indice].removeClonedNodes();
        waterSurfaces.erase(indice);
        effect->setAllShadowLightsRecalculate();
        return;
    }
    
    //IF NOT FOUND, THEN SIMPLY REMOVE FROM THE SCENE GRAPH
    node->remove();
}

ISData *CCoreData::getISDataOfSceneNode(ISceneNode *node) {
	//SEARCH TERRAINS
	for (u32 i=0; i < terrainsData.size(); i++) {
		if (terrainsData[i].getNode() == node) {
			return &terrainsData[i];
			break;
		}
	}
    
    //SEARCH OBJECTS
	for (u32 i=0; i < objectsData.size(); i++) {
		if (objectsData[i].getNode() == node) {
			return &objectsData[i];
			break;
		}
	}
    
    //SEARCH TREES
	for (u32 i=0; i < treesData.size(); i++) {
		if (treesData[i].getNode() == node) {
			return &treesData[i];
			break;
		}
	}
    
    //SEARCH LIGHTS
	for (u32 i=0; i < lightsData.size(); i++) {
		if (lightsData[i].getNode() == node) {
			return &lightsData[i];
			break;
		}
	}
    
    //SEARCH VOLUME LIGHTS
	for (u32 i=0; i < volumeLightsData.size(); i++) {
		if (volumeLightsData[i].getNode() == node) {
			return &volumeLightsData[i];
			break;
		}
	}
    
    //SEARCH WATERS
	for (u32 i=0; i < waterSurfaces.size(); i++) {
		if (waterSurfaces[i].getNode() == node) {
			return &waterSurfaces[i];
			break;
		}
	}
    
    return 0;
}

ISData *CCoreData::copySDataOfSceneNode(irr::scene::ISceneNode *node) {
    //SEARCH TERRAINS
    s32 indice = getArrayOfTerrainNodes().binary_search(node);
    if (indice != -1) {
        terrainsData.push_back(STerrainsData(terrainsData[indice]));
        return &terrainsData[terrainsData.size()-1];
    }
    
    //SEARCH OBJECTS
    indice = getArrayOfObjectNodes().binary_search(node);
    if (indice != -1) {
        objectsData.push_back(SObjectsData(objectsData[indice]));
        return &objectsData[objectsData.size()-1];
    }
    
    //SEARCH TREES
    indice = getArrayOfTreeNodes().binary_search(node);
    if (indice != -1) {
        treesData.push_back(STreesData(treesData[indice]));
        return &treesData[treesData.size()-1];
    }
    
    //SEARCH LIGHTS
    indice = getArrayOfLightNodes().binary_search(node);
    if (indice != -1) {
        lightsData.push_back(SLightsData(lightsData[indice]));
        return &lightsData[lightsData.size()-1];
    }
    
    //SEARCH VOLUME LIGHTS
    indice = getArrayOfVolumeLightNodes().binary_search(node);
    if (indice != -1) {
        volumeLightsData.push_back(SVolumeLightsData(volumeLightsData[indice]));
        return &volumeLightsData[volumeLightsData.size()-1];
    }
    
    //SEARCH WATERS
    indice = getArrayOfWaterSurfaceNodes().binary_search(node);
    if (indice != -1) {
        waterSurfaces.push_back(SWaterSurfacesData(waterSurfaces[indice]));
        return &waterSurfaces[waterSurfaces.size()-1];
    }
    
    return 0;
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

u32 CCoreData::getTerrainNodeIndice(ISceneNode *node) {
	u32 ni = -1;

	for (u32 i=0; i < terrainsData.size(); i++) {
		if (terrainsData[i].getNode() == node) {
			ni = i;
			break;
		}
	}

	return ni;
}

//---------------------------------------------------------------------------------------------
//----------------------------------PLUGINS----------------------------------------------------
//---------------------------------------------------------------------------------------------

void CCoreData::destroyMonitor(IMonitor *monitor) {
    for (u32 i=0; i < monitors.size(); i++) {
        if (monitors[i].getMonitor() == monitor) {
            monitors[i].freeInstance();
            monitors.erase(i);
            break;
        }
    }
}

void CCoreData::destroySSWEPlugin(ISSWELibPlugin *plugin) {
    for (u32 i=0; i < sswePlugins.size(); i++) {
        if (sswePlugins[i].getPlugin() == plugin) {
            sswePlugins[i].freeInstance();
            sswePlugins.erase(i);
            break;
        }
    }
}

//---------------------------------------------------------------------------------------------
//----------------------------------COLLABORATIVE----------------------------------------------
//---------------------------------------------------------------------------------------------
//FLIP COLLABORATIVE TESTS

