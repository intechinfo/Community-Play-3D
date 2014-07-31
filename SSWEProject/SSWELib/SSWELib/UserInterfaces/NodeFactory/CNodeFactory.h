//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_NODE_FACTORY_H_INCLUDED__
#define __C_UI_NODE_FACTORY_H_INCLUDED__

#include <IFactory.h>
#include "../../Device/CDevices.h"

class CNodeFactory : public cp3d::scene::IFactory {

public:

	CNodeFactory(CDevices *_devices);
	~CNodeFactory();

	ISceneNode *createCubeSceneNode(std::function<void(irr::scene::ISceneNode *node)> callback = [](irr::scene::ISceneNode *node){});

	ISceneNode *createSphereSceneNode(std::function<void(irr::scene::ISceneNode *node)> callback = [](irr::scene::ISceneNode *node){});

	ISceneNode *createPlaneMeshSceneNode(std::function<void(irr::scene::ISceneNode *node)> callback = [](irr::scene::ISceneNode *node){});

	ISceneNode *createBillBoardSceneNode(std::function<void(irr::scene::ISceneNode *node)> callback = [](irr::scene::ISceneNode *node){});

	ISceneNode *createLightSceneNode(std::function<void(irr::scene::ISceneNode *node)> callback = [](irr::scene::ISceneNode *node){});

private:

	CDevices *devices;

};

#endif