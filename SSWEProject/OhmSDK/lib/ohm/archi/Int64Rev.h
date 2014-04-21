/*****************************************************************************

        Int64Rev.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 5142

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



#if defined (ohm_archi_Int64Rev_CURRENT_HEADER)
   #error Recursive inclusion of ohm/archi/Int64Rev header.
#endif
#define  ohm_archi_Int64Rev_CURRENT_HEADER

#if ! defined (ohm_archi_Int64Rev_HEADER_INCLUDED)
#define  ohm_archi_Int64Rev_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"



namespace ohm
{
namespace archi
{



class Int64Rev
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   inline         Int64Rev ();
   inline         Int64Rev (const Int64Rev &other);
   inline         Int64Rev (Int64 val);

   inline Int64Rev &
                  operator = (const Int64Rev &other);

   inline         operator Int64 () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   Int64          _rev_val;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



}; // class Int64Rev

}  // namespace archi
}  // namespace ohm



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/Int64Rev.hpp"



#endif   // ohm_archi_Int64Rev_HEADER_INCLUDED

#undef ohm_archi_Int64Rev_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
