// -*- c++ -*- $Id: ipc.h 35 2007-05-05 15:09:55Z matt $

/* IPC-Specific classes
 * 
 * Copyright (C) 2006 Christian Unger <coder at breakbe D0T at>
 * Translated by John Frushour
 * 
 * This library is covered by the LGPL, read LICENSE for details.
 */

#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
using namespace std;

#ifndef WINDOWS
# include <pthread.h>
# include <sys/ipc.h>
# include <sys/sem.h>
# include <sys/shm.h>

# include <errno.h>
#endif


#ifdef __TEST
long device_data = 13;

class aquaero_rev4 
{
public:
  long data;
  
  object_stream & operator << ( object_stream &os ) 
  {
    data << os;
  }
};
#endif


/*
Brainstorming zum aquaero Client-Server-Model:
	Speicherung der aquaero_rev4-DS im SharedMemory. direkt mit new dort anlegen! �berladen?!


AquaeroCommunication
	SharedMemoryCommunication.read( that ):
		lock data-mutex
		copy memory from shm to "that"
		unlock data-mutex

	SharedMemoryCommunication.write( that ):
	    Client:
		lock write-mutex
		lock data-mutex
		check conflictIDs from "that" and the one in shm (exit?)
		copy memory from "that" to shm
		unlock data-mutex
		unlock "perform USB-Device-Update" semaphore  (0 -> 1)
		lock "wait for Daemon" semaphore (1 -> 0)
		unlock write-mutex
	    Server:
		Daemon wakes up and finishes lock on "perform USB-Device-Update" (1)
		update of aquaero via USB
		unlock "wait for Daemon" (0 -> 1)
		Daemon falls asleep via lock on "perform USB-Device-Update" (1 -> 0)


ID-based write-write conflict-resolution
	problems with intermediate data-changes:
		r1 w1 r2 w2
		r1 r2 w2 w1 X
		r1 r2 w1 w2 X


Daemon-Side of USB-Interaction:
	read from Device:
		lock usb-comm-mutex
		read from USB
		unlock usb-comm-mutex
		lock data-mutex
		update SHM-structure
		set the conflictID
		unlock data-mutex
	
	write to Device:
		// reading is possible
		lock usb-comm-mutex
		write to USB
		unlock usb-comm-mutex


Problem:
 - client->server: write to USB
 - server: read from USB (overwrites shm-data)
 - server writes old shm-data to USB

procs:
 - client
    - shm->ram
    - ram->shm, update
 - server.writer
    - shm->svc->usb
 - server.reader
    - usb->svc->shm


another problem:
 - client-writes: we should use another memory region for this!
   new process:
     - client writes proposed data to "the write region" and notifies the daemon
     - daemon reads the data into temp-structure, does checks and updates date/time
     - daemon writes the data to the device, updates "the read region" and notifies the client
     

**/



#define _IPC_BASE ""
//"AEInfo_IPC_aquacomputer_aquaero_"



class IPCException
{
public:
  IPCException () 
  {
    if(VERBOSE) cout << "ERROR: now issuing IPCException" << endl;
  }
};

class LockFailedException
{
public:
  LockFailedException ()
  {
    if(VERBOSE) cout << "ERROR: now issuing LockFailedException" << endl;
  }
};


class Semaphore
{
  bool _destroy;
#ifdef WINDOWS
  HANDLE h;
#else
  int sem;
#endif

  void debug_semop_error ( int errornum )
  {
    
  }
  
public:
  Semaphore ( )
    : _destroy(false)
  {
#ifdef WINDOWS
    h = 0;
#else
    sem = -1;
#endif
  }
  
