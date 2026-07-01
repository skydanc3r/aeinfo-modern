// -*- c++ -*- $Id: aehttpd.cpp 35 2007-05-05 15:09:55Z matt $

/* Webserver-Specific stuff
 * 
 * Copyright (C) 2006 Christian Unger <coder at breakbe D0T at>
 * Translated by John Frushour
 * 
 * This library is covered by the LGPL, read LICENSE for details.
 */


#include <sys/types.h>

#ifndef WINDOWS
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/time.h>
# include <sys/select.h>
# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
#else
// FIXME: do we need this header anymore?
# include <io.h>
# define MSG_NOSIGNAL 0
#endif





#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <list>
#include <set>

#include <cstring>

#ifdef WINDOWS
# define strcasecmp _strcmpi

typedef int socklen_t;

#endif

using namespace std;


const string htdocs =
#ifdef WINDOWS
"htdocs";
#else
"htdocs";
#endif

/*
 * - xp3000 (overview)
 *   +- fans (1..4)
 *   +- sensors (1..6)
 *   +- flow
 *  (+- Tubemeter)
 *   +- Power-Measurement
 *   +- Peripherals
 *   |  +- aquastream (1..2)
 *   |  +- LEDs
 *   |  +- Multiswitch
 *   | (+- Relay)
 *   |  +- Display
 *   +- Misc
 *      +- Alarms & Timers
 *     (+- Software Poweroff)
 */


class ConnectionClosedException 
{
public:
  ConnectionClosedException () {}
};


string url_decode ( string s ) 
{
  ostringstream oss;

  string::const_iterator it = s.begin(), end = s.end();
  for( ; it != end; ++it ) {
    if( *it == '+' ) {
      oss << ' ';
    }else if( *it == '%' ) {
      char tmp[3];
      tmp[0] = *(++it);
      tmp[1] = *(++it);
      tmp[2] = '\0';
      long l = strtol(tmp,NULL,16);
      oss << ((char)l);
    }else{
      oss << *it;
    }
  }
  return oss.str();
}

string sensorcolor_html ( int sensorNum ) 
{
  //  min..opt..max..alarm
  // we will use the following colors:
  //   green            : temp is low  <=>  t < min
  //   green > lt_green : temp is okay <=>  t in min..(opt+hyst+0.1)
  //   lt_green > yellow: temp is high 1 <=> t in (opt+hyst+0.1)..(opt+(max-opt)/2)
  //   yellow > brown   : temp is high 2 <=> t in (opt+(max-opt)/2)..max
  //   brown > lt_red   : temp is critical <=>  t in max..alarm
  //   lt_red           : temp is way too high <=>  t >= alarm
  char tmp[12] = "sensor.0";
  tmp[7] += sensorNum;
  string sensorPart = tmp;

  int alarm = ((PropertyDescriptor<int>*)properties[sensorPart+".alarm"])->getValue();
  int max = ((PropertyDescriptor<int>*)properties[sensorPart+".max"])->getValue();
  int min = ((PropertyDescriptor<int>*)properties[sensorPart+".min"])->getValue();
  int opt = ((PropertyDescriptor<int>*)properties[sensorPart+".opt"])->getValue();
  int t = ((PropertyDescriptor<int>*)properties[sensorPart+".temp"])->getValue();
  
  Byte hyst = ((PropertyDescriptor<Byte>*)properties[sensorPart+".hyst"])->getValue();
  
  int r = 0, g = 0;
  float p;
  if( t < min ) {
    g = 0x80;
  }else if( t < opt+hyst+1 ) {
    p = (t - min) / (float)(opt + hyst + 1 - min);
    g = 0x80 + (int)(p*0x7F);
  }else if( t < opt+(max - opt)/2 ) {
    p = (t - opt-hyst-1) / (float)((max-opt)/2.0f  - hyst - 1);
    g = 0xff;
    r = (int)(p*0xff);
  }else if( t < max ) {
    p = (t - opt-(max-opt)/2.0f) / (float)(max - opt-(max-opt)/2.0f);
    g = 0xff - (int)(p*0x80);
    r = 0xff;
  }else if( t < alarm ) {
    p = (t - max) / (float)(alarm - max);
    g = 0x79 - (int)(p*0x79);
    r = 0xff;
  }else if( t >= alarm ) {
    r = 0xff;
  }
  ostringstream oss;
  oss << '#';
  oss << hex << setw(2) << setfill('0') << r;
  oss << hex << setw(2) << setfill('0') << g;
  oss << "00";

  return oss.str();
}

/*
  WWW-Authenticate, Authentication-Info and Authorization
  header('WWW-Authenticate: Basic realm="Test Authentication System"');
  header('HTTP/1.0 401 Unauthorized');
 */

string base64_decode ( string text )
{
  static unsigned char *decode_base64_map = NULL;
  
  if( decode_base64_map == NULL ) {
    decode_base64_map = new unsigned char[255];
    int i;
    
    for( i = 0; i < 255; i++ )
      decode_base64_map[i] = 0;
    // the PAD ('=') will be mapped to zero!
    
    // decode (A-Z -> 0-25), (a-z -> 26-51), (0-9 -> 52-61)
    for( i = 'A'; i <= 'Z'; i++ )
      decode_base64_map[i] = (unsigned char)(i - 'A');
    
    for( i = 'a'; i <= 'z'; i++ )
      decode_base64_map[i] = (unsigned char)(i - 'a' + 26);
    
    for( i = '0'; i <= '9'; i++ )
      decode_base64_map[i] = (unsigned char)(i - '0' + 52);
    
    decode_base64_map['+'] = 62;
    decode_base64_map['/'] = 63;
  }
  
  const unsigned char *src = (const unsigned char*)text.c_str();
  
  int cnt = text.length() / 4;
  unsigned char *dst = new unsigned char[cnt * 3 + 1];
  
  int srcPos;
  int dstPos;
  // decoding: 4*6 bits -> 3*(2*4) bits
  for( int i = 0; i < cnt; i++ ) {
    srcPos = i*4;
    dstPos = i*3;
    
    unsigned char d0 = decode_base64_map[src[srcPos  ]];
    unsigned char d1 = decode_base64_map[src[srcPos+1]];
    unsigned char d2 = decode_base64_map[src[srcPos+2]];
    unsigned char d3 = decode_base64_map[src[srcPos+3]];
    
    dst[dstPos  ] = (unsigned char)(d0 << 2 | d1 >> 4);
    dst[dstPos+1] = (unsigned char)((d1 & 0xf) << 4 | d2 >> 2 & 0xf); // lower and upper nibble ;-)
    dst[dstPos+2] = (unsigned char)(d2 << 6 | d3);
  }
  dst[cnt*3] = '\0';
  string ret = (char*)dst;
  delete dst;
  
  return ret;
}

/** Represents an incoming client-request. The request is being parsed when calling handle().
 */
class HttpRequest 
{
public:
  
  void handle ( ) 
  {
    doHandle();
  }
  
private:
  int client;
  char *buffer;
  size_t buflen;
  unsigned int bufsize;
  unsigned int bufpos;
  
  string method;
  string resource;
  map<string,string> headers;
  map<string,list<string> > parameters;
  
  void readLine ( char * buf, const int BUF_SIZE ) 
  {
    int pos = 0;
    unsigned int bufpostmp = bufpos;
    // bool exitAfterChars = false;
    int exitChars = 0;
    char lastExitChar ='\0';
    while( true ) {
      if( bufpostmp == bufsize && exitChars == 0 ) {
        forceRecv();
      }
      char c = buffer[bufpostmp];
      
      if( c == '\n' || c == '\r' || exitChars ) {
        // this is kind of tricky!
        switch( exitChars ) {
        case 0:
          {
            // there _may_ be another (different) one
            exitChars = 1;
            lastExitChar = c;
          }
          break;
          
        case 1:
          {
            // now, we see:
            if( c == lastExitChar ) {
              // exit now!
              bufpos = bufpostmp; return;
            }else{
              //# c != lastExitChar
              if( c == '\n' || c == '\r' ) {
                // exit one character later!
                exitChars = 2;
              }else{
                // exit now
                bufpos = bufpostmp; return;
              }
            }
          }
          break;
          
        case 2:
          {
            // exit now!
            bufpos = bufpostmp; return;
          }
          break;
        }
        
        c = '\0';
      }
      buf[pos] = c;
      pos = (pos + 1) % BUF_SIZE;
      bufpostmp = (bufpostmp + 1) % buflen;
    }
  }
  
