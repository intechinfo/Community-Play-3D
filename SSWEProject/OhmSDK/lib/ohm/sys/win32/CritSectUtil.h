/*****************************************************************************

        CritSectUtil.h
        Copyright (c) 2010 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
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



#if ! defined (ohm_sys_CritSectUtil_HEADER_INCLUDED)
#define  ohm_sys_CritSectUtil_HEADER_INCLUDED
#pragma once

#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#define  WIN32_LEAN_AND_MEAN
#define  NOMINMAX
#include <windows.h>



namespace ohm
{
namespace sys
{



class CritSectUtil
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   static const char *
                  init (::CRITICAL_SECTION &crit_sec);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  CritSectUtil ();
                  CritSectUtil (const CritSectUtil &other);
   virtual        ~CritSectUtil () {}
   CritSectUtil & operator = (const CritSectUtil &other);
   bool           operator == (const CritSectUtil &other) const;
   bool           operator != (const CritSectUtil &other) const;

}; // class CritSectUtil



}  // namespace sys
}  // namespace ohm



//#include  "ohm/sys/win32/CritSectUtil.hpp"



#endif   // ohm_sys_CritSectUtil_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