  Semaphore ( string name, long key, int value, int max )
    : _destroy(true)
  {
#ifdef WINDOWS
    SECURITY_ATTRIBUTES satt;
    satt.nLength = sizeof(satt);
    satt.lpSecurityDescriptor = NULL;
    satt.bInheritHandle = FALSE;
    
    h = CreateSemaphore(&satt,
                        value,
                        max,
                        name.c_str());
    // h = CreateMutex( NULL, FALSE, name.c_str() );
    
    if( h == NULL ) {
      cerr << "ERROR creating semaphore" << endl;
    }
#else
    sem = semget( key, 1, IPC_CREAT | 0666 );
    if(VERBOSE) {
      if( sem == -1 ) {
        cerr << "ERROR creating sem" << endl;
      }else cout << "done" << endl;
    }
    
    union semun {
      int val;
      struct semid_ds *buf;
      unsigned short  *array;
    } arg;
    arg.val = value;
    semctl( sem, 0, SETVAL, arg );
#endif
    unlock(); // FIXME: should be done at every creation! do it for LInux also!
    if(VERBOSE) cout << "created semaphore " << name << endl;
  }
  
  Semaphore ( string name, long key )
    : _destroy(false)
  {
#ifdef WINDOWS
    h = OpenSemaphore(SEMAPHORE_MODIFY_STATE, // 0x0002
                      FALSE,
                      name.c_str());
    // h = OpenMutex( MUTEX_ALL_ACCESS, FALSE, name.c_str() );
    
    if( h == NULL ) {
      if(VERBOSE) {
        cerr << "ERROR getting semaphore..." << endl;
      }
      throw new IPCException();
    }else{
      if(VERBOSE) cout << "got semaphore " << name << endl;
    }
#else
    sem = semget( key, 1, /*TODO: IPC_ACCESS*/0 );
    {
      if( sem == -1 ) {
        if(VERBOSE) cerr << "ERROR getting semaphore..." << endl;
        throw new IPCException();
      }else{
        if(VERBOSE) cout << "got semaphore " << name << endl;
      }
    }
#endif
  }
  
  void lock () 
  {
#ifdef WINDOWS
    DWORD dwWaitResult = WaitForSingleObject( h, INFINITE );
    switch( dwWaitResult ) {
    case WAIT_OBJECT_0:
      // if(VERBOSE) cout << "waited" << endl;
      break;
      
    case WAIT_TIMEOUT:
      if(VERBOSE) cout << "Sem::timeout" << endl;
      break;
      
    case WAIT_ABANDONED:
      if(VERBOSE) cout << "Sem::abandon" << endl;
      break;
    }
#else
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = -1;
    buf.sem_flg = 0;
    int s = semop( sem, &buf, 1 );
    int errsv = errno;
    if( s == -1 ){
      if(VERBOSE) cout << "ERROR: semop failed " << errno << " " << strerror(errsv) << endl;
      throw new LockFailedException();
    }
#endif
  }
  
  void unlock () 
  {
#ifdef WINDOWS
    ReleaseSemaphore( h, 1, NULL );
    // ReleaseMutex( h );
#else
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = 0;
    int s = semop( sem, &buf, 1 );
    int errsv = errno;
    if( s == -1 ) {
      if(VERBOSE) cout << "ERROR: semop failed " << errno << " " << strerror(errsv) << endl;
    }
#endif
  }
  
  void destroy () 
  {
    if( _destroy ) {
#ifdef WINDOWS
      // TODO: win32 Semaphore::destroy
#else
      semctl( sem, 0, IPC_RMID );      
#endif
    }
  }
  
  ~Semaphore()
  {
    // TODO: sem = Semaphore(..) will not work with this:
    if( _destroy ) {
#ifdef WINDOWS
      // TODO: always CloseHandle??
      // CloseHandle(h);
#else
      // semctl( sem, 0, IPC_RMID );
#endif
    }
  }
};



class synchronize
{
  Semaphore &sem;
  
public:
  synchronize ( Semaphore &sem )
    : sem(sem)
  {
    // if(VERBOSE) cout << "before lock" << endl;
    sem.lock();
    // if(VERBOSE) cout << "  lock done" << endl;
  }
  
  ~synchronize () 
  {
    sem.unlock();
  }
};


class SharedMemory
{
#ifdef WINDOWS
  HANDLE hMapFile;
#else
  int memid;
#endif
  void* pBuf;
  unsigned long __size;
  bool destroyable;
  
public:
  SharedMemory ( )
  {
#ifdef WINDOWS
    hMapFile = 0;
#else
    memid = -1;
#endif
    pBuf = NULL;
    __size = 0;
  }
  