  void readContent ( int contentLength ) 
  {
    int cnt = 0;
    while( ((int)(bufsize - bufpos)) < contentLength ) {
      if( !recv() ) {
        ++cnt;
      }else{
        cnt = 0;
      }
      if( cnt == 300 ) {
        if(VERBOSE) cout << "PRE-FATAL-ERROR 38475";
        throw "TIMEOUT! 38475";
      }
      
      // Give the Browser and the network some time for delivery
      Thread::sleep(100);
    }
    string content(buffer+bufpos,contentLength);
    if(VERBOSE) cout << "content is " << content << endl;
  }
  
  void doHandle ( ) 
  {
    // first line: "GET /test HTTP/1.1" or "POST /test HTTP/1.1"
    // headers: "Keep-Alive: 300", "Content-Length: 8", ...
    // <empty line>
    // params: "foo=asdf"
    
    const int BUF_SIZE = 512;
    char buf[BUF_SIZE];
    
    bool readHeaders = false;
    // bool readContent = false;
    int contentLength = 0;
    bufpos = 0;
    
    while( true ) {
      readLine(buf,BUF_SIZE);
      
      string line(buf);
      if( line.length() > 0 ) {
        while( line[line.length()-1] == '\r'
               || line[line.length()-1] == '\n' ) {
          line = line.substr(0,line.length()-1);
          if( line.length() == 0 ) break;
        }
      }
      if( !readHeaders /*&& !readContent*/ ) {
        // --- read method and uri
        if( line.substr(0,3) == "GET" ) {
          method = "GET";
          line = line.substr(4,line.length()-4);
        }else if( line.substr(0,4) == "POST" ) {
          method = "POST";
          line = line.substr(5,line.length()-5);
        }
        if(VERBOSE) cout << "method is " << method << endl;
        unsigned int pos = 0;
        while( line[pos] != ' ' ) {
          ++pos;
          if( line.length() == pos ) break;
        }
        string uri = line.substr(0,pos);
        if(VERBOSE) cout << "URI is <" << uri << ">" << endl;
        pos = 0;
        while( uri[pos] != '?' ) {
          ++pos;
          if( pos == uri.length() ) break;
        }
        this->resource = uri.substr(0,pos);
        if(VERBOSE) cout << "resource is " << resource << endl;
        if( pos < uri.length() ) {
          string getParams = uri.substr(pos+1,uri.length() - pos-1);
          if(VERBOSE) cout << "get params are " << getParams << endl;
          parseParams(getParams);
        }
        
        readHeaders = true;
      }else if( readHeaders ) {
        // --- read current header
        if( line == "" ) {
          if( method == "POST" && contentLength > 0 ) {
            readContent(contentLength);
          }
          
          return;
        }else{
          // Connection: keep-alive
          unsigned int pos = 0;
          while( line[pos] != ':' ) {
            ++pos;
            if( pos == line.length() ) break;
          }
          string header = line.substr( 0, pos );
          string value = "";
          if( pos < line.length()-1 ) {
            value = line.substr( pos+2, line.length()-pos-2 );
          }
          headers[header] = value;
          if(VERBOSE) cout << "  found header " << header << " = " << value << endl;
          if( header == "Content-Length" ) {
            contentLength = atoi(value.c_str());
            if(VERBOSE) cout << "content-len is " << contentLength << endl;
          }
        }
      }
    }
    
  }
  
private:
  void parseParams ( string params ) 
  {
    if( params.length() > 0 ) {
      int pos1 = 0;
      int pos2 = 0;
      while( pos2 < (int)params.length() ) {
        if( params[pos2] == '&' ) {
          parseParam(params.substr(pos1,pos2-pos1));
          pos1 = pos2+1;
        }
        ++pos2;
      }
      // parse the last one
      parseParam( params.substr( pos1, pos2-pos1 ) );
    }
  }
  
  void parseParam ( string param ) 
  {
    if( param.length() == 0 ) return;
    
    int pos = 0;
    while( param[pos] != '=' ) {
      ++pos;
      if( pos == (int)param.length() ) {
        addParam( param, "" );
        return;
      }
    }
    addParam( param.substr(0,pos), param.substr(pos+1,param.length() - pos - 1)  );
  }
  
  void addParam ( string p, string v ) 
  {
    if( p == "" ) return;
    v = url_decode(v);

    parameters[p].push_back( v );
    if(VERBOSE) cout << "  found param " << p << " = " << v << endl;
  }
  
  unsigned int forceRecv () 
  {
    int cnt = 30;
    while( recv() <= 0 ) {
      --cnt;
      if( cnt == 0 ) {
        if(VERBOSE) cout << "PRE-FATAL-ERROR 345666";
        throw "COMM ERROR! TIMEOUT!";
      }
      
      // give some time for delivery...
      Thread::sleep(100);
    }
    return 0;
  }
  
  unsigned int recv () 
  {
    int ret;
    bufsize += ret = ::recv(client,(char*)buffer+bufsize,buflen-bufsize,0);
    if( ret == 0 ) {
      if(VERBOSE) {
        cout << "conn closed.."  << endl;
        cout.flush();
      }
      throw new ConnectionClosedException();
    }
#ifdef WINDOWS
    if( ret == SOCKET_ERROR ) {
      if(VERBOSE) cout << "SOCKET ERROR at HttpRequest::recv: " << WSAGetLastError() << endl;
      throw new ConnectionClosedException();
    }
#else
    if( ret == -1 ) {
      if(VERBOSE) cout << "SOCKET ERROR at recv!" << endl;
      throw new ConnectionClosedException();
    }
#endif
    return ret;
  }
  
public:
  HttpRequest ( int client, char* buffer, size_t buflen )
  {
    this->client = client;
    this->buffer = buffer;
    this->buflen = buflen;
    this->bufsize = 0;
    // handle();
  }
  
  map<string,list<string> > getParameterMap () const 
  {
    return parameters;
  }
  
  const string getResource () const 
  {
    return resource;
  }
};


class HttpRequestException
{
public:
  HttpRequestException () 
  {
    if(VERBOSE)
      cout << "ERROR (HttpRequestException): wrong request" << endl;
  }
};


#if 0
class HttpRequest2 
{
  int client;
  FILE *socket_file;
  map<string,string> headers;

  ostringstream *cur_part;
  
public:
  HttpRequest2 ( int client ) 
  {
    this->client = _dup(client);
    socket_file = _fdopen(this->client,"r");
    cur_part = new ostringstream();
    
    init();
  }
  
protected:
  void addHeader ( string name, string value ) 
  {
    headers[name] = value;
  }

  void put_to_part ( char c ) 
  {
    (*cur_part) << c;
  }
  string flush_part ( ) 
  {
    string ret = cur_part->str();
    delete cur_part;
    cur_part = new ostringstream();

    return ret;
  }

  bool readParam () 
  {
    while( (c=fgetc(socket_file)) != EOF ) {
      if( c == '=' ) break; // end-of-param-name
      if( c == '&' ) break; // end-of-param
      if( c == ' ' ) break; // end-of-params
      if( c == '\n' ) break; // end-of-params
      put_to_part(c);
    }
    string param_name = flush_part();
    string param_value;
    if( c == '=' ) {
      while( (c=fgetc(socket_file)) != EOF ) {
        if( c == '&' ) break; // end-of-param-value
        if( c == ' ' ) break; // end-of-params
        if( c == '\n' ) break; // end-of-params
        put_to_part(c);
      }
      param_value = flush_part();
    }
    if( param_name != "" ) {
      params[param_name] = param_value;
    }
    if( c == ' ' ) return false;
    return true;
  }
  
