/*****************************************************************************

        WidgetOrientableBase.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 53326

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



#if ! defined (ohm_opa_WidgetOrientableBase_HEADER_INCLUDED)
#define  ohm_opa_WidgetOrientableBase_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/base/Orientation.h"



namespace ohm
{
namespace opa
{



class WidgetOrientableBase
:  public virtual WidgetBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  WidgetOrientableBase ();
   virtual        ~WidgetOrientableBase ();
   
   void           set_orientation (Orientation orientation);
   Orientation    get_orientation () const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   Orientation    _orientation;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetOrientableBase (const WidgetOrientableBase &other);
   WidgetOrientableBase &
                  operator = (const WidgetOrientableBase &other);
   bool           operator == (const WidgetOrientableBase &other);
   bool           operator != (const WidgetOrientableBase &other);

}; // class WidgetOrientableBase



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

//#include "ohm/opa/base/WidgetOrientableBase.hpp"



#endif   // ohm_opa_WidgetOrientableBase_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
