// -*- c++ -*- $Id: definePropMap.h 35 2007-05-05 15:09:55Z matt $

/* Generated file: property-map-definition of AquaeroData_rev4
 * 
 * Copyright (C) 2006 Christian Unger <coder at breakbe D0T at>
 * Translated by John Frushour 
 * 
 * This library is covered by the LGPL, read LICENSE for details.
 */

/* WARNING: This file has been generated (semi-)automatically!
 *          DO NOT EDIT IT UNLESS YOU KNOW WHAT YOU ARE DOING
 */


#define PSTRING new StringParser()
#define PPERCENT new LinearTransformParser<unsigned char>(255.0f / 100, 0.0f)
#define PINT new ValueParser<int>()
#define PTEMP(T) new NamedValueParser<T>( new LinearTransformParser<T>(10.0f, 0.0f), 20000, "--.-" )
#define PFLOW(T) new NamedValueParser<T>( new LinearTransformParser<T>(100.0f, 0.0f, 4), 0, "--.--" )
#define AUTO new AutomaticParser(false)
#define RDONLY new AutomaticParser(true)
#define RDONLY_TEMP new ReadOnlyParser<int>(  PTEMP(int)  )
#define RDONLY_RAW  new ReadOnlyParser<string>( new RawDataParser() )
#define ENUM new AutomaticParser(false,true)


_MK_PD( Fan_Name[1], "fan.1.name", "Fan Name (11 Characters)", PSTRING );
_MK_PD( Fan_Name[2], "fan.2.name", "Fan Name (11 Characters)", PSTRING );
_MK_PD( Fan_Name[3], "fan.3.name", "Fan Name (11 Characters)", PSTRING );
_MK_PD( Fan_Name[4], "fan.4.name", "Fan Name (11 Characters)", PSTRING );
_MK_PD( Flowsensor_Name, "flowsensor.1.name", "Flowsensor Name", PSTRING );

_MK_PD( Sensor_Name[1], "sensor.1.name", "Temperature Sensor Name (11 Characters)", PSTRING );
_MK_PD( Sensor_Name[2], "sensor.2.name", "Temperature Sensor Name (11 Characters)", PSTRING );
_MK_PD( Sensor_Name[3], "sensor.3.name", "Temperature Sensor Name (11 Characters)", PSTRING );
_MK_PD( Sensor_Name[4], "sensor.4.name", "Temperature Sensor Name (11 Characters)", PSTRING );
_MK_PD( Sensor_Name[5], "sensor.5.name", "Temperature Sensor Name (11 Characters)", PSTRING );
_MK_PD( Sensor_Name[6], "sensor.6.name", "Temperature Sensor Name (11 Characters)", PSTRING );

_MK_PD( aquaero_Name, "aquaero.name", "Aquaero Name", PSTRING );

_MK_PD( Profil, "profile", "Load the specified Profile", ENUM );

_MK_PD( Fan_MinimumPower[1], "fan.1.minimumPower", "Minimal power level for fan, via PWM. Only with automatic setting.", PPERCENT );
_MK_PD( Fan_MinimumPower[2], "fan.2.minimumPower", "Minimal power level for fan, via PWM. Only with automatic setting.", PPERCENT );
_MK_PD( Fan_MinimumPower[3], "fan.3.minimumPower", "Minimal power level for fan, via PWM. Only with automatic setting.", PPERCENT );
_MK_PD( Fan_MinimumPower[4], "fan.4.minimumPower", "Minimal power level for fan, via PWM. Only with automatic setting.", PPERCENT );

_MK_PD( Fan_Pulse[1], "fan.1.pulse", "Impulses/Revolution (>1)", AUTO );
_MK_PD( Fan_Pulse[2], "fan.2.pulse", "Impulses/Revolution (>1)", AUTO );
_MK_PD( Fan_Pulse[3], "fan.3.pulse", "Impulses/Revolution (>1)", AUTO );
_MK_PD( Fan_Pulse[4], "fan.4.pulse", "Impulses/Revolution (>1)", AUTO );

_MK_PD( Flowsensor_Pulse, "flowsensor.1.pulse", "Impulses/Liter", AUTO );

_MK_PD( LCD_Backlight, "lcd.backlight", "Backlight PWM, 0-200", new LinearTransformParser<unsigned char>(200.0f / 100.0f, 0.0f) );
_MK_PD( LCD_Contrast, "lcd.contrast", "Contrast PWM, 155-255", new LinearTransformParser<unsigned char>(1.0f, 155.0f) );
_MK_PD( LCD_Fadingtime, "lcd.fadingTime", "LCD fade Timeout", AUTO );