  void readFirstLine () 
  {
    char c;
    while( (c=fgetc(socket_file)) != EOF ) {
      if( c == ' ' ) break;
      put_to_part(c);
    }
    method = flush_part();

    while( (c=fgetc(socket_file)) != EOF ) {
      if( c == '?' ) break;
      if( c == ' ' ) break;
      put_to_part(c);
    }
    uri = flush_part();
    if( c == '?' ) {
      // read params
      while( readParam() ) {
      }
    }

    while( (c=fgetc(socket_file)) != EOF ) {
      if( c == '\n' ) break;
      put_to_part(c);
    }
    flush_part(); // HTTP/X.Y
  }
  bool readHeader () 
  {
    char c;
    while( (c=fgetc(socket_file)) != EOF ) {
      if( c == '\n' ) return false;
      if( c == ':' ) break;
      put_to_part(c);
    }
    string name = flush_part();
    
    c=fgetc(socket_file); // ' '

    while( (c=fgetc(socket_file)) != EOF ) {
      if( c == '\n' ) break;
      put_to_part(c);
    }
    string value = flush_part();
    addHeader( name, value );

    return true;
  }
  void readContent () 
  {
  }

  void init () 
  {
    readFirstLine();
    
    while( readHeader() ) {
      // read headers
    }
    
    if( method == "POST" ) {
      readContent();
    }
  }
};
#endif


Semaphore webserver_mtx( _IPC_BASE "webserver_mtx", 0x13444500+0xA0, 1, 1 );


/** A very dirty HTML-Parser that recognizes "<script language=aeinfo>"-Regions
 *  and in turn executes the specified instruction.
 */
class ScriptParser 
{
  char buffer[32768];
  unsigned int bufLength;
  
  char buffer2[32768];
  unsigned int buf2Length;
  
  char *doFlush;
  unsigned int flushLen;
  
  bool useSecondBuffer;
  
  int currentPart;
  int currentPartPos;
protected:
  string currentCommand;
  string currentArg;
  HttpRequest *request;

private:
  bool isWS ( const char c ) const 
  {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r') ;
  }
  
  void switchToFirstBuffer () 
  {
    if( useSecondBuffer ) {
      doFlush = buffer2;
      flushLen = buf2Length;
      useSecondBuffer = false;
      bufLength = 0;
    }
  }

  void switchToSecondBuffer () 
  {
    if( !useSecondBuffer ) {
      doFlush = buffer;
      flushLen = bufLength;
      useSecondBuffer = true;
      buf2Length = 0;
    }
  }
  
  string getFlowUnit () 
  {
    string ret = "l/h";
    {
      typedef PropertyDescriptor<AquaeroData_rev4::FlowSensorModeRegister> flow_mode_t;
      flow_mode_t *flowMode = (flow_mode_t*)properties["flowsensor.mode"];
      if( (flowMode->getValue() & AquaeroData_rev4:: l_minute_hour) != 0 ) {
        ret = "l/m";
      }
    }
    return ret;
  }

  void executeScript () 
  {
    string ret = executeScript(this->currentCommand, this->currentArg);
    // if(VERBOSE) cout << "output is " << ret << " using buf2 = " << useSecondBuffer << endl;
    strcpy(buffer2,ret.c_str());
    buf2Length = ret.length();
#ifdef WINDOWS
    if( ret.length() > 32768 ) {
      MessageBox(NULL, "FATAL ERROR: Buffer Overflow", "aquaerod", MB_SYSTEMMODAL | MB_OK );
    }
#endif
  }
  
protected:
  
