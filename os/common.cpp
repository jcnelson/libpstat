/*
   libpstat: a library for getting information about running processes
   Copyright (C) 2014  Jude Nelson

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "common.h"

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

