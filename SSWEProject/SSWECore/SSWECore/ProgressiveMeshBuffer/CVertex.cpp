//
//  enums.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 04/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "stdafx.h"
#include "CVertex.h"
#include "CTriangle.h"

using namespace irr;

CVertex::CVertex(irr::u32 id, irr::core::vector3df position, irr::core::vector3df normal, irr::video::SColor color, irr::core::vector2df textureCoordinates) {
	this->id = id;
	this->position = position;
	this->normal = normal;
	this->color = color;
	this->textureCoordinates = textureCoordinates;
	active = true;

	// Init quadric
	for (irr::u32 i = 0; i < 16; i++) {
		quadric[i] = -1;
	}
}

CVertex::~CVertex() {
	faces.clear();
}

irr::u32 CVertex::getId() const {
	return id;
}

void CVertex::setId(irr::u32 id) {
	this->id = id;
}

const irr::core::vector3df& CVertex::getPosition() const {
	return position;
}

void CVertex::setPosition(irr::core::vector3df position) {
	this->position = position;
}

const irr::core::vector3df& CVertex::getNormal() const {
	return normal;
}

const irr::video::SColor& CVertex::getColor() const {
	return color;
}

void CVertex::setColor(irr::video::SColor color) {
	this->color = color;
}

const irr::core::vector2df& CVertex::getTextureCoordinates() const {
	return textureCoordinates;
}

void CVertex::setTextureCoordinates(irr::core::vector2df textureCoordinates) {
	this->textureCoordinates = textureCoordinates;
}

bool CVertex::isActive() const {
	return active;
}

void CVertex::setActive(bool active) {
	this->active = active;
}

const irr::core::array<CTriangle*>& CVertex::getFaces() const {
	return faces;
}

void CVertex::addFace(CTriangle* triangle) {
	if (triangle && triangle->isActive()) {
		bool add = true;

		// Check if face already exists for this vertex
		for (u32 i = 0; i < faces.size(); ++i) {
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

void CVertex::removeFace(CTriangle* triangle) {
	for (u32 i = 0; i < faces.size(); ++i) {
		CTriangle* t = faces[i];
		if (triangle == t) {
			faces.erase(i);
			break;
		}
	}
}

const irr::core::matrix4& CVertex::getQuadric() const {
	return quadric;
}

void CVertex::setQuadric(irr::core::matrix4 quadric) {
	this->quadric = quadric;
}

void CVertex::calcQuadric(bool useTriangleArea) {
	for (irr::u32 i = 0; i < 16; i++) {
		quadric[i] = 0;
	}

	for (irr::u32 i = 0; i < faces.size(); ++i) {
		CTriangle* t = faces[i];

		if (t->isActive()) {
			irr::f32 triangleArea = 1;
			if (useTriangleArea) {
				triangleArea = t->getArea();
				quadricTriangleArea += triangleArea;
			}

			const irr::core::vector3df normal = t->getNormal();
			const irr::f32 a = normal.X;
			const irr::f32 b = normal.Y;
			const irr::f32 c = normal.Z;
			const irr::f32 d = normal.dotProduct(t->getVertex1()->getPosition());

			quadric[0] += triangleArea * a * a;
			quadric[1] += triangleArea * a * b;
			quadric[2] += triangleArea * a * c;
			quadric[3] += triangleArea * a * d;

			quadric[4] += triangleArea * b * a;
			quadric[5] += triangleArea * b * b;
			quadric[6] += triangleArea * b * c;
			quadric[7] += triangleArea * b * d;

			quadric[8] += triangleArea * c * a;
			quadric[9] += triangleArea * c * b;
			quadric[10] += triangleArea * c * c;
			quadric[11] += triangleArea * c * d;

			quadric[12] += triangleArea * d * a;
			quadric[13] += triangleArea * d * b;
			quadric[14] += triangleArea * d * c;
			quadric[15] += triangleArea * d * d;
		}
	}
}

irr::f32 CVertex::getQuadricTriangleArea() const {
	return quadricTriangleArea;
}

void CVertex::setQuadricTriangleArea(irr::f32 quadricTriangleArea) {
	this->quadricTriangleArea = quadricTriangleArea;
}

void CVertex::print() const {
	if (active) {
		printf("[%d  - ACTIVE]", id);
	} else {
		printf("[%d  - INACTIVE]", id);
	}
}

bool CVertex::isOrphan() const {
	return (faces.size() == 0);
}

/// Vertex2TCoords
CVertex2TCoords::CVertex2TCoords(u32 id, core::vector3df position, core::vector3df normal, video::SColor color, core::vector2df textureCoordinates, core::vector2df textureCoordinates2) :
		CVertex(id, position, normal, color, textureCoordinates) {
	this->textureCoordinates2 = textureCoordinates2;
}

CVertex2TCoords::~CVertex2TCoords() {
}

const core::vector2df& CVertex2TCoords::getTextureCoordinates2() const {
	return textureCoordinates2;
}

void CVertex2TCoords::setTextureCoordinates2(core::vector2df textureCoordinates2) {
	this->textureCoordinates2 = textureCoordinates2;
}

/// VertexTangets
CVertexTangets::CVertexTangets(u32 id, core::vector3df position, core::vector3df normal, video::SColor color, core::vector2df textureCoordinates, core::vector3df tangent, core::vector3df binormal) :
		CVertex(id, position, normal, color, textureCoordinates) {
	this->tangent = tangent;
	this->binormal = binormal;
}

CVertexTangets::~CVertexTangets() {
}

const core::vector3df& CVertexTangets::getTangent() const {
	return tangent;
}

const core::vector3df& CVertexTangets::getBinormal() const {
	return binormal;
}
