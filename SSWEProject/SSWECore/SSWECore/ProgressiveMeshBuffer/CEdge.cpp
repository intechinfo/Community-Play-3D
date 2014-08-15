//
//  enums.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 04/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "stdafx.h"
#include "CEdge.h"
#include "CTriangle.h"

Edge::Edge(CVertex* from, CVertex* to) {
	this->from = from;
	this->to = to;
	cost = 0;

	updateFaces();
}

Edge::~Edge() {
	faces.clear();
}

CVertex* Edge::getVertexFrom() const {
	return from;
}

CVertex* Edge::getVertexTo() const {
	return to;
}

irr::f32 Edge::getCost() const {
	return cost;
}

void Edge::setCost(irr::f32 cost) {
	this->cost = cost;
}

const irr::core::array<CTriangle*>& Edge::getFaces() const {
	return faces;
}

void Edge::updateFaces() {
	faces.clear();
	// Add faces
	const irr::core::array<CTriangle*> facesFrom = from->getFaces();
	for (irr::u32 i = 0; i < facesFrom.size(); ++i) {
		CTriangle* t1 = facesFrom[i];
		if (t1 && t1->isActive()) {
			const irr::core::array<CTriangle*> facesTo = to->getFaces();
			for (irr::u32 i2 = 0; i2 < facesTo.size(); ++i2) {
				CTriangle* t2 = facesTo[i2];
				if (t1 == t2) {
					faces.push_back(t1);
				}
			}
		}
	}
}

void Edge::addFace(CTriangle* triangle) {
	if (triangle && triangle->isActive()) {
		bool add = true;

		// Check if face already exists for this edge
		for (irr::u32 i = 0; i < faces.size(); ++i) {
			CTriangle* t = faces[i];
			if (triangle == t) {
				add = false;
				break;
			}
		}
		if (add) {
			faces.push_back(triangle);
		}
	}
}

void Edge::removeFace(CTriangle* triangle) {
	for (irr::u32 i = 0; i < faces.size(); ++i) {
		CTriangle* t = faces[i];
		if (triangle == t) {
			faces.erase(i);
			break;
		}
	}
}

bool Edge::isBorder() const {
	return (faces.size() == 1);
}

bool Edge::hasVertex(CVertex* v) const {
	return (v == from || v == to);
}

void Edge::replaceVertex(CVertex* from, CVertex* to) {
	// Replace from with to vertex
	if (this->from == from) {
		this->from = to;
	} else if (this->to == from) {
		this->to = to;
	}

	// Check faces
	updateFaces();
}

bool Edge::isValid() const {
	// One of the vertexes active
	if (!from || !to) {
		return false;
	}
	if (!from->isActive() || !to->isActive()) {
		return false;
	}
	if (from == to) {
		return false;
	}
	if (faces.size() == 0) {
		return false;
	}
	return true;
}

void Edge::print() const {
	printf("Edge: ");
	from->print();
	printf(" -> ");
	to->print();
	printf(" Cost: %f Faces: %d\n", cost, faces.size());
}

irr::core::stringw Edge::getIdString() {
	irr::core::stringw string = "";
	if (from->getId() < to->getId()) {
		string += from->getId();
		string += ";";
		string += to->getId();
	} else {
		string += to->getId();
		string += ";";
		string += from->getId();
	}
	return string;
}
