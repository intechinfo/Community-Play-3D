/*****************************************************************************

        TxSessionGuard.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 64197

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

#include "ohm/flip/TxSessionGuard.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

TxSessionGuard::AutoWield::AutoWield (TxSessionGuard & tx_session_guard)
:  _tx_session_guard (tx_session_guard)
{
   assert (false);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

TxSessionGuard::AutoWield::~AutoWield ()
{
   assert (false);
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

TxSessionGuard::TxSessionGuard (DocumentBase & document)
:  _document_ptr (0)

,  _started_flag (false)
,  _transaction_flag (false)
{
   assert (&document != 0);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

TxSessionGuard::~TxSessionGuard ()
{
}



/*
==============================================================================
Name : prepare_record
==============================================================================
*/

void  TxSessionGuard::prepare_record (Scribe & scribe)
{
   assert (false);
}



/*
==============================================================================
Name : can_start
==============================================================================
*/

bool  TxSessionGuard::can_start () const
{
   assert (false);
   
   return false;
}



/*
==============================================================================
Name : start
==============================================================================
*/

bool  TxSessionGuard::start ()
{
   assert (false);
   
   return false;
}



/*
==============================================================================
Name : is_started
==============================================================================
*/

bool  TxSessionGuard::is_started () const
{
   assert (false);
   
   return false;
}



/*
==============================================================================
Name : commit
==============================================================================
*/

void  TxSessionGuard::commit ()
{
   assert (false);
}



/*
==============================================================================
Name : revert
==============================================================================
*/

void  TxSessionGuard::revert ()
{
   assert (false);
}



/*
==============================================================================
Name : wield
==============================================================================
*/

void  TxSessionGuard::wield ()
{
   assert (false);
}



/*
==============================================================================
Name : unwield
==============================================================================
*/

void  TxSessionGuard::unwield ()
{
   assert (false);
}



/*
==============================================================================
Name : can_join
==============================================================================
*/

bool  TxSessionGuard::can_join ()
{
   assert (false);
   
   return false;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
