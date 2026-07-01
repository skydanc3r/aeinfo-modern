// $Id: aeinfo_samurize.cpp 36 2007-05-08 18:41:10Z matt $
// aeinfo_samurize.cpp : Definiert den Einsprungpunkt für die DLL-Anwendung.
//

//////////////////////////////////
//////////////////////////////////
//////////////////////////////////
//////////////////////////////////

#ifdef _MSC_VER
# pragma warning( disable : 4786 4101 4503 )
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <list>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <memory>

using namespace std;



#define VERSION   "2.4"
int     VERBOSE = 0;


//#define NOMINMAX
//#include <windows.h>


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

#include "comm.h"

#include "ipc.h"

//////////////////////////////////
//////////////////////////////////
//////////////////////////////////
//////////////////////////////////


#include "stdafx.h"



#define SAMURIZE_PLUGIN_CHAR  extern "C" __declspec(dllexport) char* __stdcall
#define SAMURIZE_PLUGIN_INT   extern "C" __declspec(dllexport) int __stdcall

HWND hSamurize;


class SamurizeMeter 
{
  char *sResult;

public:
  SamurizeMeter ( ) 
    : sResult(NULL)
  {
  }
  
  void init () 
  {
    sResult = new char[1024];
    setResult( string("WAITING") );
  }
  
  void destroy () 
  {
    if( sResult != NULL ) {
      delete sResult;
      sResult = NULL;
    }
  }
  
  void setResult ( string s ) 
  {
    strncpy( sResult, s.c_str(), 1000 );
  }
  char *getResult () 
  {
    return sResult;
  }
};

int meterID = 0;
map<int,SamurizeMeter> meters;

AquaeroData_rev4 aedata;
DaemonCommunicationClient *comm = NULL;

bool connect () 
{
  if( comm != NULL ) return true;
  
  try {
    comm = new DaemonCommunicationClient();
  } catch( const DaemonNotFoundException *dnfex ) {
    comm = NULL;
  } catch( ... ) {
    comm = NULL;
  }
  //if( comm == NULL ) MessageBox(NULL, "daemon not found", "aeinfo-sam", MB_OK);
  
  return comm != NULL;
}

void disconnect () 
{
  if( comm == NULL ) return;
  delete comm;
  comm = NULL;
}

DWORD lastUpdate = -1;
bool update () 
{
  if( comm == NULL ) return false;
  
  if( GetTickCount() > lastUpdate+1000  ) {
    comm->read( aedata );
    lastUpdate = GetTickCount();
    return true;
  }
  return false;
}


/////////////////////////////////////
/////////////////////////////////////
/////////////////////////////////////
/////////////////////////////////////


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH: 
			// this one is called as soon as samurize loads the dll
			// do global initialization stuff here
      {
			  srand((unsigned int)GetTickCount());
        
        definePropertyMap(aedata);
        defineStringEnums();
        
        connect();
      }

			break;
		case DLL_PROCESS_DETACH: 
			// this is called as samurize unloads the dll
			// free/delete all your data
      {
        map<int,SamurizeMeter>::iterator it = meters.begin(), end = meters.end();
        for( ; it != end; ++it ) {
          it->second.destroy();
        }
        disconnect();
      }
			break;
	}
    return TRUE;
}

SAMURIZE_PLUGIN_INT dllstartup(HWND hwnd, int dlltype)
{
	// this is called when the plugin is loaded. we save the hwnd parameter
	// as it is the handle we need when sending messages to samurize
	// (see samurize.h for possible messages)
	// initiate the GDI+ class. do not try to call this in DllMain or your plugin
	// will crash.
	//GdiplusStartupInput  gdiplusStartupInput;
	//GdiplusStartup(&gdiplusToken,&gdiplusStartupInput,0);

  ++meterID;
  meters[meterID] = SamurizeMeter();
  meters[meterID].init();
  
	return meterID;
}