  SharedMemory ( string name, long key, unsigned long size )
    : __size(size), destroyable(true)
  {
#ifdef WINDOWS
    hMapFile = CreateFileMapping
      ( INVALID_HANDLE_VALUE,   // use paging file
        NULL,                   // default security 
        PAGE_READWRITE,         // read/write access
        0,                      // max. object size 
        size,                   // buffer size  
        name.c_str() );         // name of mapping object
    
    if( hMapFile == NULL || hMapFile == INVALID_HANDLE_VALUE ) {
      cerr << "Could not create file mapping object " << GetLastError() << endl;
      // TODO throw ...
      return;
    }
#else
    memid = shmget( key, size, IPC_CREAT | 0666 );
    if( memid == -1 ) {
      cerr << "Could not create shm" << endl;
      return;
    }
#endif
    
#ifdef WINDOWS
    pBuf = (LPTSTR) MapViewOfFile
      ( hMapFile,               // handle to map object
        FILE_MAP_ALL_ACCESS,    // read/write permission
        0,
        0,
        size );
    
    if( pBuf == NULL ) {
      if(VERBOSE) cerr << "Could not map memory into process " << GetLastError() << endl;
      // TODO throw ...
      return;
    }
#else
    pBuf = shmat( memid, NULL, 0 );
    if( -1 == (long)pBuf ) {
      if(VERBOSE) cerr << "Could not map memory into process " << endl;
      return;
    }
#endif
    
    if(VERBOSE) cout << "created and mapped shm" << endl;
    // CopyMemory((PVOID)pBuf, szMsg, strlen(szMsg));
  }
  
  SharedMemory ( string name, long key, unsigned long size, bool )
    : __size(size), destroyable(false)
  {
#ifdef WINDOWS
    hMapFile = OpenFileMapping
      ( FILE_MAP_ALL_ACCESS,    // read/write access
        FALSE,                  // do not inherit the name
        name.c_str() );         // name of mapping object
    
    if (hMapFile == NULL) {
      if(VERBOSE) cerr << "Could not open file mapping object " << GetLastError() << endl;
      return;
    }
#else
    memid = shmget( key, size, /* TODO: IPC_ACCESS*/0 );
    if( memid == -1 ) {
      if(VERBOSE) cerr << "unable to get shm" << endl;
      return;
    }
#endif
    
#ifdef WINDOWS
    pBuf = (LPTSTR) MapViewOfFile
      ( hMapFile,               // handle to map object
        FILE_MAP_ALL_ACCESS,    // read/write permission
        0,
        0,
        size );
    
    if (pBuf == NULL) {
      if(VERBOSE) cerr << "Could not map memory into process " << GetLastError() << endl; 
      return;
    }
#else
    pBuf = shmat( memid, NULL, 0 );
    if( -1 == (long)pBuf ) {
      if(VERBOSE) cerr << "Could not map memory into process" << endl;
      return;
    }
#endif
    
    if(VERBOSE) {
      cout << "got and mapped shm" << endl;
      cout << "first long is: " << *((long*)getPointer()) << endl;
    }
  }
  
  void* getPointer () 
  {
    return pBuf;
  }
  
  unsigned long size() const 
  {
    return __size;
  }
  
  void destroy () 
  {
#ifdef WINDOWS
    // TODO: SharedMemory::destroy
#else
    if( destroyable ) {
      int ret = shmctl( memid, IPC_RMID, NULL );
      if( ret == -1 ) cout << "SHM-RMID failed" << endl;
    }
    int ret = shmdt( pBuf );
    if( ret == -1 ) cout << "SHMDT failed" << endl;
#endif
  }
  
  // stringbuf get_stringbuf () 
  // {
  //   // TODO!
  //   return stringbuf( string((char*)pBuf,__size) );
  // }

  void flush () 
  {
#ifdef WINDOWS
    if( !FlushViewOfFile( getPointer(), size() ) ) { 
      if(VERBOSE) cout << "ERROR AT FLUSHING!!!" << endl;
    } 
    if(VERBOSE) cout << "first long is now: " << *((long*)getPointer()) << endl;
#else
    // TODO: flush shared memory for linux!?
#endif
  }
  