_MK_PD( Sensor_Faktor[1], "sensor.1.factor", "Correction Factor", new LinearTransformParser<unsigned char>(1.0f, 128.0f) );
_MK_PD( Sensor_Faktor[2], "sensor.2.factor", "Correction Factor", new LinearTransformParser<unsigned char>(1.0f, 128.0f) );
_MK_PD( Sensor_Faktor[3], "sensor.3.factor", "Correction Factor", new LinearTransformParser<unsigned char>(1.0f, 128.0f) );
_MK_PD( Sensor_Faktor[4], "sensor.4.factor", "Correction Factor", new LinearTransformParser<unsigned char>(1.0f, 128.0f) );
_MK_PD( Sensor_Faktor[5], "sensor.5.factor", "Correction Factor", new LinearTransformParser<unsigned char>(1.0f, 128.0f) );
_MK_PD( Sensor_Faktor[6], "sensor.6.factor", "Correction Factor", new LinearTransformParser<unsigned char>(1.0f, 128.0f) );

_MK_PD( Sensor_Offset[1], "sensor.1.offset", "Correction Offset", new LinearTransformParser<unsigned char>(10.0f, 128.0f) );
_MK_PD( Sensor_Offset[2], "sensor.2.offset", "Correction Offset", new LinearTransformParser<unsigned char>(10.0f, 128.0f) );
_MK_PD( Sensor_Offset[3], "sensor.3.offset", "Correction Offset", new LinearTransformParser<unsigned char>(10.0f, 128.0f) );
_MK_PD( Sensor_Offset[4], "sensor.4.offset", "Correction Offset", new LinearTransformParser<unsigned char>(10.0f, 128.0f) );
_MK_PD( Sensor_Offset[5], "sensor.5.offset", "Correction Offset", new LinearTransformParser<unsigned char>(10.0f, 128.0f) );
_MK_PD( Sensor_Offset[6], "sensor.6.offset", "Correction Offset", new LinearTransformParser<unsigned char>(10.0f, 128.0f) );

_MK_PD( Sensor_Alarm[1], "sensor.1.alarm", "Alarm Temperature", PTEMP(int) );
_MK_PD( Sensor_Alarm[2], "sensor.2.alarm", "Alarm Temperature", PTEMP(int) );
_MK_PD( Sensor_Alarm[3], "sensor.3.alarm", "Alarm Temperature", PTEMP(int) );
_MK_PD( Sensor_Alarm[4], "sensor.4.alarm", "Alarm Temperature", PTEMP(int) );
_MK_PD( Sensor_Alarm[5], "sensor.5.alarm", "Alarm Temperature", PTEMP(int) );
_MK_PD( Sensor_Alarm[6], "sensor.6.alarm", "Alarm Temperature", PTEMP(int) );

//Ab hier Profildaten , AUTO );", AUTO );
_MK_PD( LCD_Settings, "lcd.settings", "", AUTO );
_MK_PD( LCD_PageDelay, "lcd.pageDelay", "LCD Page Delay (255 = never change page)", new NamedValueParser<Byte>(new ValueParser<unsigned char>(),255,"never switch") );
_MK_PD( LCD_Page, "lcd.pages", "This will configure the LCD display pages", AUTO );

_MK_F2S_PD( Powersensor_1_Sensor, false, "powersensor.1.sensor.1", "Power Sensor 1, Power Measurement 1" );
_MK_F2S_PD( Powersensor_1_Sensor, true,  "powersensor.1.sensor.2", "Power Sensor 1, Power Measurement 2" );
_MK_F2S_PD( Powersensor_2_Sensor, false, "powersensor.2.sensor.1", "Power Sensor 2, Power Measurement 1" );
_MK_F2S_PD( Powersensor_2_Sensor, true,  "powersensor.2.sensor.2", "Power Sensor 2, Power Measurement 2" );

_MK_PD( Powersensor_Config, "powersensor.config", "???", AUTO );

_MK_PD( tubemeter_Config, "tubemeter.config", "???", AUTO );
_MK_PD( tubemeter_Alarmlevel, "tubemeter.alarmLevel", "Tubemeter alarm level", AUTO );
_MK_PD( tubemeter_Alarmmove, "tubemeter.alarmMove", "Tubemeter motion alarm", AUTO );
_MK_PD( tubemeter_Expansion, "tubemeter.expansion", "Tubemeter expansion bus", AUTO );

