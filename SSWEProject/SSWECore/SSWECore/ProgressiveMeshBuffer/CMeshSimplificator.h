//
//  enums.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 04/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef __C_MESH_SIMPLIFICATOR_H_INCLUDED__
#define __C_MESH_SIMPLIFICATOR_H_INCLUDED__

#include <SSWECore.h>
#include "CProgressiveMeshBuffer.h"
#include <IMeshSimplificator.h>

class SSWE_CORE_API CMeshSimplificator : public cp3d::scene::IMeshSimplificator {

public:

	/// ctor & dtor
	CMeshSimplificator(irr::scene::ISceneManager *smgr);
	~CMeshSimplificator();

	void addSimplifiedMeshBuffer(irr::scene::IMeshBuffer *buffer);
	void simplifyMeshBuffer(irr::scene::IMeshBuffer *buffer, irr::f32 percentage,
		std::function<void(irr::scene::IMeshBuffer *buffer)> callback = [](irr::scene::IMeshBuffer *buffer){});

	irr::scene::IMeshBuffer *getSimplifiedMeshBuffer(irr::scene::IMeshBuffer *buffer) { return 0; }

	void switchToSimplifiedMeshBuffer(irr::scene::IMeshBuffer *buffer);
	void switchToOriginalMeshBuffer(irr::scene::IMeshBuffer *buffer);

	void clearAll();

private:

	struct SProgressiveBuffer {
	public:
		SProgressiveBuffer() {
			meshBuffer = 0;
			pmb = 0;
			initialTriangleCount = 0;
			originalBuffer = 0;
		}
		irr::scene::IMeshBuffer *meshBuffer; /// Mesh buffer to compute
		irr::scene::ProgressiveMeshBuffer *pmb;
		irr::u32 initialTriangleCount;
		irr::scene::IMeshBuffer *originalBuffer; /// Copy of the mesh buffer
	};

	irr::core::list<SProgressiveBuffer *> progressiveBuffers;

	irr::scene::ISceneManager *smgr;

	/// Private functions
	void switchMeshBuffers(irr::scene::IMeshBuffer *buffer, irr::scene::IMeshBuffer *source);

	/// Threaded functions
	void simplifyMeshBuffer_t(irr::scene::IMeshBuffer *buffer, irr::f32 percentage, std::function<void(irr::scene::IMeshBuffer *buffer)> callback);

};


#endif
