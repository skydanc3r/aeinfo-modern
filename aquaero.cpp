// $Id: aquaero.cpp 35 2007-05-05 15:09:55Z matt $

/* The aquaero-data-structures
 * 
 * Copyright (C) 2006 Christian Unger <coder(at)breakbe.at>
 * Translated by John Frushour
 * 
 * This library is covered by the LGPL, read LICENSE for details.
 */

#include <vector>
#include <time.h>



class BoxedArrayAccessException 
{
public:
  BoxedArrayAccessException ( )
  {
    if(VERBOSE)
      cout << "FATAL ERROR: BoxedArrayAccessException! " << endl
           << "Your code has semantic errors!" << endl;
  }
};


/** A BoxedArray is a Wrapper of an ordinary c++-array `T[int]'.
 *  It emulates VisualBasic-behaviour, where arrays usually start with
 *  index `1'. Therefore, this class simply throws an error, if one tries to
 *  access the element at index `0'. If you want an array with k elements,
 *  the template-parameter `N' must be set to `k+1'...
 */
template<typename T, int N>
class BoxedArray
{
  vector<T> data;
  
public:
  BoxedArray ( )
    : data(N)
  {
  }
  ~BoxedArray () 
  {
  }
  
  T& operator [] ( const int i ) 
  {
    if( i <= 0 )
      throw new BoxedArrayAccessException();
    if( i >= N ) 
      throw new BoxedArrayAccessException();
    
    return data[i];
  }
  
  const T& operator [] ( const int i ) const 
  {
    if( i <= 0 )
      throw new BoxedArrayAccessException();
    if( i >= N ) 
      throw new BoxedArrayAccessException();
    
    return data[i];
  }
  
  object_stream& operator << ( object_stream &os ) 
  {
    for( int i = 0; i < N; ++i ) {
      data[i] << os;
    }
    return os;
  }
};



class AquaeroData_rev4
{
public:
  AquaeroData_rev4 () 
  {
    this->serverSideWriteFlag = NONE;
  }
  
  enum EServerSideWriteFlag
    {
      NONE = 0,
      RUNNING = 1,
      UPDATE_PENDING = 2
    } serverSideWriteFlag;
  
  
# include "aq_trans.h"
  
# include "aq_rev4.h"
  
  enum Friendly_Fan_ModeRegister
  {
    GET_MAX_RPM_NOW = Bit3,
    IGNORE_RPM = Bit6,
    FORCE_MIN_POWER = Bit7,
    DISABLE_RPM_TEST = Bit8,
    MANUAL_POWER = Bit1,           // PWR_RPM_Switch | Manual_Auto_Switch
    MANUAL_RPM = Bit2,            // Manual_Auto_Switch & ~PWR_RPM_Switch,
    AUTO_LINEAR = Bit4, // ~Progressiv_Linear_Switch & ~Automatik_PWM_RPM_Switch & ~Manual_Auto_Switch,
    AUTO_PROG = Bit5, // Progressiv_Linear_Switch & ~Automatik_PWM_RPM_Switch & ~Manual_Auto_Switch,
    AUTO_TARGET = Bit9 // Automatik_PWM_RPM_Switch & ~Progressiv_Linear_Switch & ~Manual_Auto_Switch
  };
  
  void synchronizeClock () 
  {
#ifndef _WIN32
    time_t curTime;
    tm cur_tm;
    
    time(&curTime);
    localtime_r(&curTime,&cur_tm);
    
    Time_Hours = cur_tm.tm_hour;
    Time_Minutes = cur_tm.tm_min;
    Time_Seconds = cur_tm.tm_sec;
    Time_Day = (DayRegister)cur_tm.tm_wday;
    
    // TODO: implement for WINDOWS!!!
#endif
  }
  
