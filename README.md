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

The library defines the following process status structure:

   
    struct pstat {

      // PID of the process 
      pid_t pid;

      // Whether or not the process is running.
      // If false, then the fields beneath this will not be initialized.
      bool running;
      

      // Path to the process binary, and a flag indicating
      // whether or not it still exists.  If it was deleted,
      // the path will not be initialized, and deleted will be true.
      char path[PATH_MAX+1];
      bool deleted;
   
      // stat structure for the process binary, if it exists.
      struct stat bin_stat;
   
      // SHA256 hash of the process binary, if it exists.
      // Optionally calculated; see below.
      unsigned char sha256[SHA256_DIGEST_LENGTH];
    }


The library defines the following constants

    // Flag to pass to pstat() to have it calculate the SHA256 
    // of the process binary.
    #define PSTAT_HASH          0x1
   

The following methods are exported:

    int pstat( pid_t pid, struct pstat* ps, int flags )
  
Get the status of the process, given its PID.  Fill in the ps structure.
On success, return 0, and fill in at least one field of ps, as documented above.
On failure, return a negative errno value.  The exact value is implementation-specific.

NOTE: This method succeeds even if some of ps cannot be filled in.  The running and deleted fields indicate whether or not their corresponding fields could be initialized.
