//
//  enums.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 04/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef EDGE_H_
#define EDGE_H_

#include <irrlicht.h>
#include "CVertex.h"

class Edge {

public:

	Edge(CVertex* from, CVertex* to);
	virtual ~Edge();

	CVertex* getVertexFrom() const;
	CVertex* getVertexTo() const;

	irr::f32 getCost() const;
	void setCost(irr::f32 cost);

	bool isBorder() const;

	const irr::core::array<CTriangle*>& getFaces() const;

	void updateFaces();

	void addFace(CTriangle* triangle);
	void removeFace(CTriangle* triangle);

	bool hasVertex(CVertex* v) const;
	void replaceVertex(CVertex* from, CVertex* to);

	bool isValid() const;
	void print() const;

	irr::core::stringw getIdString();

	bool operator<(const Edge& other) const {
		return (getCost() < other.getCost());
	}

	bool operator==(const Edge& other) const {
		return (getCost() == other.getCost());
	}

private:

	CVertex* from;
	CVertex* to;
	irr::f32 cost;
	irr::core::array<CTriangle*> faces;

};

#endif /* EDGE_H_ */
