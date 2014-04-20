/*****************************************************************************

        MenuStdId.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 53853

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



#if ! defined (ohm_opal_MenuStdId_HEADER_INCLUDED)
#define  ohm_opal_MenuStdId_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace opal
{



enum MenuStdId
{
               MenuStdId_NONE = 0,
               MenuStdId_UNDO,
               MenuStdId_REDO,
               MenuStdId_COPY,
               MenuStdId_CUT,
               MenuStdId_PASTE,
               MenuStdId_SELECT_ALL,
};

   

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_MenuStdId_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
