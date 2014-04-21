/*****************************************************************************

        AspectWidgetComboBox.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59357

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



#if ! defined (ohm_opa_AspectWidgetComboBox_HEADER_INCLUDED)
#define  ohm_opa_AspectWidgetComboBox_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Bitmap.h"
#include "ohm/opal/Color.h"
#include "ohm/opal/SprungImage.h"

#include "ohm/opak/base/AspectKeyedBase.h"

#include <vector>



namespace ohm
{
namespace opa
{



class AspectWidgetComboBox
:  public opak::AspectBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum State
   {
                  State_OFF = 0,
                  State_ON,
                  State_OVER,
                  
                  State_NBR_ELT
   };
   
                  AspectWidgetComboBox ();
   virtual        ~AspectWidgetComboBox ();
   
   // opak::AspectBase
   virtual void   make (opak::GraphicContext & context);
   
   opal::SprungImage
                  _button_arr [State_NBR_ELT];



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // opak::AspectBase
   virtual void   do_init (opak::GraphicContext & context);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   class BaseElements
   {
   public:
      opal::Bitmap
                  _base_arr [State_NBR_ELT];
      opal::Bitmap
                  _arrow;
   };
   
   static void    load_base_elements ();
   
   opak::AspectKeyedBase::Key
                  _aspect_widget_key;
   
   static BaseElements
                  _be;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  AspectWidgetComboBox (const AspectWidgetComboBox &other);
   AspectWidgetComboBox &        operator = (const AspectWidgetComboBox &other);
   bool           operator == (const AspectWidgetComboBox &other);
   bool           operator != (const AspectWidgetComboBox &other);

}; // class AspectWidgetComboBox



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opa/private/AspectWidgetComboBox.hpp"



#endif   // ohm_opa_AspectWidgetComboBox_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
