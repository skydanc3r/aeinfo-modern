// -*- c++ -*- $Id: stringEnum.h 35 2007-05-05 15:09:55Z matt $

/* Generated file: String-mappings of enumerated types
 * 
 * Copyright (C) 2006 Christian Unger <coder at breakbe D0T at>
 * Translated by John Frushour
 * 
 * This library is covered by the LGPL, read LICENSE for details.
 */

/* WARNING: This file has been generated (semi-)automatically!
 *          DO NOT EDIT IT UNLESS YOU KNOW WHAT YOU ARE DOING
 *
 * beware of the powers of the emacs gods ;-)
 */



#ifdef _INTROSPECTION_H

void defineStringEnums () 
{
  string theEnum;

#define _ADD_ENUM(aaa,bbb,ccc) stringEnum_names[theEnum][bbb] = aaa; stringEnum_help[theEnum][bbb] = ccc;
  theEnum = "Friendly_Fan_ModeRegister";
  {
    _ADD_ENUM("GET_MAX_RPM_NOW", Bit3, "Get Max RPM");
    _ADD_ENUM("IGNORE_RPM", Bit6, "Ignore RPM Signal");
    _ADD_ENUM("FORCE_MIN_POWER", Bit7, "Force Minimum Power");
    _ADD_ENUM("DISABLE_RPM_TEST", Bit8, "Disable speed tests when starting, RPM signal must be used" );
    _ADD_ENUM("MANUAL_POWER", Bit1, "Manual Power Setting");
    _ADD_ENUM("MANUAL_RPM", Bit2, "Manual RPM Setting");
    _ADD_ENUM("AUTO_LINEAR", Bit4, "Automatic setting, linear");
    _ADD_ENUM("AUTO_PROG", Bit5, "Automatic setting, progressive");
    _ADD_ENUM("AUTO_TARGET", Bit9, "Automatic setting, target ");
  }
  bitFields.insert(theEnum);


theEnum ="ProfilRegister";
{
  
  _ADD_ENUM("Profil_A", 0,"");
  _ADD_ENUM("Profil_B", 1,"");
};

theEnum ="LCDSettingsRegister";
{
  
  _ADD_ENUM("BacklightOn", Bit0,"Backlight always On");
  _ADD_ENUM("CompleteOff", Bit2,"LCD Complete Off");
  _ADD_ENUM("USBLCDBoot", Bit3,"Aquaero starts in USB-LCD-Mode");
}; bitFields.insert(theEnum);

theEnum ="LCDPageRegister";
{
  
  //Sewiten mit gesetzten Flags werden angezeigt
  _ADD_ENUM("TempOverview", Bit0,"Sensor Overview");
  _ADD_ENUM("TempSensor_1_2", Bit1,"Temperatures 1 & 2");
  _ADD_ENUM("TempSensor_3_4", Bit2,"Temperatures 3 & 4");
  _ADD_ENUM("TempSensor_5_6", Bit3,"Temperatures 5 & 6");
  _ADD_ENUM("LCDFan_1", Bit4,"Fan 1 Details");
  _ADD_ENUM("LCDFan_2", Bit5,"Fan 2 Details");
  _ADD_ENUM("LCDFan_3", Bit6,"Fan 3 Details");
  _ADD_ENUM("LCDFan_4", Bit7,"Fan 4 Details");
  _ADD_ENUM("Aquastream_1_A", Bit8,"Data Page A Aquastream 1");
  _ADD_ENUM("Aquastream_1_B", Bit9,"Data Page B Aquastream 1");
  _ADD_ENUM("Aquastream_2_A", Bit10,"Data Page A Aquastream 2");
  _ADD_ENUM("Aquastream_2_B", Bit11,"Data Page B Aquastream 2");
  _ADD_ENUM("FlowSensor_1", Bit12,"Flowsensor 1");
  _ADD_ENUM("FlowSensor_2", Bit13,"Flowsensor 2");
  _ADD_ENUM("LCDtubemeter", Bit14,"Tubemeter Sensor");
  _ADD_ENUM("aquaero", Bit15,"Aquaero Homepage");
  _ADD_ENUM("Powersensor_1", Bit16,"Powersensor 1");
  _ADD_ENUM("Powersensor_2", Bit17,"Powersensor 2");
  _ADD_ENUM("Fan_Voltage_1_2", Bit18,"Fan Voltages, Fan 1 and 2");
  _ADD_ENUM("Fan_Voltage_3_4", Bit19,"Fan Voltages, Fan 1 and 2");
  _ADD_ENUM("Fan_Overview", Bit20,"Fan overview fans 1-4");
  _ADD_ENUM("aquaero_info1", Bit21,"Information Hardware/Firmware Page 1");
  _ADD_ENUM("aquaero_info2", Bit22,"Information Hardware/Firmware Page 2");
}; bitFields.insert(theEnum);

theEnum ="Powersensor_ConfigRegister";
{
  
  _ADD_ENUM("Powersensor_1_Uses_Flow_2", Bit0,"Powersensor 1, use Flowsensor 2");
  _ADD_ENUM("Powersensor_2_Uses_Flow_2", Bit1,"Powersensor 2, use Flowsensor 2");
}; bitFields.insert(theEnum);

theEnum ="tubemeter_ConfigRegister";
{
  
  _ADD_ENUM("UNUSED", 0,"Tubemeter is not installed.");
  _ADD_ENUM("level_mm", 2,"Water level in mm");
  _ADD_ENUM("LED_ON", 4,"LED on/off");
};

theEnum ="Fan_ModeRegister";
{
  
  _ADD_ENUM("PWR_RPM_Switch", Bit0,"Power or RPM Scheme");
  _ADD_ENUM("Manual_Auto_Switch", Bit1,"Manual or sensor driven scheme");
  _ADD_ENUM("Get_Max_RPM", Bit3,"Determine the maximum speed");
  _ADD_ENUM("Progressiv_Linear_Switch", Bit4,"Progressive or linear regulation");
  _ADD_ENUM("Automatik_PWM_RPM_Switch", Bit5,"Automatic power-regulation or RPM-Regulation");
  _ADD_ENUM("Ignore_RPM", Bit6,"Ignore the RPM signal");
  _ADD_ENUM("Force_MinimumPower", Bit7,"Minimum power or hold speed");
  _ADD_ENUM("Disable_RPM_Test", Bit8,"Deactivate maximum RPM test on startup, RPM signal will be deactivated");
}; bitFields.insert(theEnum);

theEnum ="FlowSensorModeRegister";
{
  _ADD_ENUM("l_minute_hour", Bit0,"Liter/Minute or Hour (second Flowsensor)");
  _ADD_ENUM("use_fan4_as_flowsensor", Bit1,"Use fan 4 as flowsensor (second Flowsensor)");
}; bitFields.insert(theEnum);

theEnum ="LED_ModeRegister";
{
  
  _ADD_ENUM("Manual_Auto_Switch", Bit0,"Manual or automatic operation");
  _ADD_ENUM("Alarm_Flash", Bit1,"Blink on Alarm");
  _ADD_ENUM("Hot_Cold", Bit2,"With rising temperature, LED becomes brighter");
}; bitFields.insert(theEnum);

theEnum ="Alarm_SetupRegister";
{
  
  _ADD_ENUM("Fan_1", Bit0,"Activate Alarm for Fan 1");
  _ADD_ENUM("Fan_2", Bit1,"Activate Alarm for Fan 2");
  _ADD_ENUM("Fan_3", Bit2,"Activate Alarm for Fan 3");
  _ADD_ENUM("Fan_4", Bit3,"Activate Alarm for Fan 4");
  _ADD_ENUM("Sensor", Bit4,"Alarm for all Temperature sensors");
  _ADD_ENUM("Flowsensor_1", Bit5,"Alarm for flowsensor");
  _ADD_ENUM("Flowsensor_2", Bit6,"Alarm for flowsensor");
  _ADD_ENUM("tubemeter_level", Bit7,"Tubemeter level alarm");
  _ADD_ENUM("tubemeter_wave", Bit8,"Tubemeter wave alarm");
  _ADD_ENUM("aquastream_1", Bit9,"Aquastream 1");
  _ADD_ENUM("aquastream_2", Bit10,"Aquastream 1");
  _ADD_ENUM("Enable", Bit15,"Activate all alarm functions.  If not Enabled, NO alarams are active.");
}; bitFields.insert(theEnum);

theEnum ="DayRegister";
{
  
  _ADD_ENUM("Sunday", 0,"Sunday");
  _ADD_ENUM("Monday", 1,"Monday");
  _ADD_ENUM("Tuesday", 2,"Tuesday");
  _ADD_ENUM("Wednesday", 3,"Wednesday");
  _ADD_ENUM("Thursday", 4,"Thursday");
  _ADD_ENUM("Friday", 5,"Friday");
  _ADD_ENUM("Saturday", 6,"Saturday");
  _ADD_ENUM("EveryDay", 7,"Every Day");
  _ADD_ENUM("MondayToFriday", 8,"Monday to Friday (only for the Timer)");
  _ADD_ENUM("Saturday_Sunday", 9,"Saturday and Sunday (only for the Timer)");
  _ADD_ENUM("Never", 10,"Never (only for the Timer)");
};

theEnum ="aquastream_CommandRegister";
{
  
  _ADD_ENUM("Set_Default", 41,"Set defaults");
  _ADD_ENUM("Anti_Air", 75,"De-aeration mode");
  _ADD_ENUM("Set_Frequency", 150,"Set frequency");
};

theEnum ="Relay_StatusRegister";
{
  
  _ADD_ENUM("Relay_On_Off", Bit0,"Relay on/off");
  _ADD_ENUM("Relay_Usage_ATXBREAK_PWR", Bit1,"Relay usage: ATX Break line or Power Button");
}; bitFields.insert(theEnum);

theEnum ="multiswitch_BargraphRegister";
{
  
  _ADD_ENUM("Fan_1_RPM", 0,"Bargraph for RPM, Fans 1-4");
  _ADD_ENUM("Fan_2_RPM", 1,"");
  _ADD_ENUM("Fan_3_RPM", 2,"");
  _ADD_ENUM("Fan_4_RPM", 3,"");
  _ADD_ENUM("Fan_1_PWR", 4,"Bargraph for Power, Fans 1-4");
  _ADD_ENUM("Fan_2_PWR", 5,"");
  _ADD_ENUM("Fan_3_PWR", 6,"");
  _ADD_ENUM("Fan_4_PWR", 7,"");
  _ADD_ENUM("Sensor_1", 8,"Bargraph for Min and Max Temperatures, Sensors 1-6");
  _ADD_ENUM("Sensor_2", 9,"");
  _ADD_ENUM("Sensor_3", 10,"");
  _ADD_ENUM("Sensor_4", 11,"");
  _ADD_ENUM("Sensor_5", 12,"");
  _ADD_ENUM("Sensor_6", 13,"");
  _ADD_ENUM("Off", 14,"Bargraph");
};

theEnum ="multiswitch_StatusRegister";
{
  
  _ADD_ENUM("Relais_On", Bit0,"Relay from Multiswitch On/Off");
}; bitFields.insert(theEnum);

theEnum ="aquaero_ControlRegister";
{
  
  _ADD_ENUM("Default", 0,"default");
  _ADD_ENUM("Keylock", 1,"Keylock");
  _ADD_ENUM("EEPROM_Reset", 2,"EEPROM reseten");
};

theEnum ="aquaero_ScreenRegister";
{
  
  _ADD_ENUM("Setup", 0,"Start Aquaero in first-time setup mode");
  _ADD_ENUM("USBLCD", 255,"USB LCD Mode");
};

theEnum ="aquabus_StatusRegister";
{
  
  _ADD_ENUM("Fail", Bit0,"Aquabus fail signal");
  _ADD_ENUM("Busy", Bit1,"Aquabus busy");
  _ADD_ENUM("Reading", Bit2,"Aquabus is reading");
  _ADD_ENUM("Writing", Bit3,"Aquabus is writing");
  _ADD_ENUM("NewData", Bit4,"New data is waiting to be written");
  _ADD_ENUM("EEPROM_Busy", Bit5,"EEPROM is busy");
}; bitFields.insert(theEnum);

theEnum ="aquabus_DeviceRegister";
{
  
  _ADD_ENUM("aquastream_1", Bit0,"Aquastream1");
  _ADD_ENUM("aquastream_2", Bit1,"Aquastream2");
  _ADD_ENUM("tubemeter", Bit3,"Tubemeter");
  _ADD_ENUM("multiswitch_1", Bit4,"Multiswitch1");
  _ADD_ENUM("multiswitch_2", Bit5,"Multiswitch2");
}; bitFields.insert(theEnum);

theEnum ="USBLCD_ModeRegister";
{
  
  _ADD_ENUM("USBLCD_Active", Bit0,"USBLCD-Mode is active");
  _ADD_ENUM("USBLCD_NewData", Bit1,"New data is being sent to LCD");
  _ADD_ENUM("USBLCD_Flashing", Bit2,"Aquaero is being flashed");
  _ADD_ENUM("USBLCD_ConnectionPage", Bit3,"Display page with USB-mode appears");
}; bitFields.insert(theEnum);

theEnum ="LCD_TypeRegister";
{
  
  _ADD_ENUM("LCD", 0,"LCD");
  _ADD_ENUM("VFD", 1,"VFD");
  _ADD_ENUM("OLED", 2,"OLED");
  _ADD_ENUM("NODISPLAY", 255,"No Display");
};

 
}


#endif
