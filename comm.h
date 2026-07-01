// -*- c++ -*- $Id: comm.h 18 2006-06-21 15:45:56Z matt $

/* Communication-Specific classes
 * 
 * Copyright (C) 2006 Christian Unger <coder at breakbe D0T at>
 * Translated by John Frushour
 * 
 * This library is covered by the LGPL, read LICENSE for details.
 */


/** Communication-class that provides access to an aquaero-instance
 */
class AquaeroCommunication
{
public:
  virtual void read  ( AquaeroData_rev4 &aedata ) = 0;
  virtual void write ( AquaeroData_rev4 &aedata ) = 0;
};


// namespace usb_impl
// {
  
#include "usbio.h"
  
// };

// using usb_impl::USBCommunication;




