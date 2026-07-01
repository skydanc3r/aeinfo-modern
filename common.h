// -*- c++ -*- $Id: common.h 35 2007-05-05 15:09:55Z matt $

/* Some often-used functions
 * 
 * Copyright (C) 2006 Christian Unger <coder at breakbe D0T at>
 * Translated by John Frushour
 * 
 * This library is covered by the LGPL, read LICENSE for details.
 */

typedef unsigned char Byte;

class AbnormalTerminationError
{
public:
  AbnormalTerminationError () {}
};

class DeviceError
  : public AbnormalTerminationError
{
  string msg;
public:
  DeviceError (const char *c) : msg(c) {}
  string getMessage () const
  {
    return msg;
  }
};

bool startsWith ( const string& a, const string& b )
{
  if( b.length() <= a.length() ) {
    
    return b == a.substr(0,b.length());
  }else{
    
    return a == b.substr(0,a.length());
  }
}

bool endsWith ( const string& a, const string& b ) 
{
  if( b.length() <= a.length() ) {
    
    return b == a.substr( a.length() - b.length(), a.length() );
  }else{

    return a == b.substr( b.length() - a.length(), b.length() );
  }
}

string trim ( const string& s )
{
  string::const_reverse_iterator till = s.rbegin(), rend = s.rend();
  while( till != rend
         && (*till == ' ' || *till == '\t' || *till == '\n' || *till == '\r') ) {
    ++till;
  }
  
  string::const_iterator from = s.begin(), end = s.end(), begin = s.begin();
  while( from != end
         && (*from == ' ' || *from == '\t' || *from == '\n' || *from == '\r') ) {
    ++from;
  }
  
  return s.substr( from - begin, (rend - till) - (from - begin) );
}

#ifdef _WIN32
# ifndef WINDOWS
#  define WINDOWS 1
# endif
#endif
#ifdef WIN32
# ifndef WINDOWS
#  define WINDOWS 1
# endif
#endif

#ifdef WINDOWS
//#undef PINT
//namespace win32 {
typedef unsigned char byte;

# include <windows.h>
  
//};
//using namespace win32;
#endif


enum output_color
  {
    black,
    red,
    green,
    brown,
    blue,
    magenta,
    cyan,
    gray,
    
    dk_gray,
    lt_red,
    lt_green,
    yellow,
    lt_blue,
    pink,
    lt_cyan,
    white
#ifdef WINDOWS
    , bold, resetcolors
#endif
  };


const string color ( const output_color fg ) 
{
#ifdef USE_COLORS
# ifndef WINDOWS
  string ret;
  ostringstream oss;
  string add = "";
  int base = 30;
  switch( fg ) {
  case dk_gray:
  case lt_red:
  case lt_green:
  case yellow:
  case lt_blue:
  case pink:
  case lt_cyan:
  case white:
    add = ";1";
    base = 30 - 8;
    break;
  default:
    break;
    
  }
  oss << "\033[" << (base + fg) << add << "m";
  ret = oss.str();
  return ret;
# else
  int r=0,g=0,b=0,i=0;
  switch( fg ) {
  case dk_gray: i=1;break;
  case black: break;
    
  case lt_red: i=1;
  case red: r=1; break;
    
  case lt_green: i=1;
  case green: g=1;break;
    
  case yellow: i=1;
  case brown: r=g=1;break;
    
  case lt_blue: i=1;
  case blue: b=1;break;
    
  case pink: i=1;
  case magenta: r=b=1;break;
    
  case lt_cyan: i=1;
  case cyan: g=b=1;break;
    
  case bold:
  case white: i=1;
  case resetcolors: 
  case gray: r=g=b=1;break;
  }
  int v = 0;
  if( r ) v |= FOREGROUND_RED;
  if( g ) v |= FOREGROUND_GREEN;
  if( b ) v |= FOREGROUND_BLUE;
  if( i ) v |= FOREGROUND_INTENSITY;
  
  cout.flush();
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),v);
  return "";
# endif
#else
  return "";
#endif
}

#ifndef WINDOWS
# ifdef USE_COLORS
const string bold = "\033[1m";
const string resetcolors = "\033[0m";
# else
const string bold = "";
const string resetcolors = "";
# endif
#endif


ostream& operator << ( ostream &ostr, output_color col ) 
{
  ostr << color(col);
  return ostr;
}

