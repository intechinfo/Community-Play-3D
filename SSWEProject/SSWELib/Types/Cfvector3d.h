//
//  Cfvector3d.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/04/14.
//
//

/// ENCAPSULATE vector3d<class T> Object for OHM SDK

#ifndef __C_VECTOR_3D_TYPE_H_INCLUDED__
#define __C_VECTOR_3D_TYPE_H_INCLUDED__

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif

#include <irrlicht.h>
#include "ohm/flip/private/BasicType.h"

namespace ohm
{
namespace flip // Set the data into the flip namespace to tell it's relative to flip integration
{

class DocumentBase;

template <class T>
class fvector3d : public BasicType
{
	static void declare ();

	fvector3d(DocumentBase & document);
	virtual ~fvector3d();

	//---------------------------------------------------------------------------------------------
	//-----------------------------------OHM::FLIP::OBJECT-----------------------------------------
	//---------------------------------------------------------------------------------------------
	virtual void   ctor (ohm::flip::Args & args);
	void           ctor (irr::core::vector3d<T> val);

	fvector3d &      operator =  (irr::core::vector3d<T> val);
	fvector3d &      operator += (irr::core::vector3d<T> val);
	fvector3d &      operator -= (irr::core::vector3d<T> val);
	fvector3d &      operator *= (irr::core::vector3d<T> val);
	fvector3d &      operator /= (irr::core::vector3d<T> val);

	operator irr::core::vector3d<T>() const;
   
	bool           operator == (irr::core::vector3d<T> val) const;
	bool           operator != (irr::core::vector3d<T> val) const;

	bool           operator == (const fvector3d &other) const;
	bool           operator != (const fvector3d &other) const;

	bool				   did_value_change () const;
	irr::core::vector3d<T> get_old_value () const;
   
	virtual void   backup ();
	virtual void   synchronize ();
	virtual void   check_synchronized () const;
	virtual void   check_inner_invalidation_cnt (size_t invalid_cnt) const;
   
	virtual void   trace ();

private:

	friend class VirtualMachine;

	void           set (irr::core::vector3d<T> val);
	void           make_transaction_and_set (irr::core::vector3d<T> val);

	irr::core::vector3d<T> _val;
	irr::core::vector3d<T> _old_val;

	void assertValuesAreFinite(irr::core::vector3d<T> vec);

///FORBIDDEN MEMBER FUNCTIONS BECAUSE OF FLIP
private:
   fvector3d ();
   fvector3d (const fvector3d &other);
   fvector3d &operator = (const fvector3d &other);
};

typedef fvector3d<irr::f32> fvector3df;
typedef fvector3d<irr::s32> fvector3di;

//! Function multiplying a scalar and a vector component-wise.
template<class S, class T>
fvector3d<T> operator*(const S scalar, const fvector3d<T>& vector) { return vector*scalar; }

}//END FLIP NAMESPACE
}//END OHM NAMESPACE

#endif
