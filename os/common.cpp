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

#include "common.h"


#ifdef _USE_SHA256

// get the sha256 of the data referred to by file descriptor.
// new_checksum must be at least SHA256_DIGEST_LENGTH bytes long
// read to EOF 
int pstat_os_sha256_fd( int fd, unsigned char* new_checksum ) {
   
   SHA256_CTX context;
   unsigned char buf[32768];
   ssize_t num_read = 1;
   int rc = 0;
   
   SHA256_Init( &context );
   
   while( num_read > 0 ) {
      
      num_read = read( fd, buf, 32768 );
      
      if( num_read < 0 ) {
         
         rc = -errno;
         
         SHA256_Final( new_checksum, &context );
         
         return rc;
      }
      
      SHA256_Update( &context, buf, num_read );
   }
   
   SHA256_Final( new_checksum, &context );
   
   return 0;
}

#else 

// no-op
int pstat_os_sha256_fd( int fd, unsigned char* new_checksum ) {
   
   return -ENOSYS;
}

#endif