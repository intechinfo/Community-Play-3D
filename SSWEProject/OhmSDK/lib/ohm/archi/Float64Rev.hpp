/*****************************************************************************

        Float64Rev.hpp
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



#if defined (ohm_archi_Float64Rev_CURRENT_CODEHEADER)
   #error Recursive inclusion of Float64Rev code header.
#endif
#define  ohm_archi_Float64Rev_CURRENT_CODEHEADER

#if ! defined (ohm_archi_Float64Rev_CODEHEADER_INCLUDED)
#define  ohm_archi_Float64Rev_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/fnc.h"



namespace ohm
{
namespace archi
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*==========================================================================*/
/*      Name: ctor                                                          */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

Float64Rev::Float64Rev ()
{
}



Float64Rev::Float64Rev (const Float64Rev &other)
:  _rev_val (*reinterpret_cast <const Int64 *> (&other._rev_val))
{
}



Float64Rev::Float64Rev (Float64 val)
:  _rev_val (revert_byte_order_s64 (* (Int64 *) (&val)))
{
}



/*==========================================================================*/
/*      Name: operator =                                                    */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

Float64Rev &   Float64Rev::operator = (const Float64Rev &other)
{
   _rev_val = other._rev_val;

   return (*this);
}



/*==========================================================================*/
/*      Name: operator Float64                                              */
/*      Description: Get the value                                          */
/*      Returns: The right value, in native byte order.                     */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

Float64Rev::operator Float64 () const
{
   const Int64    temp = revert_byte_order_s64 (_rev_val);

   return (*reinterpret_cast <const Float64 *> (&temp));
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace archi
}  // namespace ohm



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_archi_Float64Rev_CODEHEADER_INCLUDED

#undef ohm_archi_Float64Rev_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
