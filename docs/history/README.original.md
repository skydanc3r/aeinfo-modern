Aquaero-info README ENGLISH VERSION
===================

Aquaero-info is (c) 2006, 2007 by
   Christian Unger <coder@breakbe.at>
   Translation by John Frushour <egawrangler@yahoo.com>
And distributed under the terms of the Lesser GNU General Public License (LGPL).
See the file. / COPYING in the source distribution for more information.

Revision: $ Id: README 36 2007-12-13 10:38:02 Z $ mat

-------------------------------------------------- ------------------------------

Aquaero-info together with a small library
of Control software for the device

   Aqua-computer aquaero 4.00

For more information, visit

   Http://breakbe.at/development/aquaero/

Brief Introduction to the Linux-control software "aquaero-info"
================================================== ===========
Version: 2.4

Since version 2.0, the Aquaero Web Daemon is included, aquaerod.
This is a background daemon for the Aquaero web interface.  It publishes
all Aquaero information to a TCP port for remote administration.

Requirements (Linux)
---------------------------

> Libusb-0.1.10a AND libusb-devel
         Runs seamlessly with earlier versions.
> Linux
         A "current" linux-Kernel (2.4.26 and 2.6.12).
> Libpthread
         Is practically on every GNU / Linux system


1st Installation (Linux)
-----------------------

As any user
       Cd aeinfo- <VERSION>
       Make

And as a super-OPTIONAL

       Make install

Execute (which copies the executable aeinfo to / usr / local / bin).
Note that on some systems this must be done manually, as their "make install" script
  will not (natively) handle the install procedure defined in this package
  see the makefile script included in this package for details on where to copy things

1.1 chroot-Environment
----------------------

First, with the ldd command, identify all necessary libraries.
In chroot-Environment, modify the following with all needed access:
All data in the subdirectory htdocs / + the executable aquaerod + libraries needed.
Also make sure the aquaerod is executable.


2nd Compatibility
----------------

So far, no incompatibility issues have been found.  However, note that on Debian distros,
  the daemon needs to be run LAST if using a startup script.  This is because of a startup
  glitch on the more current Debian releases (Ubuntu 7.10, for instance).

3rd Execution
-------------

The console program can also, as in version 1.0, work without using the Daemon.
. However, access to the hardware is much faster with the daemon running in the background.

3.1 console program
---------------------

Check first whether the aquaero-info was installed properly., type this from the console:

       Aeinfo

If no error occurs, then aquaero-info was 
Successfully installed. The most common mistakes is that
the kernel module is not properly detecting the Aquaero usb device. Type:

       Lsusb

to check if the Aquaero was found. Examine whether the aquaero 
"MCT electronics shop" is listed.  If it is not found, you may need
to add the line:

none                 /proc/bus/usb        usbfs      defaults              0 0

to /etc/fstab.  This will automatically mount the usb filesystem when booting.  This
allows the Aquaero to be accessed by the OS.  Note that it may already be possible to see the
Aquaero hub in your usb device list, but it probably cannot be accessed until the
above line is entered.

If "MCT Elektroniks shop" is listed, then make sure the aeinfo-user
(or group) has the proper rights.
For example:

       Chmod a + w / proc / bus / usb /

Or

       Chgrp users / proc / bus / usb /

For more information on the program, type

       Aeinfo help

(The controller is similar to subversion-tool 'svn')
For example, if you want to display the fan names...

       Aeinfo info fan.1.name fan.2.name fan.3.name fan.4.name

. All information will be available through

       Aeinfo info

. In order to modify data, you must use the set command
So for example:

       Aeinfo set fan.1.name "Fan 1" fan.2.name "Fan 2120" fan.3.name foo fan.4.name test

In order to set the time you need:

       Aeinfo set


3.2 Daemon
----------

As with the console `aeinfo ', make sure aquaerod has the appropriate rights
For the directories under "/ proc / bus / usb /", see 3.1

During installation you must make sure that the directory containing the HTML files is
directly beneath the installation directory of the aquaerod binary. So, for example, if the
executable file aquaerod is installed in:

         / Opt / aquaerod / aquaerod

Then you have this directory as the web-root:

         / Opt / aquaerod / htdocs /

Otherwise you will receive an Error 404 ;-)
The web interface can be reached via the port 30000, for example at the URL:

         Http://localhost:30000/

The daemon starts by using
        
         Cd / opt / aquaerod /
         . / Aquaerod

Again, you must make sure to start the aquaerod from it's native working directory.
Aquaerod has a dependency on the HTML docs in this directory.  If aquaerod is not run
from its working directory... something "ugly" will happen, Christian is working on a resolution..

3.2.1 Including the daemon in the starting process of Linux
-------------------------------------------------- ------

For SuSE Richard Kahn has developed a startup script, edited by John Frushour.
Information can be found in the file z001_aquaerod_suse.

For Debian distros, please refer to the z001_aquaerod_debian script, created by John Frushour.

For other distributions, please refer to the documentation, because the
setups here are extremely diverse.

3.2.2 command-line arguments of the Daemon
----------------------------------------

    - V Verbose-Ausgabe
    -- No-webserver Starting the daemon without the webserver

4th Problems
-----------

4.1 Linux and USB
-----------------

Linux - 2.6 proved to be very "sensitive". If the aquaero is not found
, One must, if necessary, re-initialize the USB-Subsystem. So
For example, remove ALL usb devices and re-insert them:

       Rmmod ohci_hcd
       Rmmod usbhid
       Modprobe ohci_hcd

(You may have to call the first module ehci_hcd [this works with ohci_hcd
!] Or uhci_hcd).
You should also check whether the aquaero was ever found after the refresh:

       Lsusb

If in the output lines it says:

       Bus 002 Device 003: ID 0c70: f0b3 MCT electronics shop
       Bus 002 Device 004: ID 0c70: f0b0 MCT electronics shop

"MCT electronics shop", then the aquaero was found!
A brief pause will be encountered while waiting for aeinfo to reconnect to the Aquaero,
if anyone has an idea on why this occurrs in kernel 2.6 and above, then please email Christian.

4.2 daemon for Linux
----------------------

For immediate stopping and starting of the daemon, you will sometimes
receive a message that the port is still open, and so the second launch of the Web 
daemon does not work

And thus the daemon is not started. In this case, just wait a moment and try again ...

  - Christian Unger aka 0x6c333337
  - John Frushour egawrangler@yahoo.com