SAMURIZE_PLUGIN_INT dllshutdown(int id)
{
	// uninitialize the GDI+ class. do not try to call this in DllMain or your
	// plugin will crash.
	//GdiplusShutdown(gdiplusToken);
  meters[id].destroy();

	return 0;
}
/*
SAMURIZE_PLUGIN_INT vis_configure(int id)
{
	// called when the user presses the "config" button in samurize.
	// you can create a fancy configure dialog here and let the user
	// change options.
	return 0;
}

SAMURIZE_PLUGIN_INT load_settings(int id, char* inifile, char* section)
{
    return 0;
}

SAMURIZE_PLUGIN_INT save_settings(int id, char* inifile, char* section)
{
    return 0;
}
*/
SAMURIZE_PLUGIN_CHAR getinfo(int infonum)
{
	// called by samurize to display something useful in the about dialog.
	switch (infonum) {
        case 0: return "aeinfo-samurize plugin";
        case 1: return "Christian Unger";
        case 2: return VERSION;
        case 3: return "May 31 2006";
        case 4: return __DATE__ " " __TIME__;
        case 5: return "http://bReaKBe.aT/develop/";
        case 6: return "coder (at) bReaKBe (D0T) aT";
	}
	return "";
}

// The value you should return is a summation of any or all of the following values:
// 1: Source plugin
// 2: Visual plugin
// ... (more to follow!)
SAMURIZE_PLUGIN_INT dlltype()
{
    return 1;
	// you can also have a source plugin and a visual plugin in the same dll,
	// if so, return the sum of the plugin type numbers (that would be 3)
}

SAMURIZE_PLUGIN_CHAR init()
{	
	// ok here you will return the list of the functions, separated by 
	// a | if there are more than one like
	// that somefunc|someotherfunc|alastfunc
	return "HelloWorld|getValue|getValueAsNumber";
}

//Samurize GetParam, returns a list of the parameters that each function needs
SAMURIZE_PLUGIN_CHAR getparam(char* func)
{
	//here you should have an if statement or a case to determnie what was function asking for params
	//the func param is the config editor asking for the parameter list of a funcion
	//so return the tex separated by | and this time it must finish with a |
	//ie : param1|param2|
	//or must be empty like in this template when there are no parameters
	if(!stricmp(func, "HelloWorld")) {
		return "Enter your Name|";
  }else if(!stricmp(func, "getValue")) {
    return "Enter the field name (e.g. fan.2.name)|";
  }else if(!stricmp(func, "getValueAsNumber")) {
    return "Enter the field name (e.g. fan.2.name)|";
	}else{
		return "|";
	}
}

SAMURIZE_PLUGIN_CHAR HelloWorld(int id, char* pYourName)
{
  return "Hello, World";
}

SAMURIZE_PLUGIN_CHAR getValue (int id, char* field)
{
  SamurizeMeter meter = meters[id];
  if( connect() ) {
    update();
    
    {
      PropertyDescriptorBase *pd = properties[field];
    
      ostringstream oss;
      if( pd != NULL ) {
        pd->printValue( oss );
      }else{
        oss << "!UNKNOWN FIELD!";
      }
      meter.setResult(oss.str());
    }
  }
  
  return meter.getResult();
}

SAMURIZE_PLUGIN_CHAR getValueAsNumber (int id, char* field)
{
  SamurizeMeter meter = meters[id];

  if( connect() ) {
    update();
    
    {
      int i;
      PropertyDescriptorBase *pd = properties[field];
      PropertyDescriptor<Long> * pl = dynamic_cast< PropertyDescriptor<Long>* >(pd);
      if( dynamic_cast<PropertyDescriptor<Long> *>(pd) != NULL ) {
        PropertyDescriptor<Long> * pdl = (PropertyDescriptor<Long> *)pd;
        i = (int)pdl->getValue();
      }else if( dynamic_cast<PropertyDescriptor<Integer> *>(pd) != NULL ) {
        PropertyDescriptor<Integer> * pdi = (PropertyDescriptor<Integer> *)pd;
        i = pdi->getValue();
      }else if( dynamic_cast<PropertyDescriptor<Byte> *>(pd) != NULL ) {
        PropertyDescriptor<Byte> * pdb = (PropertyDescriptor<Byte> *)pd;
        i = (int)pdb->getValue();
      }else{
        i = -42;
      }
      ostringstream oss;
      oss << i; 

      meter.setResult( oss.str() );
    }
  }
  
  return meter.getResult();
}

