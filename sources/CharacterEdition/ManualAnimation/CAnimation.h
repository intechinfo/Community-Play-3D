//
//  CAction.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 07/11/12.
//
//

#ifndef __C_ANIMATION_H_INCLUDED__
#define __C_ANIMATION_H_INCLUDED__

#include <irrlicht.h>

#include "CManualAction.h"

//SBONESINFORMATIONS STRUCTURE
struct SBoneInformations {

	SBoneInformations(irr::scene::IBoneSceneNode *_node, irr::core::vector3df _position, irr::core::vector3df _rotation, irr::core::vector3df _scale) {
		position = _position;
		rotation = _rotation;
		scale = _scale;

		node = _node;

		manualActions.clear();
		name = L"myName";
	}

	//SET METHODS
	void setName(irr::core::stringw _name) { name = _name; }

	void setNewPosition(irr::core::vector3df _position) { newPosition = _position; }
	void setNewRotation(irr::core::vector3df _rotation) { newRotation = _rotation; }
	void setNewScale(irr::core::vector3df _scale) { newScale = _scale; }

	//GET METHODS
	irr::core::stringw getName() { return name; }

	irr::core::vector3df getPosition() { return position; }
	irr::core::vector3df getRotation() { return rotation; }
	irr::core::vector3df getScale() { return scale; }

	irr::core::vector3df getNewPosition() { return newPosition; }
	irr::core::vector3df getNewRotation() { return newRotation; }
	irr::core::vector3df getNewScale() { return newScale; }

	irr::scene::IBoneSceneNode *getNode() { return node; }

	irr::core::array<CManualAction *> *getManualActions() { return &manualActions; }

private:
	irr::core::stringw name;

	irr::core::vector3df position, newPosition;
	irr::core::vector3df rotation, newRotation;
	irr::core::vector3df scale, newScale;

	irr::scene::IBoneSceneNode *node;

	irr::core::array<CManualAction *> manualActions;
};

//CANIMATION CLASSE
class CAnimation {

public:

	CAnimation(irr::scene::IAnimatedMeshSceneNode *_node);
	~CAnimation();

	irr::core::array<SBoneInformations> *getBonesInformations() { return &bonesInformations; }

	irr::core::stringw getName() { return name; }
	void setName(irr::core::stringw _name) { name = _name; }

	CManualAction *addManualAction();

private:

	irr::scene::IAnimatedMeshSceneNode *node;
	irr::core::array<SBoneInformations> bonesInformations;

	irr::core::stringw name;

};

#endif