  string executeScript ( string currentCommand, string currentArg )
  {
    synchronize syn(*global_aedata_mtx);
    if(VERBOSE) cout << "NOW EXECUTING SCRIPT" << endl;

    
    string ret;
    // ostringstream oss;
    if( currentCommand == "printInput" 
      || currentCommand == "printInputRO" ) {
      if(VERBOSE) cout << "printing an input for #" << currentArg << "#" << endl;
      PropertyDescriptorBase* pd = properties[currentArg];
      if( pd == NULL ) {
        ret = "ERROR:"+currentArg+"???";
      }else{
        ostringstream oss;
        oss << "<input type=text ";
        if( currentCommand == "printInputRO" ) {
          oss << " class=flatinputro readonly ";
        }else{
          oss << " name=\"" << currentArg << "\" " << " class=flatinput ";
        }
        oss << " value=\"";
        pd->printValue( oss );
        oss << "\" ";
        /*if( dynamic_cast<PropertyDescriptor<string>*>(pd) != NULL ) {
          oss << " size=4 ";
        }else{
          oss << " size=1 ";
        }*/
        oss << ">";
        ret = oss.str();
      }


    }else if( currentCommand == "printValue" 
      || currentCommand == "printFriendlyEnumValue"
      || currentCommand == "printfValue" ) {


      if(VERBOSE) cout << "printing the value for #" << currentArg << "#" << endl;
      string field = currentArg;
      string format = "";
      int p = currentArg.find(',');
      if( p != -1 ) {
        field = currentArg.substr(0,p);
        format = currentArg.substr(p+1,currentArg.length() - p - 1);
      }
      PropertyDescriptorBase* pd = properties[field];
      if( pd == NULL ) {
        ret = "ERROR:"+currentArg+"???";
      }else{
        ostringstream oss;
        if( currentCommand == "printfValue" ) {
          // bool forceSign = false; // %+d
          bool padZero = false;   // %0d
          bool padBlank = false;  // % d
          int width = -1;         // %3d
          int precision = -1;     // %.3d

          try {
            if( format.length() < 2 ) {
              if(VERBOSE) cout << "PRE-ERR 318184";
              throw "ERR";
            }
            int pos = 0;
            if( format[pos] != '%' ) {
              if(VERBOSE) cout << "PRE-ERR 7846446";
              throw "ERR";
            }
            ++pos;
            /*if( format[pos] == '+' ) {
              forceSign = true;
              ++pos;
            }*/
            if( format[pos] == '0' ) {
              padZero = true;
              ++pos;
            }else if( format[pos] == ' ' ) {
              padBlank = true;
              ++pos;
            }
            if( format[pos] >= '1' && format[pos] <= '9' ) {
              width = (int)(format[pos] - '0');
              ++pos;
            }
            if( format[pos] == '.' ) {
              ++pos;
              if( format[pos] >= '0' && format[pos] <= '9' ) {
                precision = (int)(format[pos] - '0');
                ++pos;
              }
            }
            
          } catch ( ... ) {
            oss << "<span style=\"color:red; background:black;\">ERROR:format specification contains errors or is too complex!</span>";
          }
          if( padZero || padBlank ) {
            oss << setfill( padZero ? '0' : ' ' );
          }
          if( width > -1 ) {
            oss << setw(width);
          }
          if( precision > -1 ) {
            oss << setprecision(precision);
          }
        }
        pd->printValue(oss);
        ret = oss.str();
        if( currentCommand == "printFriendlyEnumValue" ) {
          string chk = ret;
          ret = "";
          string sep;

          string enumName = pd->getEnumName();
          map<int,string> names = stringEnum_names[enumName];
          map<int,string>::iterator 
            it = names.begin(), end = names.end();
          for( ; it != end; ++it ) {
            if( chk.find(it->second) != -1 ) {
              ret += sep + stringEnum_help[enumName][it->first];
              sep = ", ";
            }
          }
        }
      }


    
    }else if( currentCommand == "printCheckbox" ) {


      int p = currentArg.find(',');
      if( p == -1 ) {
        ret = "ERROR:syntax is printCheckbox(arg1,arg2)! Comma not found!!";
      }else{
        string field = currentArg.substr(0,p);
        string value = currentArg.substr(p+1,currentArg.length() - p - 1);
        if(VERBOSE) cout << "printing checkbox for " << field << ", " << value << endl;
        
        PropertyDescriptorBase* pd = properties[field];
        if( pd == NULL ) {
          ret = "ERROR:"+field+"???";
        }else{
          bool isset = false;
          {
            string chk;
            {
              ostringstream oss;
              pd->printValue(oss);
              chk = oss.str();
              if(VERBOSE) cout << "  checking with " << chk << endl;
            }
            if( chk.find(value) != -1 ) {
              isset = true;
            }
          }
          ostringstream oss;
          oss << "<input type=checkbox name=\""
              << field << "\" ";
          if( isset ) oss << " checked ";
          oss << " value=\"" << value << "\">";
          ret = oss.str();
        }
      }



    }else if( currentCommand == "printSelect" ) {


    // printSelect(fan.1.mode,MANUAL_POWER,MANUAL_RPM,AUTO_LINEAR,AUTO_PROG,AUTO_TARGET)
      int p = currentArg.find(',');
      if( p == -1 ) {
        ret = "ERROR:syntax is printSelect(field,arg2,...,argn)! Comma not found!!";
      }else{
        list<string> lst;
        string field = currentArg.substr(0,p);
        string s = currentArg.substr(p+1,currentArg.length()-p-1);
        while( (p=s.find(',')) != -1 ) {
          lst.push_back( s.substr( 0, p ) );
          s = s.substr( p+1, s.length() - p - 1 );
        }
        lst.push_back( s );

        PropertyDescriptorBase* pd = properties[field];
        if( pd == NULL ) {
          ret = "ERROR:"+field+"???";
        }else{
          string chk;
          {
            ostringstream oss;
            pd->printValue(oss);
            chk = oss.str();
            if(VERBOSE) cout << "  checking with " << chk << endl;
          }
          
          ostringstream oss;
          oss << "<select name=\"" << field << "\">" << endl;
          list<string>::iterator it = lst.begin(), end = lst.end();
          map<int,string> names = stringEnum_names[pd->getEnumName()];
          while( it != end ) {
            string value = *it;
            string display = value;

            // find display for `value'
            {
              map<int,string>::iterator it = names.begin(), end = names.end();
              for( ; it != end; ++it ) {
                if( it->second == value ) {
                  display = stringEnum_help[pd->getEnumName()][it->first];
                  break;
                }
              }
            }

            // is this value the selected one?
            bool isset = false;
            {
              if( chk.find(value) != -1 ) {
                isset = true;
              }
            }
            oss << "<option " << (isset ? "selected" : "")
                << " value=\"" << value << "\">" << display << "</option>" << endl;

            ++it;
          }
          oss << "</select>";
          ret = oss.str();
        }
      }


    }else if( currentCommand == "printFlowUnit" ) {
      ret = getFlowUnit();

/*
    }else if( currentCommand == "test" ) {
      ostringstream oss; oss << testTemp / 10.0f;
      ret = "<tr><td width=400px style='background:"+sensorcolor_html(9)+";'>&nbsp;"+oss.str()+"</td></tr>";
*/    

    }else if( currentCommand == "printBarGraph" ) {
      float minValue = -1.0f, curValue = -1.0f, maxValue = -1.0f;
      string unit, bkcolor = "#0000c0", txtcolor = "white";
      
      if( startsWith(currentArg,"sensor") ) {
        curValue = float_property_value<int>(currentArg+".temp") / 10;
        if( curValue >= 1999.9f ) curValue = -1.0f;
        /*minValue = float_property_value<int>(currentArg+".min") / 10;
        minValue = (curValue < minValue) ? curValue : minValue;*/
        /*maxValue = float_property_value<int>(currentArg+".alarm") / 10;
        maxValue = (curValue > maxValue) ? curValue : maxValue;*/
        minValue = 0.0f;
        maxValue = 100.0f;
        unit = " 	&deg;C";

        /*float _min = minValue, _alarm = maxValue, 
          _opt = float_property_value<int>(currentArg+".opt") / 10, 
          _max = float_property_value<int>(currentArg+".max") / 10;*/
        bkcolor = sensorcolor_html(currentArg[7] - '0');
        txtcolor = "black";
      }else if( startsWith(currentArg,"fan") ) {
        curValue = (0xff & (int)((PropertyDescriptor<unsigned char>*)properties[currentArg+".setPower"])->getValue()) / 2.55f;
        curValue = (int)(curValue + 0.5f);
        minValue = 0.0f;
        maxValue = 100.0f;
        unit = "%";
      }else if( startsWith(currentArg,"flowsensor") ) {
        curValue = float_property_value<int>(currentArg+".flow") / 100;
        minValue = float_property_value<int>(currentArg+".lowAlarm") / 100;
        maxValue = minValue;
        maxValue = ((int)((curValue*0.95) / minValue + 1)) * minValue;
        unit = " "+getFlowUnit();
      //}else if( currentArg == "tubemeter" ) {
      //}else if( startsWith(currentArg,"aquastream") ) {
      }else if( startsWith(currentArg,"powersensor") ) {
        curValue = float_property_value<int>(currentArg+".power");
        minValue = 0.0f;
        maxValue = 150.0f;
        while( maxValue < curValue ) maxValue += 50.0f;
        unit =  " W";
      }else{

      }

      // <table width="90%" cellpadding=0 cellspacing=0 style="border:1px solid #808080;">
      // <tr><td style="background:#ffffff; width:0%;">&nbsp;</td><td>&nbsp;</td></tr></table>

      // XXX: still, JavaScript and CSS is a big mess! If we do an update of an BarGraph with AJAX
      // by "just replacing" the whole HTML-Code, the style-tags will not be evaluated! What a crap!
      ostringstream oss;
      if( curValue != -1.0f ) {
        float width = (curValue - minValue) / (maxValue - minValue);
        oss << "<table id=\"barGraph."<<currentArg<<"\" width=\"90%\" cellpadding=\"0\" cellspacing=\"0\" style=\"border:1px solid #808080;\">"
            << "<tr>";
        if( width > 0.0f ) {
          oss << "<td style=\"background:" << bkcolor << "; width:" << (width*100.0f) << "%;\">" /*&nbsp;*/;
          if( width > 0.2f ) {
            oss << "<span style=\"margin-left:4px; font-family:Arial; font-size:10pt; color:" << txtcolor << ";\">" 
                << curValue << unit << "</span>";
          }else{
            oss << "&nbsp;";
          }
          oss << "</td>";
        }
        if( width < 1.0f ) {
          oss << "<td>";
          if( width <= 0.2f ) {
            oss << "<span style=\"margin-left:4px; font-family:Arial; font-size:10pt; color:black;\">" 
                << curValue << unit << "</span>";
          }else{
            oss << "&nbsp;";
          }
          oss << "</td>";
        }
        oss << "</tr></table>";
      }
      ret = oss.str();      



    }else if( currentCommand == "printSensorSelection" ) {
      if(VERBOSE) cout << "printing Sensor-Selection " << currentArg << endl;
      string curValue;
      {
        ostringstream hs;
        PropertyDescriptorBase *pd = properties[currentArg];
        pd->printValue(hs);
        curValue = hs.str();
        if(VERBOSE) cout << "printSensorSelection: curValue = " << curValue << endl;
      }
      ostringstream oss;
      oss << "<select name=\""<< currentArg <<"\">" << endl;
      oss << "  <option value=\"none\" " << (curValue == "none" ? "selected" : "") << ">"
          << "Kein Sensor" << "</option>" << endl;
      if( startsWith(currentArg,"led") ) {
        oss << "  <option value=\"all\" " << (curValue == "all" ? "selected" : "") << ">"
            << "Alle Sensoren" << "</option>" << endl;
      }

      oss << "  <option value=\"sensor.1\" " << (startsWith(curValue,"sensor.1") ? "selected" : "") << ">";
      properties["sensor.1.name"]->printValue(oss);
      oss << "</option>" << endl;

      oss << "  <option value=\"sensor.2\" " << (startsWith(curValue,"sensor.2") ? "selected" : "") << ">";
      properties["sensor.2.name"]->printValue(oss);
      oss << "</option>" << endl;

      oss << "  <option value=\"sensor.3\" " << (startsWith(curValue,"sensor.3") ? "selected" : "") << ">";
      properties["sensor.3.name"]->printValue(oss);
      oss << "</option>" << endl;

      oss << "  <option value=\"sensor.4\" " << (startsWith(curValue,"sensor.4") ? "selected" : "") << ">";
      properties["sensor.4.name"]->printValue(oss);
      oss << "</option>" << endl;

      oss << "  <option value=\"sensor.5\" " << (startsWith(curValue,"sensor.5") ? "selected" : "") << ">";
      properties["sensor.5.name"]->printValue(oss);
      oss << "</option>" << endl;

      oss << "  <option value=\"sensor.6\" " << (startsWith(curValue,"sensor.6") ? "selected" : "") << ">";
      properties["sensor.6.name"]->printValue(oss);
      oss << "</option>" << endl;
      oss << "</select>";
      ret = oss.str();

    // 
    // printMultipleSelect
    // 
    }else if( currentCommand == "printDisplayPagesSelection" ) {
      // LCDPageRegister, 1..4194304
      ostringstream oss;
      oss << "<select name=\"lcd.pages\" disabled multiple size=25>" << endl;
      AquaeroData_rev4::LCDPageRegister pages = 
        ((PropertyDescriptor<AquaeroData_rev4::LCDPageRegister>*)properties["lcd.pages"])->getValue();
      int i = 1;
      for( ; i <= 4194304; i *= 2 ) {
        string curName = stringEnum_names["LCDPageRegister"][i];
        string curHelp = stringEnum_help["LCDPageRegister"][i];
        bool isset = (((unsigned long)pages) & i) != 0;
        oss << "  <option value=\""<< curName <<"\" " << (isset ? " selected " : "") << ">"
            << curHelp << "</option>" << endl;
      }
      oss << "</select>";
      ret = oss.str();


    }else if( currentCommand == "printVersionInfo" ) {
      const char *buildTime =
#ifdef WINDOWS
        __TIMESTAMP__;
#else
        __DATE__ " " __TIME__;
#endif
      const char *version = VERSION;
      ret = string(version);

      if( currentArg == "long" ) {
        ret = ret
#ifdef WINDOWS
          +string("/WIN32")
#else
          +string("/Linux")
#endif
          +string(" ")+string(buildTime);
      }
      
      
      
    }else if( currentCommand == "printParamValue" ) {
      ret = "";
      if( request != NULL ) {
        if( request->getParameterMap()[currentArg].size() > 0 ) {
          ret = request->getParameterMap()[currentArg].front();
        }
      }
      
    
    }else if( currentCommand == "printHelpOnFields" ) {
      string filter;
      
      if( request != NULL ) {
        if( request->getParameterMap()["filter"].size() > 0 ) {
          filter = request->getParameterMap()["filter"].front();
        }
      }
      
      ostringstream oss;
      oss << "<table class=helpOnFields>" << endl;
      oss << "<tr><th>Field-Name</th><th width=\"33%\">Current Value</th><th>Description</th></tr>" << endl;
      map<string,PropertyDescriptorBase*>::const_iterator it = properties
        // .begin()
        .lower_bound(filter)
        ;
      
      while( it != properties.end()
             && startsWith( (it == properties.end()) ? string("") : it->first, filter ) ) {
        if( it->second == NULL ) {
          ++it;
          continue;
        }
        
        oss << "<tr><td class=fieldName><a href=\"?filter=" << it->first << "\">" << it->first << "</a></td>"
            << "<td class=fieldValue>";
        it->second->printValue(oss);
        oss << "</td>"
            << "<td class=fieldDesc>"
            << it->second->getDescription() << "</td></tr>" << endl;
        
        
        if( it->first == filter ) {
          // give detailed information, if available (for example: the values of enum's)
          try {
            string enumName = it->second->getEnumName();
            
            if( stringEnum_help.find(enumName) != stringEnum_help.end() ) {
              // there is help available on an enum:
              oss << "<tr>"
                  << "<td class=enumInfo colspan=3>"
                  << "Further information on the values of enumerated type &quot;"
                  << "<span class=highlight>" << enumName << "</span>" << "&quot;" << "</td></tr>";
              
              map<int,string>::const_iterator
                helpIt = stringEnum_help[enumName].begin(),
                helpEnd = stringEnum_help[enumName].end();
              oss << "<tr><td class=enumInfo colspan=3><table class=enumInfoValues>";
              for( ; helpIt != helpEnd; ++helpIt ) {
                oss << "<tr>"
                    << "<td class=enumInfoDetailValue1>"
                    << stringEnum_names[enumName][helpIt->first]
                    << "</td>"
                    << "<td class=enumInfoDetailValue2>(="
                    << helpIt->first
                    << ")</td>"
                    << "<td class=enumInfoDetailHelp>";
                
                if( helpIt->second == "" ) oss << "?";
                else oss << helpIt->second;
                oss << "</td></tr>";
              }
              oss << "</table></td></tr>";
            }
          } catch( ... ) {
          }
        }
        
        ++it;
      }      
      ret = oss.str();
      
      
    }else{
      ret = "<span style=\"color:red; background:black;\">output of "+currentCommand+"("+currentArg+")</span>";
    }
    if(VERBOSE) cout << "DONE WITH SCRIPT" << endl;
    return ret;
  }
  
public:
  ScriptParser ( ) 
  {
    doFlush = NULL;
    useSecondBuffer = false;
    
    reset();
    currentPart = 1;
    currentPartPos = 0;
    bufLength = 0;
    buf2Length = 0;
  }
  