  ~SharedMemory () 
  {
    // TODO: move to destroy method!
    if( pBuf != NULL ) {
#ifdef WINDOWS
      // UnmapViewOfFile(pBuf);
      // CloseHandle(hMapFile);
#else
      // shmdt( pBuf );
      // shmctl( memid, IPC_RMID, pBuf );
#endif
    }
  }
};



//void * Thread__run_impl(void *);


class Thread 
{
  string name;
#ifdef WINDOWS
  HANDLE h;
#else
  pthread_t h;
  int tid;
#endif
  
public:
  Thread ()
    : name("UNNAMED")
  {
  }
  
  Thread ( string name )
    : name(name)
  {
  }
  virtual ~Thread () 
  {
#ifdef WINDOWS
    CloseHandle(h);
#else
#endif
  }
  
  string getName () 
  {
    return name;
  }
  
  void start () 
  {
#ifdef WINDOWS
    h = CreateThread(NULL, 0, Thread::__run_impl, this, 0, NULL);
    if( h == NULL ) {if(VERBOSE) cout << "error with create thread" << endl;}
    else {if(VERBOSE) cout << "CreateThread success " << (long)h << endl;}
#else
    if(VERBOSE) cout << "before pthread_create" << getName() << endl;
    tid = pthread_create (&h, NULL, /*(void *()(void *))*/Thread::__run_impl/*Thread::__run_impl*/, this);
    if( tid == 0 ) {
      if(VERBOSE) cout << "pthread_create done." << endl;
    }else{
      if(VERBOSE) cout << "pthread_create ERROR! " << tid << endl;
      if(VERBOSE) cout << "  " << strerror(tid) << endl;
    }
    
    int s = pthread_detach(h);
    if( s == 0 ) {
      if(VERBOSE) cout << "pthread_detach done." << endl;
    }else{
      if(VERBOSE) cout << "pthread_detach ERROR! " << s << endl;
      if(VERBOSE) cout << "  " << strerror(s) << endl;
    }
    
#endif
  }
  
  static void sleep ( unsigned long ms ) 
  {
#ifdef WINDOWS
    Sleep(ms);
#else
    usleep(1000*ms);
#endif
  }
  
  //static DWORD __run_impl ( void *__arg )
#ifdef WINDOWS
  static DWORD WINAPI __run_impl( LPVOID __arg )
#else
  static void * __run_impl (void * __arg)
#endif
  {
    Thread *t = (Thread*)__arg;
    if(VERBOSE) cout << "starting thread " << t->name << endl;
    try {
      t->run();
      if(VERBOSE) cout << "finished thread " << t->name << endl;
      return 0;
    } catch( ... ) {
      if(VERBOSE) cout << "aborted thread " << t->name << endl;
      return 0;
    }
  }
  
  virtual void run () = 0;
};

// #ifndef WINDOWS
// void * Thread__run_impl(void * __arg)
// {
//   cout << "test" << endl;
//   Thread *t = (Thread*)__arg;
//   cout << "starting thread " << t->getName() << endl;
//   t->run();
//   cout << "finished thread " << t->getName() << endl;
//   return 0;
// }
// #endif




#if 0

class RunTimeLibrary
{
  string name;
#ifdef WINDOWS
  HINSTANCE h;
#else
  void *h;
#endif
  
public:
  RunTimeLibrary ( string name ) 
  {
    // dlopen, dlsym, and dlclose
  }
  
  bool load ( ) 
  {
#ifdef WINDOWS
    h = LoadLibrary(name.c_str());
    return h == NULL ? false : true;
#else
    h = dlopen( name.c_str(), RTLD_LAZY );
    return h == NULL ? false : true;
#endif
  }
  
  void* getSymbol ( string name ) 
  {
#ifdef WINDOWS
    return GetProcAddress(h,name.c_str());
#else
    return dlsym(h, name.c_str() );
#endif
  }
  
  bool unload () 
  {
#ifdef WINDOWS
    BOOL r = FreeLibrary( h );
    h = 0;
    
    return r ? true : false;
#else
    int r = dlclose(h);
    h = NULL;
    
    return r == 0 ? true : false;
#endif
  }
  
  ~RunTimeLibrary () 
  {
    if( h != NULL ) {
      unload();
    }
  }
};

