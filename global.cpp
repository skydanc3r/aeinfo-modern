// $Id: global.cpp 35 2007-05-05 15:09:55Z matt $

/* aquaero-info is a control-software for Unix-based (and hopefully also BSD-based) OSses
 * 
 * Copyright (C) 2006, 2007 Christian Unger <coder at breakbe D0T at>
 * Translated by John Frushour
 * 
 * This library is covered by the LGPL, read LICENSE for details.
 */

#ifdef _MSC_VER
# pragma warning( disable : 4786 4101 4503 )
#endif

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <list>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <memory>

using namespace std;

#include <locale.h>


#define VERSION   "2.4"
#define MY_CMD    "aeinfo"
int     VERBOSE = 0;


/* common stuff */
#include "common.h"
#include "objectstream.h"



map<string,map<int,string> > stringEnum_names;
map<string,map<int,string> > stringEnum_help;

set<string> bitFields;



/* ValueParsers convert string's to other types and vice versa. */
#include "parsers.h"

/* allows runtime-access to all properties (attributes) of AquaeroData_rev4 */
#include "introspection.h"

/* the data-structure of aquaero */
#include "aquaero.cpp"

/* some more introspection-stuff */
#include "introspection.cpp"

/* Communication (USB etc.) */
#include "comm.h"

#include "ipc.h"







/////////////////////////////////////////////
/////  Output-Routines

PropertyDescriptorBase& property ( string name ) 
{
  return *properties[name];
}


ostream& operator << ( ostream& ostr, const PropertyDescriptorBase& pd )
{
  pd.printValue(ostr);
  return ostr;
}

output_color sensorcolor ( int sensorNum ) 
{
  //  min..opt..max..alarm
  // we will use the following colors:
  //   green   : temp is low  <=>  t < min
  //   lt_green: temp is okay <=>  t in min..(opt+hyst+0.1)
  //   yellow  : temp is high 1 <=> t in (opt+hyst+0.1)..(opt+(max-opt)/2)
  //   brown   : temp is high 2 <=> t in (opt+(max-opt)/2)..max
  //   red     : temp is critical <=>  t in max..alarm
  //   lt_red  : temp is way too high <=>  t >= alarm
  char tmp[12] = "sensor.0";
  tmp[7] += sensorNum;
  string sensorPart = tmp;
  
  int alarm = ((PropertyDescriptor<int>*)properties[sensorPart+".alarm"])->getValue();
  int max = ((PropertyDescriptor<int>*)properties[sensorPart+".max"])->getValue();
  int min = ((PropertyDescriptor<int>*)properties[sensorPart+".min"])->getValue();
  int opt = ((PropertyDescriptor<int>*)properties[sensorPart+".opt"])->getValue();
  int t = ((PropertyDescriptor<int>*)properties[sensorPart+".temp"])->getValue();
  
  Byte hyst = ((PropertyDescriptor<Byte>*)properties[sensorPart+".hyst"])->getValue();
  
  if( t < min ) {
    return green;
  }else if( t < opt+hyst+1 ) {
    return lt_green;
  }else if( t < opt+(max - opt)/2 ) {
    return yellow;
  }else if( t < max ) {
    return brown;
  }else if( t < alarm ) {
    return red;
  }else if( t == 20000 ) {
    return dk_gray;
  }
  
  return lt_red;
}





/////////////////////////////////////////////
///// command-line-args processing 





/** the subcommand that will be executed
 */
enum ControlMode {
  ERROR,
  CSV,
  INFO,
  QUICKINFO,
  SET,
  INTERACTIVE,
  HELP
} mode;



/** stores information given by commandline
 */
struct TCmdOpts
{
  bool noClose;
  
  struct TCSV // csv
  {
    bool quiet/* = false*/;
    bool noHead/* = false*/;
    bool onlyHead/* = false*/;
    long interval/* = 0*/;
    string fields_s/* = string("")*/;
  } csv;

