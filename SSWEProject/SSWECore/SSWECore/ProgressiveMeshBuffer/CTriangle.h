//
//  enums.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 04/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include <irrlicht.h>
#include "CVertex.h"
#include "CEdge.h"

class CTriangle {

public:

	CTriangle(CVertex* v1, CVertex* v2, CVertex* v3);
	virtual ~CTriangle();

	CVertex* getVertex1() const;
	CVertex* getVertex2() const;
	CVertex* getVertex3() const;
	CVertex* getVertex(irr::u32 index) const;

	bool isActive() const;
	void setActive(bool active);

	irr::core::vector3df getNormal() const;
	irr::f32 getArea() const;

	bool hasVertex(CVertex* v) const;
	void replaceVertex(CVertex* from, CVertex* to);

	void print() const;

private:

	CVertex* vertex[3];
	bool active;

};

#endif /* TRIANGLE_H_ */