#endif







class DaemonNotFoundException
{
public:
  DaemonNotFoundException () 
  {
    if(VERBOSE) cout << "ERROR: now issuing DaemonNotFoundException" << endl;
  }
};





class DaemonCommunication
#ifndef __TEST
  : public AquaeroCommunication
#endif
{
protected:
  unsigned long conflictID;
  bool server;
  
  SharedMemory shm;
  SharedMemory shm_write;
  
  Semaphore data_mtx;
  Semaphore write_mtx;
  Semaphore usb_comm_mtx;
  Semaphore performUSBDeviceUpdate;
  Semaphore waitForDaemon;
  
  DaemonCommunication ( bool server )
    : server(server)
  {
    conflictID = 13;

    if( server ) {
      data_mtx = Semaphore( _IPC_BASE "data_mtx", 0x13444500+0x01, 0, 1 );
      write_mtx = Semaphore( _IPC_BASE "write_mtx", 0x13444500+0x02, 0, 1 );
      usb_comm_mtx = Semaphore( _IPC_BASE "usb_comm_mtx", 0x13444500+0x03, 0, 1 );
      
      performUSBDeviceUpdate = Semaphore( _IPC_BASE "performUSBDeviceUpdate", 0x13444500+0x04, 0, 1 );
      waitForDaemon = Semaphore( _IPC_BASE "waitForDaemon", 0x13444500+0x05, 0, 1 );
      
      shm = SharedMemory( _IPC_BASE "shm", 0x13444500+0x06, 4096 );
      shm_write = SharedMemory( _IPC_BASE "shm_write", 0x13444500+0x07, 4096 );
    }else{
      try {
        data_mtx = Semaphore( _IPC_BASE "data_mtx", 0x13444500+0x01 );
        write_mtx = Semaphore( _IPC_BASE "write_mtx", 0x13444500+0x02 );
        usb_comm_mtx = Semaphore( _IPC_BASE "usb_comm_mtx", 0x13444500+0x03 );
      
        performUSBDeviceUpdate = Semaphore( _IPC_BASE "performUSBDeviceUpdate", 0x13444500+0x04 );
        waitForDaemon = Semaphore( _IPC_BASE "waitForDaemon", 0x13444500+0x05 );
      
        shm = SharedMemory( _IPC_BASE "shm", 0x13444500+0x06, 4096, false );
        shm_write = SharedMemory( _IPC_BASE "shm_write", 0x13444500+0x07, 4096, false );
      } catch( const IPCException *ipcex ) {
        throw new DaemonNotFoundException();
      }
    }
  }

  virtual ~DaemonCommunication () 
  {
  }
  
  void destroy () 
  {
    if( server ) {
      data_mtx.destroy();
      write_mtx.destroy();
      usb_comm_mtx.destroy();
      performUSBDeviceUpdate.destroy();
      waitForDaemon.destroy();
      shm.destroy();
      shm_write.destroy();
    }
  }
  
  void * readRegion () 
  {
    return shm.getPointer();
  }
  
  void * writeRegion () 
  {
    // return (void*)(((char*)shm.getPointer())+100);
    return shm_write.getPointer();
  }
  
public:
  Semaphore &getDataMutex()
  {
    return data_mtx;
  }
  Semaphore &getWriteMutex() 
  {
    return write_mtx;
  }
};

typedef AquaeroData_rev4 aquaero_rev4;

