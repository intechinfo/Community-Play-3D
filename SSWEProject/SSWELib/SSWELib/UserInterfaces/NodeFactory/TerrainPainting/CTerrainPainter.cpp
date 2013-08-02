/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include "CTerrainPainter.h"

CTerrainPainter::CTerrainPainter(CDevices *_devices, ITerrainSceneNode *_node, dimension2di terrainSize) {
	devices = _devices;

	node = _node;

	minHeight = 0;
	maxHeight = 0;

	IMesh* pMesh = node->getMesh(); 
	for (u32 b=0; b<pMesh->getMeshBufferCount(); ++b)  {

		array<S3DVertex2TCoords *> newVertices;
		array<bool> newAgainUpVertices;
		terrainVerticesLines.push_back(newVertices);
		againUpVertices.push_back(newAgainUpVertices);

		IMeshBuffer* pMeshBuffer = pMesh->getMeshBuffer(b); 
		if (pMeshBuffer->getVertexType() != video::EVT_2TCOORDS) continue; 
		S3DVertex2TCoords* pVertices = (S3DVertex2TCoords*)pMeshBuffer->getVertices();

		for (u32 i=0; i < pMesh->getMeshBuffer(b)->getVertexCount(); i++) {
			if (pVertices[i].Pos.Y < minHeight)
				minHeight = pVertices[i].Pos.Y;

			if (pVertices[i].Pos.Y > maxHeight)
				maxHeight = pVertices[i].Pos.Y;

			terrainVerticesLines[b].push_back(&pVertices[i]);
			againUpVertices[b].push_back(true);
		}
	}
	heightInterval = (minHeight >= 0) ? maxHeight - minHeight : maxHeight + minHeight;

	//INITIALIZING TERRAIN PAINTING DATAS
	currentStep = (maxHeight > 0) ? 1 : 0;
	currentRadius = 2;
}

CTerrainPainter::~CTerrainPainter() {

}

void CTerrainPainter::applyTransform(s32 smoothFactor) {
	IMesh* pMesh = node->getMesh(); 
	for (u32 b=0; b<pMesh->getMeshBufferCount(); ++b)  {
		IMeshBuffer* pMeshBuffer = pMesh->getMeshBuffer(b); 
		if (pMeshBuffer->getVertexType() != video::EVT_2TCOORDS) continue; 
		smoothTerrain(pMeshBuffer, b, smoothFactor);
	}

	for (u32 i=0; i < againUpVertices.size(); i++)
		for (u32 j=0; j < againUpVertices[i].size(); j++)
			againUpVertices[i][j] = true;
}

void CTerrainPainter::smoothTerrain(IMeshBuffer* mb, u32 indice, s32 smoothFactor) {
	S3DVertex2TCoords *mbv = (S3DVertex2TCoords *)mb->getVertices();
	for (s32 run = 0; run < smoothFactor; ++run) {
		s32 yd = terrainSizeWidth;
		for (s32 y = 1; y < terrainSizeWidth - 1; ++y) {
			for (s32 x = 1; x < terrainSizeWidth - 1; ++x) {
				mbv[x + yd].Pos.Y =
									(mbv[x-1 + yd].Pos.Y + //LEFT
									mbv[x+1 + yd].Pos.Y + //RIGHT
									mbv[x + yd - terrainSizeWidth].Pos.Y + //ABOVE
									mbv[x + yd + terrainSizeWidth].Pos.Y) * 0.25f; //BELOW
			}
			yd += terrainSizeWidth;
		}
	}
}

void CTerrainPainter::RaiseTerrainVertex(vector3df clickPos, f32 step, bool up) {
	f32 nodescale = node->getBoundingBox().getExtent().X;

	for (u32 i=0; i < node->getMesh()->getMeshBufferCount(); i++) {

		IMeshBuffer* meshBuffer = node->getMesh()->getMeshBuffer(i);
		if (meshBuffer->getVertexType() != video::EVT_2TCOORDS) continue; 
		S3DVertex2TCoords* mb_vertices = (S3DVertex2TCoords*) meshBuffer->getVertices();

		for (u32 j = 0; j < meshBuffer->getVertexCount(); j += 1) {
			if (!againUpVertices[i][j]) continue;

			//vector3df realPos = mb_vertices[j].Pos;//*(node->getScale()/nodescale) + (node->getPosition()-node->getTerrainCenter());
			vector3df realPos = mb_vertices[j].Pos + node->getPosition();
			clickPos.Y = realPos.Y;

			if(realPos.getDistanceFrom(clickPos) < currentRadius) {
				//f32 ratio = sin(radius - realPos.getDistanceFrom(clickPos));
				f32 ratio = currentRadius - realPos.getDistanceFrom(clickPos);
				mb_vertices[j].Pos.Y += (up) ? (currentStep * (ratio)) : -(currentStep *(ratio));///(node->getScale().X/nodescale));
				againUpVertices[i][j] = false;
			}

			//if(mb_vertices[j].Pos.Y > nodescale/4) mb_vertices[j].Pos.Y = nodescale/4;
			//if(mb_vertices[j].Pos.Y < -(nodescale*0.1f)) mb_vertices[j].Pos.Y = -(nodescale*0.1f);
		}

		//devices->getSceneManager()->getMeshManipulator()->recalculateNormals(node->getMesh(),true);
		//terrainSelector = devices->getSceneManager()->createTerrainTriangleSelector(node, 0);
	}

	node->setPosition(node->getPosition());

}