  void reset () 
  {
    flushLen = 0;
    doFlush = NULL;
  }
  
  void feed ( const char c ) 
  {
    // <script language="aeinfo">command arg</script>
    const string part1 = "<script";
    const string part2 = "language";
    const string part3 = "=";
    const string part4 = "aeinfo";
    const string part5 = ">";
    const string part8 = "</script";
    const string part9 = ">";
    
    const string *thePart = NULL;
    switch( currentPart ) {
    case 1: thePart = &part1; break;
    case 2: thePart = &part2; break;
    case 3: thePart = &part3; break;
    case 4: thePart = &part4; break;
    case 5: thePart = &part5; break;
    case 8: thePart = &part8; break;
    case 9: thePart = &part9; break;
    }
    
    bool addCharToCurrentBuffer = true;
    if( currentPartPos == 0 && isWS(c) ) {
      // OK
    }else{
      if( thePart != NULL ) {
        if( c == (*thePart)[currentPartPos] ) {
          if( currentPartPos == 0 && currentPart == 1 ) {
            switchToSecondBuffer();
          }
          
          ++currentPartPos;
          if( currentPartPos == (int)thePart->length() ) {
            ++currentPart;
            if(VERBOSE) cout << "current part is now " << currentPart << endl;
            currentPartPos = 0;
            if( currentPart == 10 ) {
              currentPart = 1;
              executeScript();
              switchToFirstBuffer();
              addCharToCurrentBuffer = false;
              currentCommand = "";
              currentArg = "";
            }
          }
        }else{
          currentPartPos = 0;
          currentPart = 1;
          switchToFirstBuffer();
        }
        
      }else{
        if( c == '<' ) {
          currentPart = 8;
          currentPartPos = 1;
        }else{
          if( isWS(c) ) {
            ++currentPart;
            currentPartPos = 0;
          }else{
            ++currentPartPos;
            switch( currentPart ) {
            case 6: currentCommand += c; break;
            case 7: currentArg += c; break;
            }
          }
        }
      }
    }
    
    if( addCharToCurrentBuffer ) {
      if( useSecondBuffer ) {
        buffer2[buf2Length++] = c;
      }else{
        buffer[bufLength++] = c;
      }
#ifdef WINDOWS
      if( bufLength>=32768 || buf2Length>=32768 ) {
        MessageBox(NULL,"ARGH!!","aeinfo",MB_OK|MB_ICONEXCLAMATION|MB_DEFBUTTON1|MB_APPLMODAL);
      }
#endif
    }
  }
  
  bool flush () const 
  {
    return doFlush != NULL && (flushLen > 0);
  }
  
