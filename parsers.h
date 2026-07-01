// -*- c++ -*- $Id: parsers.h 35 2007-05-05 15:09:55Z matt $

/* Parsing-Routines
 * 
 * Copyright (C) 2006 Christian Unger <coder at breakbe D0T at>
 * Translated by John Frushour
 * 
 * This library is covered by the LGPL, read LICENSE for details.
 */

#define _PARSERS_H

/** Algorithm to parse and print (unparse) values are encapsulated in
 *  these classes here...
 */
#include <cstdlib>
#include <cstring>


/** Occurs at unparsable input.
 */
class ParseException
{
};

/** Occurs when trying to "set" a RO-Property
 */
class ReadOnlyException
{
};


/** converts a `string' into a `long'
 */
long parseLong ( string s ) 
{
#ifdef _WIN32
  string::reverse_iterator
    till = s.rbegin(), rend = s.rend();
#else
  string::const_reverse_iterator
    till = s.rbegin(), rend = s.rend();
#endif
    
  while( till != rend
         && (*till == ' ' || *till == '\t' || *till == '\n' || *till == '\r') ) {
    ++till;
  }

  string tmp = s.substr(0,rend - till);
  const char * txt = tmp.c_str();
  char * err;
  long ret = strtol(txt,&err,10);
  if( (*txt == '\0')
      || ((*txt != '\0') && (*err == '\0')) ) {
    // parsing was OK
  }else{
    throw new ParseException();
  }
  
  return ret;
}


/** This Dummy-Type is needed for Automatic-Parsers!
 */
class Dummy
{
public: Dummy (int) { }
};


ostream& operator << ( ostream&ostr, const Dummy& d )
{
  ostr << "<ERROR:Dummy>";
  return ostr;
}







/** A ValueParser converts a string into a value of type T and vice versa.
 */
template <typename T>
class ValueParser
{
public:
  ValueParser ()
  {
  }

  /** GCC wants this
   *  thanks to "schnetzer"
   */
  virtual ~ValueParser ()
  {
  } 
  
  virtual T parse ( string s ) 
  {
    return T(0);
  }
  
  virtual void unparse ( ostream &ostr, const T& v ) const 
  {
    ostr << v;
  }
};

/** Default implementation for string->string conversion
 */
template <>
string ValueParser<string>:: parse ( string s ) 
{
  return s;
}

/** Default implementation for string->int conversion
 */
template <>
int ValueParser<int>:: parse ( string s )
{
  return (int)parseLong(s);
}

/** Default implementation for string->long conversion
 */
template <>
long ValueParser<long>:: parse ( string s )
{
  return parseLong(s);
}

/** Default implementation for string->Byte conversion
 */
template <>
unsigned char ValueParser<unsigned char>:: parse ( string s )
{
  return (unsigned char)(0xFF & parseLong(s));
}

/** Default implementation for Byte->string conversion
 */
template <>
void ValueParser<unsigned char>:: unparse ( ostream &ostr, const unsigned char& v ) const 
{
  ostr << (int)v;
}



/** Automatic-Parsers are some sort of "Dummies", which will actually not be used for parsing,
 *  but for automatically determining a typename of a Property.
 */
class AutomaticParser
  : public ValueParser<Dummy>
{
public:
  bool ro;
  bool isEnum;
  
  AutomaticParser ( bool rdOnly )
    : ro(rdOnly), isEnum(false)
  {
  }
  
  AutomaticParser ( bool rdOnly, bool isEnum )
    : ro(rdOnly), isEnum(isEnum)
  {
  }
};






/** Converts a string into a number and applies a linear transformation to it - and vice versa.
 */
template<typename T>
class LinearTransformParser
  : public ValueParser<T>
{
  float m;
  float t;
  int precision;
  
public:
  LinearTransformParser ( float m, float t )
    : m(m), t(t), precision(3)
  {
  }

  LinearTransformParser ( float m, float t, int precision )
    : m(m), t(t), precision(precision)
  {
  }
  
  virtual T parse ( string s ) 
  {
    float v = atof(s.c_str());
    
    return T(v * m + t + 1/(float)2);
  }
  
  virtual void unparse ( ostream &ostr, const T& v ) const 
  {
    float transformed = (v - t) / m;

    ostr << setprecision(precision) << transformed;
  }
};



