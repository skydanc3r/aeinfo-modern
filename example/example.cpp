/* Example application, that demonstrates usage of aquaero-info-classes
 */


// you will always need the next lines:
// --->>>
#include <iostream>
#include <string>

using namespace std;

int VERBOSE = 0;
// <<<---


// include these three files from the library..
#include "../common.h"
#include "../objectstream.h"
#include "../aquaero.cpp"
#include "../comm.h"



int main ( int, char** )
{
  AquaeroData_rev4 aedata;      // instance of aquaero-data
  USBCommunication comm;        // usb-comm-class
  comm.read( aedata );          // read from aquaero
  
  cout << "Fan 1's name is "    // print some data
       << aedata.Fan_Name[1] << endl;
  
  // comm.write( aedata ); 
  
  return 0;
}
