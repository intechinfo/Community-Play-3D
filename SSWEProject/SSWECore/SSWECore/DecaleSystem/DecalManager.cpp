/*
  Copyright (C) 2011 Thijs Ferket (RdR)

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include "stdafx.h"

#include "DecalManager.h"
#include <map>
#include "CBatchingMesh.h"
//#include "EmptyDecalSceneNode.h"

DecalManager::DecalManager(irr::scene::ISceneManager* smgr) {
	this->smgr = smgr;
	driver = smgr->getVideoDriver();
	terrain = 0;
	createMetaTriangleSelector();
}

DecalManager::~DecalManager() {
	clear();
	metaSelector->drop();
}

void DecalManager::createMetaTriangleSelector() {
	metaSelector = smgr->createMetaTriangleSelector();
}

irr::scene::IMetaTriangleSelector* DecalManager::getMetaTriangleSelector() {
	return metaSelector;
}

irr::core::array<DecalSceneNode*> DecalManager::getDecals(const irr::io::path texturePath) {
	irr::core::array<irr::scene::ISceneNode*> nodes;
	irr::core::array<DecalSceneNode*> decals;

	smgr->getSceneNodesFromType((irr::scene::ESCENE_NODE_TYPE)DECAL_SCENE_NODE_ID, nodes, smgr->getRootSceneNode());
	for (irr::u32 i = 0; i < nodes.size(); i++) {
		DecalSceneNode* decal = (DecalSceneNode*) nodes[i];
		if (decal->getTexture()->getName().getPath().equals_ignore_case(texturePath)) {
			decals.push_back(decal);
		}
	}

	nodes.clear();

	return decals;
}

irr::core::array<DecalSceneNode*> DecalManager::getPermanentDecals() {
	irr::core::array<irr::scene::ISceneNode*> nodes;
	irr::core::array<DecalSceneNode*> decals;

	smgr->getSceneNodesFromType((irr::scene::ESCENE_NODE_TYPE)DECAL_SCENE_NODE_ID, nodes, smgr->getRootSceneNode());
	for (irr::u32 i = 0; i < nodes.size(); i++) {
		DecalSceneNode* decal = (DecalSceneNode*) nodes[i];
		if (decal->getLifetime() == 0) {
			decals.push_back(decal);
		}
	}

	nodes.clear();

	return decals;
}

irr::core::array<DecalSceneNode*> DecalManager::combineDecals(irr::core::array<DecalSceneNode*> decals) {
	irr::core::array<DecalSceneNode*> newDecals;

	if (decals.size() > 1) {
		std::map<irr::io::path, CBatchingMesh*> batchingMeshes = std::map<irr::io::path, CBatchingMesh*>();

		for (irr::u32 i = 0; i < decals.size(); i++) {
			irr::video::ITexture* texture = decals[i]->getTexture();
			CBatchingMesh* batchingMesh = 0;

			// Check if already a batching mesh for this texture
			std::map<irr::io::path, CBatchingMesh*>::iterator iter = batchingMeshes.find(texture->getName().getPath());
			if (iter != batchingMeshes.end()) {
				batchingMesh = iter->second;
			}
			// Create batching mesh
			else {
				batchingMesh = new CBatchingMesh();
				batchingMeshes.insert(std::pair<irr::io::path, CBatchingMesh*>(texture->getName().getPath(), batchingMesh));
			}

			batchingMesh->addMesh(decals[i]->getMesh(), decals[i]->getAbsolutePosition());
			smgr->addToDeletionQueue(decals[i]);
		}

		// Finalize batching meshes and create new decal scene nodes
		for (std::map<irr::io::path, CBatchingMesh*>::iterator iter = batchingMeshes.begin(); iter != batchingMeshes.end(); iter++) {
			(iter->second)->finalize();

			// Create decal scene node
			irr::video::ITexture* texture = smgr->getVideoDriver()->getTexture(iter->first);
			DecalSceneNode* decal = new DecalSceneNode(smgr->getRootSceneNode(), smgr, iter->second, texture);
			decal->drop();
			newDecals.push_back(decal);
		}

		decals.clear();
		batchingMeshes.clear();
	} else if (decals.size() == 1) {
		newDecals.push_back(decals[0]);
	}
	return newDecals;
}

void DecalManager::setTerrain(irr::scene::ITerrainSceneNode* terrain) {
	this->terrain = terrain;
	metaSelector->addTriangleSelector(terrain->getTriangleSelector());
}

void DecalManager::addMesh(irr::scene::IAnimatedMeshSceneNode* node) {
	addMesh(node->getMesh(), node);
}

void DecalManager::addMesh(irr::scene::IMeshSceneNode* node) {
	addMesh(node->getMesh(), node);
}

void DecalManager::addMesh(irr::scene::IMesh* mesh, irr::scene::ISceneNode* node) {
	metaSelector->addTriangleSelector(smgr->createTriangleSelector(mesh, node));
}

void DecalManager::removeMesh(irr::scene::ISceneNode* node) {
	metaSelector->removeTriangleSelector(node->getTriangleSelector());
}

void DecalManager::clear() {
	metaSelector->removeAllTriangleSelectors();
}

irr::core::vector3df DecalManager::getNormal(irr::core::vector3df position, const bool useTerrainOnly) {
	irr::core::vector3df startPosition = position;
	if (terrain) {
		startPosition.Y = terrain->getBoundingBox().MaxEdge.Y;
	}

	irr::core::vector3df endPosition = position;
	if (terrain) {
		endPosition.Y = terrain->getBoundingBox().MinEdge.Y;
	}

	return getNormal(irr::core::line3df(startPosition, endPosition), useTerrainOnly);
}

irr::core::vector3df DecalManager::getNormal(irr::core::vector3df startPosition, irr::core::vector3df endPosition, const bool useTerrainOnly) {
	irr::core::vector3df direction = (endPosition - startPosition).normalize();
	return getNormal(irr::core::line3df(startPosition, endPosition + direction), useTerrainOnly);
}

irr::core::vector3df DecalManager::getNormal(irr::core::line3df line, const bool useTerrainOnly) {
	irr::core::vector3df normal = irr::core::vector3df(0, 1, 0);

	irr::core::triangle3df triangle;
	irr::scene::ISceneNode* node = 0;
	irr::core::vector3df collisionPoint;

	if (terrain && useTerrainOnly) {
		smgr->getSceneCollisionManager()->getCollisionPoint(line, terrain->getTriangleSelector(), collisionPoint, triangle, node);
	} else {
		smgr->getSceneCollisionManager()->getCollisionPoint(line, (irr::scene::ITriangleSelector*) metaSelector, collisionPoint, triangle, node);
	}

	normal = triangle.getNormal();
	normal.normalize();

	return normal;
}

irr::core::vector3df DecalManager::getCollisionPoint(irr::core::vector3df position, const bool useTerrainOnly) {
	irr::core::vector3df startPosition = position;
	if (terrain) {
		startPosition.Y = terrain->getBoundingBox().MaxEdge.Y;
	}

	irr::core::vector3df endPosition = position;
	if (terrain) {
		endPosition.Y = terrain->getBoundingBox().MinEdge.Y;
	}

	irr::core::line3df line = irr::core::line3df(startPosition, endPosition);
	irr::core::triangle3df triangle;
	irr::scene::ISceneNode* node = 0;
	irr::core::vector3df collisionPoint;

	if (terrain && useTerrainOnly) {
		smgr->getSceneCollisionManager()->getCollisionPoint(line, terrain->getTriangleSelector(), collisionPoint, triangle, node);
	} else {
		smgr->getSceneCollisionManager()->getCollisionPoint(line, (irr::scene::ITriangleSelector*) metaSelector, collisionPoint, triangle, node);
	}
	return collisionPoint;
}

DecalSceneNode* DecalManager::addDecal(irr::io::path filename, irr::core::vector3df position, irr::core::vector3df dimension, irr::core::vector3df normal, const irr::f32 textureRotation,
		irr::scene::ISceneNode* parent, const irr::f32 lifeTime, const irr::f32 distance, const bool useTerrainOnly) {
	return addDecal(driver->getTexture(filename), position, dimension, normal, textureRotation, parent, lifeTime, distance, useTerrainOnly);
}

DecalSceneNode* DecalManager::addDecal(irr::video::ITexture* texture, irr::core::vector3df position, irr::core::vector3df dimension, irr::core::vector3df normal, const irr::f32 textureRotation,
		irr::scene::ISceneNode* parent, const irr::f32 lifeTime, const irr::f32 distance, const bool useTerrainOnly) {
	// Check if meta selector has any selectors added
	if (metaSelector->getTriangleCount() == 0) {
		return 0;
	}

	dimension *= 0.5f;

	// Create boxes
	irr::core::aabbox3df box = irr::core::aabbox3df(-dimension, dimension);

	box.MinEdge += position;
	box.MaxEdge += position;

	// Calculate rotation
	normal.normalize();
	irr::core::quaternion quatDirection;
	quatDirection.rotationFromTo(irr::core::vector3df(0, 1, 0), normal);
	irr::core::vector3df rotation = quatDirection.getMatrix().getRotationDegrees();
	rotation.Y += textureRotation;

	// Create rotation matrix
	irr::core::matrix4 rotationMatrix;
	rotationMatrix.setRotationDegrees(rotation);
	rotationMatrix.setRotationCenter(irr::core::vector3df(0.5f, 0.5f, 0.5f), irr::core::vector3df(0, 0, 0));

	// Create mesh
	irr::scene::IMesh* mesh = createMesh(box, rotationMatrix, useTerrainOnly);

	// Create decal scene node
	if (parent == 0) {
		parent = smgr->getRootSceneNode();
	}

	position -= parent->getAbsolutePosition();
	position /= parent->getScale();

	DecalSceneNode* decal = new DecalSceneNode(parent, smgr, mesh, texture, position);
	decal->setLifetime(lifeTime);
	decal->setDistance(distance);

	decal->drop();

	return decal;
}

irr::scene::IMesh* DecalManager::createMesh(irr::core::aabbox3df box, irr::core::matrix4 rotationMatrix, const bool useTerrainOnly) {
	// Get triangles
	irr::core::array<irr::core::triangle3df> triangles;
	irr::s32 triangleCount = 0;
	if (terrain && useTerrainOnly) {
		triangles.set_used(terrain->getTriangleSelector()->getTriangleCount());
		terrain->getTriangleSelector()->getTriangles(&triangles[0], triangles.size(), triangleCount, box);
	} else {
		triangles.set_used(metaSelector->getTriangleCount());
		metaSelector->getTriangles(&triangles[0], triangles.size(), triangleCount, box);
	}

	// Create vertices and indices arrays
	irr::core::array<irr::video::S3DVertex> vertices = irr::core::array<irr::video::S3DVertex>();
	irr::core::array<irr::u16> indices = irr::core::array<irr::u16>();

	// Create translation matrix (Scale/Translate to 0,0 - 1,1 box)
	irr::core::vector3df scale = irr::core::vector3df(1, 1, 1) / box.getExtent();

	irr::core::matrix4 m;
	m.setTranslation(-(box.MinEdge * scale));

	// Create scale matrix
	irr::core::matrix4 scaleMatrix;
	scaleMatrix.setScale(scale);
	m *= scaleMatrix;

	// Clip all triangles and fill vertex and indices
	irr::u32 vertexIndex = 0;
	//const irr::f32 offsetScale = (scale.X + scale.Z) * 0.5f;
	std::map<irr::core::vector3df, irr::u32> positions;
	for (irr::u32 i = 0; i < (irr::u32) triangleCount; i++) {
		irr::u32 index = 0;

		irr::core::vector3df triangleNormal = triangles[i].getNormal().normalize();

		// Scale & Translate positions
		m.transformVect(triangles[i].pointA);
		m.transformVect(triangles[i].pointB);
		m.transformVect(triangles[i].pointC);

		// Rotate positions
		rotationMatrix.transformVect(triangles[i].pointA);
		rotationMatrix.transformVect(triangles[i].pointB);
		rotationMatrix.transformVect(triangles[i].pointC);

		// Clip triangle
		irr::core::array<irr::core::triangle3df> triangles2 = clipTriangle(triangles[i]);

		// Fill vertices and indices
		for (irr::u32 t = 0; t < triangles2.size(); t++) {

			for (irr::u32 p = 0; p < 3; p++) {
				irr::core::vector3df pos = triangles2[t].pointA;
				if (p == 1) {
					pos = triangles2[t].pointB;
				} else if (p == 2) {
					pos = triangles2[t].pointC;
				}

				// Search if vertex already exists in the vertices list
				std::map<irr::core::vector3df, irr::u32>::iterator iter = positions.find(pos);
				if (iter != positions.end()) {
					index = iter->second;
				}
				// Add vertex to list
				else {
					index = vertexIndex;
					positions.insert(std::pair<irr::core::vector3df, irr::u32>(pos, vertexIndex));

					// Set translation +0.01f to avoid flickering in draw
					//pos += triangleNormal * (0.02f * offsetScale);
					pos += triangleNormal * 0.01f;
					vertices.push_back(irr::video::S3DVertex(pos, triangleNormal, irr::video::SColor(255, 255, 255, 255), irr::core::vector2df(pos.X, 1 - pos.Z))); // 1 - pos.Z to flip the texture
					vertexIndex++;
				}
				indices.push_back(index);
			}
		}
	}

	// Create meshBuffer
	irr::scene::SMeshBuffer* meshBuffer = new irr::scene::SMeshBuffer();
	meshBuffer->append(vertices.pointer(), vertices.size(), indices.pointer(), indices.size());

	// Clear arrays
	positions.clear();
	vertices.clear();
	indices.clear();

	// Inverse rotation matrix to restore original vertex positions
	rotationMatrix.makeInverse();
	smgr->getMeshManipulator()->transform(meshBuffer, rotationMatrix);

	// Inverse scaling matrix to restore original vertex positions
	m.makeInverse();
	m.setTranslation(-(box.getExtent() * 0.5f)); // Translate center to 0,0

	// Create batching mesh
	CBatchingMesh* batchingMesh = new CBatchingMesh();
	batchingMesh->addMeshBuffer(meshBuffer, m);

	meshBuffer->drop();
	batchingMesh->finalize();

	return batchingMesh;
}

irr::core::array<irr::core::triangle3df> DecalManager::clipTriangle(irr::core::triangle3df triangle) {
	irr::core::aabbox3df box = irr::core::aabbox3df(0, 0, 0, 1, 1, 1);

	// Create list of triangles
	irr::core::array<irr::core::triangle3df> triangles;

	// Total inside box (no need for further checks)
	if (triangle.isTotalInsideBox(box)) {
		triangles.push_back(triangle);
		return triangles;
	}
	// Triangle total outside box
	else if (triangle.isTotalOutsideBox(box)) {
		return irr::core::array<irr::core::triangle3df>();
	}
	// Partial inside box
	else {
		triangles.push_back(triangle);
		return triangles;
	}
}
