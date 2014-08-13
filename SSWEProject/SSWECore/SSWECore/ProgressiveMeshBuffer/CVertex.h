//
//  enums.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 04/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef VERTEX_H_
#define VERTEX_H_

#include <irrlicht.h>

class CTriangle;

class CVertex {

public:

	CVertex(irr::u32 id, irr::core::vector3df position, irr::core::vector3df normal, irr::video::SColor color = irr::video::SColor(255, 255, 255, 255),
			irr::core::vector2df textureCoordinates = irr::core::vector2df(0, 0));
	virtual ~CVertex();

	irr::u32 getId() const;
	void setId(irr::u32 id);

	const irr::core::vector3df& getPosition() const;
	void setPosition(irr::core::vector3df position);

	const irr::core::vector3df& getNormal() const;

	const irr::video::SColor& getColor() const;
	void setColor(irr::video::SColor color);

	const irr::core::vector2df& getTextureCoordinates() const;
	void setTextureCoordinates(irr::core::vector2df textureCoordinates);

	bool isActive() const;
	void setActive(bool active);

	const irr::core::array<CTriangle*>& getFaces() const;
	void addFace(CTriangle* triangle);
	void removeFace(CTriangle* triangle);

	const irr::core::matrix4& getQuadric() const;
	void setQuadric(irr::core::matrix4 quadric);
	void calcQuadric(bool useTriangleArea);

	irr::f32 getQuadricTriangleArea() const;
	void setQuadricTriangleArea(irr::f32 quadricTriangleArea);

	void print() const;

	bool isOrphan() const;

private:

	irr::u32 id;
	irr::core::vector3df position;
	irr::core::vector3df normal;
	irr::video::SColor color;
	irr::core::vector2df textureCoordinates;
	bool active; // false if vertex has been removed
	irr::core::array<CTriangle*> faces; // triangles of which this vertex is part of
	irr::core::matrix4 quadric; // Used for Quadric error cost.
	irr::f32 quadricTriangleArea; // summed area of triangles used to computer quadrics

};

class CVertex2TCoords: public CVertex {

public:

	CVertex2TCoords(irr::u32 id, irr::core::vector3df position, irr::core::vector3df normal, irr::video::SColor color = irr::video::SColor(255, 255, 255, 255), irr::core::vector2df textureCoordinates =
			irr::core::vector2df(), irr::core::vector2df textureCoordinates2 = irr::core::vector2df());
	virtual ~CVertex2TCoords();

	const irr::core::vector2df& getTextureCoordinates2() const;
	void setTextureCoordinates2(irr::core::vector2df textureCoordinates2);

private:

	irr::core::vector2df textureCoordinates2;

};

class CVertexTangets: public CVertex {

public:

	CVertexTangets(irr::u32 id, irr::core::vector3df position, irr::core::vector3df normal, irr::video::SColor color = irr::video::SColor(255, 255, 255, 255), irr::core::vector2df textureCoordinates =
			irr::core::vector2df(), irr::core::vector3df tangent = irr::core::vector3df(), irr::core::vector3df binormal = irr::core::vector3df());
	virtual ~CVertexTangets();

	const irr::core::vector3df& getTangent() const;
	const irr::core::vector3df& getBinormal() const;

private:

	irr::core::vector3df tangent;
	irr::core::vector3df binormal;

};

#endif /* VERTEX_H_ */