_MK_F2S_PD(Fan_Sensor[1],false,"fan.1.sensor.1","Fan 1, Sensor 1");
_MK_F2S_PD(Fan_Sensor[2],false,"fan.2.sensor.1","Fan 2, Sensor 1");
_MK_F2S_PD(Fan_Sensor[3],false,"fan.3.sensor.1","Fan 3, Sensor 1");
_MK_F2S_PD(Fan_Sensor[4],false,"fan.4.sensor.1","Fan 4, Sensor 1");
_MK_F2S_PD(Fan_Sensor[1],true ,"fan.1.sensor.2","Fan 1, Sensor 2");
_MK_F2S_PD(Fan_Sensor[2],true ,"fan.2.sensor.2","Fan 1, Sensor 2");
_MK_F2S_PD(Fan_Sensor[3],true ,"fan.3.sensor.2","Fan 1, Sensor 2");
_MK_F2S_PD(Fan_Sensor[4],true ,"fan.4.sensor.2","Fan 1, Sensor 2");
/*
_MK_PD( Fan_Sensor[1], "fan.1.sensors", "Zuordnung der Sensoren/L�fter, untere 4-Byte / obere 4-Byte", AUTO );
_MK_PD( Fan_Sensor[2], "fan.2.sensors", "Zuordnung der Sensoren/L�fter, untere 4-Byte / obere 4-Byte", AUTO );
_MK_PD( Fan_Sensor[3], "fan.3.sensors", "Zuordnung der Sensoren/L�fter, untere 4-Byte / obere 4-Byte", AUTO );
_MK_PD( Fan_Sensor[4], "fan.4.sensors", "Zuordnung der Sensoren/L�fter, untere 4-Byte / obere 4-Byte", AUTO );
*/
_MK_PD( Fan_SetRPM[1], "fan.1.setRPM", "Set Speed", AUTO );
_MK_PD( Fan_SetRPM[2], "fan.2.setRPM", "Set Speed", AUTO );
_MK_PD( Fan_SetRPM[3], "fan.3.setRPM", "Set Speed", AUTO );
_MK_PD( Fan_SetRPM[4], "fan.4.setRPM", "Set Speed", AUTO );

_MK_PD( Fan_SetPower[1], "fan.1.setPower", "Set Power", PPERCENT );
_MK_PD( Fan_SetPower[2], "fan.2.setPower", "Set Power", PPERCENT );
_MK_PD( Fan_SetPower[3], "fan.3.setPower", "Set Power", PPERCENT );
_MK_PD( Fan_SetPower[4], "fan.4.setPower", "Set Power", PPERCENT );

_MK_PD( Fan_Mode[1], "fan.1.mode_raw", "Fan setting", AUTO );
_MK_PD( Fan_Mode[2], "fan.2.mode_raw", "Fan setting", AUTO );
_MK_PD( Fan_Mode[3], "fan.3.mode_raw", "Fan setting", AUTO );
_MK_PD( Fan_Mode[4], "fan.4.mode_raw", "Fan setting", AUTO );

_MK_PD( Fan_Mode[1], "fan.1.mode", "Simplified Fan Setting", new FriendlyFanModeParser() );
_MK_PD( Fan_Mode[2], "fan.2.mode", "Simplified Fan Setting", new FriendlyFanModeParser() );
_MK_PD( Fan_Mode[3], "fan.3.mode", "Simplified Fan Setting", new FriendlyFanModeParser() );
_MK_PD( Fan_Mode[4], "fan.4.mode", "Simplified Fan Setting", new FriendlyFanModeParser() );


_MK_PD( Flowsensor_Mode, "flowsensor.mode", "", AUTO );
_MK_PD( FlowSensor_lAlarm[1], "flowsensor.1.lowAlarm", "Low level Alarm in Liter", PFLOW(int) );
_MK_PD( FlowSensor_lAlarm[2], "flowsensor.2.lowAlarm", "Low level Alarm in Liter", PFLOW(int) );

_MK_PD( LED_PWM[1], "led.1.pwm", "Pulse-Width-Modulation", new LinearTransformParser<unsigned char>(0.8f, 0.0f) );
_MK_PD( LED_PWM[2], "led.2.pwm", "Pulse-Width-Modulation", new LinearTransformParser<unsigned char>(0.8f, 0.0f) );
_MK_PD( LED_Mode[1], "led.1.mode", "This is where to set the LED mode", AUTO );
_MK_PD( LED_Mode[2], "led.2.mode", "This is where to set the LED mode", AUTO );