/** string<->string conversion
 */
class StringParser
  : public ValueParser<string>
{
public:
  StringParser ()
  {
  }
  
  virtual string parse ( string s ) 
  {
    return s;
  }
  virtual void unparse ( ostream &ostr, const string& s ) const 
  {
    ostr << s;
  }
};



/** This is acutally not a parser - but this class may be used as a
 *  superclass, in order to provide a mechanism for "boxing" of parsers.
 */
template<typename T>
class LinkingParser
{
protected:
  ValueParser<T>* linked;
  
public:
  LinkingParser ( ValueParser<T>* linked )
    : linked(linked)
  {
  }
  
  ~LinkingParser () 
  {
    delete linked;
  }
  
  void setLinkedParser ( ValueParser<T>* v ) 
  {
    if( linked != NULL ) delete linked;
    
    linked = v;
  }
  const ValueParser<T>* getLinkedParser () const 
  {
    return linked;
  }
  const ValueParser<T>* getLastLinkedParser () const 
  {
    if( dynamic_cast<LinkingParser<T>*>(linked) != NULL ) {
      return dynamic_cast<LinkingParser<T>*>(linked)->getLastLinkedParser();
    }
    return linked;
  }
};



/** A ReadOnlyParser only allows conversion from T->string
 *  (which means, that no input is allowed).
 */
template<typename T>
class ReadOnlyParser
  : public ValueParser<T>, public LinkingParser<T>
{
public:
  ReadOnlyParser ()
    : LinkingParser<T>(NULL)
  {
  }
  ReadOnlyParser ( ValueParser<T>* linked )
    : LinkingParser<T>(linked)
  {
  }
  
  virtual ~ReadOnlyParser () 
  {
  }
  
  virtual T parse ( string s )  
  {
    throw new ReadOnlyException();
  }
  
  virtual void unparse ( ostream &ostr, const T& v ) const 
  {
    if( LinkingParser<T>::linked != NULL ) {
      LinkingParser<T>::linked->unparse(ostr,v);
    }else{
      ValueParser<T>::unparse(ostr,v);
    }
  }
};



/** Converts raw-data into huamn-readable from - and vice versa.
 */
class RawDataParser
  : public ValueParser<string>
{
public:
  RawDataParser ()
  {
  }
  
  virtual string parse ( string s ) 
  {
    throw "ERROR: RawDataParser:: parse(string) is not implemented!";
  }
  virtual void unparse ( ostream &ostr, const string& v ) const 
  {
    string::const_iterator it = v.begin(), end = v.end();
    for( ; it != end; ++it ) {
      ostr << hex << setw(2) << setfill('0') << (int)((unsigned char)*it);
      if( it != end ) ostr << ' ';
    }
    ostr << dec;
  }
};



/** A discrete set of numerical values may contain a value that has a special meaning.
 *  With this class one can accomplish such a association.
 */
template<typename T>
class NamedValueParser
  : public ValueParser<T>,
    public LinkingParser<T>
{
  T value;
  string name;
  
public:
  NamedValueParser ( ValueParser<T>* linked, T value, string name )
    : LinkingParser<T>(linked), value(value), name(name)
  {
  }
  virtual ~NamedValueParser () 
  {
  }
  
  virtual T parse ( string s )
  {
    if( trim(s) == name ) {
      return value;
    }
    return LinkingParser<T>::linked->parse(s);
  }
  
  virtual void unparse ( ostream &ostr, const T& v ) const 
  {
    if( v == value ) {
      ostr << name;
      return;
    }
    LinkingParser<T>::linked->unparse(ostr,v);
  }
};










/** Prints a enumValue.
 */
