//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C__MESH_FACTORY_H_INCLUDED__
#define __C__MESH_FACTORY_H_INCLUDED__

#include "../../Device/CDevices.h"

class CMeshFactory {

public:

	CMeshFactory(CDevices *_devices);
	~CMeshFactory();

	void reloadMesh(IMesh *mesh, stringc meshName);

	array<ISceneNode *> getNodesThatUse(IMesh *mesh);

private:

	CDevices *devices;

	void modifyMeshOfSceneNode(ISceneNode *node, IMesh *mesh, bool isAnimated);

};

#endif