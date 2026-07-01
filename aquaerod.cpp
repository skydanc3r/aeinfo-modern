// $Id: aquaerod.cpp 35 2007-05-05 15:09:55Z matt $

/* main-file for the aquaerod-daemon.
 * 
 * Copyright (C) 2006, 2007 Christian Unger <coder at breakbe D0T at>
 * Translated by John Frushour
 * 
 * This library is covered by the LGPL, read LICENSE for details.
 */

#ifdef _MSC_VER
# pragma warning( disable : 4786 4101 4503 4305 4309 )
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
#define MY_CMD    "aquaerod"
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

AquaeroData_rev4 *global_aedata;
Semaphore *global_aedata_mtx;
Semaphore *global_write_mtx;
DaemonCommunicationServer *global_comm_srv;

//Semaphore srv_mtx( _IPC_BASE "srv_mtx", 0x13444500+0xB0, 1, 1 );


#include "aehttpd.cpp"

AquaeroDaemon *pdaemon = NULL;


int main ( int, char ** );


#ifdef WINDOWS
int WINAPI WinMain(
  HINSTANCE hInstance,  // handle to current instance
  HINSTANCE hPrevInstance,  // handle to previous instance
  LPSTR lpCmdLine,      // pointer to command line
  int nCmdShow          // show state of window
)
{
  int argc = 1;
  char * lpCmdLinePRIV = new char[strlen(lpCmdLine)+2];
  lpCmdLinePRIV[strlen(lpCmdLine)+1] = '\0';
  strcpy(lpCmdLinePRIV,lpCmdLine);
  char ** argv = &lpCmdLinePRIV;
  // FIXME: this is very very simple - however it fits the current requirements
  {
    {
      for( int i = 0; i < strlen(lpCmdLinePRIV); ++i ) {
        if( lpCmdLinePRIV[i] == ' ' ) ++argc;
      }
    }
    argv = new char*[argc+5];
    argv[0] = &lpCmdLinePRIV[0];
    int n = 1;
    for( int i = strlen(lpCmdLinePRIV)-1; i >= 0; --i ) {
      if( lpCmdLinePRIV[i] == ' ' ) {
        argv[n] = &lpCmdLinePRIV[i+1];
        MessageBox(NULL,argv[n],"xxx",MB_OK);
        lpCmdLinePRIV[i] = '\0';
        ++n;
      }
    }
  }

  return main(argc,argv);
}
#endif

#ifndef WINDOWS
void delete_res () 
{
  if(VERBOSE) cout << "BYE!" << endl;
  if( pdaemon != NULL ) {
    pdaemon->destroy();
  }
  // TODO: close web-server-socket
  // close(s);
  if(VERBOSE) cout << "done with cleanups" << endl;
}
#endif



int main ( int argc, char ** argv ) 
{
  bool runWebServer = true;
  
#ifndef WINDOWS
  parser_mtx.lock();
#endif
  
  for( int i = 0; i < argc; ++i ) {
    if( string(argv[i]) == "-v" ) {
      VERBOSE = 1;
    }
    if( string(argv[i]) == "-vv" ) {
      VERBOSE = 3;
    }
    if( string(argv[i]) == "--no-webserver" ) {
      runWebServer = false;
    }
  }
  

#ifdef WINDOWS
  ofstream logfile = ofstream("c:\\aquaerod.log",ios::out | ios::in | ios_base::trunc /*| ios_base::app*/);
  if( VERBOSE >= 1 ) {
    std::cout = logfile;
    std::cerr = logfile;
  }
#endif
  
#ifndef WINDOWS
  atexit(delete_res);
#endif
  
  try {
    pdaemon = new AquaeroDaemon();
  
    AquaeroDaemon &daemon = *pdaemon;

    //XXX: this is very dirty!
    global_aedata_mtx = &daemon.getDataMutex();
    global_write_mtx = &daemon.getWriteMutex();
    global_aedata = &daemon.getAquaeroData();
    global_comm_srv = &daemon.getCommunication();
  
    definePropertyMap(*global_aedata);
    defineStringEnums();
  
    daemon.run();

    if( runWebServer ) {
      startWebServer();
    }
  
    while( true ) {
      Thread::sleep(5000);
#ifdef WINDOWS
      logfile.flush();
#endif
    }

  } catch( const char * msg ) {
    string message = string(msg) + "\nThe daemon will abort now.";
#ifdef WINDOWS
    MessageBox(NULL, message.c_str(), "aeinfo-daemon", MB_OK);
#else
    cerr << message << endl;
#endif
  } catch( ... ) {
    string message = "Unknown fatal Error\nThe daemon will abort now.";
#ifdef WINDOWS
    MessageBox(NULL, message.c_str(), "aeinfo-daemon", MB_OK);
#else
    cerr << message << endl;
#endif
  }
    
  return 0;
}

