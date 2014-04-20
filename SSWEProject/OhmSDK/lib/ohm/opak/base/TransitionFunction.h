/*****************************************************************************

        TransitionFunction.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 52324

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



#if ! defined (ohm_opak_TransitionFunction_HEADER_INCLUDED)
#define  ohm_opak_TransitionFunction_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace opak
{



typedef float  (*TransitionFunction) (float t);

float step (float t);
float inverse_step (float t);
float linear (float t);
float ease_in (float t);
float ease_out (float t);
float ease_in_ease_out (float t);
float elastic (float t);



}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opak/base/TransitionFunction.hpp"



#endif   // ohm_opak_TransitionFunction_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