void handleEnumValueOutput ( ostream& ostr, string enumName, int v )
{
  int knownBits = 0;
  bool printPipe = false;
  bool isBitField = bitFields.find(enumName) != bitFields.end();
  
  map<int,string>::const_iterator it = stringEnum_names[enumName].begin(),
    end = stringEnum_names[enumName].end();
  
  ostr << v << " = ";
  if( isBitField && v == 0 ) {
    ostr << "UNUSED";
  }
  
  for( ; it != end; ++it ) {
    if( isBitField ) {
      knownBits |= it->first;
      
      if( (it->first & v) != 0 ) {
        if( printPipe ) ostr << " | ";
        else printPipe = true;
        
        ostr << it->second;
      }
    }else{
      if( it->first == v ) {
        ostr << it->second;
        return;
      }
    }
  }
  
  if( isBitField
      && ((knownBits & v) != v) ) {
    ostr << " + UNKNOWN ENUM-BITS!! (" << enumName << ", " << v << ")";
  }else if( !isBitField ) {
    if( enumName == "aquastream_CommandRegister" ) {
      ostr << "Frequency";
      return;
    }
    if( enumName == "aquaero_ScreenRegister" ) {
      ostr << "?";
      return;
    }
    
    ostr << " UNKNOWN ENUM-VALUE!! (" << enumName << ", " << v << ")";
  }
  
}

/** This is not a ValueParser (since it converts a string-representation of an
 *  enum-value to an ordinary int).
 */
class StringEnumParser
{
  string s;
  string enumName;
  
  string::const_iterator begin, it, end;
  
  bool isWS () 
  {
    return !((*it != ' ') && (*it != '\t') && (*it != '\n') && (*it != '\r'));
  }
  
  void eatWS () 
  {
    while( it != end
           && isWS() ) {
      ++it;
    }
  }
  
  string readToken () 
  {
    string::const_iterator tokenStart = it;
    
    while( it != end
           && !isWS() 
           && (*it != '|') ) {
      ++it;
    }
    
    return s.substr( tokenStart - begin, it - tokenStart );
  }

  string readPipe () 
  {
    string::const_iterator tokenStart = it;
    
    if( it != end
        && (*it == '|') ) {
      ++it;
    }
    // For double-pipe...
    // if( it != end
    //     && (*it == '|') ) {
    //   ++it;
    // }
    
    return s.substr( tokenStart - begin, it - tokenStart );
  }
  
  set<string> readOrList () 
  {
    set<string> tokens;
    
    for( ; true; ) {
      eatWS();
      
      string token = readToken();
      if( token != "" ) tokens.insert( token );
      else return tokens;
      
      eatWS();
      
      token = readPipe();
      if( token == "" ) return tokens;
    }
    
    return tokens;
  }
  
public:
  StringEnumParser ( string s, string enumName )
    : s(s), enumName(enumName)
  {
  }
  
  int parse () 
  {
    if( s == "UNUSED" ) return 0;
    
    begin = s.begin();
    it = s.begin();
    end = s.end();
    
    set<string> tokens = readOrList();
    
#ifdef _WIN32
    set<string>::iterator send = tokens.end();
#else
    set<string>::const_iterator send = tokens.end();
#endif
    
    map<int,string>::const_iterator
      mit  = stringEnum_names[enumName].begin(),
      mend = stringEnum_names[enumName].end();
    
    int value = 0;
    for( ; mit != mend; ++mit ) {
      if( tokens.find(mit->second) == send ) {
        // not given
      }else{
        // found
        value = (value | mit->first);
        tokens.erase(mit->second);
      }
    }
    
    set<string>::const_iterator
      sit  = tokens.begin();
    for( ; sit != send; ++sit ) {
      cerr << "ERROR: Enum-Value does not exist: \"" << *sit << "\"" << endl;
      // FIXME: throw an exception?!
    }
    
    return value;
  }
};







/** Converts T->string and vice versa. In practice, T is an enum.
 */
template<typename T>
class EnumParser
  : public ValueParser<T>
{
  string enumName;
  
  
public:
  EnumParser ( string enumName )
    : enumName(enumName)
  {
  }
  virtual ~EnumParser () 
  {
  }
  
  virtual T parse ( string s ) 
  {
    //return T(0);
    try {
      int v =
        //ValueParser<int>::parse(s);
        parseLong(s);
      
      return (T)v;
    } catch ( ParseException* pe ) {
    // try to extract a text-representation 
    }
    
    s = trim(s);
    
    StringEnumParser sep(s,enumName);
    
    return (T)sep.parse();
  }
  
  virtual void unparse ( ostream &ostr, const T& theValue ) const 
  {
    handleEnumValueOutput(ostr, enumName, theValue);
  }
};

template <>
string EnumParser<string>:: parse ( string s ) 
{
  return "";
}

template <>
void EnumParser<string>:: unparse ( ostream &ostr, const string& v ) const 
{
  
}