  const char* getBuffer () const 
  {
    if( doFlush == NULL ) {
      if( useSecondBuffer ) {
        return buffer2;
      }else{
        return buffer;
      }
    }
    
    return doFlush;
  }
  unsigned int getBufferLength() const 
  {
    if( doFlush == NULL ) {
      if( useSecondBuffer ) {
        return buf2Length;
      }else{
        return bufLength;
      }
    }
    
    return flushLen;
  }
};


void prepare_yylex( FILE * );
void run_yylex ();

Semaphore parser_mtx(_IPC_BASE "parser_mtx", 0x13444500+0x42, 1, 1);
class ScriptParser2 *yyinst = NULL;


/** An improved parser, created with flex.
 */
class ScriptParser2 
  : protected ScriptParser
{
  ostringstream outputBuffer;
  ostringstream *tempBuffer;
  bool in_script;

public:
  ScriptParser2 ( FILE* file, HttpRequest *request ) 
  {
    tempBuffer = NULL;
    in_script = false;
    prepare_yylex(file);
    this->request = request;
  }

  void parse () 
  {
    yyinst = this;
    run_yylex();
  }

  const string executeScript ( const string &s ) 
  {
    int pos = s.find(' ');
    string cmd;
    string arg;
    
    if( pos == -1 ) {
      cmd = s;
      arg = "";
    }else{
      cmd = s.substr(0,pos);
      arg = s.substr(pos+1,s.length()-pos-1);
    }
    return this->ScriptParser::executeScript(cmd,arg);
    // return "<span style='color:red; background:black; font-weight:bold;'>SCRIPT</span>";
  }

  void startScript ( const char *yytext )
  {
    if( !in_script ) {
      tempBuffer = new ostringstream();
      in_script = true;
    }
  }
  void endScript ( const char *yytext ) 
  {
    if( in_script ) {
      string script = tempBuffer->str();
      in_script = false;
      delete tempBuffer;
      tempBuffer = NULL;
      outputBuffer << executeScript( script );
    }else{
      outputBuffer << yytext;
    }
  }
  void feed ( const char *yytext ) 
  {
    if( in_script ) {
      (*tempBuffer) << yytext;
    }else{
      outputBuffer << yytext;
    }
  }
  string getData () 
  {
    return outputBuffer.str();
  }
};

#include "lex.yy.c"

/** A STL-function for case-in-sensitive string comparison.
 */
struct icaseless 
  : public binary_function<string, string, bool> 
{
  bool operator() ( const string& x, const string& y ) const
  {
    return (strcasecmp(x.c_str(),y.c_str()) < 0 );
  }
};

/** Represents a HTTP-Response which answers a specific HttpRequest.
 */
class HttpResponse
{
  int client;
  HttpRequest request;
  map<string,string> headers;
  map<string,const char*,icaseless> mimeTypes;
  set<string> parseableMimeTypes;
  set<string> binaryMimeTypes;

  bool checkURI ( string uri ) 
  {
    if( uri == "" ) return false;
    if( uri.find("..") != -1 ) return false;
    
    return true;
  }
  
  string getHead () 
  {
    ostringstream oss;
    map<string,string>::const_iterator 
      it = headers.begin(), end = headers.end();
    for( ; it != end; ++it ) {
      oss << it->first << ": " << it->second << "\r\n";
    }
    
    return oss.str();
  }
  
public:
  void addHeader ( string name, string value ) 
  {
    headers[name] = value;
  }

  void sendOK ( ) 
  {
    send("HTTP/1.0 200 OK\r\n"+getHead()+"\r\n");
  }
  
  void sendError ( string code, string msg ) 
  {
    send("HTTP/1.0 "+code+" "+msg+"\r\n"
         +getHead()+"\r\n"
         "<html><head><title>Error "+code+"</title></head>"
         "<body><h1>Error "+code+"</h1>"+msg+"</body></html>");
  }
  
  void send ( string s ) 
  {
#ifdef WINDOWS
    int ret = ::send(client,s.c_str(),s.length(),0);
    if( ret == SOCKET_ERROR ) {
      if(VERBOSE) cout << "SOCKET_ERROR in HttpResponse::send: " << WSAGetLastError() << endl;
      throw new ConnectionClosedException();
    }
#else
    int ret = ::send(client,s.c_str(),s.length(),MSG_NOSIGNAL);
    if( ret == -1 ) {
      if(VERBOSE) cout << "SOCKET_ERROR in HttpResponse::send: " << endl;
      throw new ConnectionClosedException();
    }
#endif
  }
  
  HttpResponse ( int client, HttpRequest &request )
    : client(client), request(request) 
  {
    addHeader( "Server", "aquaerod/" VERSION );
#if 0
    {
      char tmpbuf[128];
      time_t ltime;
#ifdef WINDOWS
      _tzset();
#else
      tzset();
#endif
      time( &ltime );
      struct tm *today = localtime( &ltime );
      // DDDD, DD MMMM JJJJ HH:MM:SS ZONE
      strftime( tmpbuf, 128, "%a, %d %b %Y %H:%M:%S %z", today );
      addHeader( "Date", string(tmpbuf) );
    }
#endif
    addHeader( "Connection", "Close" );

    /* FIXME unknown MIMEs: application/octet-stream??!
     */
    mimeTypes[".js"] = "text/javascript";
    mimeTypes[".htm"] = "text/html";
    mimeTypes[".png"] = "image/png";
    mimeTypes[".php"] = "text/php";
    mimeTypes[".css"] = "text/css";
    mimeTypes[".txt"] = "text/plain";
    mimeTypes[".jpg"] = "image/jpeg";
    mimeTypes[".gif"] = "image/gif";
    mimeTypes[".swf"] = "application/x-shockwave-flash";
    mimeTypes[".xml"] = "application/xml";
    mimeTypes[".html"] = "text/html";
    mimeTypes[".jpeg"] = "image/jpeg";
    
    parseableMimeTypes.insert( "text/html" );
    parseableMimeTypes.insert( "text/php" );
    parseableMimeTypes.insert( "application/xml" );
    parseableMimeTypes.insert( "text/javascript" );

    binaryMimeTypes.insert( "image/png" );
    binaryMimeTypes.insert( "image/jpeg" );
    binaryMimeTypes.insert( "image/gif" );
    binaryMimeTypes.insert( "application/x-shockwave-flash" );
  }
  
