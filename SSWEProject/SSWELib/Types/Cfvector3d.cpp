//
//  Cfvector3d.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/04/14.
//
//

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

#include "Cfvector3d.h"
#include "ohm/flip/ClassDescription.h"
#include "ohm/flip/ClassDescManager.h"
#include "ohm/flip/private/BasicTypesName.h"
#include "ohm/flip/private/MachineCode.h"
#include "ohm/flip/private/DocumentBase.h"

#include "ohm/util/trace.h"

#include <cassert>

namespace ohm
{
namespace flip
{

template<class T>
void fvector3d<T>::declare()
{
   ClassDescription <fvector3d>::use().set_name("cp3d.fvector3d<T>");
   
   ClassDescManager::declare (ClassDescription<fvector3d>::use());
}

template<class T>
fvector3d<T>::fvector3d<T>(DocumentBase & document) : BasicType (document),
														   _val(irr::core::vector3d<T>(0)),
														   _old_val(irr::core::vector3d<T>(0))
{

}

template<class T>
fvector3d<T>::~fvector3d<T>()
{
   // destruction (undefine) is handled in base class Object
   
	assert (_val == irr::core::vector3d<T>(0));
}

template<class T>
void fvector3d<T>::ctor (ohm::flip::Args & args)
{
   if (!args.empty ())
   {
      args.pop (*this);
   }
}

template<class T>
void fvector3d<T>::ctor (irr::core::vector3d<T> val)
{
	assertValuesAreFinite(val);
   
	make_transaction_and_set (val);
}

template<class T>
fvector3d<T> &fvector3d<T>::operator = (irr::core::vector3d<T> val)
{
   assertValuesAreFinite(val);
   
   make_transaction_and_set (val);
   
   return *this;
}

template<class T>
fvector3d<T> &fvector3d<T>::operator += (irr::core::vector3d<T> val)
{
   assertValuesAreFinite(val);
   
   make_transaction_and_set (_val + val);
   
   return *this;
}

template<class T>
fvector3d<T> &fvector3d<T>::operator -= (irr::core::vector3d<T> val)
{
   assertValuesAreFinite(val);
   
   make_transaction_and_set (_val - val);
   
   return *this;
}

template<class T>
fvector3d<T> &fvector3d<T>::operator *= (irr::core::vector3d<T> val)
{
   assertValuesAreFinite(val);
   
   make_transaction_and_set (_val * val);
   
   return *this;
}

template<class T>
fvector3d<T> &fvector3d<T>::operator /= (irr::core::vector3d<T> val)
{
   //assert (val != 0.0); // Already done by core::vector3d<class T> class
   assertValuesAreFinite(val);
   
   make_transaction_and_set (_val / val);
   
   return *this;
}

template<class T>
fvector3d<T>::operator irr::core::vector3d<T> () const
{
   return _val;
}

template<class T>
bool  fvector3d<T>::operator == (irr::core::vector3d<T> val) const
{
   return (_val == val);
}

template<class T>
bool  fvector3d<T>::operator != (irr::core::vector3d<T> val) const
{
   return (_val != val);
}

template<class T>
bool  fvector3d<T>::operator == (const fvector3d<T> &other) const
{
   return operator == (irr::core::vector3d<T> (other));
}

template<class T>
bool  fvector3d<T>::operator != (const fvector3d<T> &other) const
{
   return operator != (irr::core::vector3d<T> (other));
}

template<class T>
bool  fvector3d<T>::did_value_change () const
{
   return _val != _old_val;
}

template<class T>
irr::core::vector3d<T> fvector3d<T>::get_old_value () const
{
   return _old_val;
}

template<class T>
void  fvector3d<T>::backup ()
{
	make_transaction_and_set(irr::core::vector3d<T>((T)0));
}

template<class T>
void  fvector3d<T>::synchronize ()
{
   _old_val = _val;
   
   Object::synchronize();
}

template<class T>
void  fvector3d<T>::check_synchronized () const
{
   assert(_old_val == _val);
   
   Object::check_synchronized();
}

template<class T>
void  fvector3d<T>::check_inner_invalidation_cnt (size_t invalid_cnt) const
{
   if (_old_val != _val)
   {
      ohm_util_TRACE_DEBUG ("  fvector3d");
      assert (invalid_cnt > 0);
      --invalid_cnt;
   }
   
   Object::check_inner_invalidation_cnt(invalid_cnt);
}

template<class T>
void  fvector3d<T>::trace ()
{
   Object::trace();
   
   ohm_util_TRACE_INFO2 (
      "fvector3d %1% -> %2%",
      _old_val.X,
      _val.X
   );
}

template<class T>
void  fvector3d<T>::set (irr::core::vector3d<T> val)
{
   assert (_val != val);
   
   if (_old_val == _val)
   {
      _val = val;
      
      invalidate ();
   }
   else if (_old_val == val)
   {
      _val = val;
      
      validate ();
   }
   else
   {
      _val = val;
   }
}

template<class T>
void  fvector3d<T>::make_transaction_and_set (irr::core::vector3d<T> val)
{
   if (_val == val)
   {
      return;
   }
   
   Data & transaction = use_document ().use_transaction ();

   Ref this_ref = get_ref ();
   assert (this_ref != 0);
   
   MachineCode::build_fvector3d<T>_set (transaction, this_ref, _val, val);
   
   set (val);
}

template<class T>
void fvector3d<T>::assertValuesAreFinite(irr::core::vector3d<T> vec) {
	assert(math::is_finite (val.X));
	assert(math::is_finite (val.Y));
	assert(math::is_finite (val.Z));
}

}  // namespace flip
}  // namespace ohm
