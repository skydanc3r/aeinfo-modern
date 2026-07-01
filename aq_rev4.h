// -*- c++ -*- $Id: aq_rev4.h 35 2007-05-05 15:09:55Z matt $

/* Generated file: data-elements of AquaeroData_rev4
 * 
 * Copyright (C) 2006 Christian Unger <coder at breakbe D0T at>
 * Translated by John Frushour
 * 
 * This library is covered by the LGPL, read LICENSE for details.
 */

/* WARNING: This file has been generated (semi-)automatically!
 *          DO NOT EDIT IT UNLESS YOU KNOW WHAT YOU ARE DOING
 */

#define Public_Const 
#define ByVal
#define If if
#define Then
#define Try try {
#define Catch } catch
#define End_Try }



#define Bit0 (1 << 0)             //bitkonstanten
#define Bit1 (1 << 1)
#define Bit2 (1 << 2)
#define Bit3 (1 << 3)
#define Bit4 (1 << 4)
#define Bit5 (1 << 5)
#define Bit6 (1 << 6)
#define Bit7 (1 << 7)
#define Bit8 (1 << 8)
#define Bit9 (1 << 9)
#define Bit10 (1 << 10)
#define Bit11 (1 << 11)
#define Bit12 (1 << 12)
#define Bit13 (1 << 13)
#define Bit14 (1 << 14)
#define Bit15 (1 << 15)
#define Bit16 (1 << 16)
#define Bit17 (1 << 17)
#define Bit18 (1 << 18)
#define Bit19 (1 << 19)
#define Bit20 (1 << 20)
#define Bit21 (1 << 21)
#define Bit22 (1 << 22)
#define Bit23 (1 << 23)
#define Bit24 (1 << 24)
#define Bit25 (1 << 25)
#define Bit26 (1 << 26)
#define Bit27 (1 << 27)
#define Bit28 (1 << 28)
#define Bit29 (1 << 29)
#define Bit30 (1 << 30)
#define Bit31 (1 << 31)

#define __VDECL(a,b) b a;
#define __VDECL_A(name,len,mtype) BoxedArray<mtype,len> name;

#define String string
#define Long long
#define Integer int
#define Or ||


//Aquaero-Date Structure 
    
    enum TransferTypeRegister
{
        //Transfer protocal for writing to the Aquaero
  FLASH = 100,                     //Flash the MCU (es d�rfen nur flasch daten verschickt werden)
  LOAD_DEFAULT = 101,              //Load the default date
  EEPROM_SAVE = 102,               //Save everything to the EEPROM
  RAM = 103,                       //Move to RAM (normal write-only transfers)
  AQUABUS_READ = 104,              //Direktes lesen com aquabus
  AQUABUS_WRITE = 105,             //Schreiben auf aquabus
  TTRLCD = 106,                    //Datenausgabe in den lcd puffer, AE muss per hand in den USB-LCD mode geschlten werden
  PROFIL_LOAD = 107,               //L�dt das angegebene Profil
  PROFIL_COPY = 108,               //Kopiert aktuelles Profil in das andere
  ACTIVATE_LCDMODE = 109,          //Wecheselt sofort in LCD-Mode und gibt die daten aus
  USER1 = 110,                     //testet die maximale drehzahl vom l�fter
  FLASH_DELETE = 200               //L�scht den kompletten flash
};


    __VDECL( TransferType , TransferTypeRegister);  ////<- Protokoll-Auswahl (RAM/EEPROM/LCD/FLASH)

    __VDECL( UsbLcdTextLine1 , String);             //ist der text der auf das usb lcd ausgegeben wird
    __VDECL( UsbLcdTextLine2 , String);             //ist der text der auf das usb lcd ausgegeben wird

    //Variablen f�r Flash-Modus
    __VDECL( Flash_Adress , Long);                  //Adresse (16-bit)
    __VDECL( Flash_Data , String);                  //Daten (256 Byte)
    __VDECL( Flash_Check , Long);                   //Checksumme (2 Byte)
    __VDECL( Flash_Key , String);                   //Key zum flashen
    __VDECL( Flash_Delete_Key , String);            //Passwort f�r Flash-Delete

    //Variablen f�r das Schreiben in RAM und EEPROM
    // ########### AB HIER GLOBALE DATEN, UNABH�NGIG VOM PROFIL ###########

    __VDECL_A( Fan_Name, 4+1 , String);                 //Namen der L�fter (4 x 11 Zeichen)
    __VDECL( Flowsensor_Name , String);             //Name des Durchflussensors
    __VDECL_A( Sensor_Name, 6+1 , String);              //Namen der Temperatursensoren (6x 11 Zeichen)
    __VDECL( aquaero_Name , String);                //Name des aquaeros

    
    enum ProfilRegister 
{
  
  Profil_A = 0,
  Profil_B = 1
};


    __VDECL( Profil , ProfilRegister);              //Aktuelles Profil

    __VDECL_A( Fan_MinimumPower, 4+1 , Byte);           //Minimalleistung der L�fter als PWM-Wert. Nur bei Automatik-Betrieb.
    __VDECL_A( Fan_Pulse, 4+1 , Integer);               //Impulse/Umdrehung (>1)
    __VDECL( Flowsensor_Pulse , Integer);           //Impulse/l
    __VDECL( LCD_Backlight , Byte);                 //Backlight PWM, 0-200
    __VDECL( LCD_Contrast , Byte);                  //Contrast PWM, 155-255
    __VDECL( LCD_Fadingtime , Integer);             //Zeit bis Display ausgebelendet wird
    __VDECL_A( Sensor_Faktor, 6+1 , Byte);              //Korrekturfaktor
    __VDECL_A( Sensor_Offset, 6+1 , Byte);              //KorrekturOffset in 1/10 K
    __VDECL_A( Sensor_Alarm, 6+1 , Integer);            //Alarmwert in 1/10 K
    __VDECL( FanTestMaxRpm , Byte);                 //die bits geben an bei welchem l�fter die max rpm getestet werden soll (transfertyp USER1)

    // ########### AB HIER PROFILDATEN ###########
    
    enum LCDSettingsRegister 
{
  
  BacklightOn = Bit0,                              //Backlight immer an
  CompleteOff = Bit2,                              //LCD schaltete sich nach dimmen komplett ab
  USBLCDBoot = Bit3                               //aquaero startet im USB-LCD-Modus
};


    __VDECL( LCD_Settings , LCDSettingsRegister); //<-
    __VDECL( LCD_PageDelay , Byte); //Umschaltzeit zweischen Display-Seiten (255 = nicht schalten)

    
    enum LCDPageRegister 
{
  