  struct TINFO
  {
    list<string> fields;
    bool quiet;
  } info;

  struct TSET
  {
    bool pretend;
    map<string,string> values;
  } SET;
  
  struct THELP // help
  {
    string mode;
    list<string> fields;
  } help;
  
} runOpts;




#define ITERATE_ARGS for( int itk = 2; itk < argc; ++itk ) { arg = string(args[itk]);
#define ITERATE_ARGS_DONE }
#define ITERATE_ARGS_GET_NEXT(a) if( ++itk == argc ) throw string(a);arg = string(args[itk]);



/** parses the command-line arguments and modifies `runOpts'
 */
int parse_args ( int argc, char ** args ) 
{
  /*
    control modes:
      csv : output
        -nh, --nohead       : no not display a header in the first line
        -oh, --onlyhead     : write the header to the output
        -i, --interval <xxx>: do not terminate and write a csv-line every xxx seconds
        -s, --separator <X> : use <X> as a separator
        [field1 field2 ...]
        
        e.g:  aeinfo csv -nh -i 10 -s ";" fan.1.rpm fan.2.rpm fan.3.rpm sensor.1.temp sensor.2.template
        
        
      info: output values of fields
        [field1 field2 ...]
        
        e.g:  aeinfo info  fan.1.rpm  sensor
        
      
      set : set values of fields
        -p --pretend: to not actually write to aquaero - just show what would be changed
        field1 value1 field2 value2 ...
        
        e.g:  aeinfo set -p fan.1.setPower 90 fan.1.mode "PWR_RPM_Switch | Automatik_PWM_RPM_Switch | Force_MinimumPower"
        

      interactive
        
      
      help: help on mode or fields
        <mode>: with  <mode> ::=  csv | info | set | interactive
        field <fieldname>
      
      (pipeset: input - read settings from stdin)
      
      
  */
  
  // runOpts.noClose = false;
  
  runOpts.csv.quiet = false;
  runOpts.csv.noHead = false;
  runOpts.csv.onlyHead = false;
  runOpts.csv.interval = 0;
  
  runOpts.SET.pretend = false;

  runOpts.info.quiet = false;
  
  
  
  if( argc < 2 ) return 1;
  
  bool fail = false;
  string arg;
  
  arg = string(args[1]);
  if( arg == "csv" ) {
    mode = CSV;
    
    ITERATE_ARGS;
    {
      if( arg == "-q" || arg == "--quiet" ) {
        runOpts.csv.quiet = true;
      }else if( arg == "-nh" || arg == "--nohead") {
        runOpts.csv.noHead = true;
      }else if( arg == "-oh" || arg == "--onlyhead") {
        runOpts.csv.onlyHead = true;
      }else if( arg == "-i" || arg == "--interval") {
        ITERATE_ARGS_GET_NEXT("--interval needs a numeric argument!");
        runOpts.csv.interval = atoi(arg.c_str());
      }else if( arg == "-f" || arg == "--fields" ) {
        ITERATE_ARGS_GET_NEXT("'--fields' needs a list of fields!");
        runOpts.csv.fields_s = arg;
      }else{
        cerr << "Unknown Option " << arg << endl;
        fail = true;
      }
    }
    ITERATE_ARGS_DONE;  
    
  }else if( arg == "short" ) {
    mode = QUICKINFO;
    
  }else if( arg == "info" ) {
    mode = INFO;
    
    ITERATE_ARGS;
    {
      string v = arg;
      if( v == "-q" || arg == "--quiet" ) {
        runOpts.info.quiet = true;
      }
      runOpts.info.fields.push_back( v );
    }
    ITERATE_ARGS_DONE;
    
  }else if( arg == "set" ) {
    mode = SET;
    
    bool isKey = true;
    string curKey;
    ITERATE_ARGS;
    {
      if( itk == 2 && (arg == "-p" || arg == "--pretend") ) {
        runOpts.SET.pretend = true;
        if(VERBOSE) cout << "pretending!" << endl;
      }else{
        // pairs of <key> <value>
        if( isKey ) {
          curKey = arg;
        }else{
          runOpts.SET.values[curKey] = arg;
        }
        
        isKey = !isKey;
      }
    }
    ITERATE_ARGS_DONE;
    if( !isKey ) {
      cerr << "ERROR: Not as much values as keys given!" << endl;
      fail = true;
    }
    
  }else if( arg == "interactive" ) {
    mode = INTERACTIVE;
    
  }else if( arg == "help" || arg == "--help" ) {
    mode = HELP;
    
    ITERATE_ARGS;
    {
      if( itk == 2 ) {
        runOpts.help.mode = arg;
      }else{
        runOpts.help.fields.push_back( arg );
      }
    }
    ITERATE_ARGS_DONE;  
  }else{
    mode = ERROR;
    cerr << "Don't know what to do." << endl;
    cerr << lt_red << "Type '" MY_CMD " help' for usage." << resetcolors << endl;
    return 1;
  }
  
  if( fail ) return 1;
  return 0;
}



