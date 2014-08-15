//
//  IRender.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 27/12/2013.
//
//

#ifndef __I_CP3D_MESH_SIMPLIFICATOR_H_INCLUDED__
#define __I_CP3D_MESH_SIMPLIFICATOR_H_INCLUDED__

#include <irrlicht.h>
#include <functional>

namespace cp3d {
namespace scene {

class IMeshSimplificator {
    
public:

	/// Creates a new simplified mesh buffer
	virtual void addSimplifiedMeshBuffer(irr::scene::IMeshBuffer *buffer) = 0;

	/// Simply the select mesh buffer
	virtual void simplifyMeshBuffer(irr::scene::IMeshBuffer *buffer, irr::f32 percentage,
		std::function<void(irr::scene::IMeshBuffer *buffer)> callback = [](irr::scene::IMeshBuffer *buffer){}) = 0;

	/// Returns the simplified mesh buffer of the mesh buffer "buffer"
	virtual irr::scene::IMeshBuffer *getSimplifiedMeshBuffer(irr::scene::IMeshBuffer *buffer) = 0;

	/// Replaces the mesh buffer
	virtual void switchToSimplifiedMeshBuffer(irr::scene::IMeshBuffer *buffer) = 0;

	/// Clear all simplified mesh buffers
	virtual void clearAll() = 0;
    
};

} /// End namespace scene
} /// End namespace cp3d

#endif