_MK_L2S_PD( LED_Sensor[1], false, "led.1.sensor.1", "LED 1, Sensor 1" );
_MK_L2S_PD( LED_Sensor[2], false, "led.2.sensor.1", "LED 2, Sensor 1" );
_MK_L2S_PD( LED_Sensor[1], true , "led.1.sensor.2", "LED 1, Sensor 2" );
_MK_L2S_PD( LED_Sensor[2], true , "led.2.sensor.2", "LED 2, Sensor 2" );
/*
_MK_PD( LED_Sensor[1], "led.1.sensor", "Zuordnung der Sensoren/LEDs, untere 4-Byte / obere 4-Byte", AUTO );
_MK_PD( LED_Sensor[2], "led.2.sensor", "Zuordnung der Sensoren/LEDs, untere 4-Byte / obere 4-Byte", AUTO );
*/

_MK_PD( Sensor_Min[1], "sensor.1.min", "Minimum temperature level", PTEMP(int) );
_MK_PD( Sensor_Min[2], "sensor.2.min", "Minimum temperature level", PTEMP(int) );
_MK_PD( Sensor_Min[3], "sensor.3.min", "Minimum temperature level", PTEMP(int) );
_MK_PD( Sensor_Min[4], "sensor.4.min", "Minimum temperature level", PTEMP(int) );
_MK_PD( Sensor_Min[5], "sensor.5.min", "Minimum temperature level", PTEMP(int) );
_MK_PD( Sensor_Min[6], "sensor.6.min", "Minimum temperature level", PTEMP(int) );

_MK_PD( Sensor_Max[1], "sensor.1.max", "Maximum temperature level", PTEMP(int) );
_MK_PD( Sensor_Max[2], "sensor.2.max", "Maximum temperature level", PTEMP(int) );
_MK_PD( Sensor_Max[3], "sensor.3.max", "Maximum temperature level", PTEMP(int) );
_MK_PD( Sensor_Max[4], "sensor.4.max", "Maximum temperature level", PTEMP(int) );
_MK_PD( Sensor_Max[5], "sensor.5.max", "Maximum temperature level", PTEMP(int) );
_MK_PD( Sensor_Max[6], "sensor.6.max", "Maximum temperature level", PTEMP(int) );

_MK_PD( Sensor_Hyst[1], "sensor.1.hyst", "Hysteresis", new LinearTransformParser<unsigned char>(10.0f, 0.0f) );
_MK_PD( Sensor_Hyst[2], "sensor.2.hyst", "Hysteresis", new LinearTransformParser<unsigned char>(10.0f, 0.0f) );
_MK_PD( Sensor_Hyst[3], "sensor.3.hyst", "Hysteresis", new LinearTransformParser<unsigned char>(10.0f, 0.0f) );
_MK_PD( Sensor_Hyst[4], "sensor.4.hyst", "Hysteresis", new LinearTransformParser<unsigned char>(10.0f, 0.0f) );
_MK_PD( Sensor_Hyst[5], "sensor.5.hyst", "Hysteresis", new LinearTransformParser<unsigned char>(10.0f, 0.0f) );
_MK_PD( Sensor_Hyst[6], "sensor.6.hyst", "Hysteresis", new LinearTransformParser<unsigned char>(10.0f, 0.0f) );

_MK_PD( Sensor_Opt[1], "sensor.1.opt", "Reference temperature for sensor", PTEMP(int) );
_MK_PD( Sensor_Opt[2], "sensor.2.opt", "Reference temperature for sensor", PTEMP(int) );
_MK_PD( Sensor_Opt[3], "sensor.3.opt", "Reference temperature for sensor", PTEMP(int) );
_MK_PD( Sensor_Opt[4], "sensor.4.opt", "Reference temperature for sensor", PTEMP(int) );
_MK_PD( Sensor_Opt[5], "sensor.5.opt", "Reference temperature for sensor", PTEMP(int) );
_MK_PD( Sensor_Opt[6], "sensor.6.opt", "Reference temperature for sensor", PTEMP(int) );

_MK_PD( Controler_Time[1], "sensor.1.controllerTime", "Controller Delay", AUTO );
_MK_PD( Controler_Time[2], "sensor.2.controllerTime", "Controller Delay", AUTO );
_MK_PD( Controler_Time[3], "sensor.3.controllerTime", "Controller Delay", AUTO );
_MK_PD( Controler_Time[4], "sensor.4.controllerTime", "Controller Delay", AUTO );
_MK_PD( Controler_Time[5], "sensor.5.controllerTime", "Controller Delay", AUTO );
_MK_PD( Controler_Time[6], "sensor.6.controllerTime", "Controller Delay", AUTO );