  void send () 
  {
    /*
HTTP/1.1 200 OK
Date: Mon, 19 Jun 2006 22:43:34 GMT
Server: Apache/1.3.20 (Linux/SuSE) PHP/4.0.6 mod_perl/1.26
Last-Modified: Tue, 01 Oct 2002 00:17:26 GMT
ETag: "24f07b-34f-3d98e996"
Accept-Ranges: bytes
Content-Length: 847
Keep-Alive: timeout=15, max=100
Connection: Keep-Alive
Content-Type: text/html


Authorization: Basic XXXXXXXXXXXXXXXXXXXXXXX
Der Authorization-Header wird vom Client i.d.R. nur angegeben, wenn eine 
vorherige Anfrage an den Server mit dem Status-Code 401 Unauthorized abgelehnt wurde. 
Der Client sucht daraufhin nach den Authentifizierungsdaten; Web-Browser fragen den 
Benutzer meist nach Benutzername und Passwort. Die Anfrage wird dann mit dem Authorization-Header 
erneut gestellt.

FIXME: check "3.6.1 Chunked Transfer Coding" in "RFC 2616"!
    */
    
    string res = request.getResource();
    const char* mime = NULL; // Content-Type
    bool binMode = false;
    
    if( checkURI(res) ) {
      if( res[res.length() - 1] == '/' ) {
        res += "index.htm";
        if(VERBOSE) cout << "res is " << res << endl;
      }

      {
        bool useParser = false;

        string::size_type pos;
        if( (pos=res.find_last_of('.')) != string::npos ) {
          string ext = res.substr( pos, res.length() - pos );
          mime = mimeTypes[ext];
          
          if( mime != NULL ) {
            useParser = parseableMimeTypes.find(mime) != parseableMimeTypes.end();
            binMode = binaryMimeTypes.find(mime) != binaryMimeTypes.end();
          }
        }

        if( mime != NULL ) addHeader("Content-Type", string(mime));
        
        // FIXME: Header Cache-Control: no-cache oder max-age=[Anzahl Sekunden]
        /*
cache-control Dieser Header bestimmt, wie ein Cache mit dem Dokument bzw. der Anfrage nach einem Dokument zu verfahren hat. In der Regel richtet sich dieser Header nicht an den Cache eines Browsers, sondern an den eines zwischengeschalteten Proxies. Es gibt Browser (z.B. einige WAP-Mobiltelefone), die diesen Header und andere Caching-Angaben ignorieren; dies erschwert h�ufig die Implementation von personengebundenen Anwendungen, z.B. einer HTML-Oberfl�che zur Verwaltung seiner E-Mails oder seines Bankkontos.

Anweisungen:

    * no-cache: Der Cache soll vor der Weitergabe des Dokuments pr�fen, ob es sich an der urspr�nglichen Quelle (also auf dem Server) inzwischen ver�ndert hat. Falls ja, soll er das Dokument erneut vom Server anfordern. Der Cache darf also das Dokument speichern, obwohl die Bezeichnung no-cache etwas anderes suggeriert; vor der Weitergabe muss er allerdings die G�ltigkeit pr�fen. Siehe auch no-store.
    * no-store: Der Cache darf das Dokument nach Weitergabe keinesfalls speichern. Diese Anweisung ist insbesondere f�r vertrauliche Informationen oder Daten einer Session sinnvoll. Siehe auch no-store.
    * max-age=[Anzahl Sekunden]: Die Antwort des Servers bzw. die Anfrage des Clients ist veraltet, wenn seit dem Empfang mehr als [Anzahl Sekunden] verstrichen sind. Das Dokument muss erneut angefragt bzw. gesendet werden.
    * only-if-cached: Der Cache darf das Dokument des Servers nur weiterleiten, wenn er es bereits im Cache h�lt. Er darf keine neue Anfrage an den Server stellen. Diese Anweisung macht z.B. Sinn, um Fehlermeldungen zu vermeiden, wenn die Verbindung zum Server gest�rt oder sehr langsam ist.

        */
      
        char buffer[32768];
        // TODO: sym-links!
        string fname = htdocs;
        fname += res;
#ifdef WINDOWS
        int fileFD = _open(fname.c_str(),_O_RDONLY | (binMode ? _O_BINARY : 0));
#else
        int fileFD = open(fname.c_str(), O_RDONLY );
#endif
        if(VERBOSE) {
          if( fileFD == -1 ) {
            cout << "File does not exist" << endl;
          }else{
            cout << "File exists" << endl;
          }
        }
        //ifstream istr(fname.c_str(), (binMode ? ios::binary | ios::in : ios::in) );
        //if( istr.fail() ) {
        if( fileFD == -1 ) {
          if(VERBOSE) cout << "ERROR 404" << endl;
          sendError("404","Not Found");
          // _close(fileFD);
          return;
        }
        if( useParser ) {
          // XXX: add Last-Modified = now / age = 0
          addHeader( "Cache-Control", "no-store" );
#if 0
          if(false){
            ScriptParser parser;
            bool sentOK = false;
        
            //while( !istr.eof() ) {
            
#ifdef WINDOWS
            while( !_eof(fileFD) ) 
#else
            while( true ) 
#endif
            {
#ifdef WINDOWS
              int bytesRead = _read(fileFD,(void*)buffer,32768);
#else
              int bytesRead = read(fileFD,(void*)buffer,32768);
              if( bytesRead == 0 ) break;
#endif
              //istr.read((char*)buffer,32768);

              //if( istr.fail() && !istr.eof() ) {
              if( bytesRead == -1 && !_eof(fileFD) ) {

                sendError("500","Internal Server Error");
#ifdef WINDOWS                
                _close(fileFD);
#else
                close(fileFD);
#endif
                return;
              }
              
              for( int i = 0; i < /*istr.gcount()*/bytesRead; ++i ) {
                parser.feed( buffer[i] );
                if( parser.flush() ) {
                  if( !sentOK ) {
                    sendOK();
                    sentOK = true;
                  }
                  ::send(client,(char*)parser.getBuffer(),parser.getBufferLength(),MSG_NOSIGNAL);
                  parser.reset();
                }
              }
              // ::send(client,(char*)buffer,istr.gcount(),0);
            }
            if( !sentOK ) {
              sendOK();
              sentOK = true;
            }
            ::send(client,(char*)parser.getBuffer(),parser.getBufferLength(),MSG_NOSIGNAL);
          }
#endif
          // === THE NEW PARSER ===
          {
#ifdef WINDOWS
            FILE *file = _fdopen(fileFD,"r");
#else
            FILE *file = fdopen(fileFD,"r");
#endif
            {
              synchronize syn(parser_mtx);
              {
                ScriptParser2 parser(file, &request);
                parser.parse();

                string data = parser.getData();
                sendOK();
                ::send( client, (char*)data.c_str(), data.length(), MSG_NOSIGNAL );
              }
            }

            fclose(file);
            return;
          }
        }else{
          bool sentOK = false;
          if( binMode ) {
            // add Content-Length
#ifdef WINDOWS
            struct _stat buf;
            _stat
#else
            struct stat buf;
            stat
#endif
              ( fname.c_str(), &buf );
            if( buf.st_size > 0 ) {
              char tmp[20];
#ifdef WINDOWS
              itoa(buf.st_size,tmp,10);
#else
              sprintf( tmp, "%lu", buf.st_size );
#endif
              addHeader( "Content-Length", string(tmp) );
            }
          }
          // XXX: add Last-Modified
          //while( !istr.eof() ) {
#ifdef WINDOWS
          while(!_eof(fileFD) )
#else
          while(true)
#endif
          {
            //istr.read((char*)buffer,32768);
#ifdef WINDOWS
            int bytesRead = _read(fileFD,(void*)buffer,32768);
#else
            int bytesRead = read(fileFD,(void*)buffer,32768);
            if( bytesRead == 0 ) break;
#endif

            //if( istr.fail() && !istr.eof() ) {
            if( bytesRead == -1
#ifdef WINDOWS
                && !_eof(fileFD)
#endif
                ) {
              if( !sentOK ) {
                sendError("500","Internal Server Error");
              }
#ifdef WINDOWS
              _close(fileFD);
#else
              close(fileFD);
#endif
              return;
            }
            if( !sentOK ) {
              sendOK();
              sentOK = true;
            }
            ::send(client,(char*)buffer,/*istr.gcount()*/bytesRead,MSG_NOSIGNAL);
          }
        }
#ifdef WINDOWS
        _close(fileFD);
#else
        close(fileFD);
#endif
      }
    }else{
      sendError("400","Malformed URL");
    }
  }
};



