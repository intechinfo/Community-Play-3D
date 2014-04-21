// stdafx.cpp : fichier source incluant simplement les fichiers Include standard
// SSWELib.pch représente l'en-tête précompilé
// stdafx.obj contient les informations de type précompilées

#include "stdafx.h"

#if defined (_MSC_VER)
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif

#include "CSceneNodeModel.h"

#include "ohm/flip/ClassDescription.h"

template<class T>
void CSceneNodeModel<T>::declare()
{
   using namespace ohm::flip;
   
   ClassDescription <CSceneNodeModel>::use().set_name ("cp3d.CSceneNodeModel");
   ClassDescription<CSceneNodeModel>::use().push_var_desc(&CSceneNodeModel::_position, "position");
   ClassDescription<CSceneNodeModel>::use().push_var_desc(&CSceneNodeModel::_rotation, "rotation");
   ClassDescription<CSceneNodeModel>::use().push_var_desc(&CSceneNodeModel::_scale, "scale");
   
   ClassDescManager::declare (ClassDescription <CSceneNodeModel>::use());
}

template<class T>
CSceneNodeModel<T>::CSceneNodeModel(ohm::flip::DocumentBase &document) : ohm::flip::Object (document),

																	  _position(document),
																	  _rotation(document),
																	  _scale(document),

																	  _visible(document),

																	  _tx_session_guard (document)
{

}

template<class T>
CSceneNodeModel<T>::~CSceneNodeModel () { }

template<class T>
void  CSceneNodeModel<T>::update_client_data_cache() { }

/// Position
template<class T>
irr::core::vector3d<T> CSceneNodeModel<T>::getPosition() const { return _position; }
template<class T>
void CSceneNodeModel<T>::setPosition(irr::core::vector3d<T> position) { _position = position; }

/// Rotation
template<class T>
irr::core::vector3d<T> CSceneNodeModel<T>::getRotation() const { return _rotation; }
template<class T>
void CSceneNodeModel<T>::setRotation(irr::core::vector3d<T> rotation) { _rotation = rotation; }

/// Scale
template<class T>
irr::core::vector3d<T> CSceneNodeModel<T>::getScale() const { return _scale; }
template<class T>
void CSceneNodeModel<T>::setScale(irr::core::vector3d<T> scale) { _scale = scale; }
