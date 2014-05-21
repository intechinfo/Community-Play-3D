/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include "CMeshFactory.h"

CMeshFactory::CMeshFactory(CDevices *_devices) {
	devices = _devices;
}
CMeshFactory::~CMeshFactory() {

}

array<ISceneNode *> CMeshFactory::getNodesThatUse(IMesh *mesh) {
	array<ISceneNode *> nodesThatuse(0);

	/// Get all scene nodes of current instance
	array<ISceneNode *> nodes;
	devices->getSceneManager()->getSceneNodesFromType(ESNT_ANY, nodes);

	/// Fill nodes that uses the selected mesh
	for (u32 i=0; i < nodes.size(); i++) {
		if (nodes[i]->getType() == ESNT_OCTREE || nodes[i]->getType() == ESNT_MESH) {
			IMeshSceneNode *node = (IMeshSceneNode*)nodes[i];
			if (node->getMesh() == mesh) {
				nodesThatuse.push_back(node);
			}
		} else if (nodes[i]->getType() == ESNT_ANIMATED_MESH) {
			IAnimatedMeshSceneNode *node = (IAnimatedMeshSceneNode*)nodes[i];
			if (node->getMesh() == mesh) {
				nodesThatuse.push_back(node);
			}
		}
	}

	return nodesThatuse;
}

void CMeshFactory::reloadMesh(IMesh *mesh, stringc meshName) {
	array<ISceneNode *> nodesThatuse(0);

	/// Get all scene nodes of current instance
	array<ISceneNode *> nodes;
	devices->getSceneManager()->getSceneNodesFromType(ESNT_ANY, nodes);

	/// Fill nodes that uses the selected mesh
	for (u32 i=0; i < nodes.size(); i++) {
		if (nodes[i]->getType() == ESNT_OCTREE || nodes[i]->getType() == ESNT_MESH) {
			IMeshSceneNode *node = (IMeshSceneNode*)nodes[i];
			if (node->getMesh() == mesh) {
				nodesThatuse.push_back(node);
			}
		} else if (nodes[i]->getType() == ESNT_ANIMATED_MESH) {
			IAnimatedMeshSceneNode *node = (IAnimatedMeshSceneNode*)nodes[i];
			if (node->getMesh() == mesh) {
				nodesThatuse.push_back(node);
			}
		}
	}

	/// Reload mesh
	devices->getSceneManager()->getMeshCache()->removeMesh(mesh);
	mesh = 0;

	mesh = devices->getSceneManager()->getMesh(devices->getWorkingDirectory() + meshName);
	if (!mesh)
		mesh = devices->getSceneManager()->getMesh(meshName);

	/// Restart nodes
	for (u32 i=0; i < nodesThatuse.size(); i++) {
		SData *data = (SData*)devices->getCoreData()->getISDataOfSceneNode(nodesThatuse[i]);
		assert(data != 0);

		if (nodesThatuse[i]->getType() == ESNT_OCTREE || nodesThatuse[i]->getType() == ESNT_MESH) {
			modifyMeshOfSceneNode(data->getNode(), mesh, false);
		} else if (nodesThatuse[i]->getType() == ESNT_ANIMATED_MESH) {
			modifyMeshOfSceneNode(data->getNode(), mesh, true);
		}

		data->setMesh(mesh);

		/// Restart configuration
		if (data->wasPlanarTextureMapped()) {
			devices->getSceneManager()->getMeshManipulator()->makePlanarTextureMapping(mesh, data->wasPlanarTextureMappedValue());
		}

		if (data->wasAngleWeighted() || data->wasNormalRecalculated() || data->wasSmoothed() || data->wasTangentRecalculated()) {
			IMesh *tangentsMesh = devices->getSceneManager()->getMeshManipulator()->createMeshWithTangents(mesh, data->wasNormalRecalculated(), 
				data->wasSmoothed(), data->wasAngleWeighted(), data->wasTangentRecalculated());
			if (nodesThatuse[i]->getType() == ESNT_OCTREE || nodesThatuse[i]->getType() == ESNT_MESH) {
				modifyMeshOfSceneNode(data->getNode(), tangentsMesh, false);
			} else if (nodesThatuse[i]->getType() == ESNT_ANIMATED_MESH) {
				modifyMeshOfSceneNode(data->getNode(), tangentsMesh, true);
			}
		}

	}
}

void CMeshFactory::modifyMeshOfSceneNode(ISceneNode *node, IMesh *mesh, bool isAnimated) {

	array<SMaterial> materials(node->getMaterialCount());
	for (u32 i=0; i < node->getMaterialCount(); i++) {
		materials.push_back(node->getMaterial(i));
	}

	if (isAnimated) {
		((IAnimatedMeshSceneNode*)node)->setMesh((IAnimatedMesh*)mesh);
	} else {
		((IMeshSceneNode*)node)->setMesh((IAnimatedMesh*)mesh);
	}

	for (u32 i=0; i < materials.size() && i < mesh->getMeshBufferCount(); i++) {
		node->getMaterial(i) = materials[i];
	}

}
