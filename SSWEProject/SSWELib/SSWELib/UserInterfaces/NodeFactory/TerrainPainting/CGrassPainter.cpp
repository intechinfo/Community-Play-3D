/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include "CGrassPainter.h"

CGrassPainter::CGrassPainter(CDevices *_devices, ITerrainSceneNode * _node) {
	devices = _devices;
	node = _node;

	currentQuality = 3;
	currentScale = 10;
	currentDistance = 10;

	buffer = new SMeshBuffer();
	S3DVertex vertices[4];
	vertices[0] = S3DVertex(1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, SColor(0, 255, 255, 255), 1.0f, 1.0f);
	vertices[1] = S3DVertex(1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, SColor(0, 255, 255, 255), 1.0f, 0.0f);
	vertices[2] = S3DVertex(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, SColor(0, 255, 255, 255), 0.0f, 0.0f);
	vertices[3] = S3DVertex(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, SColor(0, 255, 255, 255), 0.0f, 1.0f);
	u16 indices[6] = { 0, 1, 2, 0, 2, 3 };
	buffer->append(vertices, 4, indices, 6);

	mesh = new SMesh();
	mesh->addMeshBuffer(buffer);
}

CGrassPainter::~CGrassPainter() {
	buffer->drop();
	mesh->drop();
}

void CGrassPainter::paint(vector3df pos, s32 scale, s32 quality, s32 distance, bool remove) {
	bool canPaint = true;
	f32 scalef = scale;
	f32 currentDistancef = currentDistance;
	f32 qualityf = quality;
	f32 distancef = distance;
	f32 space =	distancef/10.f;

	core::list<SGrassData *>::Iterator grassData = grassList.begin();
	for (; grassData != grassList.end(); ++grassData) {
		ISceneNode *gdnode = (*grassData)->node;

		if (gdnode->getPosition().getDistanceFrom(vector3df(pos.X, pos.Y+(scalef/2), pos.Z)) < space) {
			canPaint = false;
			if (remove) {
				for (u32 i=0; i < (*grassData)->nodes.size(); i++) {
					devices->getXEffect()->removeShadowFromNode((*grassData)->nodes[i]);
					devices->getXEffect()->removeNodeFromDepthPass((*grassData)->nodes[i]);
					(*grassData)->nodes[i]->remove();
				}
				delete *grassData;
				grassList.erase(grassData);
				break;
			}
		}
	}

	if (!remove) {
		if (canPaint) {
			SGrassData *gdata = new SGrassData(0, pos, scale, quality);
			f32 rotation = 180.f/qualityf;
			for(u32 i=0; i < quality; i++) {
				IMeshSceneNode *gnode = devices->getSceneManager()->addOctreeSceneNode(mesh, 0, -1, 1, false);

				if (gnode) {
					gnode->setMaterialFlag(EMF_LIGHTING, false);
					gnode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
					gnode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);

					ITexture * texture = devices->getVideoDriver()->getTexture("grass.png");
					gnode->setMaterialTexture(0, texture);

					gnode->setScale(vector3df(scalef));
					gnode->setPosition(vector3df(pos.X, pos.Y+(scalef/2), pos.Z));
					gnode->setRotation(vector3df(180, rotation*i, 0));

					for (u32 j=0; j < gnode->getMesh()->getMeshBufferCount(); j++)
						gnode->getMesh()->getMeshBuffer(j)->recalculateBoundingBox();

					devices->getXEffect()->addShadowToNode(gnode, devices->getXEffectFilterType(), ESM_EXCLUDE);
					gdata->node = gnode;
					gdata->nodes.push_back(gnode);
				}
			}
			grassList.push_back(gdata);
		}
	}
}