  //Sewiten mit gesetzten Flags werden angezeigt
  TempOverview = Bit0,                             //Sensor�bersicht
  TempSensor_1_2 = Bit1,                           //Temperatur 1 & 2
  TempSensor_3_4 = Bit2,                           //Temperatur 3 & 4
  TempSensor_5_6 = Bit3,                           //Temperatur 5 & 6
  LCDFan_1 = Bit4,                                    //Fan 1 Details
  LCDFan_2 = Bit5,                                    //Fan 2 Details
  LCDFan_3 = Bit6,                                    //Fan 3 Details
  LCDFan_4 = Bit7,                                    //Fan 4 Details
  Aquastream_1_A = Bit8,                           //Daten Seite A aquastream 1
  Aquastream_1_B = Bit9,                           //Daten Seite B aquastream 1
  Aquastream_2_A = Bit10,                          //Daten Seite A aquastream 2
  Aquastream_2_B = Bit11,                          //Daten Seite B aquastream 2
  FlowSensor_1 = Bit12,                            //Durchflussensor 1
  FlowSensor_2 = Bit13,                            //Durchflussensor 2
  LCDtubemeter = Bit14,                               //tubemeter F�llstandssensor
  aquaero = Bit15,                                 //aquaero Hauptseite
  Powersensor_1 = Bit16,                           //Leistungsmessung 1
  Powersensor_2 = Bit17,                           //Leistungsmessung 2
  Fan_Voltage_1_2 = Bit18,                         //Spannung der L�fter 1 und 2
  Fan_Voltage_3_4 = Bit19,                         //Spannung der L�fter 1 und 2
  Fan_Overview = Bit20,                            //L�fterbersicht von l�fter 1-4
  aquaero_info1 = Bit21,                           //Informationen Hardware/Firmware Seite 1
  aquaero_info2 = Bit22                           //Informationen Hardware/Firmware Seite 2
};


    __VDECL( LCD_Page , LCDPageRegister);           //<- hier werden angezeigten displayseiten konfiguriert

    
    enum Powersensor_ConfigRegister 
{
  
  Powersensor_1_Uses_Flow_2 = Bit0,                //Wenn enabled wird Flow-Sensor 2 verwendet
  Powersensor_2_Uses_Flow_2 = Bit1                //Wenn enabled wird Flow-Sensor 2 verwendet
};


    __VDECL( Powersensor_1_Sensor , Byte);          //Konfiguration Temperatursensoren f�r Messung 1
    __VDECL( Powersensor_2_Sensor , Byte);          //Konfiguration Temperatursensoren f�r Messung 2
    __VDECL( Powersensor_Config , Powersensor_ConfigRegister); //<-

    
    enum tubemeter_ConfigRegister 
{
  
  UNUSED = 0,
  level_mm = 2, //Levelanzeige in mm 
  LED_ON = 4 //LED aktivieren/deaktivieren
};


    __VDECL( tubemeter_Config , tubemeter_ConfigRegister);
    __VDECL( tubemeter_Alarmlevel , Byte);          //Level f�r Alarm
    __VDECL( tubemeter_Alarmmove , Byte);           //Level f�r Alarm bei Bewegung
    __VDECL( tubemeter_Expansion , Byte);           //bisher unbelegt

    __VDECL_A( Fan_Sensor1, 4+1 , Byte);                 //Dummy-Attribut
    __VDECL_A( Fan_Sensor2, 4+1 , Byte);                 //Dummy-Attribut
    __VDECL_A( Fan_Sensor, 4+1 , Byte);                 //Zuordnung der Sensoren/L�fter, untere 4-Byte / obere 4-Byte
    __VDECL_A( Fan_SetRPM, 4+1 , Integer);              //Drehzahlvorgabe f�r L�fter
    __VDECL_A( Fan_SetPower, 4+1 , Byte);               //Leistung der L�fter

    
    enum Fan_ModeRegister 
{
  
  PWR_RPM_Switch = Bit0,                           //Regelung �ber Leistung oder RPM, nur bei maneller Regelung interessant
  Maual_Auto_Switch = Bit1,                        //Manuelle oder �ber Sensor gesteuerte Regelung
  Get_Max_RPM = Bit3,                              //Ermitteln der maximalen Drehzahl (wird durchgef�hrt oder soll ermittelt werden)
  Progressiv_Linear_Switch = Bit4,                 //Progressiv oder Linear regeln
  Automatik_PWM_RPM_Switch = Bit5,                 //Automatik-Betrieb mit Leistungs oder RPM-Regelung 
  Ignore_RPM = Bit6,                               //Ignorieren des Tachosignals
  Force_MinimumPower = Bit7,                       //Minimale Leistung oder Drehzahl halten
  Disable_RPM_Test = Bit8                          //Deaktivieren des Drehzahltests beim starten, Tachonutzung muss deaktiviert sein
};


    __VDECL_A( Fan_Mode, 4+1 , Fan_ModeRegister);       //<- hier stehen die jeweiligen l�ftereinstellungen drinn

    
    enum FlowSensorModeRegister 
{
  
  l_minute_hour = Bit0,                            //Liter/Minute oder Stunde (zweiter Durchflussensor)
  use_fan4_as_flowsensor = Bit1                   //L�fterkanal 4 als Flow-Sensor-Eingang verwenden (zweiter Durchflussensor)
};


    __VDECL( Flowsensor_Mode , FlowSensorModeRegister); //<-
    __VDECL_A( FlowSensor_lAlarm, 2+1 , Integer);       //Alarmgrenze in 1/10 l f�r beide Sensoren
    __VDECL_A( LED_PWM, 2+1 , Byte);                    //PWM-Wert der beiden LEDs

    
    enum LED_ModeRegister 
{
  
  Manual_Auto_Switch_LED = Bit0,                       //Manuell oder automatisch gesteuert
  Alarm_Flash = Bit1,                              //Bei Alarm blinken
  Hot_Cold = Bit2                                 //Bei steigender/fallender Temperaur heller werdend
};


    __VDECL_A( LED_Mode, 2+1 , LED_ModeRegister);       //<-hier stehen die LED einstellung drinn
    __VDECL_A( LED_Sensor, 2+1 , Byte);                 //Zuordnung der Sensoren/LEDs, untere 4-Byte / obere 4-Byte

    __VDECL_A( Sensor_Min, 6+1 , Integer);              //Minimaler Regelungswert in 1/10 K
    __VDECL_A( Sensor_Max, 6+1 , Integer);              //Maximaler Regelungswert in 1/10 K
    __VDECL_A( Sensor_Hyst, 6+1 , Byte);                //Hysterese in 1/10 K
    __VDECL_A( Sensor_Opt, 6+1 , Integer);              //Sollwert f�r Sensor in 1/10 K
    __VDECL_A( Controler_Time, 6+1 , Byte);             //Regler-Zeit
    __VDECL_A( Controler_Factor, 6+1 , Byte);           //Regler Faktor

    
    enum Alarm_SetupRegister 
{
  
  ALFan_1 = Bit0,                                    //Alarm f�r L�fter 1 aktivieren
  ALFan_2 = Bit1,                                    //Alarm f�r L�fter 2 aktivieren
  ALFan_3 = Bit2,                                    //Alarm f�r L�fter 3 aktivieren
  ALFan_4 = Bit3,                                    //Alarm f�r L�fter 4 aktivieren
  Sensor = Bit4,                                   //Alarm f�r alle Temperatursensoren
  Flowsensor_1 = Bit5,                             //Alarm f�r Durchflussensor
  Flowsensor_2 = Bit6,                             //Alarm f�r Durchflussensor
  tubemeter_level = Bit7,                          //F�llstandsalarm
  tubemeter_wave = Bit8,                           //Wellenbewegung
  ALaquastream_1 = Bit9,                             //aquastream 1
  ALaquastream_2 = Bit10,                            //aquastream 1
  Enable = Bit15                                  //Aktivierung f�r alle Alarmfunktionen, ohne Enable isst KEIN Alarm aktiv
};


    __VDECL( Alarm_Setup , Alarm_SetupRegister);    //<- hier werden die m�glichen alarme freigegeben
    __VDECL( Alarm_ShutdownDelay , Byte);           //Verz�gerung zwischen Alarm und Notabschaltung (0-180) Sekunden

    
    enum DayRegister 
{
  
