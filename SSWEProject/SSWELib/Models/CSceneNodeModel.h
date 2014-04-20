// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés

#ifndef __C_SCENE_NODE_MODEL_H_INCLUDED__
#define __C_SCENE_NODE_MODEL_H_INCLUDED__

/// ENCAPSULATE THE ISCENENODE CLASS

#include <irrlicht.h>

/// Types
#include "../Types/Cfvector3d.h"

/// Flip
#include "ohm/flip/TxSessionGuard.h"

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif

template<class T>
class CSceneNodeModel : public ohm::flip::Object {

public:
   
   static void declare ();

   CSceneNodeModel(ohm::flip::DocumentBase & document);
   virtual ~CSceneNodeModel();
   
   virtual void update_client_data_cache();
   
   /// Position
   void setPosition(irr::core::vector3d<T> position);
   irr::core::vector3d<T> getPosition() const;

   /// Rotation
   void setRotation(irr::core::vector3d<T> rotation);
   irr::core::vector3d<T> getRotation() const;

   /// Rotation
   void setScale(irr::core::vector3d<T> scale);
   irr::core::vector3d<T> getScale() const;

private:

	irr::scene::ISceneNode *node;

	//-----------------------------------
    //SCENE NODE PROPERTIES
	ohm::flip::fvector3d<T> _position;
	ohm::flip::fvector3d<T> _rotation;
	ohm::flip::fvector3d<T> _scale;

	ohm::flip::Bool _visible;
	//-----------------------------------

	ohm::flip::TxSessionGuard _tx_session_guard;

///FORBIDDEN MEMBER FUNCTIONS BECAUSE OF FLIP
private:
   CSceneNodeModel();
   CSceneNodeModel(const CSceneNodeModel &other);
   CSceneNodeModel &operator = (const CSceneNodeModel &other);
   bool operator == (const CSceneNodeModel &other);
   bool operator != (const CSceneNodeModel &other);

};

#endif
