// -*- c++ -*- $Id: objectstream.h 21 2006-06-21 16:03:50Z matt $

/* object_stream: C++ streaming-class for in- and output that operates on a specific memory-region... 
 * 
 * Copyright (C) 2006 Christian Unger <coder at breakbe D0T at>
 * Translated by John Frushour
 * 
 * This library is covered by the LGPL, read LICENSE for details.
 */

// TODO: Is there a generic way to get rid of <strstream>?
//   stringbuf's don't seem to do the same job as strstreambuf's (e.g. writing to a designated area of memory...)

#ifdef WINDOWS
# include <strstream>
#else
# include <strstream>
#endif

class object_stream 
{
  streambuf *pbuf;
  streambuf &buf;
  bool __load;
  
public:
  object_stream ( void* mem, unsigned long size, ios::openmode mode )
    : pbuf(
#ifdef WINDOWS
           // Wow, this works platform-independently...
           new strstreambuf((char*)mem, size, (mode & ios::out) ? (char*)mem : (char*)0 )
#else
           // new stringbuf(string((char*)mem,size),mode)
           new strstreambuf((char*)mem, size, (mode & ios::out) ? (char*)mem : (char*)0 )
#endif
           ), buf(*pbuf)
  {
#ifdef WINDOWS
#else
    // ((stringbuf*)pbuf)->pubsetbuf((char*)mem,size);
#endif
    
    if( mode & ios::out )
      __load = false;
    
    if( mode & ios::in )
      __load = true;
    
    // cout << "object_stream " << __load << endl;
  }
  
  object_stream ( streambuf &buf, ios::openmode mode )
    : pbuf(NULL), buf(buf)
  {
    if( mode & ios::out )
      __load = false;
    
    if( mode & ios::in )
      __load = true;
  }
  
  ~object_stream () 
  {
    buf.pubsync();
    
    if( pbuf != NULL ) delete pbuf;
  }
  
  void handle ( char &c ) 
  {
    if( load() ) {
      c = buf.sgetc();
    }else{
      buf.sputc( c );
    }
  }
  
  void handle ( void *p, const unsigned long n ) 
  {
    if( load() ) {
      // cout << "b " <<
      buf.sgetn( (char*)p, n );
    }else{
      // cout << "b " <<
      buf.sputn( (char*)p, n );
    }
  }
  
  void put ( const char *p, const unsigned long n ) 
  {
    buf.sputn( p, n );
  }
  
  void get ( char *p, const unsigned long n ) 
  {
    buf.sgetn( p, n );
  }
  
  bool load () const 
  {
    return __load;
  }
  
  streambuf & get_streambuf() 
  {
    return buf;
  }
  
  // const streambuf & streambuf() const 
  // {
  //   return buf;
  // }
};

#ifndef _WIN32
object_stream& operator << ( unsigned long long int &v, object_stream &os ) 
{
  os.handle( &v, sizeof(v) );
  return os;
}

object_stream& operator << ( signed long long int &v, object_stream &os ) 
{
  os.handle( &v, sizeof(v) );
  return os;
}
#endif

object_stream& operator << ( unsigned short int &v, object_stream &os ) 
{
  os.handle( &v, sizeof(v) );
  return os;
}

object_stream& operator << ( signed short int &v, object_stream &os ) 
{
  os.handle( &v, sizeof(v) );
  return os;
}

object_stream& operator << ( unsigned long int &v, object_stream &os ) 
{
  os.handle( &v, sizeof(v) );
  return os;
}

object_stream& operator << ( signed long int &v, object_stream &os ) 
{
  os.handle( &v, sizeof(v) );
  return os;
}

object_stream& operator << ( unsigned int &v, object_stream &os ) 
{
  os.handle( &v, sizeof(v) );
  return os;
}

object_stream& operator << ( signed int &v, object_stream &os ) 
{
  os.handle( &v, sizeof(v) );
  return os;
}

object_stream& operator << ( float &v, object_stream &os ) 
{
  os.handle( &v, sizeof(v) );
  return os;
}

object_stream& operator << ( double &v, object_stream &os ) 
{
  os.handle( &v, sizeof(v) );
  return os;
}

object_stream& operator << ( long double &v, object_stream &os ) 
{
  os.handle( &v, sizeof(v) );
  return os;
}

object_stream& operator << ( unsigned char &v, object_stream &os ) 
{
  os.handle( &v, sizeof(v) );
  return os;
}

object_stream& operator << ( signed char &v, object_stream &os ) 
{
  os.handle( &v, sizeof(v) );
  return os;
}


object_stream& operator << ( string &v, object_stream &os ) 
{
  // FIXME: pre-allocate internal string-memory
  // cout << "load string" << endl;
  unsigned long n = v.length();
  n << os;
  // cout << "  " << n << endl;
  
#ifdef WINDOWS
  char *tmp = new char[n+1];
#else
  char tmp[n+1];
#endif
  if( os.load() ) {
    os.get( &tmp[0], n );
    tmp[n] = '\0';
    // cout << "loaded string #" << tmp << "#" << endl;
    v = string( &tmp[0], n );
  }else{
    // cout << "wrote string #" << v.c_str() << "#" << endl;
    os.put( v.data(), n );
  }
#ifdef WINDOWS
  delete tmp;
#endif
  return os;
}

/*
template <class T>
object_stream& operator << ( vector<T> &vec, object_stream &os ) 
{
  // FIXME: pre-allocate internal vector-memory
  unsigned long n = vec.size();
  n << os;
  
  if( os.load() ) {
    // FIXME: use this? (then we don't need a default constructor for T
    // char *tmp = (char*) malloc( sizeof(T) );
    T v;
    
    for( int i = 0; i < n; ++i ) {
      operator<<( v, os );
      vec.push_back( v );
    }
    
    // free( tmp );
  }else{
    typename vector<T>::iterator it = vec.begin(), end = vec.end();
    
    for( ; it != end; ++it ) {
      *it << os;
      // os.put( &*it, sizeof(T) );
    }
  }
  
  return os;
}
*/