  Sonday = 0,
  Monday = 1,
  Tuesday = 2,
  Wednesday = 3,
  Thursday = 4,
  Friday = 5,
  Saturday = 6,
  EveryDay = 7,
  MondayToFriday = 8,                              //Montag bis Freitag (nur f�r Timer)
  Saturday_Sunday = 9,                             //Samstag & Sonntag (nur f�r Timer)
  Never = 10                                      //Niemals (nur f�r Timer)
};


    __VDECL_A( Timer_Off_Hours, 2+1 , Byte);            //Timer ausschalten Stunden
    __VDECL_A( Timer_Off_Minutes, 2+1 , Byte);          //Timer ausschalten Minuten
    __VDECL_A( Timer_Off_Seconds, 2+1 , Byte);          //Timer ausschalten Sekunden
    __VDECL_A( Timer_Off_Day, 2+1 , DayRegister);       //Timer ausschalten Tag
    __VDECL_A( Timer_Off_SwitchTime, 2+1 , Byte);       //Schaltzeit des Relays beim ausl�sen (255->f�r immer schalten)

    __VDECL_A( Timer_On_Hours, 2+1 , Byte);             //Timer einschalten Stunden
    __VDECL_A( Timer_On_Minutes, 2+1 , Byte);           //Timer einschalten Minuten
    __VDECL_A( Timer_On_Seconds, 2+1 , Byte);           //Timer einschalten Sekunden
    __VDECL_A( Timer_On_Day, 2+1 , DayRegister);        //Timer einschalten Tag
    __VDECL_A( Timer_On_SwitchTime, 2+1 , Byte);        //Schaltzeit des Relays beim ausl�sen (255->f�r immer schalten)

    
    enum aquastream_CommandRegister { 
      Set_Default = 41,                                //Default-Werte setzen
      Anti_Air = 75,                                   //Entl�ftung
      Set_Frequency = 150                             //Frequenz setzen
    };


    __VDECL_A( aquastream_Command, 2+1 , aquastream_CommandRegister);

    __VDECL( Relay_ShutdownTime , Byte);            //Bei Nutzung als PWR-Switch, Zeit f�r die der Schalter ausgel�st werden soll (255 = Immer)

    
    enum Relay_StatusRegister 
{
  
  Relay_On_Off = Bit0,                             //Relais an/aus
  Relay_Usage_ATXBREAK_PWR = Bit1                 //Benutzung als ATX oder Power-Schalter
};


    __VDECL( Relay_Status , Relay_StatusRegister);

    __VDECL_A( multiswitch_AMP_Register, 2+1 , Byte);   //Endstufen-Ausg�nge multisiwtch
    __VDECL_A( multiswitch_LED_Register, 2+1 , Byte);   //LED-Ausg�nge multiswitch

    
    enum multiswitch_BargraphRegister 
{
  
  Bargraph_Fan_1_RPM = 0,                          //Bargraph nach Drehzahl L�fter 1-4
  Bargraph_Fan_2_RPM = 1,
  Bargraph_Fan_3_RPM = 2,
  Bargraph_Fan_4_RPM = 3,
  Bargraph_Fan_1_PWR = 4,                          //Bargraph nach Leitung L�fter 1-4
  Bargraph_Fan_2_PWR = 5,
  Bargraph_Fan_3_PWR = 6,
  Bargraph_Fan_4_PWR = 7,
  Bargraph_Sensor_1 = 8,                           //Bargraph zwischen Min und Max Temperatur 1-6
  Bargraph_Sensor_2 = 9,
  Bargraph_Sensor_3 = 10,
  Bargraph_Sensor_4 = 11,
  Bargraph_Sensor_5 = 12,
  Bargraph_Sensor_6 = 13,
  Bargraph_Off = 14 //Bargraph 
};


    __VDECL_A( multiswitch_Bargraph_AMP, 2+1 , multiswitch_BargraphRegister);
    __VDECL_A( multiswitch_Bargraph_LED, 2+1 , multiswitch_BargraphRegister);

    
    enum multiswitch_StatusRegister 
{
  
  Relais_On = Bit0                                //Relais vom Multiswitch On/Off
};


    __VDECL_A( multiswitch_Status, 2+1 , multiswitch_StatusRegister);

    __VDECL( Expansion_RW , String);                //Freier Expansion RW-Bereich
    __VDECL( aquabus_Buffer , String);              //aquabusBuffer-Bereich

    __VDECL( Time_Hours , Byte);                    //Aktuelle Zeit, Stunden
    __VDECL( Time_Minutes , Byte);                  //Aktuelle Zeit, Minuten
    __VDECL( Time_Seconds , Byte);                  //Aktuelle Zeit, Sekunden
    __VDECL( Time_Day , DayRegister);               //Aktueller Tag

    
    enum aquaero_ControlRegister 
{
  Default = 0,
  Keylock = 1,                                     //Keylock
  EEPROM_Reset = 2                                //EEPROM reseten
};


    __VDECL( aquaero_Control , aquaero_ControlRegister); //Keylock usw.

    
    enum aquaero_ScreenRegister 
{
  
  Setup = 0,                                       //wenn man in einem Aquaero men� ist=0
  USBLCD = 255                                    //bei usblcd screen= 255
};


    __VDECL( aquaero_Screen , aquaero_ScreenRegister); //Aktueller Screen des aquaeros

    // ########### AB HIER GLOBALE READ-ONLY DATEN ###########
    
    enum aquabus_StatusRegister 
{
  
  Fail = Bit0,                                     //�bertragungsfehler
  Busy = Bit1,                                     //Bus belegt
  Reading = Bit2,                                  //Es wird gelesen
  Writing = Bit3,                                  //Es wird geschrieben
  NewData = Bit4,                                  //Neue Daten sethen nach dem Lesen zur Verf�gung
  EEPROM_Busy = Bit5                              //Daten werden gesichert oder geladen
};


    __VDECL( aquabus_Status , aquabus_StatusRegister);

    
    enum aquabus_DeviceRegister 
{
  
  ABaquastream_1 = Bit0,                             //Angeschlossene Ger�te Aquastream 1
  ABaquastream_2 = Bit1,                             //Angeschlossene Ger�te Aquastream 2
  ABtubemeter = Bit3,                                //Angeschlossene Ger�te tubemeter
  multiswitch_1 = Bit4,                            //Angeschlossene Ger�te multiswitch1
  multiswitch_2 = Bit5                            //Angeschlossene Ger�te multiswitch2
};


    __VDECL( aquabus_Devices , aquabus_DeviceRegister); //Gibt an welche Ger�te am aquabus vorhanden sind

    //__VDECL( multiswitch_OutputData[2+1][3+1], Byte);  //Daten die vom aquaero an den ms gesendet werden
    BoxedArray<BoxedArray<Byte,3+1>,2+1> multiswitch_OutputData; //Daten die vom aquaero an den ms gesendet werden

    __VDECL( tubemeter_rawlevel , Integer);         //Rohdaten
    __VDECL( tubemeter_wavelevel , Byte);           //Wellenbewegung
    __VDECL( tubemeter_level_mm , Byte);            //Level in mm
    __VDECL( tubemeter_level_avg , Byte);           //Durchschnittliches Level

    __VDECL( aquastream_1_RawData , String);        //16 Byte RAW-Data
    __VDECL( aquastream_1_Current , Integer);       //Strom
    __VDECL( aquastream_1_Voltage , Integer);       //Spannung
    __VDECL( aquastream_1_Power , Long);            //Leistung

    __VDECL( aquastream_2_RawData , String);        //16 Byte RAW-Data
    __VDECL( aquastream_2_Current , Integer);       //Strom
    __VDECL( aquastream_2_Voltage , Integer);       //Spannung
    __VDECL( aquastream_2_Power , Long);            //Leistung

    
    enum USBLCD_ModeRegister 
{
  
