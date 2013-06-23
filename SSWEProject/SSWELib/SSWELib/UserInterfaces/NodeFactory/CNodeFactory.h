//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_NODE_FACTORY_H_INCLUDED__
#define __C_UI_NODE_FACTORY_H_INCLUDED__

#include "../../Device/CDevices.h"

class CNodeFactory {

public:

	CNodeFactory(CDevices *_devices);
	~CNodeFactory();

	void createCubeSceneNode();
	void createSphereSceneNode();
	void createPlaneMeshSceneNode();
	void createBillBoardSceneNode();
	void createLightSceneNode();

private:

	CDevices *devices;

};

#endif