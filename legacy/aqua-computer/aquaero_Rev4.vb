'Klasse zum bidirektionalen Austausch von Daten zwischen User-Interface und USB-Hid-Klasse
'Kompatibel zu aquaeros mit Firmware ab 4.0

Imports myTranslationClasses.TranslationFunctions

Public Class aquaero_Rev4
    'bei den Datenarrays werden immer zb. Lüfter werden nur die Elemente 1-4 verwendet, nicht 0


    Public Const Bit0 = 2 ^ (0)             'bitkonstanten
    Public Const Bit1 = 2 ^ (1)
    Public Const Bit2 = 2 ^ (2)
    Public Const Bit3 = 2 ^ (3)
    Public Const Bit4 = 2 ^ (4)
    Public Const Bit5 = 2 ^ (5)
    Public Const Bit6 = 2 ^ (6)
    Public Const Bit7 = 2 ^ (7)
    Public Const Bit8 = 2 ^ (8)
    Public Const Bit9 = 2 ^ (9)
    Public Const Bit10 = 2 ^ (10)
    Public Const Bit11 = 2 ^ (11)
    Public Const Bit12 = 2 ^ (12)
    Public Const Bit13 = 2 ^ (13)
    Public Const Bit14 = 2 ^ (14)
    Public Const Bit15 = 2 ^ (15)
    Public Const Bit16 = 2 ^ (16)
    Public Const Bit17 = 2 ^ (17)
    Public Const Bit18 = 2 ^ (18)
    Public Const Bit19 = 2 ^ (19)
    Public Const Bit20 = 2 ^ (20)
    Public Const Bit21 = 2 ^ (21)
    Public Const Bit22 = 2 ^ (22)
    Public Const Bit23 = 2 ^ (23)
    Public Const Bit24 = 2 ^ (24)
    Public Const Bit25 = 2 ^ (25)
    Public Const Bit26 = 2 ^ (26)
    Public Const Bit27 = 2 ^ (27)
    Public Const Bit28 = 2 ^ (28)
    Public Const Bit29 = 2 ^ (29)
    Public Const Bit30 = 2 ^ (30)
    Public Const Bit31 = 2 ^ (31)

    'Aquaero-Daten Struktur 
    <Flags()> _
    Public Enum TransferTypeRegister As Byte
        'Transfer-Protokolle für das Schreiben auf das aqauero
        FLASH = 100                     'Flashen der MCU (es dürfen nur flasch daten verschickt werden)
        LOAD_DEFAULT = 101              'Laden der Standard-Daten
        EEPROM_SAVE = 102               'Speichern nach übertragen der Daten alles im EEprom ab
        RAM = 103                       'Datenausgabe in das RAM (normale schreibtransfers)
        AQUABUS_READ = 104              'Direktes lesen com aquabus
        AQUABUS_WRITE = 105             'Schreiben auf aquabus
        LCD = 106                       'Datenausgabe in den lcd puffer, AE muss per hand in den USB-LCD mode geschlten werden
        PROFIL_LOAD = 107               'Lädt das angegebene Profil
        PROFIL_COPY = 108               'Kopiert aktuelles Profil in das andere
        ACTIVATE_LCDMODE = 109          'Wecheselt sofort in LCD-Mode und gibt die daten aus
        USER1 = 110                     'testet die maximale drehzahl vom lüfter
        FLASH_DELETE = 200              'Löscht den kompletten flash
    End Enum

    Public Shared TransferType As TransferTypeRegister  ''<- Protokoll-Auswahl (RAM/EEPROM/LCD/FLASH)

    Public Shared UsbLcdTextLine1 As String             'ist der text der auf das usb lcd ausgegeben wird
    Public Shared UsbLcdTextLine2 As String             'ist der text der auf das usb lcd ausgegeben wird

    'Variablen für Flash-Modus
    Public Shared Flash_Adress As Long                  'Adresse (16-bit)
    Public Shared Flash_Data As String                  'Daten (256 Byte)
    Public Shared Flash_Check As Long                   'Checksumme (2 Byte)
    Public Shared Flash_Key As String                   'Key zum flashen
    Public Shared Flash_Delete_Key As String            'Passwort für Flash-Delete

    'Variablen für das Schreiben in RAM und EEPROM
    ' ########### AB HIER GLOBALE DATEN, UNABHÄNGIG VOM PROFIL ###########

    Public Shared Fan_Name(4) As String                 'Namen der Lüfter (4 x 11 Zeichen)
    Public Shared Flowsensor_Name As String             'Name des Durchflussensors
    Public Shared Sensor_Name(6) As String              'Namen der Temperatursensoren (6x 11 Zeichen)
    Public Shared aquaero_Name As String                'Name des aquaeros

    <Flags()> _
   Public Enum ProfilRegister As Byte
        Profil_A = 0
        Profil_B = 1
    End Enum
    Public Shared Profil As ProfilRegister              'Aktuelles Profil

    Public Shared Fan_MinimumPower(4) As Byte           'Minimalleistung der Lüfter als PWM-Wert. Nur bei Automatik-Betrieb.
    Public Shared Fan_Pulse(4) As Integer               'Impulse/Umdrehung (>1)
    Public Shared Flowsensor_Pulse As Integer           'Impulse/l
    Public Shared LCD_Backlight As Byte                 'Backlight PWM, 0-200
    Public Shared LCD_Contrast As Byte                  'Contrast PWM, 155-255
    Public Shared LCD_Fadingtime As Integer             'Zeit bis Display ausgebelendet wird
    Public Shared Sensor_Faktor(6) As Byte              'Korrekturfaktor
    Public Shared Sensor_Offset(6) As Byte              'KorrekturOffset in 1/10 K
    Public Shared Sensor_Alarm(6) As Integer            'Alarmwert in 1/10 K
    Public Shared FanTestMaxRpm As Byte                 'die bits geben an bei welchem lüfter die max rpm getestet werden soll (transfertyp USER1)

    ' ########### AB HIER PROFILDATEN ###########
    <Flags()> _
       Public Enum LCDSettingsRegister As Byte
        BacklightOn = Bit0                              'Backlight immer an
        CompleteOff = Bit2                              'LCD schaltete sich nach dimmen komplett ab
        USBLCDBoot = Bit3                               'aquaero startet im USB-LCD-Modus
    End Enum
    Public Shared LCD_Settings As LCDSettingsRegister '<-
    Public Shared LCD_PageDelay As Byte 'Umschaltzeit zweischen Display-Seiten (255 = nicht schalten)

    <Flags()> _
    Public Enum LCDPageRegister As Long
        'Sewiten mit gesetzten Flags werden angezeigt
        TempOverview = Bit0                             'Sensorübersicht
        TempSensor_1_2 = Bit1                           'Temperatur 1 & 2
        TempSensor_3_4 = Bit2                           'Temperatur 3 & 4
        TempSensor_5_6 = Bit3                           'Temperatur 5 & 6
        Fan_1 = Bit4                                    'Fan 1 Details
        Fan_2 = Bit5                                    'Fan 2 Details
        Fan_3 = Bit6                                    'Fan 3 Details
        Fan_4 = Bit7                                    'Fan 4 Details
        Aquastream_1_A = Bit8                           'Daten Seite A aquastream 1
        Aquastream_1_B = Bit9                           'Daten Seite B aquastream 1
        Aquastream_2_A = Bit10                          'Daten Seite A aquastream 2
        Aquastream_2_B = Bit11                          'Daten Seite B aquastream 2
        FlowSensor_1 = Bit12                            'Durchflussensor 1
        FlowSensor_2 = Bit13                            'Durchflussensor 2
        tubemeter = Bit14                               'tubemeter Füllstandssensor
        aquaero = Bit15                                 'aquaero Hauptseite
        Powersensor_1 = Bit16                           'Leistungsmessung 1
        Powersensor_2 = Bit17                           'Leistungsmessung 2
        Fan_Voltage_1_2 = Bit18                         'Spannung der Lüfter 1 und 2
        Fan_Voltage_3_4 = Bit19                         'Spannung der Lüfter 1 und 2
        Fan_Overview = Bit20                            'Lüfterbersicht von lüfter 1-4
        aquaero_info1 = Bit21                           'Informationen Hardware/Firmware Seite 1
        aquaero_info2 = Bit22                           'Informationen Hardware/Firmware Seite 2
    End Enum
    Public Shared LCD_Page As LCDPageRegister           '<- hier werden angezeigten displayseiten konfiguriert

    <Flags()> _
    Public Enum Powersensor_ConfigRegister As Byte
        Powersensor_1_Uses_Flow_2 = Bit0                'Wenn enabled wird Flow-Sensor 2 verwendet
        Powersensor_2_Uses_Flow_2 = Bit1                'Wenn enabled wird Flow-Sensor 2 verwendet
    End Enum
    Public Shared Powersensor_1_Sensor As Byte          'Konfiguration Temperatursensoren für Messung 1
    Public Shared Powersensor_2_Sensor As Byte          'Konfiguration Temperatursensoren für Messung 2
    Public Shared Powersensor_Config As Powersensor_ConfigRegister '<-

    <Flags()> _
    Public Enum tubemeter_ConfigRegister As Byte
        level_mm = 2 'Levelanzeige in mm 
        LED_ON = 4 'LED aktivieren/deaktivieren
    End Enum
    Public Shared tubemeter_Config As tubemeter_ConfigRegister
    Public Shared tubemeter_Alarmlevel As Byte          'Level für Alarm
    Public Shared tubemeter_Alarmmove As Byte           'Level für Alarm bei Bewegung
    Public Shared tubemeter_Expansion As Byte           'bisher unbelegt

    Public Shared Fan_Sensor(4) As Byte                 'Zuordnung der Sensoren/Lüfter, untere 4-Byte / obere 4-Byte
    Public Shared Fan_SetRPM(4) As Integer              'Drehzahlvorgabe für Lüfter
    Public Shared Fan_SetPower(4) As Byte               'Leistung der Lüfter

    <Flags()> _
    Public Enum Fan_ModeRegister As Integer
        PWR_RPM_Switch = Bit0                           'Regelung über Leistung oder RPM, nur bei maneller Regelung interessant
        Maual_Auto_Switch = Bit1                        'Manuelle oder über Sensor gesteuerte Regelung
        Get_Max_RPM = Bit3                              'Ermitteln der maximalen Drehzahl (wird durchgeführt oder soll ermittelt werden)
        Progressiv_Linear_Switch = Bit4                 'Progressiv oder Linear regeln
        Automatik_PWM_RPM_Switch = Bit5                 'Automatik-Betrieb mit Leistungs oder RPM-Regelung 
        Ignore_RPM = Bit6                               'Ignorieren des Tachosignals
        Force_MinimumPower = Bit7                       'Minimale Leistung oder Drehzahl halten
        Disable_RPM_Test = Bit8                         'Deaktivieren des Drehzahltests beim starten, Tachonutzung muss deaktiviert sein
    End Enum
    Public Shared Fan_Mode(4) As Fan_ModeRegister       '<- hier stehen die jeweiligen lüftereinstellungen drinn

    <Flags()> _
    Public Enum FlowSensorModeRegister As Byte
        l_minute_hour = Bit0                            'Liter/Minute oder Stunde (zweiter Durchflussensor)
        use_fan4_as_flowsensor = Bit1                   'Lüfterkanal 4 als Flow-Sensor-Eingang verwenden (zweiter Durchflussensor)
    End Enum
    Public Shared Flowsensor_Mode As FlowSensorModeRegister '<-
    Public Shared FlowSensor_lAlarm(2) As Integer       'Alarmgrenze in 1/10 l für beide Sensoren
    Public Shared LED_PWM(2) As Byte                    'PWM-Wert der beiden LEDs

    <Flags()> _
    Public Enum LED_ModeRegister As Byte
        Manual_Auto_Switch = Bit0                       'Manuell oder automatisch gesteuert
        Alarm_Flash = Bit1                              'Bei Alarm blinken
        Hot_Cold = Bit2                                 'Bei steigender/fallender Temperaur heller werdend
    End Enum
    Public Shared LED_Mode(2) As LED_ModeRegister       '<-hier stehen die LED einstellung drinn
    Public Shared LED_Sensor(2) As Byte                 'Zuordnung der Sensoren/LEDs, untere 4-Byte / obere 4-Byte

    Public Shared Sensor_Min(6) As Integer              'Minimaler Regelungswert in 1/10 K
    Public Shared Sensor_Max(6) As Integer              'Maximaler Regelungswert in 1/10 K
    Public Shared Sensor_Hyst(6) As Byte                'Hysterese in 1/10 K
    Public Shared Sensor_Opt(6) As Integer              'Sollwert für Sensor in 1/10 K
    Public Shared Controler_Time(6) As Byte             'Regler-Zeit
    Public Shared Controler_Factor(6) As Byte           'Regler Faktor

    <Flags()> _
    Public Enum Alarm_SetupRegister As Integer
        Fan_1 = Bit0                                    'Alarm für Lüfter 1 aktivieren
        Fan_2 = Bit1                                    'Alarm für Lüfter 2 aktivieren
        Fan_3 = Bit2                                    'Alarm für Lüfter 3 aktivieren
        Fan_4 = Bit3                                    'Alarm für Lüfter 4 aktivieren
        Sensor = Bit4                                   'Alarm für alle Temperatursensoren
        Flowsensor_1 = Bit5                             'Alarm für Durchflussensor
        Flowsensor_2 = Bit6                             'Alarm für Durchflussensor
        tubemeter_level = Bit7                          'Füllstandsalarm
        tubemeter_wave = Bit8                           'Wellenbewegung
        aquastream_1 = Bit9                             'aquastream 1
        aquastream_2 = Bit10                            'aquastream 1
        Enable = Bit15                                  'Aktivierung für alle Alarmfunktionen, ohne Enable isst KEIN Alarm aktiv
    End Enum
    Public Shared Alarm_Setup As Alarm_SetupRegister    '<- hier werden die möglichen alarme freigegeben
    Public Shared Alarm_ShutdownDelay As Byte           'Verzögerung zwischen Alarm und Notabschaltung (0-180) Sekunden

    <Flags()> _
 Public Enum DayRegister As Byte
        Sonday = 0
        Monday = 1
        Tuesday = 2
        Wednesday = 3
        Thursday = 4
        Friday = 5
        Saturday = 6
        EveryDay = 7
        MondayToFriday = 8                              'Montag bis Freitag (nur für Timer)
        Saturday_Sunday = 9                             'Samstag & Sonntag (nur für Timer)
        Never = 10                                      'Niemals (nur für Timer)
    End Enum
    Public Shared Timer_Off_Hours(2) As Byte            'Timer ausschalten Stunden
    Public Shared Timer_Off_Minutes(2) As Byte          'Timer ausschalten Minuten
    Public Shared Timer_Off_Seconds(2) As Byte          'Timer ausschalten Sekunden
    Public Shared Timer_Off_Day(2) As DayRegister       'Timer ausschalten Tag
    Public Shared Timer_Off_SwitchTime(2) As Byte       'Schaltzeit des Relays beim auslösen (255->für immer schalten)

    Public Shared Timer_On_Hours(2) As Byte             'Timer einschalten Stunden
    Public Shared Timer_On_Minutes(2) As Byte           'Timer einschalten Minuten
    Public Shared Timer_On_Seconds(2) As Byte           'Timer einschalten Sekunden
    Public Shared Timer_On_Day(2) As DayRegister        'Timer einschalten Tag
    Public Shared Timer_On_SwitchTime(2) As Byte        'Schaltzeit des Relays beim auslösen (255->für immer schalten)

    <Flags()> _
    Public Enum aquastream_CommandRegister As Byte
        Set_Default = 41                                'Default-Werte setzen
        Anti_Air = 75                                   'Entlüftung
        Set_Frequency = 150                             'Frequenz setzen
    End Enum
    Public Shared aquastream_Command(2) As aquastream_CommandRegister

    Public Shared Relay_ShutdownTime As Byte            'Bei Nutzung als PWR-Switch, Zeit für die der Schalter ausgelöst werden soll (255 = Immer)

    <Flags()> _
    Public Enum Relay_StatusRegister As Byte
        Relay_On_Off = Bit0                             'Relais an/aus
        Relay_Usage_ATXBREAK_PWR = Bit1                 'Benutzung als ATX oder Power-Schalter
    End Enum
    Public Shared Relay_Status As Relay_StatusRegister

    Public Shared multiswitch_AMP_Register(2) As Byte   'Endstufen-Ausgänge multisiwtch
    Public Shared multiswitch_LED_Register(2) As Byte   'LED-Ausgänge multiswitch

    <Flags()> _
    Public Enum multiswitch_BargraphRegister As Byte
        Bargraph_Fan_1_RPM = 0                          'Bargraph nach Drehzahl Lüfter 1-4
        Bargraph_Fan_2_RPM = 1
        Bargraph_Fan_3_RPM = 2
        Bargraph_Fan_4_RPM = 3
        Bargraph_Fan_1_PWR = 4                          'Bargraph nach Leitung Lüfter 1-4
        Bargraph_Fan_2_PWR = 5
        Bargraph_Fan_3_PWR = 6
        Bargraph_Fan_4_PWR = 7
        Bargraph_Sensor_1 = 8                           'Bargraph zwischen Min und Max Temperatur 1-6
        Bargraph_Sensor_2 = 9
        Bargraph_Sensor_3 = 10
        Bargraph_Sensor_4 = 11
        Bargraph_Sensor_5 = 12
        Bargraph_Sensor_6 = 13
        Bargraph_Off = 14 'Bargraph 
    End Enum
    Public Shared multiswitch_Bargraph_AMP(2) As multiswitch_BargraphRegister
    Public Shared multiswitch_Bargraph_LED(2) As multiswitch_BargraphRegister

    <Flags()> _
    Public Enum multiswitch_StatusRegister As Byte
        Relais_On = Bit0                                'Relais vom Multiswitch On/Off
    End Enum
    Public Shared multiswitch_Status(2) As multiswitch_StatusRegister

    Public Shared Expansion_RW As String                'Freier Expansion RW-Bereich
    Public Shared aquabus_Buffer As String              'aquabusBuffer-Bereich

    Public Shared Time_Hours As Byte                    'Aktuelle Zeit, Stunden
    Public Shared Time_Minutes As Byte                  'Aktuelle Zeit, Minuten
    Public Shared Time_Seconds As Byte                  'Aktuelle Zeit, Sekunden
    Public Shared Time_Day As DayRegister               'Aktueller Tag

    <Flags()> _
    Public Enum aquaero_ControlRegister As Byte
        Keylock = 1                                     'Keylock
        EEPROM_Reset = 2                                'EEPROM reseten
    End Enum
    Public Shared aquaero_Control As aquaero_ControlRegister 'Keylock usw.

    <Flags()> _
    Public Enum aquaero_ScreenRegister As Byte
        Setup = 0                                       'wenn man in einem Aquaero menü ist=0
        USBLCD = 255                                    'bei usblcd screen= 255
    End Enum
    Public Shared aquaero_Screen As aquaero_ScreenRegister 'Aktueller Screen des aquaeros

    ' ########### AB HIER GLOBALE READ-ONLY DATEN ###########
    <Flags()> _
    Public Enum aquabus_StatusRegister As Byte
        Fail = Bit0                                     'Übertragungsfehler
        Busy = Bit1                                     'Bus belegt
        Reading = Bit2                                  'Es wird gelesen
        Writing = Bit3                                  'Es wird geschrieben
        NewData = Bit4                                  'Neue Daten sethen nach dem Lesen zur Verfügung
        EEPROM_Busy = Bit5                              'Daten werden gesichert oder geladen
    End Enum
    Public Shared aquabus_Status As aquabus_StatusRegister

    <Flags()> _
    Public Enum aquabus_DeviceRegister As Byte
        aquastream_1 = Bit0                             'Angeschlossene Geräte Aquastream 1
        aquastream_2 = Bit1                             'Angeschlossene Geräte Aquastream 2
        tubemeter = Bit3                                'Angeschlossene Geräte tubemeter
        multiswitch_1 = Bit4                            'Angeschlossene Geräte multiswitch1
        multiswitch_2 = Bit5                            'Angeschlossene Geräte multiswitch2
    End Enum
    Public Shared aquabus_Devices As aquabus_DeviceRegister 'Gibt an welche Geräte am aquabus vorhanden sind

    Public Shared multiswitch_OutputData(2, 3) As Byte  'Daten die vom aquaero an den ms gesendet werden

    Public Shared tubemeter_rawlevel As Integer         'Rohdaten
    Public Shared tubemeter_wavelevel As Byte           'Wellenbewegung
    Public Shared tubemeter_level_mm As Byte            'Level in mm
    Public Shared tubemeter_level_avg As Byte           'Durchschnittliches Level

    Public Shared aquastream_1_RawData As String        '16 Byte RAW-Data
    Public Shared aquastream_1_Current As Integer       'Strom
    Public Shared aquastream_1_Voltage As Integer       'Spannung
    Public Shared aquastream_1_Power As Long            'Leistung

    Public Shared aquastream_2_RawData As String        '16 Byte RAW-Data
    Public Shared aquastream_2_Current As Integer       'Strom
    Public Shared aquastream_2_Voltage As Integer       'Spannung
    Public Shared aquastream_2_Power As Long            'Leistung

    <Flags()> _
    Public Enum USBLCD_ModeRegister As Byte
        USBLCD_Active = Bit0                            'USBLCD-Modus ist betriebsbereit
        USBLCD_NewData = Bit1                           'Neue Daten für USB-LCD vorhanden
        USBLCD_Flashing = Bit2                          'aquaero wird gerade geflasht
        USBLCD_ConnectionPage = Bit3                    'Display-Seite mit USB-Verbdindung wird angezeigt
    End Enum
    Public Shared USBLCD_Mode As USBLCD_ModeRegister

    Public Shared Powersensor_Power(2) As Integer       'Gemessene Leistung bei Leistungsmessung
    Public Shared Fan_RPM(4) As Integer                 'Aktuelle Drehzahl der Lüfter 
    Public Shared Flowsensor_l As Integer               'Durchfluss
    Public Shared Fan_MaxRPM(4) As Integer              'Maximale Drehzahl der Lüfter
    Public Shared Sensor_Temp(6) As Integer             'Aktuelle Temperaturen
    Public Shared Alarm_Status As Alarm_SetupRegister   'Aktueller Alarm wird ausgelöst von...
    Public Shared Alarm_Memory_Pointer As Byte          'Pointer auf den letzten Alarm im Ringpuffer
    Public Shared Alarm_Memory_Typ(5) As Alarm_SetupRegister 'Alarmtyp
    Public Shared Alarm_Memory_Sensor(5) As Byte        'Nummer des Sensors, der Alarm ausgegeben hat
    Public Shared Alarm_Memory_Hours(5) As Byte         'Alarmzeit, Stunden
    Public Shared Alarm_Memory_Minutes(5) As Byte       'Alarmzeit, Minuten
    Public Shared Alarm_Memory_Day(5) As DayRegister    'Alarmzeit, Sekunden
    Public Shared Firmware_Name As String               'Firmware-Revision (5 Zeichen)

    <Flags()> _
       Public Enum LCD_TypeRegister As Byte
        LCD = 0
        VFD = 1
        NODISPLAY = 255
    End Enum
    Public Shared LCD_Type As LCD_TypeRegister 'Displaytyp
    Public Shared OS_Version As Integer                 'OS-Version
    Public Shared FirmwareRevision As Integer           'Firmware-Version
    Public Shared Flashcounter As Integer               'Flashcounter
    Public Shared HardwareRevision As Integer           'Hardware-Version
    Public Shared Serialnumber As Integer               'Seriennummer
    Public Shared ProductionMonth As Byte               'Monat der Produktion
    Public Shared ProductionYear As Byte                'Jahr der Produktion
    Public Shared Language As String                    'Sprache
    Public Shared Expansion_RO As String                'Expansion-Bereich RO

    Function CreateStructureFromString(ByVal RawData As String) As Boolean

        Dim myPositionCounter As Integer 'Aktueller Positions-Zeiger

        'Struktur aufbauen
        'Globale Daten
        Fan_Name(1) = GetTextString(RawData, myPositionCounter, 11)
        Fan_Name(2) = GetTextString(RawData, myPositionCounter, 11)
        Fan_Name(3) = GetTextString(RawData, myPositionCounter, 11)
        Fan_Name(4) = GetTextString(RawData, myPositionCounter, 11)
        Flowsensor_Name = GetTextString(RawData, myPositionCounter, 11)

        Sensor_Name(1) = GetTextString(RawData, myPositionCounter, 11)
        Sensor_Name(2) = GetTextString(RawData, myPositionCounter, 11)
        Sensor_Name(3) = GetTextString(RawData, myPositionCounter, 11)
        Sensor_Name(4) = GetTextString(RawData, myPositionCounter, 11)
        Sensor_Name(5) = GetTextString(RawData, myPositionCounter, 11)
        Sensor_Name(6) = GetTextString(RawData, myPositionCounter, 11)

        aquaero_Name = GetTextString(RawData, myPositionCounter, 9)

        Profil = GetByte(RawData, myPositionCounter)

        Fan_MinimumPower(1) = GetByte(RawData, myPositionCounter)
        Fan_MinimumPower(2) = GetByte(RawData, myPositionCounter)
        Fan_MinimumPower(3) = GetByte(RawData, myPositionCounter)
        Fan_MinimumPower(4) = GetByte(RawData, myPositionCounter)

        Fan_Pulse(1) = GetInteger(RawData, myPositionCounter)
        Fan_Pulse(2) = GetInteger(RawData, myPositionCounter)
        Fan_Pulse(3) = GetInteger(RawData, myPositionCounter)
        Fan_Pulse(4) = GetInteger(RawData, myPositionCounter)

        Flowsensor_Pulse = GetInteger(RawData, myPositionCounter)

        LCD_Backlight = GetByte(RawData, myPositionCounter)
        LCD_Contrast = GetByte(RawData, myPositionCounter)
        LCD_Fadingtime = GetInteger(RawData, myPositionCounter)

        Sensor_Faktor(1) = GetByte(RawData, myPositionCounter)
        Sensor_Faktor(2) = GetByte(RawData, myPositionCounter)
        Sensor_Faktor(3) = GetByte(RawData, myPositionCounter)
        Sensor_Faktor(4) = GetByte(RawData, myPositionCounter)
        Sensor_Faktor(5) = GetByte(RawData, myPositionCounter)
        Sensor_Faktor(6) = GetByte(RawData, myPositionCounter)

        Sensor_Offset(1) = GetByte(RawData, myPositionCounter)
        Sensor_Offset(2) = GetByte(RawData, myPositionCounter)
        Sensor_Offset(3) = GetByte(RawData, myPositionCounter)
        Sensor_Offset(4) = GetByte(RawData, myPositionCounter)
        Sensor_Offset(5) = GetByte(RawData, myPositionCounter)
        Sensor_Offset(6) = GetByte(RawData, myPositionCounter)

        Sensor_Alarm(1) = GetInteger(RawData, myPositionCounter)
        Sensor_Alarm(2) = GetInteger(RawData, myPositionCounter)
        Sensor_Alarm(3) = GetInteger(RawData, myPositionCounter)
        Sensor_Alarm(4) = GetInteger(RawData, myPositionCounter)
        Sensor_Alarm(5) = GetInteger(RawData, myPositionCounter)
        Sensor_Alarm(6) = GetInteger(RawData, myPositionCounter)

        'Ab hier Profildaten 
        LCD_Settings = GetByte(RawData, myPositionCounter)
        LCD_PageDelay = GetByte(RawData, myPositionCounter)
        LCD_Page = GetLong(RawData, myPositionCounter)

        Powersensor_1_Sensor = GetByte(RawData, myPositionCounter)
        Powersensor_2_Sensor = GetByte(RawData, myPositionCounter)
        Powersensor_Config = GetByte(RawData, myPositionCounter)

        tubemeter_Config = GetByte(RawData, myPositionCounter)
        tubemeter_Alarmlevel = GetByte(RawData, myPositionCounter)
        tubemeter_Alarmmove = GetByte(RawData, myPositionCounter)
        tubemeter_Expansion = GetByte(RawData, myPositionCounter)

        Fan_Sensor(1) = GetByte(RawData, myPositionCounter)
        Fan_Sensor(2) = GetByte(RawData, myPositionCounter)
        Fan_Sensor(3) = GetByte(RawData, myPositionCounter)
        Fan_Sensor(4) = GetByte(RawData, myPositionCounter)

        Fan_SetRPM(1) = GetInteger(RawData, myPositionCounter)
        Fan_SetRPM(2) = GetInteger(RawData, myPositionCounter)
        Fan_SetRPM(3) = GetInteger(RawData, myPositionCounter)
        Fan_SetRPM(4) = GetInteger(RawData, myPositionCounter)

        Fan_SetPower(1) = GetByte(RawData, myPositionCounter)
        Fan_SetPower(2) = GetByte(RawData, myPositionCounter)
        Fan_SetPower(3) = GetByte(RawData, myPositionCounter)
        Fan_SetPower(4) = GetByte(RawData, myPositionCounter)

        Fan_Mode(1) = GetInteger(RawData, myPositionCounter)
        Fan_Mode(2) = GetInteger(RawData, myPositionCounter)
        Fan_Mode(3) = GetInteger(RawData, myPositionCounter)
        Fan_Mode(4) = GetInteger(RawData, myPositionCounter)

        Flowsensor_Mode = GetInteger(RawData, myPositionCounter)
        FlowSensor_lAlarm(1) = GetInteger(RawData, myPositionCounter)
        FlowSensor_lAlarm(2) = GetInteger(RawData, myPositionCounter)

        LED_PWM(1) = GetByte(RawData, myPositionCounter)
        LED_PWM(2) = GetByte(RawData, myPositionCounter)
        LED_Mode(1) = GetByte(RawData, myPositionCounter)
        LED_Mode(2) = GetByte(RawData, myPositionCounter)
        LED_Sensor(1) = GetByte(RawData, myPositionCounter)
        LED_Sensor(2) = GetByte(RawData, myPositionCounter)

        Sensor_Min(1) = GetInteger(RawData, myPositionCounter)
        Sensor_Min(2) = GetInteger(RawData, myPositionCounter)
        Sensor_Min(3) = GetInteger(RawData, myPositionCounter)
        Sensor_Min(4) = GetInteger(RawData, myPositionCounter)
        Sensor_Min(5) = GetInteger(RawData, myPositionCounter)
        Sensor_Min(6) = GetInteger(RawData, myPositionCounter)

        Sensor_Max(1) = GetInteger(RawData, myPositionCounter)
        Sensor_Max(2) = GetInteger(RawData, myPositionCounter)
        Sensor_Max(3) = GetInteger(RawData, myPositionCounter)
        Sensor_Max(4) = GetInteger(RawData, myPositionCounter)
        Sensor_Max(5) = GetInteger(RawData, myPositionCounter)
        Sensor_Max(6) = GetInteger(RawData, myPositionCounter)

        Sensor_Hyst(1) = GetByte(RawData, myPositionCounter)
        Sensor_Hyst(2) = GetByte(RawData, myPositionCounter)
        Sensor_Hyst(3) = GetByte(RawData, myPositionCounter)
        Sensor_Hyst(4) = GetByte(RawData, myPositionCounter)
        Sensor_Hyst(5) = GetByte(RawData, myPositionCounter)
        Sensor_Hyst(6) = GetByte(RawData, myPositionCounter)

        Sensor_Opt(1) = GetInteger(RawData, myPositionCounter)
        Sensor_Opt(2) = GetInteger(RawData, myPositionCounter)
        Sensor_Opt(3) = GetInteger(RawData, myPositionCounter)
        Sensor_Opt(4) = GetInteger(RawData, myPositionCounter)
        Sensor_Opt(5) = GetInteger(RawData, myPositionCounter)
        Sensor_Opt(6) = GetInteger(RawData, myPositionCounter)

        Controler_Time(1) = GetByte(RawData, myPositionCounter)
        Controler_Time(2) = GetByte(RawData, myPositionCounter)
        Controler_Time(3) = GetByte(RawData, myPositionCounter)
        Controler_Time(4) = GetByte(RawData, myPositionCounter)
        Controler_Time(5) = GetByte(RawData, myPositionCounter)
        Controler_Time(6) = GetByte(RawData, myPositionCounter)

        Controler_Factor(1) = GetByte(RawData, myPositionCounter)
        Controler_Factor(2) = GetByte(RawData, myPositionCounter)
        Controler_Factor(3) = GetByte(RawData, myPositionCounter)
        Controler_Factor(4) = GetByte(RawData, myPositionCounter)
        Controler_Factor(5) = GetByte(RawData, myPositionCounter)
        Controler_Factor(6) = GetByte(RawData, myPositionCounter)

        Alarm_Setup = GetInteger(RawData, myPositionCounter)
        Alarm_ShutdownDelay = GetByte(RawData, myPositionCounter)

        Timer_Off_Hours(1) = GetByte(RawData, myPositionCounter)
        Timer_Off_Minutes(1) = GetByte(RawData, myPositionCounter)
        Timer_Off_Seconds(1) = GetByte(RawData, myPositionCounter)
        Timer_Off_Day(1) = GetByte(RawData, myPositionCounter)
        Timer_Off_SwitchTime(1) = GetByte(RawData, myPositionCounter)

        Timer_Off_Hours(2) = GetByte(RawData, myPositionCounter)
        Timer_Off_Minutes(2) = GetByte(RawData, myPositionCounter)
        Timer_Off_Seconds(2) = GetByte(RawData, myPositionCounter)
        Timer_Off_Day(2) = GetByte(RawData, myPositionCounter)
        Timer_Off_SwitchTime(2) = GetByte(RawData, myPositionCounter)

        Timer_On_Hours(1) = GetByte(RawData, myPositionCounter)
        Timer_On_Minutes(1) = GetByte(RawData, myPositionCounter)
        Timer_On_Seconds(1) = GetByte(RawData, myPositionCounter)
        Timer_On_Day(1) = GetByte(RawData, myPositionCounter)
        Timer_On_SwitchTime(1) = GetByte(RawData, myPositionCounter)

        Timer_On_Hours(2) = GetByte(RawData, myPositionCounter)
        Timer_On_Minutes(2) = GetByte(RawData, myPositionCounter)
        Timer_On_Seconds(2) = GetByte(RawData, myPositionCounter)
        Timer_On_Day(2) = GetByte(RawData, myPositionCounter)
        Timer_On_SwitchTime(2) = GetByte(RawData, myPositionCounter)

        aquastream_Command(1) = GetByte(RawData, myPositionCounter)
        aquastream_Command(2) = GetByte(RawData, myPositionCounter)

        Relay_ShutdownTime = GetByte(RawData, myPositionCounter)
        Relay_Status = GetByte(RawData, myPositionCounter)

        multiswitch_AMP_Register(1) = GetByte(RawData, myPositionCounter)
        multiswitch_LED_Register(1) = GetByte(RawData, myPositionCounter)
        multiswitch_AMP_Register(2) = GetByte(RawData, myPositionCounter)
        multiswitch_LED_Register(2) = GetByte(RawData, myPositionCounter)

        multiswitch_Bargraph_AMP(1) = GetByte(RawData, myPositionCounter)
        multiswitch_Bargraph_LED(1) = GetByte(RawData, myPositionCounter)
        multiswitch_Status(1) = GetByte(RawData, myPositionCounter)
        multiswitch_Bargraph_AMP(2) = GetByte(RawData, myPositionCounter)
        multiswitch_Bargraph_LED(2) = GetByte(RawData, myPositionCounter)
        multiswitch_Status(2) = GetByte(RawData, myPositionCounter)

        'Ende Profil
        Expansion_RW = GetString(RawData, myPositionCounter, 32)
        aquabus_Buffer = GetString(RawData, myPositionCounter, 24)

        Time_Hours = GetByte(RawData, myPositionCounter)
        Time_Minutes = GetByte(RawData, myPositionCounter)
        Time_Seconds = GetByte(RawData, myPositionCounter)
        Time_Day = GetByte(RawData, myPositionCounter)

        aquaero_Control = GetByte(RawData, myPositionCounter)
        aquaero_Screen = GetByte(RawData, myPositionCounter)

        'Read-Only-Daten
        aquabus_Status = GetByte(RawData, myPositionCounter)
        aquabus_Devices = GetInteger(RawData, myPositionCounter)

        multiswitch_OutputData(1, 1) = GetByte(RawData, myPositionCounter)
        multiswitch_OutputData(1, 2) = GetByte(RawData, myPositionCounter)
        multiswitch_OutputData(1, 3) = GetByte(RawData, myPositionCounter)
        multiswitch_OutputData(2, 1) = GetByte(RawData, myPositionCounter)
        multiswitch_OutputData(2, 2) = GetByte(RawData, myPositionCounter)
        multiswitch_OutputData(2, 3) = GetByte(RawData, myPositionCounter)

        tubemeter_rawlevel = GetInteger(RawData, myPositionCounter)
        tubemeter_wavelevel = GetByte(RawData, myPositionCounter)
        tubemeter_level_mm = GetByte(RawData, myPositionCounter)
        tubemeter_level_avg = GetByte(RawData, myPositionCounter)

        aquastream_1_RawData = GetString(RawData, myPositionCounter, 16)
        aquastream_1_Current = GetInteger(RawData, myPositionCounter)
        aquastream_1_Voltage = GetInteger(RawData, myPositionCounter)
        aquastream_1_Power = GetLong(RawData, myPositionCounter)

        aquastream_2_RawData = GetString(RawData, myPositionCounter, 16)
        aquastream_2_Current = GetInteger(RawData, myPositionCounter)
        aquastream_2_Voltage = GetInteger(RawData, myPositionCounter)
        aquastream_2_Power = GetLong(RawData, myPositionCounter)

        USBLCD_Mode = GetByte(RawData, myPositionCounter)

        Powersensor_Power(1) = GetInteger(RawData, myPositionCounter)
        Powersensor_Power(2) = GetInteger(RawData, myPositionCounter)

        Fan_RPM(1) = GetInteger(RawData, myPositionCounter)
        Fan_RPM(2) = GetInteger(RawData, myPositionCounter)
        Fan_RPM(3) = GetInteger(RawData, myPositionCounter)
        Fan_RPM(4) = GetInteger(RawData, myPositionCounter)

        Flowsensor_l = GetInteger(RawData, myPositionCounter)

        Fan_MaxRPM(1) = GetInteger(RawData, myPositionCounter)
        Fan_MaxRPM(2) = GetInteger(RawData, myPositionCounter)
        Fan_MaxRPM(3) = GetInteger(RawData, myPositionCounter)
        Fan_MaxRPM(4) = GetInteger(RawData, myPositionCounter)

        Sensor_Temp(1) = GetInteger(RawData, myPositionCounter)
        Sensor_Temp(2) = GetInteger(RawData, myPositionCounter)
        Sensor_Temp(3) = GetInteger(RawData, myPositionCounter)
        Sensor_Temp(4) = GetInteger(RawData, myPositionCounter)
        Sensor_Temp(5) = GetInteger(RawData, myPositionCounter)
        Sensor_Temp(6) = GetInteger(RawData, myPositionCounter)

        Alarm_Status = GetInteger(RawData, myPositionCounter)
        Alarm_Memory_Pointer = GetByte(RawData, myPositionCounter)

        Alarm_Memory_Typ(1) = GetInteger(RawData, myPositionCounter)
        Alarm_Memory_Sensor(1) = GetByte(RawData, myPositionCounter)
        Alarm_Memory_Hours(1) = GetByte(RawData, myPositionCounter)
        Alarm_Memory_Minutes(1) = GetByte(RawData, myPositionCounter)
        Alarm_Memory_Day(1) = GetByte(RawData, myPositionCounter)

        Alarm_Memory_Typ(2) = GetInteger(RawData, myPositionCounter)
        Alarm_Memory_Sensor(2) = GetByte(RawData, myPositionCounter)
        Alarm_Memory_Hours(2) = GetByte(RawData, myPositionCounter)
        Alarm_Memory_Minutes(2) = GetByte(RawData, myPositionCounter)
        Alarm_Memory_Day(2) = GetByte(RawData, myPositionCounter)

        Alarm_Memory_Typ(3) = GetInteger(RawData, myPositionCounter)
        Alarm_Memory_Sensor(3) = GetByte(RawData, myPositionCounter)
        Alarm_Memory_Hours(3) = GetByte(RawData, myPositionCounter)
        Alarm_Memory_Minutes(3) = GetByte(RawData, myPositionCounter)
        Alarm_Memory_Day(3) = GetByte(RawData, myPositionCounter)

        Alarm_Memory_Typ(4) = GetInteger(RawData, myPositionCounter)
        Alarm_Memory_Sensor(4) = GetByte(RawData, myPositionCounter)
        Alarm_Memory_Hours(4) = GetByte(RawData, myPositionCounter)
        Alarm_Memory_Minutes(4) = GetByte(RawData, myPositionCounter)
        Alarm_Memory_Day(4) = GetByte(RawData, myPositionCounter)

        Alarm_Memory_Typ(5) = GetInteger(RawData, myPositionCounter)
        Alarm_Memory_Sensor(5) = GetByte(RawData, myPositionCounter)
        Alarm_Memory_Hours(5) = GetByte(RawData, myPositionCounter)
        Alarm_Memory_Minutes(5) = GetByte(RawData, myPositionCounter)
        Alarm_Memory_Day(5) = GetByte(RawData, myPositionCounter)

        Firmware_Name = GetString(RawData, myPositionCounter, 6)

        LCD_Type = GetByte(RawData, myPositionCounter)
        OS_Version = GetInteger(RawData, myPositionCounter)
        FirmwareRevision = GetInteger(RawData, myPositionCounter)
        Flashcounter = GetInteger(RawData, myPositionCounter)
        HardwareRevision = GetInteger(RawData, myPositionCounter)
        Serialnumber = GetInteger(RawData, myPositionCounter)
        ProductionMonth = GetByte(RawData, myPositionCounter)
        ProductionYear = GetByte(RawData, myPositionCounter)
        Language = GetString(RawData, myPositionCounter, 4)
        Expansion_RO = GetString(RawData, myPositionCounter, 24)

    End Function

    Function CreateStringFromStructure(ByVal Protokol As TransferTypeRegister) As String

        Dim RawData As String = ""

        AddByte(Protokol, RawData)

        'Daten in RAM oder EEPROM schreiben
        If Protokol = TransferTypeRegister.RAM Or _
            Protokol = TransferTypeRegister.EEPROM_SAVE Or _
            Protokol = TransferTypeRegister.PROFIL_LOAD Or _
            Protokol = TransferTypeRegister.PROFIL_COPY Or _
            Protokol = TransferTypeRegister.AQUABUS_READ Or _
            Protokol = TransferTypeRegister.AQUABUS_WRITE Then

            'Struktur aufbauen
            'Globale Daten
            AddTextString(Fan_Name(1), 11, RawData)
            AddTextString(Fan_Name(2), 11, RawData)
            AddTextString(Fan_Name(3), 11, RawData)
            AddTextString(Fan_Name(4), 11, RawData)

            AddTextString(Flowsensor_Name, 11, RawData)

            AddTextString(Sensor_Name(1), 11, RawData)
            AddTextString(Sensor_Name(2), 11, RawData)
            AddTextString(Sensor_Name(3), 11, RawData)
            AddTextString(Sensor_Name(4), 11, RawData)
            AddTextString(Sensor_Name(5), 11, RawData)
            AddTextString(Sensor_Name(6), 11, RawData)

            AddTextString(aquaero_Name, 9, RawData)

            AddByte(Profil, RawData)

            AddByte(Fan_MinimumPower(1), RawData)
            AddByte(Fan_MinimumPower(2), RawData)
            AddByte(Fan_MinimumPower(3), RawData)
            AddByte(Fan_MinimumPower(4), RawData)

            AddInteger(Fan_Pulse(1), RawData)
            AddInteger(Fan_Pulse(2), RawData)
            AddInteger(Fan_Pulse(3), RawData)
            AddInteger(Fan_Pulse(4), RawData)

            AddInteger(Flowsensor_Pulse, RawData)

            AddByte(LCD_Backlight, RawData)
            AddByte(LCD_Contrast, RawData)
            AddInteger(LCD_Fadingtime, RawData)

            AddByte(Sensor_Faktor(1), RawData)
            AddByte(Sensor_Faktor(2), RawData)
            AddByte(Sensor_Faktor(3), RawData)
            AddByte(Sensor_Faktor(4), RawData)
            AddByte(Sensor_Faktor(5), RawData)
            AddByte(Sensor_Faktor(6), RawData)

            AddByte(Sensor_Offset(1), RawData)
            AddByte(Sensor_Offset(2), RawData)
            AddByte(Sensor_Offset(3), RawData)
            AddByte(Sensor_Offset(4), RawData)
            AddByte(Sensor_Offset(5), RawData)
            AddByte(Sensor_Offset(6), RawData)

            AddInteger(Sensor_Alarm(1), RawData)
            AddInteger(Sensor_Alarm(2), RawData)
            AddInteger(Sensor_Alarm(3), RawData)
            AddInteger(Sensor_Alarm(4), RawData)
            AddInteger(Sensor_Alarm(5), RawData)
            AddInteger(Sensor_Alarm(6), RawData)

            'Ab hier Profildaten 
            AddByte(LCD_Settings, RawData)
            AddByte(LCD_PageDelay, RawData)
            AddLong(LCD_Page, RawData)

            AddByte(Powersensor_1_Sensor, RawData)
            AddByte(Powersensor_2_Sensor, RawData)
            AddByte(Powersensor_Config, RawData)

            AddByte(tubemeter_Config, RawData)
            AddByte(tubemeter_Alarmlevel, RawData)
            AddByte(tubemeter_Alarmmove, RawData)
            AddByte(tubemeter_Expansion, RawData)

            AddByte(Fan_Sensor(1), RawData)
            AddByte(Fan_Sensor(2), RawData)
            AddByte(Fan_Sensor(3), RawData)
            AddByte(Fan_Sensor(4), RawData)

            AddInteger(Fan_SetRPM(1), RawData)
            AddInteger(Fan_SetRPM(2), RawData)
            AddInteger(Fan_SetRPM(3), RawData)
            AddInteger(Fan_SetRPM(4), RawData)

            AddByte(Fan_SetPower(1), RawData)
            AddByte(Fan_SetPower(2), RawData)
            AddByte(Fan_SetPower(3), RawData)
            AddByte(Fan_SetPower(4), RawData)

            AddInteger(Fan_Mode(1), RawData)
            AddInteger(Fan_Mode(2), RawData)
            AddInteger(Fan_Mode(3), RawData)
            AddInteger(Fan_Mode(4), RawData)

            AddInteger(Flowsensor_Mode, RawData)
            AddInteger(FlowSensor_lAlarm(1), RawData)
            AddInteger(FlowSensor_lAlarm(2), RawData)

            AddByte(LED_PWM(1), RawData)
            AddByte(LED_PWM(2), RawData)
            AddByte(LED_Mode(1), RawData)
            AddByte(LED_Mode(2), RawData)
            AddByte(LED_Sensor(1), RawData)
            AddByte(LED_Sensor(2), RawData)

            AddInteger(Sensor_Min(1), RawData)
            AddInteger(Sensor_Min(2), RawData)
            AddInteger(Sensor_Min(3), RawData)
            AddInteger(Sensor_Min(4), RawData)
            AddInteger(Sensor_Min(5), RawData)
            AddInteger(Sensor_Min(6), RawData)

            AddInteger(Sensor_Max(1), RawData)
            AddInteger(Sensor_Max(2), RawData)
            AddInteger(Sensor_Max(3), RawData)
            AddInteger(Sensor_Max(4), RawData)
            AddInteger(Sensor_Max(5), RawData)
            AddInteger(Sensor_Max(6), RawData)

            AddByte(Sensor_Hyst(1), RawData)
            AddByte(Sensor_Hyst(2), RawData)
            AddByte(Sensor_Hyst(3), RawData)
            AddByte(Sensor_Hyst(4), RawData)
            AddByte(Sensor_Hyst(5), RawData)
            AddByte(Sensor_Hyst(6), RawData)

            AddInteger(Sensor_Opt(1), RawData)
            AddInteger(Sensor_Opt(2), RawData)
            AddInteger(Sensor_Opt(3), RawData)
            AddInteger(Sensor_Opt(4), RawData)
            AddInteger(Sensor_Opt(5), RawData)
            AddInteger(Sensor_Opt(6), RawData)

            AddByte(Controler_Time(1), RawData)
            AddByte(Controler_Time(2), RawData)
            AddByte(Controler_Time(3), RawData)
            AddByte(Controler_Time(4), RawData)
            AddByte(Controler_Time(5), RawData)
            AddByte(Controler_Time(6), RawData)

            AddByte(Controler_Factor(1), RawData)
            AddByte(Controler_Factor(2), RawData)
            AddByte(Controler_Factor(3), RawData)
            AddByte(Controler_Factor(4), RawData)
            AddByte(Controler_Factor(5), RawData)
            AddByte(Controler_Factor(6), RawData)

            AddInteger(Alarm_Setup, RawData)
            AddByte(Alarm_ShutdownDelay, RawData)

            AddByte(Timer_Off_Hours(1), RawData)
            AddByte(Timer_Off_Minutes(1), RawData)
            AddByte(Timer_Off_Seconds(1), RawData)
            AddByte(Timer_Off_Day(1), RawData)
            AddByte(Timer_Off_SwitchTime(1), RawData)

            AddByte(Timer_Off_Hours(2), RawData)
            AddByte(Timer_Off_Minutes(2), RawData)
            AddByte(Timer_Off_Seconds(2), RawData)
            AddByte(Timer_Off_Day(2), RawData)
            AddByte(Timer_Off_SwitchTime(2), RawData)

            AddByte(Timer_On_Hours(1), RawData)
            AddByte(Timer_On_Minutes(1), RawData)
            AddByte(Timer_On_Seconds(1), RawData)
            AddByte(Timer_On_Day(1), RawData)
            AddByte(Timer_On_SwitchTime(1), RawData)

            AddByte(Timer_On_Hours(2), RawData)
            AddByte(Timer_On_Minutes(2), RawData)
            AddByte(Timer_On_Seconds(2), RawData)
            AddByte(Timer_On_Day(2), RawData)
            AddByte(Timer_On_SwitchTime(2), RawData)

            AddByte(aquastream_Command(1), RawData)
            AddByte(aquastream_Command(2), RawData)

            AddByte(Relay_ShutdownTime, RawData)
            AddByte(Relay_Status, RawData)

            AddByte(multiswitch_AMP_Register(1), RawData)
            AddByte(multiswitch_LED_Register(1), RawData)
            AddByte(multiswitch_AMP_Register(2), RawData)
            AddByte(multiswitch_LED_Register(2), RawData)

            AddByte(multiswitch_Bargraph_AMP(1), RawData)
            AddByte(multiswitch_Bargraph_LED(1), RawData)
            AddByte(multiswitch_Status(1), RawData)
            AddByte(multiswitch_Bargraph_AMP(2), RawData)
            AddByte(multiswitch_Bargraph_LED(2), RawData)
            AddByte(multiswitch_Status(2), RawData)

            'Ende Profil
            AddString(Expansion_RW, 32, RawData)
            AddString(aquabus_Buffer, 24, RawData)

            AddByte(Time_Hours, RawData)
            AddByte(Time_Minutes, RawData)
            AddByte(Time_Seconds, RawData)
            AddByte(Time_Day, RawData)

            AddByte(aquaero_Control, RawData)
            AddByte(aquaero_Screen, RawData)
        End If

        'Firmware flashen
        If Protokol = TransferTypeRegister.FLASH Then

            Dim Checksum As Integer

            AddInteger(Flash_Adress, RawData) 'Adresse
            AddString(Flash_Data, 256, RawData) 'Daten
            AddInteger(Flash_Check, RawData) 'Checksumme
            AddString(Flash_Key, 2, RawData) 'Schlüssel zum flashen 

        End If

        'Firmware löschen
        If Protokol = TransferTypeRegister.FLASH_DELETE Then
            AddString(Flash_Delete_Key, 31, RawData) 'Schlüssel zum flashen 

        End If

        If Protokol = TransferTypeRegister.USER1 Then
            Dim j As Integer
            AddByte(FanTestMaxRpm, RawData)

            For j = 0 To 400
                AddByte(0, RawData)
            Next

        End If

        'USB-LCD-Ausgabe (nur einzelne Zeichen vorgesehen)
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


        Return RawData

    End Function

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

End Class
