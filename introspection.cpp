// $Id: introspection.cpp 33 2006-12-17 12:38:33Z matt $

/* Definition of some Introspection-Functions
 * 
 * Copyright (C) 2006 Christian Unger <coder at breakbe D0T at>
 * Translated by John Frushour
 * 
 * This library is covered by the LGPL, read LICENSE for details.
 */



template <>
string PropertyDescriptor<AquaeroData_rev4:: ProfilRegister>::getEnumName ( ) const throw(const char*)
{
  return "ProfilRegister";
}
template <>
string PropertyDescriptor<AquaeroData_rev4:: LCDSettingsRegister>::getEnumName ( ) const throw(const char*)
{
  return "LCDSettingsRegister";
}
template <>
string PropertyDescriptor<AquaeroData_rev4:: LCDPageRegister>::getEnumName ( ) const throw(const char*)
{
  return "LCDPageRegister";
}
template <>
string PropertyDescriptor<AquaeroData_rev4:: Powersensor_ConfigRegister>::getEnumName ( ) const throw(const char*)
{
  return "Powersensor_ConfigRegister";
}
template <>
string PropertyDescriptor<AquaeroData_rev4:: tubemeter_ConfigRegister>::getEnumName ( ) const throw(const char*)
{
  return "tubemeter_ConfigRegister";
}
template <>
string PropertyDescriptor<AquaeroData_rev4:: Fan_ModeRegister>::getEnumName ( ) const throw(const char*)
{
  // TODO: this is way too dirty...
  if( endsWith( getName(), "mode" ) )
    return "Friendly_Fan_ModeRegister";
  
  return "Fan_ModeRegister";
}
template <>
string PropertyDescriptor<AquaeroData_rev4:: FlowSensorModeRegister>::getEnumName ( ) const throw(const char*)
{
  return "FlowSensorModeRegister";
}
template <>
string PropertyDescriptor<AquaeroData_rev4:: LED_ModeRegister>::getEnumName ( ) const throw(const char*)
{
  return "LED_ModeRegister";
}
template <>
string PropertyDescriptor<AquaeroData_rev4:: Alarm_SetupRegister>::getEnumName ( ) const throw(const char*)
{
  return "Alarm_SetupRegister";
}
template <>
string PropertyDescriptor<AquaeroData_rev4:: DayRegister>::getEnumName ( ) const throw(const char*)
{
  return "DayRegister";
}
template <>
string PropertyDescriptor<AquaeroData_rev4:: aquastream_CommandRegister>::getEnumName ( ) const throw(const char*)
{
  return "aquastream_CommandRegister";
}
template <>
string PropertyDescriptor<AquaeroData_rev4:: Relay_StatusRegister>::getEnumName ( ) const throw(const char*)
{
  return "Relay_StatusRegister";
}
template <>
string PropertyDescriptor<AquaeroData_rev4:: multiswitch_BargraphRegister>::getEnumName ( ) const throw(const char*)
{
  return "multiswitch_BargraphRegister";
}
template <>
string PropertyDescriptor<AquaeroData_rev4:: multiswitch_StatusRegister>::getEnumName ( ) const throw(const char*)
{
  return "multiswitch_StatusRegister";
}
template <>
string PropertyDescriptor<AquaeroData_rev4:: aquaero_ControlRegister>::getEnumName ( ) const throw(const char*)
{
  return "aquaero_ControlRegister";
}
template <>
string PropertyDescriptor<AquaeroData_rev4:: aquaero_ScreenRegister>::getEnumName ( ) const throw(const char*)
{
  return "aquaero_ScreenRegister";
}
template <>
string PropertyDescriptor<AquaeroData_rev4:: aquabus_StatusRegister>::getEnumName ( ) const throw(const char*)
{
  return "aquabus_StatusRegister";
}
template <>
string PropertyDescriptor<AquaeroData_rev4:: aquabus_DeviceRegister>::getEnumName ( ) const throw(const char*)
{
  return "aquabus_DeviceRegister";
}
template <>
string PropertyDescriptor<AquaeroData_rev4:: USBLCD_ModeRegister>::getEnumName ( ) const throw(const char*)
{
  return "USBLCD_ModeRegister";
}
template <>
string PropertyDescriptor<AquaeroData_rev4:: LCD_TypeRegister>::getEnumName ( ) const throw(const char*)
{
  return "LCD_TypeRegister";
}





