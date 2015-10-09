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

#include "linux.h"

// Linux-specific implementation 
#ifdef _LINUX

uint64_t pstat_os_supported_features() {
    return LIBPSTAT_RUNNING | LIBPSTAT_BINARY | LIBPSTAT_STARTTIME;
}

static ssize_t read_uninterrupted( int fd, char* buf, size_t len ) {
   
   ssize_t num_read = 0;
   while( (unsigned)num_read < len ) {
      
      ssize_t nr = read( fd, buf + num_read, len - num_read );
      if( nr < 0 ) {
         
         int errsv = -errno;
         if( errsv == -EINTR ) {
            continue;
         }
         
         return errsv;
      }
      if( nr == 0 ) {
         break;
      }
      
      num_read += nr;
   }
   
   return num_read;
}

// stat a running process on linux
// return 0 on success, which is qualified to mean "We could fill in at least one field, besides the PID"
// return -errno on error (due to open(), readlink(), or fstat())
// return -EIO in particular if we could not parse /proc/[PID]/stat
int pstat_os( pid_t pid, struct pstat* ps, int flags ) {
   
   int rc = 0;
   
   char path[PATH_MAX+1];
   char bin_path[PATH_MAX+1];
   char stat_buf[ 8192 ];
   
   int fd = 0;
   ssize_t nr = 0;
   size_t deleted_len = strlen( " (deleted)" );
   
   // fields in /proc/[PID]/stat.  We want the starttime (which is the 22nd field)
   char* stat_ptr = NULL;
   int num_paren = 0;
   bool paren_found = false;
   int field_count = 0;
   uint64_t starttime = 0;
   
   memset( path, 0, PATH_MAX+1 );
   memset( bin_path, 0, PATH_MAX+1 );
   memset( stat_buf, 0, 8192 );
   
   memset( ps, 0, sizeof(struct pstat) );
   
   ps->pid = pid;
   
   sprintf( path, "/proc/%d/exe", pid );
   
   // open the process binary, if we can 
   fd = open( path, O_RDONLY );
   if( fd < 0 ) {
      
      rc = -errno;
      
      // not running?
      if( rc == -ENOENT ) {
         ps->running = false;
         return 0;
      }
      
      // something else
      return rc;
   }
   
   // the fact that this path exists means that it's running
   ps->running = true;
   
   // verify that it isn't deleted
   nr = readlink( path, bin_path, PATH_MAX );
   if( nr < 0 ) {
   
      rc = -errno;
      close( fd );
      return rc;
   }
   
   // on Linux, if the bin_path ends in " (deleted)", we're guaranteed that this binary no longer exists.
   if( strlen( bin_path ) > strlen( " (deleted)") && strcmp( bin_path + strlen(bin_path) - deleted_len, " (deleted)" ) == 0 ) {
      
      ps->deleted = true;
      close( fd );
      return 0;
   }
   
   rc = fstat( fd, &ps->bin_stat );
   close( fd );
   
   if( rc < 0 ) {
      
      rc = -errno;
      return rc;
   }
   
   ps->deleted = false;
   strncpy( ps->path, bin_path, PATH_MAX );
   
   // on Linux, we can see when the process started.  Fill this in from /proc/[PID]/stat
   sprintf( path, "/proc/%d/stat", pid );
   
   fd = open( path, O_RDONLY );
   if( fd < 0 ) {
       
      rc = -errno;
      
      // not running?
      if( rc == -ENOENT ) {
         ps->running = false;
         return 0;
      }
      
      return rc;
   }
   
   rc = read_uninterrupted( fd, stat_buf, 8191 );
   if( rc < 0 ) {
       
      return rc;
   }
   
   close( fd );
   rc = 0;
   
   // read ahead on stat_buf to the end of the second field (the binary path, in parentheses)
   stat_ptr = stat_buf;
   while( *stat_ptr != '\0' ) {
       
       if( *stat_ptr == '(' ) {
           num_paren++;
           paren_found = true;
       }
       else if( *stat_ptr == ')' ) {
           num_paren--;
       }
       
       if( num_paren == 0 && paren_found ) {
           // end of the second field 
           stat_ptr++;
           break;
       }
       
       stat_ptr++;
   }
   
   // read ahead to the 22nd field 
   field_count = 3;
   while( field_count < 22 ) {
       while( *stat_ptr == ' ' ) {
           stat_ptr++;
       }
       
       if( *stat_ptr == '\0' ) {
           // not found 
           rc = -EIO;
           break;
       }
       
       field_count++;
       while( *stat_ptr != ' ' ) {
           stat_ptr++;
       }
       
       if( *stat_ptr == '\0' ) {
           // not found 
           rc = -EIO;
           break;
       }
   }
   while( *stat_ptr == ' ' ) {
        stat_ptr++;
    }
   
   starttime = (uint64_t)strtoull( stat_ptr, NULL, 10 );
   if( starttime == 0 ) {
       
       // failed to parse 
       rc = -EIO;
       return rc;
   }
   
   ps->starttime = starttime;
   
   return rc;
}

#endif  // #define _LINUX