class DaemonCommunicationClient
  : public DaemonCommunication
{
public:
  DaemonCommunicationClient ()
    : DaemonCommunication(false)
  {
  }
  virtual ~DaemonCommunicationClient ()
  {
  }
  
  
  void read ( aquaero_rev4 &aedata ) 
  {
    // CALLED FROM CLIENTS
    {
      synchronize syn(data_mtx);
      
      // copy from shm:
      {
        if(VERBOSE) cout << "shm memory is " << (long)readRegion() << endl;
        object_stream os( /*shm.get_stringbuf()*/readRegion(), shm.size(), ios::in );
        conflictID << os;
        if(VERBOSE) cout << "read conflict id: " << conflictID << endl;
        long versionID=0, reserved=0;
        versionID << os;
        reserved << os;
        reserved << os;
        reserved << os;
        reserved << os;
        aedata << os;
      }
      
      // data_mtx.unlock();
    }
  }
  
  void force_write ( aquaero_rev4 &aedata ) 
  {
    conflictID = 0x7FFFFFFF;
    write( aedata );
  }
  
  void write ( aquaero_rev4 &aedata ) 
  {
    // CALLED FROM CLIENTS
    {
      synchronize syn(write_mtx);
      if(VERBOSE) cout << "sync: write_mtx" << endl;
      
      {
        synchronize syn(data_mtx);
        if(VERBOSE) cout << "sync: data_mtx" << endl;
        
        // check conflict IDs
        {
          unsigned long checkConflictID;
          
          object_stream os(/*shm.get_stringbuf()*/readRegion(), shm.size(), ios::in);
          checkConflictID << os;
          
          if( checkConflictID != conflictID ) {
            cerr << "Write-Write conflict!!" << endl;
            return;
          }
        }
        if(VERBOSE) cout << "checked conflictIDs" << endl;
        
        // copy to shm
        {
          object_stream os(/*shm.get_stringbuf()*/writeRegion(), shm_write.size(), ios::out);
          conflictID << os;
          long versionID = 0x0200, reserved=0;
          versionID << os;
          reserved << os;
          reserved << os;
          reserved << os;
          reserved << os;
          aedata << os;
        }
        if(VERBOSE) cout << "copied to shm" << endl;
        
        // data_mtx.unlock();
      }
      
      performUSBDeviceUpdate.unlock();
      
      waitForDaemon.lock();
      
      // write_mtx.unlock();
    }
  }
};

