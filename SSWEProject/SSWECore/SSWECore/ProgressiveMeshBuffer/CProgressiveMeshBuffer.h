//
//  enums.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 04/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef PROGRESSIVE_MESH_BUFFER_H_
#define PROGRESSIVE_MESH_BUFFER_H_

#include <irrlicht.h>
#include "CVertex.h"
#include "CTriangle.h"
#include "CEdge.h"

namespace irr {
namespace scene {

class ProgressiveMeshBuffer {

public:

	enum SimplificationAlgorithm {
		RANDOM = 0,
		SHORTEST,
		MELAX,
		QUADRIC,
		QUADRICTRI
	};

public:
	ProgressiveMeshBuffer(scene::IMeshBuffer* meshBuffer, SimplificationAlgorithm algorithm = QUADRICTRI);
	virtual ~ProgressiveMeshBuffer();

	const irr::core::array<CTriangle*>& getTriangles() const;
	const irr::core::array<CVertex*>& getVertices() const;

	scene::IMeshBuffer* getCurrentMeshBuffer();

	void contract(u32 numberOfEdges = 1);

	void contractTillTriangleCount(u32 numberOfTriangles);

	ProgressiveMeshBuffer::SimplificationAlgorithm getAlgorithm() const;

	core::stringc getAlgorithmString() const;

private:

	void calculateQuadric(bool useTriangleArea = true);

	void createEdges();

	void calculateEdgeCost();

	void calculateEdgeCost(Edge* edge);

	void calculateRandomEdgeCost(Edge* edge);

	void calculateShortEdgeCost(Edge* edge);

	void calculateMelaxEdgeCost(Edge* edge);

	void calculateQuadricEdgeCost(Edge* edge);

	void applyBorderPenalties(Edge* edge);

	f32 calcQuadricError(core::matrix4 quadric, CVertex* v, f32 triangleArea);

	void contract(Edge* edge);

private:

	scene::IMeshBuffer* meshBuffer;
	SimplificationAlgorithm algorithm;
	irr::core::array<CVertex*> vertices;
	irr::core::array<CTriangle*> triangles;
	irr::core::array<Edge*> edges;

};

} // end namespace scene
} // end namespace irr

#endif /* PROGRESSIVE_MESH_BUFFER_H_ */
