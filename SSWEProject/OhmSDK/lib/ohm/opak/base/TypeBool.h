/*****************************************************************************

        TypeBool.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45161

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



#if ! defined (ohm_opak_TypeBool_HEADER_INCLUDED)
#define  ohm_opak_TypeBool_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/TypeKeyframable.h"

#include <list>



namespace ohm
{
namespace opak
{



template <class Target>
class TypeBool
:  public TypeKeyframable <Target, bool, bool, bool>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef void (Target::*Proc)();
   
                  TypeBool (Target & target, Proc proc);
                  TypeBool (Target & target, Proc proc, Proc prev_proc);
   virtual        ~TypeBool ();

   TypeBool &     operator = (const TypeBool &other);
   TypeBool &     operator = (bool value);
                  operator bool () const;

   TypeBool &     push (bool value, double transition_time);
   TypeBool &     loop ();
   TypeBool &     stop ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   // TypeKeyframable
   virtual void   do_convert (bool & dst, bool src);
   virtual void   do_interpolate (bool & dst, bool start_val, bool target_val, float interp_val);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TypeBool ();
                  TypeBool (const TypeBool &other);
   bool           operator == (const TypeBool &other);
   bool           operator != (const TypeBool &other);

}; // class TypeBool



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opak/base/TypeBool.hpp"



#endif   // ohm_opak_TypeBool_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