  object_stream& operator << ( object_stream &os ) 
  {
    aquaero_Name << os;
    Fan_Name<< os;
    ((long&)TransferType) << os;
    UsbLcdTextLine1 << os;
    UsbLcdTextLine2 << os;
    Flash_Adress << os;
    Flash_Data << os;
    Flash_Check << os;
    Flash_Key << os;
    Flash_Delete_Key << os;
    Flowsensor_Name << os;
    Sensor_Name<< os;
    ((long&)Profil) << os;
    Fan_MinimumPower<< os;
    Fan_Pulse<< os;
    Flowsensor_Pulse << os;
    LCD_Backlight << os;
    LCD_Contrast << os;
    LCD_Fadingtime << os;
    Sensor_Faktor<< os;
    Sensor_Offset<< os;
    Sensor_Alarm<< os;
    FanTestMaxRpm << os;
    ((long&)LCD_Settings) << os;
    LCD_PageDelay << os;
    ((long&)LCD_Page) << os;
    Powersensor_1_Sensor << os;
    Powersensor_2_Sensor << os;
    ((long&)Powersensor_Config) << os;
    ((long&)tubemeter_Config) << os;
    tubemeter_Alarmlevel << os;
    tubemeter_Alarmmove << os;
    tubemeter_Expansion << os;
    Fan_Sensor1<< os;
    Fan_Sensor2<< os;
    Fan_Sensor<< os;
    Fan_SetRPM<< os;
    Fan_SetPower<< os;
    Fan_Mode<< os;
    ((long&)Flowsensor_Mode) << os;
    FlowSensor_lAlarm<< os;
    LED_PWM<< os;
    LED_Mode<< os;
    LED_Sensor<< os;
    Sensor_Min<< os;
    Sensor_Max<< os;
    Sensor_Hyst<< os;
    Sensor_Opt<< os;
    Controler_Time<< os;
    Controler_Factor<< os;
    ((long&)Alarm_Setup) << os;
    Alarm_ShutdownDelay << os;
    Timer_Off_Hours<< os;
    Timer_Off_Minutes<< os;
    Timer_Off_Seconds<< os;
    Timer_Off_Day<< os;
    Timer_Off_SwitchTime<< os;
    Timer_On_Hours<< os;
    Timer_On_Minutes<< os;
    Timer_On_Seconds<< os;
    Timer_On_Day<< os;
    Timer_On_SwitchTime<< os;
    aquastream_Command<< os;
    Relay_ShutdownTime << os;
    ((long&)Relay_Status) << os;
    multiswitch_AMP_Register<< os;
    multiswitch_LED_Register<< os;
    multiswitch_Bargraph_AMP<< os;
    multiswitch_Bargraph_LED<< os;
    multiswitch_Status<< os;
    Expansion_RW << os;
    aquabus_Buffer << os;
    Time_Hours << os;
    Time_Minutes << os;
    Time_Seconds << os;
    ((long&)Time_Day) << os;
    ((long&)aquaero_Control) << os;
    ((long&)aquaero_Screen) << os;
    ((long&)aquabus_Status) << os;
    ((long&)aquabus_Devices) << os;
    multiswitch_OutputData << os;
    tubemeter_rawlevel << os;
    tubemeter_wavelevel << os;
    tubemeter_level_mm << os;
    tubemeter_level_avg << os;
    aquastream_1_RawData << os;
    aquastream_1_Current << os;
    aquastream_1_Voltage << os;
    aquastream_1_Power << os;
    aquastream_2_RawData << os;
    aquastream_2_Current << os;
    aquastream_2_Voltage << os;
    aquastream_2_Power << os;
    ((long&)USBLCD_Mode) << os;
    Powersensor_Power<< os;
    Fan_RPM<< os;
    Flowsensor_l << os;
    Fan_MaxRPM<< os;
    Sensor_Temp<< os;
    ((long&)Alarm_Status) << os;
    Alarm_Memory_Pointer << os;
    Alarm_Memory_Typ<< os;
    Alarm_Memory_Sensor<< os;
    Alarm_Memory_Hours<< os;
    Alarm_Memory_Minutes<< os;
    Alarm_Memory_Day<< os;
    Firmware_Name << os;
    ((long&)LCD_Type) << os;
    OS_Version << os;
    FirmwareRevision << os;
    Flashcounter << os;
    HardwareRevision << os;
    Serialnumber << os;
    ProductionMonth << os;
    ProductionYear << os;
    Language << os;
    Expansion_RO << os;
    return os;
  }
};

object_stream& operator << ( AquaeroData_rev4::Fan_ModeRegister& e, object_stream &os )
{ ((long&)e) << os; return os; }
object_stream& operator << ( AquaeroData_rev4::LED_ModeRegister& e, object_stream &os )
{ ((long&)e) << os; return os; }
object_stream& operator << ( AquaeroData_rev4::DayRegister& e, object_stream &os )
{ ((long&)e) << os; return os; }
object_stream& operator << ( AquaeroData_rev4::aquastream_CommandRegister& e, object_stream &os )
{ ((long&)e) << os; return os; }
object_stream& operator << ( AquaeroData_rev4::multiswitch_BargraphRegister& e, object_stream &os )
{ ((long&)e) << os; return os; }
object_stream& operator << ( AquaeroData_rev4::multiswitch_StatusRegister& e, object_stream &os )
{ ((long&)e) << os; return os; }
object_stream& operator << ( AquaeroData_rev4::Alarm_SetupRegister& e, object_stream &os )
{ ((long&)e) << os; return os; }



#ifdef _PARSERS_H