void CTerrainPainter::drawBrush(vector3df pos) {
	f32 radius = currentRadius;
	const int heightMore = 5;

	vector3df position = pos;//this->getMousePosition3D(100).pickedPos;

	if (position==vector3df(0,0,0))
		return;

	SMaterial m;
	m.Lighting=false;
	devices->getVideoDriver()->setMaterial(m);
	devices->getVideoDriver()->setTransform(video::ETS_WORLD, core::matrix4());

	// Render the size of the brush.
	f32 framesize = 5;
	int step=10;
	for (int i=0; i<(360); i=i+step) {
		float degInRad = i*DEGTORAD;
		vector3df pos=position;
		pos.X+=cos(degInRad)*radius;
		pos.Z+=sin(degInRad)*radius;
		pos.Y=getHeightAt(pos)+heightMore;

		float degInRad2 = (i+step)*DEGTORAD;
		vector3df pos2=position;
		pos2.X+=cos(degInRad2)*radius;
		pos2.Z+=sin(degInRad2)*radius;
		pos2.Y=getHeightAt(pos2)+heightMore;
		//devices->getVideoDriver()->draw3DLine(pos,pos2,video::SColor(255,255,255,0));

		vector3df pos3=position;
		pos3.X+=cos(degInRad)*(radius+framesize);
		pos3.Z+=sin(degInRad)*(radius+framesize);
		pos3.Y=pos.Y;

		vector3df pos4=position;
		pos4.X+=cos(degInRad2)*(radius+framesize);
		pos4.Z+=sin(degInRad2)*(radius+framesize);
		pos4.Y=pos2.Y;

		devices->getVideoDriver()->draw3DTriangle(triangle3df(pos4,pos3,pos),video::SColor(255,255,255,255));
		devices->getVideoDriver()->draw3DTriangle(triangle3df(pos,pos2,pos4),video::SColor(255,255,255,255));
	}

	// Center circle for the brush give the center
	radius=5;
	framesize = 2;
	step=15;
	for (int i=0; i<(360); i=i+step) {
		float degInRad = i*DEGTORAD;
		vector3df pos=position;
		pos.X+=cos(degInRad)*radius;
		pos.Z+=sin(degInRad)*radius;
		pos.Y=getHeightAt(pos)+heightMore;

		float degInRad2 = (i+step)*DEGTORAD;
		vector3df pos2=position;
		pos2.X+=cos(degInRad2)*radius;
		pos2.Z+=sin(degInRad2)*radius;
		pos2.Y=getHeightAt(pos2)+heightMore;
		//devices->getVideoDriver()->draw3DLine(pos,pos2,video::SColor(255,255,255,0));

		vector3df pos3=position;
		pos3.X+=cos(degInRad)*(radius+framesize);
		pos3.Z+=sin(degInRad)*(radius+framesize);
		pos3.Y=pos.Y;

		vector3df pos4=position;
		pos4.X+=cos(degInRad2)*(radius+framesize);
		pos4.Z+=sin(degInRad2)*(radius+framesize);
		pos4.Y=pos2.Y;

		devices->getVideoDriver()->draw3DTriangle(triangle3df(pos4,pos3,pos),video::SColor(255,255,255,255));
		devices->getVideoDriver()->draw3DTriangle(triangle3df(pos,pos2,pos4),video::SColor(255,255,255,255));
	}
}

f32 CTerrainPainter::getHeightAt(vector3df pos) {
	scene::ISceneCollisionManager* collMan = devices->getSceneManager()->getSceneCollisionManager();
	core::line3d<f32> ray;
    ray.start = pos+vector3df(0,+250,0);
    ray.end = ray.start + (pos+vector3df(0,-250.0f,0) - ray.start).normalize() * 1000.0f;
	core::vector3df intersection;
    core::triangle3df hitTriangle;
	scene::ISceneNode * selectedSceneNode =
    collMan->getSceneNodeAndCollisionPointFromRay(
		ray,
		intersection,
		hitTriangle,
		100,
		0);

	if (selectedSceneNode)
		return intersection.Y;
	else
		return 0;

    IMeshBuffer* meshBuffer = ((ITerrainSceneNode*)node)->getMesh()->getMeshBuffer(0);
	S3DVertex* mb_vertices = (S3DVertex*) meshBuffer->getVertices();
	f32 height = 0;
	vector3df nearestVertex = vector3df(1000,1000,1000);

	for (unsigned int j = 0; j < meshBuffer->getVertexCount(); j += 1) {
		vector3df realPos = mb_vertices[j].Pos*(node->getScale()/node->getBoundingBox().getExtent().X) + node->getPosition();

	    if(pos.getDistanceFrom(vector3df(realPos.X,0,realPos.Z)) < pos.getDistanceFrom(vector3df(nearestVertex.X,0,nearestVertex.Z))) {
	        nearestVertex = realPos;
	        height = realPos.Y;
	    }
	}

	return height;
}