int main ( int argc, char ** args ) 
{
  runOpts.noClose = false;      // FIXME: put to parse_args

  if( argc > 1 ) {
    // FIXME: put this to parse_args..
    bool goon;
    do {
      goon = false;
      
      if(string(args[1]) == "-v") {
        VERBOSE = 1;
        ++args;
        --argc;
        goon = true;
      }else if( string(args[1]) == "-vv" ) {
        VERBOSE = 2;
        ++args;
        --argc;
        goon = true;
      }else if( string(args[1]) == "-nc" ) {
        runOpts.noClose = true;
        ++args;
        --argc;
        goon = true;
      }
    } while( goon && argc > 1 );
  }

  mode = QUICKINFO;
  
  // parse the arguments
  int ret = parse_args(argc,args);

  //cout << setlocale( LC_ALL, ".850" ) << endl;

  if( mode != INFO || !runOpts.info.quiet ) {
    cout << bold << "aquaero-info " << VERSION << resetcolors << " "
         << dk_gray << " (c) 2006, 2007 Christian Unger" << endl << " Translated by John Frushour" << resetcolors << endl
         << endl;
  }
    
  AquaeroData_rev4 aedata;

  AquaeroCommunication *pcomm = getAquaeroCommunicationInstance();
  // XXX: MSC doesn't like this dynamic_cast
#ifndef _MSC_VER
  if( dynamic_cast<USBCommunication*>(pcomm) != NULL ) {
    ((USBCommunication*)pcomm)->setDirtyNoClose( runOpts.noClose );
  }
#endif
  AquaeroCommunication &comm = *pcomm;
  
  // prepare introspection stuff
  definePropertyMap(aedata);
  defineStringEnums();
  
  try {
    // access aquaero
    if( mode != HELP && mode != ERROR )
      comm.read( aedata );
    
    // execute the requested sub-command
    switch( mode ) {
    case CSV:
      break;
      
    case QUICKINFO:
      {
        bool useFan4AsFlow2 = false;
        string flowUnit = "l/h";
        {
          typedef PropertyDescriptor<AquaeroData_rev4::FlowSensorModeRegister> flow_mode_t;
          flow_mode_t *flowMode = (flow_mode_t*)properties["flowsensor.mode"];
          if( (flowMode->getValue() & AquaeroData_rev4:: l_minute_hour) != 0 ) {
            flowUnit = "l/m";
          }
          if( (flowMode->getValue() & AquaeroData_rev4:: use_fan4_as_flowsensor) != 0 ) {
            useFan4AsFlow2 = true;
          }
        }
        
        cout << "Status of " << property("aquaero.name") << resetcolors << endl;
        cout << endl;
        
        cout << bold << "Temperatures (";
#ifdef WINDOWS
        wcout << L"\x00F8";
#else
        //wprintf(L"\x00F8");
        cout << "\xB0";
#endif
        cout << "C):" << resetcolors << endl;
        cout << resetcolors << setw(11) << property("sensor.1.name") << ": " << sensorcolor(1) 
             << setprecision(3) << showpoint << setw(4) << property("sensor.1.temp")
             << "   " << resetcolors;
        cout << setw(11) << property("sensor.2.name") << ": " << sensorcolor(2)
             << setprecision(3) << showpoint << setw(4) << property("sensor.2.temp")
             << "   " << resetcolors;
        cout << setw(11) << property("sensor.3.name") << ": " << sensorcolor(3)
             << setprecision(3) << showpoint << setw(4) << property("sensor.3.temp")
             << resetcolors << endl;
        
        cout << setw(11) << property("sensor.4.name") << ": " << sensorcolor(4) 
             << setprecision(3) << showpoint << setw(4) << property("sensor.4.temp")
             << "   " << resetcolors;
        cout << setw(11) << property("sensor.5.name") << ": " << sensorcolor(5)
             << setprecision(3) << showpoint << setw(4) << property("sensor.5.temp")
             << "   " << resetcolors;
        cout << setw(11) << property("sensor.6.name") << ": " << sensorcolor(6) 
             << setprecision(3) << showpoint << setw(4) << property("sensor.6.temp")
             << resetcolors << endl;
        
        cout << endl;
        
        cout << bold << "Fans (RPM):" << resetcolors << endl;
        cout << setw(11) << property("fan.1.name") << ": "
             << lt_blue << setw(4) << property("fan.1.rpm") << resetcolors << "   ";
        cout << setw(11) << property("fan.2.name") << ": "
             << lt_blue << setw(4) << property("fan.2.rpm") << resetcolors << "   ";
        cout << setw(11) << property("fan.3.name") << ": "
             << lt_blue << setw(4) << property("fan.3.rpm") << resetcolors << "   ";
        if( !useFan4AsFlow2 ) {
          cout << setw(11) << property("fan.4.name") << ": "
               << lt_blue << setw(4) << property("fan.4.rpm") << resetcolors << endl;
        }else{
          cout << endl;
        }
        
        cout << endl;
        cout << bold << setw(11) << property("flowsensor.1.name") << ": " << resetcolors << setw(4)
             << property("flowsensor.1.flow") << " " << flowUnit << endl;
        if( useFan4AsFlow2 ) {
          cout << bold << setw(11) << property("fan.4.name") << ": " << resetcolors << setw(4)
               << property("fan.4.rpm") << " " << flowUnit << endl;
        }
        
        cout << bold << setw(11) << "Powersensor" << ": " << resetcolors << setw(4)
             << property("powersensor.1.power") << " W" << endl;
        // TODO: cout << setw(11) << "Power 2" << ": " << setw(4) << property("powersensor.2.power") << endl;
        
        // XXX: tubemeter...
        // XXX: aquastream 1 and 2?
      }
      break;
      
    case INFO:
      
      {
        if( runOpts.info.fields.size() == 0 ) {
          // display all fields
          map<string,PropertyDescriptorBase*>::const_iterator it = properties.begin();
          while( it != properties.end() ) {
            if( !runOpts.info.quiet ) {
              cout << it->first << " = ";
            }
            it->second->printValue(cout);
            cout << endl;
            ++it;
          }
          
        }else{
          // display a subset of fields
          list<string>::const_iterator fieldIt = runOpts.info.fields.begin();
          while( fieldIt != runOpts.info.fields.end() ) {
	    
	    if( (*fieldIt)[0] == '.' ) {
	      // extract fields ending with <FIELDSPEC>
	      map<string,PropertyDescriptorBase*>::const_iterator 
		it = properties.begin(), end = properties.end();
	      while( it != end ) {
		if( endsWith( it->first, *fieldIt ) ) {
      if( !runOpts.info.quiet ) {
		    cout << it->first << " = ";
      }
		  it->second->printValue(cout);
		  cout << endl;
		}
		++it;
	      }
	    }else{
	      // extract fields starting with <FIELDSPEC>
	      map<string,PropertyDescriptorBase*>::const_iterator 
		it = properties.lower_bound(*fieldIt),
		end = properties.end();
	      while( it != end
		     && startsWith( it->first, *fieldIt ) ) {
          if( !runOpts.info.quiet ) {
            cout << it->first << " = ";
          }
		it->second->printValue(cout);
		cout << endl;
		++it;
	      }
	    }
            
            ++fieldIt;
          }
        }
      }
      break;
      
      
      
    case SET:
      {
        // set the specified fields to the specified values
        try {
          aedata.synchronizeClock();
          
          map<string,string>::const_iterator
            vit = runOpts.SET.values.begin(), vend = runOpts.SET.values.end();
          while( vit != vend ) {
            if( properties.find(vit->first) != properties.end() ) {
              PropertyDescriptorBase *prop = properties[vit->first];
              
              cout << "setting " << vit->first << " = ";
              prop->printValue(cout);
              cout << endl;
              
              prop->readValue( vit->second );
              
              cout << "  to value ";
              prop->printValue(cout);
              cout << endl;
            }
            ++vit;
          }
          
          // send the data to the aquaero
          if( !runOpts.SET.pretend ) {
            comm.write( aedata );
          }
          
        } catch ( ReadOnlyException *rdo ) {
          cerr << "ERROR: Trying to write to read-only property" << endl;
        } catch ( ParseException *pex ) {
          cerr << "ERROR: ParseException" << endl;
	} catch ( const char * sss ) {
	  cerr << "ERROR: " << sss << endl;
        } catch ( ... ) {
          cerr << "ERROR: Unknown exception" << endl;
        }
        
      }
      break;
      
      
      
    case INTERACTIVE:
      break;
      
      
      
    case HELP:
      {
        const 
#ifndef WINDOWS
          string 
#else
          output_color
#endif
          dflt = resetcolors;
        if( runOpts.help.mode == "field" || runOpts.help.mode == "fields" ) {
          cout << "Help on fields: " << endl;
          
          // display a subset of fields
          if( runOpts.help.fields.size() == 0 )
            runOpts.help.fields.push_back( "" );
          
          list<string>::const_iterator
            fieldIt = runOpts.help.fields.begin(),
            fieldEnd = runOpts.help.fields.end();
          while( fieldIt != fieldEnd ) {
            map<string,PropertyDescriptorBase*>::const_iterator it = properties.lower_bound(*fieldIt);
            
            while( it != properties.end()
                   && startsWith( (it == properties.end()) ? string("") : it->first, *fieldIt ) ) {
              cout << lt_cyan << setw(26) << left << it->first << resetcolors << ": "
                   // << it->second->getTypeString() 
                   << green << it->second->getDescription() << resetcolors << endl;
              
              if( it->first == *fieldIt ) {
                // give detailed information, if available (for example: the values of enum's)
                try {
                  string enumName = it->second->getEnumName();
                  
                  if( stringEnum_help.find(enumName) != stringEnum_help.end() ) {
                    // there is help available on an enum:
                    cout << " --> Further information on the values of enumerated type \""
                         << blue << enumName << resetcolors << "\"" << endl;
                    
                    map<int,string>::const_iterator
                      helpIt = stringEnum_help[enumName].begin(),
                      helpEnd = stringEnum_help[enumName].end();
                    for( ; helpIt != helpEnd; ++helpIt ) {
                      cout << "    "
                           << lt_green << setw(20) << stringEnum_names[enumName][helpIt->first]
                           << resetcolors
                           << " (=" << setw(3) << helpIt->first << "): ";
                      if( helpIt->second == "" ) cout << "?";
                      else cout << green << helpIt->second << resetcolors;
                      cout << endl;
                    }
                  }
                } catch( ... ) {
                }
              }
              ++it;
            }
            ++fieldIt;
          }
        }else if( runOpts.help.mode == "info" ) {
          cout << "info: display field-values of aquaero." << endl
               << "usage: "<<lt_cyan<<"info "<<dflt<<"["<<lt_green<<"options"<<dflt<<"] [FIELDSPEC...]" << endl
               << endl
               << "  Display current values of aquaero." << endl
               << "  FIELDSPEC may be a fully qualified field-identifier (e.g: fan.1.name) or a " << endl
               << "  substring. Then, every field will be displayed whose field-identifier starts " << endl
               << "  with FIELDSPEC. " << endl
               << "  For example:  \"led.1\" matches \"led.1.mode\" \"led.1.pwm\" \"led.1.sensor\""
               << endl
               << "  Example:" << endl << bold
               << "    " MY_CMD " info  sensor  fan.1.rpm fan.2.rpm" << dflt << endl
               << endl
               << "Valid options:" << endl
               << "  "<<lt_green<<"-q "<<dflt<<"["<<lt_green<<"--quiet"<<dflt<<"]             : "<<bold<<"_only_"<<dflt<<" print field-values" << endl
               << endl;
          
          
          
        }else if( runOpts.help.mode == "set" ) {
          cout << "set: Set field-values of aquaero." << endl
               << "usage: "<<lt_cyan<<"set "<<dflt<<"["<<lt_green<<"options"<<dflt<<"] [FIELDNAME VALUE  FIELDNAME VALUE  ...]" << endl
               << endl
               << "  Set field-values of aquaero and synchronize the clock." << endl
               << "  There are different types of fields: " << endl
               << "   1. strings" << endl
               << "   2. numbers" << endl
               << "   3. enumerated types" << endl
               << "  Specifying values for fields of type 1 and 2 is straight-forward. Enumerated " << endl
               << "  types are a bit more complicated. The value can be specified as a combination" << endl
               << "  of its named values or through its internal numeric value. " << endl
               << "  A combination of named values is done via the pipe-operator (\"|\"). " << endl
               << "  All named values of a enumerated type can be retrieved with  " << endl
               << "    " << bold << MY_CMD " help field FIELDNAME" << dflt << endl
               << endl
               << "  Please note that there is one special field: "<<bold<<"fan.X.mode"<<dflt<<endl
               << "  This field has been introduced, in order to ease the fan-configuration," <<endl
               << "  because the values of 'fan.1.mode_raw' are quite weird." <<endl
               << "  See  "<<bold<<MY_CMD<<" help fields fan.1.mode fan.1.mode_raw"<<dflt<<"  for more information about" << endl
               << "  this."<<endl
               << endl
               << "  For example: In order to set the mode of 'Fan 1' to "<< endl
               << "    \"automatic control towards target-value with minimum power\"" << endl
               << "  one can do this in the following ways: " << endl << bold
               << "    " MY_CMD " set fan.1.mode_raw 161" << endl << dflt
               << "  or " << endl << bold
               << "    " MY_CMD " set fan.1.mode_raw \"PWR_RPM_Switch | Automatik_PWM_RPM_Switch | Force_MinimumPower\"" << dflt << endl
               << "  or " << endl << bold
               << "    " MY_CMD " set fan.1.mode \"FORCE_MIN_POWER | AUTO_TARGET\"" << dflt << endl
            //<< "  ir "
               << endl
               << "  Example:" << endl << bold
               << "    " MY_CMD " set  "<<dflt<<blue<<"fan.1.name "<<lt_blue<<"\"Fan 1\"  "<<dflt<<blue<<"fan.1.minimumPower "<<lt_blue<<"33  "<<dflt<<blue<<"sensor.1.name "<<lt_blue<<"\"CPU 1\"  "<<dflt<<blue<<"fan.1.mode_raw"<<lt_blue<<" \"PWR_RPM_Switch | Automatik_PWM_RPM_Switch | Force_MinimumPower\"" << endl << dflt
               << endl
               << "Valid options:" << endl
               << "  "<<lt_green<<"-p "<<dflt<<"["<<lt_green<<"--pretend"<<dflt<<"]           : do not write data to aquaero" << endl
               << endl;
          
          
          
        }else if( runOpts.help.mode == "csv" ) {
        }else if( runOpts.help.mode == "interactive" ) {
        }else if( runOpts.help.mode == "help" ) {
        }else if( runOpts.help.mode == "short" ) {
          cout << "short: Display most relevant information in a compact way." << endl
               << "usage: "<<lt_cyan<<"short" <<dflt<< endl
               << endl
               << "  Displays temperatures, fan speeds, flow and measured power." << endl
               << endl;
          
          
          
        }else{
          cout << "usage: "
               << lt_cyan << MY_CMD << dflt
               << " [" << lt_green << "global options" << dflt << "]" 
               << " <" << lt_cyan << "subcommand"<< dflt <<"> "
               << "[" << lt_green << "options" << dflt << "] "
               << "[" << lt_green << "args" << dflt << "]" << endl
            
               << "Type \"" << lt_cyan << MY_CMD " help "<<dflt<<"<"<<lt_cyan<<"subcommand"<<dflt<<">\""
               << " for help on a specific command or " << endl
            
               << "type \"" << lt_cyan << MY_CMD " help fields"<<dflt<<"\" for a list of supported fields of aquaero." << endl
               << endl
            
               << "type \"" << lt_cyan << MY_CMD " help fields"<<dflt<<" [FIELDSPEC...]\" for help on a subset of fields." << endl

               << "type \"" << lt_cyan << MY_CMD " help fields"<<dflt<<" [FIELDNAME...]\" for detailed help on specific fields." << endl
               << endl
            
               << "Available subcommands:" << endl
               << lt_cyan 
            //<< "   csv" << endl
               << "   help" << endl
               << "   info" << endl
            //<< "   interactive" << endl
               << "   set" << endl
               << "   short" << endl
               << dflt
               << endl
               << "Valid global options:" << endl
               << "  "<<lt_green<<"-nc"<<dflt<<"                      : \"No Close\" - this will not close the aquaero " << endl
               << "  "            "   "        "                        usb-device immediately, and may make this program" << endl
               << "  "            "   "        "                        perform better on linux-2.6 kernels. Give it a " << endl
               << "  "            "   "        "                        try, if you experience \"lags\"." << endl
               << "  "<<lt_green<<"-v"<<dflt<<"                       : Be verbose" << endl
               << "  "<<lt_green<<"-vv"<<dflt<<"                      : Be more verbose" << endl
               << endl;
        }
      }
      break;
      
      
      
    case ERROR:
      // cout << " No mode specified. Run  aeinfo help  for further instructions." << endl;
      break;
      
    default:
      cerr << "ERROR: wrong mode..." << endl;
    }
    
  }catch ( string s ) {
    cerr << "FATAL ERROR: " << s << endl;
    return 1;
  }catch ( const char * sss ) {
    cout << "FATAL ERROR: " << sss << endl;
    return 1;
  }catch ( const AbnormalTerminationError * ) {
    return 1;
  }catch ( ... ) {
    cout << "FATAL ERROR: exiting due to an uncaught exception" << endl;
    return 1;
  }
  
  // clean up...
#ifndef WINDOWS                         // TODO: why is there a crash under w32?
  map<string,PropertyDescriptorBase*>::iterator
    it = properties.begin(), end = properties.end();
  while( it != end ) {
      delete it->second;
      properties.erase(it++);
  }
#endif
  
  return 0;
}