_MK_PD( Controler_Factor[1], "sensor.1.controllerFactor", "Controller Factor", AUTO );
_MK_PD( Controler_Factor[2], "sensor.2.controllerFactor", "Controller Factor", AUTO );
_MK_PD( Controler_Factor[3], "sensor.3.controllerFactor", "Controller Factor", AUTO );
_MK_PD( Controler_Factor[4], "sensor.4.controllerFactor", "Controller Factor", AUTO );
_MK_PD( Controler_Factor[5], "sensor.5.controllerFactor", "Controller Factor", AUTO );
_MK_PD( Controler_Factor[6], "sensor.6.controllerFactor", "Controller Factor", AUTO );

_MK_PD( Alarm_Setup, "alarm.setup", "Alarm setup menu", AUTO );
_MK_PD( Alarm_ShutdownDelay, "alarm.shutdownDelay", "Alarm Shutdown delay (0-180) Seonds", AUTO );

_MK_PD( Timer_Off_Hours[1], "timer.1.off.hours", "Timer Off-time (hours)", AUTO );
_MK_PD( Timer_Off_Minutes[1], "timer.1.off.minutes", "Timer Off-time (minutes)", AUTO );
_MK_PD( Timer_Off_Seconds[1], "timer.1.off.seconds", "Time Off-time (seconds)", AUTO );
_MK_PD( Timer_Off_Day[1], "timer.1.off.day", "Timer Off-time (days)", AUTO );
_MK_PD( Timer_Off_SwitchTime[1], "timer.1.off.switchTime", "Timer for relay Off-Time (255->for always on)", new NamedValueParser<Byte>(new ValueParser<unsigned char>(),255,"always on") );

_MK_PD( Timer_Off_Hours[2], "timer.2.off.hours", "Timer Off-time (hours)", AUTO );
_MK_PD( Timer_Off_Minutes[2], "timer.2.off.minutes", "Timer Off-time (minutes)", AUTO );
_MK_PD( Timer_Off_Seconds[2], "timer.2.off.seconds", "Time Off-time (seconds)", AUTO );
_MK_PD( Timer_Off_Day[2], "timer.2.off.day", "Timer Off-time (days)", AUTO );
_MK_PD( Timer_Off_SwitchTime[2], "timer.2.off.switchTime", "Timer for relay Off-Time (255->for always on)", new NamedValueParser<Byte>(new ValueParser<unsigned char>(),255,"always on") );

_MK_PD( Timer_On_Hours[1], "timer.1.on.hours", "Timer On-time (hours)", AUTO );
_MK_PD( Timer_On_Minutes[1], "timer.1.on.minutes", "Timer On-time (minutes)", AUTO );
_MK_PD( Timer_On_Seconds[1], "timer.1.on.seconds", "Time On-time (seconds)", AUTO );
_MK_PD( Timer_On_Day[1], "timer.1.on.day", "Timer On-time (days)", AUTO );
_MK_PD( Timer_On_SwitchTime[1], "timer.1.on.switchTime", "Timer for relay On-Time (255->for always on)", new NamedValueParser<Byte>(new ValueParser<unsigned char>(),255,"always on") );

_MK_PD( Timer_On_Hours[2], "timer.2.on.hours", "Timer On-time (hours)", AUTO );
_MK_PD( Timer_On_Minutes[2], "timer.2.on.minutes", "Timer On-time (minutes)", AUTO );
_MK_PD( Timer_On_Seconds[2], "timer.2.on.seconds", "Time On-time (seconds)", AUTO );
_MK_PD( Timer_On_Day[2], "timer.2.on.day", "Timer On-time (days)", AUTO );
_MK_PD( Timer_On_SwitchTime[2], "timer.2.on.switchTime", "Timer for relay On-Time (255->for always on)", new NamedValueParser<Byte>(new ValueParser<unsigned char>(),255,"always on") );

_MK_PD( aquastream_Command[1], "aquastream.1.command", "", AUTO );
_MK_PD( aquastream_Command[2], "aquastream.2.command", "", AUTO );

