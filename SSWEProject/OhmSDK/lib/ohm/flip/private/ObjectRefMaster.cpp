/*****************************************************************************

        ObjectRefMaster.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45327

        This file is part of Ohm Force Libraries.

        This program is free software; you can redistribute it and/or
        modify it under the terms of the GNU General Public License
        version 2 as published by the Free Software Foundation;

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program;
        if not, write to the Free Software Foundation, Inc.,
        59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*Tab=3***********************************************************************/



#if defined (_MSC_VER)
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/ObjectRefMaster.h"
#include "ohm/flip/private/BinaryKey.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*
==============================================================================
Note :
   Pure virtual classes are not supported in the flip C++ translation model.
   This is so because ClassDescription template will create the function
   'create' which inherently allocate an 'ObjectRefMaster' object.
   We cannot therefore detect pure virtual concepts at compilation time,
   but only at runtime.
==============================================================================
*/


/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ObjectRefMaster::ObjectRefMaster (DocumentBase & document)
:  BasicType (document)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ObjectRefMaster::~ObjectRefMaster ()
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

void  ObjectRefMaster::ctor (ohm::flip::Args & args)
{
   // see note
   assert (false);
}



/*
==============================================================================
Name : init
==============================================================================
*/

/*void   ObjectRefMaster::init ()
{
   // this is needed because the init call chain must not
   // fallback on Object::init () which was called just before
   // this call
}*/



/*
==============================================================================
Name : restore
==============================================================================
*/

/*void   ObjectRefMaster::restore ()
{
   // this is needed because the init call chain must not
   // fallback on Object::init () which was called just before
   // this call
}*/



/*
==============================================================================
Name : get
==============================================================================
*/

Ref   ObjectRefMaster::get () const
{
   // see note
   assert (false);
   
   return 0;
}



/*
==============================================================================
Name : set
==============================================================================
*/

void  ObjectRefMaster::set (Ref ref)
{
   // see note
   assert (false);
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

void  ObjectRefMaster::ctor (Ref val)
{
   // see note
   assert (false);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : set_ref
==============================================================================
*/

void  ObjectRefMaster::set_ref (Ref ref)
{
   // see note
   assert (false);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
