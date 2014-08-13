//
//  enums.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 04/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "stdafx.h"
#include "CTriangle.h"

using namespace irr;

CTriangle::CTriangle(CVertex* v1, CVertex* v2, CVertex* v3) {
	vertex[0] = v1;
	vertex[1] = v2;
	vertex[2] = v3;

	active = true;

	for (u32 i = 0; i < 3; i++) {
		// Add this triangle / face for each vertex
		vertex[i]->addFace(this);
	}
}

CTriangle::~CTriangle() {
	vertex[0]->removeFace(this);
	vertex[1]->removeFace(this);
	vertex[2]->removeFace(this);
}

CVertex* CTriangle::getVertex1() const {
	return vertex[0];
}

CVertex* CTriangle::getVertex2() const {
	return vertex[1];
}

CVertex* CTriangle::getVertex3() const {
	return vertex[2];
}

CVertex* CTriangle::getVertex(u32 index) const {
	if (index > 3) {
		index = 0;
	}
	return vertex[index];
}

bool CTriangle::isActive() const {
	return active;
}

void CTriangle::setActive(bool active) {
	this->active = active;
}

core::vector3df CTriangle::getNormal() const {
	return (vertex[1]->getPosition() - vertex[0]->getPosition()).crossProduct(vertex[2]->getPosition() - vertex[0]->getPosition());
}

f32 CTriangle::getArea() const {
	return (vertex[1]->getPosition() - vertex[0]->getPosition()).crossProduct(vertex[2]->getPosition() - vertex[0]->getPosition()).getLength() * 0.5f;
}

bool CTriangle::hasVertex(CVertex* v) const {
	return (v == vertex[0] || v == vertex[1] || v == vertex[2]);
}

void CTriangle::replaceVertex(CVertex* from, CVertex* to) {
	// Remove this triangle from from vertex
	from->removeFace(this);

	// Replace from with to vertex
	for (u32 i = 0; i < 3; i++) {
		if (vertex[i] == from) {
			vertex[i] = to;
			vertex[i]->addFace(this);
			break;
		}
	}
}

void CTriangle::print() const {
	printf("Triangle: ");
	for (u32 i = 0; i < 3; i++) {
		vertex[i]->print();
		if (i < 2) {
			printf(" -> ");
		} else {
			printf("\n");
		}
	}
}
