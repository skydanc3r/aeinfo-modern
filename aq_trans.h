// -*- c++ -*- $Id: aq_trans.h 16 2006-01-31 20:58:13Z matt $

/* Generated file: General Data-Conversion tasks
 * 
 * Copyright (C) 2006 Christian Unger <coder at breakbe D0T at>
 * Translated by John Frushour
 * 
 * This library is covered by the LGPL, read LICENSE for details.
 */

/* WARNING: This file has been generated (semi-)automatically!
 *          DO NOT EDIT IT UNLESS YOU KNOW WHAT YOU ARE DOING
 */

/* FIXME: This file may want to see some big-/little-endian checks...
 */


#define byte unsigned char


  static unsigned char Asc ( string s ) 
  {
    return s[0];
  }

  static string Chr( unsigned int i ) 
  {
    char tmp[1];
    tmp[0] = (char)i;
    return string(tmp,1);
  
  }

  static int Len ( string s ) 
  {
    return s.length();
  }


  static string GetTextString(string RawData, int& PositionCounter, int Length) 
  { 
    string Character; 
    string StringBuffer; 
    if ((PositionCounter + Length) > (int)RawData.length()) { 
      return ""; 
    } 
    try { 
      for (int i = 1; i <= Length; i++) { 
        Character = RawData.substr(PositionCounter, 1); 
        if (Character == "") { 
          Character = "\0"; 
        } 
        if (Asc(Character) == 0) { 
          Character = Chr(32); 
        } else if (Asc(Character) == 245) { 
          Character = Chr(252); 
        } else if (Asc(Character) == 225) { 
          Character = Chr(228); 
        } else if (Asc(Character) == 239) { 
          Character = Chr(246); 
        } else if (Asc(Character) == 226) { 
          Character = Chr(223); 
        } else if (Asc(Character) == 228) { 
          Character = Chr(181); 
        } 
        StringBuffer = StringBuffer + Character; 
        PositionCounter = PositionCounter + 1; 
      } 
    } /*catch (string ex) { 
      return " "; 
      } */
    catch ( string s ) {
      cerr << s << endl;
    }
    return StringBuffer; 
  }


  static byte GetByte(string RawData, int& PositionCounter) 
  { 
    byte Buffer; 
    if ((PositionCounter) > (int)RawData.length()) { 
      return 0;
    } 
    //try { 
    Buffer = Asc(RawData.substr(PositionCounter, 1)); 
    PositionCounter = PositionCounter + 1; 
    /*} catch (Exception ex) { 
      return (0); 
      } */
    return Buffer; 
  }

  static int GetInteger(string RawData, int& PositionCounter) 
  { 
    int Buffer; 
    if ((PositionCounter + 1) > (int)RawData.length()) { 
      return 0;
    } 
    // try { 
    Buffer = (Asc(RawData.substr(PositionCounter, 1)) * 256) + Asc(RawData.substr(PositionCounter + 1, 1)); 
    PositionCounter = PositionCounter + 2; 
    // } catch (Exception ex) { 
    //   return (0); 
    // } 
    return Buffer; 
  } 
 
  static long GetLong(string RawData, int& PositionCounter) 
  { 
    long Buffer; 
    if ((PositionCounter + 3) > (int)RawData.length()) { 
      return 0; 
    } 
    // try { 
    Buffer = Asc(RawData.substr(PositionCounter, 1)) * (16777216) + Asc(RawData.substr(PositionCounter + 1, 1)) * (65536) + Asc(RawData.substr(PositionCounter + 2, 1)) * (256) + Asc(RawData.substr(PositionCounter + 3, 1)); 
    PositionCounter = PositionCounter + 4; 
    return Buffer; 
    // } catch (Exception ex) { 
    //   return (0); 
    // } 
  } 
 
  static string GetString(string RawData, int& PositionCounter, int Length) 
  { 
    string StringBuffer; 
    if ((PositionCounter + Length) > (int)RawData.length()) { 
      return 0; 
    } 
    // try { 
    StringBuffer = RawData.substr(PositionCounter, Length); 
    // if (Asc(StringBuffer) == 0) { 
    //   StringBuffer = Chr(32); 
    // } 
    PositionCounter = PositionCounter + Length; 
    // } catch (Exception ex) { 
    //   return (" "); 
    // } 
    return StringBuffer; 
  } 


  static void AddByte(byte myByte, string& RawData) 
  { 
    //try { 
      RawData += Chr(myByte); 
    // } catch (Exception ex) { 
    //   HandleException("AddByte", ex); 
    // } 
  } 
 
  static void AddTextString(string myString, int Length, string& RawData) 
  { 
    string Character; 
    // try { 
      for (int i = 0; i <= (Length - 1); i++) { 
        if (i > (Len(myString) - 1)) { 
          Character = Chr(32);
        } else { 
          Character = myString.substr(i, 1); 
          if (Asc(Character) == 252 || Asc(Character) == 220) { 
            Character = Chr(245); 
          } else if (Asc(Character) == 196 || Asc(Character) == 192 || Asc(Character) == 193) { 
            Character = Chr(225); 
          } else if (Asc(Character) == 228 || Asc(Character) == 196 || Asc(Character) == 192 || Asc(Character) == 193) { 
            Character = Chr(225); 
          } else if (Asc(Character) == 201 || Asc(Character) == 200 || Asc(Character) == 202) { 
            Character = Chr(69); 
          } else if (Asc(Character) == 237 || Asc(Character) == 236) { 
            Character = Chr(105); 
          } else if (Asc(Character) == 204 || Asc(Character) == 205) { 
            Character = Chr(73); 
          } else if (Asc(Character) == 246 || Asc(Character) == 214) { 
            Character = Chr(239); 
          } else if (Asc(Character) == 243 || Asc(Character) == 242) { 
            Character = Chr(111); 
          } else if (Asc(Character) == 223) { 
            Character = Chr(226); 
          } else if (Asc(Character) == 233 || Asc(Character) == 232) { 
            Character = Chr(101); 
          } else if (Asc(Character) == 181) { 
            Character = Chr(228); 
          } 
        } 
        RawData += Character; 
      } 
    // } catch (Exception ex) { 
    //   HandleException("AddTextString", ex); 
    // } 
  }

  static void AddString(string myString, int Length, string& RawData) 
  { 
    string Character; 
    // try { 
      for (int i = 0; i <= (Length - 1); i++) { 
        if (i > (Len(myString) - 1)) { 
          Character = Chr(0); 
        } else { 
          Character = myString.substr(i, 1); 
        } 
        RawData += Character; 
      } 
    // } catch (Exception ex) { 
    //   HandleException("AddString", ex); 
    // } 
  } 
 
  static void AddInteger(int myInteger, string& RawData) 
  { 
    // int a; 
    // int b; 
    // try { 
    //   a = Int(myInteger / (Math.Pow(2, 8))); 
    //   b = myInteger - (a * (Math.Pow(2, 8))); 
    //   RawData += Chr(a); 
    //   RawData += Chr(b);
    RawData += Chr( (myInteger >> 8) & 0xFF );
    RawData += Chr( myInteger & 0xFF );
    // } catch (Exception ex) { 
    //   HandleException("AddInteger", ex); 
    // } 
  } 
 
  static void AddLong(long myLong, string& RawData) 
  { 
    // int a; 
    // int b; 
    // int c; 
    // int d; 
    // long Rest; 
    // try { 
    //   a = myLong / (Math.Pow(2, 24)); 
    //   myLong = myLong - (a * (Math.Pow(2, 24))); 
    //   b = myLong / (Math.Pow(2, 16)); 
    //   myLong = myLong - (b * (Math.Pow(2, 16))); 
    //   c = myLong / (Math.Pow(2, 8)); 
    //   d = myLong - (c * (Math.Pow(2, 8))); 
    //   RawData += Chr(a); 
    //   RawData += Chr(b); 
    //   RawData += Chr(c); 
    //   RawData += Chr(d);
    RawData += Chr( (myLong >> 24) & 0XFF );
    RawData += Chr( (myLong >> 16) & 0XFF );
    RawData += Chr( (myLong >>  8) & 0XFF );
    RawData += Chr( (myLong >>  0) & 0XFF );
    // } catch (Exception ex) { 
    //   HandleException("AddLong", ex); 
    // } 
  } 
 