  USBLCD_Active = Bit0,                            //USBLCD-Modus ist betriebsbereit
  USBLCD_NewData = Bit1,                           //Neue Daten f�r USB-LCD vorhanden
  USBLCD_Flashing = Bit2,                          //aquaero wird gerade geflasht
  USBLCD_ConnectionPage = Bit3                    //Display-Seite mit USB-Verbdindung wird angezeigt
};


    __VDECL( USBLCD_Mode , USBLCD_ModeRegister);

    __VDECL_A( Powersensor_Power, 2+1 , Integer);       //Gemessene Leistung bei Leistungsmessung
    __VDECL_A( Fan_RPM, 4+1 , Integer);                 //Aktuelle Drehzahl der L�fter 
    __VDECL( Flowsensor_l , Integer);               //Durchfluss
    __VDECL_A( Fan_MaxRPM, 4+1 , Integer);              //Maximale Drehzahl der L�fter
    __VDECL_A( Sensor_Temp, 6+1 , Integer);             //Aktuelle Temperaturen
    __VDECL( Alarm_Status , Alarm_SetupRegister);   //Aktueller Alarm wird ausgel�st von...
    __VDECL( Alarm_Memory_Pointer , Byte);          //Pointer auf den letzten Alarm im Ringpuffer
    __VDECL_A( Alarm_Memory_Typ, 5+1 , Alarm_SetupRegister); //Alarmtyp
    __VDECL_A( Alarm_Memory_Sensor, 5+1 , Byte);        //Nummer des Sensors, der Alarm ausgegeben hat
    __VDECL_A( Alarm_Memory_Hours, 5+1 , Byte);         //Alarmzeit, Stunden
    __VDECL_A( Alarm_Memory_Minutes, 5+1 , Byte);       //Alarmzeit, Minuten
    __VDECL_A( Alarm_Memory_Day, 5+1 , DayRegister);    //Alarmzeit, Sekunden
    __VDECL( Firmware_Name , String);               //Firmware-Revision (5 Zeichen)

    
    enum LCD_TypeRegister 
{
  
  LCD = 0,
  VFD = 1,
  OLED = 2,
  NODISPLAY = 255
};


    __VDECL( LCD_Type , LCD_TypeRegister); //Displaytyp
    __VDECL( OS_Version , Integer);                 //OS-Version
    __VDECL( FirmwareRevision , Integer);           //Firmware-Version
    __VDECL( Flashcounter , Integer);               //Flashcounter
    __VDECL( HardwareRevision , Integer);           //Hardware-Version
    __VDECL( Serialnumber , Integer);               //Seriennummer
    __VDECL( ProductionMonth , Byte);               //Monat der Produktion
    __VDECL( ProductionYear , Integer);                //Jahr der Produktion
    __VDECL( Language , String);                    //Sprache
    __VDECL( Expansion_RO , String);                //Expansion-Bereich RO





