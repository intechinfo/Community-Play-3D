#ifndef IGRAPHIC_TOOL_H
#define IGRAPHIC_TOOL_H

#include <irrlicht.h>

#include "../SSWECore/SSWECore/DecaleSystem/DecalManager.h"
#include "../SSWECore/SSWECore/DecaleSystem/CBatchingMesh.h"

namespace cp3d {
namespace tool {

class IGraphicTool {

public:

	//DECAL MANAGER
	virtual void setDecalManager(DecalManager *decalManager) = 0;
	virtual DecalManager *getDecalManager() const = 0;

};

} /// End namespace tool
} /// End namespace cp3d

#endif