class FriendlyFanModeParser
  : public ValueParser<AquaeroData_rev4:: Fan_ModeRegister>
{
  typedef AquaeroData_rev4:: Fan_ModeRegister T;
  typedef AquaeroData_rev4:: Friendly_Fan_ModeRegister friendly_mode_t;
  
  EnumParser<AquaeroData_rev4:: Friendly_Fan_ModeRegister> friendlyEnumParser;
  
  bool checkBit ( const int& value, const int& bit ) const 
  {
    if( (value & bit) == bit ) {
      return true;
    }
    return false;
  }
  
  friendly_mode_t toFriendly ( const T& v ) const 
  {
    int a = v;
    int ret = a & (Get_Max_RPM | Ignore_RPM | Force_MinimumPower | Disable_RPM_Test);
    
    if( checkBit(a, PWR_RPM_Switch) && checkBit(a, Manual_Auto_Switch) ) {
      ret = ret | MANUAL_POWER;
    }
    if( checkBit(a, Manual_Auto_Switch) && !checkBit(a, PWR_RPM_Switch) ) {
      ret = ret | MANUAL_RPM;
    }
    if( !checkBit(a, Progressiv_Linear_Switch)
        && !checkBit(a, Automatik_PWM_RPM_Switch)
        && !checkBit(a, Manual_Auto_Switch) ) {
      ret = ret | AUTO_LINEAR;
    }
    if( checkBit(a, Progressiv_Linear_Switch)
        && !checkBit(a, Automatik_PWM_RPM_Switch)
        && !checkBit(a, Manual_Auto_Switch) ) {
      ret = ret | AUTO_PROG;
    }
    if( checkBit(a, Automatik_PWM_RPM_Switch)
        && !checkBit(a, Progressiv_Linear_Switch)
        && !checkBit(a, Manual_Auto_Switch) ) {
      ret = ret | AUTO_TARGET;
    }
    
    return (friendly_mode_t)ret;
  }
  
  T toUnfriendly ( const friendly_mode_t& v ) const 
  {
    const char*
      errMsg = "Please specify only one control mode! (either MANUAL_POWER, "
      "MANUAL_RPM, AUTO_LINEAR, AUTO_PROG  or  AUTO_TARGET)";
    
    int a = v;
    int ret = a & (Get_Max_RPM | Ignore_RPM | Force_MinimumPower | Disable_RPM_Test);
    
    bool done = false;
    if( checkBit(a, MANUAL_POWER) ) {
      ret = ret | PWR_RPM_Switch | Manual_Auto_Switch;
      if( done ) throw errMsg;
      done = true;
    }
    if( checkBit(a, MANUAL_RPM) ) {
      ret = ret | Manual_Auto_Switch;
      if( done ) throw errMsg;
      done = true;
    }
    if( checkBit(a, AUTO_LINEAR) ) {
      ret = ret;
      if( done ) throw errMsg;
      done = true;
    }
    if( checkBit(a, AUTO_PROG) ) {
      ret = ret | Progressiv_Linear_Switch;
      if( done ) throw errMsg;
      done = true;
    }
    if( checkBit(a, AUTO_TARGET) ) {
      ret = ret | Automatik_PWM_RPM_Switch;
      if( done ) throw errMsg;
      done = true;
    }
    
    
    return (T)ret;
  }
  
  int Get_Max_RPM;
  int Ignore_RPM;
  int Force_MinimumPower;
  int Disable_RPM_Test;
  int PWR_RPM_Switch;
  int Manual_Auto_Switch;
  int MANUAL_POWER;
  int MANUAL_RPM;
  int Progressiv_Linear_Switch;
  int Automatik_PWM_RPM_Switch;
  int AUTO_LINEAR;
  int AUTO_TARGET;
  int AUTO_PROG;
  
public:
  FriendlyFanModeParser ( )
    : friendlyEnumParser("Friendly_Fan_ModeRegister")
  {
    Get_Max_RPM = AquaeroData_rev4:: Get_Max_RPM;
    Ignore_RPM = AquaeroData_rev4:: Ignore_RPM;
    Force_MinimumPower = AquaeroData_rev4:: Force_MinimumPower;
    Disable_RPM_Test = AquaeroData_rev4:: Disable_RPM_Test;
    PWR_RPM_Switch = AquaeroData_rev4:: PWR_RPM_Switch;
    Manual_Auto_Switch = AquaeroData_rev4:: Maual_Auto_Switch;
    MANUAL_POWER = AquaeroData_rev4:: MANUAL_POWER;
    MANUAL_RPM = AquaeroData_rev4:: MANUAL_RPM;
    Progressiv_Linear_Switch = AquaeroData_rev4:: Progressiv_Linear_Switch;
    Automatik_PWM_RPM_Switch = AquaeroData_rev4:: Automatik_PWM_RPM_Switch;
    AUTO_LINEAR = AquaeroData_rev4:: AUTO_LINEAR;
    AUTO_TARGET = AquaeroData_rev4:: AUTO_TARGET;
    AUTO_PROG = AquaeroData_rev4:: AUTO_PROG;
  }
  virtual ~FriendlyFanModeParser () 
  {
  }
  
  virtual T parse ( string s ) 
  {
    const friendly_mode_t v = friendlyEnumParser.parse( s );
    return toUnfriendly(v);
  }
  
  virtual void unparse ( ostream &ostr, const T& theValue ) const 
  {
    friendly_mode_t v = toFriendly(theValue);
    friendlyEnumParser.unparse( ostr, v );
  }
};

#endif



#include "stringEnum.h"