/*Function CreateStructureFromString(ByVal RawData As String) As Boolean*/
void CreateStructureFromString ( string RawData ) 
{
  
        int myPositionCounter = 0; //Aktueller Positions-Zeiger

        //Struktur aufbauen
        //Globale Daten
        Fan_Name[1] = GetTextString(RawData, myPositionCounter, 11);
        Fan_Name[2] = GetTextString(RawData, myPositionCounter, 11);
        Fan_Name[3] = GetTextString(RawData, myPositionCounter, 11);
        Fan_Name[4] = GetTextString(RawData, myPositionCounter, 11);
        Flowsensor_Name = GetTextString(RawData, myPositionCounter, 11);
;
        Sensor_Name[1] = GetTextString(RawData, myPositionCounter, 11);
        Sensor_Name[2] = GetTextString(RawData, myPositionCounter, 11);
        Sensor_Name[3] = GetTextString(RawData, myPositionCounter, 11);
        Sensor_Name[4] = GetTextString(RawData, myPositionCounter, 11);
        Sensor_Name[5] = GetTextString(RawData, myPositionCounter, 11);
        Sensor_Name[6] = GetTextString(RawData, myPositionCounter, 11);
;
        aquaero_Name = GetTextString(RawData, myPositionCounter, 9);
;
        Profil = (ProfilRegister)GetByte(RawData, myPositionCounter);
;
        Fan_MinimumPower[1] = GetByte(RawData, myPositionCounter);
        Fan_MinimumPower[2] = GetByte(RawData, myPositionCounter);
        Fan_MinimumPower[3] = GetByte(RawData, myPositionCounter);
        Fan_MinimumPower[4] = GetByte(RawData, myPositionCounter);
;
        Fan_Pulse[1] = GetInteger(RawData, myPositionCounter);
        Fan_Pulse[2] = GetInteger(RawData, myPositionCounter);
        Fan_Pulse[3] = GetInteger(RawData, myPositionCounter);
        Fan_Pulse[4] = GetInteger(RawData, myPositionCounter);
;
        Flowsensor_Pulse = GetInteger(RawData, myPositionCounter);
;
        LCD_Backlight = GetByte(RawData, myPositionCounter);
        LCD_Contrast = GetByte(RawData, myPositionCounter);
        LCD_Fadingtime = GetInteger(RawData, myPositionCounter);
;
        Sensor_Faktor[1] = GetByte(RawData, myPositionCounter);
        Sensor_Faktor[2] = GetByte(RawData, myPositionCounter);
        Sensor_Faktor[3] = GetByte(RawData, myPositionCounter);
        Sensor_Faktor[4] = GetByte(RawData, myPositionCounter);
        Sensor_Faktor[5] = GetByte(RawData, myPositionCounter);
        Sensor_Faktor[6] = GetByte(RawData, myPositionCounter);
;
        Sensor_Offset[1] = GetByte(RawData, myPositionCounter);
        Sensor_Offset[2] = GetByte(RawData, myPositionCounter);
        Sensor_Offset[3] = GetByte(RawData, myPositionCounter);
        Sensor_Offset[4] = GetByte(RawData, myPositionCounter);
        Sensor_Offset[5] = GetByte(RawData, myPositionCounter);
        Sensor_Offset[6] = GetByte(RawData, myPositionCounter);
;
        Sensor_Alarm[1] = GetInteger(RawData, myPositionCounter);
        Sensor_Alarm[2] = GetInteger(RawData, myPositionCounter);
        Sensor_Alarm[3] = GetInteger(RawData, myPositionCounter);
        Sensor_Alarm[4] = GetInteger(RawData, myPositionCounter);
        Sensor_Alarm[5] = GetInteger(RawData, myPositionCounter);
        Sensor_Alarm[6] = GetInteger(RawData, myPositionCounter);
;
        //Ab hier Profildaten ;
        LCD_Settings = (LCDSettingsRegister)GetByte(RawData, myPositionCounter);
        LCD_PageDelay = GetByte(RawData, myPositionCounter);
        LCD_Page = (LCDPageRegister)GetLong(RawData, myPositionCounter);
;
        Powersensor_1_Sensor = GetByte(RawData, myPositionCounter);
        Powersensor_2_Sensor = GetByte(RawData, myPositionCounter);
        Powersensor_Config = (Powersensor_ConfigRegister)GetByte(RawData, myPositionCounter);
;
        tubemeter_Config = (tubemeter_ConfigRegister)GetByte(RawData, myPositionCounter);
        tubemeter_Alarmlevel = GetByte(RawData, myPositionCounter);
        tubemeter_Alarmmove = GetByte(RawData, myPositionCounter);
        tubemeter_Expansion = GetByte(RawData, myPositionCounter);
;
        Fan_Sensor[1] = GetByte(RawData, myPositionCounter);
        Fan_Sensor[2] = GetByte(RawData, myPositionCounter);
        Fan_Sensor[3] = GetByte(RawData, myPositionCounter);
        Fan_Sensor[4] = GetByte(RawData, myPositionCounter);
;
        Fan_SetRPM[1] = GetInteger(RawData, myPositionCounter);
        Fan_SetRPM[2] = GetInteger(RawData, myPositionCounter);
        Fan_SetRPM[3] = GetInteger(RawData, myPositionCounter);
        Fan_SetRPM[4] = GetInteger(RawData, myPositionCounter);
;
        Fan_SetPower[1] = GetByte(RawData, myPositionCounter);
        Fan_SetPower[2] = GetByte(RawData, myPositionCounter);
        Fan_SetPower[3] = GetByte(RawData, myPositionCounter);
        Fan_SetPower[4] = GetByte(RawData, myPositionCounter);
;
        Fan_Mode[1] = (Fan_ModeRegister)GetInteger(RawData, myPositionCounter);
        Fan_Mode[2] = (Fan_ModeRegister)GetInteger(RawData, myPositionCounter);
        Fan_Mode[3] = (Fan_ModeRegister)GetInteger(RawData, myPositionCounter);
        Fan_Mode[4] = (Fan_ModeRegister)GetInteger(RawData, myPositionCounter);
;
        Flowsensor_Mode = (FlowSensorModeRegister)GetInteger(RawData, myPositionCounter);
        FlowSensor_lAlarm[1] = GetInteger(RawData, myPositionCounter);
        FlowSensor_lAlarm[2] = GetInteger(RawData, myPositionCounter);
;
        LED_PWM[1] = GetByte(RawData, myPositionCounter);
        LED_PWM[2] = GetByte(RawData, myPositionCounter);
        LED_Mode[1] = (LED_ModeRegister)GetByte(RawData, myPositionCounter);
        LED_Mode[2] = (LED_ModeRegister)GetByte(RawData, myPositionCounter);
        LED_Sensor[1] = GetByte(RawData, myPositionCounter);
        LED_Sensor[2] = GetByte(RawData, myPositionCounter);
;
        Sensor_Min[1] = GetInteger(RawData, myPositionCounter);
        Sensor_Min[2] = GetInteger(RawData, myPositionCounter);
        Sensor_Min[3] = GetInteger(RawData, myPositionCounter);
        Sensor_Min[4] = GetInteger(RawData, myPositionCounter);
        Sensor_Min[5] = GetInteger(RawData, myPositionCounter);
        Sensor_Min[6] = GetInteger(RawData, myPositionCounter);
;
        Sensor_Max[1] = GetInteger(RawData, myPositionCounter);
        Sensor_Max[2] = GetInteger(RawData, myPositionCounter);
        Sensor_Max[3] = GetInteger(RawData, myPositionCounter);
        Sensor_Max[4] = GetInteger(RawData, myPositionCounter);
        Sensor_Max[5] = GetInteger(RawData, myPositionCounter);
        Sensor_Max[6] = GetInteger(RawData, myPositionCounter);
;
        Sensor_Hyst[1] = GetByte(RawData, myPositionCounter);
        Sensor_Hyst[2] = GetByte(RawData, myPositionCounter);
        Sensor_Hyst[3] = GetByte(RawData, myPositionCounter);
        Sensor_Hyst[4] = GetByte(RawData, myPositionCounter);
        Sensor_Hyst[5] = GetByte(RawData, myPositionCounter);
        Sensor_Hyst[6] = GetByte(RawData, myPositionCounter);
;
        Sensor_Opt[1] = GetInteger(RawData, myPositionCounter);
        Sensor_Opt[2] = GetInteger(RawData, myPositionCounter);
        Sensor_Opt[3] = GetInteger(RawData, myPositionCounter);
        Sensor_Opt[4] = GetInteger(RawData, myPositionCounter);
        Sensor_Opt[5] = GetInteger(RawData, myPositionCounter);
        Sensor_Opt[6] = GetInteger(RawData, myPositionCounter);
;
        Controler_Time[1] = GetByte(RawData, myPositionCounter);
        Controler_Time[2] = GetByte(RawData, myPositionCounter);
        Controler_Time[3] = GetByte(RawData, myPositionCounter);
        Controler_Time[4] = GetByte(RawData, myPositionCounter);
        Controler_Time[5] = GetByte(RawData, myPositionCounter);
        Controler_Time[6] = GetByte(RawData, myPositionCounter);
;
        Controler_Factor[1] = GetByte(RawData, myPositionCounter);
        Controler_Factor[2] = GetByte(RawData, myPositionCounter);
        Controler_Factor[3] = GetByte(RawData, myPositionCounter);
        Controler_Factor[4] = GetByte(RawData, myPositionCounter);
        Controler_Factor[5] = GetByte(RawData, myPositionCounter);
        Controler_Factor[6] = GetByte(RawData, myPositionCounter);
;
        Alarm_Setup = (Alarm_SetupRegister)GetInteger(RawData, myPositionCounter);
        Alarm_ShutdownDelay = GetByte(RawData, myPositionCounter);
;
        Timer_Off_Hours[1] = GetByte(RawData, myPositionCounter);
        Timer_Off_Minutes[1] = GetByte(RawData, myPositionCounter);
        Timer_Off_Seconds[1] = GetByte(RawData, myPositionCounter);
        Timer_Off_Day[1] = (DayRegister)GetByte(RawData, myPositionCounter);
        Timer_Off_SwitchTime[1] = GetByte(RawData, myPositionCounter);
;
        Timer_Off_Hours[2] = GetByte(RawData, myPositionCounter);
        Timer_Off_Minutes[2] = GetByte(RawData, myPositionCounter);
        Timer_Off_Seconds[2] = GetByte(RawData, myPositionCounter);
        Timer_Off_Day[2] = (DayRegister)GetByte(RawData, myPositionCounter);
        Timer_Off_SwitchTime[2] = GetByte(RawData, myPositionCounter);
;
        Timer_On_Hours[1] = GetByte(RawData, myPositionCounter);
        Timer_On_Minutes[1] = GetByte(RawData, myPositionCounter);
        Timer_On_Seconds[1] = GetByte(RawData, myPositionCounter);
        Timer_On_Day[1] = (DayRegister)GetByte(RawData, myPositionCounter);
        Timer_On_SwitchTime[1] = GetByte(RawData, myPositionCounter);
;
        Timer_On_Hours[2] = GetByte(RawData, myPositionCounter);
        Timer_On_Minutes[2] = GetByte(RawData, myPositionCounter);
        Timer_On_Seconds[2] = GetByte(RawData, myPositionCounter);
        Timer_On_Day[2] = (DayRegister)GetByte(RawData, myPositionCounter);
        Timer_On_SwitchTime[2] = GetByte(RawData, myPositionCounter);
;
        aquastream_Command[1] = (aquastream_CommandRegister)GetByte(RawData, myPositionCounter);
        aquastream_Command[2] = (aquastream_CommandRegister)GetByte(RawData, myPositionCounter);
;
        Relay_ShutdownTime = GetByte(RawData, myPositionCounter);
        Relay_Status = (Relay_StatusRegister)GetByte(RawData, myPositionCounter);
;
        multiswitch_AMP_Register[1] = GetByte(RawData, myPositionCounter);
        multiswitch_LED_Register[1] = GetByte(RawData, myPositionCounter);
        multiswitch_AMP_Register[2] = GetByte(RawData, myPositionCounter);
        multiswitch_LED_Register[2] = GetByte(RawData, myPositionCounter);
;
        multiswitch_Bargraph_AMP[1] = (multiswitch_BargraphRegister)GetByte(RawData, myPositionCounter);
        multiswitch_Bargraph_LED[1] = (multiswitch_BargraphRegister)GetByte(RawData, myPositionCounter);
        multiswitch_Status[1] = (multiswitch_StatusRegister)GetByte(RawData, myPositionCounter);
        multiswitch_Bargraph_AMP[2] = (multiswitch_BargraphRegister)GetByte(RawData, myPositionCounter);
        multiswitch_Bargraph_LED[2] = (multiswitch_BargraphRegister)GetByte(RawData, myPositionCounter);
        multiswitch_Status[2] = (multiswitch_StatusRegister)GetByte(RawData, myPositionCounter);
;
        //Ende Profil;
        Expansion_RW = GetString(RawData, myPositionCounter, 32);
        aquabus_Buffer = GetString(RawData, myPositionCounter, 24);
;
        Time_Hours = GetByte(RawData, myPositionCounter);
        Time_Minutes = GetByte(RawData, myPositionCounter);
        Time_Seconds = GetByte(RawData, myPositionCounter);
        Time_Day = (DayRegister)GetByte(RawData, myPositionCounter);
;
        aquaero_Control = (aquaero_ControlRegister)GetByte(RawData, myPositionCounter);
        aquaero_Screen = (aquaero_ScreenRegister)GetByte(RawData, myPositionCounter);
;
        //Read-Only-Daten;
        aquabus_Status = (aquabus_StatusRegister)GetByte(RawData, myPositionCounter);
        aquabus_Devices = (aquabus_DeviceRegister)GetInteger(RawData, myPositionCounter);
;
        multiswitch_OutputData[1][ 1] = GetByte(RawData, myPositionCounter);
        multiswitch_OutputData[1][ 2] = GetByte(RawData, myPositionCounter);
        multiswitch_OutputData[1][ 3] = GetByte(RawData, myPositionCounter);
        multiswitch_OutputData[2][ 1] = GetByte(RawData, myPositionCounter);
        multiswitch_OutputData[2][ 2] = GetByte(RawData, myPositionCounter);
        multiswitch_OutputData[2][ 3] = GetByte(RawData, myPositionCounter);
;
        tubemeter_rawlevel = GetInteger(RawData, myPositionCounter);
        tubemeter_wavelevel = GetByte(RawData, myPositionCounter);
        tubemeter_level_mm = GetByte(RawData, myPositionCounter);
        tubemeter_level_avg = GetByte(RawData, myPositionCounter);
;
        aquastream_1_RawData = GetString(RawData, myPositionCounter, 16);
        aquastream_1_Current = GetInteger(RawData, myPositionCounter);
        aquastream_1_Voltage = GetInteger(RawData, myPositionCounter);
        aquastream_1_Power = GetLong(RawData, myPositionCounter);
;
        aquastream_2_RawData = GetString(RawData, myPositionCounter, 16);
        aquastream_2_Current = GetInteger(RawData, myPositionCounter);
        aquastream_2_Voltage = GetInteger(RawData, myPositionCounter);
        aquastream_2_Power = GetLong(RawData, myPositionCounter);
;
        USBLCD_Mode = (USBLCD_ModeRegister)GetByte(RawData, myPositionCounter);
;
        Powersensor_Power[1] = GetInteger(RawData, myPositionCounter);
        Powersensor_Power[2] = GetInteger(RawData, myPositionCounter);
;
        Fan_RPM[1] = GetInteger(RawData, myPositionCounter);
        Fan_RPM[2] = GetInteger(RawData, myPositionCounter);
        Fan_RPM[3] = GetInteger(RawData, myPositionCounter);
        Fan_RPM[4] = GetInteger(RawData, myPositionCounter);
;
        Flowsensor_l = GetInteger(RawData, myPositionCounter);
;
        Fan_MaxRPM[1] = GetInteger(RawData, myPositionCounter);
        Fan_MaxRPM[2] = GetInteger(RawData, myPositionCounter);
        Fan_MaxRPM[3] = GetInteger(RawData, myPositionCounter);
        Fan_MaxRPM[4] = GetInteger(RawData, myPositionCounter);
;
        Sensor_Temp[1] = GetInteger(RawData, myPositionCounter);
        Sensor_Temp[2] = GetInteger(RawData, myPositionCounter);
        Sensor_Temp[3] = GetInteger(RawData, myPositionCounter);
        Sensor_Temp[4] = GetInteger(RawData, myPositionCounter);
        Sensor_Temp[5] = GetInteger(RawData, myPositionCounter);
        Sensor_Temp[6] = GetInteger(RawData, myPositionCounter);
;
        Alarm_Status = (Alarm_SetupRegister)GetInteger(RawData, myPositionCounter);
        Alarm_Memory_Pointer = GetByte(RawData, myPositionCounter);
;
        Alarm_Memory_Typ[1] = (Alarm_SetupRegister)GetInteger(RawData, myPositionCounter);
        Alarm_Memory_Sensor[1] = GetByte(RawData, myPositionCounter);
        Alarm_Memory_Hours[1] = GetByte(RawData, myPositionCounter);
        Alarm_Memory_Minutes[1] = GetByte(RawData, myPositionCounter);
        Alarm_Memory_Day[1] = (DayRegister) GetByte(RawData, myPositionCounter);
;
        Alarm_Memory_Typ[2] = (Alarm_SetupRegister) GetInteger(RawData, myPositionCounter);
        Alarm_Memory_Sensor[2] = GetByte(RawData, myPositionCounter);
        Alarm_Memory_Hours[2] = GetByte(RawData, myPositionCounter);
        Alarm_Memory_Minutes[2] = GetByte(RawData, myPositionCounter);
        Alarm_Memory_Day[2] = (DayRegister) GetByte(RawData, myPositionCounter);
;
        Alarm_Memory_Typ[3] = (Alarm_SetupRegister) GetInteger(RawData, myPositionCounter);
        Alarm_Memory_Sensor[3] = GetByte(RawData, myPositionCounter);
        Alarm_Memory_Hours[3] = GetByte(RawData, myPositionCounter);
        Alarm_Memory_Minutes[3] = GetByte(RawData, myPositionCounter);
        Alarm_Memory_Day[3] = (DayRegister) GetByte(RawData, myPositionCounter);
;
        Alarm_Memory_Typ[4] = (Alarm_SetupRegister) GetInteger(RawData, myPositionCounter);
        Alarm_Memory_Sensor[4] = GetByte(RawData, myPositionCounter);
        Alarm_Memory_Hours[4] = GetByte(RawData, myPositionCounter);
        Alarm_Memory_Minutes[4] = GetByte(RawData, myPositionCounter);
        Alarm_Memory_Day[4] = (DayRegister) GetByte(RawData, myPositionCounter);
;
        Alarm_Memory_Typ[5] = (Alarm_SetupRegister) GetInteger(RawData, myPositionCounter);
        Alarm_Memory_Sensor[5] = GetByte(RawData, myPositionCounter);
        Alarm_Memory_Hours[5] = GetByte(RawData, myPositionCounter);
        Alarm_Memory_Minutes[5] = GetByte(RawData, myPositionCounter);
        Alarm_Memory_Day[5] = (DayRegister) GetByte(RawData, myPositionCounter);
;
        Firmware_Name = GetString(RawData, myPositionCounter, 6);
;
        LCD_Type = (LCD_TypeRegister) GetByte(RawData, myPositionCounter);
        OS_Version = GetInteger(RawData, myPositionCounter);
        FirmwareRevision = GetInteger(RawData, myPositionCounter);
        Flashcounter = GetInteger(RawData, myPositionCounter);
        HardwareRevision = GetInteger(RawData, myPositionCounter);
        Serialnumber = GetInteger(RawData, myPositionCounter);
        ProductionMonth = GetByte(RawData, myPositionCounter);
        ProductionYear = GetByte(RawData, myPositionCounter);
        Language = GetString(RawData, myPositionCounter, 4);
        Expansion_RO = GetString(RawData, myPositionCounter, 24);

    }

