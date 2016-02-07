libpstat: OS-agnostic process stat
==================================

libpstat is a small library for getting information on running processes in an OS-agnostic way.  The source is structured to make it easy to add support for additional operating systems.  Currently, Linux is the only supported OS.

Building
--------
To build:

    $ make OS=$OS_NAME
  
Substitute $OS_NAME for "LINUX".

Installing
----------

To install libpstat to /lib and headers to /usr/include/pstat:

    $ sudo make install PREFIX=/ INCLUDE_PREFIX=/usr
  
**Advanced:** If you want to install libpstat's headers to a custom location, you can do so by setting the `INCLUDE_DIR` variable instead of the `INCLUDE_PREFIX` variable.  For example, passing `INCLUDE_DIR=/custom/location/` would install the headers directly to `/custom/location`, whereas `INCLUDE_PREFIX=/custom/location` would install the headers to `/custom/location/include/pstat`.

Documentation
-------------

TODO