_MK_PD_CAST( int, aquastream_Command[1], "aquastream.1.rawFrequency", "Raw-Data: Frequency in Hz (40-74) or Anti_Air for De-aeration mode", new NamedValueParser<int>(new ValueParser<int>(),75,"Anti_Air") );
_MK_PD_CAST( int, aquastream_Command[2], "aquastream.2.rawFrequency", "Raw-Data: Frequency in Hz (40-74) or Anti_Air for De-aeration mode", new NamedValueParser<int>(new ValueParser<int>(),75,"Anti_Air") );

_MK_PD( Relay_ShutdownTime, "relay.shutdownTime", "For use as power switch, the time setting must always be max (255 = always)", new NamedValueParser<Byte>(new ValueParser<unsigned char>(),255,"always") );
_MK_PD( Relay_Status, "relay.status", "Relay Status - from multiswitch", AUTO );

_MK_PD( multiswitch_AMP_Register[1], "multiswitch.1.amp.register", "Multiswitch amp", AUTO );
_MK_PD( multiswitch_LED_Register[1], "multiswitch.1.led.register", "Multiswitch LED", AUTO );
_MK_PD( multiswitch_AMP_Register[2], "multiswitch.2.amp.register", "Multiswitch amp", AUTO );
_MK_PD( multiswitch_LED_Register[2], "multiswitch.2.led.register", "Multiswitch LED", AUTO );

_MK_PD( multiswitch_Bargraph_AMP[1], "multiswitch.1.bargraph.amp", "Relay from Multiswitch On/Off", AUTO );
_MK_PD( multiswitch_Bargraph_LED[1], "multiswitch.1.bargraph.led", "Relay from Multiswitch On/Off", AUTO );
_MK_PD( multiswitch_Status[1], "multiswitch.1.status", "Free-range expansion, RW", AUTO );
_MK_PD( multiswitch_Bargraph_AMP[2], "multiswitch.2.bargraph.amp", "Relay from Multiswitch On/Off", AUTO );
_MK_PD( multiswitch_Bargraph_LED[2], "multiswitch.2.bargraph.led", "Relay from Multiswitch On/Off", AUTO );
_MK_PD( multiswitch_Status[2], "multiswitch.2.status", "Free-range expansion RW", AUTO );

//Ende Profil", AUTO );
_MK_PD( Expansion_RW, "expansion.rw", "Free-range expansion RW", new RawDataParser() );
_MK_PD( aquabus_Buffer, "aquabus.buffer", "Aquabus buffer status", new RawDataParser() );

_MK_PD( Time_Hours, "time.hours", "Actual time, Hours", AUTO );
_MK_PD( Time_Minutes, "time.minutes", "Actual Time, Minutes", AUTO );
_MK_PD( Time_Seconds, "time.seconds", "Actual Time, Seconds", AUTO );
_MK_PD( Time_Day, "time.day", "Actual Days", AUTO );

_MK_PD( aquaero_Control, "aquaero.control", "Aquaero keylock", AUTO );
_MK_PD( aquaero_Screen, "lcd.screen", "Aquaero screen control", AUTO );

//Read-Only-Daten
_MK_PD( aquabus_Status, "aquabus.status", "Aquabus status from Aquastream 1", RDONLY );
_MK_PD( aquabus_Devices, "aquabus.devices", "Aquabus connected devices", new ReadOnlyParser<AquaeroData_rev4::aquabus_DeviceRegister>(new EnumParser<AquaeroData_rev4::aquabus_DeviceRegister>("aquabus_DeviceRegister")) );

_MK_PD( multiswitch_OutputData[1][ 1], "multiswitch.1.outputdata.1", "Data from the multiswitch sent to the Aquaero", RDONLY );
_MK_PD( multiswitch_OutputData[1][ 2], "multiswitch.1.outputdata.2", "Data from the multiswitch sent to the Aquaero", RDONLY );
_MK_PD( multiswitch_OutputData[1][ 3], "multiswitch.1.outputdata.3", "Data from the multiswitch sent to the Aquaero", RDONLY );
_MK_PD( multiswitch_OutputData[2][ 1], "multiswitch.2.outputdata.1", "Data from the multiswitch sent to the Aquaero", RDONLY );
_MK_PD( multiswitch_OutputData[2][ 2], "multiswitch.2.outputdata.2", "Data from the multiswitch sent to the Aquaero", RDONLY );
_MK_PD( multiswitch_OutputData[2][ 3], "multiswitch.2.outputdata.3", "Data from the multiswitch sent to the Aquaero", RDONLY );