class DaemonCommunicationServer
  : public DaemonCommunication
{
  USBCommunication comm;

public:
  DaemonCommunicationServer ()
    : DaemonCommunication(true)
  {
    comm.setLeaveOpen(true);
    
    // FIXME: this is a game with fire!
    performUSBDeviceUpdate.lock();
  }
  
  virtual ~DaemonCommunicationServer ()
  {
  }
  
  void destroy () 
  {
    this->DaemonCommunication::destroy();
  }
  
  void read ( aquaero_rev4 &aedata ) 
  {
    if(VERBOSE) cout << "now in read" << endl;
    {
      synchronize syn(write_mtx);   // a client write must exclude a server-side read!

      // XXX: first read to tmp, then compare with aedata, aedata = tmp, write to shm
      aquaero_rev4 tmp;
      {
        synchronize syn(usb_comm_mtx);
        
        // read from USB into temporary structure
        {
#ifdef __TEST
          cout << "reading from device: " << device_data << endl;
          tmp.data = device_data;
          Sleep(2000);
          cout << "done reading: " << tmp.data << endl;
#else
          if(VERBOSE) cout << "before USB-access" << endl;
          comm.read( tmp );
#endif
        }
      }
      
      {
        synchronize syn(data_mtx);
        
        // TODO: do not use ints here
        if( aedata.serverSideWriteFlag == 0/*NONE*/
            || aedata.serverSideWriteFlag == 2/*UPDATE_PENDING*/ ) {
          if(VERBOSE) cout << "UPDATING AEDATA NOW!" << endl;
          aedata = tmp;
          
          // XXX: compare temporary structure with current aquaero-data (for conflictID)
          //  -> update conflict ID
          // memcmp is not sufficient - due to temps, fan-speeds, time etc.
          // if( memcmp(&aedata, &tmp, sizeof(aquaero_rev4)) != 0 ) {
          //   cout << "change in data, changing conflict id" << endl;
          //   ++conflictID;
          //   memcpy(&aedata, &tmp, sizeof(aquaero_rev4));
          // }
          
          // update shm...
          {
            object_stream os(readRegion(), shm.size(), ios::out);
            conflictID << os;
            if(VERBOSE) cout << "wrote conflict id: " << conflictID << endl;
            long versionID = 0x0200, reserved=0;
            versionID << os;
            reserved << os;
            reserved << os;
            reserved << os;
            reserved << os;
            aedata << os;
          }
          shm.flush();
          
          aedata.serverSideWriteFlag = (AquaeroData_rev4::EServerSideWriteFlag)0/*NONE*/;
          if(VERBOSE) cout << "DONE WITH UPDATE OF AEDATA" << endl;
        }else{
          if(VERBOSE) cout << "OMITTING update of aedata, since a write was requested!" << endl;
        }
      }
    }
  }
  
  void write ( aquaero_rev4 &aedata ) 
  {
    if(VERBOSE) cout << "now in write" << endl;
    {
      if(VERBOSE) cout << "before UPDATE LOCK" << endl;
      performUSBDeviceUpdate.lock();
      if(VERBOSE) cout << "after UPDATE LOCK" << endl;

      // update aedata from shm!
      {
        synchronize syn(data_mtx);
        
        object_stream os(writeRegion(), shm_write.size(), ios::in);
        
        unsigned long theConflictID = 0;
        theConflictID << os;
        if( theConflictID < conflictID ) {
          if(VERBOSE) cout << "ERROR: TODO: write-write conflict!" << endl;
        }
        long versionID = 0x0200, reserved=0;
        versionID << os;
        reserved << os;
        reserved << os;
        reserved << os;
        reserved << os;
        
        aedata << os;
        aedata.serverSideWriteFlag = (AquaeroData_rev4::EServerSideWriteFlag)1/*RUNNING*/;
      }
      
      // FIXME: change the conflict-ID if and only if the data has changed...
      //   For now, it is sufficient to assume that data changed.
      //   Note that this is the securest approach, but may lead to "unwanted"
      //   behaviour due to too much rollbacks when used in a
      //   *multi*-user environment. 
      ++conflictID;
      
      // update shm...
      {
        synchronize syn(data_mtx);
        
        aedata.synchronizeClock();
        
        object_stream os( readRegion(), shm.size(), ios::out );
        conflictID << os;
        long versionID = 0x0200, reserved=0;
        versionID << os;
        reserved << os;
        reserved << os;
        reserved << os;
        reserved << os;
        aedata << os;
      }
      
      // update aquaero via USB...
      {
        synchronize syn(usb_comm_mtx);
        comm.write( aedata );
      }
      
      {
        synchronize syn(data_mtx);
        aedata.serverSideWriteFlag = (AquaeroData_rev4::EServerSideWriteFlag)2/*UPDATE_PENDING*/;
      }
      
      waitForDaemon.unlock();
    }
  }
  
  /** Assumes, that data_mtx (and write_mtx??) has been locked outside!
   */
  void serverSideWrite ( aquaero_rev4 &aedata ) 
  {
    {
      synchronize syn(usb_comm_mtx);
      comm.write( aedata );
    }
  }
};


#if 0

class Configuration 
{
  string filename;
  map<string, string> cfg;
  
  void load ( ) 
  {
    ifstream ifs(filename, ios:in);
    // TODO: weiter!!
  }
  
public:
  Configuration ( string filename )
    : filename(filename);
  {
    load();
  }
  
  string get ( string key ) 
  {
    return cfg[key];
  }
};

#endif

class Configuration 
{
public:
  Configuration()
  {
  }
};

class AquaeroDaemonConfiguration
  : Configuration
{
public:
};




class AquaeroDaemonService 
{
public:
  AquaeroDaemonService () 
  {
  }
  
  virtual void init ( AquaeroDaemonConfiguration * config, aquaero_rev4 * aedata ) = 0;
  virtual void start () = 0;
  virtual void update ( bool changed ) = 0;
  virtual void stop () = 0;
  virtual void destroy () = 0;
};



class AquaeroDaemon 
{
  bool exitAll;
  int readInterval;
  DaemonCommunicationServer commServer;
  aquaero_rev4 aedata;
  
  class ReaderThread : public Thread
  {
    bool &exitAll;
    aquaero_rev4 &aedata;
    DaemonCommunicationServer &commServer;
    int &readInterval;
    
  public:
    ReaderThread ( bool &exitAll, aquaero_rev4 &aedata, DaemonCommunicationServer &commServer, int &readInterval )
      : Thread("Reader"), exitAll(exitAll), aedata(aedata), commServer(commServer), 
        readInterval(readInterval)
    {
    }
    virtual ~ReaderThread () 
    {
    }
    