/** Holds all PropertyDescriptors that belong to AquaeroData_rev4
 */
map<string,PropertyDescriptorBase*> properties;


/** Helper function for defining PropertyDescriptors
 */
template <class T>
PropertyDescriptor<T>* make_PD ( T *a, string b, string c, ValueParser<T> *parser ) 
{
  PropertyDescriptor<T>* ret = new PropertyDescriptor<T>(a,b,c,parser);
  properties[b] = ret;
  return ret;
}

/** Helper function for defining PropertyDescriptors
 */
template <class T>
PropertyDescriptor<T>* make_PD ( T *a, string b, string c, AutomaticParser* automatic ) 
{
  // this looks strange - but fast to code
  
  ValueParser<T> *parser;
  
  if( automatic->ro ) {
    parser = new ReadOnlyParser<T>();
  }else{
    /*if( automatic->isEnum ) {
      parser = NULL; // will be instanciated at runtime...
    }else{
      parser = new ValueParser<T>();
    }*/
    
    parser = NULL; // will be instanciated at runtime, with respect to enums
  }
  delete automatic;
  
  PropertyDescriptor<T>* ret = new PropertyDescriptor<T>(a,b,c,parser);
  properties[b] = ret;
  return ret;
}

PropertyDescriptor<Byte>* make_FanToSensorPD ( Byte *a, bool upper, string name, string desc )
{
  FanToSensorPropertyDescriptor *ret =
    new FanToSensorPropertyDescriptor(a,upper,name,desc);
  properties[name] = ret;
  return ret;
}

PropertyDescriptor<Byte>* make_LEDToSensorPD ( Byte *a, bool upper, string name, string desc )
{
  LEDToSensorPropertyDescriptor *ret =
    new LEDToSensorPropertyDescriptor(a,upper,name,desc);
  properties[name] = ret;
  return ret;
}



#define _MK_PD(aaa,bbb,ccc,ddd) make_PD( &ae. aaa, string(bbb), string(ccc), ddd );
#define _MK_PD_CAST(ttt,aaa,bbb,ccc,ddd) make_PD( (ttt*)&ae. aaa, string(bbb), string(ccc), (ValueParser<ttt>*)ddd );
#define _MK_F2S_PD(aaa,upper,bbb,ccc) make_FanToSensorPD( &ae. aaa, upper, string(bbb), string(ccc) );
#define _MK_L2S_PD(aaa,upper,bbb,ccc) make_LEDToSensorPD( &ae. aaa, upper, string(bbb), string(ccc) );
/** Defines all properties of `AquaeroData_rev4'. Needed for
 *  introspection-like-stuff.
 */
void definePropertyMap ( AquaeroData_rev4& ae )
{
#include "definePropMap.h"
}



template <typename T>
float float_property_value ( string prop ) 
{
  PropertyDescriptor<T> * pd = (PropertyDescriptor<T>*)properties[prop];
  T v = pd->getValue();
  return (float)v;
}





// template<typename T>
// string typenameToString () 
// {
//   return "<UNKNOWN>";
// }
// template<>
// string typenameToString<Byte> () 
// {
//   return "number";
// }
// template<>
// string typenameToString<string> () 
// {
//   return "text";
// }
// template<>
// string typenameToString<int> () 
// {
//   return "number";
// }
// template<>
// string typenameToString<long> () 
// {
//   return "number";
// }



