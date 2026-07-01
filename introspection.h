// -*- c++ -*- $Id: introspection.h 30 2006-09-27 00:35:25Z matt $

/* Introspection-classes
 * 
 * Copyright (C) 2006 Christian Unger <coder at breakbe D0T at>
 * Translated by John Frushour
 * 
 * This library is covered by the LGPL, read LICENSE for details.
 */

#define _INTROSPECTION_H

/** This is the Base-class of all PropertyDescriptors. This allows us to
 *  create a homogenous container.
 */
class PropertyDescriptorBase 
{
  string name;
  string description;
public:
  PropertyDescriptorBase(string name, string description)
    : name(name), description(description)
  {
  }
  virtual ~PropertyDescriptorBase () 
  {
  }
  
  /** Returns the name of an property. For example: "sensor.2.temp"
   */
  const string& getName () const 
  {
    return name;
  }
  
  /** Returns the description of an property. For example: "Aktuelle Temperatur"
   */
  const string& getDescription () const 
  {
    return description;
  }
  
  /** Prints (unparses) the current value of this property to an ostream.
   */
  virtual void printValue ( ostream& ostr ) const = 0;
  
  /** Prints (unparses) the old value (before all "set" operations) to an ostream
   */ 
  virtual void printOldValue ( ostream& ostr ) const = 0;
  
  /** Performs a "set" operation - this will convert (parse) the provided string into
   *  the internal value of this property and change the current value.
   */
  virtual void readValue ( string s ) = 0;
  
  /** If the internal type of this property is an enum, this method must return the
   *  string-representation name of the enum (e.g. "Fan_ModeRegister"). Otherwise,
   *  this method throws a "const char*"
   */
  virtual string getEnumName () const throw(const char*) = 0;
  
  // string getTypeString ( ) 
  // {
  //   try {
  //     string ret = "enum "+getEnumName();
  //   }
  //   catch ( ... ) {
  //   }
  //   return typenameToString<T>();
  // }
};


extern map<string,PropertyDescriptorBase*> properties;


template <class T>
class PropertyDescriptor
  : public PropertyDescriptorBase
{
  T *element;
  T oldValue;
  typedef T internal_t;
  
protected:
  ValueParser<T> *parser;
  
public:
  
  /** constructs a PropertyDescriptor. An instance of this class usually points to
   *  an attribute of AquaeroData_rev4. The parser is used for converting the
   *  strings into values of type T.
   */
  PropertyDescriptor ( T *elem, string name, string description, ValueParser<T> *aparser )
    : PropertyDescriptorBase(name,description), element(elem), parser(aparser)
  {
    oldValue = *element;
    
    try {
      // in order to support enum's transparently, we have to re-adjust things
      string enumName = getEnumName();
      
      // automatically instanciate a EnumParser, since this element is an enum-type
      if( dynamic_cast<LinkingParser<T>*>(this->parser) != NULL ) {
        if( dynamic_cast<LinkingParser<T>*>(this->parser)->getLinkedParser() == NULL ) {
          EnumParser<T>  *nuParser = new EnumParser<T>(enumName);
          if(VERBOSE >= 3) cout << "creating RO for " << name << " " << enumName << endl;
          
          dynamic_cast<LinkingParser<T>*>(this->parser)->setLinkedParser(nuParser);
        }
      }else{
        if( this->parser == NULL ) {
          if(VERBOSE >= 3) cout << "creating normal for " << name << " " << enumName << endl;
          this->parser = new EnumParser<T>(enumName);
        }else{
          if(VERBOSE >= 3) cout << "creating nothing for " << name << endl;
        }
      }
      
    } catch ( ... ) {
      /* ignore - this type is not an enum! */
    }
    if( this->parser == NULL ) {
      this->parser = new ValueParser<T>();
    }
  }
  
  /** returns the current value
   */
  const T& getValue () const
  {
    return *element;
  }
  
  /** returns the old-value
   */
  const T& getOldValue () const 
  {
    return oldValue;
  }
  
  /** sets the value of this property (modifies the aquaero-field).
   */
  T& setValue ( T v ) 
  {
    return *element = v;
  }
  
  /** Returns true, when this property has been modified.
   */
  bool hasChanged () const 
  {
    return oldValue != *element;
  }
  
  void printValue ( ostream& ostr ) const 
  {
    printValue(ostr, *element);
  }
  
  void printOldValue ( ostream& ostr ) const 
  {
    printValue(ostr, getOldValue());
  }
  
  virtual void printValue ( ostream& ostr, const T& theValue ) const 
  {
    parser->unparse(ostr,theValue);
  }

  
public:
  
  void readValue ( string s ) 
  {
    T value = parser->parse(s);
    setValue(value);
  }
  
  virtual string getEnumName () const throw(const char*)
  {
    throw "";
  }
};



template <bool enableAllSensors = false>
class ObjectToSensorPropertyDescriptor
  : public PropertyDescriptor<Byte>
{
  int mask;
  int shift;
  
public:
  ObjectToSensorPropertyDescriptor ( Byte *elem, bool upper, string name, string description )
    : PropertyDescriptor<Byte>(elem,name,description,new ValueParser<Byte>())
  {
    if( upper ) {
      mask = 0xf0;
      shift = 4;
    }else{
      mask = 0x0f;
      shift = 0;
    }
  }
  
  virtual void printValue ( ostream& ostr, const Byte& theValue ) const 
  {
    int sensorNum = ((theValue & mask) >> shift) + 1;
    
    if( sensorNum == 0x10 ) {
      ostr << "none";
    }else if( enableAllSensors && sensorNum == 0x0F ) {
      ostr << "all";
    }else{
      ostr << "sensor." << sensorNum;
      char tmp[15] = "sensor.0.name";
      tmp[7] += sensorNum;
      ostr << " (";
      if( sensorNum >= 1 && sensorNum <=6 ) {
        properties[tmp]->printValue(ostr);
      }else{
        ostr << "!ERROR!";
      }
      ostr << ")";
    }
  }
  
  virtual void readValue ( string s ) 
  {
    if(VERBOSE) cout << "ObjectToSensor: value before "
		     <<(int)getValue() << endl;
    
    int sensorNum;
    // XXX: s = s.lcase();
    s = trim(s);
    if( s == "none" ) {
      sensorNum = 0x0F;
    }else if( enableAllSensors && (s== "all") ) {
      sensorNum = 0x0E;
    }else{
      const string sensorWord = "sensor.";
      if( startsWith(s,sensorWord) ) {
	      s = s.substr( sensorWord.length(), s.length() );
      }
      sensorNum = parser->parse(s);
      sensorNum -= 1;
    }
    Byte nuValue = (getValue() & ~mask) | (sensorNum << shift);
    setValue( nuValue );
    if(VERBOSE) cout << "ObjectToSensor: value after "
		     << (int)getValue() << endl;
  }
};

typedef ObjectToSensorPropertyDescriptor<false> FanToSensorPropertyDescriptor;
typedef ObjectToSensorPropertyDescriptor<true>  LEDToSensorPropertyDescriptor;









