// -*- c++ -*- $Id: usbio.h 33 2006-12-17 12:38:33Z matt $

/* USB-related routines
 * 
 * Copyright (C) 2006 Christian Unger <coder at breakbe D0T at>
 * 
 * This library is covered by the LGPL, read LICENSE for details.
 */


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


#ifndef WINDOWS
# include <unistd.h>
# include <sys/ioctl.h>
#endif

#include <sys/stat.h>
#include <fcntl.h>


#ifndef WINDOWS
# include <usb.h>
#endif


#ifdef WINDOWS
# undef PINT

namespace win32 {
  
# define NOMINMAX
# include <windows.h>
# include <setupapi.h>

  typedef struct _HIDD_ATTRIBUTES {
    ULONG  Size;
    USHORT  VendorID;
    USHORT  ProductID;
    USHORT  VersionNumber;
  } HIDD_ATTRIBUTES, *PHIDD_ATTRIBUTES;
  
  // FIXME: this is just a dirty hack
  typedef int usb_device;
  typedef int usb_dev_handle;
  
}; // namespace win32

using namespace win32;

#endif


#define VID         0x0c70
#define PID         0xf0b0

#define TIMEOUT     1000
    
#define SET_REQUEST 0x21
// USB_ENDPOINT_OUT + USB_TYPE_CLASS + USB_RECIP_INTERFACE = 0x00 + (0x01 << 5) + 1 = 0x21
    
#define GET_REQUEST 0xA1
// USB_ENDPOINT_IN  + USB_TYPE_CLASS + USB_RECIP_INTERFACE = 0x80 + (0x01 << 5) + 1 = 0xA1


#define SET_REPORT  0x09
// REPORT_SET = 0x09
    
#define GET_REPORT  0x01
// REPORT_GET = 0x01

    
#define OUTPUT_REPORT_TYPE  (0x02 << 8)
// ReportID + (RT_OUTPUT << 8) = 0 + (0x02 << 8)
    
#define INPUT_REPORT_TYPE   (0x01 << 8)
// ReportID + (RT_INPUT << 8)  = 0 + (0x01 << 8)
    
#define INDEX 0


#define INPUT_REPORT_LEN  552
#define OUTPUT_REPORT_LEN 376



/** Communication-class that provides a direct access to an aquaero-instance via USB (libusb).
 */
