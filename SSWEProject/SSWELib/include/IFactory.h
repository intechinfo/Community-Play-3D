//
//  IRender.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 27/12/2013.
//
//

#ifndef __I_CP3D_FACTORY_H_INCLUDED__
#define __I_CP3D_FACTORY_H_INCLUDED__

#include <irrlicht.h>
#include <functional>

namespace cp3d {
namespace scene {

class IFactory {
    
public:

	/// Creates a cube and returns it.
	/// callback: a lambda function
	virtual irr::scene::ISceneNode *createCubeSceneNode(std::function<void(irr::scene::ISceneNode *node)> callback = [](irr::scene::ISceneNode *node){}) = 0;


	virtual irr::scene::ISceneNode *createSphereSceneNode(std::function<void(irr::scene::ISceneNode *node)> callback = [](irr::scene::ISceneNode *node){}) = 0;


	virtual irr::scene::ISceneNode *createPlaneMeshSceneNode(std::function<void(irr::scene::ISceneNode *node)> callback = [](irr::scene::ISceneNode *node){}) = 0;


	virtual irr::scene::ISceneNode *createBillBoardSceneNode(std::function<void(irr::scene::ISceneNode *node)> callback = [](irr::scene::ISceneNode *node){}) = 0;


	virtual irr::scene::ISceneNode *createLightSceneNode(std::function<void(irr::scene::ISceneNode *node)> callback = [](irr::scene::ISceneNode *node){}) = 0;
    
};

} /// End namespace scene
} /// End namespace cp3d

#endif
