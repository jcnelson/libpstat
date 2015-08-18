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

To install libpstat to /usr/local/lib and headers to /usr/local/include/pstat:

    $ sudo make install
  

Documentation
-------------

TODO