class USBCommunication
  : public AquaeroCommunication
{
  bool dirtyNoClose;
  bool leaveOpen;
  bool isOpen;

#ifdef WINDOWS
  HINSTANCE hidDll;
  
  HANDLE DeviceHandle;
  HANDLE WriteHandle;
  HANDLE ReadHandle;
  
  HANDLE hDevInfo;
  
  VOID    (_stdcall *HidD_GetHidGuid)    (OUT LPGUID HidGuid);
  BOOLEAN (_stdcall *HidD_GetAttributes) (IN HANDLE  HidDeviceObject,
                                 OUT PHIDD_ATTRIBUTES  Attributes);
  BOOLEAN (_stdcall *HidD_GetInputReport) (IN HANDLE  HidDeviceObject,
                                           IN OUT  PVOID  ReportBuffer,
                                           IN ULONG  ReportBufferLength);
  BOOLEAN (_stdcall *HidD_SetOutputReport) (HANDLE,
                                            PVOID,
                                            ULONG);
  
  ULONG (_stdcall *HidD_Hello) (PCHAR Buffer,
                       ULONG BufferLength);
#endif
  usb_dev_handle *open_handle;
  
public:
  USBCommunication ( ) 
  {
    dirtyNoClose = false;
    leaveOpen = false;
    isOpen = false;
#ifdef WINDOWS
    hidDll = LoadLibrary("hid.dll");
    if( hidDll == NULL ) {
      if(VERBOSE) cout << "hid-dll not found" << endl;
      throw "hid.dll not found or loadable!";
    }else{
      if(VERBOSE) cout << "extracting entry-points" << endl;
      HidD_GetHidGuid = (VOID (_stdcall *)(OUT LPGUID))GetProcAddress(hidDll,"HidD_GetHidGuid");
      HidD_GetAttributes = (BOOLEAN (_stdcall *) (HANDLE,PHIDD_ATTRIBUTES))GetProcAddress(hidDll,"HidD_GetAttributes");
      HidD_GetInputReport = (BOOLEAN (_stdcall *) (IN HANDLE,IN OUT PVOID,IN ULONG))GetProcAddress(hidDll,"HidD_GetInputReport");
      HidD_SetOutputReport = (BOOLEAN (_stdcall *) (HANDLE,PVOID,ULONG))GetProcAddress(hidDll,"HidD_SetOutputReport");
      HidD_Hello = (ULONG (_stdcall *) (OUT PCHAR,IN ULONG))GetProcAddress(hidDll,"HidD_Hello");
      
      if(VERBOSE) {
        char tmp[10];
        tmp[9] = '\0';
        cout << "testing hidhello" << endl;
        HidD_Hello((PCHAR)tmp,10);
        cout << tmp << endl;
      }
    }
    DeviceHandle = NULL;
    WriteHandle = NULL;
    hDevInfo = NULL;
    
#endif
  }

  virtual ~USBCommunication () 
  {
    close_device();
#ifdef WINDOWS
    FreeLibrary(hidDll);
#endif
  }
  
private:
  void close_device () 
  {
    if( isOpen ) {
#ifdef WINDOWS
      CloseHandle(ReadHandle);
      CloseHandle(WriteHandle);
      CloseHandle(DeviceHandle);
#else
      usb_close(open_handle);
#endif
    }
    isOpen = false;
  }

  void debug_buffer_hex ( char *buffer, int len ) 
  {
    int i;
  
    printf( "hex-dump of recv'd buffer:\n" );
    printf( "---------------------------\n" );
  
    for( i = 0; i < len; ) {
      unsigned char c = (unsigned char)buffer[i];
      printf( "%02X ", (int)c );
      ++i;
    
      if( (i % 20) == 0 ) printf( "\n" );
    }
    printf( "\n" "---------------------------" "\n" );
  }


  void debug_buffer ( char *buffer, int len ) 
  {
    int i;
    char tmp[2];
    tmp[0] = ' ';
    tmp[1] = '\0';
  
    printf( "buffer:>>>" );
    for( i = 0; i < len; ++i ) {
      char c = buffer[i];
    
      if( c >= ' ' && c <= 'z' ) {
        tmp[0] = c;
      }else{
        tmp[0] = '*';
      }
    
      printf( "%s", (char*)tmp );
    }
    printf( "<<<" "\n" );
  }


  void prep_buffer ( char *buffer ) 
  {
    int i;
    for( i = 0; i < INPUT_REPORT_LEN+1; ++i ) {
      buffer[i] = (char)0x6c;
    }
    buffer[0] = buffer[1] = buffer[2] = buffer[3] = 0;
  }


  void handle_buffer ( char *buffer ) 
  {
    debug_buffer_hex( buffer, INPUT_REPORT_LEN+1 );
    debug_buffer( buffer, INPUT_REPORT_LEN+1 );
    
#ifndef _WIN32
    usleep( 20000 );
#endif
  }

  
  void kernel_detach ( usb_dev_handle *aq )
  {
#ifndef WINDOWS
# ifdef LIBUSB_HAS_DETACH_KERNEL_DRIVER_NP
    usb_detach_kernel_driver_np(aq, 0);
# else
    cout << "WARNING: libusb has no detach_kernel_driver! Trying to continue..." << endl;
# endif
#endif
  }
  
  
  /** returns the aquaero-usb_device
   */
#ifndef WINDOWS
  struct 
#endif
  usb_device *init_device()
  {
#ifndef WINDOWS
    struct usb_bus *bus;
    struct usb_device *dev;
  
    usb_init();
    usb_find_busses();
    usb_find_devices();
  
    for (bus = usb_get_busses(); bus; bus = bus->next) 
      {
        for (dev = bus->devices; dev; dev = dev->next)
          {
            if ((dev->descriptor.idVendor  == VID) &&
                (dev->descriptor.idProduct == PID))
              return dev;
          }
      }
    return NULL;
#else
    if( DeviceHandle != NULL )
      return (usb_device*)1;
    
    // this source-code is copy'n'pasted from sources found at www.lvr.com
    GUID HidGuid;

    HidD_GetHidGuid(&HidGuid);
    
    hDevInfo=SetupDiGetClassDevs 
      (&HidGuid, 
       NULL, 
       NULL, 
       DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);
    if(VERBOSE) cout << "2" << endl;
    
    SP_DEVICE_INTERFACE_DATA devInfoData;
    devInfoData.cbSize = sizeof(devInfoData);
    int MemberIndex = 0;
    bool LastDevice = FALSE;
    ULONG Length;
    ULONG Required;
    HIDD_ATTRIBUTES Attributes;
    bool MyDeviceDetected = false;
    
    do
      {
        LONG Result=SetupDiEnumDeviceInterfaces 
          (hDevInfo, 
           0, 
           &HidGuid, 
           MemberIndex, 
           &devInfoData);
        if(VERBOSE) cout << "3" << endl;
        
        if (Result != 0)
          {
            Result = SetupDiGetDeviceInterfaceDetail 
              (hDevInfo, 
               &devInfoData, 
               NULL, 
               0, 
               &Length, 
               NULL);
            if(VERBOSE) cout << "4" << endl;
            
            PSP_DEVICE_INTERFACE_DETAIL_DATA detailData
              = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(Length);
            
            //Set cbSize in the detailData structure.

            detailData -> cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
            
            //Call the function again, this time passing it the returned buffer size.
            
            Result = SetupDiGetDeviceInterfaceDetail 
              (hDevInfo, 
               &devInfoData, 
               detailData, 
               Length, 
               &Required, 
               NULL);
            if(VERBOSE) cout << "5" << endl;
            DeviceHandle=CreateFile 
              (detailData->DevicePath, 
               0, 
               FILE_SHARE_READ|FILE_SHARE_WRITE, 
               (LPSECURITY_ATTRIBUTES)NULL,
               OPEN_EXISTING, 
               0, 
               NULL);
            if(VERBOSE) cout  << "6" << endl;
            
            //DisplayLastError("CreateFile: ");
            Attributes.Size = sizeof(Attributes);
            
            Result = HidD_GetAttributes 
              (DeviceHandle, 
               &Attributes);
            if(VERBOSE) cout << "7" << endl;
            
            //DisplayLastError("HidD_GetAttributes: ");
            
            //Is it the desired device?
            
            MyDeviceDetected = FALSE;
            
            if (Attributes.VendorID == VID)
              {
                if (Attributes.ProductID == PID)
                  {
                    //MyDevicePathName = detailData->DevicePath;
                    // Get a handle for writing Output reports.
                    MyDeviceDetected = TRUE;
                    
                    if(VERBOSE) cout << "found the device!!" << Attributes.VersionNumber  << endl;
                    
                    WriteHandle=CreateFile 
                      (detailData->DevicePath, 
                       GENERIC_WRITE, 
                       FILE_SHARE_READ|FILE_SHARE_WRITE, 
                       (LPSECURITY_ATTRIBUTES)NULL,
                       OPEN_EXISTING, 
                       0, 
                       NULL);
                    if(VERBOSE) cout << "8" << endl;
                    ReadHandle=CreateFile 
                      (detailData->DevicePath, 
                       GENERIC_READ, 
                       FILE_SHARE_READ|FILE_SHARE_WRITE, 
                       (LPSECURITY_ATTRIBUTES)NULL,
                       OPEN_EXISTING, 
                       0, 
                       NULL);
                    
                    // ?? PrepareForOverlappedTransfer();
                  } //if (Attributes.ProductID == ProductID)
                
                else
                  //The Product ID doesn't match.
                  
                  CloseHandle(DeviceHandle);
              }
            else
              CloseHandle(DeviceHandle);
            
            free(detailData);
          }  //if (Result != 0)
        else
          //SetupDiEnumDeviceInterfaces returned 0, so there are no more devices to check.
          
          LastDevice=TRUE;
        //If we haven't found the device yet, and haven't tried every available device,
        //try the next one.
        
        MemberIndex = MemberIndex + 1;
        
      } //do
    
    while ((LastDevice == FALSE) && (MyDeviceDetected == FALSE));

    return (usb_device*)MyDeviceDetected;
#endif
  }
  
  /** returns a "ready-for-action" aquaero-device-handle 
   */
  usb_dev_handle * get_aquaero_handle () 
  {
    if( isOpen ) {
#ifndef WINDOWS
      // FIXME: Linux' USB-Code is a mess... In order to get proper data, we have to close and open every time...
      close_device();
      isOpen = false;
#else
      return this->open_handle;
#endif
    }

#ifndef WINDOWS
    struct 
#endif
      usb_device *dev = NULL;
#ifndef WINDOWS
    struct 
#endif
      usb_dev_handle *aq = NULL;
    int arg;
    
    dev = init_device();
    
    if (!dev)
      {
#ifndef WINDOWS
#ifdef USE_COLORS
        cout << color(lt_red);
#endif
        
        cout << " !! Is your USB-Module loaded?" << endl
             << " !! Check the output of 'lsusb' for 'MCT Elektronikladen'.."
          
#ifdef USE_COLORS
             << resetcolors
#endif
          
             << endl;
#endif
        throw "Unable to find device. \n";
      }
    
#ifndef WINDOWS
    aq = usb_open(dev);
    
    if (!aq)
      {
        throw "Unable to open aquaero. \n";
      }
    
    kernel_detach(aq);
    
    arg = usb_set_configuration(aq, 1);
    if ( arg < 0 )
      {
        
#ifdef USE_COLORS
        cout << color(lt_red);
#endif
        
        cout << " !! Do you have the permission to write to aquaero?" << endl
             << " !! What about 'chgrp -R users /proc/bus/usb/'?!"
          
#ifdef USE_COLORS
             << resetcolors
#endif
          
             << endl 
             << "WARNING: Unable to set config. Trying to continue..." << endl
             << endl;
      }
    if(VERBOSE>=2) cout << "INFO: did set config" << endl;
    
    {
      if(VERBOSE>=2)
        cout << "INFO: trying to claim interface..." << endl;
      
      int retries = 3;
      arg = usb_claim_interface(aq, 0);
      if(VERBOSE) cout << "INFO: first try = " << arg << endl;
      while (retries-- > 0 && (arg = usb_claim_interface(aq, 0)) < 0 ) {
        cout << "INFO: retrying(claim).. " << (10 - retries) << endl;
        kernel_detach(aq);
      }
      if (arg < 0) {
        cout << "ERROR: unable to claim interface... " << arg << endl;
        usb_close(aq);
        throw "unable to claim interface";
      }
    }
    
    // === this will crash on 2.6 ===
    // {
    //   int retries = 3;
    //   while (retries-- > 0 && (arg = usb_set_altinterface(aq, 0) < 0)) {
    //     printf( "retrying.. %d\n", 10 - retries );
    //     usb_detach_kernel_driver_np(aq, 0);
    //   }
    //   if (arg < 0) {
    //     printf( "ERROR: set_alt failed... %d\n", arg );
    //     usb_close(aq);
    //     throw "ERROR: unable to set_alt";
    //   }
    // }
#endif    

    isOpen = true;
    open_handle = aq;

    return aq;
  }
  
  
  /** reads raw-data from aquaero
   */
  string readAEString () 
  {
    usb_dev_handle *aq = get_aquaero_handle();
    char *buffer = (char*)malloc(INPUT_REPORT_LEN+10);
    int arg;
    
    prep_buffer(buffer);
#ifndef WINDOWS
    {
      arg = usb_interrupt_read( aq,
                                0x81, // endpoint
                                buffer,
                                INPUT_REPORT_LEN+1,
                                TIMEOUT );
    }
    if(VERBOSE) cout << "INFO: retval " << arg << endl;
    if( arg < 0 ) {
      cout << "ERROR: unable to read: " << arg << endl;
      if( !leaveOpen ) {
        close_device();
      }
      throw "unable to read";
    }
#else
    {
      DWORD NumberOfBytesRead;
      
      LONG Result = ReadFile 
        (ReadHandle, 
         buffer, 
         INPUT_REPORT_LEN+1, 
         &NumberOfBytesRead,
         NULL);
      
      if( Result == 0 ) {
        cout << "ERROR: unable to read" << endl;
        if( !leaveOpen ) {
          close_device();
        }
        throw "unable to read";
      }
    }
#endif
    if(VERBOSE >= 2) handle_buffer(buffer);
    
    if( !leaveOpen ) {
      if(VERBOSE) cout << "INFO: Closing aquaero..." << endl;
#ifndef WINDOWS
      // NOTE: this lags unter 2.6
      if(!dirtyNoClose) usb_close(aq);
      //cout << "WARNING: omitting close" << endl;
#else
      close_device();
#endif
      if(VERBOSE) cout << "INFO: done." << endl;
      isOpen = false;
    }
    
#ifndef WINDOWS
    string ret = string(buffer,INPUT_REPORT_LEN+1);
#else
    string ret = string(buffer+1,INPUT_REPORT_LEN+1);
#endif
    delete buffer;
    
    return ret;
  }
  
  /** sends raw-data to aquaero
   */
  void writeAEString ( string data )
  {
    usb_dev_handle *aq = get_aquaero_handle();  
    int arg;
  
    if( data.length() != OUTPUT_REPORT_LEN ) {
      cerr << "FATAL ERROR: The generated Output-Report has the wrong length!" << endl;
      close_device();
      throw "Wrong Report-Length";
    }
    
#ifndef WINDOWS
    {
      const char *buffer = data.c_str();
      arg = usb_control_msg(aq,
                            SET_REQUEST,                /* Request Type */ 
                            SET_REPORT,                 /* request */
                            OUTPUT_REPORT_TYPE,         /* value */
                            INDEX,                      /* index */
                            (char*)buffer,              /* bytes */
                            OUTPUT_REPORT_LEN,          /* length */
                            TIMEOUT);                   /* timeout */
    
    }
    if(VERBOSE) cout << "INFO: retval " << arg << endl;
    if( arg < 0 ) {
      cout << "ERROR: unable to send the Output-Report" << arg << endl;
      if( !leaveOpen ) {
        close_device();
      }
      throw "unable to send the Output-Report";
    }
#else
    {
      // throw "not yet implemented!";
      if( WriteHandle == INVALID_HANDLE_VALUE ) {
        cout << "ERROR: invalid handle!" << endl;
        cout.flush();
      }

      char *buffer = new char[data.length()+1];
      buffer[0] = 0;
      memcpy( buffer+1, data.c_str(), data.length() );
      /*
      LONG Result = HidD_SetOutputReport
			  (WriteHandle,
			   (void*)buffer,
			   OUTPUT_REPORT_LEN);
      //*/
      
      //*
      ULONG BytesWritten;
      LONG Result = WriteFile 
			  (WriteHandle, 
			  (void*)buffer, 
			  OUTPUT_REPORT_LEN+1, 
			  &BytesWritten, 
			  NULL);
      //*/

      if( !Result ) {
        cout << "ERROR: unable to send the Output-Report" << Result << endl;
        cout << "last error " << GetLastError() << endl;
        cout.flush();
        if( !leaveOpen ) {
          close_device();
        }
        throw "unable to send the Output-Report";
      }
      delete buffer;
    }
#endif

    if( !leaveOpen ) {
      if(VERBOSE) cout << "INFO: Closing aquaero..." << endl;
#ifndef WINDOWS
      // NOTE: this lags unter 2.6
      if(!dirtyNoClose) usb_close(aq);
#else
      close_device();
#endif
      if(VERBOSE) cout << "INFO: done." << endl;
    }
  }

public:
  
  /** reads from aquaero and fills the structure with data
   */
  void read ( AquaeroData_rev4 &aedata )
  {
    if(VERBOSE) cout << "reading from aquaero..." << endl;
    string aeStringData;
    try {
      aeStringData = readAEString();
    } catch( const char * sss ) {
      cout << "FATAL ERROR: " << sss << endl;
      cout << "trying to continue..." << endl;
      // cout << "aborting..." << endl;
      // throw new DeviceError(sss);
      return;
    }
    
    if(VERBOSE) cout << "done." << endl;
    if(VERBOSE) cout << "extracting data..." << endl;
    try{
      aedata.CreateStructureFromString(aeStringData);
    } catch( const char * sss ) {
      cout << "FATAL ERROR: " << sss << endl;
      cout << "aborting..." << endl;
      throw new DeviceError(sss);
    }
    if(VERBOSE) cout << "done." << endl;
  }
  
  /** writes the given settings to aquaero 
   */
  void write ( AquaeroData_rev4 &aedata ) 
  {
    writeAEString( aedata.CreateStringFromStructure() );
  }
  
  void setDirtyNoClose ( bool v ) 
  {
    dirtyNoClose = v;
  }

  void setLeaveOpen ( bool v ) 
  {
    leaveOpen = v;
  }
};


// windows.h  seems to define a Macro called `ERROR', which collides with our enum
#undef ERROR