    void run () 
    {
      try {
        while( !this->exitAll ) {
          if(VERBOSE) cout << "new loop of read" << endl;
          
          this->commServer.read( this->aedata );
          
          sleep( readInterval );
          
          if(VERBOSE) cout << "did sleep" << endl;
          
        }
      } catch( const LockFailedException *e ) {
        if(VERBOSE) cout << "Leaving ReaderThread::run due to failed semaphore lock" << endl;
      } catch( const DeviceError *e ) {
        cerr << "Error with device: " << e->getMessage() << endl;
        cerr << "aborting..." << endl;
#ifdef WINDOWS
        string msg = e->getMessage();
        msg += "\nThe daemon will abort now.";
        MessageBox(NULL,msg.c_str(),"aeinfo-daemon",MB_OK|MB_ICONERROR);
#endif
        exit(1);
      }
    }
  } reader;
  
  class WriterThread : public Thread 
  {
    bool &exitAll;
    aquaero_rev4 &aedata;
    DaemonCommunicationServer &commServer;
    
  public:
    WriterThread ( bool &exitAll, aquaero_rev4 &aedata, DaemonCommunicationServer &commServer )
      : Thread("Writer"), exitAll(exitAll), aedata(aedata), commServer(commServer)
    {
    }
    virtual ~WriterThread () 
    {
    }
    
    void run () 
    {
      if(VERBOSE) cout << "entering WriterThread::run" << endl;
      try {
        while( !this->exitAll ) {
          if(VERBOSE) cout << "new loop of write" << endl;
          
          this->commServer.write( this->aedata );
        }
      } catch( const LockFailedException *e ) {
        if(VERBOSE) cout << "Leaving WriterThread::run due to failed semaphore lock" << endl;
      } catch( const DeviceError *e ) {
        cerr << "Error with device: " << e->getMessage() << endl;
        cerr << "aborting..." << endl;
#ifdef WINDOWS
        string msg = e->getMessage();
        msg += "\nThe daemon will abort now.";
        MessageBox(NULL,msg.c_str(),"aeinfo-daemon",MB_OK|MB_ICONERROR);
#endif
        exit(1);
      }
    }
  } writer;
  
  
public:
  AquaeroDaemon ()
    : exitAll(false), readInterval(500), commServer(), 
      reader(exitAll,aedata,commServer,readInterval), 
      writer(exitAll,aedata,commServer)
  {
  }
  
  void destroy ()
  {
    exitAll = true;
    commServer.destroy();
  }
  
  aquaero_rev4 & getAquaeroData () 
  {
    return aedata;
  }
  
  Semaphore & getDataMutex () 
  {
    return commServer.getDataMutex();
  }
  DaemonCommunicationServer & getCommunication () 
  {
    return commServer;
  }
  Semaphore & getWriteMutex ()
  {
    return commServer.getWriteMutex();
  }
  
  void run () 
  {
    // read configuration...
    
    // start reader- and writer-thread
    if(VERBOSE) cout << "now in daemon::run" << endl;
    
    reader.start();
    // delayed startup - there should be some time for the reader to start up
    {
      Thread::sleep( readInterval*2+1000 );
      writer.start();
    }
    
    // start services:
    //  - httpd-Service
    //  - logging-Services
    
    // endless-loop: wait for HUP, TERM or BREAK signal...
    //   man 2 sigprocmask
    
    //   GetMsgProc
    //     MSG msg;
    //     while( GetMessage( &msg, NULL, 0, 0 ) ) {
    //       TranslateMessage( &msg );
    //       DispatchMessage( &msg );
    //     }
  }
};



AquaeroCommunication * getAquaeroCommunicationInstance () 
{
  AquaeroCommunication *ret = NULL;
  try {
    ret = new DaemonCommunicationClient();
    if(VERBOSE) cout << "Connecting to aquaero via DAEMON" << endl;
  } catch( const DaemonNotFoundException *dnfex ) {
  } catch( ... ) {
  }
  if( ret == NULL ) {
    ret = new USBCommunication();
    if(VERBOSE) cout << "Connecting to aquaero via USB" << endl;
  }
  return ret;
}


