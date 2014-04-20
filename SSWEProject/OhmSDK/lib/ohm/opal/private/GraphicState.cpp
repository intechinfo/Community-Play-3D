/*****************************************************************************

        GraphicState.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 39877

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

#include "ohm/opal/private/GraphicState.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
   initialize the default graphic state with predefined values.
==============================================================================
*/

GraphicState::GraphicState ()
:  _alpha (1.f)
,  _stroke_color (1.f, 0.f, 0.f, 0.f)
,  _fill_color (1.f, 0.f, 0.f, 0.f)
,  _clear_color (0.f, 0.f, 0.f, 0.f)
,  _blend_mode (BlendMode_NORMAL)

,  _local_clip_rect ()
,  _global_clip_rect ()
,  _total_translation (0.f, 0.f)

,  _fragment_program_proc (0)
/*,   _fragment_var_arr ()*/
{
   for (size_t i = 0 ; i < MAX_FRAGMENT_VARS ; ++i)
   {
      _fragment_var_arr [i] = 0.f;
   }
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

GraphicState::~GraphicState ()
{
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
