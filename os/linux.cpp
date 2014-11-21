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

#include "linux.h"

// Linux-specific implementation 
#ifdef _LINUX

// stat a running process on linux
// return 0 on success, which is qualified to mean "We could fill in at least one field, besides the PID"
// return -errno on error (due to open(), readlink(), or fstat())
int pstat_os( pid_t pid, struct pstat* ps, int flags ) {
   
   int rc = 0;
   
   char proc_path[PATH_MAX+1];
   char bin_path[PATH_MAX+1];
   int fd = 0;
   ssize_t nr = 0;
   static size_t deleted_len = strlen( " (deleted)" );
   
   memset( proc_path, 0, PATH_MAX+1 );
   memset( bin_path, 0, PATH_MAX+1 );
   
   ps->pid = pid;
   
   sprintf( proc_path, "/proc/%d/exe", pid );
   
   // open the process binary, if we can 
   fd = open( proc_path, O_RDONLY );
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
   
   // verify that it isn't deleted
   nr = readlink( proc_path, bin_path, PATH_MAX );
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
   
   if( rc < 0 ) {
      
      rc = -errno;
      close( fd );
      return rc;
   }
   
   ps->deleted = false;
   strcpy( ps->path, bin_path );
   
   if( flags & PSTAT_HASH ) {
      rc = pstat_os_sha256_fd( fd, ps->sha256 );
   }
   
   close( fd );
   
   return 0;
}

#endif  // #define _LINUX