#if 1

string CreateStringFromStructure(/*const TransferTypeRegister Protokol*/)
{
  string RawData = "";
  
  const TransferTypeRegister Protokol = RAM;
  
  AddByte((byte)Protokol, RawData);
  
  
  //Daten in RAM oder EEPROM schreiben
  if( Protokol == /*TransferTypeRegister.*/RAM Or 
      Protokol == /*TransferTypeRegister.*/EEPROM_SAVE Or 
      Protokol == /*TransferTypeRegister.*/PROFIL_LOAD Or 
      Protokol == /*TransferTypeRegister.*/PROFIL_COPY Or 
      Protokol == /*TransferTypeRegister.*/AQUABUS_READ Or 
      Protokol == /*TransferTypeRegister.*/AQUABUS_WRITE )
    {
      //Struktur aufbauen
      //Globale Daten
      AddTextString(Fan_Name[1], 11, RawData);
      AddTextString(Fan_Name[2], 11, RawData);
      AddTextString(Fan_Name[3], 11, RawData);
      AddTextString(Fan_Name[4], 11, RawData);
      ;
      AddTextString(Flowsensor_Name, 11, RawData);
      ;
      AddTextString(Sensor_Name[1], 11, RawData);
      AddTextString(Sensor_Name[2], 11, RawData);
      AddTextString(Sensor_Name[3], 11, RawData);
      AddTextString(Sensor_Name[4], 11, RawData);
      AddTextString(Sensor_Name[5], 11, RawData);
      AddTextString(Sensor_Name[6], 11, RawData);
      ;
      AddTextString(aquaero_Name, 9, RawData);
      ;
      AddByte(Profil, RawData);
      ;
      AddByte(Fan_MinimumPower[1], RawData);
      AddByte(Fan_MinimumPower[2], RawData);
      AddByte(Fan_MinimumPower[3], RawData);
      AddByte(Fan_MinimumPower[4], RawData);
      ;
      AddInteger(Fan_Pulse[1], RawData);
      AddInteger(Fan_Pulse[2], RawData);
      AddInteger(Fan_Pulse[3], RawData);
      AddInteger(Fan_Pulse[4], RawData);
      ;
      AddInteger(Flowsensor_Pulse, RawData);
      ;
      AddByte(LCD_Backlight, RawData);
      AddByte(LCD_Contrast, RawData);
      AddInteger(LCD_Fadingtime, RawData);
      ;
      AddByte(Sensor_Faktor[1], RawData);
      AddByte(Sensor_Faktor[2], RawData);
      AddByte(Sensor_Faktor[3], RawData);
      AddByte(Sensor_Faktor[4], RawData);
      AddByte(Sensor_Faktor[5], RawData);
      AddByte(Sensor_Faktor[6], RawData);
      ;
      AddByte(Sensor_Offset[1], RawData);
      AddByte(Sensor_Offset[2], RawData);
      AddByte(Sensor_Offset[3], RawData);
      AddByte(Sensor_Offset[4], RawData);
      AddByte(Sensor_Offset[5], RawData);
      AddByte(Sensor_Offset[6], RawData);
      ;
      AddInteger(Sensor_Alarm[1], RawData);
      AddInteger(Sensor_Alarm[2], RawData);
      AddInteger(Sensor_Alarm[3], RawData);
      AddInteger(Sensor_Alarm[4], RawData);
      AddInteger(Sensor_Alarm[5], RawData);
      AddInteger(Sensor_Alarm[6], RawData);
      ;
      //Ab hier Profildaten ;
      AddByte(LCD_Settings, RawData);
      AddByte(LCD_PageDelay, RawData);
      AddLong(LCD_Page, RawData);
      ;
      AddByte(Powersensor_1_Sensor, RawData);
      AddByte(Powersensor_2_Sensor, RawData);
      AddByte(Powersensor_Config, RawData);
      ;
      AddByte(tubemeter_Config, RawData);
      AddByte(tubemeter_Alarmlevel, RawData);
      AddByte(tubemeter_Alarmmove, RawData);
      AddByte(tubemeter_Expansion, RawData);
      ;
      AddByte(Fan_Sensor[1], RawData);
      AddByte(Fan_Sensor[2], RawData);
      AddByte(Fan_Sensor[3], RawData);
      AddByte(Fan_Sensor[4], RawData);
      ;
      AddInteger(Fan_SetRPM[1], RawData);
      AddInteger(Fan_SetRPM[2], RawData);
      AddInteger(Fan_SetRPM[3], RawData);
      AddInteger(Fan_SetRPM[4], RawData);
      ;
      AddByte(Fan_SetPower[1], RawData);
      AddByte(Fan_SetPower[2], RawData);
      AddByte(Fan_SetPower[3], RawData);
      AddByte(Fan_SetPower[4], RawData);
      ;
      AddInteger(Fan_Mode[1], RawData);
      AddInteger(Fan_Mode[2], RawData);
      AddInteger(Fan_Mode[3], RawData);
      AddInteger(Fan_Mode[4], RawData);
      ;
      AddInteger(Flowsensor_Mode, RawData);
      AddInteger(FlowSensor_lAlarm[1], RawData);
      AddInteger(FlowSensor_lAlarm[2], RawData);
      ;
      AddByte(LED_PWM[1], RawData);
      AddByte(LED_PWM[2], RawData);
      AddByte(LED_Mode[1], RawData);
      AddByte(LED_Mode[2], RawData);
      AddByte(LED_Sensor[1], RawData);
      AddByte(LED_Sensor[2], RawData);
      ;
      AddInteger(Sensor_Min[1], RawData);
      AddInteger(Sensor_Min[2], RawData);
      AddInteger(Sensor_Min[3], RawData);
      AddInteger(Sensor_Min[4], RawData);
      AddInteger(Sensor_Min[5], RawData);
      AddInteger(Sensor_Min[6], RawData);
      ;
      AddInteger(Sensor_Max[1], RawData);
      AddInteger(Sensor_Max[2], RawData);
      AddInteger(Sensor_Max[3], RawData);
      AddInteger(Sensor_Max[4], RawData);
      AddInteger(Sensor_Max[5], RawData);
      AddInteger(Sensor_Max[6], RawData);
      ;
      AddByte(Sensor_Hyst[1], RawData);
      AddByte(Sensor_Hyst[2], RawData);
      AddByte(Sensor_Hyst[3], RawData);
      AddByte(Sensor_Hyst[4], RawData);
      AddByte(Sensor_Hyst[5], RawData);
      AddByte(Sensor_Hyst[6], RawData);
      ;
      AddInteger(Sensor_Opt[1], RawData);
      AddInteger(Sensor_Opt[2], RawData);
      AddInteger(Sensor_Opt[3], RawData);
      AddInteger(Sensor_Opt[4], RawData);
      AddInteger(Sensor_Opt[5], RawData);
      AddInteger(Sensor_Opt[6], RawData);
      ;
      AddByte(Controler_Time[1], RawData);
      AddByte(Controler_Time[2], RawData);
      AddByte(Controler_Time[3], RawData);
      AddByte(Controler_Time[4], RawData);
      AddByte(Controler_Time[5], RawData);
      AddByte(Controler_Time[6], RawData);
      ;
      AddByte(Controler_Factor[1], RawData);
      AddByte(Controler_Factor[2], RawData);
      AddByte(Controler_Factor[3], RawData);
      AddByte(Controler_Factor[4], RawData);
      AddByte(Controler_Factor[5], RawData);
      AddByte(Controler_Factor[6], RawData);
      ;
      AddInteger(Alarm_Setup, RawData);
      AddByte(Alarm_ShutdownDelay, RawData);
      ;
      AddByte(Timer_Off_Hours[1], RawData);
      AddByte(Timer_Off_Minutes[1], RawData);
      AddByte(Timer_Off_Seconds[1], RawData);
      AddByte(Timer_Off_Day[1], RawData);
      AddByte(Timer_Off_SwitchTime[1], RawData);
      ;
      AddByte(Timer_Off_Hours[2], RawData);
      AddByte(Timer_Off_Minutes[2], RawData);
      AddByte(Timer_Off_Seconds[2], RawData);
      AddByte(Timer_Off_Day[2], RawData);
      AddByte(Timer_Off_SwitchTime[2], RawData);
      ;
      AddByte(Timer_On_Hours[1], RawData);
      AddByte(Timer_On_Minutes[1], RawData);
      AddByte(Timer_On_Seconds[1], RawData);
      AddByte(Timer_On_Day[1], RawData);
      AddByte(Timer_On_SwitchTime[1], RawData);
      ;
      AddByte(Timer_On_Hours[2], RawData);
      AddByte(Timer_On_Minutes[2], RawData);
      AddByte(Timer_On_Seconds[2], RawData);
      AddByte(Timer_On_Day[2], RawData);
      AddByte(Timer_On_SwitchTime[2], RawData);
      ;
      AddByte(aquastream_Command[1], RawData);
      AddByte(aquastream_Command[2], RawData);
      ;
      AddByte(Relay_ShutdownTime, RawData);
      AddByte(Relay_Status, RawData);
      ;
      AddByte(multiswitch_AMP_Register[1], RawData);
      AddByte(multiswitch_LED_Register[1], RawData);
      AddByte(multiswitch_AMP_Register[2], RawData);
      AddByte(multiswitch_LED_Register[2], RawData);
      ;
      AddByte(multiswitch_Bargraph_AMP[1], RawData);
      AddByte(multiswitch_Bargraph_LED[1], RawData);
      AddByte(multiswitch_Status[1], RawData);
      AddByte(multiswitch_Bargraph_AMP[2], RawData);
      AddByte(multiswitch_Bargraph_LED[2], RawData);
      AddByte(multiswitch_Status[2], RawData);
      ;
      //Ende Profil;
      AddString(Expansion_RW, 32, RawData);
      AddString(aquabus_Buffer, 24, RawData);
      ;
      AddByte(Time_Hours, RawData);
      AddByte(Time_Minutes, RawData);
      AddByte(Time_Seconds, RawData);
      AddByte(Time_Day, RawData);
      ;
      AddByte(aquaero_Control, RawData);
      AddByte(aquaero_Screen, RawData);
      ;
    }

            /*

        //Firmware flashen
        If Protokol = TransferTypeRegister.FLASH Then

            Dim Checksum As Integer

            AddInteger(Flash_Adress, RawData) //Adresse
            AddString(Flash_Data, 256, RawData) //Daten
            AddInteger(Flash_Check, RawData) //Checksumme
            AddString(Flash_Key, 2, RawData) //Schl�ssel zum flashen 

        End If

        //Firmware l�schen
        If Protokol = TransferTypeRegister.FLASH_DELETE Then
            AddString(Flash_Delete_Key, 31, RawData) //Schl�ssel zum flashen 

        End If

        If Protokol = TransferTypeRegister.USER1 Then
            Dim j As Integer
            AddByte(FanTestMaxRpm, RawData)

            For j = 0 To 400
                AddByte(0, RawData)
            Next

        End If

        //USB-LCD-Ausgabe (nur einzelne Zeichen vorgesehen)
        If Protokol = TransferTypeRegister.ACTIVATE_LCDMODE Then
            Dim j As Integer
            Dim LcdData(3) As String

            AddByte(1, RawData)
            AddByte(128, RawData)
            AddByte(0, RawData)
            AddByte(0, RawData)
            AddByte(0, RawData)
            AddByte(0, RawData)
            AddByte(0, RawData)
            AddByte(0, RawData)
            AddByte(0, RawData)


            LcdData = GetLcdData(UsbLcdTextLine1)
            For j = 0 To 8
                AddByte(Convert.ToByte(LcdData(0).Chars(j)), RawData)
            Next

            For j = 0 To 8
                AddByte(Convert.ToByte(LcdData(1).Chars(j)), RawData)
            Next

            For j = 0 To 8
                AddByte(Convert.ToByte(LcdData(2).Chars(j)), RawData)
            Next


            AddByte(192, RawData)
            AddByte(0, RawData)
            AddByte(0, RawData)
            AddByte(0, RawData)
            AddByte(0, RawData)
            AddByte(0, RawData)
            AddByte(0, RawData)
            AddByte(0, RawData)
            AddByte(0, RawData)

            LcdData = GetLcdData(UsbLcdTextLine2)
            For j = 0 To 8
                AddByte(Convert.ToByte(LcdData(0).Chars(j)), RawData)
            Next

            For j = 0 To 8
                AddByte(Convert.ToByte(LcdData(1).Chars(j)), RawData)
            Next

            For j = 0 To 8
                AddByte(Convert.ToByte(LcdData(2).Chars(j)), RawData)
            Next

            For j = 0 To 360
                AddByte(0, RawData)
            Next

        End If
            */


  return RawData;
}

#if 0
    Private Function GetLcdData(ByVal Text As String) As String()
        Dim Result(3) As String
        Dim SubString1 As String
        Dim SubString2 As String
        Dim SubString3 As String

        Dim i As Integer
        For i = Text.Length - 1 To 30
            Text += " "
        Next

        SubString1 = Text.Substring(0, 8)
        SubString1 += Chr(255)

        SubString2 = Text.Substring(8, 8)
        SubString2 += Chr(255)

        SubString3 = Text.Substring(16, 4)
        SubString3 += Chr(0)
        SubString3 += Chr(0)
        SubString3 += Chr(0)
        SubString3 += Chr(0)
        SubString3 += Chr(15)

        Result(0) = SubString1
        Result(1) = SubString2
        Result(2) = SubString3

        Return (Result)
    End Function
#endif
    
#endif 
