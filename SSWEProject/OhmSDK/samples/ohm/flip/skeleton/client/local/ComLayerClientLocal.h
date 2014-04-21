/*****************************************************************************

        ComLayerClientLocal.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71330

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



#if ! defined (client_local_ComLayerClientLocal_HEADER_INCLUDED)
#define  client_local_ComLayerClientLocal_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/ComLayerClientBase.h"



namespace ohm
{
namespace util
{
class BinaryStreamInput;
}
}

namespace client
{
namespace local
{



class ClientLocal;

class ComLayerClientLocal
:  public ohm::flip::ComLayerClientBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  ComLayerClientLocal (const char * user_name_0);
   virtual        ~ComLayerClientLocal ();

   void           local_receive (const ohm::flip::DataMaster & data);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // ComLayerClientBase
   virtual void   do_init ();
   virtual void   do_restore ();
   virtual int    do_connect ();
   virtual void   do_disconnect ();
   virtual void   do_send (const ohm::flip::DataMaster & data);
   virtual void   do_notify_running ();
   virtual void   do_notify_exiting ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   const std::string
                  _user_name;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ComLayerClientLocal ();
                  ComLayerClientLocal (const ComLayerClientLocal &other);
   ComLayerClientLocal &
                  operator = (const ComLayerClientLocal &other);
   bool           operator == (const ComLayerClientLocal &other);
   bool           operator != (const ComLayerClientLocal &other);

}; // class ComLayerClientLocal



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace local
}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/local/ComLayerClientLocal.hpp"



#endif   // client_local_ComLayerClientLocal_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