class AsyncResponder 
  : public Thread
{
  int client;
  bool finished;

  void close () 
  {
#ifdef WINDOWS
    {
      int ret = closesocket(client);
      if( ret == SOCKET_ERROR ) {
        MessageBox(NULL,"ERRRRRRRRRRRRR 646484531","aeinfo",MB_OK|MB_ICONEXCLAMATION|MB_DEFBUTTON1|MB_APPLMODAL);
        if(VERBOSE) {
          cout << "ERROR during closesocket" << endl;
        }
      }
    }
#else
    ::close(client);
#endif
  }

public:
  AsyncResponder ( int client ) 
    : Thread("AEHTTPD"), client(client), finished(false)
  {
  }
  virtual ~AsyncResponder ()
  {
  }
  
  bool hasFinished () 
  {
    return finished;
  }
  
  void run () 
  {
      // long bytesGot;
      char buffer[655360];                // 640k
      try {
        HttpRequest req( client, buffer, 655360 );
        req.handle();


        
        if( req.getResource() == "/setdata" ) {
          ///////////////////////////
          //////// set values ///////
          ///////////////////////////
          map<string,list<string> > params = req.getParameterMap();
          
          {
            global_aedata_mtx->lock();
            
            map<string,list<string> >::const_iterator 
              it = params.begin(), end = params.end();
          
            for( ; it != end; ++it ) {
              if(VERBOSE) cout << "processing property " << it->first << endl;
            
              PropertyDescriptorBase *pd = properties[it->first];
              try {
                if( pd != NULL ) {
                  // the parameter-key is a property-name
                  list<string> values = it->second;
                  if( values.size() == 0 ) {
                    // --- do nothing


                  }else if( values.size() == 1 ) {
                    // --- directly set the value
                    if(VERBOSE) cout << "there is one value " << values.front() << endl;
                    pd->readValue( values.front() );


                  }else{
                    // --- more than one value for this property...
                    //     construct a enum-string-value
                    //#values.size() > 1
                    ostringstream oss;
                    {
                      list<string>::const_iterator vit = values.begin(), vend = values.end();
                      oss << *vit;
                      ++vit;
                      for( ; vit != vend; ++vit ) {
                        oss << '|';
                        oss << *vit;
                      }
                    }
                    if(VERBOSE) cout << "constructed compound value " << oss.str() << endl;
                    pd->readValue( oss.str() );


                  }
                }
              } catch ( const ReadOnlyException *rex ) {
                if(VERBOSE) cout << "caught READONLY EXCEPTION" << endl;
              }
            } // for `it' in request-names
            
            global_aedata->synchronizeClock();
            aquaero_rev4 dataToWrite = *global_aedata;
            global_aedata->serverSideWriteFlag = (AquaeroData_rev4::EServerSideWriteFlag)1/*RUNNING*/;
            global_aedata_mtx->unlock();
            
            // now, the aedata-structure represents the requested values and concurrent reads are blocked. Therefore,
            // we can safely redirect the user...
            if( params["nextAction"].size() == 0 ) {
              HttpResponse resp( client, req );
              resp.sendOK();
            }else{
              HttpResponse resp( client, req );
              resp.addHeader( "Location", "/wait?nextAction="+params["nextAction"].front() );
              resp.sendError( "302", "Found" );
            }
            this->close();
            
            // the user sees now, his updated values -- the write will be performed in the background...
            // TODO: we should include the write-routine into the Server-Side read-loop (if the ServerSideWriteFlag says
            //   "WRITE_PENDING" -- we can perform the write using the global_aedata). This will avoid unnecessary writes...
            {
              synchronize syn1(*global_write_mtx);
              
              if(VERBOSE) cout << "WWWWWWWWWW   BEFORE serverSideWrite" << endl;
              global_comm_srv->serverSideWrite( dataToWrite );
              if(VERBOSE) cout << "WWWWWWWWWW   INTERMEDIATE serverSideWrite" << endl;
              global_aedata->serverSideWriteFlag = (AquaeroData_rev4::EServerSideWriteFlag)2/*UPDATE_PENDING*/;
              if(VERBOSE) cout << "WWWWWWWWWW   AFTER  serverSideWrite" << endl;
            }
          } // BLOCK
          // // TODO: if the writes are within the Server-Side read-loop, we can drop this (this is only )
          // global_comm_srv->read( *global_aedata );

          finished = true;
          return;
	  

        }else if( req.getResource() == "/shutdown" ) {
          ///////////////////////////
          ////////  shutdown  ///////
          ///////////////////////////
          HttpResponse resp( client, req );
          resp.sendOK();
          resp.send("<html><body>The daemon has been stopped.</body></html>");
          // leave the connection open (will be closed upon stopping :-))
#ifdef WINDOWS
          ExitProcess(0);
#else
          exit(0);
#endif
          return;

        }else if( req.getResource() == "/wait" ) {
          //////////////////////////////
          //////// wait          ///////
          //////////////////////////////
          Thread::sleep( 1000 );
          map<string,list<string> > params = req.getParameterMap();
          HttpResponse resp( client, req );
          if( params["nextAction"].size() == 0 ) {
            resp.sendError( "500", "Internal Server Error" );
          }else{
            resp.addHeader( "Location", params["nextAction"].front() );
            resp.sendError( "302", "Found" );
          }


        }else{
          //////////////////////////////
          //////// send document ///////
          //////////////////////////////
          if(VERBOSE) cout << "response..." << endl;

          HttpResponse resp( client, req );
          resp.send();
        }


      } catch( const ConnectionClosedException *ccex ) {
        if(VERBOSE) {
          cout << "discarding request - connection has been closed gracefully" << endl;
          cout.flush();
        }
      } catch( const char* ccc ) {
        if(VERBOSE) {
          cout << "FATAL ERROR: " << ccc << endl;
          cout.flush();
        }
      // } catch( const void* vvv ) {
      //   if(VERBOSE) {
      //     cout << "FATAL ERROR: " << typeid(vvv).name() << endl;
      //     cout.flush();
      //   }
      } catch( ... ) {
        if(VERBOSE) {
          cout << "FATAL ERROR: unknown exception caught" << endl;
          cout.flush();
        }
      }

      this->close();

      finished = true;
  }
};


class WebServerMasterThread 
  : public Thread
{
  int s;
  list<AsyncResponder*> threads;

  void init () 
  {
#ifdef WINDOWS
    WSADATA wsaData;
    {
      int err = WSAStartup( 0x0002, &wsaData );
      if ( err ) if(VERBOSE) cout << "WSA error" << endl;
    }
#endif

    s = socket(PF_INET, SOCK_STREAM, 0);
    if(VERBOSE) cout << "socket " << s << endl;
#ifndef WINDOWS
    int on = 1;
    if(VERBOSE) cout << "setsockopt(SO_REUSEADDR)" << endl;
    if( setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0 ) {
      cout << "ERROR: setsockopt(SO_REUSEADDR) failed" << endl;
    }
#endif
    
    sockaddr_in soAddr;
    soAddr.sin_addr.s_addr = inet_addr( "0.0.0.0" );
    soAddr.sin_family = AF_INET;
    soAddr.sin_port = 0x3075;             // port 30000

    int err = bind( s, (sockaddr*)&soAddr, sizeof( soAddr ) );
    if(VERBOSE) cout << "bind " << err << endl;
    if( err == -1 ) {
      cerr << "ERROR: at binding..."  << endl;
#ifndef WINDOWS
      if(VERBOSE) cout << "  errno = " << errno << endl;
      if( errno == EADDRINUSE ) {
        cerr << "Address already in use!" << endl;
      }
#else
      if(VERBOSE) cout << "  errno = " << errno << ", " << GetLastError() << endl;
      if( GetLastError() == WSAEADDRINUSE ) {
        cerr << "Address already in use!" << endl;
      }
#endif
      cerr << " !! Webserver will not be started !!" << endl;
      return;
    }
  }

public:
  WebServerMasterThread ()
    : Thread("WebServerMasterThread")
  {
    s = -1;
  }

  void run () 
  {
    this->init();

    int foo = listen(s, 10);
    if(VERBOSE) cout << "listen " << foo << endl;

    do {
      sockaddr_in fromAddr;
      socklen_t len = sizeof(fromAddr);
      // FIXME: synchronize accept?!?
      int client = accept( s, (struct sockaddr*) &fromAddr, &len );
      if(VERBOSE) cout << "acctepted client socket " << client << endl;
      if( client == -1 ) {
        if(VERBOSE) cout << "ERROR with socket: errno = " << errno << endl;
      }
      
      AsyncResponder *cur = new AsyncResponder(client);
      //*
      cur->start();
      
      // --- are there some threads that can be cleaned up??
      {
        list<AsyncResponder*>::iterator it = threads.begin();
        while( it != threads.end() ) {
          if( (*it)->hasFinished() ) {
            delete *it;
            it = threads.erase(it);
          }else{
            ++it;
          }
        }
      }

      threads.push_back(cur);
      //*/
      /*cur->run();
      delete cur;*/
    } while( true );
  }
  
};


void startWebServer ( )
{
  WebServerMasterThread *wsmt = new WebServerMasterThread();
  wsmt->start();
}

#if 0
int main ( int, char ** )
{
  startWebServer();

  sleep(3600);
  cout << "exiting..." << endl;

  return 0;
}
#endif
