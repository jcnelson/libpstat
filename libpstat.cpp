/*
   libpstat: a library for getting information about running processes
   Copyright (C) 2014  Jude Nelson

   This program is dual-licensed: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License version 3 or later as 
   published by the Free Software Foundation. For the terms of this 
   license, see LICENSE.LGPLv3+ or <http://www.gnu.org/licenses/>.

   You are free to use this program under the terms of the GNU Lesser General
   Public License, but WITHOUT ANY WARRANTY; without even the implied 
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU Lesser General Public License for more details.

   Alternatively, you are free to use this program under the terms of the 
   Internet Software Consortium License, but WITHOUT ANY WARRANTY; without
   even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   For the terms of this license, see LICENSE.ISC or 
   <http://www.isc.org/downloads/software-support-policy/isc-license/>.
*/

#include "libpstat.h"

#ifdef _LINUX
#include "os/linux.h"
#endif

#ifdef _UNKNOWN
#error "Undefined OS target"
#endif

// insert more OSs here

// get the status of a running processes
// return 0 on success 
// return negative on error
int pstat( pid_t pid, struct pstat* ps, int flags ) {
   return pstat_os( pid, ps, flags );
}


#ifdef _USE_SHA256

// get the SHA256 of a running process
int pstat_sha256( struct pstat* ps ) {
   
   int fd = 0;
   int rc = 0;
   
   if( ps->deleted ) {
      return -ENOENT;
   }
   if( !ps->running ) {
      return -ECHILD;
   }
   
   fd = open( ps->path, O_RDONLY );
   if( fd < 0 ) {
      
      rc = -errno;
      return rc;
   }
   
   rc = pstat_os_sha256_fd( fd, ps->sha256 );
   
   close( fd );
   return rc;
}

#else 

// no-op 
int pstat_sha256( struct pstat* ps ) {
   return -ENOSYS;
}

#endif