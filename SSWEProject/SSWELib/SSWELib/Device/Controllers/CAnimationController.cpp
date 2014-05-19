//
//  CUIOpenFileDialog.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//
#include "stdafx.h"

#include "../CDevices.h"
#include "CAnimationController.h"

namespace cp3d {
namespace controllers {

CAnimationController::CAnimationController(CDevices *_devices) {
	devices = _devices;
}

 CAnimationController::~CAnimationController() {

 }

void CAnimationController::applyAnimationToModel(irr::scene::ISceneNode *node, irr::u32 animationNumber) {
	if (node->getType() == ESNT_ANIMATED_MESH) {
		SObjectsData *sdatat = (SObjectsData*)devices->getCoreData()->getISDataOfSceneNode(node);
		if (animationNumber >= sdatat->getActions()->size())
			return;

		IAnimatedMeshSceneNode *node = (IAnimatedMeshSceneNode*)sdatat->getNode();
		if (sdatat->getActions()->operator[](animationNumber)->getAnimationPath() == "Current"
			|| sdatat->getActions()->operator[](animationNumber)->getAnimationPath() == "")
		{
			((ISkinnedMesh*)node->getMesh())->useAnimationFrom((ISkinnedMesh*)sdatat->getMesh());
		} else {
			IAnimatedMesh *smesh = devices->getSceneManager()->getMesh(sdatat->getActions()->operator[](animationNumber)->getAnimationPath());
			((ISkinnedMesh*)node->getMesh())->useAnimationFrom((ISkinnedMesh*)smesh);
		}

		((IAnimatedMeshSceneNode*)node)->setFrameLoop(sdatat->getActions()->operator[](animationNumber)->getStart(),
													  sdatat->getActions()->operator[](animationNumber)->getEnd());
		((IAnimatedMeshSceneNode*)node)->setAnimationSpeed(sdatat->getActions()->operator[](animationNumber)->getAnimSpeed());
	}
}

void CAnimationController::applyAnimationFromNameToModel(irr::scene::ISceneNode *node, irr::core::stringc name) {
	SObjectsData *sdatat = (SObjectsData*)devices->getCoreData()->getISDataOfSceneNode(node);
	if (sdatat) {
		for (u32 i=0; i < sdatat->getActions()->size(); i++) {
			if (name.make_upper() == sdatat->getActions()->operator[](i)->getName().make_upper()) {
				this->applyAnimationToModel(node, i);
				break;
			}
		}
	}
}

irr::s32 CAnimationController::getCurrentAnimationIndiceOf(irr::scene::ISceneNode *node) {
	IAnimatedMesh *mesh = ((IAnimatedMeshSceneNode*)node)->getMesh();
	SObjectsData *sdatat = (SObjectsData*)devices->getCoreData()->getISDataOfSceneNode(node);
	if (sdatat) {
		for (u32 i=0; i < sdatat->getActions()->size(); i++) {
			if (((IAnimatedMeshSceneNode*)node)->getFrameNr() >= sdatat->getActions()->operator[](i)->getStart()
				&& ((IAnimatedMeshSceneNode*)node)->getFrameNr() <= sdatat->getActions()->operator[](i)->getEnd())
			{
				for (u32 j=0; j < sdatat->getAnimationMeshes()->size(); j++) {
					if (mesh == sdatat->getAnimationMeshes()->operator[](j)) {
						return i;
					}
				}
			}
		}
	}
	return -1;
}

} /// End namespace controllers
} /// End namespace cp3d
