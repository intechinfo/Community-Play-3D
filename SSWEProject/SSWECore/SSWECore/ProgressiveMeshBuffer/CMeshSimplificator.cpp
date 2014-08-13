//
//  enums.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 04/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "stdafx.h"
#include "CMeshSimplificator.h"

#include <thread>

using namespace irr;
using namespace core;
using namespace scene;
using namespace cp3d::scene;

//---------------------------------------------------------------------------------------------
//-----------------------------------PUBLIC----------------------------------------------------
//---------------------------------------------------------------------------------------------
CMeshSimplificator::CMeshSimplificator(irr::scene::ISceneManager *smgr) {
	this->smgr = smgr;
}

CMeshSimplificator::~CMeshSimplificator() {

}

void CMeshSimplificator::clearAll() {
	list<SProgressiveBuffer *>::ConstIterator it = progressiveBuffers.begin();
	for (; it != progressiveBuffers.end(); ++it) {
		delete (*it)->pmb;
	}
	progressiveBuffers.clear();
}

void CMeshSimplificator::addSimplifiedMeshBuffer(IMeshBuffer *buffer) {
	SProgressiveBuffer *spb = new SProgressiveBuffer();
	spb->meshBuffer = buffer;
	spb->pmb = new ProgressiveMeshBuffer(buffer, ProgressiveMeshBuffer::MELAX);
	spb->initialTriangleCount = buffer->getIndexCount() / 3;
	spb->originalBuffer = spb->pmb->getCurrentMeshBuffer();

	progressiveBuffers.push_back(spb);
}

void CMeshSimplificator::switchToSimplifiedMeshBuffer(irr::scene::IMeshBuffer *buffer) {
	list<SProgressiveBuffer *>::ConstIterator it = progressiveBuffers.begin();
	for (; it != progressiveBuffers.end(); ++it) {
		if ((*it)->meshBuffer == buffer) {
			switchMeshBuffers(buffer, (*it)->pmb->getCurrentMeshBuffer());
			break;
		}
	}
}

void CMeshSimplificator::switchToOriginalMeshBuffer(irr::scene::IMeshBuffer *buffer) {
	list<SProgressiveBuffer *>::ConstIterator it = progressiveBuffers.begin();
	for (; it != progressiveBuffers.end(); ++it) {
		if ((*it)->meshBuffer == buffer) {
			switchMeshBuffers(buffer, (*it)->originalBuffer);
			break;
		}
	}
}

void CMeshSimplificator::simplifyMeshBuffer(IMeshBuffer *buffer, irr::f32 percentage, std::function<void(irr::scene::IMeshBuffer *buffer)> callback) {
	std::thread t(&CMeshSimplificator::simplifyMeshBuffer_t, *this, buffer, percentage, callback);
	t.detach();
}

//---------------------------------------------------------------------------------------------
//-----------------------------------PRIVATE---------------------------------------------------
//---------------------------------------------------------------------------------------------

void CMeshSimplificator::switchMeshBuffers(irr::scene::IMeshBuffer *buffer, irr::scene::IMeshBuffer *source) {

	video::S3DVertex *vertices = (video::S3DVertex *)source->getVertices();
	video::S3DVertex2TCoords *vertices2tc = (video::S3DVertex2TCoords *)source->getVertices();
	video::S3DVertexTangents *verticestan = (video::S3DVertexTangents *)source->getVertices();

	/// Check for skinned mesh buffers (SSkinMeshBuffer)
	if (dynamic_cast<SSkinMeshBuffer *>(buffer)) {
		((SSkinMeshBuffer*)buffer)->Indices.clear();

		if (buffer->getVertexType() == video::EVT_STANDARD) {
			((SSkinMeshBuffer*)buffer)->Vertices_Standard.clear();

			for (u32 i=0; i < source->getVertexCount(); i++) {
				((SSkinMeshBuffer*)buffer)->Vertices_Standard.push_back(vertices[i]);
			}
			for (u32 i=0; i < source->getIndexCount(); i++) {
				((SSkinMeshBuffer*)buffer)->Indices.push_back(source->getIndices()[i]);
			}
		} else if (buffer->getVertexType() == video::EVT_TANGENTS) {
			((SSkinMeshBuffer*)buffer)->Vertices_Tangents.clear();

			for (u32 i=0; i < source->getVertexCount(); i++) {
				((SSkinMeshBuffer*)buffer)->Vertices_Tangents.push_back(verticestan[i]);
			}
		}

		/// Finish SSkinMeshBuffer
		for (u32 i=0; i < source->getIndexCount(); i++) {
			((SSkinMeshBuffer*)buffer)->Indices.push_back(source->getIndices()[i]);
		}
		((SSkinMeshBuffer*)buffer)->recalculateBoundingBox();
	}
	/// Else if CMeshBuffer
	else if (dynamic_cast<CMeshBuffer<video::S3DVertex> *>(buffer)
		|| dynamic_cast<CMeshBuffer<video::S3DVertex2TCoords> *>(buffer)
		|| dynamic_cast<CMeshBuffer<video::S3DVertexTangents> *>(buffer))
	{
		CMeshBuffer<video::S3DVertex> *mb = (CMeshBuffer<video::S3DVertex> *)buffer;
		mb->Vertices.clear();
		mb->Indices.clear();

		for (u32 i=0; i < source->getVertexCount(); i++) {
			if (source->getVertexType() == video::EVT_STANDARD) {
				mb->Vertices.push_back(vertices[i]);
			} else if (source->getVertexType() == video::EVT_2TCOORDS) {
				((CMeshBuffer<video::S3DVertex2TCoords>*)buffer)->Vertices.push_back(vertices2tc[i]);
			} else {
				((CMeshBuffer<video::S3DVertexTangents>*)buffer)->Vertices.push_back(verticestan[i]);
			}
		}
		for (u32 i=0; i < source->getIndexCount(); i++) {
			mb->Indices.push_back(source->getIndices()[i]);
		}
		mb->recalculateBoundingBox();
	} else {
		exit(0);
	}

	buffer->setDirty();
}

//---------------------------------------------------------------------------------------------
//-----------------------------------THREADS---------------------------------------------------
//---------------------------------------------------------------------------------------------

void CMeshSimplificator::simplifyMeshBuffer_t(IMeshBuffer *buffer, irr::f32 percentage, std::function<void(irr::scene::IMeshBuffer *buffer)> callback) {
	list<SProgressiveBuffer *>::ConstIterator it = progressiveBuffers.begin();
	for (; it != progressiveBuffers.end(); ++it) {
		if ((*it)->meshBuffer == buffer) {

			u32 triangleCount = ((*it)->pmb->getCurrentMeshBuffer()->getIndexCount() / 3) - ((f32)(*it)->initialTriangleCount * percentage / 100.f);
			if ((*it)->pmb->getTriangles().size() < triangleCount) {
				triangleCount = 2;
			}
			(*it)->pmb->contractTillTriangleCount(triangleCount);

			callback((*it)->pmb->getCurrentMeshBuffer());
			buffer->setDirty();
			break;
		}
	}
}