_MK_PD( tubemeter_rawlevel, "tubemeter.rawLevel", "Tubemeter level", RDONLY );
_MK_PD( tubemeter_wavelevel, "tubemeter.waveLevel", "Tubemeter wave level", RDONLY );
_MK_PD( tubemeter_level_mm, "tubemeter.level.mm", "Level in mm", RDONLY );
_MK_PD( tubemeter_level_avg, "tubemeter.level.avg", "Average Level", RDONLY );

_MK_PD( aquastream_1_RawData, "aquastream.1.rawdata", "16 Byte RAW-Data", RDONLY_RAW );
_MK_PD( aquastream_1_Current, "aquastream.1.current", "Current", RDONLY );
_MK_PD( aquastream_1_Voltage, "aquastream.1.voltage", "Voltage", new ReadOnlyParser<int>(new LinearTransformParser<int>(10.0f, 0.0f)) );
_MK_PD( aquastream_1_Power, "aquastream.1.power", "Power", new ReadOnlyParser<long>(new LinearTransformParser<long>(10.0f, 0.0f)) );

_MK_PD( aquastream_2_RawData, "aquastream.2.rawdata", "16 Byte RAW-Data", RDONLY_RAW );
_MK_PD( aquastream_2_Current, "aquastream.2.current", "Current", RDONLY );
_MK_PD( aquastream_2_Voltage, "aquastream.2.voltage", "Voltage", new ReadOnlyParser<int>(new LinearTransformParser<int>(10.0f, 0.0f)) );
_MK_PD( aquastream_2_Power, "aquastream.2.power", "Power", new ReadOnlyParser<long>(new LinearTransformParser<long>(10.0f, 0.0f)) );

_MK_PD( USBLCD_Mode, "lcd.usblcd.mode", "", RDONLY );

_MK_PD( Powersensor_Power[1], "powersensor.1.power", "Power Measurement", RDONLY );
_MK_PD( Powersensor_Power[2], "powersensor.2.power", "Power Measurement", RDONLY );

_MK_PD( Fan_RPM[1], "fan.1.rpm", "Current fan RPM", RDONLY );
_MK_PD( Fan_RPM[2], "fan.2.rpm", "Current fan RPM", RDONLY );
_MK_PD( Fan_RPM[3], "fan.3.rpm", "Current fan RPM", RDONLY );
_MK_PD( Fan_RPM[4], "fan.4.rpm", "Current fan RPM", RDONLY );

_MK_PD( Flowsensor_l, "flowsensor.1.flow", "Flowsensor ", new ReadOnlyParser<int>(  PFLOW(int)  ) );

_MK_PD( Fan_MaxRPM[1], "fan.1.maxRPM", "Maximum fan RPM", RDONLY );
_MK_PD( Fan_MaxRPM[2], "fan.2.maxRPM", "Maximum fan RPM", RDONLY );
_MK_PD( Fan_MaxRPM[3], "fan.3.maxRPM", "Maximum fan RPM", RDONLY );
_MK_PD( Fan_MaxRPM[4], "fan.4.maxRPM", "Maximum fan RPM", RDONLY );

_MK_PD( Sensor_Temp[1], "sensor.1.temp", "Actual Temperature", RDONLY_TEMP );
_MK_PD( Sensor_Temp[2], "sensor.2.temp", "Actual Temperature", RDONLY_TEMP );
_MK_PD( Sensor_Temp[3], "sensor.3.temp", "Actual Temperature", RDONLY_TEMP );
_MK_PD( Sensor_Temp[4], "sensor.4.temp", "Actual Temperature", RDONLY_TEMP );
_MK_PD( Sensor_Temp[5], "sensor.5.temp", "Actual Temperature", RDONLY_TEMP );
_MK_PD( Sensor_Temp[6], "sensor.6.temp", "Actual Temperature", RDONLY_TEMP );

_MK_PD( Alarm_Status, "alarm.status", "Alarm status...", RDONLY );
_MK_PD( Alarm_Memory_Pointer, "alarm.memory.pointer", "Pointer to the last alarm in the buffer", RDONLY );

_MK_PD( Alarm_Memory_Typ[1], "alarm.1.memory.typ", "Alarm Type", RDONLY );
_MK_PD( Alarm_Memory_Sensor[1], "alarm.1.memory.sensor", "Sensor Number, that issued the alarm", RDONLY );
_MK_PD( Alarm_Memory_Hours[1], "alarm.1.memory.hours", "Alarm Time, Hours", RDONLY );
_MK_PD( Alarm_Memory_Minutes[1], "alarm.1.memory.minutes", "Alarm Time, Minutes", RDONLY );
_MK_PD( Alarm_Memory_Day[1], "alarm.1.memory.day", "Alarm Time, Seconds", RDONLY );

_MK_PD( Alarm_Memory_Typ[2], "alarm.2.memory.typ", "Alarm Type", RDONLY );
_MK_PD( Alarm_Memory_Sensor[2], "alarm.2.memory.sensor", "Sensor Number, that issued the alarm", RDONLY );
_MK_PD( Alarm_Memory_Hours[2], "alarm.2.memory.hours", "Alarm Time, Hours", RDONLY );
_MK_PD( Alarm_Memory_Minutes[2], "alarm.2.memory.minutes", "Alarm Time, Minutes", RDONLY );
_MK_PD( Alarm_Memory_Day[2], "alarm.2.memory.day", "Alarm Time, Seconds", RDONLY );

_MK_PD( Alarm_Memory_Typ[3], "alarm.3.memory.typ", "Alarm Type", RDONLY );
_MK_PD( Alarm_Memory_Sensor[3], "alarm.3.memory.sensor", "Sensor Number, that issued the alarm", RDONLY );
_MK_PD( Alarm_Memory_Hours[3], "alarm.3.memory.hours", "Alarm Time, Hours", RDONLY );
_MK_PD( Alarm_Memory_Minutes[3], "alarm.3.memory.minutes", "Alarm Time, Minutes", RDONLY );
_MK_PD( Alarm_Memory_Day[3], "alarm.3.memory.day", "Alarm Time, Seconds", RDONLY );

_MK_PD( Alarm_Memory_Typ[4], "alarm.4.memory.typ", "Alarm Type", RDONLY );
_MK_PD( Alarm_Memory_Sensor[4], "alarm.4.memory.sensor", "Sensor Number, that issued the alarm", RDONLY );
_MK_PD( Alarm_Memory_Hours[4], "alarm.4.memory.hours", "Alarm Time, Hours", RDONLY );
_MK_PD( Alarm_Memory_Minutes[4], "alarm.4.memory.minutes", "Alarm Time, Minutes", RDONLY );
_MK_PD( Alarm_Memory_Day[4], "alarm.4.memory.day", "Alarm Time, Seconds", RDONLY );

_MK_PD( Alarm_Memory_Typ[5], "alarm.5.memory.typ", "Alarm Type", RDONLY );
_MK_PD( Alarm_Memory_Sensor[5], "alarm.5.memory.sensor", "Sensor Number, that issued the alarm", RDONLY );
_MK_PD( Alarm_Memory_Hours[5], "alarm.5.memory.hours", "Alarm time, Hours", RDONLY );
_MK_PD( Alarm_Memory_Minutes[5], "alarm.5.memory.minutes", "Alarm time, Minutes", RDONLY );
_MK_PD( Alarm_Memory_Day[5], "alarm.5.memory.day", "Alarm time, Seconds", RDONLY );

_MK_PD( Firmware_Name, "aquaero.firmware.name", "Firmware-Revision (5 Characters)", RDONLY );

_MK_PD( LCD_Type, "lcd.typename", "Display Type", new ReadOnlyParser<AquaeroData_rev4::LCD_TypeRegister>(new EnumParser<AquaeroData_rev4::LCD_TypeRegister>("LCD_TypeRegister")) );
_MK_PD( OS_Version, "aquaero.os.version", "OS-Version", new ReadOnlyParser<int>(new LinearTransformParser<int>(100.0f, 0.0f)) );
_MK_PD( FirmwareRevision, "aquaero.firmware.revision", "Firmware-Version", new ReadOnlyParser<int>(new LinearTransformParser<int>(100.0f, 0.0f)) );
_MK_PD( Flashcounter, "aquaero.flashCounter", "Flashcounter", RDONLY );
_MK_PD( HardwareRevision, "aquaero.hardwareRevision", "Hardware-Version", new ReadOnlyParser<int>(new LinearTransformParser<int>(100.0f, 0.0f)) );
_MK_PD( Serialnumber, "aquaero.serialNumber", "Serial Number", RDONLY );
_MK_PD( ProductionMonth, "aquaero.productionMonth", "Konth of Production", RDONLY );
_MK_PD( ProductionYear, "aquaero.productionYear", "Year of Production", new ReadOnlyParser<Integer>(new LinearTransformParser<Integer>(1.0f, -2000.0f)) );
_MK_PD( Language, "aquaero.language", "Language", RDONLY );
_MK_PD( Expansion_RO, "expansion.ro", "Expansion-Bus RO", RDONLY_RAW );
